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
#include "FileWvIn.h"
#include "Envelope.h"
#include <vector>


// init audio
bool raka_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool raka_audio_start();

// play some notes
void raka_playNotes( float pitch, float velocity );
void playStar(int starIndex);
void toggleSynth();
void addStarToSynth(int starIndex);
void resetSynth();
void resetMasterSynth();


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
//    SAMPLE play();
//    void starOn();
//    void starOff();
    int getGrainStart();
    
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
    NEBClusterSound(std::string trackPath, std::string trackTitle);
    ~NEBClusterSound();
public:
    //SAMPLE playStar(int starIndex);
    void addStars(int numStars);
    void setGrainLength(float grainLengthSecs, float synthRestSecs);
    
    void starOn(int starIndex);
    void starOff();
    
    void addStarToSynth(int starIndex);
    void resetSynth();
    void playSynth();
    void pauseSynth();
    
    
    SAMPLE play();
    
    void startStarTimer();
    void tickStarTimer();
    
    void startSynthTimer();
    void tickSynthTimer();
    
    
protected:
    int m_fileLength;
    int m_grainLength;
    std::vector<NEBStarSound *> m_stars;
    int m_numStars;
    stk::FileWvIn *wvIn;
    stk::Envelope *env;
    double m_timeNow;
    bool m_timerOn;
    std::vector<int> m_synth;
    double m_synthTimeNow;
    bool m_synthTimerOn;
    int m_synthRest;
    int m_synthIndex;

    
};





#endif
