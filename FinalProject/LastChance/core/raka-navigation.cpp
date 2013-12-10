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
    Globals::lastLookTo = Globals::lookTo;
    Globals::lookTo.set(0, Globals::hemiRadius, 1);
}

void turnDown(){
    Globals::lookTo = Globals::lastLookTo;
}

void turnLeft(){
    // Globals::lookTo.set(<#GLfloat _x#>, <#GLfloat _y#>, <#GLfloat _z#>);
}

void turnRight(){
    // Globals::lookTo.set(<#GLfloat _x#>, <#GLfloat _y#>, <#GLfloat _z#>);
}