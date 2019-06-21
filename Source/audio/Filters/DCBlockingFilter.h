#pragma once


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
    //TODO doSomethingWithR()
  }

  protected:
  double xm1 = 0;
  double ym1 = 0;

  double R = 0.995;
};
