#include "Utilities.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

#include "../JuceLibraryCode/JuceHeader.h"

Utilities::Utilities()
//    :
//#include "Wavetables/Tables/WavetableData.h" //include initializer list
{
#ifdef ODIN_DEBUG
	#include "audio/Oscillators/WavetableCoefficients.h"
#endif

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

Utilities::~Utilities() {
	// delete wavetables
	for (int table = 0; table < NUMBER_OF_WAVETABLES; ++table) {
		for (int sub = 0; sub < SUBTABLES_PER_WAVETABLE; ++sub) {
			delete[] m_wavetables[table][sub];
		}
		delete[] m_wavetables[table];
	}
	delete[] m_wavetables;
}

std::string to_string_no_comma(float p_input) {
	std::string out = std::to_string(p_input);
	std::replace(out.begin(), out.end(), ',', '.');
	return out;
}

void Utilities::createWavetables(float p_samplerate) {

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

			// don't allow more than 800 harmonics (for big Samplerates this might
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

void Utilities::createLFOtables(float p_samplerate) {

	// //loop over all wavetables
	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_LFOTABLES; ++index_wavetable) {

		double seed_freq = 27.5; // A0
		float max        = 0.f;

		// allocate memory for actual tables
		float *next_table = new float[WAVETABLE_LENGTH];
		memset(next_table, 0, WAVETABLE_LENGTH * sizeof(float));

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this might
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
}

void Utilities::createLFOCoefficientsFromConstSections(int p_table_nr,
                                                       float p_const_section_values[],
                                                       int p_number_of_sections,
                                                       std::string p_table_name) {
	// first generate the fourier coefficients
	float LFO_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

	float step_width = 2 * PI / (float)p_number_of_sections;

	for (int harmonic = 0; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		float coeff_sine   = 0.f;
		float coeff_cosine = 0.f;

		for (int segment = 0; segment < p_number_of_sections; ++segment) {

			coeff_sine += const_segment_one_overtone_sine(
			    segment * step_width, (segment + 1) * step_width, p_const_section_values[segment], harmonic);
			coeff_cosine += const_segment_one_overtone_cosine(
			    segment * step_width, (segment + 1) * step_width, p_const_section_values[segment], harmonic);
		}
		LFO_coefficients[0][harmonic] = coeff_sine / PI;
		LFO_coefficients[1][harmonic] = coeff_cosine / PI;
		if (harmonic == 0) {
			// no idea why this is necessary...
			LFO_coefficients[1][0] *= 0.5f;
		}
	}

	std::ofstream output_file;
	output_file.open("~/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/LFO/LFO" +
	                 p_table_name + ".h");

	output_file << "#define WT_NR " + to_string_no_comma(p_table_nr) + "\n\n";
	output_file << "m_LFO_names[WT_NR] = \"" + p_table_name + "\";\n\n";
	for (int harmonic = 0; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		output_file << "m_LFO_fourier_coeffs[WT_NR][0][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(LFO_coefficients[0][harmonic]) + ";\n";
		output_file << "m_LFO_fourier_coeffs[WT_NR][1][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(LFO_coefficients[1][harmonic]) + ";\n";
	}

	output_file << "#undef WT_NR\n";
	output_file.close();
}

void Utilities::createLFOCoefficientsFromLinSections(int p_table_nr,
                                                     float p_lin_section_values[],
                                                     int p_number_of_sections,
                                                     std::string p_table_name) {
	// first generate the fourier coefficients
	float LFO_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

	float step_width = 2 * PI / (float)p_number_of_sections;

	for (int harmonic = 0; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		float coeff_sine   = 0.f;
		float coeff_cosine = 0.f;

		for (int segment = 0; segment < p_number_of_sections; ++segment) {

			coeff_sine += lin_segment_one_overtone_sine(segment * step_width,
			                                            (segment + 1) * step_width,
			                                            p_lin_section_values[segment],
			                                            p_lin_section_values[segment + 1],
			                                            harmonic);
			coeff_cosine += lin_segment_one_overtone_cosine(segment * step_width,
			                                                (segment + 1) * step_width,
			                                                p_lin_section_values[segment],
			                                                p_lin_section_values[segment + 1],
			                                                harmonic);
		}
		LFO_coefficients[0][harmonic] = coeff_sine / PI;
		LFO_coefficients[1][harmonic] = coeff_cosine / PI;
		if (harmonic == 0) {
			// no idea why this is necessary...
			LFO_coefficients[1][0] *= 0.5f;
		}
	}

	std::ofstream output_file;
	output_file.open("~/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/LFO/LFO" +
	                 p_table_name + ".h");

	output_file << "#define WT_NR " + std::to_string(p_table_nr) + "\n\n";
	output_file << "m_LFO_names[WT_NR] = \"" + p_table_name + "\";\n\n";
	for (int harmonic = 0; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		output_file << "m_LFO_fourier_coeffs[WT_NR][0][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(LFO_coefficients[0][harmonic]) + ";\n";
		output_file << "m_LFO_fourier_coeffs[WT_NR][1][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(LFO_coefficients[1][harmonic]) + ";\n";
	}

	output_file << "#undef WT_NR\n";
	output_file.close();
}

void Utilities::createChipdrawTable(int p_table_nr, float p_chipdraw_values[CHIPDRAW_STEPS_X], float p_samplerate) {

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

		// don't allow more than 800 harmonics (for big Samplerates this might
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

void Utilities::writeWavedrawTable(float p_wavedraw_values[WAVEDRAW_STEPS_X], std::string p_table_name) {
	// first generate the fourier coefficients
	float wavedraw_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

	float step_width = 2 * PI / WAVEDRAW_STEPS_X;

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		float coeff_sine   = 0.f;
		float coeff_cosine = 0.f;

		for (int segment = 0; segment < WAVEDRAW_STEPS_X; ++segment) {
			// use either const sections or linear sections

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
		wavedraw_coefficients[0][harmonic] = coeff_sine;
		wavedraw_coefficients[1][harmonic] = coeff_cosine;
	}

	// now create the wavetable from the fourier coefficients
	double seed_freq                                                 = 27.5; // A0
	float max                                                        = 0.f;
	float wavedraw_tables[SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] = {0};
	float p_samplerate                                               = 44100;

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
				// cosine
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(wavedraw_tables[index_sub_table][index_position]) > max) {
				max = fabs(wavedraw_tables[index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}

	std::ofstream container;
	container.open("/home/frot/odinvst/Source/audio//Oscillators/WavetableCoefficients.h",
	               std::ofstream::out | std::ofstream::app);

	// container << "#include \"Wavetables/Coefficients/" + p_table_name +
	//                 ".h\" //" + std::to_string(m_highest_loaded_table + 1) +
	//                 "\n";
	container.close();

	DBG("CREATING TABLE /home/frot/odinvst/Source/audio/Oscillators/"
	    "Wavetables/Coefficients/" +
	    p_table_name + ".h " + std::to_string(m_highest_loaded_table + 1));

	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Coefficients/" +
	                 p_table_name + ".h");

	output_file << "#define WT_NR " << ++m_highest_loaded_table << "\n\n";
	output_file << "m_highest_loaded_table = WT_NR > m_highest_loaded_table ? "
	               "WT_NR : m_highest_loaded_table;\n";
	output_file << "m_wavetable_names_1D[WT_NR] = \"" + p_table_name + "\";\n\n";

	output_file << "m_fourier_coeffs[WT_NR][1][0] = " + to_string_no_comma(max) + ";//scalar\n\n";

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		output_file << "m_fourier_coeffs[WT_NR][0][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[0][harmonic]) + ";\n";
		output_file << "m_fourier_coeffs[WT_NR][1][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[1][harmonic]) + ";\n";
	}

	output_file << "#undef WT_NR\n";
	output_file.close();
}

void Utilities::writeChipdrawTable(float p_chipdraw_values[WAVEDRAW_STEPS_X], std::string p_table_name) {
	// first generate the fourier coefficients
	float wavedraw_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

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
		wavedraw_coefficients[0][harmonic] = coeff_sine;
		wavedraw_coefficients[1][harmonic] = coeff_cosine;
	}

	// now create the wavetable from the fourier coefficients
	double seed_freq                                                 = 27.5; // A0
	float max                                                        = 0.f;
	float wavedraw_tables[SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] = {0};
	float p_samplerate                                               = 44100;

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
				// cosine
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(wavedraw_tables[index_sub_table][index_position]) > max) {
				max = fabs(wavedraw_tables[index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}

	seedRandom();

	std::string random_string = std::to_string(rand());

	DBG("CREATING TABLE /home/frot/odinvst/Source/audio/Oscillators/"
	    "Wavetables/Coefficients/" +
	    p_table_name + random_string + ".h " + std::to_string(m_highest_loaded_table + 1));

	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Coefficients/" +
	                 p_table_name + random_string + ".h");

	output_file << "#define WT_NR "
	            << "\n\n";
	output_file << "m_highest_loaded_table = WT_NR > m_highest_loaded_table ? "
	               "WT_NR : m_highest_loaded_table;\n";
	output_file << "m_wavetable_names_1D[WT_NR] = \"" + p_table_name + random_string + "\";\n\n";

	output_file << "m_fourier_coeffs[WT_NR][1][0] = " + to_string_no_comma(max) + ";//scalar\n\n";

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		output_file << "m_fourier_coeffs[WT_NR][0][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[0][harmonic]) + ";\n";
		output_file << "m_fourier_coeffs[WT_NR][1][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[1][harmonic]) + ";\n";
	}

	output_file << "#undef WT_NR\n";
	output_file.close();
}

void Utilities::writeSpecdrawTable(float p_specdraw_values[SPECDRAW_STEPS_X], std::string p_table_name) {

	// now create the wavetable from the fourier coefficients
	double seed_freq                                                 = 27.5; // A0
	float max                                                        = 0.f;
	float wavedraw_tables[SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] = {0};
	float p_samplerate                                               = 44100;

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > SPECDRAW_STEPS_X - 1 ? SPECDRAW_STEPS_X - 1 : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				wavedraw_tables[index_sub_table][index_position] +=
				    p_specdraw_values[index_harmonics - 1] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(wavedraw_tables[index_sub_table][index_position]) > max) {
				max = fabs(wavedraw_tables[index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}

	std::ofstream container;
	container.open("/home/frot/odinvst/Source/audio//Oscillators/WavetableCoefficients.h",
	               std::ofstream::out | std::ofstream::app);

	// container << "#include \"Wavetables/Coefficients/" + p_table_name +
	//                 ".h\" //" + std::to_string(m_highest_loaded_table + 1) +
	//                 "\n";
	container.close();

	DBG("CREATING TABLE /home/frot/odinvst/Source/audio/Oscillators/"
	    "Wavetables/Coefficients/" +
	    p_table_name + ".h " + std::to_string(m_highest_loaded_table + 1));

	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Coefficients/" +
	                 p_table_name + ".h");

	output_file << "#define WT_NR " << ++m_highest_loaded_table << "\n\n";
	output_file << "m_highest_loaded_table = WT_NR > m_highest_loaded_table ? "
	               "WT_NR : m_highest_loaded_table;\n";
	output_file << "m_wavetable_names_1D[WT_NR] = \"" + p_table_name + "\";\n\n";

	output_file << "m_fourier_coeffs[WT_NR][1][0] = " + to_string_no_comma(max) + ";//scalar\n\n";

	for (int harmonic = 1; harmonic < SPECDRAW_STEPS_X; ++harmonic) {

		output_file << "m_fourier_coeffs[WT_NR][0][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(p_specdraw_values[harmonic - 1]) + ";\n";
	}

	output_file << "#undef WT_NR\n";
	output_file.close();
}

void Utilities::createWavedrawTable(int p_table_nr,
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

		// don't allow more than 800 harmonics (for big Samplerates this might
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

void Utilities::createSpecdrawTable(int p_table_nr, float p_specdraw_values[SPECDRAW_STEPS_X], float p_samplerate) {
	// now create the wavetable from the fourier coefficients
	double seed_freq = 27.5; // A0
	float max        = 0.f;

	// delete old table
	memset(m_specdraw_tables[p_table_nr], 0, SUBTABLES_PER_WAVETABLE * WAVETABLE_LENGTH * sizeof(float));

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this might
		// happen)
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

float Utilities::lin_segment_one_overtone_sine(float p_a, float p_b, float p_fa, float p_fb, int p_ot) {

	float m = (p_fb - p_fa) / (p_b - p_a); // slope of linear function
	float c = p_fa - m * p_a;              // const offset of linear function

	if (p_ot == 0) {
		return m / 2.f * p_b * p_b + c * p_b - m / 2.f * p_a * p_a - c * p_a;
	}

	return (m * (sin(p_b * p_ot) - sin(p_a * p_ot)) + p_ot * (p_a * m + c) * cos(p_a * p_ot) -
	        p_ot * (p_b * m + c) * cos(p_b * p_ot)) /
	       p_ot / p_ot;
}

float Utilities::lin_segment_one_overtone_cosine(float p_a, float p_b, float p_fa, float p_fb, int p_ot) {

	if (p_ot == 0) {
		// this isn't actually zero but const offsets (cos(0*x)) are ignored anyway
		return 0;
	}

	float m = (p_fb - p_fa) / (p_b - p_a); // slope of linear function
	float c = p_fa - m * p_a;              // const offset of linear function

	return (-p_ot * (p_a * m + c) * sin(p_a * p_ot) - m * cos(p_a * p_ot) + p_ot * (p_b * m + c) * sin(p_b * p_ot) +
	        m * cos(p_b * p_ot)) /
	       p_ot / p_ot;
}

float Utilities::const_segment_one_overtone_sine(float p_start, float p_end, float p_height, int p_harmonic) {
	if (p_harmonic == 0) {
		return 0;
	}
	return p_height / (float)p_harmonic * (cos(p_start * (float)p_harmonic) - cos(p_end * (float)p_harmonic));
}

float Utilities::const_segment_one_overtone_cosine(float p_start, float p_end, float p_height, int p_harmonic) {
	if (p_harmonic == 0) {
		return (p_end - p_start) * p_height;
	}
	return p_height / (float)p_harmonic * (sin(p_end * (float)p_harmonic) - sin(p_start * (float)p_harmonic));
}

float **Utilities::getChipdrawPointer(int p_chipdraw_index) {
	return m_chipdraw_pointers[p_chipdraw_index];
}

float **Utilities::getWavedrawPointer(int p_wavedraw_index) {
	// DBG((long)m_wavedraw_pointers[p_wavedraw_index][0]);
	// DBG("=======");
	return m_wavedraw_pointers[p_wavedraw_index];
}

float **Utilities::getSpecdrawPointer(int p_specdraw_index) {
	return m_specdraw_pointers[p_specdraw_index];
}

const float **Utilities::getWavetablePointers(int p_wavetable) {
	return m_const_wavetable_pointers[p_wavetable];
}

const float **Utilities::getWavetablePointers(std::string p_name) {
	// for(int wt = 0; wt < NUMBER_OF_WAVETABLES; ++wt){
	//     if(p_name == m_wavetable_names_1D[wt]){
	//         return m_wavetable_pointers[wt];
	//     }
	// }

	auto it = m_name_index_map.find(p_name);
	if (it != m_name_index_map.end()) {
		return m_const_wavetable_pointers[it->second];
	}
	DBG("COULDN'T FIND WAVETABLE WITH NAME " + p_name);
	return m_const_wavetable_pointers[0]; // return sine if no wt found
}

const float **Utilities::getLFOPointers(std::string p_name) {
	auto it = m_LFO_name_index_map.find(p_name);
	if (it != m_LFO_name_index_map.end()) {
		return m_const_LFO_pointers[it->second];
	}
	DBG("Couldn't find LFO table " + p_name);
	return m_const_LFO_pointers[0];
}

void Utilities::writeScaleFactorsToFile() {
	std::ofstream file;
	file.open("E:\\odin\\scaleFactors.h");

	for (int wt = 0; wt < NUMBER_OF_WAVETABLES; ++wt) {
		float max = 0.f;

		for (int sub = 0; sub < SUBTABLES_PER_WAVETABLE; ++sub) {
			float const *next_table = m_wavetable_pointers[wt][sub];
			for (int pos = 0; pos < WAVETABLE_LENGTH; ++pos) {
				if (fabs(next_table[pos]) > max) {
					max = fabs(next_table[pos]);
				}
			}
		}

		file << "m_fourier_coeffs[WT_NR][1][0] = " << 1.f / max * 0.99f * m_fourier_coeffs[wt][1][0] << "; //"
		     << m_wavetable_names_1D[wt] << " scalar\n";
		file << "max was " << max << "\n";
	}

	file.close();
}

void Utilities::loadWavetablesFromConstData() {

	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		for (int index_subtable = 0; index_subtable < SUBTABLES_PER_WAVETABLE; ++index_subtable) {
			m_const_wavetable_pointers[index_wavetable][index_subtable] =
			    getOneSubTable(index_wavetable, index_subtable);
		}
		m_name_index_map.insert(std::pair<std::string, int>(m_wavetable_names_1D[index_wavetable], index_wavetable));
	}

	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		int index_subtable                                    = 0;
		m_const_LFO_pointers[index_wavetable][index_subtable] = getOneLFOTable(index_wavetable);
		m_LFO_name_index_map.insert(std::pair<std::string, int>(m_LFO_names[index_wavetable], index_wavetable));
	}
}

void Utilities::loadWavetablesAfterFourierCreation() {

	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		for (int index_subtable = 0; index_subtable < SUBTABLES_PER_WAVETABLE; ++index_subtable) {
			// m_const_wavetable_pointers[index_wavetable][index_subtable] =
			// getOneSubTable(index_wavetable, index_subtable);
			m_const_wavetable_pointers[index_wavetable][index_subtable] = m_wavetables[index_wavetable][index_subtable];
		}
		m_name_index_map.insert(std::pair<std::string, int>(m_wavetable_names_1D[index_wavetable], index_wavetable));
	}
}

void Utilities::writeWavetablesToFile() {
	startWriteWavetablesToFile();
	for (int i = 0; i < NUMBER_OF_WAVETABLES; ++i) {
		DBG("Write table to file " + std::to_string(i));
		writeWavetableToFile(i);
	}
	endWriteWavetablesToFile();
}

void Utilities::startWriteWavetablesToFile() {
	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Tables/WavetableData.cpp");

	output_file << "#include \"WavetableData.h\"\n\nconst float "
	               "wavetable_data[NUMBER_OF_WAVETABLES][SUBTABLES_PER_WAVETABLE]"
	               "[WAVETABLE_LENGTH] = {";
}

void Utilities::writeWavetableToFile(int index_wavetable) {

	float p_samplerate                                             = 44100.f;
	float wavetables[1][SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] = {0};

	float max = 0;

	// this flags hackishly if a waveform only uses sine components
	bool sine_only = m_fourier_coeffs[index_wavetable][0][0] > 0.5f ? true : false;

	double seed_freq = 27.5; // A0
	// float max = 0.f;

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {
			for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {

				// fill table with //sine harmonics
				wavetables[0][index_sub_table][index_position] +=
				    m_fourier_coeffs[index_wavetable][0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH) *
				    m_fourier_coeffs[index_wavetable][1][0]; // last term is normalization
				if (!sine_only) {
					// cosine
					wavetables[0][index_sub_table][index_position] +=
					    m_fourier_coeffs[index_wavetable][1][index_harmonics] *
					    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH) *
					    m_fourier_coeffs[index_wavetable][1][0]; // last term is normalization
				}
			}
		}

		for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {
			for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
				if (fabs(wavetables[0][index_sub_table][index_position]) > max) {
					max = fabs(wavetables[0][index_sub_table][index_position]);
				}
			}
		}

		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;
	}

	// avoid division by zero
	max = max < 0.0001 ? 1 : max;

	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Tables/WavetableData.cpp",
	                 std::ofstream::out | std::ofstream::app);

	// output_file << "m_wavetables{";

	output_file << "{";

	for (int index_subtable = 0; index_subtable < SUBTABLES_PER_WAVETABLE; ++index_subtable) {
		output_file << "{";

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			output_file << wavetables[0][index_subtable][index_position] / max;
			if (index_position != WAVETABLE_LENGTH - 1) {
				output_file << ",";
			}
		}
		output_file << "}";
		if (index_subtable != SUBTABLES_PER_WAVETABLE - 1) {
			output_file << ",";
		}
	}
	output_file << "}";
	if (index_wavetable != NUMBER_OF_WAVETABLES - 1) {
		output_file << ",";
	}
	output_file.close();
}

void Utilities::endWriteWavetablesToFile() {
	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Tables/WavetableData.cpp",
	                 std::ofstream::out | std::ofstream::app);

	output_file << "};\n\nconst float "
	               "(*getWavetableData())[NUMBER_OF_WAVETABLES][SUBTABLES_PER_"
	               "WAVETABLE]["
	               "WAVETABLE_LENGTH]{\n    return &wavetable_data;\n}\nconst "
	               "float * "
	               "getOneSubTable(int p_wavetable, int p_subtable){\n return "
	               "wavetable_data[p_wavetable][p_subtable];\n}";
	output_file.close();
}

void Utilities::writeLFOtablesToFiles() {

	float p_samplerate                                          = 44100.f;
	float wavetables[NUMBER_OF_WAVETABLES][1][WAVETABLE_LENGTH] = {0};

	// //loop over all wavetables
	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {

		// this flags hackishly if a waveform only uses sine components
		bool sine_only = false;

		double seed_freq = 27.5; // A0
		float max        = 0.f;

		// loop over subtables
		int index_sub_table = 0;

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this
		// might happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {
			for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {

				// fill table with //sine harmonics
				wavetables[index_wavetable][index_sub_table][index_position] +=
				    m_LFO_fourier_coeffs[index_wavetable][0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH) *
				    m_LFO_fourier_coeffs[index_wavetable][1][0]; // last term is normalization
				// cosine
				wavetables[index_wavetable][index_sub_table][index_position] +=
				    m_LFO_fourier_coeffs[index_wavetable][1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH) *
				    m_LFO_fourier_coeffs[index_wavetable][1][0]; // last term is normalization
			}
		}
	}
	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_WAVETABLES; ++index_wavetable) {
		int index_sub_table = 0;

		float max = 0;
		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			if (fabs(wavetables[index_wavetable][index_sub_table][index_position]) > max) {
				max = fabs(wavetables[index_wavetable][index_sub_table][index_position]);
			}
		}

		// normalize
		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			wavetables[index_wavetable][index_sub_table][index_position] /= max;
		}
	}

	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Tables/LFOTableData.cpp");

	output_file << "const float "
	               "LFO_table_data[NUMBER_OF_LFOTABLES][1][WAVETABLE_LENGTH] = {";

	for (int index_wavetable = 0; index_wavetable < NUMBER_OF_LFOTABLES; ++index_wavetable) {
		output_file << "{";

		int index_subtable = 0;
		output_file << "{";

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			// output_file <<
			// wavetables[index_wavetable][index_subtable][index_position]
			output_file << m_lfotable_pointers[index_wavetable][index_subtable][index_position] << ",";
		}
		output_file << "}";

		output_file << "}";
		if (index_wavetable != NUMBER_OF_LFOTABLES - 1) {
			output_file << ",";
		}
	}
	output_file << "};\nconst float "
	               "(*getWavetableData())[NUMBER_OF_LFOTABLES][1]["
	               "WAVETABLE_LENGTH]{\n   return &wavetable_data;}\n\nconst "
	               "float * "
	               "getOneSubTable(int p_wavetable, int p_subtable){\nreturn "
	               "wavetable_data[p_wavetable][p_subtable];\n}";

	output_file.close();
}

void Utilities::generateAudioValueCode() {
	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/GeneratedAudioValueCode.h");

	output_file << "//This code was automatically generated by "
	               "Utilities::generateAudioValueCode()\n\n";
	output_file << "//WAVEDRAW\n\n";

	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
		float val = sin(2 * M_PI * i / (float)WAVEDRAW_STEPS_X) * 0.9;
		output_file << "std::make_unique<AudioParameterFloat> (\"osc1_wavedraw[" + std::to_string(i) +
		                   "]\",\"osc1_wavedraw[" + std::to_string(i) + "]\",-1,1," + std::to_string(val) + "),\n";
		output_file << "std::make_unique<AudioParameterFloat> (\"osc2_wavedraw[" + std::to_string(i) +
		                   "]\",\"osc2_wavedraw[" + std::to_string(i) + "]\",-1,1," + std::to_string(val) + "),\n";
		output_file << "std::make_unique<AudioParameterFloat> (\"osc3_wavedraw[" + std::to_string(i) +
		                   "]\",\"osc3_wavedraw[" + std::to_string(i) + "]\",-1,1," + std::to_string(val) + "),\n";
	}

	output_file << "\n//CHIPDRAW\n\n";

	for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
		float val = i < CHIPDRAW_STEPS_X / 2 ? 0.875f : -0.875f;
		output_file << "std::make_unique<AudioParameterFloat> (\"osc1_chipdraw[" + std::to_string(i) +
		                   "]\",\"osc1_chipdraw[" + std::to_string(i) + "]\",-1,1," + std::to_string(val) + "),\n";
		output_file << "std::make_unique<AudioParameterFloat> (\"osc2_chipdraw[" + std::to_string(i) +
		                   "]\",\"osc2_chipdraw[" + std::to_string(i) + "]\",-1,1," + std::to_string(val) + "),\n";
		output_file << "std::make_unique<AudioParameterFloat> (\"osc3_chipdraw[" + std::to_string(i) +
		                   "]\",\"osc3_chipdraw[" + std::to_string(i) + "]\",-1,1," + std::to_string(val) + "),\n";
	}

	output_file << "\n//SPECDRAW\n\n";

	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		float val = i == 0 ? 1 : 0;
		output_file << "std::make_unique<AudioParameterFloat> (\"osc1_specdraw[" + std::to_string(i) +
		                   "]\",\"osc1_specdraw[" + std::to_string(i) + "]\",0,1," + std::to_string(val) + "),\n";
		output_file << "std::make_unique<AudioParameterFloat> (\"osc2_specdraw[" + std::to_string(i) +
		                   "]\",\"osc2_specdraw[" + std::to_string(i) + "]\",0,1," + std::to_string(val) + "),\n";
		output_file << "std::make_unique<AudioParameterFloat> (\"osc3_specdraw[" + std::to_string(i) +
		                   "]\",\"osc3_specdraw[" + std::to_string(i) + "]\",0,1," + std::to_string(val) + "),\n";
	}

	output_file.close();
}

void Utilities::writeSampleTableToFile(std::string p_filename) {

	// load in data
	std::string filename = "/home/frot/odinvst/samples/" + p_filename;
	File file(filename);
	WavAudioFormat wavAudioFormat;
	AudioFormatReader *audioFormatReader = wavAudioFormat.createReaderFor(file.createInputStream().get(), true);
	float *wavData[2]; // new int *[3];
	wavData[0] = new float[audioFormatReader->lengthInSamples];
	wavData[1] = new float[audioFormatReader->lengthInSamples];
	audioFormatReader->read(wavData, 2, 0, audioFormatReader->lengthInSamples);
	int n_samples = audioFormatReader->lengthInSamples;
#define WAVE wavData[0]

	// remove offsets from beginning start by adding linear * cos function
	float difference = WAVE[n_samples - 1] - WAVE[0];

	for (int i = 0; i < n_samples; ++i) {
		WAVE[i] -= difference * (cos((2 * M_PI * i) / (float)n_samples)) * (float)i / (float)(n_samples);
	}

	// CREATE WAVEFILE

	float wavedraw_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

	float step_width = 2 * PI / (float)n_samples;

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		float coeff_sine   = 0.f;
		float coeff_cosine = 0.f;

		for (int segment = 0; segment < n_samples; ++segment) {
			// use either const sections or linear sections

			// wrap function value at end to start
			float segment_end_value = (segment == n_samples - 1) ? WAVE[0] : WAVE[segment + 1];

			coeff_sine += lin_segment_one_overtone_sine(
			    segment * step_width, (segment + 1) * step_width, WAVE[segment], segment_end_value, harmonic);
			coeff_cosine += lin_segment_one_overtone_cosine(
			    segment * step_width, (segment + 1) * step_width, WAVE[segment], segment_end_value, harmonic);
		}
		wavedraw_coefficients[0][harmonic] = coeff_sine;
		wavedraw_coefficients[1][harmonic] = coeff_cosine;
	}

	// now create the wavetable from the fourier coefficients
	double seed_freq                                                 = 27.5; // A0
	float max                                                        = 0.f;
	float wavedraw_tables[SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] = {0};
	float p_samplerate                                               = 44100;

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this
		// might happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
				// cosine
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(wavedraw_tables[index_sub_table][index_position]) > max) {
				max = fabs(wavedraw_tables[index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}

	std::string p_table_name = p_filename;
	if (p_filename.find(".wav") != std::string::npos) {
		p_table_name = p_table_name.substr(0, p_table_name.size() - 4);
	}

	std::ofstream container;
	container.open("/home/frot/odinvst/Source/audio//Oscillators/WavetableCoefficients.h",
	               std::ofstream::out | std::ofstream::app);

	// container << "#include \"Wavetables/Coefficients/" + p_table_name +
	//                 ".h\" //" + std::to_string(m_highest_loaded_table + 1) +
	//                 "\n";
	container.close();

	DBG("CREATING TABLE /home/frot/odinvst/Source/audio/Oscillators/"
	    "Wavetables/Coefficients/" +
	    p_table_name + ".h");

	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Coefficients/" +
	                 p_table_name + ".h");

	output_file << "#define WT_NR " << ++m_highest_loaded_table << "\n\n";
	output_file << "m_wavetable_names_1D[WT_NR] = \"" + p_table_name + "\";\n\n";
	output_file << "m_highest_loaded_table = WT_NR > m_highest_loaded_table ? "
	               "WT_NR : m_highest_loaded_table;\n";
	output_file << "m_fourier_coeffs[WT_NR][1][0] = " + to_string_no_comma(max) + ";//scalar\n\n";

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		output_file << "m_fourier_coeffs[WT_NR][0][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[0][harmonic]) + ";\n";
		output_file << "m_fourier_coeffs[WT_NR][1][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[1][harmonic]) + ";\n";
	}

	output_file << "#undef WT_NR\n";
	output_file.close();
}

void Utilities::mutateWavetable(
    std::string p_table_name, int number_of_mutations, float percent, bool p_consecutive_mutation, int p_start_id) {

	auto it = m_name_index_map.find(p_table_name);
	if (it == m_name_index_map.end()) {
		DBG("COULDN'T FIND WAVETABLE WITH NAME " + p_table_name + " IN WAVETABLEMUTATOR");
		return;
	}

	float consecutive_store[SIN_AND_COS][NUMBER_OF_HARMONICS] = {0};
	if (p_consecutive_mutation) {
		for (int i = 0; i < NUMBER_OF_HARMONICS; ++i) {
			for (int j = 0; j < SIN_AND_COS; ++j) {
				consecutive_store[j][i] = m_fourier_coeffs[it->second][j][i];
			}
		}
	}
	// m_fourier_coeffs[it->second];

	// seed random
	seedRandom();

	for (int mutation = 1; mutation <= number_of_mutations; ++mutation) {
		float p_specdraw_values[SIN_AND_COS][NUMBER_OF_HARMONICS] = {0};

		for (int i = 0; i < NUMBER_OF_HARMONICS; ++i) {
			for (int j = 0; j < SIN_AND_COS; ++j) {

				float random = (float)rand();
				random       = 2.f * ((float)random / (float)RAND_MAX) - 1.f;
				random *= percent / 100.f;
				random = 1.f + random;

				if (p_consecutive_mutation) {
					p_specdraw_values[j][i] = consecutive_store[j][i] * random;
					consecutive_store[j][i] = p_specdraw_values[j][i];
				} else {
					p_specdraw_values[j][i] = m_fourier_coeffs[it->second][j][i] * random;
				}
			}
		}

		// now create the wavetable from the fourier coefficients
		double seed_freq                                                 = 27.5; // A0
		float max                                                        = 0.f;
		float wavedraw_tables[SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] = {0};
		float p_samplerate                                               = 44100;

		// loop over subtables
		for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

			// how many harmonics are needed for this subtable
			int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

			// don't allow more than 800 harmonics (for big Samplerates this might
			// happen)
			number_of_harmonics =
			    number_of_harmonics > NUMBER_OF_HARMONICS - 1 ? NUMBER_OF_HARMONICS - 1 : number_of_harmonics;

			for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
				for (int index_sin = 0; index_sin < SIN_AND_COS; ++index_sin) {
					for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

						// fill table with
						// sine harmonics
						wavedraw_tables[index_sub_table][index_position] +=
						    p_specdraw_values[index_sin][index_harmonics] *
						    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
					}
					// find max among all tables
					if (fabs(wavedraw_tables[index_sub_table][index_position]) > max) {
						max = fabs(wavedraw_tables[index_sub_table][index_position]);
					}
				}
			}
			// increment seed frequency by minor third = 2^(3/12)
			seed_freq *= 1.1892071150;
		}

		// do another round to scale the table
		// avoid division by 0
		if (max > 1e-5) {
			max = 1.f / max; // for faster computation
		}

		std::ofstream container;
		container.open("/home/frot/odinvst/Source/audio//Oscillators/WavetableCoefficients.h",
		               std::ofstream::out | std::ofstream::app);

		// container << "#include \"Wavetables/Coefficients/" + p_table_name +
		//                 "Mutated" + std::to_string(mutation) + ".h\" //" +
		//                 std::to_string(m_highest_loaded_table + 1) + "\n";
		container.close();

		DBG("CREATING TABLE /home/frot/odinvst/Source/audio/Oscillators/"
		    "Wavetables/Coefficients/" +
		    p_table_name + "Mutated" + std::to_string(mutation) + ".h   WT_NR = " + std::to_string(p_start_id));

		std::ofstream output_file;
		output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
		                 "Wavetables/Coefficients/" +
		                 p_table_name + "Mutated" + std::to_string(mutation) + ".h");

		output_file << "#define WT_NR " << p_start_id++ << "\n\n";
		output_file << "m_highest_loaded_table = WT_NR > m_highest_loaded_table ? "
		               "WT_NR : m_highest_loaded_table;\n";
		output_file << "m_wavetable_names_1D[WT_NR] = \"" + p_table_name + "Mutated" + std::to_string(mutation) +
		                   "\";\n\n";

		output_file << "m_fourier_coeffs[WT_NR][1][0] = " + to_string_no_comma(max) + ";//scalar\n\n";

		for (int harmonic = 1; harmonic < SPECDRAW_STEPS_X; ++harmonic) {
			for (int index_sin = 0; index_sin < SIN_AND_COS; ++index_sin) {

				output_file << "m_fourier_coeffs[WT_NR][" + std::to_string(index_sin) + "][" +
				                   std::to_string(harmonic) +
				                   "] = " + to_string_no_comma(p_specdraw_values[index_sin][harmonic]) + ";\n";
			}
		}

		output_file << "#undef WT_NR\n";
		output_file.close();
	}
}

void Utilities::seedRandom() {
	if (!m_random_seeded) {
		std::srand(std::time(nullptr));
		m_random_seeded = true;
	}
}

// interpolates by cosine, frac in [0,1]
float cosInterpol(float p_left, float p_right, float p_frac) {
	float cosine = 0.5f - 0.5f * cos(M_PI * p_frac);
	return p_left + (p_right - p_left) * cosine;
}

void Utilities::writePerlinTableToFile(std ::string p_table_name, int p_steps, float p_percent) {
	seedRandom();

	// first generate the grid:
	std::vector<float> grid = {0.f};
	std::vector<float> step_length;
	std::vector<float> values = {0.f}; // first value is zero (and last)

	float grid_step_size = 1.f / (float)p_steps;

	bool grid_finished = false;

	while (!grid_finished) {
		// generate next grid point with variation
		float random = (float)rand();
		random       = 2.f * ((float)random / (float)RAND_MAX) - 1.f;
		random *= p_percent / 100.f;
		random = 1.f + random;

		float this_step_size = grid_step_size * random;

		// cap it in case of end reached:
		if (grid[grid.size() - 1] + this_step_size > 1) {
			this_step_size = 1.f - grid[grid.size() - 1];
			grid_finished  = true;
		}

		// push data on vectors
		grid.push_back(grid[grid.size() - 1] + this_step_size);
		step_length.push_back(this_step_size);
	}

	// next generate the values on the grid (start and endpoint are zero)
	for (int i = 1; i < grid.size() - 1; ++i) {
		float random = (float)rand();
		random       = 2.f * ((float)random / (float)RAND_MAX) - 1.f;
		values.push_back(random);
	}
	values.push_back(0.f);

	DBG("gridsize: " + std::to_string(grid.size()));
	DBG("stepsize: " + std::to_string(step_length.size()));
	DBG("valuesize: " + std::to_string(values.size()));
	DBG("======");

#define PERLIN_STEPS_X 10000

	// now fill table with perlin noise
	float perlin_table[PERLIN_STEPS_X] = {0};

	// std::ofstream perlin_file;
	// perlin_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	//                  "Wavetables/Coefficients/" +
	//                  p_table_name + ".csv");

	// perlin_file << "axis" << ";" << "data" << ";\n";

	for (int i = 0; i < PERLIN_STEPS_X; ++i) {
		float x = (float)i / (float)PERLIN_STEPS_X;

		int segment = 0;
		for (int seg = 0; seg < grid.size(); ++seg) {
			if (x < grid[seg]) {
				segment = seg - 1;
				break;
			}
		}

		float left  = values[segment];
		float right = values[segment + 1];
		float frac  = (x - grid[segment]) / step_length[segment];

		perlin_table[i] = cosInterpol(left, right, frac);
		// perlin_file << i << ";" << perlin_table[i] << ";\n";
	}

	// perlin_file.close();

	float wavedraw_coefficients[SIN_AND_COS][NUMBER_OF_HARMONICS];

	float step_width = 2 * PI / PERLIN_STEPS_X;

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		float coeff_sine   = 0.f;
		float coeff_cosine = 0.f;

		for (int segment = 0; segment < PERLIN_STEPS_X; ++segment) {
			// use either const sections or linear sections

			// wrap function value at end to start
			float segment_end_value = (segment == PERLIN_STEPS_X - 1) ? perlin_table[0] : perlin_table[segment + 1];

			coeff_sine += lin_segment_one_overtone_sine(
			    segment * step_width, (segment + 1) * step_width, perlin_table[segment], segment_end_value, harmonic);
			coeff_cosine += lin_segment_one_overtone_cosine(
			    segment * step_width, (segment + 1) * step_width, perlin_table[segment], segment_end_value, harmonic);
		}
		wavedraw_coefficients[0][harmonic] = coeff_sine;
		wavedraw_coefficients[1][harmonic] = coeff_cosine;
	}

	// now create the wavetable from the fourier coefficients
	double seed_freq                                                 = 27.5; // A0
	float max                                                        = 0.f;
	float wavedraw_tables[SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] = {0};
	float p_samplerate                                               = 44100;

	// loop over subtables
	for (int index_sub_table = 0; index_sub_table < SUBTABLES_PER_WAVETABLE; ++index_sub_table) {

		// how many harmonics are needed for this subtable
		int number_of_harmonics = (int)((p_samplerate * 0.5f / seed_freq) - 1);

		// don't allow more than 800 harmonics (for big Samplerates this might
		// happen)
		number_of_harmonics = number_of_harmonics > NUMBER_OF_HARMONICS ? NUMBER_OF_HARMONICS : number_of_harmonics;

		for (int index_position = 0; index_position < WAVETABLE_LENGTH; ++index_position) {
			for (int index_harmonics = 1; index_harmonics < number_of_harmonics; ++index_harmonics) {

				// fill table with
				// sine harmonics
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[0][index_harmonics] *
				    sin(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
				// cosine
				wavedraw_tables[index_sub_table][index_position] +=
				    wavedraw_coefficients[1][index_harmonics] *
				    cos(2.f * PI * index_position * index_harmonics / (float)WAVETABLE_LENGTH);
			}
			// find max among all tables
			if (fabs(wavedraw_tables[index_sub_table][index_position]) > max) {
				max = fabs(wavedraw_tables[index_sub_table][index_position]);
			}
		}
		// increment seed frequency by minor third = 2^(3/12)
		seed_freq *= 1.1892071150;
	}

	// do another round to scale the table
	// avoid division by 0
	if (max > 1e-5) {
		max = 1.f / max; // for faster computation
	}

	std::ofstream container;
	container.open("/home/frot/odinvst/Source/audio//Oscillators/WavetableCoefficients.h",
	               std::ofstream::out | std::ofstream::app);

	// container << "#include \"Wavetables/Coefficients/" + p_table_name +
	//                 ".h\" //" + std::to_string(m_highest_loaded_table + 1) +
	//                 "\n";
	container.close();

	DBG("CREATING PERLIN TABLE /home/frot/odinvst/Source/audio/Oscillators/"
	    "Wavetables/Coefficients/" +
	    p_table_name + ".h " + std::to_string(m_highest_loaded_table + 1));

	std::ofstream output_file;
	output_file.open("/home/frot/odinvst/Source/audio/Oscillators/"
	                 "Wavetables/Coefficients/" +
	                 p_table_name + ".h");

	output_file << "#define WT_NR " << ++m_highest_loaded_table << "\n\n";
	output_file << "m_highest_loaded_table = WT_NR > m_highest_loaded_table ? "
	               "WT_NR : m_highest_loaded_table;\n";
	output_file << "m_wavetable_names_1D[WT_NR] = \"" + p_table_name + "\";\n\n";

	output_file << "m_fourier_coeffs[WT_NR][1][0] = " + to_string_no_comma(max) + ";//scalar\n\n";

	for (int harmonic = 1; harmonic < NUMBER_OF_HARMONICS; ++harmonic) {

		output_file << "m_fourier_coeffs[WT_NR][0][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[0][harmonic]) + ";\n";
		output_file << "m_fourier_coeffs[WT_NR][1][" + std::to_string(harmonic) +
		                   "] = " + to_string_no_comma(wavedraw_coefficients[1][harmonic]) + ";\n";
	}

	output_file << "#undef WT_NR\n";
	output_file.close();
}

std::string to_string_padded(int input) {
	std::string out = std::to_string(input);
	if (input < 10) {
		out = "0" + out;
	}
	if (input < 100) {
		out = "0" + out;
	}
	return out;
}

void Utilities::fixWavetableCoefficientFile() {

	int highest_table = 4;

	std::ifstream filein("/home/frot/odinvst/Source/audio//Oscillators/"
	                     "WavetableCoefficients.h");                                   // File to read from
	std::ofstream fileout("/home/frot/odinvst/Source/audio//Oscillators/TEMPORARY.h"); // Temporary
	                                                                                   // file
	if (!filein || !fileout) {
		DBG("Error opening files!");
		return;
	}

	std::string line;
	while (getline(filein, line)) {
		if (line.find("->") != std::string::npos) {
			line += to_string_padded(++highest_table);
		}
		line += "\n";
		fileout << line;
	}
}

void Utilities::fixWavetableIndexInFiles() {
	std::ifstream filein("/home/frot/odinvst/Source/audio//Oscillators/"
	                     "WavetableCoefficients.h"); // File to read from

	if (!filein) {
		DBG("Error opening files!");
		return;
	}

	std::string line;
	while (getline(filein, line)) {
		if (line.find("->") != std::string::npos) {
			unsigned first     = line.find("Coefficients/");
			unsigned last      = line.find(".h");
			std::string strNew = line.substr(first, last - first);
			std::string number = line.substr(line.length() - 3, 3);
			DBG("name is: " + strNew);
			DBG("new number is: " + number);
			fixWavetableIndexInSingleFile(strNew, std::stoi(number));
		}
	}
}

void Utilities::fixWavetableIndexInSingleFile(std::string p_filename, int p_number) {
	std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/" + p_filename +
	                     ".h"); // File to read from
	std::ofstream fileout("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/TEMP/" + p_filename + ".h");

	if (!filein || !fileout) {
		DBG("Error opening files!");
		return;
	}

	std::string line;
	while (getline(filein, line)) {
		if (line.find("#define WT_NR") != std::string::npos) {
			line = "#define WT_NR " + std::to_string(p_number);
		}
		line += "\n";
		fileout << line;
	}
}

float sgn(float x) {
	return x < 0 ? -1 : 1;
}

std::string giveSectionAfterDelimiter(std::string const &str, const char delim) {
	size_t start;
	size_t end = 0;

	std::string out;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
		end = str.find(delim, start);
		out = str.substr(start, end - start);
	}

	return out;
}

std::string makeFloatCorrectOutput(float p_input) {
	std::string out = std::to_string(p_input);
	if (out.find(".") == std::string::npos) {
		out = out + ".";
	}
	out = out + "f;";
	return out;
}

void Utilities::eliminatePhaseInWavetableCoefficients(std::string p_filename) {

	DBG("ELIMINATING PHASE IN WT FILE " + p_filename);
	// see whether files open
	{
		std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/Coefficients/" + p_filename +
		                     ".h"); // File to read from
		std::ofstream fileout("/home/frot/odinvst/Source/audio/Oscillators/"
		                      "Wavetables/Coefficients/PhaseEliminated/" +
		                      p_filename + ".h");
		if (!filein) {
			DBG("Error opening input file!");
			return;
		}
		if (!fileout) {
			DBG("Error opening output file!");
			return;
		}
	}

	// create arrays
	float sin_coeffs[NUMBER_OF_HARMONICS] = {0};
	float cos_coeffs[NUMBER_OF_HARMONICS] = {0};

	const char delimiter = '=';

	// find all sin coeffs
	for (int i = 1; i < NUMBER_OF_HARMONICS; ++i) {
		std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/Coefficients/" + p_filename +
		                     ".h"); // File to read from

		std::string string_to_find = "m_fourier_coeffs[WT_NR][0][" + std::to_string(i) + "]";

		std::string line;
		while (getline(filein, line)) {
			// DBG(line);
			if (line.find(string_to_find) != std::string::npos) {
				// get section after "="
				std::string number_as_string = giveSectionAfterDelimiter(line, delimiter);

				// remove ";" and possible trailing whitespaces
				while (number_as_string.find(";") != std::string::npos) {
					number_as_string = number_as_string.substr(0, number_as_string.size() - 1);
				}

				// remove "f"
				while (number_as_string.find("f") != std::string::npos) {
					number_as_string = number_as_string.substr(0, number_as_string.size() - 1);
				}

				// remove whitespaces at beginning
				while (number_as_string.find(" ") != std::string::npos) {
					number_as_string = number_as_string.substr(1, number_as_string.size());
				}

				sin_coeffs[i] = std::stof(number_as_string);

				// DBG(sin_coeffs[i]);
			}
		}
	}

	// find all cos coeffs
	for (int i = 1; i < NUMBER_OF_HARMONICS; ++i) {
		std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/Coefficients/" + p_filename +
		                     ".h"); // File to read from

		std::string string_to_find = "m_fourier_coeffs[WT_NR][1][" + std::to_string(i) + "]";

		std::string line;
		while (getline(filein, line)) {
			// DBG(line);
			if (line.find(string_to_find) != std::string::npos) {
				// get section after "="
				std::string number_as_string = giveSectionAfterDelimiter(line, delimiter);

				// remove ";" and possible trailing whitespaces
				while (number_as_string.find(";") != std::string::npos) {
					number_as_string = number_as_string.substr(0, number_as_string.size() - 1);
				}

				// remove "f"
				while (number_as_string.find("f") != std::string::npos) {
					number_as_string = number_as_string.substr(0, number_as_string.size() - 1);
				}

				// remove whitespaces at beginning
				while (number_as_string.find(" ") != std::string::npos) {
					number_as_string = number_as_string.substr(1, number_as_string.size());
				}

				cos_coeffs[i] = std::stof(number_as_string);

				// DBG(sin_coeffs[i]);
			}
		}
	}

	float scalar = 1;
	{
		std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/Coefficients/" + p_filename +
		                     ".h"); // File to read from

		std::string string_to_find = "m_fourier_coeffs[WT_NR][1][0]";

		std::string line;
		while (getline(filein, line)) {
			// DBG(line);
			if (line.find(string_to_find) != std::string::npos) {
				// get section after "="
				std::string number_as_string = giveSectionAfterDelimiter(line, delimiter);

				// remove ";" and possible trailing whitespaces
				while (number_as_string.find(";") != std::string::npos) {
					number_as_string = number_as_string.substr(0, number_as_string.size() - 1);
				}

				// remove "f"
				while (number_as_string.find("f") != std::string::npos) {
					number_as_string = number_as_string.substr(0, number_as_string.size() - 1);
				}

				// remove whitespaces at beginning
				while (number_as_string.find(" ") != std::string::npos) {
					number_as_string = number_as_string.substr(1, number_as_string.size());
				}
				scalar = std::stof(number_as_string);
			}
		}
	}

	// now write to file
	std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/Coefficients/" + p_filename +
	                     ".h"); // File to read from

	std::ofstream fileout("/home/frot/odinvst/Source/audio/Oscillators/"
	                      "Wavetables/Coefficients/PhaseEliminated/" +
	                      p_filename + ".h");

	// write the first few lines which are not coefficients
	std::string line;
	while (getline(filein, line)) {
		if (line.find("coeffs[WT_NR]") == std::string::npos && line.find("undef") == std::string::npos) {
			line += "\n";
			fileout << line;
		}
	}

	// write the scalar
	fileout << "\n\nm_fourier_coeffs[WT_NR][1][0] = " + makeFloatCorrectOutput(scalar) + " // scalar\n\n";

	// write the actual table to file
	for (int i = 1; i < NUMBER_OF_HARMONICS; ++i) {

		// calc new coeff.
		// see https://de.wikipedia.org/wiki/Formelsammlung_Trigonometrie
		// section "Sinusoid und Linearkombination mit gleicher Phase"
		// note: ommiting sgn(x)

		float new_coefficient = sqrt((sin_coeffs[i]) * (sin_coeffs[i]) + (cos_coeffs[i]) * (cos_coeffs[i]));

		fileout << "m_fourier_coeffs[WT_NR][0][" + std::to_string(i) +
		               "] = " + makeFloatCorrectOutput(new_coefficient) + "\n";
	}

	// write undef
	fileout << "\n\n#undef WT_NR";
}

int Utilities::getWavetableIndexFromName(std::string p_name) {
	auto it = m_name_index_map.find(p_name);
	if (it != m_name_index_map.end()) {
		return it->second;
	}
	DBG("getWavetableIndexFromName(): COULDNT FIND WT WITH NAME " + p_name);
	return 0;
}

bool containsTooHighHarmonics(std::string p_input) {
	// return true if it contains "[500]" for example
	for (int i = 256; i < 802; ++i) {
		if (p_input.find("[" + std::to_string(i) + "]") != std::string::npos) {
			return true;
		}
	}
	return false;
}

void Utilities::fixTooHighHarmonics(std::string p_filename) {
	DBG("REMOVING TOO HIGH HARMONICS IN " + p_filename);

	std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/Wavetables/Coefficients/" + p_filename +
	                     ".h"); // File to read from
	std::ofstream fileout("/home/frot/odinvst/Source/audio/Oscillators/"
	                      "Wavetables/Coefficients/TEMP/" +
	                      p_filename + ".h");

	if (!filein || !fileout) {
		DBG("Error opening files!");
		return;
	}

	std::string line;
	while (getline(filein, line)) {
		if (!containsTooHighHarmonics(line)) {
			fileout << line << "\n";
		}
	}

	fileout.close();
}

void Utilities::convertWTFromOdin1(int p_odin_1_nr, int p_odin_2_nr, std::string p_name) {

	// see whether files open

	std::ifstream filein("/home/frot/odinvst/odin_1_wavetables.h"); // File to read from
	std::ofstream fileout("/home/frot/odinvst/Source/audio/Oscillators/"
	                      "Wavetables/Coefficients/" +
	                      p_name + ".h");
	if (!filein) {
		DBG("Error opening input file!");
		return;
	}
	if (!fileout) {
		DBG("Error opening output file!");
		return;
	}

	// customWT27CoeffsEven[787] = -0.000000;

	fileout << "#define WT_NR " << p_odin_2_nr << "\n\n";
	fileout << "m_wavetable_names_1D[WT_NR] = \"" + p_name + "\";\n\n";
	fileout << "m_fourier_coeffs[WT_NR][1][0] = 1;//scalar\n\n";

	std::string line;
	while (getline(filein, line)) {
		if (line.find("customWT" + std::to_string(p_odin_1_nr) + "Coeffs") != std::string::npos &&
		    !containsTooHighHarmonics(line)) {
			// here line is below 256 and is desired wavetable
			// now replace name
			int number_size = p_odin_1_nr >= 10 ? 2 : 1;
			if (line.find("Even") != std::string::npos) {
				line = line.substr(18 + number_size);
				line = "m_fourier_coeffs[WT_NR][1]" + line;
			}
			if (line.find("Odd") != std::string::npos) {
				line = line.substr(17 + number_size);
				line = "m_fourier_coeffs[WT_NR][0]" + line;
			}
			if (line.find("[1][0]") == std::string::npos) {
				fileout << line << "\n";
			}
		}
	}

	// write undef
	fileout << "\n\n#undef WT_NR";

	fileout.close();

	eliminatePhaseInWavetableCoefficients(p_name);

	std::ifstream src("/home/frot/odinvst/Source/audio/Oscillators/"
	                  "Wavetables/Coefficients/PhaseEliminated/" +
	                      p_name + ".h",
	                  std::ios::binary);
	std::ofstream dst("/home/frot/odinvst/Source/audio/Oscillators/"
	                  "Wavetables/Coefficients/" +
	                      p_name + ".h",
	                  std::ios::binary);

	dst << src.rdbuf();

	dst.close();
}

void Utilities::printWTsNotUsedAsCoeffs() {

	std::vector<std::string> string_vec = {"lol"};

	for (auto const &wavetable_name : string_vec) {
		std::ifstream filein("/home/frot/odinvst/Source/audio/Oscillators/"
		                     "WavetableOsc2D.cpp"); // File to read from

		if (!filein) {
			DBG("Error opening file!");
			return;
		}

		std::string line;
		bool found_string = false;
		while (getline(filein, line)) {
			if (line.find(wavetable_name) != std::string::npos) {
				found_string = true;
			}
		}
		if (!found_string) {
			std::cout << "USELESS WT: " << wavetable_name << "\n";
		}
	}
}
