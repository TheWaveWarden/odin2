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
#include "audio/Oscillators/LFO.h"
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
//#include "audio/Filters/SEMFilter24.h"

#include "audio/ADSR.h"

class VoiceManager {
public:
  // returns free voice index or steals one and sets it busy
  int getVoice(int p_note) {
    // first check if the note to be played is on the sustain list:
    if (m_sustain_active) {
      for (int voice = 0; voice < VOICES; ++voice) {
        if (m_kill_list[voice] && m_kill_list_note[voice] == p_note) {
          // note is on sustain... just remove it from kill list and be done
          m_kill_list[voice] = false;
          DBG("Note " + std::to_string(p_note) +
              " is already ins sustain on voice " + std::to_string(voice));
          return -1;
        }
      }
    }
    for (int i = 0; i < VOICES; ++i) {
      if (!voice_busy[i]) {
        voice_busy[i] = true;
        DBG("Voice manager returned voice " + std::to_string(i));
        removeFromKillList(i);
        updateVoiceHistory(i);
        return i;
      }
    }
    // TODO all voices busy, steal
    DBG("Voice manager STOLE voice " + std::to_string(m_voice_history[11]));
    removeFromKillList(m_voice_history[11]);
    int ret = m_voice_history[11];
    updateVoiceHistory(ret);
    return ret;
  }

  // marks a voice as free again
  void freeVoice(int p_voice) {
    voice_busy[p_voice] = false;
    DBG("Voice manager freed voice " + std::to_string(p_voice));
  }

  void setSustainActive(bool p_active) { m_sustain_active = p_active; }

  bool getSustainActive() { return m_sustain_active; }

  // adds to killlist to be killed after sustain pedal gets lifted
  void addToKillList(int p_voice, int p_note) {
    m_kill_list_note[p_voice] = p_note;
    m_kill_list[p_voice] = true;
  }

  void removeFromKillList(int p_voice) { m_kill_list[p_voice] = false; }

  bool isOnKillList(int p_voice) { return m_kill_list[p_voice]; }

  void clearKillList() {
    for (int voice = 0; voice < VOICES; ++voice) {
      m_kill_list[voice] = false;
    }
  }

  void updateVoiceHistory(int p_next_voice){
    int index = 0;
    while(p_next_voice != m_voice_history[index]){
      ++index;
    }

    //p_next_voice was at position index, move all above on down:
    for(int voice = index; voice > 0; --voice){
      m_voice_history[voice] = m_voice_history[voice - 1];
    }

    //now set new as newest value:
    m_voice_history[0] = p_next_voice;

    DBG(std::to_string(m_voice_history[0]) + " " + std::to_string(m_voice_history[1]) + " " +std::to_string( m_voice_history[2]) + " " + std::to_string(m_voice_history[3]) + " " + std::to_string(m_voice_history[4]) + " " + std::to_string(m_voice_history[5]) + " " + std::to_string(m_voice_history[6]) + " " + std::to_string(m_voice_history[7]) + " " + std::to_string(m_voice_history[8]) + " " + std::to_string(m_voice_history[9])+ " " + std::to_string(m_voice_history[10]) + " " + std::to_string(m_voice_history[11]));
  }

protected:
  //used to determine oldest voice for stealing
  int m_voice_history[VOICES] = {0,1,2,3,4,5,6,7,8,9,10,11};
  bool m_sustain_active = false;
  bool m_kill_list[VOICES];
  int m_kill_list_note[VOICES];
  bool voice_busy[VOICES]; // is voice busy
};

// one voice of the polyphonic voices, i.e. everything up to the amplifier
struct Voice {

  // implicit conversion to bool
  operator bool() const { return m_voice_active; }

  Voice() {
    // env[0].onEnvelopeEnd = [this]() { 
    //   onEnvelopeEnd(); 
    //   //MIDI_key_mod_source = 1.f;
    //   };
  }

  float MIDINoteToFreq(int p_MIDI_note) {
    return 27.5f * pow(2.f, (float)(p_MIDI_note - 21) / 12.f);
  }

  void start(int p_MIDI_key, int p_MIDI_velocity, int p_last_MIDI_key) {
    reset();
    setOscBaseFreq(MIDINoteToFreq(p_MIDI_key), MIDINoteToFreq(p_last_MIDI_key));
    setFilterMIDIValues(p_MIDI_key, p_MIDI_velocity);
    m_voice_active = true;
    m_MIDI_key = p_MIDI_key;
    MIDI_key_mod_source = (float)p_MIDI_key / 127.f;
    MIDI_velocity_mod_source = (float)p_MIDI_velocity / 127.f;
    for (int mod = 0; mod < 4; ++mod) {
      env[mod].reset();
    }
    DBG("Started voice");
  }

  // starts release on envelopes if this is the key that was pressed
  // returns true if the voice was actually stopped
  bool keyUp(int p_MIDI_key) {
    if (m_MIDI_key == p_MIDI_key) {
      DBG("Stopping envelopes on key " + std::to_string(m_MIDI_key));
      env[0].startRelease();
      env[1].startRelease();
      env[2].startRelease();
      env[3].startRelease();

      return true;
    }
    return false;
  }

  void startRelease() {
    DBG("Stopping envelopes on key " + std::to_string(m_MIDI_key) +
        " after sustian was released");
    env[0].startRelease();
    env[1].startRelease();
    env[2].startRelease();
    env[3].startRelease();
  }

  bool usesThisMIDIKey(int p_MIDI_key) {
    if (m_MIDI_key == p_MIDI_key) {
      return true;
    }
    return false;
  }

  void setOctave(int p_octave, int p_osc) {
    analog_osc[p_osc].m_octave = p_octave;
    wavedraw_osc[p_osc].m_octave = p_octave;
    chipdraw_osc[p_osc].m_octave = p_octave;
    specdraw_osc[p_osc].m_octave = p_octave;
    wavetable_osc[p_osc].m_octave = p_octave;
    multi_osc[p_osc].m_octave = p_octave;
    vector_osc[p_osc].m_octave = p_octave;
    chiptune_osc[p_osc].m_octave = p_octave;
    fm_osc[p_osc].m_octave = p_octave;
  }

  void setSemitones(int p_semi, int p_osc) {
    analog_osc[p_osc].m_semitones = p_semi;
    wavedraw_osc[p_osc].m_semitones = p_semi;
    chipdraw_osc[p_osc].m_semitones = p_semi;
    specdraw_osc[p_osc].m_semitones = p_semi;
    wavetable_osc[p_osc].m_semitones = p_semi;
    multi_osc[p_osc].m_semitones = p_semi;
    vector_osc[p_osc].m_semitones = p_semi;
    chiptune_osc[p_osc].m_semitones = p_semi;
    fm_osc[p_osc].m_semitones = p_semi;
  }

  void setFinetune(float p_fine, int p_osc) {
    analog_osc[p_osc].m_cent = p_fine;
    wavedraw_osc[p_osc].m_cent = p_fine;
    chipdraw_osc[p_osc].m_cent = p_fine;
    specdraw_osc[p_osc].m_cent = p_fine;
    wavetable_osc[p_osc].m_cent = p_fine;
    multi_osc[p_osc].m_cent = p_fine;
    vector_osc[p_osc].m_cent = p_fine;
    chiptune_osc[p_osc].m_cent = p_fine;
    fm_osc[p_osc].m_cent = p_fine;
  }

  void setGlide(float p_glide) {
    for (int osc = 0; osc < 3; ++osc) {
      analog_osc[osc].setGlide(p_glide);
      wavedraw_osc[osc].setGlide(p_glide);
      chipdraw_osc[osc].setGlide(p_glide);
      specdraw_osc[osc].setGlide(p_glide);
      wavetable_osc[osc].setGlide(p_glide);
      multi_osc[osc].setGlide(p_glide);
      vector_osc[osc].setGlide(p_glide);
      chiptune_osc[osc].setGlide(p_glide);
      fm_osc[osc].setGlide(p_glide);
    }
  }

  void killGlide(int p_osc) {
      analog_osc[p_osc].killGlide();
      wavedraw_osc[p_osc].killGlide();
      chipdraw_osc[p_osc].killGlide();
      specdraw_osc[p_osc].killGlide();
      wavetable_osc[p_osc].killGlide();
      multi_osc[p_osc].killGlide();
      vector_osc[p_osc].killGlide();
      chiptune_osc[p_osc].killGlide();
      fm_osc[p_osc].killGlide();
  }

  void setReset(bool p_reset, int p_osc) {
    analog_osc[p_osc].setResetActive(p_reset);
    wavedraw_osc[p_osc].setResetActive(p_reset);
    chipdraw_osc[p_osc].setResetActive(p_reset);
    specdraw_osc[p_osc].setResetActive(p_reset);
    wavetable_osc[p_osc].setResetActive(p_reset);
    multi_osc[p_osc].setResetActive(p_reset);
    vector_osc[p_osc].setResetActive(p_reset);
    chiptune_osc[p_osc].setResetActive(p_reset);
    fm_osc[p_osc].setResetActive(p_reset);
  }

  void setOscBaseFreq(float p_freq, float p_last_freq) {
    for (int osc = 0; osc < 3; ++osc) {
      analog_osc[osc].setBaseFrequency(p_last_freq);
      analog_osc[osc].setGlideTargetFrequency(p_freq);
      wavedraw_osc[osc].setBaseFrequency(p_last_freq);
      wavedraw_osc[osc].setGlideTargetFrequency(p_freq);
      chipdraw_osc[osc].setBaseFrequency(p_last_freq);
      chipdraw_osc[osc].setGlideTargetFrequency(p_freq);
      specdraw_osc[osc].setBaseFrequency(p_last_freq);
      specdraw_osc[osc].setGlideTargetFrequency(p_freq);
      wavetable_osc[osc].setBaseFrequency(p_last_freq);
      wavetable_osc[osc].setGlideTargetFrequency(p_freq);
      multi_osc[osc].setBaseFrequency(p_last_freq);
      multi_osc[osc].setGlideTargetFrequency(p_freq);
      vector_osc[osc].setBaseFrequency(p_last_freq);
      vector_osc[osc].setGlideTargetFrequency(p_freq);
      chiptune_osc[osc].setBaseFrequency(p_last_freq);
      chiptune_osc[osc].setGlideTargetFrequency(p_freq);
      fm_osc[osc].setBaseFrequency(p_last_freq);
      fm_osc[osc].setGlideTargetFrequency(p_freq);
    }
  }

  void reset() {
    for (int osc = 0; osc < 3; ++osc) {
      //use start voice, oscs will reset if reset is active
      analog_osc[osc].voiceStart();
      wavetable_osc[osc].voiceStart();
      wavedraw_osc[osc].voiceStart();
      chipdraw_osc[osc].voiceStart();
      specdraw_osc[osc].voiceStart();
      multi_osc[osc].voiceStart();
      vector_osc[osc].voiceStart();
      chiptune_osc[osc].voiceStart();
      fm_osc[osc].voiceStart();
      wavedraw_osc[osc].voiceStart();
      specdraw_osc[osc].voiceStart();
      chipdraw_osc[osc].voiceStart();
    }
    for (int mod = 0; mod < 4; ++mod) {
      lfo[mod].voiceStart();
    }
    for (int fil = 0; fil < 2; ++fil) {
      ladder_filter[fil].reset();
      diode_filter[fil].reset();
      formant_filter[fil].reset();
      korg_filter[fil].reset();
      SEM_filter_12[fil].reset();
    }
  }

  void setFilterMIDIValues(int p_MIDI_note, int p_MIDI_vel) {
    // shift note here so the lowest note possible is 21
    p_MIDI_note -= 21;
    p_MIDI_note = p_MIDI_note < 0 ? 0 : p_MIDI_note;

    for (int fil = 0; fil < 3; ++fil) {
      ladder_filter[fil].m_MIDI_note = p_MIDI_note;
      diode_filter[fil].m_MIDI_note = p_MIDI_note;
      korg_filter[fil].m_MIDI_note = p_MIDI_note;
      // SEM_filter_24[fil].m_MIDI_note = p_MIDI_note;
      SEM_filter_12[fil].m_MIDI_note = p_MIDI_note;
      comb_filter[fil].m_MIDI_note = p_MIDI_note;

      ladder_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      diode_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      korg_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      // SEM_filter_24[fil].m_MIDI_velocity = p_MIDI_vel;
      SEM_filter_12[fil].m_MIDI_velocity = p_MIDI_vel;
      comb_filter[fil].m_MIDI_velocity = p_MIDI_vel;
      formant_filter[fil].m_MIDI_velocity = p_MIDI_vel;
    }
  }

  void setKbd(float p_kbd, int p_fil) {
    ladder_filter[p_fil].m_kbd_mod_amount = p_kbd;
    SEM_filter_12[p_fil].m_kbd_mod_amount = p_kbd;
    // SEM_filter_24[p_fil].m_kbd_mod_amount = p_kbd;
    korg_filter[p_fil].m_kbd_mod_amount = p_kbd;
    diode_filter[p_fil].m_kbd_mod_amount = p_kbd;
    comb_filter[p_fil].m_kbd_mod_amount = p_kbd;
  }

  void setVelModAmount(float p_vel, int p_fil) {
    ladder_filter[p_fil].m_vel_mod_amount = p_vel;
    SEM_filter_12[p_fil].m_vel_mod_amount = p_vel;
    // SEM_filter_24[p_fil].m_vel_mod_amount = p_vel;
    korg_filter[p_fil].m_vel_mod_amount = p_vel;
    diode_filter[p_fil].m_vel_mod_amount = p_vel;
    comb_filter[p_fil].m_vel_mod_amount = p_vel;
    formant_filter[p_fil].m_vel_mod_amount = p_vel;
  }

  void setSaturation(float p_sat, int p_fil) {
    ladder_filter[p_fil].m_overdrive = p_sat;
    SEM_filter_12[p_fil].m_overdrive = p_sat;
    // SEM_filter_24[p_fil].m_overdrive = p_sat;
    korg_filter[p_fil].m_overdrive = p_sat;
    diode_filter[p_fil].m_overdrive = p_sat;
  }

  void setEnvModAmount(float p_env, int p_fil) {
    // set as quadratic
    ladder_filter[p_fil].m_env_mod_amount = p_env;
    SEM_filter_12[p_fil].m_env_mod_amount = p_env;
    // SEM_filter_24[p_fil].m_env_mod_amount = p_env;
    korg_filter[p_fil].m_env_mod_amount = p_env;
    diode_filter[p_fil].m_env_mod_amount = p_env;
    comb_filter[p_fil].m_env_mod_amount = p_env;
    formant_filter[p_fil].m_env_mod_amount = p_env;
  }

  void setFilterEnvValue(float p_env) {
    ladder_filter[0].m_env_value = p_env;
    SEM_filter_12[0].m_env_value = p_env;
    // SEM_filter_24[0].m_env_value = p_env;
    korg_filter[0].m_env_value = p_env;
    diode_filter[0].m_env_value = p_env;
    comb_filter[0].m_env_value = p_env;
    formant_filter[0].m_env_value = p_env;

    ladder_filter[1].m_env_value = p_env;
    SEM_filter_12[1].m_env_value = p_env;
    // SEM_filter_24[1].m_env_value = p_env;
    korg_filter[1].m_env_value = p_env;
    diode_filter[1].m_env_value = p_env;
    comb_filter[1].m_env_value = p_env;
    formant_filter[1].m_env_value = p_env;
  }

  void setFilterFreq(float p_freq, int p_fil) {
    // note: not used right now since control is smoothed
    ladder_filter[p_fil].m_freq_base = p_freq;
    SEM_filter_12[p_fil].m_freq_base = p_freq;
    // SEM_filter_24[p_fil].m_freq_base = p_freq;
    korg_filter[p_fil].m_freq_base = p_freq;
    diode_filter[p_fil].m_freq_base = p_freq;
    comb_filter[p_fil].setCombFreq(p_freq);
  }

  void setFilterRes(float p_res, int p_fil) {
    ladder_filter[p_fil].setResControl(p_res);
    SEM_filter_12[p_fil].setResControl(p_res);
    // SEM_filter_24[p_fil].setResControl(p_res);
    korg_filter[p_fil].setResControl(p_res);
    diode_filter[p_fil].setResControl(p_res);
    comb_filter[p_fil].setResonance(p_res);
  }

  void setSampleRate(float p_samplerate) {
    env[0].setSamplerate(p_samplerate);
    env[1].setSamplerate(p_samplerate);
    env[2].setSamplerate(p_samplerate);
    env[3].setSamplerate(p_samplerate);
  }

  // oscs
  AnalogOscillator analog_osc[3];
  WavetableOsc2D wavetable_osc[3];
  MultiOscillator multi_osc[3];
  VectorOscillator vector_osc[3];
  ChiptuneOscillator chiptune_osc[3];
  FMOscillator fm_osc[3];
  NoiseOscillator noise_osc[3];
  WavetableOsc1D wavedraw_osc[3];
  WavetableOsc1D chipdraw_osc[3];
  WavetableOsc1D specdraw_osc[3];

  // filter
  LadderFilter ladder_filter[2];
  SEMFilter12 SEM_filter_12[2];
  // SEMFilter24 SEM_filter_24[2];
  Korg35Filter korg_filter[2];
  DiodeFilter diode_filter[2];
  FormantFilter formant_filter[2];
  CombFilter comb_filter[2];
  // ADSRs
  ADSREnvelope env[4];
  LFO lfo[4];

  // LFOs
  // todo

  // modulation values
  float MIDI_key_mod_source = 0.f;
  float MIDI_velocity_mod_source = 0.f;

  // called when the envelope ends to signal voice end to voice manager
  //std::function<void()> onEnvelopeEnd = []() {};
  bool m_voice_active = false;
  int m_MIDI_key = 0;
};
