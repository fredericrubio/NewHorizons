//
//  main.cpp
//  HEM_Client
//
//  Created by Frédéric Rubio on 20/01/2019.
//  Copyright © 2019 Frédéric Rubio. All rights reserved.
//

#include <iostream>
#include <unistd.h>

#include "HEM_Client.hpp"



int main(int argc, const char * argv[]) {
    
    HEM_Client* lClient = new HEM_Client(51717);
    lClient->initiate();
    
    unsigned int microseconds = 10000000;
    while(1) {
        usleep(microseconds);
    }
    delete lClient;
    
    return 0;
}
