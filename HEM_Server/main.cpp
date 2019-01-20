/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: fredericrubio
 *
 * Created on 16 octobre 2018, 21:20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

#ifdef _RASPBIAN
#include <raspicam.h>
#include "HEM_Message.hpp"
#endif

#include "HEM_Server.hpp"
#include "HEM_Message.hpp"

#include "NHO_LOG.hpp"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    fprintf(stdout, "size: %u\n", sizeof(size_t));
    HEM_Message lHEMMsg(clock());
    
    NHO_FILELog::ReportingLevel() = NHO_FILELog::FromString("DEBUG");

    HEM_Server* server = new HEM_Server(51717, 1);
    
    server->initiate();
    
    std::cout << "Server running  Main Loop\n";
    unsigned int microseconds = 10000000;
    while(1) {
        usleep(microseconds);
    }
    
    server->terminate();

    return 0;
}

