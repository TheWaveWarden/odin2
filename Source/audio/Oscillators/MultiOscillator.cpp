#include "MultiOscillator.h"

MultiOscillator::MultiOscillator() {
  m_nr_of_wavetables = 99;
}

MultiOscillator::~MultiOscillator() {}

float MultiOscillator::doOscillate() {
  // if (!m_note_on)
  //{
  //    return 0.f;
  //}
  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
  vol_mod_factor = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT
                                                        : vol_mod_factor;

  return doWavetableMulti() * vol_mod_factor;
}

void MultiOscillator::update() {
  float glide_input_modded = m_glide + *(m_glide_mod);
  glide_input_modded = glide_input_modded > 1 ? 1 : 0;
  double glide_modded = calculateGlide(glide_input_modded);

  m_osc_freq_base = m_osc_freq_glide_target * (1.f - glide_modded) +
                    (glide_modded)*m_osc_freq_base;
  // we overwrite even Oscillator here...
  m_osc_freq_modded =
      m_osc_freq_base *
      pitchShiftMultiplier(*m_pitchbend +
                           (*m_pitch_mod_exp) * OSC_EXP_MOD_RANGE +
                           m_mod_freq_exp + m_mod_exp_other + m_mod_pitch_bend +
                           m_octave * 12.0 + m_semitones + m_cent / 100.0);

  m_osc_freq_modded += (*m_pitch_mod_lin) * m_osc_freq_modded * 2;

  float detune_modded = m_detune + *m_detune_mod;
  detune_modded = detune_modded < 0 ? 0 : detune_modded;

  m_oscillator_freq_multi[0] =
      m_osc_freq_modded * cheapPitchShiftMultiplier(0.97f * detune_modded) +
      m_mod_freq_lin;
  m_oscillator_freq_multi[1] =
      m_osc_freq_modded * cheapPitchShiftMultiplier(-0.348f * detune_modded) +
      m_mod_freq_lin;
  m_oscillator_freq_multi[2] =
      m_osc_freq_modded * cheapPitchShiftMultiplier(0.238f * detune_modded) +
      m_mod_freq_lin;
  m_oscillator_freq_multi[3] =
      m_osc_freq_modded * cheapPitchShiftMultiplier(-1.f * detune_modded) +
      m_mod_freq_lin;

  // apply pitch mod to multioscs here...
  for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc) {
    if (m_oscillator_freq_multi[osc] > OSC_FO_MAX)
      m_oscillator_freq_multi[osc] = OSC_FO_MAX;
    if (m_oscillator_freq_multi[osc] < -OSC_FO_MAX)
      m_oscillator_freq_multi[osc] = -OSC_FO_MAX;

    m_wavetable_inc_multi[osc] =
        m_oscillator_freq_multi[osc] * m_one_over_samplerate * WAVETABLE_LENGTH;
  }

  // use derived getTableIndex which gets only table for the highest multiosc
  int table_index = getTableIndex(m_oscillator_freq_multi[0]);

  for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc) {
    m_current_table_2D_multi[osc] =
        m_wavetable_pointers_2D[m_wavetable_index]
                               [table_index];
  }
}

void MultiOscillator::reset() {
  WavetableOsc2D::reset();
  for (int subosc = 0; subosc < OSCS_PER_MULTIOSC; ++subosc) {
    m_read_index_multi[subosc] = 0;
  }
}

int MultiOscillator::getTableIndex(float p_freq) {
  float seed_freq = 27.5; // A0
  for (int table = 0; table < SUBTABLES_PER_WAVETABLE; table++) {
    if (p_freq < seed_freq) {
      return table;
    }
    seed_freq *= 1.1892f; // minor third up
  }
  return SUBTABLES_PER_WAVETABLE - 1; // never gets here
}
//int MultiOscillator::getTableIndex() {
//  for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc) {
//    float seed_freq = 27.5f; // Key A0
//    for (int table = 0; table < SUBTABLES_PER_WAVETABLE; table++) {
//      if (m_oscillator_freq_multi[osc] < seed_freq) {
//        m_sub_table_index_multi[osc] = table;
//        break;
//      }
//      seed_freq *= 1.18920f; // minor third up
//    }
// }
//  return 0; // return doesn't matter here
//}

float MultiOscillator::doWavetableMulti() {
  // set up all multiple variables, so we can run through osc loop
  int read_index_trunc_multi[OSCS_PER_MULTIOSC];
  int read_index_next_multi[OSCS_PER_MULTIOSC];
  float fractional_multi[OSCS_PER_MULTIOSC];
  int left_table_multi[OSCS_PER_MULTIOSC];
  int right_table_multi[OSCS_PER_MULTIOSC];
  float interpolation_value_multi[OSCS_PER_MULTIOSC];
  float output_left_multi[OSCS_PER_MULTIOSC];
  float output_right_multi[OSCS_PER_MULTIOSC];

  float output = 0.f;

  for (int osc = 0; osc < OSCS_PER_MULTIOSC; ++osc) {
    // table indices
    read_index_trunc_multi[osc] = (int)m_read_index_multi[osc];
    fractional_multi[osc] =
        m_read_index_multi[osc] - read_index_trunc_multi[osc];
    read_index_next_multi[osc] =
        read_index_trunc_multi[osc] + 1 >= WAVETABLE_LENGTH
            ? 0
            : read_index_trunc_multi[osc] + 1;

    // spread oscs over wavetable
    m_position_2D_multi[osc] =
        m_position_2D +
        ((float)osc - 1.5f) * (m_wavetable_multi_spread + (*m_spread_mod)) *
            0.5f +
        *m_pos_mod;
    m_position_2D_multi[osc] =
        m_position_2D_multi[osc] > 1.f ? 1.f : m_position_2D_multi[osc];
    m_position_2D_multi[osc] =
        m_position_2D_multi[osc] < 0.f ? 0.f : m_position_2D_multi[osc];

    getTableIndicesAndInterpolation(
        left_table_multi[osc], right_table_multi[osc],
        interpolation_value_multi[osc], m_position_2D_multi[osc]);

    // do linear interpolation
    output_left_multi[osc] = linearInterpolation(
        m_current_table_2D_multi[osc][left_table_multi[osc]]
                                [read_index_trunc_multi[osc]],
        m_current_table_2D_multi[osc][left_table_multi[osc]]
                                [read_index_next_multi[osc]],
        fractional_multi[osc]);
    output_right_multi[osc] = linearInterpolation(
        m_current_table_2D_multi[osc][right_table_multi[osc]]
                                [read_index_trunc_multi[osc]],
        m_current_table_2D_multi[osc][right_table_multi[osc]]
                                [read_index_next_multi[osc]],
        fractional_multi[osc]);

    m_read_index_multi[osc] += m_wavetable_inc_multi[osc];
    checkWrapIndex(m_read_index_multi[osc]);

    output += (1.f - interpolation_value_multi[osc]) * output_left_multi[osc] +
              interpolation_value_multi[osc] * output_right_multi[osc];
  }
  return output * 0.25f;
}

float MultiOscillator::cheapPitchShiftMultiplier(float p_semitones) {

  //2^(x/12) = e^(ln(2)/12*x)
  //ln(2)/12 = 0.057762265f
  p_semitones *= 0.057762265f;

  // use taylor series for e^x, O(n^4):
  // https://www.wolframalpha.com/input/?i=1+%2B+x+%2B+x%5E2%2F2%2B+x%5E3+%2F+6+%2B+x%5E4+%2F+24
  // use horners scheme for evaluating  x^4 function efficiently:
  // https://en.wikipedia.org/wiki/Horner%27s_method
  return 1.f + p_semitones * (1.f + p_semitones * (0.5f + p_semitones * (0.1666666f + p_semitones * 0.04166666)));


  //float pot = p_semitones;
  //float ret = 1 + p_semitones; // O(1)
  //pot *= p_semitones;
  //ret += pot * 0.5f; // O(2)
  //pot *= p_semitones;
  //return ret + pot * 0.16666f + pot * p_semitones * 0.041666667f; // O(3) & O(4)
}
