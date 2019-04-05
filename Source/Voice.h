/*
  ==============================================================================

    Voice.h
    Created: 30 Mar 2019 1:52:30am
    Author:  frot

  ==============================================================================
*/
#pragma once

#include "GlobalIncludes.h"
#include "audio/Oscillators/AnalogOscillator.h"
#include "audio/Oscillators/ChiptuneOscillator.h"
#include "audio/Oscillators/FMOscillator.h"
#include "audio/Oscillators/MultiOscillator.h"
#include "audio/Oscillators/NoiseOscillator.h"
#include "audio/Oscillators/VectorOscillator.h"
#include "audio/Oscillators/WavetableOsc2D.h"

// wavedraw
// chipdraw
// specdraw
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Filters/SEMFilter24.h"


#include "audio/ADSR.h"

// one voice of the polyphonic voices, i.e. everything up to the amplifier
struct Voice {

  // implicit conversion to bool
  operator bool() const { return voice_active; }

  Voice() { env1.setFinishFlag(&voice_active); }

  float MIDINoteToFreq(int p_MIDI_note) {
    return 27.5f * pow(2.f, (float)(p_MIDI_note - 21) / 12.f);
  }

  void start(int p_MIDI_note) {
    reset();
    setOscBaseFreq(MIDINoteToFreq(p_MIDI_note));
    voice_active = true;
    DBG("Started voice");
  }

  void setOscBaseFreq(float p_freq) {
    analog_osc.setBaseFrequency(p_freq);
    wavetable_osc.setBaseFrequency(p_freq);
    multi_osc.setBaseFrequency(p_freq);
    vector_osc.setBaseFrequency(p_freq);
    chiptune_osc.setBaseFrequency(p_freq);
  }

  void reset() {
    analog_osc.reset();
    wavetable_osc.reset();
    multi_osc.reset();
    vector_osc.reset();
    chiptune_osc.reset();
    fm_osc.reset();
    // draw oscs
    ladder_filter_1.reset();
    diode_filter_1.reset(), formant_filter_1.reset();
    korg_filter_1.reset();
    SEM_filter_12_1.reset();
    SEM_filter_24_1.reset();
    ladder_filter_2.reset();
    diode_filter_2.reset(), formant_filter_2.reset();
    korg_filter_2.reset();
    SEM_filter_12_2.reset();
    SEM_filter_24_2.reset();
  }

  // oscs
  AnalogOscillator analog_osc;
  WavetableOsc2D wavetable_osc;
  MultiOscillator multi_osc;
  VectorOscillator vector_osc;
  ChiptuneOscillator chiptune_osc;
  FMOscillator fm_osc;
  NoiseOscillator noise_osc;

  // filter1
  LadderFilter ladder_filter_1;
  DiodeFilter diode_filter_1;
  FormantFilter formant_filter_1;
  Korg35Filter korg_filter_1;
  SEMFilter12 SEM_filter_12_1;
  SEMFilter24 SEM_filter_24_1;
  // filter2
  LadderFilter ladder_filter_2;
  DiodeFilter diode_filter_2;
  FormantFilter formant_filter_2;
  Korg35Filter korg_filter_2;
  SEMFilter12 SEM_filter_12_2;
  SEMFilter24 SEM_filter_24_2;
  // ADSRs
  ADSREnvelope env1; // amp
  ADSREnvelope env2; // filter
  ADSREnvelope env3;
  ADSREnvelope env4;

  // LFOs
  // todo

  // voice_active
  bool voice_active = false;
};