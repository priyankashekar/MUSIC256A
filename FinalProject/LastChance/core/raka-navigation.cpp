//
//  raka-navigation.cpp
//  rakakaka
//
//  Created by Priyanka Shekar on 12/4/13.
//  Copyright (c) 2013 Ge Wang. All rights reserved.
//

#include "raka-navigation.h"
#include "raka-globals.h"

void turnUp(){
    Globals::lastLookTo = Globals::lookTo.actual();
    Globals::lookTo.update(Vector3D(0, Globals::hemiRadius, 1)); //NOTE 1 HERE!
}

void turnDown(){
    Globals::lookTo.update(Globals::lastLookTo);
}

void turnLeft(){
    
    Globals::viewAngle -= (2 * M_PI) / Globals::numTracks;
    Globals::lookTo.update(Vector3D(Globals::hemiRadius * sin(Globals::viewAngle), 0, Globals::hemiRadius * -cos(Globals::viewAngle)));
    
    Globals::activeNeb += (Globals::numTracks - 1);
    Globals::activeNeb %= Globals::numTracks;
}

void turnRight(){
    
    Globals::viewAngle += (2 * M_PI) / Globals::numTracks;
    Globals::lookTo.update(Vector3D(Globals::hemiRadius * sin(Globals::viewAngle), 0, Globals::hemiRadius * -cos(Globals::viewAngle)));
    
    Globals::activeNeb++;
    Globals::activeNeb %= Globals::numTracks;
}
