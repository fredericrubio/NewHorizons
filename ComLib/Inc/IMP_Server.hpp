//
//  IMP_Server.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 05/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_Server_hpp
#define IMP_Server_hpp

#include <stdio.h>
#include <thread>

class IMP_Server {
    
public :
    IMP_Server(unsigned int pPort);
    
    bool initiate();
    
    bool terminate();
    
    bool waitForConnection();
    
    bool echoing();

    void manageConnection(int);
    
    bool sendMessage(const size_t pSize, const char* const pMessage);
    
protected:
    unsigned int port;
    
    int sockfd;
    
    int newsockfd; // to manage a client connection
    
    std::thread* clientThread;
    std::thread* connectionThread;

};

#endif /* IMP_Server_hpp */
