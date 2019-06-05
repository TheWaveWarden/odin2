#include "WavetableOsc2D.h"



WavetableOsc2D::WavetableOsc2D()
{
    m_nr_of_wavetables = 20;
}


WavetableOsc2D::~WavetableOsc2D()
{
}


void WavetableOsc2D::loadWavetables(){
    setWavetablePointer(0, 0, WavetableContainer::getInstance().getWavetablePointers("Saw"));
    setWavetablePointer(0, 1, WavetableContainer::getInstance().getWavetablePointers("ChiptuneSquare50"));
    setWavetablePointer(0, 2, WavetableContainer::getInstance().getWavetablePointers("Triangle"));
    setWavetablePointer(0, 3, WavetableContainer::getInstance().getWavetablePointers("Sine"));


    setWavetablePointer(1, 0, WavetableContainer::getInstance().getWavetablePointers("Additive1"));
    setWavetablePointer(1, 1, WavetableContainer::getInstance().getWavetablePointers("Additive2"));
    setWavetablePointer(1, 2, WavetableContainer::getInstance().getWavetablePointers("Additive3"));
    setWavetablePointer(1, 3, WavetableContainer::getInstance().getWavetablePointers("Additive4"));

    setWavetablePointer(2, 0, WavetableContainer::getInstance().getWavetablePointers("Additive5"));
    setWavetablePointer(2, 1, WavetableContainer::getInstance().getWavetablePointers("Additive6"));
    setWavetablePointer(2, 2, WavetableContainer::getInstance().getWavetablePointers("Additive7"));
    setWavetablePointer(2, 3, WavetableContainer::getInstance().getWavetablePointers("Additive8"));

    setWavetablePointer(3, 0, WavetableContainer::getInstance().getWavetablePointers("Additive9"));
    setWavetablePointer(3, 1, WavetableContainer::getInstance().getWavetablePointers("Additive10"));
    setWavetablePointer(3, 2, WavetableContainer::getInstance().getWavetablePointers("Additive11"));
    setWavetablePointer(3, 3, WavetableContainer::getInstance().getWavetablePointers("Additive12"));

    setWavetablePointer(4, 0, WavetableContainer::getInstance().getWavetablePointers("Additive13"));
    setWavetablePointer(4, 1, WavetableContainer::getInstance().getWavetablePointers("Additive14"));
    setWavetablePointer(4, 2, WavetableContainer::getInstance().getWavetablePointers("Additive15"));
    setWavetablePointer(4, 3, WavetableContainer::getInstance().getWavetablePointers("Additive16"));

    setWavetablePointer(5, 0, WavetableContainer::getInstance().getWavetablePointers("Harmonics9"));//overtones 1-4
    setWavetablePointer(5, 1, WavetableContainer::getInstance().getWavetablePointers("Harmonics10"));
    setWavetablePointer(5, 2, WavetableContainer::getInstance().getWavetablePointers("Harmonics11"));
    setWavetablePointer(5, 3, WavetableContainer::getInstance().getWavetablePointers("Harmonics12"));

    setWavetablePointer(6, 0, WavetableContainer::getInstance().getWavetablePointers("Harmonics13"));//overtones 5-9
    setWavetablePointer(6, 1, WavetableContainer::getInstance().getWavetablePointers("Harmonics14"));
    setWavetablePointer(6, 2, WavetableContainer::getInstance().getWavetablePointers("Harmonics15"));
    setWavetablePointer(6, 3, WavetableContainer::getInstance().getWavetablePointers("Harmonics16"));

    setWavetablePointer(7, 0, WavetableContainer::getInstance().getWavetablePointers("Harmonics5"));
    setWavetablePointer(7, 1, WavetableContainer::getInstance().getWavetablePointers("Harmonics6"));
    setWavetablePointer(7, 2, WavetableContainer::getInstance().getWavetablePointers("Harmonics7"));
    setWavetablePointer(7, 3, WavetableContainer::getInstance().getWavetablePointers("Harmonics8"));

    setWavetablePointer(8, 0, WavetableContainer::getInstance().getWavetablePointers("Harmonics1"));
    setWavetablePointer(8, 1, WavetableContainer::getInstance().getWavetablePointers("Harmonics2"));
    setWavetablePointer(8, 2, WavetableContainer::getInstance().getWavetablePointers("Harmonics3"));
    setWavetablePointer(8, 3, WavetableContainer::getInstance().getWavetablePointers("Harmonics4"));

    setWavetablePointer(9, 0, WavetableContainer::getInstance().getWavetablePointers("Organ1"));
    setWavetablePointer(9, 1, WavetableContainer::getInstance().getWavetablePointers("Organ2"));
    setWavetablePointer(9, 2, WavetableContainer::getInstance().getWavetablePointers("Organ3"));
    setWavetablePointer(9, 3, WavetableContainer::getInstance().getWavetablePointers("Organ4"));

    setWavetablePointer(10, 0, WavetableContainer::getInstance().getWavetablePointers("BrokenSine1"));
    setWavetablePointer(10, 1, WavetableContainer::getInstance().getWavetablePointers("BrokenSine2"));
    setWavetablePointer(10, 2, WavetableContainer::getInstance().getWavetablePointers("BrokenSine3"));
    setWavetablePointer(10, 3, WavetableContainer::getInstance().getWavetablePointers("BrokenSine4"));

    setWavetablePointer(11, 0, WavetableContainer::getInstance().getWavetablePointers("Skyline1"));
    setWavetablePointer(11, 1, WavetableContainer::getInstance().getWavetablePointers("Skyline2"));
    setWavetablePointer(11, 2, WavetableContainer::getInstance().getWavetablePointers("Skyline3"));
    setWavetablePointer(11, 3, WavetableContainer::getInstance().getWavetablePointers("Skyline4"));

    setWavetablePointer(12, 0, WavetableContainer::getInstance().getWavetablePointers("Soft1"));
    setWavetablePointer(12, 1, WavetableContainer::getInstance().getWavetablePointers("Soft2"));
    setWavetablePointer(12, 2, WavetableContainer::getInstance().getWavetablePointers("Soft3"));
    setWavetablePointer(12, 3, WavetableContainer::getInstance().getWavetablePointers("Soft4"));

    setWavetablePointer(13, 0, WavetableContainer::getInstance().getWavetablePointers("MultiSaw1"));
    setWavetablePointer(13, 1, WavetableContainer::getInstance().getWavetablePointers("MultiSaw2"));
    setWavetablePointer(13, 2, WavetableContainer::getInstance().getWavetablePointers("MultiSaw3"));
    setWavetablePointer(13, 3, WavetableContainer::getInstance().getWavetablePointers("MultiSaw4"));

    setWavetablePointer(14, 0, WavetableContainer::getInstance().getWavetablePointers("Rectangular1"));
    setWavetablePointer(14, 1, WavetableContainer::getInstance().getWavetablePointers("Rectangular2"));
    setWavetablePointer(14, 2, WavetableContainer::getInstance().getWavetablePointers("Rectangular3"));
    setWavetablePointer(14, 3, WavetableContainer::getInstance().getWavetablePointers("Rectangular4"));
}


float WavetableOsc2D::doOscillate(){
    //if(!m_note_on){
    //    return 0.f;
    //}
    float vol_mod_factor = (*m_vol_mod) > 0 ? 1.f + 4 *(*m_vol_mod) : (1.f + *m_vol_mod);
    
    return doWavetable2D() * vol_mod_factor;
}

void WavetableOsc2D::update(){
    //overwrite implementation of Wave1D entirely

    Oscillator::update();

    m_wavetable_inc = WAVETABLE_LENGTH * m_increment;
    m_sub_table_index = getTableIndex();

    //set wavetable pointer
    m_current_table_2D = m_wavetable_pointers_2D[m_wavetable_index][m_sub_table_index];

}


void WavetableOsc2D::setWavetablePointer(int p_wavetable_index, int p_2D_sub_table, const float* p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]){
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
    float position_modded = m_position_2D + (*m_pos_mod);
    position_modded = position_modded > 1 ? 1 : position_modded;
    position_modded = position_modded < 0 ? 0 : position_modded;
    getTableIndicesAndInterpolation(left_table, right_table, interpolation_value, position_modded);
    
    // do linear interpolation 
    float output_left = linearInterpolation(m_current_table_2D[left_table][read_index_trunc], m_current_table_2D[left_table][read_index_next], fractional);
    float output_right = linearInterpolation(m_current_table_2D[right_table][read_index_trunc], m_current_table_2D[right_table][read_index_next], fractional);

    m_read_index += m_wavetable_inc;
    checkWrapIndex(m_read_index);

    return (1.f - interpolation_value) * output_left + interpolation_value * output_right;
}


	