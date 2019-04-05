#include "FMOscillator.h"



FMOscillator::FMOscillator(){}


FMOscillator::~FMOscillator(){}


void FMOscillator::loadWavetables(){
    m_carrier_osc.loadWavetables();
    m_modulator_osc.loadWavetables();
}

void FMOscillator::update() {
    //do this Oscillator calculation first.
    Oscillator::update();

    //use modded freq of this osc for basefreq of child oscs
    m_carrier_osc.m_osc_freq_base = m_osc_freq_modded;
    m_modulator_osc.m_osc_freq_base = m_osc_freq_modded * m_modulator_ratio / m_carrier_ratio;

    //now do modulator osc so we can use it as modultion
    m_modulator_osc.update(); // TODO this uses pitchshiftmodulator although it needn't
    float mod_value = m_modulator_osc.doOscillate();

    //set mod and update
    if(m_FM_is_exponential){
        m_carrier_osc.m_mod_freq_exp = mod_value * m_FM_amount * EXP_FM_SEMITONES;
    } else {
        m_carrier_osc.m_mod_freq_lin = mod_value * 2 * m_osc_freq_base * m_FM_amount;
    }
    m_carrier_osc.update();
}


void FMOscillator::reset() {
    m_carrier_osc.reset();
    m_modulator_osc.reset();
}

float FMOscillator::doOscillate() {
    return m_carrier_osc.doOscillate();
}


//void FMOscillator::startOscillator(){
    //always reset this one to avoid unpredictable behaviour
    //reset();
    //m_note_on = true;
    //m_carrier_osc.startOscillator();
    //m_modulator_osc.startOscillator();
//}

//void FMOscillator::stopOscillator(){
//    //m_note_on = false;
//    m_carrier_osc.stopOscillator();
//    m_modulator_osc.stopOscillator();
//}
