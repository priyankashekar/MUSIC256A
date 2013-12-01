//-----------------------------------------------------------------------------
// name: raka-entity.cpp
// desc: entities for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-entity.h"
#include "raka-globals.h"
#include "x-fun.h"
#include "rgb_hsv_hsl.hpp"

#define RAND_FLOAT ((float)rand()) / RAND_MAX

using namespace std;



// texture coordinates
static const GLshort g_coord[ ] = { 0, 0, 1, 0, 0, 1, 1, 1 };




//-----------------------------------------------------------------------------
// name: set()
// desc: start with this
//-----------------------------------------------------------------------------
void RAKAJellyFish::set( int _texture, float _size, float _alpha )
{
    setTexture( _texture );
    setSize( _size );
    alpha = _alpha;
}




//-----------------------------------------------------------------------------
// name: setSize()
// desc: set size
//-----------------------------------------------------------------------------
void RAKAJellyFish::setSize( float _size )
{
    _size *= 0.5f;
    vertices[0] = -_size; vertices[1] = -_size;
    vertices[2] = _size; vertices[3] = -_size;
    vertices[6] = _size; vertices[7] = _size;
    vertices[4] = -_size; vertices[5] = _size;
}




//-------------------------------------------------------------------------------
// name: setTexture()
// desc: ...
//-------------------------------------------------------------------------------
void RAKAJellyFish::setTexture( int _texture )
{
    // set it
    texture = _texture;
}

void RAKAJellyFish::bloat( float target, float slew )
{
    // setting the goal of the interpolator to the target value
    xScale.update( target, slew );
    ALPHA.update( 1, slew );
}

void RAKAJellyFish::shrink( float target, float slew )
{
    // setting the goal of the interpolator to the target value
    xScale.update( target, slew );
    ALPHA.update( 0, slew );
}


//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void RAKAJellyFish::update( YTimeInterval dt )
{
    // slew
    ALPHA.interp( dt );
    xScale.interp( dt );

    // set
    this->alpha = ALPHA.value;
    this->sca.x = xScale.value;
}




//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void RAKAJellyFish::render( )
{
    // disable depth
    glDisable( GL_DEPTH_TEST );
    // enable texture
    glEnable( GL_TEXTURE_2D );
    // set blend function
    glBlendFunc( GL_ONE, GL_ONE );
    // glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // enable blend
    glEnable( GL_BLEND );
    
    // bind to texture
    glBindTexture( GL_TEXTURE_2D, Globals::textures[this->texture] );
    
    // set color
    glColor4f( col.x, col.y, col.z, alpha * ALPHA.value );
    
    // set vertex coordinates
    glVertexPointer( 2, GL_FLOAT, 0, this->vertices );
    glEnableClientState( GL_VERTEX_ARRAY );
    
    // set texture coordinates
    glTexCoordPointer( 2, GL_SHORT, 0, g_coord );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    // draw stuff!
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    
    // disable texture
    glDisable( GL_TEXTURE_2D );
    // disable blend
    glDisable( GL_BLEND );
    
    // disable client states
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    
    glEnable( GL_LIGHTING );
    glEnable( GL_DEPTH_TEST );
    // HACK: legs
    glTranslatef( 0, -.2, 0 );
    glutSolidCube( .2 );
    glDisable( GL_LIGHTING );
}




//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void RAKATeapot::update( YTimeInterval dt )
{
    // do nothing for now
}




//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void RAKATeapot::render()
{
    // enable lighting
    glEnable( GL_LIGHTING );
    // set color
    glColor4f( col.x, col.y, col.z, alpha );
    // render stuff
    glutSolidTeapot( 1.0 );
    // disable lighting
    glDisable( GL_LIGHTING );
}

//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void SKYhemi::update(YTimeInterval dt)
{
    
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    clock_t t2 = clock();
    
    int prevColorIndex = now->tm_hour;
    int secsAfterHour = now->tm_min * 60 + now->tm_sec;
    
    Vector3D interpHSL;
    Vector3D prevHSL;
    Vector3D nextHSL;
    int interpR;
    int interpG;
    int interpB;
    
    rgb_to_hsl(palette[prevColorIndex].x * 255, palette[prevColorIndex].y * 255, palette[prevColorIndex].z * 255, prevHSL.x, prevHSL.y, prevHSL.z);
    rgb_to_hsl(palette[(prevColorIndex+1) % 24].x * 255, palette[(prevColorIndex+1) % 24].y * 255, palette[(prevColorIndex+1) % 24].z * 255, nextHSL.x, nextHSL.y, nextHSL.z);
    
    interpHSL = prevHSL + (nextHSL - prevHSL) * ((GLfloat)secsAfterHour / (60 * 60));

    hsl_to_rgb( interpHSL.x, interpHSL.y, interpHSL.z, interpR, interpG, interpB);
    
    Globals::bgColor.updateSet(Vector3D(interpR / 255.0, interpG / 255.0, interpB / 255.0));
    
    //angle = ((float)t2 / CLOCKS_PER_SEC) / (60 * 60) * M_PI * 2;
    trailAngle = M_PI;
    
}


//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void SKYhemi::render()
{
    
    
    // enable lighting
    glEnable( GL_LIGHTING );
    
    DrawArc(0, 0, 10, M_PI_2, trailAngle, 100, 1, 1);
    //glColor4f( col.x, col.y, col.z, alpha );
    //glutSolidSphere(50, 100, 100);
    
    glDisable( GL_LIGHTING );
}

//-----------------------------------------------------------------------------
// name: set()
// desc:
//-----------------------------------------------------------------------------
void SKYhemi::init()
{
    
    palette[0] = Globals::MidnightBlue;
    palette[1] = Globals::Plum2;
    palette[2] = Globals::LightSteelBlue3;
    palette[3] = Globals::MediumPurple3;
    palette[4] = Globals::Lavender;
    palette[5] = Globals::Turquoise;
    palette[6] = Globals::RosyBrown;
    palette[7] = Globals::Wheat;
    palette[8] = Globals::LemonChiffon1;
    palette[9] = Globals::Honeydew3;
    palette[10] = Globals::Sienna1;
    palette[11] = Globals::DarkSlateBlue;
    palette[12] = Globals::MidnightBlue;
    palette[13] = Globals::Plum2;
    palette[14] = Globals::LightSteelBlue3;
    palette[15] = Globals::MediumPurple3;
    palette[16] = Globals::Lavender;
    palette[17] = Globals::Turquoise;
    palette[18] = Globals::RosyBrown;
    palette[19] = Globals::Wheat;
    palette[20] = Globals::LemonChiffon1;
    palette[21] = Globals::Honeydew3;
    palette[22] = Globals::Sienna1;
    palette[23] = Globals::DarkSlateBlue;
    
    trailAngle = 0.00001;
    
    for (int i = 0; i < sizeof(stars) / sizeof(*stars); i++){
        stars[0].yPos = RAND_FLOAT * Globals::hemiRadius;
        stars[0].startingAngle = RAND_FLOAT * M_PI * 2;
        stars[0].alpha =  RAND_FLOAT * 0.8 + 0.2;
        stars[0].lineWidth = RAND_FLOAT * 2 + 0.2;
    }
    
    
}

void SKYhemi::DrawArc(float cx, float cz, float r, float start_angle, float arc_angle, int num_segments,  float alpha, float lineWidth)
{
	float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    
	float tangetial_factor = tanf(theta);
    
	float radial_factor = cosf(theta);
    
	
	float x = r * cosf(start_angle);//we now start at the start angle
	float z = r * sinf(start_angle);
    
    glEnable(GL_LINE_SMOOTH);
	
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTranslatef(0, 10, 0);
    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    
    glLineWidth(lineWidth);
	for(int ii = 0; ii < num_segments; ii++)
	{
        
        glColor4f(1, 1, 1, alpha);
		glVertex3f(x + cx, 0, z + cz);
        
		float tx = -z;
		float tz = x;
        
		x += tx * tangetial_factor;
		z += tz * tangetial_factor;
        
		x *= radial_factor;
		z *= radial_factor;
	}
	glEnd();
    
    glDisable(GL_LINE_SMOOTH);
    glDisable( GL_BLEND );
}


