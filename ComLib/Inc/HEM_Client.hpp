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

#include "HEM_Data.hpp"

class HEM_Client {
    
    public :
    HEM_Client(const int pDataPort);
    
    bool initiate();
    
    bool terminate();
    
    bool waitForConnection();
    
    bool receiveDataMessage();

protected:
    unsigned int dataPort;

    int dataSocket;

    std::thread* dataThread;
    
    // Data info
    HEM_Data     data;
};
#endif /* IMP_Client_hpp */
