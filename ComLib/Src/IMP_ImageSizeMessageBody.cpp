//
//  IMP_ImageSizeMessageBody.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "IMP_ImageSizeMessageBody.hpp"

/**
 * Constructors
 **/
IMP_ImageSizeMessageBody::IMP_ImageSizeMessageBody(const unsigned int pWidth, unsigned int pHeight):width(pWidth), height(pHeight) {
    
}

IMP_ImageSizeMessageBody::IMP_ImageSizeMessageBody():width(0), height(0) {
    
}

bool IMP_ImageSizeMessageBody::serialize(char* const pSerializedArray) {
    
    // copy values
    /// width
    memcpy(pSerializedArray, &width, sizeof(width));
    /// height
    memcpy(pSerializedArray + sizeof(width), &height, sizeof(height));
    
    return true;
}

bool IMP_ImageSizeMessageBody::unserialize(const char * const pSerializedArray) {
  
    unsigned int offset = 0;
    // copy values
    /// width
    memcpy(&width, pSerializedArray, sizeof(width));
    offset += sizeof(width);
    /// height
    memcpy(&height, pSerializedArray + offset, sizeof(height));
    
    return true;
    
}

size_t IMP_ImageSizeMessageBody::getSize() {
    
    return sizeof(unsigned int) * 2;
    
}
