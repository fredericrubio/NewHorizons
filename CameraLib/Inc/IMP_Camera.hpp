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

class IMP_Camera {
public:
    IMP_Camera();
    IMP_Camera(const IMP_Camera& orig);
    virtual ~IMP_Camera();
private:
    RaspiCam* raspiCam;
    
};

#endif /* IMP_CAMERA_HPP */

