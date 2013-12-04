//-----------------------------------------------------------------------------
// name: raka-audio.h
// desc: audio stuff
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __RAKA_AUDIO_H__
#define __RAKA_AUDIO_H__

#include "x-audio.h"



// init audio
bool raka_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool raka_audio_start();

// play some notes
void raka_playNotes( float pitch, float velocity );


//-----------------------------------------------------------------------------
// name: class NEBStarSound
// desc: for testing
//-----------------------------------------------------------------------------
class NEBStarSound
{
    
public:
    NEBStarSound(int fileLength, int grainLength);
    ~NEBStarSound();
    
public:
    SAMPLE play();
    
protected:
    int m_grainStart;

    
};


//-----------------------------------------------------------------------------
// name: class NEBClusterSound
// desc: for testing
//-----------------------------------------------------------------------------
class NEBClusterSound
{
public:
    NEBClusterSound();
    ~NEBClusterSound();
public:
    SAMPLE playStar(int starIndex);
    void addStars();
    void setGrainLength(int grainLength);
protected:
    int m_fileLength;
    int m_grainLength;
    NEBStarSound **m_stars;
    
};





#endif
