//
//  raka-audio2graphics.cpp
//  rakakaka
//
//  Created by Priyanka Shekar on 12/10/13.
//  Copyright (c) 2013 Ge Wang. All rights reserved.
//

#include "raka-audio2graphics.h"
#include "raka-globals.h"


std::vector<Audio2Graphics *> g_interface;

void selectStarA2G(int starIndex, int nebIndex){
    
    g_interface[nebIndex]->getNebSee()->selectStar(starIndex);
}

void deselectStarA2G(int nebIndex){
 
    g_interface[nebIndex]->getNebSee()->deselectStar();
}

void playStarA2G(int starIndex, int nebIndex){
    
    g_interface[nebIndex]->getNebSee()->playStar(starIndex);
}

void stopStarA2G(int nebIndex){
    
     g_interface[nebIndex]->getNebSee()->stopStar();
}

void initA2G(std::vector<NEBClusterSee *> nebSee){
    
    for (int i = 0; i < Globals::numTracks; i++){
        
        Audio2Graphics *nextInterface = new Audio2Graphics();
        g_interface.push_back(nextInterface);
        g_interface[i]->setNebSee(nebSee[i]);
    }
    
}

void setTrackTitleA2G(std::string trackTitle){
    
    g_interface[Globals::activeNeb]->getNebSee()->setTrackTitle(trackTitle);
}

NEBClusterSee * Audio2Graphics::getNebSee(){
   
    return m_nebSee;
}

void Audio2Graphics::setNebSee(NEBClusterSee *nebSee){
    
    m_nebSee = nebSee;
}