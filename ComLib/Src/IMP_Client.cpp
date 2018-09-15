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

IMP_Client::IMP_Client(const std::string pHostName, const unsigned int pPort) :
hostName(pHostName), port(pPort) {
    
}

bool IMP_Client::initiate() {
 
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cout << "ERROR opening socket" << std::endl;
        return false;
    }
    
    server = gethostbyname(hostName.c_str());
    if (server == NULL) {
        std::cout << "ERROR, no such host"  << std::endl;
        return false;
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "ERROR connecting"  << std::endl;
        return false;
    }
    
    serverThread = new std::thread(&IMP_Client::receiveMessage, std::ref(*this));

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

bool IMP_Client::receiveMessage() {
#ifdef _DEBUG
    std::cout << "IMP_Client::receiveMessage" << std::endl;
#endif
    
    char lBuffer[IMP_Message::MAX_SIZE];
    long lReceivedBytes;
    IMP_Message* lMessage = NULL;
    
    while (1) {
        // wait for an answer
        bzero(lBuffer, sizeof(unsigned int));
        lReceivedBytes = read(sockfd, lBuffer, IMP_Message::MAX_SIZE);
        if (lReceivedBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveMessage" << std::endl;
            return(false);
        }

        switch (IMP_Message::getType(lBuffer)) {
            case IMP_Message::eImageSize:
                lMessage = new IMP_Message(IMP_Message::getDate(lBuffer), IMP_Message::getType(lBuffer));
                lMessage->unserialize(lBuffer);
                std::cout << "IMP_Client::receiveMessage Image Size " <<
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
        
        ssize_t lWrittenBytes = write(sockfd, lContents, lAckMsg->getSize());
        if (lWrittenBytes < 0) {
            std::cout << "ERROR IMP_Client::receiveMessage" << std::endl;
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
    std::cout << "IMP_Client::receiveMessage End\n";
#endif
    return true;
}

bool IMP_Client::sendMessage(const size_t pSize, const char* const pMessage) {
    
    // send message
    size_t lWrittenBytes = write(sockfd, pMessage, pSize);
    if (lWrittenBytes != pSize) {
        std::cout << "ERROR IMP_Server::sendMessage" << std::endl;
        return(false);
    }
    
    // wait for an answer
    char buffer[sizeof(unsigned int)];
    long lReceivedBytes;
    bzero(buffer, sizeof(unsigned int));
    lReceivedBytes = read(sockfd, buffer, sizeof(unsigned int));
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
