#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//found at https://www.dsprelated.com/freebooks/filters/DC_Blocker.html
class DCBlockingFilter {
  public:
  float doFilter(double p_input){
    double y = p_input - xm1 + R * ym1;
    xm1 = p_input;
    ym1 = y;
    return (float)y;
  }

  void setSampleRate(float p_samplerate){
     
    DBG("SetSampleRate DCBlock");
    //we gettin REAL dirty here...
    R = 0.995;
    if(p_samplerate > 90000){
      R = 0.9965;
    }
    if(p_samplerate > 120000){
      R = 0.997;
    }
  }

  void reset(){
    xm1 = 0;
    ym1 = 0;
  }

  protected:
  double xm1 = 0;
  double ym1 = 0;

  double R = 0.995;
};
