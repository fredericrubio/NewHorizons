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
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "NHO_LOG.hpp"
#include "HEM_Client.hpp"
#include "HEM_Message.hpp"

#define _DEBUG

HEM_Client::HEM_Client(const int pDataPort) : dataPort(pDataPort) {
    
}
#define MAXBUFLEN 100
bool HEM_Client::initiate() {

    // local variables
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int broadcast = 1;
    socklen_t optlen = sizeof(broadcast);
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;//AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, std::to_string(dataPort).c_str(), &hints, &servinfo)) != 0) {
        std::cout << "ERROR HEM_Client::initiate getaddrinfo:" << gai_strerror(rv) << std::endl;
        return 1;
    }
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((dataSocket = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            continue;
        }
        
        if (bind(dataSocket, p->ai_addr, p->ai_addrlen) == -1) {
            close(dataSocket);
            continue;
        }
        break;
    }
    
    if (p == NULL) {
        std::cout << "ERROR HEM_Client::initiate failed to bind socket." << std::endl;
        return 2;
    }

/*    struct sockaddr_in client;
    struct hostent* he = gethostbyname("192.168.0.255");
/*
    bzero(&client, sizeof(client));
//    client.sin_addr.s_addr = htonl(INADDR_ANY);
    client.sin_addr = *((struct in_addr *)he->h_addr);
    client.sin_family      = AF_INET;
    client.sin_port        = htons(dataPort);
    int code = bind(dataSocket, (struct sockaddr *)&client, sizeof(struct sockaddr));
    if (code) {
        fprintf(stderr, "bind() : %s\n", strerror(errno));
        close(dataSocket);
        return -1;
    }
*/
/*    if (setsockopt( dataSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, optlen) == -1) {
        NHO_FILE_LOG(logERROR) << "HEM_Client::receiveDataMessage: setsockopt (SO_BROADCAST)" << std::endl;
        NHO_FILE_LOG(logERROR) << "HEM_Client::receiveDataMessage: " << strerror(errno) << std::endl;
        return(false);
    }
*/
    dataThread = new std::thread(&HEM_Client::receiveDataMessage, std::ref(*this));
    return true;
}

bool HEM_Client::terminate() {
 
    return true;
    
}

bool HEM_Client::waitForConnection() {
    
    return true;
    
}

/////////////////////////////////////
// Never ending loop
// Wait for HEM messages in the dedicated socket
bool HEM_Client::receiveDataMessage() {
#ifdef _DEBUG
    std::cout << "HEM_Client::receiveDataMessage" << std::endl;
#endif
    long numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    
/*    char lBuffer[IMP_Message::MAX_SIZE];
    long lReceivedBytes;
    IMP_Message* lMessage = NULL;
 */
    HEM_Message* lMessage = new HEM_Message(clock());
    
    while (1) {
        
#ifdef _DEBUG
        std::cout << "HEM_Client::receiveDataMessage stalled on recvfrom" << std::endl;
#endif
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(dataSocket, buf, MAXBUFLEN-1 , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            std::cout << "HEM_Client::receiveDataMessage recvfrom" << std::endl;
            return(false);
        }
        lMessage->setMsg(numbytes, buf);
        lMessage->unserialize();
#ifdef _DEBUG
        std::cout << "HEM_Client::receiveDataMessage CPU:" << lMessage->getDate()->getCPUUsage() << std::endl;
        std::cout << "HEM_Client::receiveDataMessage Memory:" << lMessage->getDate()->getMemoryUsage() << std::endl;
        std::cout << "HEM_Client::receiveDataMessage Temperature:" << lMessage->getDate()->getTemperature() << std::endl;
#endif
    }
    

#ifdef _DEBUG
    std::cout << "IMP_Client::receiveDataMessage End\n";
#endif
    return true;
}

