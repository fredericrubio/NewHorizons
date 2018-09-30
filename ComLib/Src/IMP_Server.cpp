//
//  IMP_Server.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 05/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <mutex>

#include "IMP_Server.hpp"
#include "NHO_LOG.hpp"
#include "IMP_Message.hpp"
#include "IMP_ImageSizeMessageBody.hpp"
#include "IMP_AckMessageBody.hpp"
#include "IMP_Image.hpp"


#include <sys/ioctl.h>
#include <netinet/tcp.h>

#define _DEBUG

IMP_Server::IMP_Server(const unsigned int pInfoPort, const unsigned int pDataPort):
infoPort(pInfoPort), dataPort(pDataPort),  infoConnexionSocket(0), dataConnexionSocket(0),
dataClientSocket(0) {

    period = 2000;
    
    image = new IMP_Image();
 
#ifdef _RASPBIAN
    camera.open();
    
    // wait  for initialization 
    std::this_thread::sleep_for (std::chrono::seconds(4));
#endif
}

bool IMP_Server::initiate() {
#ifdef _DEBUG
    std::cout << "IMP_Server::initiate \n";
#endif
    struct sockaddr_in lInfoServAddr;
    // service channel
    infoConnexionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (infoConnexionSocket < 0) {
        fprintf(stderr, "ERROR opening socket");
        return(false);
    }
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    lInfoServAddr.sin_addr.s_addr = INADDR_ANY;
    lInfoServAddr.sin_port = htons(infoPort);
    if (bind(infoConnexionSocket, (struct sockaddr *) &lInfoServAddr, sizeof(lInfoServAddr)) < 0) {
        std::cout << "IMP_Server: ERROR on binding " << std::endl;
        return(false);
    }

    // data channel
    struct sockaddr_in serv_addr;
    dataConnexionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (dataConnexionSocket < 0) {
        fprintf(stderr, "ERROR opening socket");
        return(false);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(dataPort);
    if (bind(dataConnexionSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "IMP_Server: ERROR on binding " << std::endl;
        return(false);
    }
    
    // launch the connexion thread
    /// service channel
    serviceConnectionThread = new std::thread(&IMP_Server::waitForConnectionOnServiceSocket, std::ref(*this));
    /// data channel
    dataConnectionThread = new std::thread(&IMP_Server::waitForConnectionOnDataSocket, std::ref(*this));
#ifdef _RASPBIAN
    imageCaptureThread = new std::thread(&IMP_Server::captureImage, std::ref(*this));
#endif
    
#ifdef _DEBUG
    std::cout << "IMP_Server::initiate End\n";
#endif
    
    return(true);
    
}

bool IMP_Server::terminate() {
#ifdef _DEBUG
     std::cout << "IMP_Server::terminate \n";
#endif
    // close all client connections
    close(infoClientSocket);
    
    // close main socket
    close(infoConnexionSocket);
    
#ifdef _DEBUG
    std::cout << "IMP_Server::terminate End\n";
#endif
    return(true);
    
}
#include <time.h> 
/////////////////////////////////
// Method called by the thread capturging the images.
// Only one image is available: the latest captured.
void IMP_Server::captureImage() {
#ifdef _RASPBIAN
    unsigned char*  lPixels;
    unsigned int    lSize;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    unsigned int lElapsedTime = 0;

    // check whether the camera is opened or not
    
    do {
        // get time at the begining of the send image process
        start_time = std::chrono::steady_clock::now();
        if (camera.captureImage() == true) {
            lPixels = camera.getImage(&lSize);
            if (lSize > 0) {
                // set image pixels
                image->setPixels(lSize, lPixels);
                // send image to client
                sendImage(image);     
            }
            // get current time
            end_time = std::chrono::steady_clock::now();                        
            lElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();            
        }
        else {
            // no image to get: elapsed time <- 0
            lElapsedTime = 0;
            NHO_FILE_LOG(logWARNING) << "IMP_Server::captureImage: No Capture" << std::endl;
        }

        // if elapsed time is greater than 'period', sleep for another 'period'
        if (lElapsedTime > period) {
            NHO_FILE_LOG(logWARNING) << "IMP_Server::captureImage: elapsed time greater than cycle" << std::endl;      
            std::this_thread::sleep_for (std::chrono::milliseconds(period));
        }
        else {
            // reduce the next loop start
            std::this_thread::sleep_for (std::chrono::milliseconds(period - lElapsedTime));
        }
        // memory management
        image->deletePixels();
    }
    while(1);
#endif
}

///////////////////////////
// Send an image message.
bool IMP_Server::sendImage(const IMP_Image* const pImage) {

    if (dataClientSocket <= 0) {
        std::cout << "ERROR IMP_Server::sendImage no data socket" << std::endl;
        return(false);
    }
    // send message
    size_t lWrittenBytes = write(dataClientSocket, pImage->getPixels(), pImage->getDataSize());
    if (lWrittenBytes != pImage->getDataSize()) {
        std::cout << "ERROR IMP_Server::sendImage (number of written bytes:" <<
                lWrittenBytes << "/" << pImage->getDataSize() << "))." << std::endl;
        return(false);
    }
    
    // wait for an answer
    // Useless: the sent image is in chunks (STREAM SOCKETS)
    IMP_AckMessageBody* lAckMsg = new IMP_AckMessageBody();
    long lReceivedBytes;
    char* lBuffer = (char *) calloc(lAckMsg->getSize(), sizeof(char));
    lReceivedBytes = read(dataClientSocket, lBuffer, sizeof(size_t));
    // we can detect a problem in the transmission of the image
    if (lReceivedBytes < 0) {
        std::cout << "ERROR IMP_Server::sendImage (number of read bytes) " << lReceivedBytes << std::endl;
    }
    else {
        // check the answer
        lAckMsg->unserialize(lBuffer);
        if (lAckMsg->getBodyMessage() != pImage->getDataSize()) {
                std::cout << "ERROR IMP_Server::sendImage: lost image (" << std::endl;
        }
    }
    // memory management
    delete lBuffer;
    delete lAckMsg;
 
    return(true);
}

////////////////////////////////
// Never ending loop.
// Wait and register a connexion on the main socket.
// This socket is the exit for captured images.
bool IMP_Server::waitForConnectionOnDataSocket() {
#ifdef _DEBUG
    std::cout << "IMP_Server::waitForConnectionOnDataSocket \n";
#endif
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    
    listen(dataConnexionSocket, 5);
    clilen = sizeof(cli_addr);
    
    while (1) {
        dataClientSocket = accept(dataConnexionSocket,
                              (struct sockaddr *) &cli_addr,
                              &clilen);
        if (dataClientSocket < 0) {
            fprintf(stderr, "ERROR on accept");
            return(false);
        }
        // Set the socket I/O mode: In this case FIONBIO
        // enables or disables the blocking mode for the
        // socket based on the numerical value of iMode.
        // If iMode = 0, blocking is enabled;
        // If iMode != 0, non-blocking mode is enabled.
        int lMode = 0;
        ioctl(dataClientSocket, FIONBIO, &lMode);

        // an attempt to flush socket
//        int flag = 1;
//        setsockopt(dataClientSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
#ifdef _DEBUG
        std::cout << "IMP_Server::waitForConnectionOnDataSocket another loop\n";
#endif
    }
    
#ifdef _DEBUG
    std::cout << "IMP_Server::waitForConnectionOnDataSocket End\n";
#endif
    return(true);
    
}

////////////////////////////////
// Never ending loop.
// Wait and register a connexion on the main socket.
// Launch a thread to send all service messages at once to the new connected client.
bool IMP_Server::waitForConnectionOnServiceSocket() {
#ifdef _DEBUG
    std::cout << "IMP_Server::waitForConnection \n";
#endif
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    
    listen(infoConnexionSocket,5);
    clilen = sizeof(cli_addr);
    
    while (1) {
        // establish connextion for service channel
        infoClientSocket = accept(infoConnexionSocket,
                           (struct sockaddr *) &cli_addr,
                           &clilen);
        if (infoClientSocket < 0) {
            fprintf(stderr, "ERROR on accept");
            return(false);
        }
        
        // send all service messages
        serviceMessageSendingThread = new std::thread(&IMP_Server::sendServiceMessages, std::ref(*this), infoClientSocket);

    }
    
#ifdef _DEBUG
    std::cout << "IMP_Server::waitForConnection End\n";
#endif
    return(true);
    
}

/**
 * Manage a connection with a client (channel dedicated to parameters (not images).
 * Send all service messages to a client on a dedicated port.
 **/
void IMP_Server::sendServiceMessages(int pClientPort) {
#ifdef _RASPBIAN
    
#ifdef _DEBUG
     std::cout << "IMP_Server::sendServiceMessages \n";
#endif
    
    // set image size
    /// encode message
    // create and serialize message
    char*   lArray = NULL;
    IMP_Message* lMessage = new IMP_Message(clock(), IMP_Message::eImageSize);
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setWidth(camera.getWidth());
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setHeight(camera.getHeight());
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setFormat(IMP_Image::FORMAT_RGB);
    lMessage->serialize(&lArray);

    /// send message
    sendServiceMessage(pClientPort, lMessage->getSize(), lArray);
    
    // set camera parameters
    /// encode message
    /// send message
    
#ifdef _DEBUG
    std::cout << "IMP_Server::sendServiceMessages End\n";
#endif
#endif
}

///////////////////////////
// Send one service message.
bool IMP_Server::sendServiceMessage(const int pClientSocket, const size_t pSize, const char* const pMessage) {
#ifdef _DEBUG
    std::cout << "IMP_Server::sendServiceMessage \n";
#endif
    // send message
    size_t lWrittenBytes = write(pClientSocket, pMessage, pSize);
    if (lWrittenBytes != pSize) {
        std::cout << "ERROR IMP_Server::sendServiceMessage" << std::endl;
        return(false);
    }
    
    // wait for an answer
    long lReceivedBytes;
    IMP_AckMessageBody* lAckMsg = new IMP_AckMessageBody();
    char* lBuffer = (char *) calloc(lAckMsg->getSize(), sizeof(char));
    
    lReceivedBytes = read(pClientSocket, lBuffer, lAckMsg->getSize());
    if (lReceivedBytes < 0) {
        std::cout << "ERROR IMP_Server::sendServiceMessage" << std::endl;
        return(false);
    }
    
    // check the answer
    lAckMsg->unserialize(lBuffer);
    if (lAckMsg->getBodyMessage() != pSize) {
        std::cout << "ERROR IMP_Server::sendServiceMessage <invalid ack status>" << std::endl;
        return(false);
    }
    
    // memory management
    if (lAckMsg != NULL) {
        delete lAckMsg;
    }
    if (lBuffer != NULL) {
        delete lBuffer;
    }
#ifdef _DEBUG
    std::cout << "IMP_Server::sendServiceMessage End" << std::endl;
#endif
    return(true);
}
bool IMP_Server::echoing() {
    
    char buffer[256];
    long n;
    
    bzero(buffer,256);
    n = read(infoClientSocket,buffer,255);
    if (n < 0) {
        std::cout << "ERROR reading from socket";
        return(false);
    }

    printf("Here is the message: %s\n",buffer);
    n = write(infoClientSocket,"I got your message",18);
    if (n < 0) {
        std::cout << "ERROR writing to socket";
        return(false);
    }

    return(true);
    
}
