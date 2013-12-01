//-----------------------------------------------------------------------------
// name: raka-gfx.cpp
// desc: graphics stuff for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-gfx.h"
#include "raka-globals.h"
#include "raka-sim.h"
#include "raka-audio.h"

#include "x-fun.h"
#include "x-gfx.h"
#include "x-loadlum.h"
#include "x-vector3d.h"

#include <iostream>
#include <vector>
using namespace std;


// jellyfish
RAKAJellyFish * g_jellyfish = NULL;

// bokehs
vector<YBokeh *> g_bokehs;

SKYhemi *g_sky;


//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void idleFunc();
void displayFunc();
void reshapeFunc( int width, int height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void specialFunc( int key, int x, int y );

void initialize_graphics();
void initialize_simulation();
bool initialize_data();
void loadTextures();
bool checkTexDim( int dim );
void setupTexture( const char * filename, GLuint texobj,
                  GLenum minFilter, GLenum maxFilter,
                  int useMipMaps = 0 );

void renderBackground();
void blendPane();
void updateNodeEntities();
void renderNodeEntities();




//-----------------------------------------------------------------------------
// name: raka_gfx_init( )
// desc: graphics/interaction entry point
//-----------------------------------------------------------------------------
bool raka_gfx_init( int argc, const char ** argv )
{
#ifdef __APPLE__
    // save working dir
    char * cwd = getcwd( NULL, 0 );
    // set path
    // #ifdef __VQ_RELEASE__
#if 1
    // HACK: use this hard-coded path
    Globals::path = "./";
#else
    // store this globally
    Globals::path = cwd;
#endif
    // compute the datapath
    Globals::datapath = Globals::path + Globals::relpath;
#endif
    
    // initialize GLUT
    glutInit( &argc, (char **)argv );
    
#ifdef __APPLE__
    //restore working dir
    chdir( cwd );
    free( cwd );
#endif
    
    // print about
    raka_about();
    raka_endline();
    
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( Globals::windowWidth, Globals::windowHeight );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow( "RAKAKAKAKAKAKA!!!!!");
    // full screen
    if( Globals::fullscreen )
        glutFullScreen();
    
    // set the idle function - called when idle
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    // for arrow keys, etc
	glutSpecialFunc (specialFunc );
    
    // do our own initialization
    initialize_graphics();
    // simulation
    initialize_simulation();
    // do data
    if( !initialize_data() )
    {
        // done
        return false;
    }
    
    // print keys
    // raka_endline();
    // raka_keys();
    // raka_line();
    // raka_endline();
    
    return true;
}




//-----------------------------------------------------------------------------
// name: raka_gfx_loop( )
// desc: hand off to graphics loop
//-----------------------------------------------------------------------------
void raka_gfx_loop()
{
    // let GLUT handle the current thread from here
    glutMainLoop();
}




//-----------------------------------------------------------------------------
// Name: initialize_graphics( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize_graphics()
{
    // log
    cerr << "[bokeh]: initializing graphics system..." << endl;
    
    // reset time
    XGfx::resetCurrentTime();
    // set simulation speed
    XGfx::setDeltaFactor( 1.0f );
    // get the first
    XGfx::getCurrentTime( true );
    // random
    XFun::srand();
    
    // set the GL clear color - use when the color buffer is cleared
    glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
    // set the shading model to 'smooth'
    glShadeModel( GL_SMOOTH );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    // set the front faces of polygons
    glFrontFace( GL_CCW );
    // set fill mode
    glPolygonMode( GL_FRONT_AND_BACK, Globals::fillmode );
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable lighting for front
    glLightModeli( GL_FRONT_AND_BACK, GL_TRUE );
    // material have diffuse and ambient lighting
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    // enable color
    glEnable( GL_COLOR_MATERIAL );
    // normalize (for scaling)
    glEnable( GL_NORMALIZE );
    // line width
    glLineWidth( Globals::linewidth );
    
    // enable light 0
    glEnable( GL_LIGHT0 );
    
    // setup and enable light 1
    glLightfv( GL_LIGHT1, GL_AMBIENT, Globals::light1_ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, Globals::light1_diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, Globals::light1_specular );
    glEnable( GL_LIGHT1 );
    
    // load textures
    loadTextures();
    
    // fog
    Globals::fog_mode[0] = 0;
    Globals::fog_mode[1] = GL_LINEAR;
    // fog_mode[1] = GL_EXP; fog_mode[2] = GL_EXP2;
    Globals::fog_filter = 0;
    Globals::fog_density = .04f;
    
    // fog color
    GLfloat fogColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};
    // fog mode
    if( Globals::fog_filter ) glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
    // set fog color
    glFogfv( GL_FOG_COLOR, fogColor );
    // fog density
    glFogf( GL_FOG_DENSITY, Globals::fog_density );
    // fog hint
    glHint( GL_FOG_HINT, GL_DONT_CARE );
    // fog start depth
    glFogf( GL_FOG_START, 1.0f );
    // fog end depth
    glFogf( GL_FOG_END, 10.5f );
    // enable
    if( Globals::fog_filter ) glEnable( GL_FOG );
    
    // check global flag
    if( Globals::fog )
    {
        // fog mode
        glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
        // enable
        glEnable(GL_FOG);
    }
    else
    {
        // disable
        glDisable(GL_FOG);
    }
    
    // clear the color buffer once
    glClear( GL_COLOR_BUFFER_BIT );
}




//-----------------------------------------------------------------------------
// name: initialize_simulation( )
// desc: simulation setup
//-----------------------------------------------------------------------------
void initialize_simulation()
{
    // instantiate simulation
    Globals::sim = new RAKASim();
    
//    // create test teapot
//    RAKATeapot * teapot = new RAKATeapot();
//    // set attributes
//    teapot->col = Globals::ourSoftYellow;
//    teapot->loc.z = -10;
//    // add to simulation
//    Globals::sim->root().addChild( teapot );
//    
//    // create test teapot
//    teapot = new RAKATeapot();
//    // set attributes
//    teapot->col = Globals::ourBlue;
//    teapot->loc.set( -2, 2, -5 );
//    // add to simulation
//    Globals::sim->root().addChild( teapot );
//    
//    // create test teapot
//    teapot = new RAKATeapot();
//    // set attributes
//    teapot->col = Globals::ourGreen;
//    teapot->loc.set( 0, 0, 0 );
//    teapot->sca.set( .2, .2, .2 );
//    // add to simulation
//    Globals::sim->root().addChild( teapot );

//    // create a spark
//    g_jellyfish = new RAKAJellyFish();
//    // set attributes
//    g_jellyfish->set( RAKA_TEX_JELLY_BODY, 1.0f, 1.0f );
//    // add to simulation
//    Globals::sim->root().addChild( g_jellyfish );
//
//    // create a spark
//    spark = new RAKASpark();
//    // set attributes
//    spark->set( RAKA_TEX_FLARE_TNG_1, .5f, 1.0f );
//    spark->loc.set( .1, 0, 0 );
//    // add to simulation
//    Globals::sim->root().addChild( spark );
    
//    // bokeh
//    YBokeh * bokeh = new YBokeh();
//    bokeh->set( 1, 1, 1, 1, RAKA_TEX_FLARE_TNG_5 );
//    bokeh->setBokehParams(0, 1, 10, Vector3D(0,0,0), Vector3D(1, 1, .5 ) );
//    Globals::sim->root().addChild( bokeh );
    
//    for( int i = 0; i < 2000; i++ )
//    {
//        // create a spark
//        YBokeh * bokeh = new YBokeh();
//        // set attributes
//        bokeh->set( 1.0f, 1.0f, 1.0f, 1.0f, RAKA_TEX_FLARE_TNG_5 );
//        bokeh->sca.set( 1, 1, 1 );
//        // set bokeh
//        bokeh->setBokehParams( // initial time
//                              XFun::rand2f(0,10),
//                              // freq
//                              XFun::rand2f(1,3),
//                              // time step
//                              50,
//                              // location
//                              Vector3D(XFun::rand2f(-2,2),XFun::rand2f(-1,1), XFun::rand2f(-1,1)),
//                              // color
//                              Vector3D(XFun::rand2f(0,.1),XFun::rand2f(0,.2), XFun::rand2f(0,.1)) );
//        // alpha
//        bokeh->setAlpha( 1 );
//        // add to simulation
//        Globals::sim->root().addChild( bokeh );
//        g_bokehs.push_back( bokeh );
//    }
    
    g_sky = new SKYhemi();
    g_sky->init();
    Globals::sim->root().addChild(g_sky);
}




//-----------------------------------------------------------------------------
// name: initialize_data( )
// desc: load initial data
//-----------------------------------------------------------------------------
bool initialize_data()
{
    // maybe read in MIDI files and such here...

    // done
    return true;
}




//-----------------------------------------------------------------------------
// name: raka_about()
// desc: ...
//-----------------------------------------------------------------------------
void raka_about()
{
    raka_line();
    fprintf( stderr, "[bokeh]: Bokeh Madness\n" );
    raka_line();
    fprintf( stderr, "   | by Ge Wang\n" );
    fprintf( stderr, "   | Stanford University | CCRMA\n" );
    fprintf( stderr, "   | http://ccrma.stanford.edu/~ge/\n" );
    fprintf( stderr, "   | version: %s\n", Globals::version.c_str() );
}




//-----------------------------------------------------------------------------
// name: raka_keys()
// desc: ...
//-----------------------------------------------------------------------------
void raka_keys()
{
    raka_line();
    fprintf( stderr, "[bokeh]: run-time control\n" );
    raka_line();
    fprintf( stderr, "  'h' - print this help message\n" );
    fprintf( stderr, "  's' - toggle fullscreen\n" );
    fprintf( stderr, "  'b' - toggle animated trails\n" );
    fprintf( stderr, "  'f' - toggle fog rendering\n" );
    fprintf( stderr, "  '[' and ']' - rotate automaton\n" );
    fprintf( stderr, "  '-' and '+' - zoom away/closer to center of automaton\n" );
    fprintf( stderr, "  'n' and 'm' - adjust amount of blending\n" );
    fprintf( stderr, "  '<' and '>' - adjust fog density\n" );
    fprintf( stderr, "  ''' - bg: white\n" );
    fprintf( stderr, "  ';' - bg: black\n" );
    fprintf( stderr, "  ',' - bg: blue\n" );
    fprintf( stderr, "  '.' - bg: gray\n" );
    fprintf( stderr, "  'q' - quit\n" );
}




//-----------------------------------------------------------------------------
// name: raka_help()
// desc: ...
//-----------------------------------------------------------------------------
void raka_help()
{
    raka_endline();
    raka_keys();
    raka_endline();
}




//-----------------------------------------------------------------------------
// name: raka_usage()
// desc: ...
//-----------------------------------------------------------------------------
void raka_usage()
{
    raka_line();
    fprintf( stderr, "[bokeh]: command line arguments\n" );
    raka_line();
    fprintf( stderr, "usage: bokeh --[options] [name]\n" );
    fprintf( stderr, "   [options] = help | fullscreen" );
}




//-----------------------------------------------------------------------------
// name: raka_endline()
// desc: ...
//-----------------------------------------------------------------------------
void raka_endline()
{
    fprintf( stderr, "\n" );
}




//-----------------------------------------------------------------------------
// name: raka_line()
// desc: ...
//-----------------------------------------------------------------------------
void raka_line()
{
    fprintf( stderr, "---------------------------------------------------------\n" );
}




//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( int w, int h )
{
    // save the new window size
    Globals::windowWidth = w; Globals::windowHeight = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
}




//-----------------------------------------------------------------------------
// Name: look( )
// Desc: ...
//-----------------------------------------------------------------------------
void look( )
{
    // go
    Globals::fov.interp( XGfx::delta() );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( Globals::fov.value, (GLfloat)Globals::windowWidth / (GLfloat)Globals::windowHeight, .005, 500.0 );
    //glOrtho(-Globals::windowWidth/(double)Globals::windowHeight, Globals::windowWidth/(double)Globals::windowHeight, -1, 1, 1, -1 );
    
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity();
    // position the view point
    gluLookAt( 0.0f,
              Globals::viewRadius.x * sin( Globals::viewEyeY.x ),
              Globals::viewRadius.x * cos( Globals::viewEyeY.x ),
              0.0f, 0.0f, 0.0f,
              0.0f, ( cos( Globals::viewEyeY.x ) < 0 ? -1.0f : 1.0f ), 0.0f );
    
    // set the position of the lights
    glLightfv( GL_LIGHT0, GL_POSITION, Globals::light0_pos );
    glLightfv( GL_LIGHT1, GL_POSITION, Globals::light1_pos );
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    // system keys (handled first)
    switch( key )
    {
        case 'q':
        {
            exit( 0 );
            break;
        }
        case '1':
            g_jellyfish->bloat( 5, 1 );
            break;
            
        case '2':
            g_jellyfish->shrink( 1, 1 );
            break;
        case 'r':
        {
            for( int i = 0; i < g_bokehs.size(); i++ )
            {
                YBokeh * bokeh = g_bokehs[i];
                bokeh->iLoc.update( Vector3D(XFun::rand2f(-2,2),XFun::rand2f(-1,1), XFun::rand2f(-1,1) ) );
                bokeh->iRGB.update( Vector3D(XFun::rand2f(0,.3),XFun::rand2f(0,.3), XFun::rand2f(0,.3)) );
            }
            break;
        }
        case 'a':
        {
            raka_endline();
            raka_about();
            raka_endline();
            break;
        }
        case 'b':
        {
            static GLfloat blendAlpha = 0.15f;
            Globals::blendScreen = !Globals::blendScreen;
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal = blendAlpha;
                Globals::blendAlpha.slew = .5f;
            }
            else
            {
                blendAlpha = Globals::blendAlpha.goal;
                Globals::blendAlpha.goal = 1;
            }
            fprintf( stderr, "[bokeh]: blendscreen:%s\n", Globals::blendScreen ? "ON" : "OFF" );
            break;
        }
        case 'f':
        {
            Globals::fog = !Globals::fog;
            fprintf( stderr, "[bokeh]: fog:%s\n", Globals::fog ? "ON" : "OFF" );
            if( Globals::fog )
            {
                // fog mode
                glFogi( GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter] );
                // enable
                glEnable( GL_FOG );
            }
            else
            {
                // disable
                glDisable( GL_FOG );
            }
            break;
        }
        case 'h':
        {
            raka_help();
            break;
        }
        case 'm':
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal -= .005;
                if( Globals::blendAlpha.goal < 0 ) Globals::blendAlpha.goal = 0;
            }
            break;
        case 'n':
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal += .01;
                if( Globals::blendAlpha.goal > 1 ) Globals::blendAlpha.goal = 1;
            }
            break;
        case 'M':
        case 'N':
            if( Globals::blendScreen )
            {
                Globals::blendAlpha.goal = .15f;
            }
            break;
        case 's':
        {
            if( !Globals::fullscreen )
            {
                Globals::lastWindowWidth = Globals::windowWidth;
                Globals::lastWindowHeight = Globals::windowHeight;
                glutFullScreen();
            }
            else
                glutReshapeWindow( Globals::lastWindowWidth, Globals::lastWindowHeight );
            
            Globals::fullscreen = !Globals::fullscreen;
            fprintf( stderr, "[bokeh]: fullscreen:%s\n", Globals::fullscreen ? "ON" : "OFF" );
            break;
        }
        case '<':
            Globals::fog_density *= .95f;
            fprintf( stderr, "[bokeh]: fog density:%f\n", Globals::fog_density );
            glFogf(GL_FOG_DENSITY, Globals::fog_density);
            break;
        case '>':
            Globals::fog_density *= 1.05f;
            fprintf( stderr, "[bokeh]: fog density:%f\n", Globals::fog_density );
            glFogf(GL_FOG_DENSITY, Globals::fog_density);
            break;
            
            //        case 'A':
            //        case 'B':
            //        case 'C':
            //        case 'D':
            //        case 'E':
            //        case 'F':
            //        case 'G':
            //        case 'H':
            //        case 'I':
            //        case 'J':
            //            Globals::core->noteOn( Globals::selectedVoice, key - 'A' + 48 );
            //            break;
            //
            //        case '6':
            //        case '7':
            //        case '8':
            //        case '9':
            //            Globals::selectedVoice = key - '6';
            //            break;
            
            //        case 'k':
            //            for( int i = 0; i < Globals::hud->histoVoice().numBins(); i++ )
            //                Globals::hud->histoVoice().bin(i)->setValue( XFun::rand2f(.1,.9) );
            //            for( int i = 0; i < Globals::hud->histoPitch().numBins(); i++ )
            //                Globals::hud->histoPitch().bin(i)->setValue( XFun::rand2f(.1,.9) );
            //            break;
            
            //        case 'V':
            //            Globals::skore->start();
            //            break;
    }
    
    // check if something else is handling viewing
    bool handled = false;
    
    // post visualizer handling (if not handled)
    if( !handled )
    {
        switch( key )
        {
            case ']':
                Globals::viewEyeY.y -= .1f;
                //fprintf( stderr, "[vismule]: yview:%f\n", g_eye_y.y );
                break;
            case '[':
                Globals::viewEyeY.y += .1f;
                //fprintf( stderr, "[vismule]: yview:%f\n", g_eye_y.y );
                break;
            case '=':
                Globals::viewRadius.y = .975 * Globals::viewRadius.y;
                if( Globals::viewRadius.y < .001 ) Globals::viewRadius.y = .001;
                // fprintf( stderr, "[vismule]: view radius:%f->%f\n", Globals::viewRadius.x, Globals::viewRadius.y );
                break;
            case '-':
                Globals::viewRadius.y = 1.025 * Globals::viewRadius.y;
                // fprintf( stderr, "[vismule]: view radius:%f->%f\n", Globals::viewRadius.x, Globals::viewRadius.y );
                break;
            case '_':
            case '+':
                Globals::viewRadius.y = Globals::viewRadius.x + .7*(Globals::viewRadius.y-Globals::viewRadius.x);
                break;
            case '\'':
                Globals::bgColor.update( Vector3D( 1,1,1 ) );
                break;
            case ';':
                Globals::bgColor.update( Vector3D( 0,0,0 ) );
                break;
            case '.':
                Globals::bgColor.update( Vector3D( .5f, .5f, .5f ) );
                break;
            case ',':
                Globals::bgColor.update( Vector3D( .5f, .75f, 1.0f ) );
                break;
        }
    }
    
    // do a reshape since viewEyeY might have changed
    reshapeFunc( Globals::windowWidth, Globals::windowHeight );
    // post redisplay
    glutPostRedisplay( );
}



void addBokeh( float x, float y )
{
    // create a spark
    YBokeh * bokeh = new YBokeh();
    // set attributes
    bokeh->set( 1.0f, 1.0f, 1.0f, 1.0f, RAKA_TEX_JELLY_BODY );
    bokeh->sca.set( 1, 1, 1 );
    // set bokeh
    bokeh->setBokehParams( // initial time
                          XFun::rand2f(0,10),
                          // freq
                          XFun::rand2f(.1,.2),
                          // time step
                          50,
                          // location
                          Vector3D( x, y, 0 ), // Vector3D(XFun::rand2f(-1,1),XFun::rand2f(-1,1), XFun::rand2f(-1,1)),
                          // color
                          Vector3D(XFun::rand2f(0,.7),XFun::rand2f(0,1), XFun::rand2f(0,.7)) );
    // alpha
    bokeh->setAlpha( 1 );
    // add to simulation
    Globals::sim->root().addChild( bokeh );
    g_bokehs.push_back( bokeh );
}




//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    switch( button )
    {
        // left mouse button
        case 0:
            // down
            if( state == 0 )
            {
                float pitch = 48 + (x/1280.0)*48;
                float velocity = .5+y/2000.0;
                
                float x1 = (x - Globals::windowWidth/2.0) / Globals::windowWidth * 7;
                float y1 = (-y + Globals::windowHeight/2.0) / Globals::windowHeight * 7;

                cerr << "pitch: " << pitch << " velocity: " << velocity
                     << " x: " << x << " x1: " << x1 << " y " << y << " y1: " << y1 << endl;

                addBokeh( x1, y1 );
                raka_playNotes( pitch, velocity );
            }
            break;
    }
    // glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: specialFunc( )
// Desc: handles arrow stuff
//-----------------------------------------------------------------------------
void specialFunc( int key, int x, int y )
{
    // check
    bool handled = false;
    
    // if not handled
    if( !handled )
    {
        switch( key )
        {
            case GLUT_KEY_LEFT:
                break;
            case GLUT_KEY_RIGHT:
                break;
            case GLUT_KEY_UP:
                break;
            case GLUT_KEY_DOWN:
                break;
        }
    }
}




//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
    // update time
    XGfx::getCurrentTime( TRUE );
    
    // update
    Globals::bgColor.interp( XGfx::delta() );
    Globals::blendAlpha.interp( XGfx::delta() );
    
    // clear or blend
    if( Globals::blendScreen && Globals::blendAlpha.value > .0001 )
    {
        // clear the depth buffer
        glClear( GL_DEPTH_BUFFER_BIT );
        // blend screen
        blendPane();
    }
    else
    {
        // set the GL clear color - use when the color buffer is cleared
        glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
        // clear the color and depth buffers
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }
    
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    
    // save state
    glPushMatrix();
    
    // slew
    Globals::viewEyeY.interp( XGfx::delta());
    Globals::viewRadius.interp( XGfx::delta() );
    look();
    
    // cascade simulation
    Globals::sim->systemCascade();
    
    // pop state
    glPopMatrix();
    
    //    // draw any HUD here
    //    Globals::hud->project();
    //    Globals::hud->updateAll( Globals::sim->delta() );
    //    Globals::hud->drawAll();
    
    // flush gl commands
    glFlush();
    // swap the buffers
    glutSwapBuffers();
}




//-----------------------------------------------------------------------------
// name: blendPane()
// desc: blends a pane into the current scene
//-----------------------------------------------------------------------------
void blendPane()
{
    // enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // disable lighting
    glDisable( GL_LIGHTING );
    // disable depth test
    glDisable( GL_DEPTH_TEST );
    // blend in a polygon
    glColor4f( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, Globals::blendAlpha.value );
    // glColor4f( Globals::blendRed, Globals::blendRed, Globals::blendRed, Globals::blendAlpha );
    // reduce the red component
    // Globals::blendRed -= .02f;
    // if( Globals::blendRed < 0.0f ) Globals::blendRed = 0.0f;
    
    GLfloat h = 10;
    GLfloat d = -1;
    
    // draw the polyg
    glBegin( GL_QUADS );
    glVertex3f( -h, -h, d );
    glVertex3f( h, -h, d );
    glVertex3f( h, h, d );
    glVertex3f( -h, h, d );
    glEnd();
    
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    // disable blending
    glDisable( GL_BLEND );
}




//-----------------------------------------------------------------------------
// name: renderBackground()
// desc: ...
//-----------------------------------------------------------------------------
void renderBackground()
{
    // save the current matrix
    glPushMatrix( );
    
    // restore
    glPopMatrix( );
}




//-------------------------------------------------------------------------------
// name: loadTexture()
// desc: load textures
//-------------------------------------------------------------------------------
void loadTextures()
{
    char filename[256];
    GLenum minFilter, maxFilter;
    int i;
    
    // log
    fprintf( stderr, "[bokeh]: loading textures...\n" );
    
    // set store alignment
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    
    // set filter types
    minFilter = GL_LINEAR;
    maxFilter = GL_LINEAR;
    
    // load tng flares
    for( i = RAKA_TEX_FLARE_TNG_1; i <= RAKA_TEX_FLARE_TNG_5; i++ )
    {
        glGenTextures( 1, &(Globals::textures[i]) );
        sprintf( filename, "%sflare-tng-%d.bw", Globals::datapath.c_str(), i - RAKA_TEX_FLARE_TNG_1 + 1 );
        setupTexture( filename, Globals::textures[i], minFilter, maxFilter );
    }
    
    glGenTextures( 1, &(Globals::textures[RAKA_TEX_JELLY_BODY]) );
    sprintf( filename, "%sjelly-body-1.bw", Globals::datapath.c_str() );
    setupTexture( filename, Globals::textures[RAKA_TEX_JELLY_BODY], minFilter, maxFilter );
}




//-------------------------------------------------------------------------------
// name: raka_loadTexture()
// desc: load texture
//-------------------------------------------------------------------------------
XTexture * raka_loadTexture( const string & filename )
{
    // instantiate image data
    XTexture * tex = new XTexture;
    
    // load the texture
    if( !raka_initTexture( filename, tex ) )
    {
        delete tex;
        return NULL;
    }
    
    return tex;
}




//-------------------------------------------------------------------------------
// name: raka_loadTexture()
// desc: load texture
//-------------------------------------------------------------------------------
bool raka_initTexture( const string & filename, XTexture * tex )
{
    // set desired resize
    tex->resizeWidth = 512;
    tex->resizeHeight = 512;
    
    // generate the texture
    glGenTextures( 1, &tex->name );
    // bind the texture
    glBindTexture( GL_TEXTURE_2D, tex->name );
    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    // setting parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    // load the texture
    if( !XGfx::loadTexture( filename, tex ) )
    {
        cerr << "[bokeh]: error - cannot load texture '" << filename.c_str() << "'..." << endl;
        return false;
    }
    
    return true;
}




//--------------------------------------------------------------------------------
// name: setupTexture()
// desc: ...
//--------------------------------------------------------------------------------
void setupTexture( const char * filename, GLuint texobj,
                  GLenum minFilter, GLenum maxFilter, int useMipMaps )
{
    unsigned char * buf = NULL;
    int width = 0, height = 0, components = 0;
    
    glBindTexture( GL_TEXTURE_2D, texobj );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter );
    
    // log
    // fprintf( stderr, "[bokeh]: loading %s...\n", filename );
    
    // load luminance
    buf = loadLuminance( filename, &width, &height, &components );
    
    // log
    // fprintf( stderr, "[bokeh]: '%s' : %dx%dx%d\n", filename, width, height, components);
    
    // build mip maps
    if( useMipMaps )
    {
        gluBuild2DMipmaps( GL_TEXTURE_2D, 1, width, height,
                          GL_LUMINANCE, GL_UNSIGNED_BYTE, buf );
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, 1, width, height, 0,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, buf );
    }
    
    free(buf);
}




//-----------------------------------------------------------------------------
// name: checkTexDim( )
// desc: checks to see if a dim is a valid opengl texture dimension
//-----------------------------------------------------------------------------
bool checkTexDim( int dim )
{
    if( dim < 0 )
        return false;
    
    int i, count = 0;
    
    // count bits
    for( i = 0; i < 31; i++ )
        if( dim & ( 0x1 << i ) )
            count++;
    
    // this is true only if dim is power of 2
    return count == 1;
}
