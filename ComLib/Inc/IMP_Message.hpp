//
//  IMP_Message.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_Message_hpp
#define IMP_Message_hpp

#include <stdio.h>

#include "IMP_MessageBody.hpp"

class IMP_Message {
    
    public :
    typedef enum {eUnknown, eImageSize, eCameraParameters, eImage} IMP_MessageType;
    
    /**
     * Constructors
     **/
    IMP_Message();
    IMP_Message(long long pDate, IMP_MessageType pType);

    /**
     * Destructor
     **/
    ~IMP_Message();
    
    size_t serialize(char **);

    bool unserialize(const char *);

    const IMP_MessageBody* getBody() {return body;};

    virtual size_t getSize() {
        size_t lSize = sizeof(IMP_MessageType) + sizeof(unsigned long);;
        if (body != NULL) {
            lSize += body->getSize();
        }
        return lSize;
    };
    
    /////////////////////////////////////////
    // Constants and static methods
    // Maximum size of a message (to ease the reception of a message we don't know the type)
    static const unsigned int MAX_SIZE = 255;
    
    static IMP_MessageType getType(const char * const pMessage) {
        unsigned int lOffset = 0;
        IMP_MessageType type;
        lOffset = sizeof(unsigned long);
        memcpy(&type, pMessage + lOffset, sizeof(IMP_MessageType));
        return type; 
    }
    
    static long long getDate(const char * const pMessage) {
        unsigned int lOffset = 0;
        unsigned long lDate;
        memcpy(&lDate, pMessage + lOffset, sizeof(unsigned long));
        return lDate;
    }
    // Constants and static methods
    /////////////////////////////////////////

    protected :
        IMP_MessageType type;
    
        long long   date; // 'long long' to force 64bits on 32bits OS
    
        IMP_MessageBody* body;
    
};
#endif /* IMP_Message_hpp */
