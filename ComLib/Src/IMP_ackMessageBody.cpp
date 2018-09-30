//
//  IMP_AckMessageBody.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 11/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//
#include <string.h>
#include <stdlib.h>

#include "IMP_AckMessageBody.hpp"
/**
 * Constructors
 **/

bool IMP_AckMessageBody::serialize(char* const pSerializedArray) {
    
    // copy values
    /// size
    memcpy(pSerializedArray, &status, sizeof(size_t));
    
    return true;
}

bool IMP_AckMessageBody::unserialize(const char * const pSerializedArray) {
    
    // copy values
    /// size
    memcpy(&status, pSerializedArray, sizeof(size_t));

    return true;
    
}

unsigned int IMP_AckMessageBody::getSize() {
    
    return sizeof(size_t) ;
    
}
