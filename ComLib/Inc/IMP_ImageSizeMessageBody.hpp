//
//  IMP_ImageSizeMessageBody.hpp
//  ComLib
//
//  Created by Frédéric Rubio on 04/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_ImageSizeMessageBody_hpp
#define IMP_ImageSizeMessageBody_hpp

#include <stdio.h>

#include "IMP_MessageBody.hpp"

class IMP_ImageSizeMessageBody : public IMP_MessageBody {
    
    public :
    IMP_ImageSizeMessageBody();
    IMP_ImageSizeMessageBody(const unsigned int pWidth, unsigned int pHeight) ;
    
    bool serialize(char * const);

    bool unserialize(const char *const);

    virtual size_t getSize();
    
    /**
     * Getters and setters
     **/
    unsigned int getWidth() {return width;} ;
    unsigned int getHeight() {return height;} ;
    void setWidth(const unsigned int pValue) {width = pValue;} ;
    void setHeight(const unsigned int pValue) {height = pValue;} ;

    protected :
    unsigned int width;
    unsigned int height;

};
#endif /* IMP_ImageSizeMessageBody_hpp */
