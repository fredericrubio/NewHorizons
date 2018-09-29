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
#include <mutex>

#include "IMP_Image.hpp"
#ifdef _RASPBIAN
#include "IMP_Camera.hpp"
#endif
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
    void captureImage();

    bool echoing();

    void sendServiceMessages(int);
    
    bool sendServiceMessage(const int pClientSocket, const size_t pSize, const char* const pMessage);
    
    bool sendImage(const IMP_Image* const pImage);
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

    // Image Section
#ifdef _RASPBIAN
    /// Capture Thread
    std::thread* imageCaptureThread;
#endif
    /// Current Image 
    IMP_Image* image;
    /// Mutex
    std::mutex imageMutex;
    /// time elapsed betwween to capture (ms)
    unsigned short period; 
#ifdef _RASPBIAN
    IMP_Camera camera;
#endif
};

#endif /* IMP_Server_hpp */
