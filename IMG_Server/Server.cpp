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
#endif

#include "IMP_Server.hpp"

#include "NHO_LOG.hpp"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    fprintf(stdout, "size: %u\n", sizeof(size_t));
    
    NHO_FILELog::ReportingLevel() = NHO_FILELog::FromString("DEBUG");

    IMP_Server* server = new IMP_Server(51717, 51718);
    
    server->initiate();
    
    std::cout << "Server running  Main Loop\n";
    unsigned int microseconds = 10000000;
    while(1) {
        usleep(microseconds);
//        std::cout << "Server running < " << clock() << ">\n";
    }
    
    server->terminate();

    return 0;
}
