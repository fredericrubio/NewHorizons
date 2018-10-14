//
//  HEM_Message.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 13/10/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef HEM_Message_hpp
#define HEM_Message_hpp

#include <stdio.h>

class HEM_Message {
  
public:
    /**
     * Constructors
     **/
    HEM_Message(const long long   pDate);
    
    /**
     * Destructor
     **/
    ~HEM_Message();
   
    /**
     * Serializes the current values of the health monitoring message (allocates and fills the
     * the buffer readdy to be sent.
     **/
    size_t serialize();
    
    /**
     * Unserializes the buffer into the attributes.
     * Returns false if buffer not initialized.
     * The management of values out of definition ranges is still to be tackled.
     **/
    bool unserialize();

    /**
     * Returns the size (in bytes) of a HEM message
     **/
    inline size_t getSize() const {
        return (sizeof(date) + sizeof(cpu) + sizeof(temp) + sizeof(memory)) ;
    }
    
    /**
     * Getters
     **/
    inline short getCPUUsage() const {return cpu;};
    inline short getTemperature() const {return temp;};
    inline short getMemoryUsage() const {return memory;};
    
    
    /**
     * Setters
     **/
    inline void setCPUUsage(const short pCPU) {cpu = pCPU;};
    inline void setTemperature(const short pTemperature) {temp = pTemperature;};
    inline void setMemoryUsage(const short pMemory) {memory = pMemory;};
    
private:
    long long   date; // 'long long' to force 64bits on 32bits OS
    
    short cpu;
    short temp;
    short memory;
    
    char* msg;
    
    /**
     * Fetch CPU
     **/
    bool fetchCPUUsage();
    
    /**
     * Fetch Temperature
     **/
    bool fetchTemperature();
    
    /**
     * Fetch Memory
     **/
    bool fetchMemoryUsage();
    
};
#endif /* HEM_Message_hpp */
