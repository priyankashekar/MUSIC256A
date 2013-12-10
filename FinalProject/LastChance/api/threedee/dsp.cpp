#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include <fftw3.h>
#include <iostream>
#include <string>

//#include "chuck_fft.h"

#include "global.h"

using namespace std;

// our sample
//#define SAMPLE double

#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)



//-----------------------------------------------------------------------------
// name: next_power_2()
// desc: ...
// thanks: to Niklas Werner / music-dsp
//-----------------------------------------------------------------------------
unsigned long next_power_2( unsigned long n )
{
    unsigned long nn = n;
    for( ; n &= n-1; nn = n );
    return nn * 2;
}

//-----------------------------------------------------------------------------
// name: convolve_fft()
// desc: compute FFT using FFTW library
//-----------------------------------------------------------------------------
void convolve_fft( SAMPLE * f, int fsize, SAMPLE * g, int gsize, SAMPLE * buffy, int size )
{
  // determine FFT size
  unsigned int fftsize = next_power_2( fsize + gsize - 1 );

  // allocate memory
  double *in = new double[fftsize];
  double *h = new double[fftsize];
  double *out = new double[fftsize];
  fftw_complex *inFFT = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftsize);
  fftw_complex *hFFT = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftsize);
  fftw_complex *outFFT = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftsize);

  // initialize
  memset( in, 0, sizeof(SAMPLE) * fftsize );
  memset( h, 0, sizeof(SAMPLE) * fftsize );
  memset( out, 0, sizeof(SAMPLE) * fftsize );
  memcpy( in, f, sizeof(SAMPLE) * fsize );
  memcpy( h, g, sizeof(SAMPLE) * gsize );

  // set FFT
  fftw_plan p1, p2, ip;
  p1 = fftw_plan_dft_r2c_1d(fftsize, in, inFFT, FFTW_ESTIMATE);
  p2 = fftw_plan_dft_r2c_1d(fftsize, h, hFFT, FFTW_ESTIMATE);
  ip = fftw_plan_dft_c2r_1d(fftsize, outFFT, out, FFTW_ESTIMATE);  

  // perform FFT
  fftw_execute(p1);
  fftw_execute(p2);

  // FFT multiplication, actually convolution
  for ( int i = 0; i < fftsize; i++ ) {
    outFFT[i][0] = (inFFT[i][0]*hFFT[i][0] - inFFT[i][1]*hFFT[i][1])/fftsize;
    outFFT[i][1] = (inFFT[i][0]*hFFT[i][1] + inFFT[i][1]*hFFT[i][0])/fftsize;
  }

  // perform inverse FFT
  fftw_execute(ip);

  // copy the restul to output
  memcpy( buffy, out, sizeof(SAMPLE) * size );

  // free memory
  fftw_destroy_plan(p1);
  fftw_destroy_plan(p2);
  fftw_destroy_plan(ip);

  fftw_free(inFFT);  fftw_free(hFFT); fftw_free(outFFT);
  delete [] in; delete [] h; delete [] out;
}


#if 0
// convolve in freq domain
void convolve_fft( SAMPLE * f, int fsize, SAMPLE * g, int gsize, SAMPLE * buffy, int size )
{
  // sanity check
  //assert( (fsize + gsize - 1) == size );
  
  // make buffers to hold kernel and signal
  unsigned int fftsize = next_power_2( fsize + gsize - 1 );

  // do it
  SAMPLE * fbuf = new SAMPLE[fftsize];
  SAMPLE * gbuf = new SAMPLE[fftsize];
  SAMPLE * result = new SAMPLE[fftsize];

  // clear
  memset( fbuf, 0, sizeof(SAMPLE) * fftsize );
  memset( gbuf, 0, sizeof(SAMPLE) * fftsize );
  memset( result, 0, sizeof(SAMPLE) * fftsize );
  
  // copy in
  memcpy( fbuf, f, sizeof(SAMPLE) * fsize );
  memcpy( gbuf, g, sizeof(SAMPLE) * gsize );
  
  // take fft
  rfft( fbuf, fftsize/2, FFT_FORWARD );
  rfft( gbuf, fftsize/2, FFT_FORWARD );
  
  // complex
  complex * fcomp = (complex *)fbuf;
  complex * gcomp = (complex *)gbuf;
  complex * rcomp = (complex *)result;
  
  // variables
  float fre, fim, gre, gim;
  // loop
  for( int i = 0; i < fftsize/2; i++ ) {
    // local copies
    fre = fcomp[i].re; fim = fcomp[i].im;
    gre = gcomp[i].re; gim = gcomp[i].im;
    // complex multiply
    rcomp[i].re = fre * gre - fim * gim;
    rcomp[i].im = fre * gim + fim * gre;
  }

  //printf("%f\n", rcomp[0].re*1000);
    
  // invers fft
  rfft( result, fftsize/2, FFT_INVERSE );

  // copy into buffy
  memcpy( buffy, result, sizeof(SAMPLE) * size );
  
  //printf("%f\n", result[0]*1000);

  delete [] fbuf;
  delete [] gbuf;
  delete [] result;
}
#endif
