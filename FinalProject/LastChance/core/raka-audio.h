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
#include <vector>


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
    void starOn();
    void starOff();
    
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
    //SAMPLE playStar(int starIndex);
    void addStars(int numStars);
    void setGrainLength(int grainLengthSecs);
    
    void startOneStar(int starIndex);
    void stopOneStar(int starIndex);
    
    void startStepSynth();
    void stopStepSynth();
    
    
    SAMPLE play();
    
    
protected:
    int m_fileLength;
    int m_grainLength;
    std::vector<NEBStarSound *> m_stars;
    int m_numStars;
    
};





#endif
