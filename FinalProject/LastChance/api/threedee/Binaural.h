
#include <string.h>

#include "global.h"
#include "DelayL.h"
#include "Delay.h"

#define NUMS_FILTER_COEF 128
#define ELE_MAX 40
#define ELE_MIN -40
#define ELE_INC 10
#define AZI_INC 15
#define AZI_STEPS 24
#define ELE_STEPS 9

class Binaural 
{
 public:
  Binaural(int bufferSize);
  ~Binaural();
  
  void setPosition( float distance, float azimuth, float elevation);
  void process(SAMPLE *output, unsigned int outputSize, SAMPLE *input, unsigned int inputSize);
  
  void enableOverlapAdd(bool enable);


  int selectSubject(int subject);

 private:
  // Head-Related Impulse Response 
  void updateHRTF();

  // pointer to HRTF
  float *HRIR_left_ptr;
  float *HRIR_right_ptr;
  SAMPLE HRIR_left[NUMS_FILTER_COEF];
  SAMPLE HRIR_right[NUMS_FILTER_COEF];

  // buffer
  SAMPLE * m_leftDelayOut;  
  SAMPLE * m_rightDelayOut;
  SAMPLE * m_leftOut;
  SAMPLE * m_rightOut;

  // Delayline
  stk::DelayL m_TV_delayLine;
stk::Delay m_delayLine;

  // parameters
  float m_gain;
  float m_azimuth;
  float m_elevation;
  float m_prevITD;
  float m_ITD;

  // enable overlapAdd
  bool m_overlapAdd;

  // buffer size
  int m_bufferSize;
  static int m_defaultDelay;

  enum HRTF_SUBJECT {
    HRTF_JUHAN,
    HRTF_MIRIAM
  };


};

