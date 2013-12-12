//-----------------------------------------------------------------------------
// name: raka-audio.cpp
// desc: audio stuff for bokeh
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-audio.h"
#include "raka-globals.h"
#include "raka-sim.h"
#include "x-thread.h"
#include "y-fft.h"
#include "y-waveform.h"
#include "y-fluidsynth.h"
#include "y-echo.h"
#include "raka-audio2graphics.h"
#include <iostream>
#include "SoundObject.h"
#include "Binaural.h"


#include "x-fun.h"
using namespace std;



// globals
YFluidSynth * g_synth;
YEcho * g_echo;
double g_now;
double g_nextTime;
int g_prog = 0;




vector<NEBClusterSound *> g_nebSound;
vector<int> g_masterSynth;
int g_masterIndex = -1;

SoundObject g_3DObject;
Binaural g_3DBinaural(MY_BUFFER_SIZE);

// basic note struct
struct Note
{
    int channel;
    float pitch;
    float velocity;
    float duration; // in seconds
    // add more stuff?

    // constructor
    Note( int c, float p, float v, float d )
    {
        channel = c;
        pitch = p;
        velocity = v;
        duration = d;
    }
};


// HACK: vector of notes
vector<Note> g_notes;
int g_noteIndex = 0;
XMutex g_mutex;




// play some notes
void raka_playNotes( float pitch, float velocity )
{
//    // lock
//    g_mutex.acquire();
//    // clear notes
//    g_notes.clear();
//    for( int i = 0; i < 24; i++ )
//    {
//        // next notes
//        g_notes.push_back( Note( 0, pitch + i*2, (1 - i/24.0), .05 + .15*(1 - i/24.0) ) );
//    }
//    // unlock
//    g_mutex.release();
//
//    // reset the index
//    g_noteIndex = 0;
//    
//    // play now!
//    g_nextTime = g_now;
    


    
}

void playStar(int starIndex){

    g_nebSound[Globals::activeNeb]->starOn(starIndex);
}

//void toggleSynth(){
//
//    if (!Globals::synthOn){
//        g_nebSound[Globals::activeNeb]->playSynth();
//        Globals::synthOn = true;
//    } else {
//        g_nebSound[Globals::activeNeb]->pauseSynth();
//        Globals::synthOn = false;
//    }
//}

void toggleSynth(){
    
    if (!Globals::synthOn){
        //for (int i = 0; i < g_masterSynth.size(); i++){
        if (g_masterIndex > -1){
            g_nebSound[g_masterSynth[g_masterIndex]]->playSynth();
        }
        Globals::synthOn = true;
    } else {
        for (int i = 0; i < g_masterSynth.size(); i++){
            g_nebSound[g_masterSynth[i]]->pauseSynth();
        }
        Globals::synthOn = false;
    }
}

void addStarToSynth(int starIndex){
    
    g_nebSound[Globals::activeNeb]->addStarToSynth(starIndex);
}

void resetSynth(){
    
    g_nebSound[Globals::activeNeb]->resetSynth();
}

void resetMasterSynth(){
    
    g_masterSynth.clear();
    g_masterIndex = -1;
}


//-----------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{
  
    // keep track of current time in samples
//    g_now += numFrames;
    
    // HACK: rough time keeping for next notes - this logic really should be
    // somewhere else: e.g., in its own class and not directly in the audio callback!
//    if( g_now > g_nextTime )
//    {
//        // lock (to protect vector)
//        g_mutex.acquire();
//        // move down the vector
//        if( g_noteIndex < g_notes.size() )
//        {
//            // temporary note pointer
//            Note * n = &g_notes[g_noteIndex];
//            // note on!
//            g_synth->noteOn( n->channel, n->pitch, n->velocity * 120 );
//            // HACK: with a major 3rd above!
//            g_synth->noteOn( n->channel, n->pitch + 4, n->velocity * 80 );
//            // check to see next time
//            g_nextTime += n->duration * RAKA_SRATE;
//            // move to next note for next time
//            g_noteIndex++;
//        }
//        // release lock
//        g_mutex.release();
//    }
    
//    // sum
//    SAMPLE sum = 0;
//    // num channels
//    unsigned int channels = Globals::lastAudioBufferChannels;
//    
//    // zero out
//    memset( Globals::lastAudioBuffer, 0,
//           sizeof(SAMPLE)*Globals::lastAudioBufferFrames*channels );
//    memset( Globals::lastAudioBufferMono, 0,
//           sizeof(SAMPLE)*Globals::lastAudioBufferFrames );
//    
//    // copy to global buffer
//    memcpy( Globals::lastAudioBuffer, buffer,
//           sizeof(SAMPLE)*numFrames*channels );
//    
//    // copy to mono buffer
//    for( int i = 0; i < numFrames; i++ )
//    {
//        // zero out
//        sum = 0;
//        // loop over channels
//        for( int j = 0; j < channels; j++ )
//        {
//            // sum
//            sum += buffer[i*channels + j];
//        }
//        // set
//        Globals::lastAudioBufferMono[i] = sum / channels;
//    }
//    
//    // window it for taper in visuals
//    for( int i = 0; i < numFrames; i++ )
//    {
//        // multiply
//        Globals::lastAudioBufferMono[i] *= Globals::audioBufferWindow[i];
//    }
//
//    // set in the wave
//    Globals::waveform->set( Globals::lastAudioBufferMono, numFrames );
    
    // synthesize it
    //g_synth->synthesize2( buffer, numFrames );
    // echo it
    //g_echo->synthesize2( buffer, numFrames );
    
    //g_now += numFrames;
   
    
    // lock (to protect vector)
        g_mutex.acquire();
    
    float azi, ele, r;
    g_3DObject.setAzi(90);
    azi = g_3DObject.getAzi();
    ele = g_3DObject.getEle();
    r = g_3DObject.getDistance();

    
    g_3DBinaural.setPosition(r, azi, ele);
    
    for (int i = 0; i < numFrames; i++){ //numFrames = 1024

        if (Globals::binauralOn){
            

            Globals::lastAudioBufferMono[i] = g_nebSound[Globals::activeNeb]->play();
            
            if (g_masterIndex > -1){

            Globals::lastAudioBufferMono[i] = g_nebSound[g_masterSynth[g_masterIndex]]->play();
            }
         
        } else {
            buffer[2*i] = buffer[2*i+1] = g_nebSound[Globals::activeNeb]->play();
            
            if (g_masterIndex > -1 && Globals::activeNeb != g_masterSynth[g_masterIndex]){
                buffer[2*i] = buffer[2*i+1] += g_nebSound[g_masterSynth[g_masterIndex]]->play();
            }
        }

       
        g_nebSound[Globals::activeNeb]->tickStarTimer();

        
        if (g_masterIndex > -1){
           
            
               g_nebSound[g_masterSynth[g_masterIndex]]->tickSynthTimer();
            
            if (Globals::activeNeb != g_masterSynth[g_masterIndex]){
             
                 g_nebSound[g_masterSynth[g_masterIndex]]->tickStarTimer();
            }
        }
        
        
        
    }
    
    if (Globals::binauralOn){
        
        g_3DBinaural.process(Globals::lastAudioBuffer, numFrames, Globals::lastAudioBufferMono, numFrames);
        
        for (int i = 0; i < 2 * numFrames; i++){
            buffer[i] = (SAMPLE)Globals::lastAudioBuffer[i];
        }
    }
    // release lock
    g_mutex.release();
}




//-----------------------------------------------------------------------------
// name: raka_audio_init()
// desc: initialize audio system
//-----------------------------------------------------------------------------
bool raka_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels )
{
    
    // initialize
    if( !XAudioIO::init( 0, 0, srate, frameSize, channels, audio_callback, NULL ) )
    {
        // done
        return false;
    }
    
    // instantiate
    g_synth = new YFluidSynth();
    // init
    g_synth->init( srate, 32 );
    // load the soundfont
    g_synth->load( "data/sfonts/rocking8m11e.sf2", "" );
    // map program changes
    g_synth->programChange( 0, 0 );
    g_synth->programChange( 1, 79 );
    g_synth->programChange( 2, 4 );
    g_synth->programChange( 3, 10 );
    g_synth->programChange( 4, 13 );

    // allocate echo
    g_echo = new YEcho( srate );
    g_echo->setDelay( 0, .25 );
    g_echo->setDelay( 1, .5 );

//    // make a note
//    g_note = makeNote( 0, 60, .9, .5, 0 );
    
    // allocate
    Globals::lastAudioBuffer = new double[frameSize*channels];
    // allocate mono buffer
    Globals::lastAudioBufferMono = new double[frameSize];
    // allocate window buffer
    Globals::audioBufferWindow = new SAMPLE[frameSize];
    // set frame size (could have changed in XAudioIO::init())
    Globals::lastAudioBufferFrames = frameSize;
    // set num channels
    Globals::lastAudioBufferChannels = channels;
    
    // compute the window
    hanning( Globals::audioBufferWindow, frameSize );
    
    // create waveform
    Globals::waveform = new YWaveform();
    // place it
    Globals::waveform->loc.y = 1.5f;
    // set the width
    Globals::waveform->setWidth( 2.5f );
    // set the height
    Globals::waveform->setHeight( .75f );
    // initialize it
    Globals::waveform->init( frameSize );
    // active?
    Globals::waveform->active = true;
    
    // add to sim
    Globals::sim->root().addChild( Globals::waveform );
    
//    g_wvIn = new stk::FileWvIn("data/sound/ThinkinBoutYouVariation.wav");
//    g_env = new stk::Envelope();
//    g_env->setTime(6);
//    g_env->keyOn();
    
    for (int i = 0; i < Globals::numTracks; i++){
        
        NEBClusterSound *nextNeb = new NEBClusterSound(Globals::trackPaths[i], Globals::trackTitles[i], i);
        g_nebSound.push_back(nextNeb);
        g_nebSound[i]->setGrainLength(0.5 , 0.2);
        g_nebSound[i]->addStars(Globals::numStarsPerNeb);
    }
 
    g_3DObject.setObjectType(SoundObject::FIXED);


    
    return true;
}




//-----------------------------------------------------------------------------
// name: vq_audio_start()
// desc: start audio system
//-----------------------------------------------------------------------------
bool raka_audio_start()
{
    // start the audio
    if( !XAudioIO::start() )
    {
        // done
        return false;
    }
    
    return true;
}


//-----------------------------------------------------------------------------
// name: vq_audio_start()
// desc: start audio system
//-----------------------------------------------------------------------------
NEBClusterSound::NEBClusterSound(std::string trackPath, std::string trackTitle, int nebIndex){
    
    setTrackTitleA2G(trackTitle, nebIndex);
    
    wvIn = new stk::FileWvIn(trackPath);
    wvIn->setRate(1);
    m_fileLength = wvIn->getSize();
    
    env = new stk::Envelope();
    env->setTime(0.2);
    env->keyOff();
    
    m_timeNow = 0;
    m_timerOn = false;
    
    m_synthTimeNow = 0;
    m_synthTimerOn = false;
    
    //m_synthOn = false;
    
}

void NEBClusterSound::setGrainLength(float grainLengthSecs, float synthRestSecs){
    
    m_grainLength = grainLengthSecs * RAKA_SRATE;
    m_synthRest = synthRestSecs * RAKA_SRATE;
}

void NEBClusterSound::addStars(int numStars){
    
    for (int i = 0; i < numStars; i++){
        NEBStarSound *addStar = new NEBStarSound(m_fileLength, m_grainLength);
        m_stars.push_back(addStar);
        
    }
    
    m_numStars = numStars;
}



NEBStarSound::NEBStarSound(int fileLength, int grainLength){

    m_grainStart = XFun::map(((double)rand()), 0, RAND_MAX, 0, fileLength - grainLength);

}


int NEBStarSound::getGrainStart(){
    
    return m_grainStart;
}


void NEBClusterSound::starOn(int starIndex){
    
    wvIn->reset();
    wvIn->addTime(m_stars[starIndex]->getGrainStart());
    //wvIn->setRate(1);
    
    env->keyOn();
    
    startStarTimer();
    
    int nebIndex;
    
    if (g_masterIndex == -1){
        nebIndex = Globals::activeNeb;
    } else {
        nebIndex = g_masterSynth[g_masterIndex];
    }
    
    playStarA2G(starIndex, nebIndex);
}

void NEBClusterSound::starOff(){
    
   
    //wvIn->setRate(0);
    
    env->keyOff();
    
    int nebIndex;
    
    if (g_masterIndex == -1){
        nebIndex = Globals::activeNeb;
    } else {
        nebIndex = g_masterSynth[g_masterIndex];
    }
    
    stopStarA2G(nebIndex);
}

//void NEBClusterSound::playSynth(){
//    
//    //if (m_synthOn){
//    if (m_synthIndex > -1){
//
//        g_nebSound[Globals::activeNeb]->starOn(m_synth[m_synthIndex]);
//        m_synthIndex++;
//        m_synthIndex %= m_synth.size();
//        startSynthTimer();
//        
//    }
//    
//}

void NEBClusterSound::playSynth(){
    
    if (m_synthIndex > -1){
        
        if (m_synthIndex < m_synth.size()){
            
            starOn(m_synth[m_synthIndex]);
            //g_nebSound[g_masterSynth[g_masterIndex]]->starOn(m_synth[m_synthIndex]);
            m_synthIndex++;
            startSynthTimer();
            
         
            
        } else {
        
            pauseSynth();
            g_masterIndex++;
            g_masterIndex %= g_masterSynth.size();
            
            //cerr << g_masterIndex << endl;
            
            int blah = g_masterSynth[g_masterIndex];
            g_nebSound[blah]->playSynth();
        }
    
    }
  
}

void NEBClusterSound::startSynthTimer(){
    
    m_synthTimeNow = 0;
    m_synthTimerOn = true;
}

void NEBClusterSound::tickSynthTimer(){
    
    if (m_synthTimerOn){
        
        if (m_synthTimeNow > (m_grainLength + m_synthRest)){

            
            playSynth();

        }
        
        m_synthTimeNow++;
    }
}



void NEBClusterSound::pauseSynth(){
    
    m_synthTimerOn = false;
    m_synthIndex = 0;
    
}



//void NEBClusterSound::startOneStar(int starIndex){
//    
//    //DO WE NEED TO STOP OTHER STARS?
////    for (int i = 0; i < m_numStars; i++){
////        m_stars[starIndex]->starOff();
////    }
//    
//        m_stars[starIndex]->starOn();
//    
//}
//
//void NEBClusterSound::stopOneStar(int starIndex){
//    
//    m_stars[starIndex]->starOff();
//}


void NEBClusterSound::addStarToSynth(int starIndex){
   
    m_synth.push_back(starIndex);
    m_synthIndex = 0;
    g_masterIndex = 0;
    
    if (m_synth.size() == 1){
        g_masterSynth.push_back(Globals::activeNeb);
    }
    
    selectStarA2G(starIndex, g_masterSynth[g_masterIndex]);
}


void NEBClusterSound::resetSynth(){
    
    m_synth.clear();
    m_synthIndex = -1;
    
    for (int i = 0; i < g_masterSynth.size(); i++){
        if (g_masterSynth[i] == Globals::activeNeb){
            g_masterSynth.erase(g_masterSynth.begin() + i);
        }
    }

    
    deselectStarA2G(g_masterSynth[g_masterIndex]);
}




SAMPLE NEBClusterSound::play(){
    
//    SAMPLE sumSounds = 0;
//    
//    for (int i = 0; i < m_numStars; i++){
//        sumSounds += m_stars[i]->play();
//    }
//    
//    return sumSounds;
    
     return wvIn->tick() * env->tick();
    
}

void NEBClusterSound::startStarTimer(){
    
    m_timeNow = 0;
    m_timerOn = true;
}

void NEBClusterSound::tickStarTimer(){
    
    if (m_timerOn){
    
        if (m_timeNow > m_grainLength){
            starOff();
            m_timerOn = false;
        }
        
        m_timeNow++;
    }
}
