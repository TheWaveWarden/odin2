#include "WavetableOsc1D.h"
#include <cmath>

//#include "../JuceLibraryCode/JuceHeader.h"

#include "../../WavetableMappings.h"

WavetableOsc1D::WavetableOsc1D() {
}

WavetableOsc1D::~WavetableOsc1D() {
}

void WavetableOsc1D::setVoldB(float p_dB) {
	//m_volume_factor = pow(10, dB / 20);
	//ln(10) / 20 = 0.11512f
	m_volume_factor = juce::dsp::FastMathApproximations::exp(0.1151292 * p_dB);
}

void WavetableOsc1D::reset() {

	// call baseclass first
	Oscillator::reset();

	m_read_index = 0.0;
	m_dc_blocking_filter.reset();

	// downsampling filter buffers
	for (int i = 0; i < 10; ++i) {
		xv[i] = yv[i] = 0;
	}
}

void WavetableOsc1D::update() {

	// baseclass call
	Oscillator::update();

	m_wavetable_inc   = WAVETABLE_LENGTH * m_increment;
	m_sub_table_index = getTableIndex();

	// set wavetable pointerd
	m_current_table = m_wavetable_pointers[m_wavetable_index][m_sub_table_index];
}

int WavetableOsc1D::getTableIndex() {

	//compare to mid freq of last used interval
	if (m_one_over_last_mid_freq * m_osc_freq_modded < ROOT_OF_MINOR_THIRD &&
	    m_one_over_last_mid_freq * m_osc_freq_modded > ONE_OVER_ROOT_OF_MINOR_THIRD) {
		//still is in range of last used note
		return m_last_table_index;
	}

	float seed_freq = 27.5; // A0
	float abs_freq  = fabs(m_osc_freq_modded);
	for (int table = 0; table < SUBTABLES_PER_WAVETABLE; ++table) {
		if (abs_freq < seed_freq) {
			//we save the inverse midfreq of last found interval to use a few lines up
			m_one_over_last_mid_freq = m_table_one_over_mid_freq_values[table];
			m_last_table_index       = table;
			return table;
		}
		seed_freq *= 1.189207f; // minor third up
	}
	return SUBTABLES_PER_WAVETABLE - 1; // never gets here
}

float WavetableOsc1D::doWavetable() {

	// prepare both sides and interpol value
	int read_index_trunc = (int)m_read_index;
	float fractional     = m_read_index - (float)read_index_trunc;
	int read_index_next  = read_index_trunc + 1 >= WAVETABLE_LENGTH ? 0 : read_index_trunc + 1;

	// do linear interpolation
	float output = linearInterpolation(m_current_table[read_index_trunc], m_current_table[read_index_next], fractional);

	m_read_index += m_wavetable_inc * m_sync_anti_aliasing_inc_factor;
	checkWrapIndex(m_read_index);

	return output;
}

float WavetableOsc1D::doOscillate() {

	float vol_mod_factor = (*m_vol_mod) > 0 ? 1.f + 4 * (*m_vol_mod) : (1.f + *m_vol_mod);
	vol_mod_factor       = vol_mod_factor > VOL_MOD_UPPER_LIMIT ? VOL_MOD_UPPER_LIMIT : vol_mod_factor;

	return doWavetable() * m_volume_factor * vol_mod_factor;
}

void WavetableOsc1D::setWavetablePointer(int p_wavetable_index,
                                         const float *p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]) {
	for (int sub_table = 0; sub_table < SUBTABLES_PER_WAVETABLE; sub_table++) {
		m_wavetable_pointers[p_wavetable_index][sub_table] = p_wavetable_pointers[sub_table];
	}
}

// nonconst is needed for draw tables
void WavetableOsc1D::setWavetablePointerNONCONST(int p_wavetable_index,
                                                 float *p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]) {
	for (int sub_table = 0; sub_table < SUBTABLES_PER_WAVETABLE; sub_table++) {
		m_wavetable_pointers[p_wavetable_index][sub_table] = p_wavetable_pointers[sub_table];
	}
}

void WavetableOsc1D::selectWavetable(int p_wavetable_index) {
	if (p_wavetable_index < m_nr_of_wavetables) {
		m_wavetable_index = p_wavetable_index;
	}
}

void WavetableOsc1D::loadWavetables() {
	// for standard osc simply get all wavetables
	for (int wavetable = 0; wavetable < NUMBER_OF_WAVETABLES; ++wavetable) {
		setWavetablePointer(wavetable, WavetableContainer::getInstance().getWavetablePointers(wavetable));
	}
	for (int i = 0; i < 3; ++i) {
		// wavedraw
		setWavetablePointerNONCONST(NUMBER_OF_WAVETABLES + i, WavetableContainer::getInstance().getWavedrawPointer(i));
		// chipdraw
		setWavetablePointerNONCONST(NUMBER_OF_WAVETABLES + i + 3,
		                            WavetableContainer::getInstance().getChipdrawPointer(i));
		// specdraw
		setWavetablePointerNONCONST(NUMBER_OF_WAVETABLES + i + 6,
		                            WavetableContainer::getInstance().getSpecdrawPointer(i));
	}
}

void WavetableOsc1D::loadWavedrawTables(int p_osc) {
	setWavetablePointerNONCONST(0, WavetableContainer::getInstance().getWavedrawPointer(p_osc));
	m_nr_of_wavetables = 1;
}

void WavetableOsc1D::loadSpecdrawTables(int p_osc) {
	setWavetablePointerNONCONST(0, WavetableContainer::getInstance().getSpecdrawPointer(p_osc));
	m_nr_of_wavetables = 1;
}

void WavetableOsc1D::loadChipdrawTables(int p_osc) {
	setWavetablePointerNONCONST(0, WavetableContainer::getInstance().getChipdrawPointer(p_osc));
	m_nr_of_wavetables = 1;
}

void WavetableOsc1D::initiateSync() {
	m_read_index = m_sync_oscillator->m_reset_position;
}

float WavetableOsc1D::doOscillateWithSync() {

	// do sync shit: 3x oversampling for AA
	if (m_sync_enabled && m_sync_oscillator) {
		// check if a new reset flag was set:
		if (m_sync_oscillator->m_reset_flag) {
			initiateSync();
		}
		m_sync_anti_aliasing_inc_factor = 0.3333333f;

		float input_upsampled[3] = {doOscillate(), doOscillate(), doOscillate()};

		xv[0] = xv[1];
		xv[1] = xv[2];
		xv[2] = xv[3];
		xv[3] = xv[4];
		xv[4] = xv[5];
		xv[5] = xv[6];
		xv[6] = xv[7];
		xv[7] = xv[8];
		xv[8] = xv[9];
		xv[9] = input_upsampled[0] * 0.019966841051093;
		yv[0] = yv[1];
		yv[1] = yv[2];
		yv[2] = yv[3];
		yv[3] = yv[4];
		yv[4] = yv[5];
		yv[5] = yv[6];
		yv[6] = yv[7];
		yv[7] = yv[8];
		yv[8] = yv[9];
		yv[9] = (xv[0] + xv[9]) + 9 * (xv[1] + xv[8]) + 36 * (xv[2] + xv[7]) + 84 * (xv[3] + xv[6]) +
		        126 * (xv[4] + xv[5]) + (-0.0003977153 * yv[0]) + (-0.0064474617 * yv[1]) + (-0.0476997403 * yv[2]) +
		        (-0.2185829743 * yv[3]) + (-0.6649234123 * yv[4]) + (-1.4773657709 * yv[5]) + (-2.2721421641 * yv[6]) +
		        (-2.6598673212 * yv[7]) + (-1.8755960587 * yv[8]);
		// next output value = yv[9];
		xv[0] = xv[1];
		xv[1] = xv[2];
		xv[2] = xv[3];
		xv[3] = xv[4];
		xv[4] = xv[5];
		xv[5] = xv[6];
		xv[6] = xv[7];
		xv[7] = xv[8];
		xv[8] = xv[9];
		xv[9] = input_upsampled[1] * 0.019966841051093;
		yv[0] = yv[1];
		yv[1] = yv[2];
		yv[2] = yv[3];
		yv[3] = yv[4];
		yv[4] = yv[5];
		yv[5] = yv[6];
		yv[6] = yv[7];
		yv[7] = yv[8];
		yv[8] = yv[9];
		yv[9] = (xv[0] + xv[9]) + 9 * (xv[1] + xv[8]) + 36 * (xv[2] + xv[7]) + 84 * (xv[3] + xv[6]) +
		        126 * (xv[4] + xv[5]) + (-0.0003977153 * yv[0]) + (-0.0064474617 * yv[1]) + (-0.0476997403 * yv[2]) +
		        (-0.2185829743 * yv[3]) + (-0.6649234123 * yv[4]) + (-1.4773657709 * yv[5]) + (-2.2721421641 * yv[6]) +
		        (-2.6598673212 * yv[7]) + (-1.8755960587 * yv[8]);
		// next output value = yv[9];
		xv[0] = xv[1];
		xv[1] = xv[2];
		xv[2] = xv[3];
		xv[3] = xv[4];
		xv[4] = xv[5];
		xv[5] = xv[6];
		xv[6] = xv[7];
		xv[7] = xv[8];
		xv[8] = xv[9];
		xv[9] = input_upsampled[2] * 0.019966841051093;
		yv[0] = yv[1];
		yv[1] = yv[2];
		yv[2] = yv[3];
		yv[3] = yv[4];
		yv[4] = yv[5];
		yv[5] = yv[6];
		yv[6] = yv[7];
		yv[7] = yv[8];
		yv[8] = yv[9];
		yv[9] = (xv[0] + xv[9]) + 9 * (xv[1] + xv[8]) + 36 * (xv[2] + xv[7]) + 84 * (xv[3] + xv[6]) +
		        126 * (xv[4] + xv[5]) + (-0.0003977153 * yv[0]) + (-0.0064474617 * yv[1]) + (-0.0476997403 * yv[2]) +
		        (-0.2185829743 * yv[3]) + (-0.6649234123 * yv[4]) + (-1.4773657709 * yv[5]) + (-2.2721421641 * yv[6]) +
		        (-2.6598673212 * yv[7]) + (-1.8755960587 * yv[8]);

		return m_dc_blocking_filter.doFilter(yv[9]);

	} else {
		// either sync off or syncosc not set:
		m_sync_anti_aliasing_inc_factor = 1.f;
		return doOscillate();
	}
}
