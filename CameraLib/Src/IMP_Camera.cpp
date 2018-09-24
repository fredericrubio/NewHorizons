/* 
 * File:   IMP_Camera.cpp
 * Author: fredericrubio
 * 
 * Created on 21 septembre 2018, 21:23
 */

#include "IMP_Camera.hpp"

#include "NHO_LOG.hpp"

IMP_Camera::IMP_Camera() {
    
    raspCam = new raspicam::RaspiCam();
    
}

IMP_Camera::~IMP_Camera() {
    
//    if (raspCam != NULL) {
//        delete raspCam;
//    }
    
}


bool IMP_Camera::open() {
    
    if (raspCam->open()) {
        NHO_FILE_LOG(logDEBUG) << "IMP_Camera::open: OK" << std::endl;
        return true;
    }
    
    NHO_FILE_LOG(logERROR) << "IMP_Camera::open: Error opening camera" << std::endl;
    return false;
    
}

/*
 * Indicates if camera is opened
 */
bool IMP_Camera::isOpened() const {

//    if (raspCam == NULL) {
//        NHO_FILE_LOG(logDEBUG) << "IMP_Camera::isOpened: Camera not initialized" << std::endl;
//        return false;
//    }
    
    return  raspCam->isOpened();
    
}

bool IMP_Camera::captureImage() {
    
    bool lCapture = raspCam->grab();
    
#ifdef _DEBUG
    NHO_FILE_LOG(logDEBUG) << "IMP_Camera::captureImage: " << lCapture << std::endl;
#endif
    
    return lCapture;
    
}

unsigned char* IMP_Camera::getImage(unsigned int* const pSize) {

//    if (raspCam == NULL) {
//        NHO_FILE_LOG(logERROR) << "IMP_Camera::getImage: Camera not initialized" << std::endl;
//        return NULL;
//    }
    
    unsigned char* lData = NULL;
    // get the image
    *pSize = raspCam->getImageTypeSize( raspicam::RASPICAM_FORMAT_RGB );
    lData = new unsigned char[*pSize];
    
    //extract the image in rgb format
    raspCam->retrieve(lData, raspicam::RASPICAM_FORMAT_RGB );

    return lData;
    
}

