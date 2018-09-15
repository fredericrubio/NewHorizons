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

#include "IMP_Server.hpp"

#include "IMP_Message.hpp"
#include "IMP_ImageSizeMessageBody.hpp"
#include "IMP_AckMessageBody.hpp"

#define _DEBUG

IMP_Server::IMP_Server(unsigned int pPort): port(pPort), sockfd(0) {
    
}

bool IMP_Server::initiate() {
#ifdef _DEBUG
    std::cout << "IMP_Server::initiate \n";
#endif
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "ERROR opening socket");
        return(false);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "IMP_Server: ERROR on binding " << std::endl;
        return(false);
    }

    connectionThread = new std::thread(&IMP_Server::waitForConnection, std::ref(*this));
//    connectionThread->join();
    
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
    close(newsockfd);
    
    // close main socket
    close(sockfd);
    
#ifdef _DEBUG
    std::cout << "IMP_Server::terminate End\n";
#endif
    return(true);
    
}

bool IMP_Server::waitForConnection() {
#ifdef _DEBUG
    std::cout << "IMP_Server::waitForConnection \n";
#endif
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    while (1) {
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr,
                           &clilen);
        if (newsockfd < 0) {
            fprintf(stderr, "ERROR on accept");
            return(false);
        }
        
        clientThread = new std::thread(&IMP_Server::manageConnection, std::ref(*this), newsockfd);

    }
    
#ifdef _DEBUG
    std::cout << "IMP_Server::waitForConnection End\n";
#endif
    return(true);
    
}

bool IMP_Server::sendMessage(const size_t pSize, const char* const pMessage) {
#ifdef _DEBUG
    std::cout << "IMP_Server::sendMessage \n";
#endif
    // send message
    size_t lWrittenBytes = write(newsockfd, pMessage, pSize);
    if (lWrittenBytes != pSize) {
        std::cout << "ERROR IMP_Server::sendMessage" << std::endl;
        return(false);
    }
    
    // wait for an answer
    long lReceivedBytes;
    IMP_AckMessageBody* lAckMsg = new IMP_AckMessageBody();
    char* lBuffer = (char *) calloc(lAckMsg->getSize(), sizeof(char));

    lReceivedBytes = read(newsockfd, lBuffer, lAckMsg->getSize());
    if (lReceivedBytes < 0) {
        std::cout << "ERROR IMP_Server::sendMessage" << std::endl;
        return(false);
    }
    
    // check the answer
    lAckMsg->unserialize(lBuffer);
    if (lAckMsg->getStatus() != pSize) {
        std::cout << "ERROR IMP_Server::sendMessage <invalid ack status>" << std::endl;
        return(false);
    }

    if (lBuffer != NULL) {
        delete lBuffer;
    }
#ifdef _DEBUG
    std::cout << "IMP_Server::sendMessage End" << std::endl;
#endif
    return(true);
}
/**
 * Manage a connection with a client (channel dedicated to parameters (not images).
 **/
void IMP_Server::manageConnection(int pClientPort) {
    
#ifdef _DEBUG
     std::cout << "IMP_Server::manageConnection \n";
#endif
    usleep(100000);
    
    // set image size
    /// encode message
    // create message
    char*   lArray = NULL;
    IMP_Message* lMessage = new IMP_Message(clock(), IMP_Message::eImageSize);
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setWidth(123);
    ((IMP_ImageSizeMessageBody*) (lMessage->getBody()))->setHeight(456);
    lMessage->serialize(&lArray);

    /// send message
    sendMessage(lMessage->getSize(), lArray);
    std::cout << "IMP_Server::manageConnection sendMessage\n";

    // set camera parameters
    /// encode message
    /// send message
    
#ifdef _DEBUG
    std::cout << "IMP_Server::manageConnection End\n";
#endif
}

bool IMP_Server::echoing() {
    
    char buffer[256];
    long n;
    
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) {
        std::cout << "ERROR reading from socket";
        return(false);
    }

    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) {
        std::cout << "ERROR writing to socket";
        return(false);
    }

    return(true);
    
}

