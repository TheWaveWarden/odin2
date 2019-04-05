#include "WavetableOsc2D.h"



WavetableOsc2D::WavetableOsc2D()
{
    m_nr_of_wavetables = 1;
}


WavetableOsc2D::~WavetableOsc2D()
{
}


void WavetableOsc2D::loadWavetables(){
    setWavetablePointer(0, 0, WavetableContainer::getInstance().getWavetablePointers("Saw"));
    setWavetablePointer(0, 1, WavetableContainer::getInstance().getWavetablePointers("ChiptuneSquare50"));
    setWavetablePointer(0, 2, WavetableContainer::getInstance().getWavetablePointers("Triangle"));
    setWavetablePointer(0, 3, WavetableContainer::getInstance().getWavetablePointers("Sine"));
}


float WavetableOsc2D::doOscillate(){
    //if(!m_note_on){
    //    return 0.f;
    //}

    return doWavetable2D();
}

void WavetableOsc2D::update(){
    //overwrite implementation of Wave1D entirely

    Oscillator::update();

    m_wavetable_inc = WAVETABLE_LENGTH * m_increment;
    m_sub_table_index = getTableIndex();

    //set wavetable pointer
    m_current_table_2D = m_wavetable_pointers_2D[m_wavetable_index][m_sub_table_index];

}


void WavetableOsc2D::setWavetablePointer(int p_wavetable_index, int p_2D_sub_table, float* p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]){
    for (int sub_table = 0; sub_table < SUBTABLES_PER_WAVETABLE; sub_table++){
		m_wavetable_pointers_2D[p_wavetable_index][sub_table][p_2D_sub_table] = p_wavetable_pointers[sub_table];
    }
}


float WavetableOsc2D::doWavetable2D(){
    //prepare both sides and interpol value
    int read_index_trunc = (int) m_read_index;
    float fractional = m_read_index - (float)read_index_trunc;
    int read_index_next = read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;

    //prepare variables for double wavetable accesses
    int left_table;
    int right_table;
    float interpolation_value;
    getTableIndicesAndInterpolation(left_table, right_table, interpolation_value, m_position_2D);

    
    // do linear interpolation 
    float output_left = linearInterpolation(m_current_table_2D[left_table][read_index_trunc], m_current_table_2D[left_table][read_index_next], fractional);
    float output_right = linearInterpolation(m_current_table_2D[right_table][read_index_trunc], m_current_table_2D[right_table][read_index_next], fractional);

    m_read_index += m_wavetable_inc;
    checkWrapIndex(m_read_index);

    return (1.f - interpolation_value) * output_left + interpolation_value * output_right;
}


	