//
//  IMP_Image.hpp
//  DataLib
//
//  Created by Frédéric Rubio on 01/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_Image_hpp
#define IMP_Image_hpp

#include <stdlib.h>

class IMP_Image {

    public :
        /**
         Default constructor
         **/
        IMP_Image();
    
        /*
         *Image formats
         */
       typedef enum {
           FORMAT_YUV420 = 0,
           FORMAT_GRAY,
           FORMAT_BGR,
           FORMAT_RGB,
           FORMAT_IGNORE //do not use
       } IMAGE_FORMAT;
    
       /**
        * Getters and setters
        **/
       inline unsigned int getWidth() const {return width;} ;
       inline unsigned int getHeight() const {return height;} ;
       inline unsigned int getFormat() const {return format;} ;
       inline const unsigned char* getPixels() const {return pixels;} ;
       inline  unsigned int getDataSize() const {return size; } ;
       
       void setWidth(const unsigned int pValue) {width = pValue;} ;
       void setHeight(const unsigned int pValue) {height = pValue;} ;
       void setFormat(const IMAGE_FORMAT pValue) {format = pValue; size = width * height * 3;} ;
       void setPixels(const unsigned int pSize, unsigned char* const pPixels) {
           size = pSize;
/*           if (pixels != NULL) {
               free(pixels);
           }
*/           pixels = pPixels;
       } ;
    /*
     *
     */
       void deletePixels() {
           if (pixels != NULL) {
               free(pixels);
               pixels = NULL;
           }
       }
    private :
        unsigned int width;
        unsigned int height;
        IMAGE_FORMAT format;
        unsigned int size;
        unsigned char* pixels;
    
    protected :
    
};
#endif /* IMP_Image_hpp */
