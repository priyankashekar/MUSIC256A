//
//  raka-audio2graphics.cpp
//  rakakaka
//
//  Created by Priyanka Shekar on 12/10/13.
//  Copyright (c) 2013 Ge Wang. All rights reserved.
//

#include "raka-audio2graphics.h"

Audio2Graphics *g_interface = new Audio2Graphics();

void selectStarA2G(int starIndex){
    
    g_interface->getNebSee()->selectStar(starIndex);
}

void playStarA2G(int starIndex){
    
    g_interface->getNebSee()->playStar(starIndex);
}

void initA2G(NEBClusterSee *nebSee){
    
   g_interface->setNebSee(nebSee);
}

NEBClusterSee * Audio2Graphics::getNebSee(){
   
    return m_nebSee;
}

void Audio2Graphics::setNebSee(NEBClusterSee *nebSee){
    
    m_nebSee = nebSee;
}