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

/**
 * Constructors
 **/
HEM_Message::HEM_Message(const long long   pDate):
date(pDate), msg(NULL){
    
}

/**
 * Destructor
 **/
HEM_Message::~HEM_Message() {
    
    if (msg != NULL) {
        free(msg);
    }
    
}

/**
 * Serializes the current values of the health monitoring message (allocates and fills the
 * the buffer readdy to be sent.
 **/
size_t HEM_Message::serialize() {
    
    // calculate message size
    size_t lSize = getSize();

    if (msg != NULL) {
        free(msg);
    }
    
    msg = (char *) calloc(lSize, sizeof(char));
    
    // copy values
    unsigned int offset = 0;
    /// date
    memcpy((void *) msg, &date, sizeof(date));
    offset += sizeof(date);
    /// cpu
    memcpy((void *) (msg + offset), &cpu, sizeof(cpu));
    offset += sizeof(cpu);
    /// temp
    memcpy((void *) (msg + offset), &temp, sizeof(temp));
    offset += sizeof(temp);
    /// mempry
    memcpy((void *) (msg + offset), &memory, sizeof(memory));
    offset += sizeof(memory);
    
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
    
    if (msg == NULL) {
        return false;
    }
    
    // copy values
    /// date
    memcpy(&date, (void *) msg, sizeof(date));
    offset += sizeof(date);
    /// cpu
    memcpy(&cpu, (void *) msg, sizeof(cpu));
    offset += sizeof(cpu);
    /// temp
    memcpy(&date, (void *) msg, sizeof(temp));
    offset += sizeof(temp);
    /// memory
    memcpy(&date, (void *) msg, sizeof(memory));
    offset += sizeof(memory);
    
    return true;
}

/**
 * Fetch CPU
 **/
bool HEM_Message::fetchCPUUsage() {
#ifdef _RASPBIAN
    int FileHandler;
    char FileBuffer[1024];
    float load;
    
    FileHandler = open("/proc/loadavg", O_RDONLY);
    if(FileHandler < 0) {
        return -1; }
    read(FileHandler, FileBuffer, sizeof(FileBuffer) - 1);
    sscanf(FileBuffer, "%f", &load);
    close(FileHandler);
    return (int)(load * 100);
#endif
}

/**
 * Fetch Temperature
 **/
bool HEM_Message::fetchTemperature() {
    
}

/**
 * Fetch Memory
 **/
bool HEM_Message::fetchMemoryUsage() {
    
}
