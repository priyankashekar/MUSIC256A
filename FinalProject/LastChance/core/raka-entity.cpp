//-----------------------------------------------------------------------------
// name: raka-entity.cpp
// desc: entities for bokeh visualization
//
// author: Ge Wang (ge@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "raka-entity.h"
#include "raka-globals.h"
#include "raka-sim.h"
#include "x-fun.h"
#include "rgb_hsv_hsl.hpp"



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
    
    //Globals::bgColor.updateSet(Vector3D(0, 0, 0));
    
    
    //angle = ((float)t2 / CLOCKS_PER_SEC) / (60 * 60) * M_PI * 2;
    trailAngle = M_PI;
    
}


//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void SKYhemi::render()
{
    
//    
//    // enable lighting
//    glEnable( GL_LIGHTING );
//    
//    for (int i = 0; i < sizeof(stars) / sizeof(*stars); i++){
//        DrawArc(0, 0, stars[i].radius, stars[i].startingAngle, trailAngle, 500, stars[i].yPos, stars[i].alpha, stars[i].lineWidth);
//    }
//    
//    //glColor4f( col.x, col.y, col.z, alpha );
//    //glutSolidSphere(50, 100, 100);
//    
//    glDisable( GL_LIGHTING );
}

//-----------------------------------------------------------------------------
// name: set()
// desc:
//-----------------------------------------------------------------------------
void SKYhemi::init()
{
    
    palette[0] = Globals::MidnightBlue;
    palette[1] = Globals::Black;
    palette[2] = Globals::DarkSlateBlue;
    palette[3] = Globals::DarkSlateGray;
    palette[4] = Globals::Turquoise4;
    palette[5] = Globals::RosyBrown4;
    palette[6] = Globals::HotPink4;
    palette[7] = Globals::Purple4;
    palette[8] = Globals::DarkGoldenRod4;
    palette[9] = Globals::SeaGreen4;
    palette[10] = Globals::MediumBlue;
    palette[11] = Globals::MediumOrchid4;
    palette[12] = Globals::MidnightBlue;
    palette[13] = Globals::Black;
    palette[14] = Globals::DarkSlateBlue;
    palette[15] = Globals::DarkSlateGray;
    palette[16] = Globals::Turquoise4;
    palette[17] = Globals::RosyBrown4;
    palette[18] = Globals::HotPink4;
    palette[19] = Globals::Purple4;
    palette[20] = Globals::DarkGoldenRod4;
    palette[21] = Globals::SeaGreen4;
    palette[22] = Globals::MediumBlue;
    palette[23] = Globals::MediumOrchid4;
    
    trailAngle = 0.00001;
    
    for (int i = 0; i < sizeof(stars) / sizeof(*stars); i++){
        

    
        stars[i].yPos = XFun::rand2f(0, 1) * Globals::hemiRadius;
        stars[i].startingAngle = XFun::rand2f(0, 1) * M_PI * 2;
        
        //formula from http://en.wikipedia.org/wiki/Spherical_cap
        stars[i].radius = pow(Globals::hemiRadius * 2 * (Globals::hemiRadius - stars[i].yPos) - pow(Globals::hemiRadius - stars[i].yPos, 2), 0.5);
        stars[i].alpha = XFun::rand2f(0, 1) * 0.8 + 0.2;
        stars[i].lineWidth = XFun::rand2f(0, 1) * 20 + 0.2;
    }
    
    
}

void SKYhemi::DrawArc(float cx, float cz, float r, float start_angle, float arc_angle, int num_segments,  float y, float alpha, float lineWidth)
{
	float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    
	float tangetial_factor = tanf(theta);
    
	float radial_factor = cosf(theta);
    
	
	float x = r * cosf(start_angle);//we now start at the start angle
	float z = r * sinf(start_angle);
    
    glEnable(GL_LINE_SMOOTH);
	
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glTranslatef(0, y, 0);
    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    
    glLineWidth(lineWidth);
	for(int ii = 0; ii < num_segments; ii++)
	{
        
        glColor4f(0, 0, 0, 1);
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

NEBStarSee::NEBStarSee(Vector3D location, Vector3D color, float rotation){
    

    m_color = color;
    m_fixedColor = color;
    m_location = location;
    
    m_star = new YBokeh();
    m_star->set( 1.0f, 1.0f, 1.0f, 1.0f, RAKA_TEX_FLARE_TNG_6 );
            m_star->sca.set( 4, 4, 4 );
    //        // set bokeh
    m_star->setBokehParams( // initial time
                                  20,
    //                              // freq
                                  0,
    //                              // time step
                                  5,
    //                              // location
                                  location,
    //                              // color
                                  color);
    //        // alpha
            m_star->setAlpha(1);
    m_star->setRotation(rotation);
    //        // add to simulation
            Globals::sim->root().addChild(m_star);
    
    //GLfloat time, GLfloat freq, GLfloat time_step,

 
}
//
void NEBClusterSee::selectStar(int starIndex){
    
    
    
    Vector3D curColor = m_stars[starIndex]->getColor();
    
    
    
    m_stars[starIndex]->setColor(Vector3D(1.5-curColor.x, 1.5-curColor.y, 1.5-curColor.z));

}


void NEBClusterSee::deselectStar(){
    
    Vector3D fixedColor = m_stars[0]->getFixedColor();
    
    for (int i = 0; i < m_numStars; i++){

        
        m_stars[i]->getStar()->setBokehParams( // initial time
                                                      20,
                                                      //                              // freq
                                                      0,
                                                      //                              // time step
                                                      5,
                                                      //                              // location
                                                      m_stars[i]->getLocation(),
                                                      //                              // color
                                                      fixedColor);
        
        
        
    }
    
    
    
    
}

void NEBClusterSee::playStar(int starIndex){

    m_stars[starIndex]->getStar()->setBokehParams( // initial time
                                                  20,
                                                  //                              // freq
                                                  65,
                                                  //                              // time step
                                                  5,
                                                  //                              // location
                                                  m_stars[starIndex]->getLocation(),
                                                  //                              // color
                                                  m_stars[starIndex]->getColor());

    
}

void NEBClusterSee::stopStar(){
    
    for (int i = 0; i < m_numStars; i++){
        m_stars[i]->getStar()->setBokehParams( // initial time
                                                  20,
                                                  //                              // freq
                                                  0,
                                                  //                              // time step
                                                  5,
                                                  //                              // location
                                                  m_stars[i]->getLocation(),
                                                  //                              // color
                                                  m_stars[i]->getColor());
    }
}

void NEBClusterSee::setTrackTitle(string trackTitle){
    
    m_trackTitle = trackTitle;
}

Vector3D NEBStarSee::getLocation(){
    return m_location;
}

Vector3D NEBStarSee::getColor(){
    return m_color;
}

Vector3D NEBStarSee::getFixedColor(){
    return m_fixedColor;
}

void NEBStarSee::setColor(Vector3D color){
    m_color = color;
}


YBokeh * NEBStarSee::getStar(){
    return m_star;
}

void NEBClusterSee::render()
{
 
    glColor4f(1, 1, 1, 1);
    glTranslatef(m_center.x, m_center.y, m_center.z);
 
    glScalef(10, 10, 10);
    glRasterPos3f(0, 0, 0);
    
    for (int i = 0; i < m_trackTitle.length(); i++){
        glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, m_trackTitle[i]);
    }
    
  

}

NEBClusterSee::NEBClusterSee(int numStars, Vector3D center, float spreadRadius, float rotation){
    
    m_numStars = numStars;
    m_center = center;
    NEBStarSee *addStar;
    m_starScale.set(10, 10, 0.5);
    
    Vector3D inverseColor = Vector3D(1 - Globals::bgColor.actual().x, 1 - Globals::bgColor.actual().y, 1 - Globals::bgColor.actual().z);
    
    if (m_center.x < 0){
        for (int i = 0; i < numStars; i++){
        
           
            
            addStar = new NEBStarSee(Vector3D(center.x, XFun::rand2f(center.y - spreadRadius, center.y + spreadRadius), XFun::rand2f(center.z - spreadRadius, center.z + spreadRadius)), Vector3D(0, inverseColor.y, inverseColor.z), rotation);


            m_stars.push_back(addStar);
        }

        } else if (m_center.x > 0){
            for (int i = 0; i < numStars; i++){
                
                
                
                addStar = new NEBStarSee(Vector3D(center.x, XFun::rand2f(center.y - spreadRadius, center.y + spreadRadius), XFun::rand2f(center.z - spreadRadius, center.z + spreadRadius)), Vector3D(inverseColor.x, 0, inverseColor.z), rotation);
                
                
                m_stars.push_back(addStar);
                
            }
        



    } else if (m_center.z < 0){
        
        
        for (int i = 0; i < numStars; i++){

            
            addStar = new NEBStarSee(Vector3D(XFun::rand2f(center.x - spreadRadius, center.x + spreadRadius), XFun::rand2f(center.y - spreadRadius, center.y + spreadRadius), center.z), inverseColor, rotation);
            
        
            m_stars.push_back(addStar);
        
        }
        
        
       
    } else if (m_center.z > 0){
            
            
            for (int i = 0; i < numStars; i++){
                
                
                addStar = new NEBStarSee(Vector3D(XFun::rand2f(center.x - spreadRadius, center.x + spreadRadius), XFun::rand2f(center.y - spreadRadius, center.y + spreadRadius), center.z), Vector3D(inverseColor.x, inverseColor.y, 0), rotation);
                
                
                m_stars.push_back(addStar);
                
            }
            
            
            
        }
    

  
}




int NEBClusterSee::clickStar(int xMouse, int yMouse){
    
    double xWorld;
    double yWorld;
    double zWorld;
    
    if (m_center.x < 0){
        
        recoverClick(xMouse, yMouse, -m_center.x, zWorld, yWorld);
        Vector3D starLocation = m_stars[1]->getLocation();
        
        for (int i = 0; i < m_numStars; i++){
            Vector3D starLocation = m_stars[i]->getLocation();
            
            double clickDistFromStar = (starLocation - Vector3D(m_center.x, -yWorld, zWorld)).magnitude();
            
            if (clickDistFromStar < Globals::starRadiusFudge){
                
                return i;
            }
            
        }
        
        return -1;
        
    }
    
    if (m_center.x > 0){
        
        recoverClick(xMouse, yMouse, -m_center.x, zWorld, yWorld);
               Vector3D starLocation = m_stars[1]->getLocation();
        
        for (int i = 0; i < m_numStars; i++){
            Vector3D starLocation = m_stars[i]->getLocation();
            
            double clickDistFromStar = (starLocation - Vector3D(m_center.x, yWorld, zWorld)).magnitude();
            
            if (clickDistFromStar < Globals::starRadiusFudge){
                
                return i;
            }
            
        }
        
        return -1;
        
    }
    
    if (m_center.z < 0){
        
        recoverClick(xMouse, yMouse, m_center.z, xWorld, yWorld);
               Vector3D starLocation = m_stars[1]->getLocation();
        
        for (int i = 0; i < m_numStars; i++){
            Vector3D starLocation = m_stars[i]->getLocation();
            
            double clickDistFromStar = (starLocation - Vector3D(xWorld, yWorld, m_center.z)).magnitude();
            
            if (clickDistFromStar < Globals::starRadiusFudge){
                
                return i;
            }
            
        }
        
        return -1;
    }
    
    if (m_center.z > 0){
        
        recoverClick(xMouse, yMouse, -m_center.z, xWorld, yWorld);
               Vector3D starLocation = m_stars[1]->getLocation();
        
        for (int i = 0; i < m_numStars; i++){
            Vector3D starLocation = m_stars[i]->getLocation();
            
            double clickDistFromStar = (starLocation - Vector3D(-xWorld, yWorld, m_center.z)).magnitude();
            
            if (clickDistFromStar < Globals::starRadiusFudge){
                
                return i;
            }
            
        }
        
        return -1;
    }
    
    return -1;
}

void recoverClick(int iX, int iY, double z_distance, double &oX, double &oY){
    // http://www.3dbuzz.com/forum/threads/191296-OpenGL-gluUnProject-ScreenCoords-to-WorldCoords-problem
    GLdouble posX1, posY1, posZ1, posX2, posY2, posZ2, modelview[16], projection[16];
    GLint viewport[4];
    
    // Get matrices
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Create ray
    gluUnProject(iX, viewport[1] + viewport[3] - iY, 0, modelview, projection, viewport, &posX1, &posY1, &posZ1);  // Near plane
    gluUnProject(iX, viewport[1] + viewport[3] - iY, 1, modelview, projection, viewport, &posX2, &posY2, &posZ2);  // Far plane
    
    
    GLfloat t = (posZ1 - z_distance) / (posZ1 - posZ2);
    
    // so here are the desired (x, y) coordinates
    oX = (posX1 + (posX2 - posX1) * t);
    oY = (posY1 + (posY2 - posY1) * t);
}

void INTROScreen::render(){
    
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );

    glColor4f( 0.0f, 0.0f, 0.0f, Globals::fadeInAlpha.x);
    glutSolidCube(100);

    glDisable(GL_BLEND);
}