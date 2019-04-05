/*
  ==============================================================================

    Voice.h
    Created: 30 Mar 2019 1:52:30am
    Author:  frot

  ==============================================================================
*/
#pragma once

#include "audio/Oscillators/AnalogOscillator.h"
#include "audio/Oscillators/WavetableOsc2D.h"
#include "audio/Oscillators/MultiOscillator.h"
#include "audio/Oscillators/VectorOscillator.h"
#include "audio/Oscillators/ChiptuneOscillator.h"
#include "audio/Oscillators/FMOscillator.h"
#include "audio/Oscillators/NoiseOscillator.h"
//wavedraw
//chipdraw
//specdraw
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Filters/SEMFilter24.h"

#include "audio/ADSR.h"



// one voice of the polyphonic voices, i.e. everything up to the amplifier
struct Voice {
  //oscs
  AnalogOscillator analog_osc;
  WavetableOsc2D wavetable_osc;
  MultiOscillator multi_osc;
  VectorOscillator vector_osc;
  ChiptuneOscillator chiptune_osc;
  FMOscillator fm_osc;
  NoiseOscillator noise_osc;

  //filters
  LadderFilter ladder_filter;
  DiodeFilter diode_filter;
  FormantFilter formant_filter;
  Korg35Filter korg_filter;
  SEMFilter12 SEM_filter_12;
  SEMFilter24 SEM_filter_24;

  //ADSR
  ADSREnvelope env1; //amp
  ADSREnvelope env2; //filter
  ADSREnvelope env3; 
  ADSREnvelope env4; 

};