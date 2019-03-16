/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEM_Server.cpp
 * Author: fredericrubio
 * 
 * Created on 16 octobre 2018, 21:25
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>  
#include <netdb.h>
#include <arpa/inet.h>

#include "HEM_Server.hpp"

#include "NHO_LOG.hpp"
#include "HEM_Message.hpp"
#include "HEM_Data.hpp"
HEM_Server::HEM_Server(const unsigned int pEmissionPort, const unsigned int pEmisionPeriod):
emissionPort(pEmissionPort),
period(pEmisionPeriod) {
    
}
    
/**
 * launch the thread dedicaded to connexion
 **/
bool HEM_Server::initiate() {
#ifdef _DEBUG
    std::cout << "IMP_Server::initiate \n";
#endif
    struct sockaddr_in lInfoServAddr;
    int broadcast = 1;
//    char broadcast = '1';
    socklen_t optlen = sizeof(broadcast);
    
    // service channel
    emissionSocket = socket(AF_INET, SOCK_DGRAM, 0);//IPPROTO_UDP);
    if (emissionSocket < 0) {
        NHO_FILE_LOG(logERROR) << "HEM_Server::initiate: Unable to create emission socket" << std::endl;
        return(false);
    }
    // this call is what allows broadcast packets to be sent:
/*    getsockopt( emissionSocket, SOL_SOCKET, SO_BROADCAST, 
                    &broadcast, &optlen);
*/    if (setsockopt( emissionSocket, SOL_SOCKET, SO_BROADCAST, 
                    &broadcast, optlen) == -1) {
        NHO_FILE_LOG(logERROR) << "HEM_Server::initiate: " << strerror(errno) << std::endl;
        NHO_FILE_LOG(logERROR) << "HEM_Server::initiate: setsockopt (SO_BROADCAST)" << std::endl;
        return(false);
    }
    
    // launch the emission thread
    emissionThread = new std::thread(&HEM_Server::sendHEM, std::ref(*this));
    
    return(true);
}

bool HEM_Server::terminate() {
    return false;
}

bool HEM_Server::sendHEM() {
    
    HEM_Message* lMessage = new HEM_Message(clock());
    HEM_Data*    lData = new HEM_Data();
    lMessage->setData(lData);
    
    do {
        // update data
        lData->fetch();
        
        /// encode message
        // serialize message
        lMessage->serialize();
            
        HEM_Server::sendServiceMessage(HEM_Data::getSize(), lMessage->getMsg());
        
        // sleep for a while
        std::this_thread::sleep_for (std::chrono::milliseconds(period));
        
    }
    while(1);
    return false;
}

bool HEM_Server::sendServiceMessage(const size_t pSize, const char* const pMessage) {
    
    struct sockaddr_in lInfoServAddr;  
//    struct hostent* he = gethostbyname("255.255.255.255");
    struct hostent* he = gethostbyname("192.168.0.255");

    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
//    lInfoServAddr.sin_addr.s_addr = INADDR_ANY; // INADDR_BROADCAST //? sure about that ?
    lInfoServAddr.sin_addr = *((struct in_addr *)he->h_addr);
    lInfoServAddr.sin_port = htons(emissionPort);

    socklen_t optlen = sizeof(lInfoServAddr);
    
 //   inet_pton(AF_INET, "255.255.255.255", &lInfoServAddr.sin_addr);
    
    // send message
    size_t lWrittenBytes = sendto(  emissionSocket, 
                                    pMessage, 
                                    pSize, 
                                    0, 
                                    (struct sockaddr *)&lInfoServAddr, 
                                    optlen);  
//    size_t lWrittenBytes = write(emissionSocket, pMessage, pSize);
    if (lWrittenBytes != pSize) {
        NHO_FILE_LOG(logERROR) << "HEM_Server::sendMessage: Sendig HEM failed <" << lWrittenBytes << ">" 
                << " vs <" << pSize << ">" << std::endl;
        return(false);
    }
    
    return(true);
}


