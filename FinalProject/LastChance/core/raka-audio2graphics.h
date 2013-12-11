//
//  raka-audio2graphics.h
//  rakakaka
//
//  Created by Priyanka Shekar on 12/10/13.
//  Copyright (c) 2013 Ge Wang. All rights reserved.
//

#ifndef __rakakaka__raka_audio2graphics__
#define __rakakaka__raka_audio2graphics__

#include <iostream>
#include "raka-entity.h"
#include <vector>

#endif /* defined(__rakakaka__raka_audio2graphics__) */

void selectStarA2G(int starIndex);
void playStarA2G(int starIndex);
void initA2G(std::vector<NEBClusterSee *> nebSee);

class Audio2Graphics
{
public:
    NEBClusterSee * getNebSee();
    void setNebSee(NEBClusterSee *nebSee);
protected:
    NEBClusterSee *m_nebSee;
};