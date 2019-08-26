#include "WavetableOsc2D.h"

WavetableOsc2D::WavetableOsc2D() {
  m_nr_of_wavetables = NUMBER_OF_WAVETABLES_2D;
}

WavetableOsc2D::~WavetableOsc2D() {}

void WavetableOsc2D::loadWavetables() {
  setWavetablePointer(
      0, 0, WavetableContainer::getInstance().getWavetablePointers("Saw"));
  setWavetablePointer(0, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50"));
  setWavetablePointer(
      0, 2, WavetableContainer::getInstance().getWavetablePointers("Triangle"));
  setWavetablePointer(
      0, 3, WavetableContainer::getInstance().getWavetablePointers("Sine"));

  setWavetablePointer(
      1, 0,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine1"));
  setWavetablePointer(
      1, 1,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine2"));
  setWavetablePointer(
      1, 2,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine3"));
  setWavetablePointer(
      1, 3,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine4"));

  setWavetablePointer(
      2, 0, WavetableContainer::getInstance().getWavetablePointers("Skyline1"));
  setWavetablePointer(
      2, 1, WavetableContainer::getInstance().getWavetablePointers("Skyline2"));
  setWavetablePointer(
      2, 2, WavetableContainer::getInstance().getWavetablePointers("Skyline3"));
  setWavetablePointer(
      2, 3, WavetableContainer::getInstance().getWavetablePointers("Skyline4"));

  setWavetablePointer(
      3, 0,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace1"));
  setWavetablePointer(
      3, 1,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace2"));
  setWavetablePointer(
      3, 2,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace3"));
  setWavetablePointer(
      3, 3,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace4"));

  setWavetablePointer(
      4, 0,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular1"));
  setWavetablePointer(
      4, 1,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular2"));
  setWavetablePointer(
      4, 2,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular3"));
  setWavetablePointer(
      4, 3,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular4"));

  setWavetablePointer(5, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0010"));
  setWavetablePointer(5, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0011"));
  setWavetablePointer(5, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0014"));
  setWavetablePointer(5, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0004"));

  setWavetablePointer(
      6, 0,
      WavetableContainer::getInstance().getWavetablePointers("BagPipe"));
  setWavetablePointer(
      6, 1,
      WavetableContainer::getInstance().getWavetablePointers("BagPipeMutated1"));
  setWavetablePointer(
      6, 2,
      WavetableContainer::getInstance().getWavetablePointers("BagPipeMutated5"));
  setWavetablePointer(
      6, 3,
      WavetableContainer::getInstance().getWavetablePointers("BagPipeMutated4"));

  setWavetablePointer(7, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0011"));
  setWavetablePointer(7, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0032"));
  setWavetablePointer(7, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0034"));
  setWavetablePointer(7, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0081"));

  setWavetablePointer(
      8, 0, WavetableContainer::getInstance().getWavetablePointers("Sparse1"));
  setWavetablePointer(
      8, 1, WavetableContainer::getInstance().getWavetablePointers("Sparse2"));
  setWavetablePointer(
      8, 2, WavetableContainer::getInstance().getWavetablePointers("Sparse3"));
  setWavetablePointer(
      8, 3, WavetableContainer::getInstance().getWavetablePointers("Sparse4"));

  setWavetablePointer(9, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_bitreduced_0002"));
  setWavetablePointer(9, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_bitreduced_0003"));
  setWavetablePointer(9, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_bitreduced_0006"));
  setWavetablePointer(9, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_bitreduced_0011"));

  setWavetablePointer(
      10, 0, WavetableContainer::getInstance().getWavetablePointers("Violin1"));
  setWavetablePointer(
      10, 1, WavetableContainer::getInstance().getWavetablePointers("Violin2"));
  setWavetablePointer(
      10, 2, WavetableContainer::getInstance().getWavetablePointers("Cello1"));
  setWavetablePointer(
      10, 3, WavetableContainer::getInstance().getWavetablePointers("Cello2"));

  setWavetablePointer(
      11, 0, WavetableContainer::getInstance().getWavetablePointers("Piano1"));
  setWavetablePointer(
      11, 1, WavetableContainer::getInstance().getWavetablePointers("Piano2"));
  setWavetablePointer(
      11, 2, WavetableContainer::getInstance().getWavetablePointers("Piano3"));
  setWavetablePointer(
      11, 3, WavetableContainer::getInstance().getWavetablePointers("Piano4"));

  setWavetablePointer(
      12, 0, WavetableContainer::getInstance().getWavetablePointers("Organ1"));
  setWavetablePointer(
      12, 1, WavetableContainer::getInstance().getWavetablePointers("Organ2"));
  setWavetablePointer(
      12, 2, WavetableContainer::getInstance().getWavetablePointers("Organ3"));
  setWavetablePointer(
      12, 3, WavetableContainer::getInstance().getWavetablePointers("Organ4"));

  setWavetablePointer(
      13, 0, WavetableContainer::getInstance().getWavetablePointers("Oboe1"));
  setWavetablePointer(
      13, 1, WavetableContainer::getInstance().getWavetablePointers("Oboe2"));
  setWavetablePointer(
      13, 2, WavetableContainer::getInstance().getWavetablePointers("Oboe3"));
  setWavetablePointer(
      13, 3, WavetableContainer::getInstance().getWavetablePointers("Oboe4"));

  setWavetablePointer(
      14, 0, WavetableContainer::getInstance().getWavetablePointers("Flute1"));
  setWavetablePointer(
      14, 1, WavetableContainer::getInstance().getWavetablePointers("Flute2"));
  setWavetablePointer(
      14, 2, WavetableContainer::getInstance().getWavetablePointers("Flute3"));
  setWavetablePointer(
      14, 3, WavetableContainer::getInstance().getWavetablePointers("Flute4"));

  setWavetablePointer(
      15, 0,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet1"));
  setWavetablePointer(
      15, 1,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet2"));
  setWavetablePointer(
      15, 2,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet3"));
  setWavetablePointer(
      15, 3,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet4"));

  setWavetablePointer(16, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0002"));
  setWavetablePointer(16, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0010"));
  setWavetablePointer(16, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0014"));
  setWavetablePointer(16, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0019"));

  setWavetablePointer(17, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0020"));
  setWavetablePointer(17, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0021"));
  setWavetablePointer(17, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0029"));
  setWavetablePointer(17, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0032"));

  setWavetablePointer(18, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0037"));
  setWavetablePointer(18, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0041"));
  setWavetablePointer(18, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0047"));
  setWavetablePointer(18, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0049"));

  setWavetablePointer(19, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0056"));
  setWavetablePointer(19, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0064"));
  setWavetablePointer(19, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0071"));
  setWavetablePointer(19, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0093"));

  setWavetablePointer(
      20, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive1"));
  setWavetablePointer(
      20, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive2"));
  setWavetablePointer(
      20, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive3"));
  setWavetablePointer(
      20, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive4"));

  setWavetablePointer(
      21, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive5"));
  setWavetablePointer(
      21, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive6"));
  setWavetablePointer(
      21, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive7"));
  setWavetablePointer(
      21, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive8"));

  setWavetablePointer(
      22, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive9"));
  setWavetablePointer(
      22, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive10"));
  setWavetablePointer(
      22, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive11"));
  setWavetablePointer(
      22, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive12"));

  setWavetablePointer(
      23, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive13"));
  setWavetablePointer(
      23, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive14"));
  setWavetablePointer(
      23, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive15"));
  setWavetablePointer(
      23, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive16"));

  setWavetablePointer(24, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "Harmonics9")); // overtones 1-4
  setWavetablePointer(
      24, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics10"));
  setWavetablePointer(
      24, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics11"));
  setWavetablePointer(
      24, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics12"));

  setWavetablePointer(25, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "Harmonics13")); // overtones 5-9
  setWavetablePointer(
      25, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics14"));
  setWavetablePointer(
      25, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics15"));
  setWavetablePointer(
      25, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics16"));

  setWavetablePointer(
      26, 0,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics5"));
  setWavetablePointer(
      26, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics6"));
  setWavetablePointer(
      26, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics7"));
  setWavetablePointer(
      26, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics8"));

  setWavetablePointer(
      27, 0,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics1"));
  setWavetablePointer(
      27, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics2"));
  setWavetablePointer(
      27, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics3"));
  setWavetablePointer(
      27, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics4"));

  setWavetablePointer(
      28, 0,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated1"));
  setWavetablePointer(
      28, 1,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated2"));
  setWavetablePointer(
      28, 2,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated3"));
  setWavetablePointer(
      28, 3,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated4"));

  setWavetablePointer(
      29, 0,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated5"));
  setWavetablePointer(
      29, 1,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated6"));
  setWavetablePointer(
      29, 2,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated7"));
  setWavetablePointer(
      29, 3,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated8"));

  setWavetablePointer(30, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated1"));
  setWavetablePointer(30, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated2"));
  setWavetablePointer(30, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated3"));
  setWavetablePointer(30, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated4"));

  setWavetablePointer(31, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated5"));
  setWavetablePointer(31, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated6"));
  setWavetablePointer(31, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated7"));
  setWavetablePointer(31, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated8"));

  setWavetablePointer(
      32, 0, WavetableContainer::getInstance().getWavetablePointers("Perlin1"));
  setWavetablePointer(
      32, 1, WavetableContainer::getInstance().getWavetablePointers("Perlin2"));
  setWavetablePointer(
      32, 2, WavetableContainer::getInstance().getWavetablePointers("Perlin3"));
  setWavetablePointer(
      32, 3, WavetableContainer::getInstance().getWavetablePointers("Perlin4"));

  setWavetablePointer(
      33, 0, WavetableContainer::getInstance().getWavetablePointers("Perlin5"));
  setWavetablePointer(
      33, 1, WavetableContainer::getInstance().getWavetablePointers("Perlin6"));
  setWavetablePointer(
      33, 2, WavetableContainer::getInstance().getWavetablePointers("Perlin7"));
  setWavetablePointer(
      33, 3, WavetableContainer::getInstance().getWavetablePointers("Perlin8"));

  setWavetablePointer(
      34, 0, WavetableContainer::getInstance().getWavetablePointers("Perlin9"));
  setWavetablePointer(
      34, 1,
      WavetableContainer::getInstance().getWavetablePointers("Perlin10"));
  setWavetablePointer(
      34, 2,
      WavetableContainer::getInstance().getWavetablePointers("Perlin11"));
  setWavetablePointer(
      34, 3,
      WavetableContainer::getInstance().getWavetablePointers("Perlin12"));
}

float WavetableOsc2D::doOscillate() {
  // if(!m_note_on){
  //    return 0.f;
  //}
  float vol_mod_factor =
      (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
  vol_mod_factor = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT
                                                        : vol_mod_factor;

  return doWavetable2D() * vol_mod_factor;
}

void WavetableOsc2D::update() {
  // overwrite implementation of Wave1D entirely

  Oscillator::update();

  m_wavetable_inc = WAVETABLE_LENGTH * m_increment;
  m_sub_table_index = getTableIndex();

  // set wavetable pointer
  m_current_table_2D =
      m_wavetable_pointers_2D[m_wavetable_index][m_sub_table_index];
}

void WavetableOsc2D::setWavetablePointer(
    int p_wavetable_index, int p_2D_sub_table,
    const float *p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]) {
  for (int sub_table = 0; sub_table < SUBTABLES_PER_WAVETABLE; sub_table++) {
    m_wavetable_pointers_2D[p_wavetable_index][sub_table][p_2D_sub_table] =
        p_wavetable_pointers[sub_table];
  }
}

float WavetableOsc2D::doWavetable2D() {
  // prepare both sides and interpol value
  int read_index_trunc = (int)m_read_index;
  float fractional = m_read_index - (float)read_index_trunc;
  int read_index_next =
      read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;

  // prepare variables for double wavetable accesses
  int left_table;
  int right_table;
  float interpolation_value;
  float position_modded = m_position_2D + (*m_pos_mod);
  position_modded = position_modded > 1 ? 1 : position_modded;
  position_modded = position_modded < 0 ? 0 : position_modded;
  getTableIndicesAndInterpolation(left_table, right_table, interpolation_value,
                                  position_modded);

  // do linear interpolation
  float output_left = linearInterpolation(
      m_current_table_2D[left_table][read_index_trunc],
      m_current_table_2D[left_table][read_index_next], fractional);
  float output_right = linearInterpolation(
      m_current_table_2D[right_table][read_index_trunc],
      m_current_table_2D[right_table][read_index_next], fractional);

  m_read_index += m_wavetable_inc;
  checkWrapIndex(m_read_index);

  return (1.f - interpolation_value) * output_left +
         interpolation_value * output_right;
}
