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

  setWavetablePointer(1, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0010"));
  setWavetablePointer(1, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0011"));
  setWavetablePointer(1, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0014"));
  setWavetablePointer(1, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_birds_0004"));

  setWavetablePointer(
      2, 0, WavetableContainer::getInstance().getWavetablePointers("BagPipe"));
  setWavetablePointer(2, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "BagPipeMutated1"));
  setWavetablePointer(2, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "BagPipeMutated5"));
  setWavetablePointer(2, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "BagPipeMutated4"));

  setWavetablePointer(
      3, 0, WavetableContainer::getInstance().getWavetablePointers("Glass"));
  setWavetablePointer(
      3, 1,
      WavetableContainer::getInstance().getWavetablePointers("GlassMutated1"));
  setWavetablePointer(
      3, 2,
      WavetableContainer::getInstance().getWavetablePointers("GlassMutated2"));
  setWavetablePointer(
      3, 3,
      WavetableContainer::getInstance().getWavetablePointers("GlassMutated3"));

  setWavetablePointer(4, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0011"));
  setWavetablePointer(4, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0032"));
  setWavetablePointer(4, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0034"));
  setWavetablePointer(4, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_fmsynth_0081"));

  setWavetablePointer(
      5, 0,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine1"));
  setWavetablePointer(
      5, 1,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine2"));
  setWavetablePointer(
      5, 2,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine3"));
  setWavetablePointer(
      5, 3,
      WavetableContainer::getInstance().getWavetablePointers("BrokenSine4"));

  setWavetablePointer(
      6, 0, WavetableContainer::getInstance().getWavetablePointers("Skyline1"));
  setWavetablePointer(
      6, 1, WavetableContainer::getInstance().getWavetablePointers("Skyline2"));
  setWavetablePointer(
      6, 2, WavetableContainer::getInstance().getWavetablePointers("Skyline3"));
  setWavetablePointer(
      6, 3, WavetableContainer::getInstance().getWavetablePointers("Skyline4"));

  setWavetablePointer(
      7, 0,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace1"));
  setWavetablePointer(
      7, 1,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace2"));
  setWavetablePointer(
      7, 2,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace3"));
  setWavetablePointer(
      7, 3,
      WavetableContainer::getInstance().getWavetablePointers("PerlinReplace4"));

  setWavetablePointer(
      8, 0,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular1"));
  setWavetablePointer(
      8, 1,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular2"));
  setWavetablePointer(
      8, 2,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular3"));
  setWavetablePointer(
      8, 3,
      WavetableContainer::getInstance().getWavetablePointers("Rectangular4"));

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
      11, 0, WavetableContainer::getInstance().getWavetablePointers("Piano2"));
  setWavetablePointer(
      11, 1, WavetableContainer::getInstance().getWavetablePointers("Piano3"));
  setWavetablePointer(
      11, 2, WavetableContainer::getInstance().getWavetablePointers("Piano4"));
  setWavetablePointer(
      11, 3, WavetableContainer::getInstance().getWavetablePointers("Piano1"));

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
      14, 0,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet1"));
  setWavetablePointer(
      14, 1,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet2"));
  setWavetablePointer(
      14, 2,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet3"));
  setWavetablePointer(
      14, 3,
      WavetableContainer::getInstance().getWavetablePointers("Trumpet4"));

  setWavetablePointer(
      15, 0,
      WavetableContainer::getInstance().getWavetablePointers("LegToyBox"));
  setWavetablePointer(
      15, 1, WavetableContainer::getInstance().getWavetablePointers("LegRip2"));
  setWavetablePointer(
      15, 2, WavetableContainer::getInstance().getWavetablePointers("LegMale"));
  setWavetablePointer(
      15, 3,
      WavetableContainer::getInstance().getWavetablePointers("LegBarbedWire"));



  setWavetablePointer(
      16, 0, WavetableContainer::getInstance().getWavetablePointers("LegAdd8"));
  setWavetablePointer(
      16, 1,
      WavetableContainer::getInstance().getWavetablePointers("LegSharp"));
  setWavetablePointer(
      16, 2,
      WavetableContainer::getInstance().getWavetablePointers("LegPiano"));
  setWavetablePointer(
      16, 3, WavetableContainer::getInstance().getWavetablePointers("LegAdd1"));



  setWavetablePointer(
      17, 0,
      WavetableContainer::getInstance().getWavetablePointers("LegCello"));
  setWavetablePointer(
      17, 1, WavetableContainer::getInstance().getWavetablePointers("LegAah"));
  setWavetablePointer(
      17, 2,
      WavetableContainer::getInstance().getWavetablePointers("LegHarm2"));
  setWavetablePointer(
      17, 3,
      WavetableContainer::getInstance().getWavetablePointers("LegNoBass"));




  setWavetablePointer(
      18, 0, WavetableContainer::getInstance().getWavetablePointers("LegBags"));
  setWavetablePointer(
      18, 1,
      WavetableContainer::getInstance().getWavetablePointers("LegOrgan"));
  setWavetablePointer(
      18, 2,
      WavetableContainer::getInstance().getWavetablePointers("LegTriQuad"));
  setWavetablePointer(
      18, 3, WavetableContainer::getInstance().getWavetablePointers("LegAdd3"));

  setWavetablePointer(19, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0002"));
  setWavetablePointer(19, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0010"));
  setWavetablePointer(19, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0014"));
  setWavetablePointer(19, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0019"));

  setWavetablePointer(20, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0020"));
  setWavetablePointer(20, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0021"));
  setWavetablePointer(20, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0029"));
  setWavetablePointer(20, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0032"));

  setWavetablePointer(21, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0037"));
  setWavetablePointer(21, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0041"));
  setWavetablePointer(21, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0047"));
  setWavetablePointer(21, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0049"));

  setWavetablePointer(22, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0056"));
  setWavetablePointer(22, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0064"));
  setWavetablePointer(22, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0071"));
  setWavetablePointer(22, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "AKWF_hvoice_0093"));

  setWavetablePointer(
      23, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive1"));
  setWavetablePointer(
      23, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive2"));
  setWavetablePointer(
      23, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive3"));
  setWavetablePointer(
      23, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive4"));

  setWavetablePointer(
      24, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive5"));
  setWavetablePointer(
      24, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive6"));
  setWavetablePointer(
      24, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive7"));
  setWavetablePointer(
      24, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive8"));

  setWavetablePointer(
      25, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive10"));
  setWavetablePointer(
      25, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive11"));
  setWavetablePointer(
      25, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive12"));
  setWavetablePointer(
      25, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive9"));

  setWavetablePointer(
      26, 0,
      WavetableContainer::getInstance().getWavetablePointers("Additive13"));
  setWavetablePointer(
      26, 1,
      WavetableContainer::getInstance().getWavetablePointers("Additive14"));
  setWavetablePointer(
      26, 2,
      WavetableContainer::getInstance().getWavetablePointers("Additive15"));
  setWavetablePointer(
      26, 3,
      WavetableContainer::getInstance().getWavetablePointers("Additive16"));

  setWavetablePointer(27, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "Harmonics9")); // overtones 1-4
  setWavetablePointer(
      27, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics10"));
  setWavetablePointer(
      27, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics11"));
  setWavetablePointer(
      27, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics12"));

  setWavetablePointer(28, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "Harmonics13")); // overtones 5-9
  setWavetablePointer(
      28, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics14"));
  setWavetablePointer(
      28, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics15"));
  setWavetablePointer(
      28, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics16"));

  setWavetablePointer(
      29, 0,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics5"));
  setWavetablePointer(
      29, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics6"));
  setWavetablePointer(
      29, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics7"));
  setWavetablePointer(
      29, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics8"));

  setWavetablePointer(
      30, 0,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics1"));
  setWavetablePointer(
      30, 1,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics2"));
  setWavetablePointer(
      30, 2,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics3"));
  setWavetablePointer(
      30, 3,
      WavetableContainer::getInstance().getWavetablePointers("Harmonics4"));

  setWavetablePointer(
      31, 0,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated1"));
  setWavetablePointer(
      31, 1,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated2"));
  setWavetablePointer(
      31, 2,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated3"));
  setWavetablePointer(
      31, 3,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated4"));

  setWavetablePointer(
      32, 0,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated5"));
  setWavetablePointer(
      32, 1,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated6"));
  setWavetablePointer(
      32, 2,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated7"));
  setWavetablePointer(
      32, 3,
      WavetableContainer::getInstance().getWavetablePointers("FatSawMutated8"));

  setWavetablePointer(33, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated1"));
  setWavetablePointer(33, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated2"));
  setWavetablePointer(33, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated3"));
  setWavetablePointer(33, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated4"));

  setWavetablePointer(34, 0,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated5"));
  setWavetablePointer(34, 1,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated6"));
  setWavetablePointer(34, 2,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated7"));
  setWavetablePointer(34, 3,
                      WavetableContainer::getInstance().getWavetablePointers(
                          "ChiptuneSquare50Mutated8"));
}

std::string WavetableOsc2D::getWavetableName(int p_wt_2D, int sub_table_2D) {

  switch (p_wt_2D) {

  case 0:
    switch (sub_table_2D) {
    case 0:
      return "Saw";
      break;
    case 1:
      return "ChiptuneSquare50";
      break;
    case 2:
      return "Triangle";
      break;
    case 3:
      return "Sine";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;

  case 1:
    switch (sub_table_2D) {
    case 0:
      return "AKWF_birds_0010";
      break;
    case 1:
      return "AKWF_birds_0011";
      break;
    case 2:
      return "AKWF_birds_0014";
      break;
    case 3:
      return "AKWF_birds_0004";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 2:
    switch (sub_table_2D) {
    case 0:
      return "BagPipe";
      break;
    case 1:
      return "BagPipeMutated1";
      break;
    case 2:
      return "BagPipeMutated5";
      break;
    case 3:
      return "BagPipeMutated4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 3:
    switch (sub_table_2D) {
    case 0:
      return "Glass";
      break;
    case 1:
      return "GlassMutated1";
      break;
    case 2:
      return "GlassMutated2";
      break;
    case 3:
      return "GlassMutated3";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 4:
    switch (sub_table_2D) {
    case 0:
      return "AKWF_fmsynth_0011";
      break;
    case 1:
      return "AKWF_fmsynth_0032";
      break;
    case 2:
      return "AKWF_fmsynth_0034";
      break;
    case 3:
      return "AKWF_fmsynth_0081";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;

  case 5:
    switch (sub_table_2D) {
    case 0:
      return "BrokenSine1";
      break;
    case 1:
      return "BrokenSine2";
      break;
    case 2:
      return "BrokenSine3";
      break;
    case 3:
      return "BrokenSine4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 6:
    switch (sub_table_2D) {
    case 0:
      return "Skyline1";
      break;
    case 1:
      return "Skyline2";
      break;
    case 2:
      return "Skyline3";
      break;
    case 3:
      return "Skyline4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 7:
    switch (sub_table_2D) {
    case 0:
      return "PerlinReplace1";
      break;
    case 1:
      return "PerlinReplace2";
      break;
    case 2:
      return "PerlinReplace3";
      break;
    case 3:
      return "PerlinReplace4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 8:
    switch (sub_table_2D) {
    case 0:
      return "Rectangular1";
      break;
    case 1:
      return "Rectangular2";
      break;
    case 2:
      return "Rectangular3";
      break;
    case 3:
      return "Rectangular4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;

  case 9:
    switch (sub_table_2D) {
    case 0:
      return "AKWF_bitreduced_0002";
      break;
    case 1:
      return "AKWF_bitreduced_0003";
      break;
    case 2:
      return "AKWF_bitreduced_0006";
      break;
    case 3:
      return "AKWF_bitreduced_0011";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 10:
    switch (sub_table_2D) {
    case 0:
      return "Violin1";
      break;
    case 1:
      return "Violin2";
      break;
    case 2:
      return "Cello1";
      break;
    case 3:
      return "Cello2";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 11:
    switch (sub_table_2D) {
    case 0:
      return "Piano2";
      break;
    case 1:
      return "Piano3";
      break;
    case 2:
      return "Piano4";
      break;
    case 3:
      return "Piano1";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 12:
    switch (sub_table_2D) {
    case 0:
      return "Organ1";
      break;
    case 1:
      return "Organ2";
      break;
    case 2:
      return "Organ3";
      break;
    case 3:
      return "Organ4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 13:
    switch (sub_table_2D) {
    case 0:
      return "Oboe1";
      break;
    case 1:
      return "Oboe2";
      break;
    case 2:
      return "Oboe3";
      break;
    case 3:
      return "Oboe4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 14:
    switch (sub_table_2D) {
    case 0:
      return "Trumpet1";
      break;
    case 1:
      return "Trumpet2";
      break;
    case 2:
      return "Trumpet3";
      break;
    case 3:
      return "Trumpet4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 15:
    switch (sub_table_2D) {
    case 0:
      return "LegToyBox";
      break;
    case 1:
      return "LegRip2";
      break;
    case 2:
      return "LegBarbedWire";
      break;
    case 3:
      return "LegMale";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 16:
    switch (sub_table_2D) {
    case 0:
      return "LegAdd8";
      break;
    case 1:
      return "LegSharp";
      break;
    case 2:
      return "LegPiano";
      break;
    case 3:
      return "LegAdd1";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 17:
    switch (sub_table_2D) {
    case 0:
      return "LegCello";
      break;
    case 1:
      return "LegAah";
      break;
    case 2:
      return "LegHarm2";
      break;
    case 3:
      return "LegNoBass";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 18:
    switch (sub_table_2D) {
    case 0:
      return "LegBags";
      break;
    case 1:
      return "LegOrgan";
      break;
    case 2:
      return "LegTriQuad";
      break;
    case 3:
      return "LegAdd3";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 19:
    switch (sub_table_2D) {
    case 0:
      return "AKWF_hvoice_0002";
      break;
    case 1:
      return "AKWF_hvoice_0010";
      break;
    case 2:
      return "AKWF_hvoice_0014";
      break;
    case 3:
      return "AKWF_hvoice_0019";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 20:
    switch (sub_table_2D) {
    case 0:
      return "AKWF_hvoice_0020";
      break;
    case 1:
      return "AKWF_hvoice_0021";
      break;
    case 2:
      return "AKWF_hvoice_0029";
      break;
    case 3:
      return "AKWF_hvoice_0032";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 21:
    switch (sub_table_2D) {
    case 0:
      return "AKWF_hvoice_0037";
      break;
    case 1:
      return "AKWF_hvoice_0041";
      break;
    case 2:
      return "AKWF_hvoice_0047";
      break;
    case 3:
      return "AKWF_hvoice_0049";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 22:
    switch (sub_table_2D) {
    case 0:
      return "AKWF_hvoice_0056";
      break;
    case 1:
      return "AKWF_hvoice_0064";
      break;
    case 2:
      return "AKWF_hvoice_0071";
      break;
    case 3:
      return "AKWF_hvoice_0093";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 23:
    switch (sub_table_2D) {
    case 0:
      return "Additive1";
      break;
    case 1:
      return "Additive2";
      break;
    case 2:
      return "Additive3";
      break;
    case 3:
      return "Additive4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 24:
    switch (sub_table_2D) {
    case 0:
      return "Additive5";
      break;
    case 1:
      return "Additive6";
      break;
    case 2:
      return "Additive7";
      break;
    case 3:
      return "Additive8";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 25:
    switch (sub_table_2D) {
    case 0:
      return "Additive10";
      break;
    case 1:
      return "Additive11";
      break;
    case 2:
      return "Additive12";
      break;
    case 3:
      return "Additive9";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 26:
    switch (sub_table_2D) {
    case 0:
      return "Additive13";
      break;
    case 1:
      return "Additive14";
      break;
    case 2:
      return "Additive15";
      break;
    case 3:
      return "Additive16";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 27:
    switch (sub_table_2D) {
    case 0:
      return "Harmonics9";
      break;
    case 1:
      return "Harmonics10";
      break;
    case 2:
      return "Harmonics11";
      break;
    case 3:
      return "Harmonics12";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 28:
    switch (sub_table_2D) {
    case 0:
      return "Harmonics13";
      break;
    case 1:
      return "Harmonics14";
      break;
    case 2:
      return "Harmonics15";
      break;
    case 3:
      return "Harmonics16";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 29:
    switch (sub_table_2D) {
    case 0:
      return "Harmonics5";
      break;
    case 1:
      return "Harmonics6";
      break;
    case 2:
      return "Harmonics7";
      break;
    case 3:
      return "Harmonics8";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 30:
    switch (sub_table_2D) {
    case 0:
      return "Harmonics1";
      break;
    case 1:
      return "Harmonics2";
      break;
    case 2:
      return "Harmonics3";
      break;
    case 3:
      return "Harmonics4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 31:
    switch (sub_table_2D) {
    case 0:
      return "FatSawMutated1";
      break;
    case 1:
      return "FatSawMutated2";
      break;
    case 2:
      return "FatSawMutated3";
      break;
    case 3:
      return "FatSawMutated5";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 32:
    switch (sub_table_2D) {
    case 0:
      return "FatSawMutated5";
      break;
    case 1:
      return "FatSawMutated6";
      break;
    case 2:
      return "FatSawMutated7";
      break;
    case 3:
      return "FatSawMutated8";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 33:
    switch (sub_table_2D) {
    case 0:
      return "ChiptuneSquare50Mutated1";
      break;
    case 1:
      return "ChiptuneSquare50Mutated2";
      break;
    case 2:
      return "ChiptuneSquare50Mutated3";
      break;
    case 3:
      return "ChiptuneSquare50Mutated4";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
    break;
  case 34:
    switch (sub_table_2D) {
    case 0:
      return "ChiptuneSquare50Mutated5";
      break;
    case 1:
      return "ChiptuneSquare50Mutated6";
      break;
    case 2:
      return "ChiptuneSquare50Mutated7";
      break;
    case 3:
      return "ChiptuneSquare50Mutated8";
      break;
    default:
      DBG("Tried to get name for illegal table combination (" +
          std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
      return "UNDEFINED";
      break;
    }
  default:
    DBG("Tried to get name for illegal table combination (" +
        std::to_string(p_wt_2D) + "," + std::to_string(sub_table_2D) + ")");
    return "UNDEFINED";
    break;
  }
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
  // smooth position value
  m_position_2D_smooth += (m_position_2D - m_position_2D_smooth) * 0.001;

  // prepare both sides and interpol value
  int read_index_trunc = (int)m_read_index;
  float fractional = m_read_index - (float)read_index_trunc;
  int read_index_next =
      read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;

  // prepare variables for double wavetable accesses
  int left_table;
  int right_table;
  float interpolation_value;
  float position_modded = m_position_2D_smooth + (*m_pos_mod);
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

  m_read_index += m_wavetable_inc * m_sync_anti_aliasing_inc_factor;
  checkWrapIndex(m_read_index);

  return (1.f - interpolation_value) * output_left +
         interpolation_value * output_right;
}
