#include "MultiOscillator.h"

MultiOscillator::MultiOscillator()
{
    m_nr_of_wavetables = 99;

    // generate table for cheap pitchshiftfunction
    
    // std::ofstream output;
    // output.open("E:\\odin\\pitchshift.h");
    // output << "double m_pitch_shift_table[1001] = {";
    // for(size_t x = 0; x < 1001; ++x){
    //     // use 100 points per semitone from -5 to +5
    //     output << pitchShiftMultiplier(((float)x - 500.f)/100.f) << ", ";
    // }
    // output << "};\n";
    // output.close();
}

MultiOscillator::~MultiOscillator()
{
}

// void MultiOscillator::loadWavetables()
// {
//     setWavetablePointer(0, 0, WavetableContainer::getInstance().getWavetablePointers("Saw"));
//     setWavetablePointer(0, 1, WavetableContainer::getInstance().getWavetablePointers("ChiptuneSquare50"));
//     setWavetablePointer(0, 2, WavetableContainer::getInstance().getWavetablePointers("Triangle"));
//     setWavetablePointer(0, 3, WavetableContainer::getInstance().getWavetablePointers("Sine"));
// }

float MultiOscillator::doOscillate()
{
    // if (!m_note_on)
    //{
    //    return 0.f;
    //}
    float vol_mod_factor = (*m_vol_mod) > 0 ? 1.f + 4 *(*m_vol_mod) : (1.f + *m_vol_mod);
    
    return doWavetableMulti() * vol_mod_factor;
}

void MultiOscillator::update()
{
        m_osc_freq_base =
        m_osc_freq_glide_target * (1.f - m_glide) + (m_glide)*m_osc_freq_base;
    //we overwrite even Oscillator here...
    m_osc_freq_modded = m_osc_freq_base * pitchShiftMultiplier(*m_pitchbend + (*m_pitch_mod_exp) * OSC_EXP_MOD_RANGE + m_mod_freq_exp + m_mod_exp_other +
                                            m_mod_pitch_bend +
                                            m_octave * 12.0 +
                                            m_semitones +
                                            m_cent / 100.0);

    m_osc_freq_modded += (*m_pitch_mod_lin) * m_osc_freq_modded * 2;

    float detune_modded = m_detune + *m_detune_mod;
    detune_modded = detune_modded < 0 ? 0 : detune_modded;

    //TODO THIS IS VERY BAD!!!!!
    m_oscillator_freq_multi[0] = m_osc_freq_modded * cheapPitchShiftMultiplier(-1.f * detune_modded) + m_mod_freq_lin;
    m_oscillator_freq_multi[1] = m_osc_freq_modded * cheapPitchShiftMultiplier(-0.348 * detune_modded) + m_mod_freq_lin;
    m_oscillator_freq_multi[2] = m_osc_freq_modded * cheapPitchShiftMultiplier(0.238 * detune_modded) + m_mod_freq_lin;
    m_oscillator_freq_multi[3] = m_osc_freq_modded * cheapPitchShiftMultiplier(1.f * detune_modded) + m_mod_freq_lin;

    //apply pitch mod to multioscs here...
    for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc)
    {
        if (m_oscillator_freq_multi[osc] > OSC_FO_MAX)
            m_oscillator_freq_multi[osc] = OSC_FO_MAX;
        if (m_oscillator_freq_multi[osc] < -OSC_FO_MAX)
            m_oscillator_freq_multi[osc] = -OSC_FO_MAX;

        m_wavetable_inc_multi[osc] = m_oscillator_freq_multi[osc] / m_samplerate * WAVETABLE_LENGTH;
    }

    //use derived getTableIndex which gets all four
    getTableIndex();

    for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc)
    {
        m_current_table_2D_multi[osc] = m_wavetable_pointers_2D[m_wavetable_index][m_sub_table_index_multi[osc]];
    }

}

void MultiOscillator::reset() {}

int MultiOscillator::getTableIndex()
{
    bool table_found[OSCS_PER_MULTIOSC] = {false};
    double seed_freq = 27.5; //A0

    for (int table = 0; table < SUBTABLES_PER_WAVETABLE; table++)
    {
        for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc)
        {
            if (!table_found[osc] && m_oscillator_freq_multi[osc] < seed_freq)
            {
                m_sub_table_index_multi[osc] = table;
                table_found[osc] = true;
            }
        }
        seed_freq *= 1.1892071150; //minor third up
    }
    return 0; //return doesn't matter here
}

float MultiOscillator::doWavetableMulti()
{
    //set up all multiple variables, so we can run through osc loop
    int read_index_trunc_multi[OSCS_PER_MULTIOSC];
    int read_index_next_multi[OSCS_PER_MULTIOSC];
    float fractional_multi[OSCS_PER_MULTIOSC];
    int left_table_multi[OSCS_PER_MULTIOSC];
    int right_table_multi[OSCS_PER_MULTIOSC];
    float interpolation_value_multi[OSCS_PER_MULTIOSC];
    float output_left_multi[OSCS_PER_MULTIOSC];
    float output_right_multi[OSCS_PER_MULTIOSC];

    float output = 0.f;

    for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc)
    {
        //table indices
        read_index_trunc_multi[osc] = (int)m_read_index_multi[osc];
        fractional_multi[osc] = m_read_index_multi[osc] - read_index_trunc_multi[osc];
        read_index_next_multi[osc] = read_index_trunc_multi[osc] + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc_multi[osc] + 1;

        //spread oscs over wavetable
        //TODO range... now is +-0.75
        m_position_2D_multi[osc] = m_position_2D + ((float)osc - 1.5f) * (m_wavetable_multi_spread + (*m_spread_mod)) * 0.5f + *m_pos_mod;
        m_position_2D_multi[osc] = m_position_2D_multi[osc] > 1.f ? 1.f : m_position_2D_multi[osc];
        m_position_2D_multi[osc] = m_position_2D_multi[osc] < 0.f ? 0.f : m_position_2D_multi[osc];

        getTableIndicesAndInterpolation(left_table_multi[osc], right_table_multi[osc], interpolation_value_multi[osc], m_position_2D_multi[osc]);

        // do linear interpolation
        output_left_multi[osc] = linearInterpolation(m_current_table_2D_multi[osc][left_table_multi[osc]][read_index_trunc_multi[osc]], m_current_table_2D_multi[osc][left_table_multi[osc]][read_index_next_multi[osc]], fractional_multi[osc]);
        output_right_multi[osc] = linearInterpolation(m_current_table_2D_multi[osc][right_table_multi[osc]][read_index_trunc_multi[osc]], m_current_table_2D_multi[osc][right_table_multi[osc]][read_index_next_multi[osc]], fractional_multi[osc]);
    
        m_read_index_multi[osc] += m_wavetable_inc_multi[osc];
        checkWrapIndex(m_read_index_multi[osc]);

        output += (1.f - interpolation_value_multi[osc]) * output_left_multi[osc] + interpolation_value_multi[osc] * output_right_multi[osc];
    }
    return output * 0.25f;
}


double MultiOscillator::cheapPitchShiftMultiplier(double p_semitones){
    
    if(p_semitones > 5. || p_semitones < -5.){
        //function is only suited for -5 to 5
        return pitchShiftMultiplier(p_semitones);
    }

    float index = (p_semitones + 5.) * 100.;
    int index_trunc = (int)index;
    int index_next = index + 1;
    float interpolation = index - (float)index_trunc;


    //never trust floating point operations
    index_trunc = index_trunc > 1000 ? 1000 : index_trunc;
    index_next = index_next > 1000 ? 1000 : index_next;
    index_trunc = index_trunc < 0 ? 0 : index_trunc;
    index_next = index_next < 0 ? 0 : index_next;

    return linearInterpolation(m_pitch_shift_table[index_trunc], m_pitch_shift_table[index_next], interpolation);
}
