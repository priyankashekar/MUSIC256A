//-----------------------------------------------------------------------------
// name: raka-globals.h
// desc: global stuff for visquin visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-globals.h"


// defaults
#define DEFAULT_FULLSCREEN    FALSE
#define DEFAULT_WINDOW_WIDTH  1280
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_BLENDSCREEN   FALSE
#define DEFAULT_FOG           FALSE
#define DEFAULT_VERSION       "1.0.0"

RAKASim * Globals::sim = NULL;

GLsizei Globals::windowWidth = DEFAULT_WINDOW_WIDTH;
GLsizei Globals::windowHeight = DEFAULT_WINDOW_HEIGHT;
GLsizei Globals::lastWindowWidth = Globals::windowWidth;
GLsizei Globals::lastWindowHeight = Globals::windowHeight;

double * Globals::lastAudioBuffer = NULL;
double * Globals::lastAudioBufferMono = NULL;
SAMPLE * Globals::audioBufferWindow = NULL;
unsigned int Globals::lastAudioBufferFrames = 0;
unsigned int Globals::lastAudioBufferChannels = 0;
YWaveform * Globals::waveform = NULL;

GLboolean Globals::fullscreen = DEFAULT_FULLSCREEN;
GLboolean Globals::blendScreen = DEFAULT_BLENDSCREEN;

Vector3D Globals::blendAlpha( 1, 1, .5f );
GLfloat Globals::blendRed = 0.0f;
GLenum Globals::fillmode = GL_FILL;
iSlew3D Globals::bgColor( .5 );
Vector3D Globals::viewRadius( 5, 2, 1 );
Vector3D Globals::viewEyeY( 2, 0, 1.5f );
Vector3D Globals::fov( 80, 100, .2f );

Vector3D Globals::lookFrom(0, 0, 0);
iSlew3D Globals::lookTo(0, 0, -hemiRadius);
const Vector3D Globals::upVector(0, 1, 0);
Vector3D Globals::lastLookTo(0, 0, 0);

int Globals::numTracks = 4;
GLfloat Globals::viewAngle = 0;

GLuint Globals::textures[RAKA_MAX_TEXTURES];

GLfloat Globals::light0_pos[4] = { 2.0f, 1.2f, 4.0f, 1.0f };
GLfloat Globals::light1_ambient[4] = { .2f, .2f, .2f, 1.0f };
GLfloat Globals::light1_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Globals::light1_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat Globals::light1_pos[4] = { -2.0f, 0.0f, -4.0f, 1.0f };
GLfloat Globals::linewidth = 1.0f;
GLboolean Globals::fog = DEFAULT_FOG;
GLuint Globals::fog_mode[4];
GLuint Globals::fog_filter;
GLfloat Globals::fog_density;

// colors
Vector3D Globals::ourWhite( 1, 1, 1 );
Vector3D Globals::ourRed( 1, .5, .5 );
Vector3D Globals::ourBlue( 102.0f/255, 204.0f/255, 1.0f );
Vector3D Globals::ourOrange( 1, .75, .25 );
Vector3D Globals::ourGreen( .7, 1, .45 );
Vector3D Globals::ourGray( .4, .4, .4 );
Vector3D Globals::ourYellow( 1, 1, .25 );
Vector3D Globals::ourSoftYellow( .7, .7, .1 );
Vector3D Globals::ourPurple( .6, .25, .6 );

std::string Globals::path = "";
std::string Globals::relpath = "data/texture/";
std::string Globals::datapath = "";
std::string Globals::version = DEFAULT_VERSION;

Vector3D Globals::LightSteelBlue3(162.0/255, 181.0/255, 205.0/255);
Vector3D Globals::MediumPurple3(137.0/255, 104.0/255, 205.0/255);
Vector3D Globals::MidnightBlue(25.0/255, 25.0/255, 112.0/255);
Vector3D Globals::Plum2(238.0/255, 174.0/255, 238.0/255);
Vector3D Globals::Lavender(230.0/255, 230.0/255, 230.0/255);
Vector3D Globals::Turquoise(64.0/255, 224.0/255, 208.0/255);
Vector3D Globals::RosyBrown(188.0/255, 143.0/255, 143.0/255);
Vector3D Globals::Wheat(245.0/255, 222.0/255, 179.0/255);
Vector3D Globals::LemonChiffon1(255.0/255, 250.0/255, 205.0/255);
Vector3D Globals::Honeydew3(193.0/255, 205.0/255, 193.0/255);
Vector3D Globals::Sienna1(255.0/255, 130.0/255, 71.0/255);
Vector3D Globals::DarkSlateBlue(72.0/255, 61.0/255, 139.0/255);

float Globals::starRadiusFudge = 5;

float Globals::hemiRadius = 50;
float Globals::spreadRadius = 40;

bool Globals::tabHold = false;
bool Globals::synthOn = false;

int Globals::activeNeb = 0;

int Globals::numStarsPerNeb = 2;

