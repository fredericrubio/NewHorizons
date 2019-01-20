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
    // service channel
    emissionSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (emissionSocket < 0) {
        NHO_FILE_LOG(logERROR) << "HEM_Server::initiate: Unable to create emission socket" << std::endl;
        return(false);
    }
    bzero((char *) &lInfoServAddr, sizeof(lInfoServAddr));
    lInfoServAddr.sin_family = AF_INET;
    lInfoServAddr.sin_addr.s_addr = INADDR_ANY;
    lInfoServAddr.sin_port = htons(emissionPort);
    if (bind(emissionSocket, (struct sockaddr *) &lInfoServAddr, sizeof(lInfoServAddr)) < 0) {
        NHO_FILE_LOG(logERROR) << "HEM_Server::initiate: Unable to bind" << std::endl;
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
            
        if (HEM_Server::sendServiceMessage(HEM_Data::getSize(), lMessage->getMsg())) {
            NHO_FILE_LOG(logWARNING) << "IHEM_Server::sendHEM: Sendig HEM failed" << std::endl;
        }
        
        // sleep for a while
        std::this_thread::sleep_for (std::chrono::milliseconds(period));
        
    }
    while(1);
    return false;
}

bool HEM_Server::sendServiceMessage(const size_t pSize, const char* const pMessage) {

    // send message
    size_t lWrittenBytes = write(emissionSocket, pMessage, pSize);
    if (lWrittenBytes != pSize) {
        NHO_FILE_LOG(logERROR) << "IHEM_Server::sendMessage: Sendig HEM" << std::endl;
        return(false);
    }
    
    return(true);
}


