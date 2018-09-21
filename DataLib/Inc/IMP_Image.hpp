//
//  IMP_Image.hpp
//  DataLib
//
//  Created by Frédéric Rubio on 01/09/2018.
//  Copyright © 2018 Frédéric Rubio. All rights reserved.
//

#ifndef IMP_Image_hpp
#define IMP_Image_hpp

class IMP_Image {
    
    typdef enum {} ImageType; // have a look on the campi definition
    public :
        /**
         Default constructor
         **/
        IMP_Image();
    
    private :
        unsigned int width;
        unsigned int height;
    
        float* pixels;
    
    protected :
    
};
#endif /* IMP_Image_hpp */
