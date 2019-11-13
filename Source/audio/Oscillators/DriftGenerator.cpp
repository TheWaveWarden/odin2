#include "DriftGenerator.h"
#include "stdlib.h"

DriftGenerator::DriftGenerator() {}

DriftGenerator::~DriftGenerator() {}

void DriftGenerator::initialize(float p_samlerate) {

  m_samplerate = p_samlerate;
  m_drift_length = (int)(DRIFT_LENGTH_SECONDS * p_samlerate);

  m_last_value = getRand();
  m_next_value = getRand();
}

float DriftGenerator::doDrift() {
  if (++m_counter > m_drift_length) {
    m_counter = 0;
    calcNewCoeffs();
  }

  return (m_next_value - m_last_value) * (float)m_counter / m_drift_length +
         m_last_value;
}

void DriftGenerator::setSampleRate(float p_samplerate) {
  m_samplerate = p_samplerate;
  m_one_over_samplerate = 1. / p_samplerate;
  m_drift_length = (int)(DRIFT_LENGTH_SECONDS * p_samplerate);
}

void DriftGenerator::calcNewCoeffs() {
  m_last_value = m_next_value;
  m_next_value = getRand();
}

float DriftGenerator::getRand() {
  float random = (float)rand();
  return 2.f * ((float)random / (float)RAND_MAX) - 1.f;
}
