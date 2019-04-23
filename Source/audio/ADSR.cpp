#include "ADSR.h"

// todo remove for profiling only
#include "../JuceLibraryCode/JuceHeader.h"
#include <ctime>

ADSREnvelope::ADSREnvelope() {
  // m_cheap_functions = CheapFunctions::getInstance();
}

ADSREnvelope::~ADSREnvelope() {}

float ADSREnvelope::doEnvelope() {
  if (m_current_section == -1) {
    m_attack_start_value = 0.f;
    m_current_section = 0; // go to attack
    m_current_value = 0.;
  }

  if (m_current_section == 0) { // attack
    m_current_value += (1. - m_attack_start_value) / m_samplerate / m_attack;
    DBG(m_current_value);
    if (m_current_value >= 1) {
      m_current_value = 1;
      m_current_section = 1; // move to decay
    }
    return m_current_value;
  }

  else if (m_current_section == 1) { // decay
    // just decay to zero but return scaled
    m_decay_factor = calcDecayFactorCheap(m_decay);
    m_current_value *= m_decay_factor;
    if (m_current_value < MIN_DECAY_RELEASE_VAL) {
      if (m_loop) {
        m_current_section = 0; // go back to attack
        m_attack_start_value = m_sustain;
        m_current_value = m_sustain + (1 - m_sustain) * m_current_value;
        return m_sustain;
      } else {
        m_current_section = 2; // go to sustain
        m_current_value = 0;
      }
    }
    return m_sustain + (1 - m_sustain) * m_current_value;
  }

  else if (m_current_section == 2) { // sustain
    // just return sustain here
    if (m_loop) {
      m_current_section = 0; // go back to attack
      m_attack_start_value = m_sustain;
      m_current_value = m_sustain + (1 - m_sustain) * m_current_value;
    }
    return m_sustain;
  }

  else if (m_current_section == 3) { // release
    m_release_factor = calcDecayFactorCheap(m_release);
    // again just decay from 1 to 0 and output scaled version
    m_current_value *= m_release_factor;
    if (m_current_value < MIN_DECAY_RELEASE_VAL) {
      // envelope ends here...
      m_current_value = 0.;
      onEnvelopeEnd();
      // if(m_envelope_finish_flag){
      //    *m_envelope_finish_flag = false;
      //}
    }

    return m_current_value * m_release_start_value;
  }

  return 0.; // should never get here...
}

void ADSREnvelope::startRelease() {
  if (m_current_section == 1) {
    m_release_start_value = m_current_value * (1. - m_sustain) + m_sustain;
  } else if (m_current_section == 2) {
    m_release_start_value = m_sustain;
  } else {
    m_release_start_value = m_current_value;
  }

  m_current_value = 1.;
  m_current_section = 3; // set to release
}

int ADSREnvelope::getCurrentSection() { return m_current_section; }

void ADSREnvelope::testADSRPow() {
  std::ofstream output;
  output.open("E:\\odin\\DEBUG.txt");
  for (size_t i = 0; i < 10000; ++i) {
    output << calcDecayFactor(((double)i + 0.5) * 0.001) -
                  calcDecayFactorCheap(((double)i + 0.5) * 0.001)
           << "," << calcDecayFactor(((double)i + 0.5) * 0.001) << ","
           << calcDecayFactorCheap(((double)i + 0.5) * 0.001) << ","
           << ((double)i + 0.5) * 0.001 << "\n";
  }
  output.close();
}

void ADSREnvelope::profileCheapPow() {

  using namespace std;
  clock_t cheap_begin = clock();
  for (size_t i = 0; i < 1000000000; ++i) {
    calcDecayFactorCheap((double)i / 10000000.);
  }
  clock_t cheap_end = clock();

  clock_t pow_begin = clock();
  for (size_t i = 0; i < 1000000000; ++i) {
    calcDecayFactor((double)i / 10000000.);
  }
  clock_t pow_end = clock();

  double elapsed_cheap = double(cheap_end - cheap_begin) / CLOCKS_PER_SEC;
  double elapsed_pow = double(pow_end - pow_begin) / CLOCKS_PER_SEC;

  std::ofstream output;
  output.open("E:\\odin\\DEBUG.txt");

  output << "Table: " << elapsed_cheap << " sek\n";
  output << "Pow: " << elapsed_pow << " sek\n";

  output.close();
}
