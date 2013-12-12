//-----------------------------------------------------------------------------
// name: raka-globals.h
// desc: global stuff for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __RAKA_GLOBALS_H__
#define __RAKA_GLOBALS_H__


#include "x-def.h"
#include "x-audio.h"
#include "x-gfx.h"
#include "x-vector3d.h"
#include "y-waveform.h"

// c++
#include <string>
#include <map>
#include <vector>
#include <utility>

// defines
#define RAKA_SRATE        44100
#define RAKA_FRAMESIZE    1024
#define RAKA_NUMCHANNELS  2
#define RAKA_MAX_TEXTURES 32




//-----------------------------------------------------------------------------
// name: enum BokehTextureNames
// desc: texture names
//-----------------------------------------------------------------------------
enum BokehTextureNames
{
    RAKA_TEX_NONE = 0,
    RAKA_TEX_FLARE_TNG_1,
    RAKA_TEX_FLARE_TNG_2,
    RAKA_TEX_FLARE_TNG_3,
    RAKA_TEX_FLARE_TNG_4,
    RAKA_TEX_FLARE_TNG_5,
    RAKA_TEX_JELLY_BODY
};


enum starMode {
    PLAY_STAR = 0,
    SELECT_STAR = 1,
};



// forward reference
class RAKASim;




//-----------------------------------------------------------------------------
// name: class Globals
// desc: the global class
//-----------------------------------------------------------------------------
class Globals
{
public:
    // top level root simulation
    static RAKASim * sim;
    
    // path
    static std::string path;
    // path to datapath
    static std::string relpath;
    // datapath
    static std::string datapath;
    // version
    static std::string version;
    
    // last audio buffer
    static double * lastAudioBuffer;
    static double * lastAudioBufferMono;
    static SAMPLE * audioBufferWindow;
    static unsigned int lastAudioBufferFrames;
    static unsigned int lastAudioBufferChannels;
    
    // width and height of the window
    static GLsizei windowWidth;
    static GLsizei windowHeight;
    static GLsizei lastWindowWidth;
    static GLsizei lastWindowHeight;
    
    // waveform
    static YWaveform * waveform;
    
    // graphics fullscreen
    static GLboolean fullscreen;
    // blend pane instead of clearing screen
    static GLboolean blendScreen;
    // blend screen parameters
    static Vector3D blendAlpha;
    static GLfloat blendRed;
    // fill mode
    static GLenum fillmode;
    // background color
    static iSlew3D bgColor;
    // view stuff
    static Vector3D viewRadius;
    static Vector3D viewEyeY;
    
    static iSlew3D lookFrom;
    static iSlew3D lookTo;
    static const Vector3D upVector;
    static Vector3D lastLookTo;
    
    static int numTracks;
    static GLfloat viewAngle;
    
    static Vector3D fadeInAlpha;
    
    static Vector3D fov;
    
    // textures
    static GLuint textures[];
    
    // light 0 position
    static GLfloat light0_pos[4];
    // light 1 parameters
    static GLfloat light1_ambient[4];
    static GLfloat light1_diffuse[4];
    static GLfloat light1_specular[4];
    static GLfloat light1_pos[4];
    // line width
    static GLfloat linewidth;
    // do
    static GLboolean fog;
    static GLuint fog_mode[4];   // storage for three/four types of fog
    static GLuint fog_filter;    // which fog to use
    static GLfloat fog_density;  // fog density
    
    // colors
    static Vector3D ourWhite;
    static Vector3D ourRed;
    static Vector3D ourBlue;
    static Vector3D ourOrange;
    static Vector3D ourGreen;
    static Vector3D ourGray;
    static Vector3D ourYellow;
    static Vector3D ourSoftYellow;
    static Vector3D ourPurple;
    
    //color chart from http://gucky.uni-muenster.de/cgi-bin/rgbtab-en
    static Vector3D MidnightBlue;
    static Vector3D Black;
    static Vector3D DarkSlateBlue;
    static Vector3D DarkSlateGray;
    static Vector3D Turquoise4;
    static Vector3D RosyBrown4;
    static Vector3D HotPink4;
    static Vector3D Purple4;
    static Vector3D DarkGoldenRod4;
    static Vector3D SeaGreen4;
    static Vector3D MediumBlue;
    static Vector3D MediumOrchid4;
    
    static float starRadiusFudge;
    
    static float hemiRadius;
    static float spreadRadius;

    static bool tabHold;
    static bool synthOn;
    static bool binauralOn;
    
    static int activeNeb;
    static int numStarsPerNeb;
    

};




#endif
