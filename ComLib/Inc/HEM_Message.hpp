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

class HEM_Data;

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
     * Set the payload.
     **/
    inline void setData(HEM_Data* const pData) {data = pData;};
    
    inline const char* getMsg() const {return msg;};
    
private:
    long long   date; // 'long long' to force 64bits on 32bits OS
    
    HEM_Data*   data;
    
    char* msg;

    
};
#endif /* HEM_Message_hpp */
