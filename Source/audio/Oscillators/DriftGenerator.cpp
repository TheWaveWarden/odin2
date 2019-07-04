#include "DriftGenerator.h"
#include "stdlib.h"
#include "../JuceLibraryCode/JuceHeader.h"//todo


DriftGenerator::DriftGenerator()
{

}


DriftGenerator::~DriftGenerator()
{
}


void DriftGenerator::initialize(float p_samlerate){
    
    m_samplerate = p_samlerate;
    m_drift_length = (int)(DRIFT_LENGTH_SECONDS * p_samlerate);

    m_last_value = getRand();
    m_next_value = getRand();

    //DBG(m_last_value);
    //DBG(m_next_value);

    // generate first coeffs random
    // float f_0 = getRand();
    // float f_strich_0 = 0;
    // float f_1 = getRand();
    
    // m_a = (f_1 - f_strich_0 * m_drift_length - f_0) / (m_drift_length * m_drift_length);
    // m_b = f_strich_0;
    // m_c = f_0; 
}

float DriftGenerator::doDrift(){
    if(++m_counter > m_drift_length){
        m_counter = 0;
        calcNewCoeffs();
    }

    return (m_next_value - m_last_value) * (float)m_counter / m_drift_length + m_last_value ;
}

void DriftGenerator::setSampleRate(float p_samplerate){ 
  DBG("SetSampleRate driftgen");
    m_samplerate = p_samplerate;
    m_drift_length = (int)(DRIFT_LENGTH_SECONDS * p_samplerate);
}


void DriftGenerator::calcNewCoeffs(){
     m_last_value = m_next_value;
     m_next_value = getRand();
}

float DriftGenerator::getRand(){
    //generate random in range -1 +1
    float random = (float)rand();
	return 2.f * ((float)random / (float)RAND_MAX) - 1.f;
}

