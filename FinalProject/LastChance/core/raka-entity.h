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
#include <string>

struct SKYtrail{
    float yPos;
    float radius;
    float startingAngle;
    float alpha;
    float lineWidth;
};

void recoverClick(int iX, int iY, double z_distance, double &oX, double &oY);


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


//-----------------------------------------------------------------------------
// name: class SKYsphere
// desc: for testing
//-----------------------------------------------------------------------------
class SKYhemi : public YEntity
{
public:
    // update
    void update(YTimeInterval dt);
    // render
    void render();
    void init();
    
protected:
    void DrawArc(float cx, float cz, float r, float start_angle, float arc_angle, int num_segments, float y, float alpha, float lineWidth);
    
protected:
    Vector3D palette[24];
    GLfloat trailAngle;
    SKYtrail stars[10];
    
    
};


class NEBStarSee
{
public:
    NEBStarSee(Vector3D location, Vector3D color, float rotation);
    ~NEBStarSee();
public:
    void update(YTimeInterval dt);
    void render();
    Vector3D getLocation();
    Vector3D getColor();
    void setColor(Vector3D color);
 
    YBokeh *getStar();
protected:
    Vector3D m_location;
    Vector3D m_color;
    YBokeh *m_star;
 

    
};

class NEBClusterSee : public YEntity
{
public:
    NEBClusterSee(int numStars, Vector3D center, float spreadRadius, float rotation);
    ~NEBClusterSee();
    int clickStar(int xMouse, int yMouse);
    void selectStar(int starIndex);
    void playStar(int starIndex);
    void stopStar();
    void setTrackTitle(std::string trackTitle);
    void render();
protected:
    Vector3D m_center;
    int m_numStars;
    std::vector<NEBStarSee *> m_stars;
    std::string m_trackTitle;
    Vector3D m_starScale;
    
};

class INTROScreen : public YEntity
{
public:

    void render();
};


#endif









