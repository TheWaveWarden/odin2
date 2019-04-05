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

  Voice(){
    env1.setFinishFlag(&voice_active);
  }

  //oscs
  AnalogOscillator analog_osc;
  WavetableOsc2D wavetable_osc;
  MultiOscillator multi_osc;
  VectorOscillator vector_osc;
  ChiptuneOscillator chiptune_osc;
  FMOscillator fm_osc;
  NoiseOscillator noise_osc;

  //filter1
  LadderFilter ladder_filter_1;
  DiodeFilter diode_filter_1;
  FormantFilter formant_filter_1;
  Korg35Filter korg_filter_1;
  SEMFilter12 SEM_filter_12_1;
  SEMFilter24 SEM_filter_24_1;
  //filter2
  LadderFilter ladder_filter_2;
  DiodeFilter diode_filter_2;
  FormantFilter formant_filter_2;
  Korg35Filter korg_filter_2;
  SEMFilter12 SEM_filter_12_2;
  SEMFilter24 SEM_filter_24_2;
  //ADSRs
  ADSREnvelope env1; //amp
  ADSREnvelope env2; //filter
  ADSREnvelope env3; 
  ADSREnvelope env4; 

  //LFOs
  //todo

  //voice_active
  bool voice_active = false;
};