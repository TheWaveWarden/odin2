#pragma once

#include <string> // why tf is memset defined in cstring?
#include "../OdinConstants.h"
#include <map>
#include <cstring>

class WavetableContainer
{
public:
	static WavetableContainer& getInstance(){
		static WavetableContainer instance;
		return instance;
	}
	WavetableContainer(WavetableContainer const &) = delete;
	void operator=(WavetableContainer const&)      = delete;

	virtual ~WavetableContainer();

	
	void createWavetables(float p_sample_rate);
	void createChipdrawTable(int p_table_nr, float p_chipdraw_values[32], float p_sample_rate);

	void destroyWavetables();
	void changeSampleRate(float p_sample_rate);

	float** getWavetablePointers(int p_wavetable);
	float** getWavetablePointers(std::string p_name);
	float** getChipDrawPointer(int p_chipdraw_index);

	void writeScaleFactorsToFile();

private:
	WavetableContainer();

protected:


	float const_segment_one_overtone_sine(float p_start, float p_end, float p_height, int p_harmonic);
	float const_segment_one_overtone_cosine(float p_start, float p_end, float p_height, int p_harmonic);
	
	std::map<std::string, int> m_name_index_map;

	//Wavetable pointers
	float* m_wavetable_pointers[NUMBER_OF_WAVETABLES][SUBTABLES_PER_WAVETABLE];
	float* m_chipdraw_pointers[NUMBER_OF_CHIPDRAW_TABLES][SUBTABLES_PER_WAVETABLE];

	//Fourrier Coefficients
	float m_fourrier_coeffs[NUMBER_OF_WAVETABLES][SIN_AND_COS][NUMBER_OF_HARMONICS] = {0}; //index [x][1][0] will store scalar, since it is usually constant offset

	//Chipdraw tables
	float m_chipdraw_tables[NUMBER_OF_CHIPDRAW_TABLES][SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH];

	std::string m_wavetable_names_1D[NUMBER_OF_WAVETABLES];

	bool m_wavetables_created = false;
};

