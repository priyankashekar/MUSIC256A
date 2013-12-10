#ifndef __SOUNDOBJECT_H_
#define __SOUNDOBJECT_H_

class SoundObject {
 public:
  SoundObject();
  ~SoundObject();

  float getXpos(void);
  float getYpos(void);
  float getZpos(void);
  float getAzi(void);
  float getEle(void);
  float getDistance(void);
  void setAzi(float azi);
  void setEle(float ele);

  void setMovingType(int type);
  
  void setObjectType(int objectType);
  int getObjectType();

  // discrete movement in FIXED mode
  void moveClockwise();
  void moveCounterClockwise();
  void moveUp();
  void moveDown();

  void resetPos(void);

  void updatePos(void);
  void setSpeed(int speed);
  
  enum {
    CIRCLE,
    ELLIPSE,
    FIXED,
    RANDOM_ATTACK,
    RANDOM
  };

 private:
  float rand2f( float a, float b );
 
  float m_x, m_y, m_z;
  float m_theta;
  float m_azi_vel;
  float m_azi_vel_org;
  float m_azi;
  float m_ele;
  float m_ele_vel;
  float m_ele_vel_org;

  float m_distance;
  int m_movingType;

  // degree of angle inc/dec for FIXED mode
  float m_moveStep;
};


#endif
