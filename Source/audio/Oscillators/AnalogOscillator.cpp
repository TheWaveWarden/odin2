#include "AnalogOscillator.h"
#include <cstdlib>


#define DRIFT_NOISE_SMOOTH_FACTOR 0.0001

AnalogOscillator::AnalogOscillator()
{
    m_nr_of_wavetables = 4;
}

AnalogOscillator::~AnalogOscillator()
{
}

void AnalogOscillator::loadWavetables()
{
    //load the saw table for square as well since PWM is generated from saw
    setWavetablePointer(0, WavetableContainer::getInstance().getWavetablePointers("Saw")); //TODO FATSAW
    setWavetablePointer(1, WavetableContainer::getInstance().getWavetablePointers("Saw"));    
    setWavetablePointer(2, WavetableContainer::getInstance().getWavetablePointers("Triangle"));
    setWavetablePointer(3, WavetableContainer::getInstance().getWavetablePointers("Sine"));

    //init drift generator
    m_drift_generator.initialize(m_samplerate);
    //TODO ALL OSCILLATORS MUST RECEIVE SAMPLE RATE WHICH THEY DONT

    // std::ofstream out;
    // out.open("E:\\odin\\DEBUG.txt");
    // out << "pos;DATA;\n";
    // for(size_t i = 0; i < 200000; ++i){
    //     out << i << ";" << m_drift_generator.doDrift() << ";\n";
    // }
    // out.close();
}

float AnalogOscillator::generateDrift(){
    return m_drift_generator.doDrift();
}

void  AnalogOscillator::update(){
    m_mod_exp_other = generateDrift() * m_drift * DRIFT_RANGE_SEMITONES;
    //DBG(m_mod_exp_other);
    WavetableOsc1D::update();
}



float AnalogOscillator::doOscillate(){
    //if (!m_note_on){
    //    return 0.f;
    //}
    float vol_mod_factor = (*m_vol_mod) > 0 ? 1.f + 4 *(*m_vol_mod) : (1.f + *m_vol_mod);

    if(m_wavetable_index != 1){
        return doWavetable() * vol_mod_factor;
    }

    return doSquare()* vol_mod_factor;
}

float AnalogOscillator::doSquare(){
    //square is done by subtracting two saw waves
    //prepare both sides and interpol value for base and offset value
    double read_index_offset = m_read_index + (m_duty + (*pwm_mod) / 2) * WAVETABLE_LENGTH;
    checkWrapIndex(read_index_offset);

    int read_index_trunc = (int) m_read_index;
    int read_index_trunc_offset = (int) (read_index_offset);

    float fractional = m_read_index - (float)read_index_trunc;
    float fractional_offset = read_index_offset - (float)read_index_trunc_offset;

    int read_index_next = read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;
    int read_index_next_offset = read_index_trunc_offset + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc_offset + 1;

    // do linear interpolation 
    float output = linearInterpolation(m_current_table[read_index_trunc], m_current_table[read_index_next], fractional);
    float output_offset = linearInterpolation(m_current_table[read_index_trunc_offset], m_current_table[read_index_next_offset], fractional_offset);

    m_read_index += m_wavetable_inc;
    checkWrapIndex(m_read_index);

    return output - output_offset;//TODO scale, seems to clip for duty near 0 or 1
}


