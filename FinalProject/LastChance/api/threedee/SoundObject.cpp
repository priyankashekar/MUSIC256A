#include "SoundObject.h"
#include "global.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


SoundObject::SoundObject() 
{
  m_x = m_y = m_z = 0.0;
  m_theta = 0.0;

  m_azi_vel = 0;
  m_azi = 0.0;
  m_azi_vel_org = 0;

  m_ele = 0;
  m_ele_vel = 0;
  m_ele_vel_org = 0;

  m_distance = 1.0;

  m_movingType = FIXED;

  // degree of angle inc/dec for FIXED mode
  m_moveStep = 2.0;

  resetPos();
}

SoundObject::~SoundObject()
{

}

int SoundObject::getObjectType()
{
  return m_movingType;
}


void SoundObject::setObjectType(int objectType)
{  
  m_movingType = objectType;
  resetPos();

  switch (m_movingType) {
  case CIRCLE:
    fprintf(stderr," soundObject::circle!\n");
    break;
  case ELLIPSE:
    fprintf(stderr," soundObject::ellipse!\n");
    break;
  case FIXED:
    fprintf(stderr," soundObject::fixed!\n");
    break;
  }

}

float SoundObject::getXpos() 
{
  return m_x;
}

float SoundObject::getYpos()
{
  return m_y;
}

float SoundObject::getZpos()
{
  return m_z;
}

float SoundObject::getAzi()
{
  return m_azi;
}

float SoundObject::getEle()
{
  return m_ele;
}

float SoundObject::getDistance()
{
  return m_distance;
}

void SoundObject::updatePos()
{
  switch (m_movingType) {
  case ELLIPSE:
    setAzi(m_azi -  m_azi_vel);
    m_x = 0.4*sin(m_azi*MY_PIE/180);
    m_z = -25*cos(m_azi*MY_PIE/180);
    m_y = 0;
    m_distance = sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
    break;
  case CIRCLE:
    setAzi(m_azi +  m_azi_vel);
    break;
  case RANDOM:
    setAzi(m_azi + rand2f(-4,2));
    setEle(m_ele + rand2f(-4,4));
    break;
  }

  if ( (m_movingType == CIRCLE) || (m_movingType == FIXED) || (m_movingType == RANDOM) ) {
    m_x = m_distance*cos(m_ele*MY_PIE/180)*sin(m_azi*MY_PIE/180);
    m_y = m_distance*sin(m_ele*MY_PIE/180);
    m_z = -m_distance*cos(m_ele*MY_PIE/180)*cos(m_azi*MY_PIE/180);    
  }

  // fprintf(stderr, "azi = %f, ele = %f, r = %f,  x_pos = %f, y_pos = %f, z_pos = %f\n", 
  //  m_azi, m_ele, m_distance, m_x, m_y, m_z);

}



//-----------------------------------------------------------------------------
// name: rand2f()
// desc: generate a random float
//-----------------------------------------------------------------------------
float SoundObject::rand2f( float a, float b )
{
    return a + (b-a)*(rand() / (float)RAND_MAX);
}

//-----------------------------------------------------------------------------
// name: resetPos()
// desc: set object to initial positions
//-----------------------------------------------------------------------------
void SoundObject::resetPos()
{
  switch (m_movingType) {
  case CIRCLE:
    m_azi = 0;
    m_ele = 0.0;
    m_azi_vel = 1.0f;
    m_azi_vel_org = m_azi_vel;
    m_x = sin(m_azi*MY_PIE/180);
    m_z = cos(m_azi*MY_PIE/180);
    m_y = 0;
    m_distance = 0.0;
    break;
  case ELLIPSE:
    m_azi = 0.0f;
    m_azi_vel = 0.30f;
    m_azi_vel_org = m_azi_vel_org;
    m_ele = 0.0f;
    m_x = 0.4*sin(m_azi*MY_PIE/180);
    m_z = -25*cos(m_azi*MY_PIE/180);
    m_y = 0;
    m_distance = 0.0;
    break;
  case FIXED:
    m_distance = 1.0f;
    m_azi = 0.0;  m_ele = 0.0f;
    m_x = 0.0f;  m_y = 0.0f;
    m_z = -m_distance;
    break;
  case RANDOM_ATTACK:    
    break;
  case RANDOM:
    m_distance = 1.0f;
    m_azi = 0.0;
    m_ele = 0.0f;
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = m_distance;
    break;
  }
  
  m_distance = sqrt(m_x*m_x + m_y*m_y + m_z*m_z);

  //fprintf(stderr, "%f %f %f\n", m_x, m_y, m_z);
  
}

void SoundObject::setSpeed(int speed)
{
  if ( (m_movingType == FIXED ) || ( m_movingType == CIRCLE) ){
    m_moveStep = speed*2.0;
  }

  if ( (m_movingType == ELLIPSE ) || ( m_movingType == CIRCLE) ){
    if ( ( speed > 0 ) && ( speed < 10 ))
      m_azi_vel = m_azi_vel_org * speed;
  }
}


void SoundObject::setAzi(float azi)
{
  if ( azi < -180 ) 
    azi += 360;
  else if ( azi >= 180 )
    azi -= 360;

  m_azi = azi;
  //fprintf(stderr, "SoundObject::azimith set to %f\n", azi);
}

void SoundObject::setEle(float ele)
{
  if ( (ele >= -40) && (ele <= 40) ) {    
    m_ele = ele;
    //fprintf(stderr, "SoundObject::elevation set to %f\n", ele);
  }
}



void SoundObject::moveClockwise()
{
  setAzi(m_azi + m_moveStep);
}

void SoundObject::moveCounterClockwise()
{
  setAzi(m_azi - m_moveStep);
}

void SoundObject::moveUp()
{
  setEle(m_ele + m_moveStep);
}

void SoundObject::moveDown()
{
  setEle(m_ele - m_moveStep);
}


