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

#include "IMP_Image.hpp"

/**
 * Constructors
 **/
IMP_ImageSizeMessageBody::IMP_ImageSizeMessageBody(
        const unsigned int pWidth, 
        const unsigned int pHeight,
        const unsigned int pFormat):width(pWidth), height(pHeight), format(pFormat) {
    
}

IMP_ImageSizeMessageBody::IMP_ImageSizeMessageBody():width(0), height(0), format(IMP_Image::FORMAT_IGNORE) {
    
}

bool IMP_ImageSizeMessageBody::serialize(char* const pSerializedArray) {
    
    // copy values
    /// width
    memcpy(pSerializedArray, &width, sizeof(width));
    /// height
    memcpy(pSerializedArray + sizeof(width), &height, sizeof(height));
    /// format
    memcpy(pSerializedArray + sizeof(width) + sizeof(height), &format, sizeof(format));
    
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
    offset += sizeof(height);
    /// format
    memcpy(&format, pSerializedArray + offset, sizeof(format));
    
    return true;
    
}

size_t IMP_ImageSizeMessageBody::getSize() {
    
    return sizeof(unsigned int) * 3;
    
}
