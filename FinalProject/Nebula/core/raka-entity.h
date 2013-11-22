//-----------------------------------------------------------------------------
// name: raka-entity.h
// desc: entities for visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __RAKA_ENTITY_H__
#define __RAKA_ENTITY_H__

#include "y-entity.h"
#include "x-buffer.h"
#include <vector>




//-----------------------------------------------------------------------------
// name: class RAKAJellyFish
// desc: ...
//-----------------------------------------------------------------------------
class RAKAJellyFish : public YEntity
{
public:
    // constructor
    RAKAJellyFish() : texture(0), ALPHA( 1, 1, 1 ), xScale(1,1,1) { }
    
public:
    // set
    void set( int _texture, float _size, float _alpha );
    void setTexture( int _texture );
    void setSize( float _size );
    
public:
    void bloat( float target, float slew = 1 );
    void shrink( float target, float slew = 1 );
    
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();
    
public:
    // which ripple texture
    GLuint texture;
    // vertices
    GLfloat vertices[8];
    // alpha ramp
    Vector3D ALPHA;
    // alpha
    Vector3D xScale;
};




//-----------------------------------------------------------------------------
// name: class RAKATeapot
// desc: for testing
//-----------------------------------------------------------------------------
class RAKATeapot : public YEntity
{
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();
};





#endif














