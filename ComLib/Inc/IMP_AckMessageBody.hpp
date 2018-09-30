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
    IMP_AckMessageBody(const unsigned int pSize):status(pSize) {};
    
    bool serialize(char * const);
    
    bool unserialize(const char *const);
    
    virtual unsigned int getSize();
    
    unsigned int getBodyMessage() {return status;};
    
    protected :
    unsigned int status;
};

#endif /* IMP_ackMessageBody_hpp */
