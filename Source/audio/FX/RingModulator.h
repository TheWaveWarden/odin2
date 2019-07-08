#pragma once

#include "../Oscillators/WavetableOsc1D.h"



class RingModulator : public WavetableOsc1D{
public:
  RingModulator();
  float doRingModulator(float p_input);

  void reset() override;

protected:
  int getTableIndex() override;

   float m_modulation_dummy = 0;
};
