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

#define _DEBUG

IMP_Server::IMP_Server(const unsigned int pInfoPort, const unsigned int pDataPort):
infoPort(pInfoPort), dataPort(pDataPort),  infoConnexionSocket(0), dataConnexionSocket(0) {

    period = 1000;
    
    image = new IMP_Image();
 
    camera.open();
    std::this_thread::sleep_for (std::chrono::seconds(4));
 
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
    imageCaptureThread = new std::thread(&IMP_Server::captureImage, std::ref(*this));
    
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

    unsigned char*  lPixels;
    unsigned int    lSize;
    std::chrono::time_point<std::chrono::steady_clock> start_time;
    std::chrono::time_point<std::chrono::steady_clock> end_time;
    std::chrono::system_clock::duration duration;
    clock_t t1, t2;
    
    // check whether the camera is opened or not
    
    do {
        t1 = clock();
        start_time = std::chrono::steady_clock::now();
        if (camera.captureImage() == true) {
            lPixels = camera.getImage(&lSize);
            image->setPixels(lSize, lPixels);
            
            end_time = std::chrono::steady_clock::now();
            t2 = clock();

            NHO_FILE_LOG(logDEBUG) << "IMP_Server::captureImage: " << t2 - t1 << std::endl;
//        NHO_FILE_LOG(logDEBUG) << "IMP_Server::captureImage: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << std::endl;
//        NHO_FILE_LOG(logDEBUG) << "IMP_Server::captureImage " << std::endl;
        }
        else {
            NHO_FILE_LOG(logDEBUG) << "IMP_Server::captureImage: No Capture" << std::endl;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(period));
    }
    while(1);
    
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
    
#ifdef _DEBUG
     std::cout << "IMP_Server::sendServiceMessages \n";
#endif
    usleep(100000);
    
    // set image size
    /// encode message
    // create and serialize message
    char*   lArray = NULL;
    IMP_Message* lMessage = new IMP_Message(clock(), IMP_Message::eImageSize);
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setWidth(123);
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setHeight(456);
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setFormat(IMP_Image::FORMAT_RGB);
    lMessage->serialize(&lArray);

    /// send message
    sendServiceMessage(pClientPort, lMessage->getSize(), lArray);

    // set image format
    /// encode message
    // create and serialize message
    
    // set camera parameters
    /// encode message
    /// send message
    
#ifdef _DEBUG
    std::cout << "IMP_Server::manageConnection End\n";
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
    if (lAckMsg->getStatus() != pSize) {
        std::cout << "ERROR IMP_Server::sendServiceMessage <invalid ack status>" << std::endl;
        return(false);
    }
    
    // memory management
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
