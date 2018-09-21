/* 
 * File:   IMP_Camera.cpp
 * Author: fredericrubio
 * 
 * Created on 21 septembre 2018, 21:23
 */

#include "IMP_Camera.hpp"

#include "NHO_LOG.hpp"

IMP_Camera::IMP_Camera(): raspiCam(NULL) {
    
    raspiCam = new raspicam::RaspiCam();
    
}

IMP_Camera::~IMP_Camera() {
    
    if (raspiCam != NULL) {
        delete raspiCam;
    }
    
}


bool IMP_Camera::open() {
    
    if (raspiCam->open()) {
        NHO_FILE_LOG(logDEBUG) << "IMP_Camera::open: OK" << std::endl;
        return true;
    }
    
    NHO_FILE_LOG(logERROR) << "IMP_Camera::open: Error opening camera" << std::endl;
    return false;
    
}

bool IMP_Camera::captureImage() {
    
    bool lCapture = raspiCam->grab();
    
    NHO_FILE_LOG(logDEBUG) << "IMP_Camera::captureImage: " << lCapture << std::endl;
 
    return lCapture;
    
}

char* IMP_Camera::getImage() {
    
    return NULL;
    
}

