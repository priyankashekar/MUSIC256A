//----------------------------------------------------------------------------
// name: main.cpp
// desc: bokeh entry point
//
// author: Ge Wang (ge@ccrma.stanford.edu)
// date: fall 2013
//----------------------------------------------------------------------------
#include <iostream>
// #include "raka-audio.h"
#include "raka-gfx.h"
#include "raka-globals.h"
using namespace std;




//----------------------------------------------------------------------------
// name: main()
// desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{
    // invoke graphics setup and loop
    if( !raka_gfx_init( argc, argv ) )
    {
        // error message
        cerr << "[bokeh]: cannot initialize graphics/data system..." << endl;
        return -1;
    }
    
    // graphics loop
    raka_gfx_loop();
    
    return 0;
}