#pragma once
//#include "../pluginconstants.h"
//#include "../synthfunctions.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

#define OSC_FO_MOD_RANGE 2          // 2 semitone default
#define OSC_HARD_SYNC_RATIO_RANGE 4 // 4
#define OSC_PITCHBEND_MOD_RANGE 12  // 12 semitone default
#define OSC_FO_MIN 20               // 20Hz
#define OSC_FO_MAX 20480            // 20.480kHz = 10 octaves up from 20Hz
#define OSC_FO_DEFAULT 440.0        // A5
#define OSC_EXP_MOD_RANGE 24

class Oscillator {

public:
  Oscillator(void);
  virtual ~Oscillator(void);
  //
  // --- modulo functions for master/slave operation
  // --- increment the modulo counters
  inline void incModulo() { m_modulo += m_increment; }

  // todo potential bottleneck
  float pitchShiftMultiplier(float p_semitones) {
    return pow(2.f, p_semitones / 12.f);
  }

  // --- check and wrap the modulo
  //     returns true if modulo wrapped
  inline bool checkWrapModulo() {
    // --- for positive frequencies
    if (m_increment > 0 && m_modulo >= 1.0) {
      m_modulo -= 1.0;
      return true;
    }
    // --- for negative frequencies
    if (m_increment < 0 && m_modulo <= 0.0) {
      m_modulo += 1.0;
      return true;
    }
    return false;
  }
  // --- reset the modulo (required for master->slave operations)
  inline void resetModulo(double d = 0.0) { m_modulo = d; }
  inline void setPitchBendMod(double dMod) { m_mod_pitch_bend = dMod; }
  // virtual void startOscillator() {m_note_on = true;}
  // virtual void stopOscillator() {m_note_on = false;}
  virtual float doOscillate() = 0;
  virtual void setSampleRate(float p_samplerate) {
    m_samplerate = p_samplerate;
  }
  virtual void reset();

  void voiceStart(){
    if(m_reset_active){
      reset();
    }
  }

	
  inline void setResetActive(bool p_reset){
		m_reset_active = p_reset;
	}  

  // INLINE FUNCTIONS: these are inlined because they will be
  //                   called every sample period
  //					 You may want to move them to the .cpp file
  //and
  //                   enable the compiler Optimization setting for
  //					 Inline Function Expansion: Any Suitable
  //though 					 inlining here forces it.
  //
  // --- update the frequency, amp mod and PWM
  inline virtual void update() {
    //DBG(m_osc_freq_base);
    //DBG(m_osc_freq_glide_target);
    //DBG("===");

    float glide_input_modded = m_glide + *(m_glide_mod);
    glide_input_modded = glide_input_modded > 1 ? 1 : glide_input_modded;
    double glide_modded = calculateGlide(glide_input_modded);

    m_osc_freq_base =
        m_osc_freq_glide_target * (1.f - glide_modded) + (glide_modded)*m_osc_freq_base;

    // --- do the  complete frequency mod
    m_osc_freq_modded =
        m_osc_freq_base *
        pitchShiftMultiplier(
            *(m_pitchbend) + (*m_pitch_mod_exp) * OSC_EXP_MOD_RANGE +
            m_mod_freq_exp + m_mod_exp_other + m_mod_pitch_bend +
            m_octave * 12.0 + m_semitones + m_cent / 100.0);
    // --- apply linear FM (not used in book projects)
    m_osc_freq_modded +=
        (*m_pitch_mod_lin) * m_osc_freq_modded * 2 + m_mod_freq_lin;

    // --- bound Fo (can go outside for FM/PM mod)
    //     +/- 20480 for FM/PM
    if (m_osc_freq_modded > OSC_FO_MAX)
      m_osc_freq_modded = OSC_FO_MAX;
    if (m_osc_freq_modded < -OSC_FO_MAX)
      m_osc_freq_modded = -OSC_FO_MAX;

    // --- calculate increment (a.k.a. phase a.m_k.a. phaseIncrement, etc...)
    m_increment = m_osc_freq_modded / m_samplerate;
  }

  inline virtual void setGlideTargetFrequency(float p_freq) {
    m_osc_freq_glide_target = p_freq;
  }

  inline virtual void setBaseFrequency(float p_freq) {
    m_osc_freq_base = p_freq;
  }

  //moves the freq to glide target on osc change, otherwise after oscillator change the oscs glide again
  void killGlide(){
    m_osc_freq_base = m_osc_freq_glide_target;
  }

  void setPitchModExpPointer(float *p_pointer) { m_pitch_mod_exp = p_pointer; }
  void setPitchModLinPointer(float *p_pointer) { m_pitch_mod_lin = p_pointer; }
  void setVolModPointer(float *p_pointer) { m_vol_mod = p_pointer; }
  void setPitchBendPointer(float *p_pointer) { m_pitchbend = p_pointer; }
  void setGlidePointer(float *p_pointer) { m_glide_mod = p_pointer; }

  void setGlide(float p_glide) {
    m_glide = p_glide;
  }

  inline double calculateGlide(float p_glide){
    if (p_glide > 0.01) {
      return 0.9985f + p_glide * 0.0014;
    } else {
      return 0.;//todo this is correct....?
    }
  }

public:
	bool m_reset_active = false;
  // bool m_note_on; // --- oscillator run flag
  double m_osc_freq_glide_target; // the target freq after glide from MIDI note
                                  // number
  double
      m_osc_freq_base; // oscillator frequency which glides towards glide target
  double m_amplitude;  // 0->1 from GUI
  double m_modulo;     // modulo counter 0->1
  double m_increment;  // phase inc = fo/fs

  double m_mod_freq_exp; // modulation input -1 to +1 */
  double m_mod_freq_lin; // FM modulation input -1 to +1 (not actually used in
                         // Yamaha FM!) */

  float m_glide = 0.;
  float *m_glide_mod;

  float *m_pitch_mod_exp;
  float *m_pitch_mod_lin;
  float *m_vol_mod;
  float *m_pitchbend;

  int m_octave;    // octave tweak
  int m_semitones; // semitones tweak
  int m_cent;      // cents tweak

  //bool m_reset = false; // if this is true, osc will reset on noteon

  bool m_is_master_osc; // flag indicating we are a master oscillator

  Oscillator *m_buddy_oscillator;

protected:
  double m_samplerate;      // fs
  double m_osc_freq_modded; // current (actual) frequency of oscillator

  double m_mod_pitch_bend; // modulation input -1 to +1 */
  double m_mod_amp; // output amplitude modulation for AM 0 to +1 (not dB)*/
  float m_mod_exp_other =
      0.f; // can be used for whatever (Chiptune Arp uses it)
};
