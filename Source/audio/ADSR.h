#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#include "CheapFunctions.h"
#include <fstream> //todo  delay
#include <functional>

#define MIN_DECAY_RELEASE_VAL 0.001
#include <cmath>

class ADSREnvelope {
public:
  ADSREnvelope();
  ~ADSREnvelope();

  float doEnvelope();
  void startRelease();
  int getCurrentSection();

  void testADSRPow();
  void profileCheapPow();

  // void setFinishFlag(bool* p_finish_flag){
  //	m_envelope_finish_flag = p_finish_flag;
  //}

  inline void reset() {
    m_current_section = -1;
    m_current_value = 0.f;
    m_attack_start_value = 0.f;
  }

  inline void setLoop(bool p_loop) { m_loop = p_loop; }

  inline void setSamplerate(float p_samplerate) { m_samplerate = p_samplerate; }

  inline void setAttack(float p_attack) { m_attack = p_attack; }

  inline void setDecay(float p_decay) { m_decay = p_decay; }

  inline void setSustain(float p_sustain) { m_sustain = p_sustain; }

  inline void setRelease(float p_release) { m_release = p_release; }

  inline double calcModFactor(double p_mod) { return pow(2, 3 * p_mod); }

  inline double calcDecayFactor(double p_decay) {
    if (p_decay != m_last_decay) {
      // DBG("DELAY RECALCULATED");
      m_last_decay_return =
          pow(MIN_DECAY_RELEASE_VAL, 1. / m_samplerate / p_decay);
      m_last_decay = p_decay;
    }
    return m_last_decay_return;
  }

  inline double calcReleaseFactor(double p_release) {
    if (p_release != m_last_release) {
      // DBG("RELEASE RECALCULATED");
      m_last_release_return =
          pow(MIN_DECAY_RELEASE_VAL, 1. / m_samplerate / p_release);
      m_last_release = p_release;
    }
    return m_last_release_return;
  }

  /*inline */ double calcDecayFactorCheap(double p_decay) {
    return CheapFunctions::getInstance().cheapADSRPow(m_samplerate * p_decay);
  }

  void setAttackModPointer(float *p_pointer) { m_attack_mod = p_pointer; }
  void setDecayModPointer(float *p_pointer) { m_decay_mod = p_pointer; }
  void setSustainModPointer(float *p_pointer) { m_sustain_mod = p_pointer; }
  void setReleaseModPointer(float *p_pointer) { m_release_mod = p_pointer; }

  // used to signal voice endign if this is amp-envelope
  // std::function<void()> onEnvelopeEnd = []() {};
  void onEnvelopeEnd() {
    if (m_voice_bool_pointer && m_voice_manager_bool_pointer) {
      DBG("1");
      *m_voice_manager_bool_pointer = false;
      DBG("ENNNND:");
      DBG((long)m_voice_bool_pointer);
      DBG((long)m_test);      
      *m_test = false;
    }
  }

  //! WEIRD STUFF! 500th workaround for visual studio cant handle my
  //! lambda..........
  void setEnvelopeEndPointers(bool *p_voice, bool *p_manager) {
    m_voice_manager_bool_pointer = p_manager;
    m_voice_bool_pointer = p_voice;
    m_test = p_voice;
    // todo remove

    DBG("STARRRT:");
    DBG((long)m_voice_bool_pointer);
    DBG((long)m_test);
    *m_voice_manager_bool_pointer = false;
    *m_test = false;
  }

protected:
  // CheapFunctions& m_cheap_functions;

  //! WEIRD STUFF! 500th workaround for visual studio cant handle my
  //! lambda..........
  bool *m_voice_manager_bool_pointer = nullptr;
  bool *m_voice_bool_pointer = nullptr;
  bool *m_test = nullptr;

  int m_current_section = -1; // before start:-1, A0, D1, S2, R3
  bool m_loop = false;

  // store values so we dont calculate pow() all the time
  double m_last_decay = 0.f;
  double m_last_decay_return = 0.f;
  double m_last_release = 0.f;
  double m_last_release_return = 0.f;

  float *m_attack_mod;
  float *m_decay_mod;
  float *m_sustain_mod;
  float *m_release_mod;

  double m_attack = 0.3f;
  double m_attack_start_value = 0.f;
  double m_decay = 1.f;
  double m_decay_factor = 0.9998;
  double m_sustain = 0.5f;
  double m_release = 1.f;
  double m_release_factor = 0.9998;
  double m_release_start_value = 1.;

  double m_current_value = 0.f;
  double m_samplerate;

  // bool* m_envelope_finish_flag = nullptr;
};
