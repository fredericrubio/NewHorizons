/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IMP_Camera.hpp
 * Author: fredericrubio
 *
 * Created on 21 septembre 2018, 21:23
 */

#ifndef IMP_CAMERA_HPP
#define IMP_CAMERA_HPP

#include "raspicam.h"

class IMP_Camera {
public:
    IMP_Camera();

    bool open();
    
    /* 
     * Captures the next frame and keeps it in internal buffer. 
     * Blocks until next frame arrives.
     */
    bool captureImage();
    
    char* getImage();
    
    virtual ~IMP_Camera();
private:
    raspicam::RaspiCam* raspiCam;
    
};

#endif /* IMP_CAMERA_HPP */

