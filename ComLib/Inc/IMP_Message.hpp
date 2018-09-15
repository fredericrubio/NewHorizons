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
    IMP_Message(unsigned long pDate, IMP_MessageType pType);

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
    
    // Maximum size of a message (to ease the reception of a message we don't know the type)
    static const unsigned int MAX_SIZE = 255;
    
    static IMP_MessageType getType(const char * const pMessage) {
        unsigned int lOffset = 0;
        IMP_MessageType type;
        lOffset = sizeof(unsigned long);
        memcpy(&type, pMessage + lOffset, sizeof(type));
        return type;
    }
    
    static unsigned long getDate(const char * const pMessage) {
        unsigned long lDate = 0;
        memcpy(&lDate, pMessage, sizeof(unsigned long));
        return lDate;
    };
    
    protected :
        IMP_MessageType type;
    
        unsigned long   date;
    
        IMP_MessageBody* body;
    
};
#endif /* IMP_Message_hpp */
