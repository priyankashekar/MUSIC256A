#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Binaural.h"

using namespace std;

// constant delay for input
int Binaural::m_defaultDelay = 50;

// filter table
extern float ITD_s1[216];
extern float hrir_diff_l_s1[216][128];
extern float hrir_diff_r_s1[216][128];

extern void convolve_fft( SAMPLE_2 * f, int fsize, SAMPLE_2 * g, int gsize, SAMPLE_2 * buffy, int size );


Binaural::Binaural(int bufferSize) 
{
  m_gain = 0.0;

  // set initial direction
  m_azimuth = 0.0f;  m_elevation = 0.0f;
  
  // set initial HRIR table and ITD
  updateHRTF();

  // set ITD
  m_ITD = 0; m_prevITD;

  // enable overlapAdd 
  m_overlapAdd = true;

  m_bufferSize = bufferSize;

  m_leftDelayOut = new SAMPLE_2[bufferSize];
  m_rightDelayOut = new SAMPLE_2[bufferSize];
  m_leftOut = new SAMPLE_2[2 * bufferSize];
  m_rightOut = new SAMPLE_2[2 * bufferSize]; 

  m_delayLine.setDelay(m_defaultDelay);
  m_TV_delayLine.setDelay(m_defaultDelay);
}

Binaural::~Binaural()
{
  delete [] m_leftDelayOut;
  delete [] m_rightDelayOut;
  delete [] m_leftOut; 
  delete [] m_rightOut; 
}

void Binaural::enableOverlapAdd(bool enable)
{
  m_overlapAdd = enable;
}

// TODO 
int Binaural::selectSubject(int subject)
{
  switch ( subject ) {
  case HRTF_JUHAN:    
    break;
  default:  
    break;
  }
    
    return 0; //HACK
}

void Binaural::setPosition( float distance, float azimuth, float elevation)
{
  // update filter only if angluar position has changed
  if ( ( m_azimuth != azimuth ) || ( m_elevation != elevation ) ) {

    // set new angles
    m_azimuth = azimuth;
    if ( elevation > ELE_MAX ) 
      m_elevation = ELE_MAX;
    else if ( elevation < ELE_MIN ) 
      m_elevation = ELE_MIN;
    else
      m_elevation = elevation;

    // interpolate HRTF for new direction
    updateHRTF();
  }


  // meter to gain
  float gain;
  if ( distance > 0.25 ) {
    gain = 1.0/distance;
  }
  else {
    gain = 4.0;
  }

  if ( gain != m_gain ) {
    m_gain = gain;
    updateHRTF();
  }

  //fprintf(stderr,"azi = %f, ele = %f, gain = %f\n", m_azimuth, m_elevation, m_gain);
}

void Binaural::updateHRTF()
{
  int aziLower, aziHigher;
  int eleLower, eleHigher;
  float aziWeight, eleWeight;

  // find lower and higher bounds of the nearest measured HRIR
  aziLower = floor((m_azimuth + 180.0)/ AZI_INC); 
  aziHigher = ( aziLower + 1 ) % AZI_STEPS;
  aziWeight = fmod((m_azimuth + 180.0),AZI_INC)/AZI_INC;

#ifdef DEBUG
  printf("aziLower = %d\n", aziLower);
  printf("aziHiger = %d\n", aziHigher);
  printf("aziWeight = %f\n", aziWeight);
#endif

  if ( m_elevation < ELE_MAX ) {
    eleLower = floor((m_elevation - ELE_MIN) / ELE_INC) * AZI_STEPS; 
    eleHigher = eleLower + AZI_STEPS;
    eleWeight = fmod((m_elevation - ELE_MIN), ELE_INC) / ELE_INC;
  }
  else {
    // when elevation is max, there is no higher elevation angle.
    // So, decrease lower bound and set low weight to 1
    eleLower = floor((m_elevation - ELE_MIN - 1) / ELE_INC) * AZI_STEPS ; 
    eleHigher = eleLower + AZI_STEPS;
    eleWeight = 1.0;
  }

#ifdef DEBUG
  printf("eleLower = %d\n", eleLower);
  printf("eleHiger = %d\n", eleHigher);
  printf("eleWeight = %f\n", eleWeight);
#endif

  // linear interpolation of filter table
  for ( int i = 1; i < NUMS_FILTER_COEF; i++ ) {
    HRIR_left[i] = m_gain * 
      (hrir_diff_l_s1[eleLower+aziLower][i]*(1.0-aziWeight)*(1.0-eleWeight) + 
       hrir_diff_l_s1[eleLower+aziHigher][i]*aziWeight*(1.0-eleWeight) + 
       hrir_diff_l_s1[eleHigher+aziLower][i]*(1.0-aziWeight)*eleWeight + 
       hrir_diff_l_s1[eleHigher+aziHigher][i]*aziWeight*eleWeight);

    HRIR_right[i] = m_gain * 
      (hrir_diff_r_s1[eleLower+aziLower][i]*(1.0-aziWeight)*(1.0-eleWeight) + 
       hrir_diff_r_s1[eleLower+aziHigher][i]*aziWeight*(1.0-eleWeight) + 
       hrir_diff_r_s1[eleHigher+aziLower][i]*(1.0-aziWeight)*eleWeight + 
       hrir_diff_r_s1[eleHigher+aziHigher][i]*aziWeight*eleWeight);
  }  

  // update ITD
  m_prevITD = m_ITD;
  m_ITD = ITD_s1[eleLower+aziLower]*(1.0-aziWeight)*(1.0-eleWeight) + 
    ITD_s1[eleLower+aziHigher]*aziWeight*(1.0-eleWeight) + 
    ITD_s1[eleHigher+aziLower]*(1.0-aziWeight)*eleWeight + 
    ITD_s1[eleHigher+aziHigher]*aziWeight*eleWeight;

#ifdef DEBUG
  fprintf(stderr, "ITD = %f\n", m_ITD);
  fprintf(stderr, "m_gain = %f\n", m_gain);
#endif

}

void Binaural::process(SAMPLE_2 *output, unsigned int outputSize, SAMPLE_2 *input, unsigned int inputSize)
{
  //fprintf( stderr, "." );
  
  //
  // processing time-varying delay for ITD
  //
  float tvDelay = m_prevITD;
  float deltaDelay = (m_ITD - m_prevITD)/inputSize;

  for (int i=0; i < inputSize; i++ ) {
    // set time-varying delay
    tvDelay += deltaDelay; 
    m_TV_delayLine.setDelay(m_defaultDelay + tvDelay);

    // execute delay
    m_leftDelayOut[i] = m_TV_delayLine.tick(input[i]);
    m_rightDelayOut[i] = m_delayLine.tick(input[i]);
  }

  if ( m_overlapAdd ) {
    //
    // left channel
    //
    // backup the previous output
    SAMPLE_2 prevOut[NUMS_FILTER_COEF];
    memcpy(&prevOut,&(m_leftOut[inputSize]), sizeof(SAMPLE_2) * (NUMS_FILTER_COEF-1) );
    
    // perform FFT
    convolve_fft(m_leftDelayOut, inputSize, HRIR_left, NUMS_FILTER_COEF,
		 m_leftOut, inputSize+NUMS_FILTER_COEF);
    
    // overlapped area
    for ( int i = 0; i < (NUMS_FILTER_COEF-1); i++ )
      output[2*i] = m_leftOut[i]+ prevOut[i];
    //output[2*i] = m_leftDelayOut[i];

    // non-overlapped area
    for ( int i = (NUMS_FILTER_COEF-1); i < outputSize; i++ )
      output[2*i] = m_leftOut[i];
    //output[2*i] = m_leftDelayOut[i];

    //
    // right channel
    //
    // backup the previous output
    memcpy(&prevOut, &(m_rightOut[inputSize]), sizeof(SAMPLE_2) * (NUMS_FILTER_COEF-1) );

    // perform FFT
    convolve_fft(m_rightDelayOut, inputSize, HRIR_right, NUMS_FILTER_COEF, 
		 m_rightOut, inputSize+NUMS_FILTER_COEF);

    // overlapped area
    for ( int i = 0; i < (NUMS_FILTER_COEF-1); i++ )
      output[2*i+1] = (m_rightOut[i]+ prevOut[i]);
    //output[2*i+1] = m_rightDelayOut[i];
    
    // non-overlapped area
    for ( int i = (NUMS_FILTER_COEF-1); i < outputSize; i++ )
      output[2*i+1] = m_rightOut[i];
    //output[2*i+1] = m_rightDelayOut[i];

  }
  else {
    //for ( int i = 0; i < 2; i++ ) {
    //convolve_fft(input, inputSize, HRIR_left, NUMS_FILTER_COEF, output[0], outputSize);
    //convolve_fft(input, inputSize, HRIR_right, NUMS_FILTER_COEF, output[1], outputSize);
    //}
  }
}
