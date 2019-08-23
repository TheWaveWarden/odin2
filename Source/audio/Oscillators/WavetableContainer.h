#pragma once

#include "../../GlobalIncludes.h"
#include "../OdinConstants.h"
#include "Wavetables/Tables/LFOTableData.h"
#include "Wavetables/Tables/WavetableData.h"
#include <map>
#include <string>

class WavetableContainer {
public:
  static WavetableContainer &getInstance() {
    static WavetableContainer instance;
    return instance;
  }
  WavetableContainer(WavetableContainer const &) = delete;
  void operator=(WavetableContainer const &) = delete;

  virtual ~WavetableContainer();

  void fixWavetableCoefficientFile();
  void fixWavetableIndexInFiles();
  void fixWavetableIndexInSingleFile(std::string p_filename, int p_number);

  void loadWavetablesFromConstData(); // assign pointers to wavetables from
                                      // files directly
  void loadWavetablesAfterFourierCreation(); // assign pointers to wavetables
                                             // from files directly
  void createWavetables(float p_samplerate); // create and allocate memory from
                                             // coefficients and assign pointers
  void createLFOtables(float p_samplerate);
  void createLFOCoefficientsFromConstSections(int p_table_nr,
                                              float p_const_section_values[],
                                              int p_number_of_sections,
                                              std::string p_table_name);
  void createLFOCoefficientsFromLinSections(int p_table_nr,
                                            float p_const_section_values[],
                                            int p_number_of_sections,
                                            std::string p_table_name);
  void createChipdrawTable(int p_table_nr, float p_chipdraw_values[32],
                           float p_samplerate);
  void createWavedrawTable(int p_table_nr,
                           float p_wavedraw_values[WAVEDRAW_STEPS_X],
                           float p_samplerate, bool p_const_sections = false);
  void createSpecdrawTable(int p_table_nr,
                           float p_fourier_values[SPECDRAW_STEPS_X],
                           float p_samplerate);

  void changeSampleRate(float p_samplerate);

  const float **getWavetablePointers(int p_wavetable);
  const float **getWavetablePointers(std::string p_name);
  float **getChipdrawPointer(int p_chipdraw_index);
  float **getWavedrawPointer(int p_wavedraw_index);
  float **getSpecdrawPointer(int p_specdraw_index);
  const float **getLFOPointers(std::string p_name);

  void writeScaleFactorsToFile();
  void writeWavedrawTable(float p_wavedraw_values[WAVEDRAW_STEPS_X],
                          std::string p_name);
  void writeChipdrawTable(float p_wavedraw_values[WAVEDRAW_STEPS_X],
                          std::string p_name);
  void writeSpecdrawTable(float p_wavedraw_values[WAVEDRAW_STEPS_X],
                          std::string p_name);

  void writeSampleTableToFile(std::string p_filname);
  void writePerlinTableToFile(std::string p_filename, int p_steps,
                              float p_percent);

  //this will calculate the amplitude of the wave which arises from a sin + b cos and use it for sin, b = 0
  void eliminatePhaseInWavetableCoefficients(std::string p_filename);

  // call this to make wavetablefiles from wavetable coefficients
  void writeWavetablesToFile();
  void startWriteWavetablesToFile();
  void
  writeWavetableToFile(int index_wavetable); // single table, else stackoverflow
  void endWriteWavetablesToFile();

  void mutateWavetable(std::string p_table_name, int number_of_mutations,
                       float percent, bool p_consecutive_mutation);

  void writeLFOtablesToFiles();

  void generateAudioValueCode();

  // void deleteWavedraw() {
  //	m_wavedraw_tables = {0};
  //}
private:
  WavetableContainer();

protected:
  void seedRandom();
  bool m_random_seeded = false;

  int m_highest_loaded_table = 0;

  // const float
  // *[NUMBER_OF_WAVETABLES][SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH];

  float const_segment_one_overtone_sine(float p_start, float p_end,
                                        float p_height, int p_harmonic);
  float const_segment_one_overtone_cosine(float p_start, float p_end,
                                          float p_height, int p_harmonic);
  float lin_segment_one_overtone_sine(float p_a, float p_b, float p_fa,
                                      float p_fb, int p_ot);
  float lin_segment_one_overtone_cosine(float p_a, float p_b, float p_fa,
                                        float p_fb, int p_ot);

  std::map<std::string, int> m_name_index_map;
  std::map<std::string, int> m_LFO_name_index_map;

  const float *m_const_wavetable_pointers[NUMBER_OF_WAVETABLES]
                                         [SUBTABLES_PER_WAVETABLE];
  const float *m_const_LFO_pointers[NUMBER_OF_WAVETABLES][1];

  // Wavetable pointers
  float *m_wavetable_pointers[NUMBER_OF_WAVETABLES][SUBTABLES_PER_WAVETABLE];
  float
      *m_chipdraw_pointers[NUMBER_OF_CHIPDRAW_TABLES][SUBTABLES_PER_WAVETABLE];
  float
      *m_wavedraw_pointers[NUMBER_OF_WAVEDRAW_TABLES][SUBTABLES_PER_WAVETABLE];
  float
      *m_specdraw_pointers[NUMBER_OF_SPECDRAW_TABLES][SUBTABLES_PER_WAVETABLE];
  float *m_lfotable_pointers[NUMBER_OF_LFOTABLES][1];

  // Fourrier Coefficients
  float m_fourier_coeffs[NUMBER_OF_WAVETABLES][SIN_AND_COS]
                        [NUMBER_OF_HARMONICS] = {
                            0}; // index [x][1][0] will store scalar, since it
                                // is usually constant offset
  float m_LFO_fourier_coeffs[NUMBER_OF_LFOTABLES][SIN_AND_COS]
                            [NUMBER_OF_HARMONICS] = {
                                0}; // index [x][1][0] will store scalar, since
                                    // it is usually constant offset

  // drawn tables
  float m_chipdraw_tables[NUMBER_OF_CHIPDRAW_TABLES][SUBTABLES_PER_WAVETABLE]
                         [WAVETABLE_LENGTH];
  float m_wavedraw_tables[NUMBER_OF_WAVEDRAW_TABLES][SUBTABLES_PER_WAVETABLE]
                         [WAVETABLE_LENGTH] = {0};
  float m_specdraw_tables[NUMBER_OF_SPECDRAW_TABLES][SUBTABLES_PER_WAVETABLE]
                         [WAVETABLE_LENGTH];

  // specdraw scalar (1/sqrt(harmonic))
  float m_specdraw_scalar[SPECDRAW_STEPS_X];

  std::string m_wavetable_names_1D[NUMBER_OF_WAVETABLES];
  std::string m_LFO_names[NUMBER_OF_WAVETABLES];

  bool m_wavetables_created = false;

  //float m_wavetables[NUMBER_OF_WAVETABLES][SUBTABLES_PER_WAVETABLE]
  //                  [WAVETABLE_LENGTH] = {0};
  float*** m_wavetables;//dynamic allocation
  
  //#include "Wavetables/Tables/WavetableData.h" //include initializer list
};
