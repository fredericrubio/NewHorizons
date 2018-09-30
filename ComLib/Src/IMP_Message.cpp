//
//  IMP_Message.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "IMP_Message.hpp"

#include "IMP_ImageSizeMessageBody.hpp"

IMP_Message::IMP_Message(): date(0), type(eUnknown), body(NULL) {
}

IMP_Message::IMP_Message(long long pDate, IMP_MessageType pType): date(pDate), type(pType) {

    switch(type) {
        case eImageSize:
            body = new IMP_ImageSizeMessageBody();
        break;
        case eCameraParameters:
        case eImage:
        case eUnknown:
        break;
    }
    
}

/**
 * Destructor
 **/
IMP_Message::~IMP_Message() {
    
    if (body != NULL) {
        delete body;
    }
    
}

unsigned int IMP_Message::serialize(char **pSerializedArray) {
    
    // calculate message size
    size_t size = sizeof(date) + sizeof(type);
    
    switch(type) {
        case eImageSize:
            size += body->getSize();
        break;
        case eUnknown:
        case eCameraParameters:
        case eImage:
        break;
    }
    
    *pSerializedArray = (char *) calloc(size, sizeof(char));
    
    // copy values
    unsigned int offset = 0;
    /// date
    memcpy((void *) (*pSerializedArray), &date, sizeof(date));
    offset += sizeof(date);
    /// type
    memcpy((void *) (*pSerializedArray + offset), &type, sizeof(type));
    offset += sizeof(type);

    body->serialize(*pSerializedArray + offset);
    
    return size;
}

bool IMP_Message::unserialize(const char *pSerializedArray) {
    
    // calculate message size
    unsigned int offset = 0;

    // copy values
    /// date
    memcpy(&date, (void *) pSerializedArray, sizeof(date));
    offset += sizeof(date);
    /// type
    memcpy(&type, pSerializedArray + offset, sizeof(type));
    offset += sizeof(type);
    
    if (body == NULL) {
        switch(type) {
            case eImageSize:
                body = new IMP_ImageSizeMessageBody();
                body->unserialize(pSerializedArray + offset);
                break;
            case eCameraParameters:
            case eImage:
            case eUnknown:
                break;
        }
    }
    body->unserialize(pSerializedArray + offset);
    
    return true;
    
}



