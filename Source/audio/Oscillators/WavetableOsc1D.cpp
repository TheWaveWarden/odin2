#include "WavetableOsc1D.h"
#include <cmath>


#include "../JuceLibraryCode/JuceHeader.h"


WavetableOsc1D::WavetableOsc1D(){}


WavetableOsc1D::~WavetableOsc1D(){}

void WavetableOsc1D::setVoldB(float dB){
    //TODO lookup table
    m_volume_factor = pow(10, dB / 20);
}

void WavetableOsc1D::reset(){

    //call baseclass first
    Oscillator::reset();

    m_read_index = 0.0;
}

void WavetableOsc1D::update(){

    //baseclass call
    Oscillator::update();

    m_wavetable_inc = WAVETABLE_LENGTH * m_increment;
    m_sub_table_index = getTableIndex();

    //set wavetable pointer
    m_current_table = m_wavetable_pointers[m_wavetable_index][m_sub_table_index];
}

int WavetableOsc1D::getTableIndex(){
    
    double seed_freq = 27.5; //A0
    float abs_freq = fabs(m_osc_freq_modded);
    for(int table = 0; table < SUBTABLES_PER_WAVETABLE; table++){
        if(abs_freq < seed_freq){
            return table;
        }
        seed_freq *= 1.1892071150; //minor third up
    }

    return SUBTABLES_PER_WAVETABLE -1;//never gets here
}

float WavetableOsc1D::doWavetable(){
    
    //prepare both sides and interpol value
    int read_index_trunc = (int) m_read_index;
    float fractional = m_read_index - (float)read_index_trunc;
    int read_index_next = read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;

    //DBG((long)m_current_table);

    // do linear interpolation 
    float output = linearInterpolation(m_current_table[read_index_trunc], m_current_table[read_index_next], fractional);

    m_read_index += m_wavetable_inc;
    checkWrapIndex(m_read_index);

    return output;
}

float WavetableOsc1D::doOscillate(){
    //if(!m_note_on){
    //    return 0.f;
    //}
    return doWavetable() * m_volume_factor;
}

void WavetableOsc1D::setWavetablePointer(int p_wavetable_index, float* p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]){
    for (int sub_table = 0; sub_table < SUBTABLES_PER_WAVETABLE; sub_table++){
		m_wavetable_pointers[p_wavetable_index][sub_table] = p_wavetable_pointers[sub_table];
    }
}

void WavetableOsc1D::selectWavetable(int p_wavetable_index){
    if(p_wavetable_index < m_nr_of_wavetables){
        m_wavetable_index = p_wavetable_index;
    }
}

void WavetableOsc1D::loadWavetables(){
    //for standard osc simply get all wavetables
    for(int wavetable = 0; wavetable < NUMBER_OF_WAVETABLES; ++wavetable){
		setWavetablePointer(wavetable, WavetableContainer::getInstance().getWavetablePointers(wavetable));
	}
}

void WavetableOsc1D::loadWavedrawTables(int p_osc){
    //for(int wavetable = 0; wavetable < NUMBER_OF_WAVETABLES; ++wavetable){
	setWavetablePointer(0, WavetableContainer::getInstance().getWavedrawPointer(p_osc));
    m_nr_of_wavetables = 1;
	//}
}