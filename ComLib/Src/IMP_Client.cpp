//
//  IMP_Client.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 09/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "IMP_Client.hpp"

#include "IMP_Message.hpp"
#include "IMP_ImageSizeMessageBody.hpp"
#include "IMP_AckMessageBody.hpp"

#define _DEBUG

IMP_Client::IMP_Client(const std::string pHostName, const unsigned int pInfoPort, const int pDataPort) :
hostName(pHostName), infoPort(pInfoPort), dataPort(pDataPort) {
    
}

bool IMP_Client::initiate() {

    // local variables
    struct hostent *server;

    struct sockaddr_in lInfoServAddr;
    infoSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (infoSocket < 0) {
        std::cout << "ERROR opening socket" << std::endl;
        return false;
    }
    
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        std::cout << "ERROR, no such host"  << std::endl;
        return false;
    }
    
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&lInfoServAddr.sin_addr.s_addr,
          server->h_length);
    lInfoServAddr.sin_port = htons(infoPort);
    if (connect(infoSocket, (struct sockaddr *) &lInfoServAddr, sizeof(lInfoServAddr)) < 0) {
        std::cout << "ERROR connecting info port"  << std::endl;
        return false;
    }
    
    ///////////////////
    // data connection
    struct sockaddr_in lDataServAddr;
    dataSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (dataSocket < 0) {
        std::cout << "ERROR opening data socket" << std::endl;
        return false;
    }
    
    bzero((char *) &lDataServAddr, sizeof(lDataServAddr));
    lDataServAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&lDataServAddr.sin_addr.s_addr,
          server->h_length);
    lDataServAddr.sin_port = htons(dataPort);
    if (connect(dataSocket, (struct sockaddr *) &lDataServAddr, sizeof(lDataServAddr)) < 0) {
        std::cout << "ERROR connecting data port"  << std::endl;
        return false;
    }
    
    infoThread = new std::thread(&IMP_Client::receiveInfoMessage, std::ref(*this));
    dataThread = new std::thread(&IMP_Client::receiveDataMessage, std::ref(*this));
    
    return true;
}

bool IMP_Client::terminate() {
 
    return true;
    
}

bool IMP_Client::waitForConnection() {
    
    return true;
    
}

void IMP_Client::manageConnection(int) {
    
}

/////////////////////////////////////
// Never ending loop
// Wait for data messages on the dedicated socket
bool IMP_Client::receiveDataMessage() {
#ifdef _DEBUG
    std::cout << "IMP_Client::receiveDataMessage" << std::endl;
#endif
    
    // the sizes of transferred images is known:
    char lBuffer[IMP_Message::MAX_SIZE];
    long lReceivedBytes;
    IMP_Message* lMessage = NULL;
    
    while (1) {
        // wait for an answer
        bzero(lBuffer, sizeof(unsigned int));
        lReceivedBytes = read(dataSocket, lBuffer, IMP_Message::MAX_SIZE);
        if (lReceivedBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveDataMessage" << std::endl;
            return(false);
        }
        
        switch (IMP_Message::getType(lBuffer)) {
            case IMP_Message::eImageSize:
                lMessage = new IMP_Message(IMP_Message::getDate(lBuffer), IMP_Message::getType(lBuffer));
                lMessage->unserialize(lBuffer);
                std::cout << "IMP_Client::receiveDataMessage Image Size " <<
                ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->getWidth()
                << " " <<
                ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->getHeight()
                << std::endl;
                break;
                
            default:
                break;
        }
        // send an aknownledgement
        IMP_AckMessageBody* lAckMsg = new IMP_AckMessageBody(lReceivedBytes);
        char* lContents = (char *) calloc(lAckMsg->getSize(), sizeof(char));
        lAckMsg->serialize(lContents);
        delete lAckMsg;
        
        ssize_t lWrittenBytes = write(dataSocket, lContents, lAckMsg->getSize());
        if (lWrittenBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveDataMessage" << std::endl;
            return(false);
        }
        
        // delete last received message
        if (lMessage != NULL) {
            delete lMessage;
        }
        if (lContents != NULL) {
            delete lContents;
        }
    }
    
#ifdef _DEBUG
    std::cout << "IMP_Client::receiveDataMessage End\n";
#endif
    return true;
}

/////////////////////////////////////
// Never ending loop
// Wait for service messages in the dedicated socket
bool IMP_Client::receiveInfoMessage() {
#ifdef _DEBUG
    std::cout << "IMP_Client::receiveInfoMessage" << std::endl;
#endif
    
    char lBuffer[IMP_Message::MAX_SIZE];
    long lReceivedBytes;
    IMP_Message* lMessage = NULL;
    
    while (1) {
        // wait for an answer
        bzero(lBuffer, sizeof(unsigned int));
        lReceivedBytes = read(infoSocket, lBuffer, IMP_Message::MAX_SIZE);
        if (lReceivedBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveInfoMessage" << std::endl;
            return(false);
        }

        switch (IMP_Message::getType(lBuffer)) {
            case IMP_Message::eImageSize:
                lMessage = new IMP_Message(IMP_Message::getDate(lBuffer), IMP_Message::getType(lBuffer));
                lMessage->unserialize(lBuffer);
                std::cout << "IMP_Client::receiveInfoMessage Image Size " <<
                    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->getWidth()
                    << " " <<
                    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->getHeight()
                    << " " <<
                    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->getFormat()
                    << std::endl;
                break;
                
            default:
                break;
        }
        // send an aknownledgement
        IMP_AckMessageBody* lAckMsg = new IMP_AckMessageBody(lReceivedBytes);
        char* lContents = (char *) calloc(lAckMsg->getSize(), sizeof(char));
        lAckMsg->serialize(lContents);
        delete lAckMsg;
        
        ssize_t lWrittenBytes = write(infoSocket, lContents, lAckMsg->getSize());
        if (lWrittenBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveInfoMessage" << std::endl;
            return(false);
        }
        
        // delete last received message
        if (lMessage != NULL) {
            delete lMessage;
        }
        if (lContents != NULL) {
            delete lContents;
        }
    }

#ifdef _DEBUG
    std::cout << "IMP_Client::receiveInfoMessage End\n";
#endif
    return true;
}

bool IMP_Client::sendMessage(const size_t pSize, const char* const pMessage) {
    
    // send message
    size_t lWrittenBytes = write(infoSocket, pMessage, pSize);
    if (lWrittenBytes != pSize) {
        std::cout << "ERROR IMP_Server::sendMessage" << std::endl;
        return(false);
    }
    
    // wait for an answer
    char buffer[sizeof(unsigned int)];
    long lReceivedBytes;
    bzero(buffer, sizeof(unsigned int));
    lReceivedBytes = read(infoSocket, buffer, sizeof(unsigned int));
    if (lReceivedBytes != sizeof(unsigned int)) {
        std::cout << "ERROR IMP_Server::sendMessage" << std::endl;
        return(false);
    }
    
    // check the answer
    if (lReceivedBytes != lWrittenBytes) {
        std::cout << "ERROR IMP_Server::sendMessage" << std::endl;
        return(false);
    }
    
    return(true);
}
