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

#include "HEM_Client.hpp"

#define _DEBUG

HEM_Client::HEM_Client(const int pDataPort) : dataPort(pDataPort) {
    
}
#define MAXBUFLEN 100
bool HEM_Client::initiate() {

    // local variables
    struct hostent *server;
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
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
    std::cout << "IMP_Client::receiveDataMessage" << std::endl;
#endif
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    
/*    char lBuffer[IMP_Message::MAX_SIZE];
    long lReceivedBytes;
    IMP_Message* lMessage = NULL;
 */
    while (1) {
        
        addr_len = sizeof their_addr;
        if ((numbytes = recvfrom(dataSocket, buf, MAXBUFLEN-1 , 0,
                                 (struct sockaddr *)&their_addr, &addr_len)) == -1) {
            std::cout << "IMP_Client::receiveDataMessage recvfrom" << std::endl;
            return(false);
        }
        
#ifdef _DEBUG
                std::cout << "IMP_Client::receiveDataMessage image initialized" << std::endl;
#endif
    }
    

#ifdef _DEBUG
    std::cout << "IMP_Client::receiveDataMessage End\n";
#endif
    return true;
}

