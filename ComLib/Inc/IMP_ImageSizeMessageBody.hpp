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
    IMP_ImageSizeMessageBody(   const unsigned int pWidth, 
                                const unsigned int pHeight, 
                                const unsigned int pFormat,
                                const unsigned int pDataSize) ;
    
    bool serialize(char * const);

    bool unserialize(const char *const);

    virtual size_t getSize();
    
    /**
     * Getters and setters
     **/
    inline unsigned int getWidth() const {return width;} ;
    inline unsigned int getHeight() const {return height;} ;
    inline unsigned int getFormat() const {return format;} ;
    inline unsigned int getDataSize() const {return dataSize;} ;
    
    inline void setWidth(const unsigned int pValue) {width = pValue;} ;
    inline void setHeight(const unsigned int pValue) {height = pValue;} ;
    inline void setFormat(const unsigned int pValue) {format = pValue;} ;
    inline void setDataSize(const unsigned int pValue) {dataSize = pValue;} ;

    protected :
    unsigned int width;
    unsigned int height;
    unsigned int format;
    unsigned int dataSize;

};
#endif /* IMP_ImageSizeMessageBody_hpp */
