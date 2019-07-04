#include "ADSR.h"

#include "../JuceLibraryCode/JuceHeader.h"

ADSREnvelope::ADSREnvelope() {
  // m_cheap_functions = CheapFunctions::getInstance();
}

ADSREnvelope::~ADSREnvelope() {}

float ADSREnvelope::doEnvelope() {
  if (m_current_section == 4) {
    m_last_actual_value = 0.f;
    return 0.f;
  }

  if (m_current_section == -1) {
    m_attack_start_value = 0.f;
    m_current_section = 0; // go to attack
    m_current_value = 0.;
  }

  if (m_current_section == 0) { // attack
    double attack_modded = m_attack;
    if (*m_attack_mod) {
      attack_modded *= calcModFactor(*m_attack_mod);
    }
    m_current_value += 1. / m_samplerate / attack_modded;
    if (m_current_value >= 1) {
      m_current_value = 1;
      m_current_section = 1; // move to decay
    }
    m_last_actual_value = m_current_value;

    return m_last_actual_value;
  }

  else if (m_current_section == 1) { // decay
    double decay_modded = m_decay;
    if (*m_decay_mod) {
      decay_modded *= calcModFactor(*m_decay_mod);
    }

    // just decay to zero but return scaled
    m_decay_factor = calcDecayFactor(decay_modded);
    m_current_value *= m_decay_factor;
    double sustain_modded = m_sustain + *m_sustain_mod;
    sustain_modded = sustain_modded < 0 ? 0 : sustain_modded;
    sustain_modded = sustain_modded > 1 ? 1 : sustain_modded;
    if (m_current_value < MIN_DECAY_RELEASE_VAL) {
      if (m_loop) {
        m_current_section = 0; // go back to attack
        m_attack_start_value = sustain_modded;
        m_current_value =
            sustain_modded + (1 - sustain_modded) * m_current_value;

        m_last_actual_value = sustain_modded;
        return m_last_actual_value;
      } else {
        m_current_section = 2; // go to sustain
        m_current_value = 0;   // why?
      }
    }
    m_last_actual_value = sustain_modded + (1 - sustain_modded) * m_current_value;
    return m_last_actual_value;
  }

  else if (m_current_section == 2) { // sustain
    double sustain_modded = m_sustain + *m_sustain_mod;
    sustain_modded = sustain_modded < 0 ? 0 : sustain_modded;
    sustain_modded = sustain_modded > 1 ? 1 : sustain_modded;
    // just return sustain here
    if (m_loop) {
      m_current_section = 0; // go back to attack
      m_attack_start_value = sustain_modded;
      m_current_value = sustain_modded + (1 - sustain_modded) * m_current_value;
    }
    m_last_actual_value = sustain_modded;
    return m_last_actual_value;
  }

  else if (m_current_section == 3) { // release
    double release_modded = m_release;
    if (*m_release_mod) {
      release_modded *= calcModFactor(*m_release_mod * 2);
    }

    m_release_factor = calcReleaseFactor(release_modded);
    // again just decay from 1 to 0 and output scaled version
    m_current_value *= m_release_factor;
    if (m_current_value < MIN_DECAY_RELEASE_VAL) {
      // envelope ends here...
      m_current_value = 0.;
      onEnvelopeEnd();
      m_current_section = 4; // envelope over
    }
    m_last_actual_value = m_current_value * m_release_start_value;
    return m_last_actual_value;
  }

  return 0.; // should never get here...
}

void ADSREnvelope::startRelease() {
  if (m_current_section == 3) {
    return;
  }
  double sustain_modded = m_sustain + *m_sustain_mod;
  sustain_modded = sustain_modded < 0 ? 0 : sustain_modded;
  sustain_modded = sustain_modded > 1 ? 1 : sustain_modded;
  if (m_current_section == 1) {
    m_release_start_value =
        m_current_value * (1. - sustain_modded) + sustain_modded;
  } else if (m_current_section == 2) {
    m_release_start_value = sustain_modded;
  } else {
    m_release_start_value = m_current_value;
  }

  m_current_value = 1.;
  m_current_section = 3; // set to release
}

int ADSREnvelope::getCurrentSection() { return m_current_section; }

