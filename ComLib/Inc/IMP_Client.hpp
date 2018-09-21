//
//  IMP_Client.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 09/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_Client_hpp
#define IMP_Client_hpp

#include <string>
#include <thread>
#include <stdio.h>

class IMP_Client {
    
    public :
    IMP_Client(const std::string pHostName, const unsigned int pInfoPort, const int pDataPort);
    
    bool initiate();
    
    bool terminate();
    
    bool waitForConnection();
    
    void manageConnection(int);
    
    bool receiveInfoMessage();
    
    bool receiveDataMessage();

    bool sendMessage(const size_t pSize, const char* const pMessage);

protected:
    unsigned int infoPort;
    unsigned int dataPort;

    std::string hostName;
    
    int infoSocket;
    int dataSocket;

    std::thread* infoThread;
    std::thread* dataThread;
    std::thread* connectionThread;
    
};
#endif /* IMP_Client_hpp */
