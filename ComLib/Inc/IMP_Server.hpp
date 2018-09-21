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
    IMP_Server(const unsigned int pInfoPort, const unsigned int pDataPort);
    
    /**
     * launch the thread dedicaded to connexion
     **/
    bool initiate();
    
    bool terminate();
    
    bool waitForConnectionOnServiceSocket();
    bool waitForConnectionOnDataSocket();

    bool echoing();

    void sendServiceMessages(int);
    
    bool sendServiceMessage(const int pClientSocket, const size_t pSize, const char* const pMessage);
    
protected:
    unsigned int infoPort;
    unsigned int dataPort;

    int infoConnexionSocket;
    int dataConnexionSocket;

    int infoClientSocket; // to manage a client connection (information, service message, commands, ...)
    int dataClientSocket; // to manage a client connection

    std::thread* serviceMessageSendingThread;
    std::thread* serviceConnectionThread;
    
    std::thread* dataConnectionThread;

};

#endif /* IMP_Server_hpp */
