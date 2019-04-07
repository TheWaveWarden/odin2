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
#include "audio/Filters/CombFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Filters/SEMFilter24.h"

#include "audio/ADSR.h"

class VoiceManager {
public:
  // returns free voice index or steals one and sets it busy
  int getVoice() {
    for (int i = 0; i < VOICES; ++i) {
      if (!voice_busy[i]) {
        voice_busy[i] = true;
        return i;
      }
    }
    // TODO all voices busy, steal
    return 0;
  }

  // marks a voice as free again
  void freeVoice(int p_voice) { voice_busy[p_voice] = false; }

protected:
  bool voice_busy[VOICES] = {false}; // is voice busy
};

// one voice of the polyphonic voices, i.e. everything up to the amplifier
struct Voice {

  // implicit conversion to bool
  operator bool() const { return m_voice_active; }

  Voice() {
    env1.onEnvelopeEnd = [&]() { onEnvelopeEnd(); };
  }

  float MIDINoteToFreq(int p_MIDI_note) {
    return 27.5f * pow(2.f, (float)(p_MIDI_note - 21) / 12.f);
  }

  void start(int p_MIDI_key) {
    reset();
    setOscBaseFreq(MIDINoteToFreq(p_MIDI_key));
    m_voice_active = true;
    DBG("Started voice");
    m_MIDI_key = p_MIDI_key;
  }

  // starts release on envelopes if this is the key that was pressed
  void keyUp(int p_MIDI_key) {
    if (m_MIDI_key = p_MIDI_key) {
      env1.startRelease();
      env2.startRelease();
      env3.startRelease();
      env4.startRelease();
    }
  }

  void setOctave(int p_octave, int p_osc) {
    analog_osc[p_osc].m_octave = p_octave;
    wavetable_osc[p_osc].m_octave = p_octave;
    multi_osc[p_osc].m_octave = p_octave;
    vector_osc[p_osc].m_octave = p_octave;
    chiptune_osc[p_osc].m_octave = p_octave;
    fm_osc[p_osc].m_octave = p_octave;
  }

  void setSemitones(int p_semi, int p_osc) {
    analog_osc[p_osc].m_semitones = p_semi;
    wavetable_osc[p_osc].m_semitones = p_semi;
    multi_osc[p_osc].m_semitones = p_semi;
    vector_osc[p_osc].m_semitones = p_semi;
    chiptune_osc[p_osc].m_semitones = p_semi;
    fm_osc[p_osc].m_semitones = p_semi;
  }

  void setFinetune(float p_fine, int p_osc) {
    analog_osc[p_osc].m_cent = p_fine;
    wavetable_osc[p_osc].m_cent = p_fine;
    multi_osc[p_osc].m_cent = p_fine;
    vector_osc[p_osc].m_cent = p_fine;
    chiptune_osc[p_osc].m_cent = p_fine;
    fm_osc[p_osc].m_cent = p_fine;
  }

  void setReset(bool p_reset, int p_osc) {
    analog_osc[p_osc].m_reset = p_reset;
    wavetable_osc[p_osc].m_reset = p_reset;
    multi_osc[p_osc].m_reset = p_reset;
    vector_osc[p_osc].m_reset = p_reset;
    chiptune_osc[p_osc].m_reset = p_reset;
    fm_osc[p_osc].m_reset = p_reset;
  }

  void setOscBaseFreq(float p_freq) {
    for (int osc = 0; osc < 3; ++osc) {
      analog_osc[osc].setBaseFrequency(p_freq);
      wavetable_osc[osc].setBaseFrequency(p_freq);
      multi_osc[osc].setBaseFrequency(p_freq);
      vector_osc[osc].setBaseFrequency(p_freq);
      chiptune_osc[osc].setBaseFrequency(p_freq);
      fm_osc[osc].setBaseFrequency(p_freq);
    }
  }

  void reset() {
    for (int osc = 0; osc < 3; ++osc) {
      analog_osc[osc].reset();
      wavetable_osc[osc].reset();
      multi_osc[osc].reset();
      vector_osc[osc].reset();
      chiptune_osc[osc].reset();
      fm_osc[osc].reset();

      // todo draw oscs
    }
    for (int fil = 0; fil < 2; ++fil) {
      ladder_filter[fil].reset();
      diode_filter[fil].reset();
      formant_filter[fil].reset();
      korg_filter[fil].reset();
      SEM_filter_12[fil].reset();
    }
  }

  void setFilterFreq(float p_freq, int p_fil) {
      ladder_filter[p_fil].m_freq_base = p_freq;
      SEM_filter_12[p_fil].m_freq_base = p_freq;
      SEM_filter_24[p_fil].m_freq_base = p_freq;
      korg_filter[p_fil].m_freq_base = p_freq;
      diode_filter[p_fil].m_freq_base = p_freq;
      comb_filter[p_fil].setCombFreq(p_freq);
  }

  void setFilterRes(float p_res, int p_fil) {
      ladder_filter[p_fil].setResControl(p_res);
      SEM_filter_12[p_fil].setResControl(p_res);
      SEM_filter_24[p_fil].setResControl(p_res);
      korg_filter[p_fil].setResControl(p_res);
      diode_filter[p_fil].setResControl(p_res);
      comb_filter[p_fil].setResonance(p_res);
  }

  // oscs
  AnalogOscillator analog_osc[3];
  WavetableOsc2D wavetable_osc[3];
  MultiOscillator multi_osc[3];
  VectorOscillator vector_osc[3];
  ChiptuneOscillator chiptune_osc[3];
  FMOscillator fm_osc[3];
  NoiseOscillator noise_osc[3];

  // filter
  LadderFilter ladder_filter[2];
  SEMFilter12 SEM_filter_12[2];
  SEMFilter24 SEM_filter_24[2];
  Korg35Filter korg_filter[2];
  DiodeFilter diode_filter[2];
  FormantFilter formant_filter[2];
  CombFilter comb_filter[2];
  // ADSRs
  ADSREnvelope env1; // amp
  ADSREnvelope env2; // filter
  ADSREnvelope env3;
  ADSREnvelope env4;

  // LFOs
  // todo

  // called when the envelope ends to signal voice end to voice manager
  std::function<void()> onEnvelopeEnd = []() {};
  bool m_voice_active = false;
  int m_MIDI_key = 0;
};
