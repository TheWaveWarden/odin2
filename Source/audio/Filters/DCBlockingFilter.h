/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

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
