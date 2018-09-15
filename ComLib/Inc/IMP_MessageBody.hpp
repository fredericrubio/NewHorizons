//
//  IMP_MessageBody.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_MessageBody_hpp
#define IMP_MessageBody_hpp

#include <stdio.h>

class IMP_MessageBody {
    
    public :
        virtual bool serialize( char * const) = 0;
        
        virtual bool unserialize(const char * const) = 0;

        virtual size_t getSize();
    
};

#endif /* IMP_MessageBody_hpp */
