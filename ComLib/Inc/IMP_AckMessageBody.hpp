//
//  IMP_ackMessageBody.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 11/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_AckMessageBody_hpp
#define IMP_AckMessageBody_hpp

#include <stdio.h>

#include "IMP_MessageBody.hpp"

class IMP_AckMessageBody : public IMP_MessageBody {
    
    public :
    IMP_AckMessageBody() {};
    IMP_AckMessageBody(const size_t pSize):status(pSize) {};
    
    bool serialize(char * const);
    
    bool unserialize(const char *const);
    
    virtual size_t getSize();
    
    size_t getStatus() {return status;};
    
    protected :
    size_t status;
};

#endif /* IMP_ackMessageBody_hpp */
