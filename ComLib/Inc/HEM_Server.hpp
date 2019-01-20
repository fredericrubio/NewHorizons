/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEM_Server.hpp
 * Author: fredericrubio
 *
 * Created on 16 octobre 2018, 21:25
 */

#ifndef HEM_SERVER_HPP
#define HEM_SERVER_HPP

#include <stdio.h>
#include <thread>

class HEM_Server {
public :
    HEM_Server(const unsigned int pEmissionPort, const unsigned int pEmisionPeriod);
    
    /**
     * launch the thread dedicaded to connexion
     **/
    bool initiate();
    
    bool terminate();
    
    bool sendHEM();    
    
    ///////////////////////////
    // Send a data message.
    bool sendServiceMessage(const size_t pSize, const char* const pMessage);

protected:
    unsigned int emissionPort;
    int emissionSocket;
    std::thread* emissionThread;
    unsigned int period;
};

#endif /* HEM_SERVER_HPP */

