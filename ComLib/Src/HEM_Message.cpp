//
//  HEM_Message.cpp
//  ComLib
//
//  Created by Frédéric Rubio on 13/10/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//
#include <string.h>
#include <iostream>

#include "HEM_Message.hpp"

#include "HEM_Data.hpp"

/**
 * Constructors
 **/
HEM_Message::HEM_Message(const long long   pDate):
date(pDate), data(NULL), msg(NULL){
        
}

/**
 * Destructor
 **/
HEM_Message::~HEM_Message() {
    
    delete data;
    
    if (msg != NULL) {
        free(msg);
    }
    
}

/**
 * Serializes the current values of the health monitoring message (allocates and fills the
 * the buffer readdy to be sent.
 **/
size_t HEM_Message::serialize() {
    
    if (data == NULL) {
        return 0;    
    }
    
    if (msg != NULL) {
        free(msg);
    }
    
    // calculate message size
    size_t lSize = data->getSize();

    msg = (char *) calloc(lSize, sizeof(char));
    
    // copy values
    unsigned int offset = 0;
    /// date
    long long lDate = data->getDate();
    memcpy((void *) msg, &lDate, sizeof(lDate));
    offset += sizeof(lDate);
    /// cpu
    short lCpu = data->getCPUUsage();
    memcpy((void *) (msg + offset), &lCpu, sizeof(lCpu));
    offset += sizeof(lCpu);
    
    /// temp
    short lTemp = data->getTemperature();
    memcpy((void *) (msg + offset), &lTemp, sizeof(lTemp));
    offset += sizeof(lTemp);
    
    /// mempry
    short lMemory = data->getMemoryUsage();
    memcpy((void *) (msg + offset), &lMemory, sizeof(lMemory));
    offset += sizeof(lMemory);
    
    return lSize;
}

/**
 * Unserializes the buffer into the attributes.
 * Returns false if buffer not initialized.
 * The management of values out of definition ranges is still to be tackled.
 **/
bool HEM_Message::unserialize() {
    // calculate message size
    unsigned int offset = 0;
    HEM_Data* lData = new HEM_Data();
    
    if (msg == NULL) {
        return false;
    }
    
    // copy values
    /// date
    long long lDate;
    memcpy(&lDate, (void *) msg, sizeof(lDate));
    offset += sizeof(lDate);
    lData->setDate(lDate);
    
    /// cpu
    short lCpu;
    memcpy(&lCpu, (void *) (msg + offset), sizeof(lCpu));
    offset += sizeof(lCpu);
    lData->setCPUUsage(lCpu);
    
    /// temp
    short lTemperature;
    memcpy(&lTemperature, (void *) (msg + offset), sizeof(lTemperature));
    offset += sizeof(lTemperature);
    lData->setTemperature(lTemperature);
    
    /// memory
    short lMemory;
    memcpy(&lMemory, (void *) (msg + offset), sizeof(lMemory));
    offset += sizeof(lMemory);
    lData->setMemoryUsage(lMemory);
    
    setData(lData);
    
    return true;
}

