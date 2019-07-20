#include "NoiseOscillator.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "stdlib.h"

NoiseOscillator::NoiseOscillator() {
  m_lowpass.m_freq_base = FILTER_FC_MAX;
  m_lowpass.setLP();

  m_highpass.m_freq_base = FILTER_FC_MIN;
  m_highpass.setHP();

  m_lowpass.setSampleRate(44100.f);
  m_highpass.setSampleRate(44100.f);

  m_lowpass.reset();
  m_highpass.reset();
}

NoiseOscillator::~NoiseOscillator() {}

float NoiseOscillator::doNoise() {
  // if(!m_note_on){
  //    return 0.f;
  //}

  m_lowpass.update();
  m_highpass.update();

  float white_noise = (float)rand();
  white_noise = 2 * (white_noise / RAND_MAX) - 1;

  // do 2nd order like this?
  white_noise = m_lowpass.doFilter(white_noise);
  white_noise = m_highpass.doFilter(white_noise);

  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
  vol_mod_factor = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT
                                                        : vol_mod_factor;

  return white_noise * vol_mod_factor;
}

void NoiseOscillator::setFilterFreqs(float p_lowpass_freq,
                                     float p_highpass_freq) {
  m_lowpass.m_freq_base = p_lowpass_freq;
  m_highpass.m_freq_base = p_highpass_freq;
}

void NoiseOscillator::setHPFreq(float p_freq) {
  m_highpass.m_freq_base = p_freq;
}

void NoiseOscillator::setLPFreq(float p_freq) {
  m_lowpass.m_freq_base = p_freq;
}
