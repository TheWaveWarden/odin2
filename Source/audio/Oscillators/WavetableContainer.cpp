#include "WavetableContainer.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

#ifndef BENCHMARK
#include "../JuceLibraryCode/JuceHeader.h"
#else
#define DBG(text)
#endif

WavetableContainer::WavetableContainer() {
//#ifdef ODIN_DEBUG
#include "WavetableCoefficients.h"
//#endif

	// dynamically allocate wavetables
	// m_wavetables = new
	// float[NUMBER_OF_WAVETABLES][SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH];
	m_wavetables = new float **[NUMBER_OF_WAVETABLES];
	for (int table = 0; table < NUMBER_OF_WAVETABLES; ++table) {
		m_wavetables[table] = new float *[SUBTABLES_PER_WAVETABLE];
		for (int sub = 0; sub < SUBTABLES_PER_WAVETABLE; ++sub) {
			m_wavetables[table][sub] = new float[WAVETABLE_LENGTH];
		}
	}

	// create specdraw scalar
	for (int harmonic = 1; harmonic < SPECDRAW_STEPS_X + 1; ++harmonic) {
		m_specdraw_scalar[harmonic - 1] = 1.f / sqrtf((float)harmonic);
	}
}

WavetableContainer::~WavetableContainer() {
	// delete wavetables
	for (int table = 0; table < NUMBER_OF_WAVETABLES; ++table) {
		for (int sub = 0; sub < SUBTABLES_PER_WAVETABLE; ++sub) {
			delete[] m_wavetables[table][sub];
		}
		delete[] m_wavetables[table];
	}
	delete[] m_wavetables;
}
/*
void WavetableContainer::createWavetables(float p_samplerate) {

	std::clock_t begin = std::clock();
	std::clock_t end   = std::clock();

	// //loop over all wavetables
	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		DBG("generating wavetable: " + std::to_string(index_wavetable));

		// this flags hackishly if a waveform only uses sine components
		bool sine_only = m_fourier_coeffs[index_wavetable][0][0] > 0.5f ? true : false;

		double seed_freq = 27.5; // A0
		float max        = 0.f;

		// loop over subtables
		for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

			// allocate memory for actual tables
			// float *next_table = new float[WAVETABLE_LENGTH];
			float *next_table = m_wavetables[index_wavetable][index_sub_table];
			memset(next_table, 0, WAVETABLE_LENGTH * sizeof(float));

			// how many harmonics are needed for this subtable
			int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

			// don't allow more than 256 harmonics (for big Samplerates this might
			// happen)
			number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {
				for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {

					// fill table with //sine harmonics
					next_table[index_position] +=
					    m_fourier_coeffs[index_wavetable][0][index_harmonics] *
					    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH) *
					    m_fourier_coeffs[index_wavetable][1][0]; // last term is normalization
					if (!sine_only) {
						// cosine
						next_table[index_position] +=
						    m_fourier_coeffs[index_wavetable][1][index_harmonics] *
						    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH) *
						    m_fourier_coeffs[index_wavetable][1][0]; // last term is normalization
					}
				}
			}

			for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
				if (fabs(next_table[index_position]) > max) {
					max = fabs(next_table[index_position]);
				}
			}

			// assign array to corresponding pointer
			m_wavetable_pointers[index_wavetable][index_sub_table] = next_table;

			// increment seed frequency by minor third = 2^(3/12)
			seed_freq *= 1.1892071150;
		}

		// normalize
		if (max > 0) {
			DBG("max " + std::to_string(max));
			for (int sub = 0; sub < SUBTABLES_PER_WAVETABLE; ++sub) {
				for (int pos = 0; pos < WAVETABLE_LENGTH; ++pos) {
					m_wavetable_pointers[index_wavetable][sub][pos] /= max;
				}
			}
		}

		m_name_index_map.insert(std::pair<std::string, int>(m_wavetable_names_1D[index_wavetable], index_wavetable));
	}

	end                 = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	DBG("Oscillator WT creation took " + std::to_string(elapsed_secs) +
	    " seconds. Average: " + std::to_string(elapsed_secs / (float)NUMBER_OF_WAVETABLES));
}

void WavetableContainer::createLFOtables(float p_samplerate) {

	// //loop over all wavetables
	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_LFOTABLES; ++index_wavetable) {

		double seed_freq = 27.5; // A0
		float max        = 0.f;

		// allocate memory for actual tables
		float *next_table = new float[WAVETABLE_LENGTH];
		memset(next_table, 0, WAVETABLE_LENGTH * sizeof(float));

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 256 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_harmonics = 0; index_harmonics < number_of_harmonics; ++index_harmonics) {
			for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {

				// fill table with //sine harmonics
				next_table[index_position] +=
				    m_LFO_fourier_coeffs[index_wavetable][0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH); // *
				// m_LFO_fourier_coeffs[index_wavetable][1]
				//[0]; // last term is normalization
				// cosine
				next_table[index_position] +=
				    m_LFO_fourier_coeffs[index_wavetable][1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH); // *
				// m_LFO_fourier_coeffs[index_wavetable][1]
				//                  [0]; // last term is
				//                  normalization
			}
		}

		// assign array to corresponding pointer
		m_lfotable_pointers[index_wavetable][0] = next_table;

		m_LFO_name_index_map.insert(std::pair<std::string, int>(m_LFO_names[index_wavetable], index_wavetable));
	}
}*/

void WavetableContainer::createChipdrawTable(int p_table_nr,
                                             float p_chipdraw_values[CHIPDRAW_STEPS_X],
                                             float p_samplerate) {

	// first generate the fourier coefficients
	float chipdraw_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

	float step_width = 2 * PI / CHIPDRAW_STEPS_X;

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		float coeff_sine   = 0.f;
		float coeff_cosine = 0.f;

		for (int segment = 0; segment < CHIPDRAW_STEPS_X; ++segment) {
			coeff_sine += const_segment_one_overtone_sine(
			    segment * step_width, (segment + 1) * step_width, p_chipdraw_values[segment], harmonic);
			coeff_cosine += const_segment_one_overtone_cosine(
			    segment * step_width, (segment + 1) * step_width, p_chipdraw_values[segment], harmonic);
		}
		chipdraw_coefficients[0][harmonic] = coeff_sine;
		chipdraw_coefficients[1][harmonic] = coeff_cosine;
	}

	// now create the wavetable from the fourier coefficients
	double seed_freq = 27.5; // A0
	float max        = 0.f;

	// delete old table
	memset(m_chipdraw_tables[p_table_nr], 0, SUBTABLES_PER_WAVETABLE * WAVETABLE_LENGTH * sizeof(float));

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 256 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				m_chipdraw_tables[p_table_nr][index_sub_table][index_position] +=
				    chipdraw_coefficients[0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
				// cosine
				m_chipdraw_tables[p_table_nr][index_sub_table][index_position] +=
				    chipdraw_coefficients[1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(m_chipdraw_tables[p_table_nr][index_sub_table][index_position]) > max) {
				max = fabs(m_chipdraw_tables[p_table_nr][index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;

		// set pointers
		m_chipdraw_pointers[p_table_nr][index_sub_table] = m_chipdraw_tables[p_table_nr][index_sub_table];
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {
		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			m_chipdraw_tables[p_table_nr][index_sub_table][index_position] *= max;
		}
	}
}

void WavetableContainer::createWavedrawTable(int p_table_nr,
                                             float p_wavedraw_values[WAVEDRAW_STEPS_X],
                                             float p_samplerate,
                                             bool p_const_sections) {

	// first generate the fourier coefficients
	float wavedraw_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

	float step_width = 2 * PI / WAVEDRAW_STEPS_X;

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		float coeff_sine   = 0.f;
		float coeff_cosine = 0.f;

		for (int segment = 0; segment < WAVEDRAW_STEPS_X; ++segment) {
			// use either const sections or linear sections
			if (p_const_sections) {
				coeff_sine += const_segment_one_overtone_sine(
				    segment * step_width, (segment + 1) * step_width, p_wavedraw_values[segment], harmonic);
				coeff_cosine += const_segment_one_overtone_cosine(
				    segment * step_width, (segment + 1) * step_width, p_wavedraw_values[segment], harmonic);
			} else {
				// wrap function value at end to start
				float segment_end_value =
				    (segment == WAVEDRAW_STEPS_X - 1) ? p_wavedraw_values[0] : p_wavedraw_values[segment + 1];

				coeff_sine += lin_segment_one_overtone_sine(segment * step_width,
				                                            (segment + 1) * step_width,
				                                            p_wavedraw_values[segment],
				                                            segment_end_value,
				                                            harmonic);
				coeff_cosine += lin_segment_one_overtone_cosine(segment * step_width,
				                                                (segment + 1) * step_width,
				                                                p_wavedraw_values[segment],
				                                                segment_end_value,
				                                                harmonic);
			}
		}
		wavedraw_coefficients[0][harmonic] = coeff_sine;
		wavedraw_coefficients[1][harmonic] = coeff_cosine;
	}

	// now create the wavetable from the fourier coefficients
	double seed_freq = 27.5; // A0
	float max        = 0.f;

	// delete old table
	memset(m_wavedraw_tables[p_table_nr], 0, SUBTABLES_PER_WAVETABLE * WAVETABLE_LENGTH * sizeof(float));

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 256 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				m_wavedraw_tables[p_table_nr][index_sub_table][index_position] +=
				    wavedraw_coefficients[0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
				// cosine
				m_wavedraw_tables[p_table_nr][index_sub_table][index_position] +=
				    wavedraw_coefficients[1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(m_wavedraw_tables[p_table_nr][index_sub_table][index_position]) > max) {
				max = fabs(m_wavedraw_tables[p_table_nr][index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;

		// set pointers
		m_wavedraw_pointers[p_table_nr][index_sub_table] = m_wavedraw_tables[p_table_nr][index_sub_table];
		// DBG((long)m_wavedraw_pointers[p_table_nr][index_sub_table]);
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {
		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			m_wavedraw_tables[p_table_nr][index_sub_table][index_position] *= max;
		}
	}
}

void WavetableContainer::createSpecdrawTable(int p_table_nr,
                                             float p_specdraw_values[SPECDRAW_STEPS_X],
                                             float p_samplerate) {
	// now create the wavetable from the fourier coefficients
	double seed_freq = 27.5; // A0
	float max        = 0.f;

	// delete old table
	memset(m_specdraw_tables[p_table_nr], 0, SUBTABLES_PER_WAVETABLE * WAVETABLE_LENGTH * sizeof(float));

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		number_of_harmonics = number_of_harmonics > SPECDRAW_STEPS_X + 1 ? SPECDRAW_STEPS_X + 1 : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				m_specdraw_tables[p_table_nr][index_sub_table][index_position] +=
				    p_specdraw_values[index_harmonics - 1] * m_specdraw_scalar[index_harmonics - 1] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(m_specdraw_tables[p_table_nr][index_sub_table][index_position]) > max) {
				max = fabs(m_specdraw_tables[p_table_nr][index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;

		// set pointers
		m_specdraw_pointers[p_table_nr][index_sub_table] = m_specdraw_tables[p_table_nr][index_sub_table];
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {
		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			m_specdraw_tables[p_table_nr][index_sub_table][index_position] *= max;
		}
	}
}

float WavetableContainer::lin_segment_one_overtone_sine(float p_a, float p_b, float p_fa, float p_fb, int p_ot) {

	float m = (p_fb - p_fa) / (p_b - p_a); // slope of linear function
	float c = p_fa - m * p_a;              // const offset of linear function

	if (p_ot == 0) {
		return m / 2.f * p_b * p_b + c * p_b - m / 2.f * p_a * p_a - c * p_a;
	}

	return (m * (sin(p_b * p_ot) - sin(p_a * p_ot)) + p_ot * (p_a * m + c) * cos(p_a * p_ot) -
	        p_ot * (p_b * m + c) * cos(p_b * p_ot)) /
	       p_ot / p_ot;
}

float WavetableContainer::lin_segment_one_overtone_cosine(float p_a, float p_b, float p_fa, float p_fb, int p_ot) {

	if (p_ot == 0) {
		// is actually not zero, but idgaf since const offsets are not used
		return 0;
	}

	float m = (p_fb - p_fa) / (p_b - p_a); // slope of linear function
	float c = p_fa - m * p_a;              // const offset of linear function

	return (-p_ot * (p_a * m + c) * sin(p_a * p_ot) - m * cos(p_a * p_ot) + p_ot * (p_b * m + c) * sin(p_b * p_ot) +
	        m * cos(p_b * p_ot)) /
	       p_ot / p_ot;
}

float WavetableContainer::const_segment_one_overtone_sine(float p_start, float p_end, float p_height, int p_harmonic) {
	if (p_harmonic == 0) {
		return 0;
	}
	return p_height / (float)p_harmonic * (cos(p_start * (float)p_harmonic) - cos(p_end * (float)p_harmonic));
}

float WavetableContainer::const_segment_one_overtone_cosine(float p_start, float p_end, float p_height, int p_harmonic) {
	if (p_harmonic == 0) {
		return (p_end - p_start) * p_height;
	}
	return p_height / (float)p_harmonic * (sin(p_end * (float)p_harmonic) - sin(p_start * (float)p_harmonic));
}

float **WavetableContainer::getChipdrawPointer(int p_chipdraw_index) {
	return m_chipdraw_pointers[p_chipdraw_index];
}

float **WavetableContainer::getWavedrawPointer(int p_wavedraw_index) {
	// DBG((long)m_wavedraw_pointers[p_wavedraw_index][0]);
	// DBG("=======");
	return m_wavedraw_pointers[p_wavedraw_index];
}

float **WavetableContainer::getSpecdrawPointer(int p_specdraw_index) {
	return m_specdraw_pointers[p_specdraw_index];
}

const float **WavetableContainer::getWavetablePointers(int p_wavetable) {
	return m_const_wavetable_pointers[p_wavetable];
}

const float **WavetableContainer::getWavetablePointers(std::string p_name) {
	// for(int wt = 0; wt < NUMBER_OF_WAVETABLES; ++wt){
	//     if(p_name == m_wavetable_names_1D[wt]){
	//         return m_wavetable_pointers[wt];
	//     }
	// }

	auto it = m_name_index_map.find(p_name);
	if (it != m_name_index_map.end()) {
		return m_const_wavetable_pointers[it->second];
	}

	
	return m_const_wavetable_pointers[0]; // return sine if no wt found
}

const float **WavetableContainer::getLFOPointers(std::string p_name) {
	auto it = m_LFO_name_index_map.find(p_name);
	if (it != m_LFO_name_index_map.end()) {
		return m_const_LFO_pointers[it->second];
	}
	DBG("Couldn't find LFO table " + p_name);
	return m_const_LFO_pointers[0];
}

void WavetableContainer::loadWavetablesFromConstData() {

	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		for (int index_subtable = 0; index_subtable < SUBTABLES_PER_WAVETABLE; ++index_subtable) {
			m_const_wavetable_pointers[index_wavetable][index_subtable] =
			    getOneSubTable(index_wavetable, index_subtable);
		}
		m_name_index_map.insert(std::pair<std::string, int>(m_wavetable_names_1D[index_wavetable], index_wavetable));
		//String DEBUG_LIST_FUCK = "";

	//for (auto it = m_name_index_map.begin(); it != m_name_index_map.end(); it++) {
	//	DEBUG_LIST_FUCK = DEBUG_LIST_FUCK + it->first + ", ";
	//}

	//DBG("COULDN'T FIND WAVETABLE WITH NAME " + p_name);
	//AlertWindow::showMessageBox(
	  //  AlertWindow::AlertIconType::WarningIcon,  " MAPSIZE AT: "+String(m_wavetable_names_1D[index_wavetable]) +"..." + String(m_name_index_map.size()), DEBUG_LIST_FUCK, m_wavetable_names_1D[index_wavetable]);
	}

	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		int index_subtable                                    = 0;
		m_const_LFO_pointers[index_wavetable][index_subtable] = getOneLFOTable(index_wavetable);
		m_LFO_name_index_map.insert(std::pair<std::string, int>(m_LFO_names[index_wavetable], index_wavetable));
	}
}

void WavetableContainer::loadWavetablesAfterFourierCreation() {

	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		for (int index_subtable = 0; index_subtable < SUBTABLES_PER_WAVETABLE; ++index_subtable) {
			// m_const_wavetable_pointers[index_wavetable][index_subtable] =
			// getOneSubTable(index_wavetable, index_subtable);
			m_const_wavetable_pointers[index_wavetable][index_subtable] = m_wavetables[index_wavetable][index_subtable];
		}
		m_name_index_map.insert(std::pair<std::string, int>(m_wavetable_names_1D[index_wavetable], index_wavetable));
	}
}

int WavetableContainer::getWavetableIndexFromName(std::string p_name) {
	auto it = m_name_index_map.find(p_name);
	if (it != m_name_index_map.end()) {
		return it->second;
	}
	DBG("getWavetableIndexFromName(): COULDNT FIND WT WITH NAME " + p_name);
	return 0;
}
