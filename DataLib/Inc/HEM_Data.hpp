/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HEM_Data.h
 * Author: fredericrubio
 *
 * Created on 14 octobre 2018, 21:52
 */

#ifndef HEM_DATA_H
#define HEM_DATA_H

class HEM_Data {
public:
    HEM_Data();
    HEM_Data(const long long pDate);
    HEM_Data(const HEM_Data& orig);
    virtual ~HEM_Data();
    
    /**
     * Returns the size (in bytes) of a HEM message
     **/
    static inline size_t getSize() {
        return (sizeof(date) + sizeof(cpu) + sizeof(temp) + sizeof(usedMemory)) ;
    }
    
    /**
     * Getters
     **/
    inline long long getDate() const {return date;};
    inline short getCPUUsage() const {return cpu;};
    inline short getTemperature() const {return temp;};
    inline short getMemoryUsage() const {return usedMemory;};
    
    /**
     * Setters
     **/
    inline void setCPUUsage(const short pCPU) {cpu = pCPU;};
    inline void setTemperature(const short pTemperature) {temp = pTemperature;};
    inline void setMemoryUsage(const short pMemory) {usedMemory = pMemory;};
    inline void setDate(const long long pDate) {date = pDate;};
    
    /**
     * Update health monitoring sata
     **/
    bool fetch();
    
private: 
    long long   date; // 'long long' to force 64bits on 32bits OS
    
    short cpu;
    short temp;
    short usedMemory;
    
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

#endif /* HEM_DATA_H */

