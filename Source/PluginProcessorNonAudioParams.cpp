/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

//this file is included from PluginProcessor.cpp to split the class implementation

#include "PluginProcessor.h"

void OdinAudioProcessor::addNonAudioParametersToTree() {

	writeDefaultWavedrawValuesToTree(1);
	writeDefaultWavedrawValuesToTree(2);
	writeDefaultWavedrawValuesToTree(3);

	writeDefaultChipdrawValuesToTree(1);
	writeDefaultChipdrawValuesToTree(2);
	writeDefaultChipdrawValuesToTree(3);

	writeDefaultSpecdrawValuesToTree(1);
	writeDefaultSpecdrawValuesToTree(2);
	writeDefaultSpecdrawValuesToTree(3);

	auto node = m_value_tree_fx;
	node.setProperty("delay_synctime_numerator", 2, nullptr);
	node.setProperty("delay_synctime_denominator", 5, nullptr);
	node.setProperty("phaser_synctime_numerator", 2, nullptr);
	node.setProperty("phaser_synctime_denominator", 5, nullptr);
	node.setProperty("flanger_synctime_numerator", 2, nullptr);
	node.setProperty("flanger_synctime_denominator", 5, nullptr);
	node.setProperty("chorus_synctime_numerator", 2, nullptr);
	node.setProperty("chorus_synctime_denominator", 5, nullptr);
	node.setProperty("delay_selected", 1, nullptr);
	node.setProperty("phaser_selected", 0, nullptr);
	node.setProperty("flanger_selected", 0, nullptr);
	node.setProperty("chorus_selected", 0, nullptr);
	node.setProperty("reverb_selected", 0, nullptr);
	node.setProperty("delay_position", 0, nullptr);
	node.setProperty("phaser_position", 1, nullptr);
	node.setProperty("chorus_position", 2, nullptr);
	node.setProperty("flanger_position", 3, nullptr);
	node.setProperty("reverb_position", 4, nullptr);
	node.setProperty("delay_sync", 0, nullptr);
	node.setProperty("phaser_sync", 0, nullptr);
	node.setProperty("flanger_sync", 0, nullptr);
	node.setProperty("chorus_sync", 0, nullptr);

	node = m_value_tree_lfo;
	node.setProperty("lfo1_synctime_numerator", 2, nullptr);
	node.setProperty("lfo1_synctime_denominator", 5, nullptr);
	node.setProperty("lfo2_synctime_numerator", 2, nullptr);
	node.setProperty("lfo2_synctime_denominator", 5, nullptr);
	node.setProperty("lfo3_synctime_numerator", 2, nullptr);
	node.setProperty("lfo3_synctime_denominator", 5, nullptr);
	node.setProperty("lfo4_synctime_numerator", 2, nullptr);
	node.setProperty("lfo4_synctime_denominator", 5, nullptr);
	node.setProperty("lfo_left_selected", 1, nullptr);
	node.setProperty("lfo_right_selected", 1, nullptr);
	node.setProperty("lfo1_wave", 0, nullptr);
	node.setProperty("lfo2_wave", 0, nullptr);
	node.setProperty("lfo3_wave", 0, nullptr);
	node.setProperty("lfo4_wave", 0, nullptr);
	node.setProperty("lfo1_sync", 0, nullptr);
	node.setProperty("lfo2_sync", 0, nullptr);
	node.setProperty("lfo3_sync", 0, nullptr);
	node.setProperty("lfo4_sync", 0, nullptr);

	node = m_value_tree_misc;
	// legato 0, poly 1, mono 2, BUT see mapping in globalincludes.h (valuetree != dropdown values)
	node.setProperty("legato", 1, nullptr);
	node.setProperty("dist_algo", 1.f, nullptr);
	node.setProperty("BPM", 120, nullptr);
	node.setProperty("env_left_selected", 1, nullptr);
	node.setProperty("env_right_selected", 1, nullptr);
	node.setProperty("fil1_type", FILTER_TYPE_LP24, nullptr);
	node.setProperty("fil2_type", 1, nullptr);
	node.setProperty("fil3_type", 1, nullptr);
	node.setProperty("patch_name", String("Init Patch"), nullptr);
	// these are synth version
	node.setProperty("version_minor", ODIN_MINOR_VERSION, nullptr);
	node.setProperty("version_patch", ODIN_PATCH_VERSION, nullptr);
	// this is patch version
	node.setProperty("patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);
	node.setProperty("dist_on", 0, nullptr);
	node.setProperty("fil1_comb_polarity", 0, nullptr);
	node.setProperty("fil2_comb_polarity", 0, nullptr);
	node.setProperty("fil3_comb_polarity", 0, nullptr);
	node.setProperty("fil1_vowel_left", 0, nullptr);
	node.setProperty("fil2_vowel_left", 0, nullptr);
	node.setProperty("fil3_vowel_left", 0, nullptr);
	node.setProperty("fil1_vowel_right", 2, nullptr);
	node.setProperty("fil2_vowel_right", 2, nullptr);
	node.setProperty("fil3_vowel_right", 2, nullptr);
	node.setProperty("unison_voices", 1, nullptr);
	node.setProperty("pitchbend_amount", 12, nullptr);
	node.setProperty("current_patch_filename", "Init Patch", nullptr); //needed for up/down buttons in patch loading
	node.setProperty("current_patch_directory",
	                 File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName(),
	                 nullptr);
	node.setProperty("arp_synctime_numerator", 1, nullptr);
	node.setProperty("arp_synctime_denominator", 5, nullptr);
	node.setProperty("arp_octaves", 2, nullptr);
	node.setProperty("arp_direction", (int)OdinArpeggiator::ArpPattern::Up, nullptr);
	node.setProperty("arp_steps", 16, nullptr);
	node.setProperty("arp_gate", 50, nullptr);
	node.setProperty("arp_mod_selected", 0, nullptr); // switches arp, modmatrix and patchbrowser
	node.setProperty("preset_soundbank_selected", "", nullptr);
	node.setProperty("preset_category_selected", "", nullptr);
	node.setProperty("preset_patch_selected", "", nullptr);

	node = m_value_tree_mod;
	node.setProperty("source_row_0", 0, nullptr);
	node.setProperty("source_row_1", 0, nullptr);
	node.setProperty("source_row_2", 0, nullptr);
	node.setProperty("source_row_3", 0, nullptr);
	node.setProperty("source_row_4", 0, nullptr);
	node.setProperty("source_row_5", 0, nullptr);
	node.setProperty("source_row_6", 0, nullptr);
	node.setProperty("source_row_7", 0, nullptr);
	node.setProperty("source_row_8", 0, nullptr);
	node.setProperty("dest_1_row_0", 0, nullptr);
	node.setProperty("dest_1_row_1", 0, nullptr);
	node.setProperty("dest_1_row_2", 0, nullptr);
	node.setProperty("dest_1_row_3", 0, nullptr);
	node.setProperty("dest_1_row_4", 0, nullptr);
	node.setProperty("dest_1_row_5", 0, nullptr);
	node.setProperty("dest_1_row_6", 0, nullptr);
	node.setProperty("dest_1_row_7", 0, nullptr);
	node.setProperty("dest_1_row_8", 0, nullptr);
	node.setProperty("dest_2_row_0", 0, nullptr);
	node.setProperty("dest_2_row_1", 0, nullptr);
	node.setProperty("dest_2_row_2", 0, nullptr);
	node.setProperty("dest_2_row_3", 0, nullptr);
	node.setProperty("dest_2_row_4", 0, nullptr);
	node.setProperty("dest_2_row_5", 0, nullptr);
	node.setProperty("dest_2_row_6", 0, nullptr);
	node.setProperty("dest_2_row_7", 0, nullptr);
	node.setProperty("dest_2_row_8", 0, nullptr);
	node.setProperty("scale_row_0", 0, nullptr);
	node.setProperty("scale_row_1", 0, nullptr);
	node.setProperty("scale_row_2", 0, nullptr);
	node.setProperty("scale_row_3", 0, nullptr);
	node.setProperty("scale_row_4", 0, nullptr);
	node.setProperty("scale_row_5", 0, nullptr);
	node.setProperty("scale_row_6", 0, nullptr);
	node.setProperty("scale_row_7", 0, nullptr);
	node.setProperty("scale_row_8", 0, nullptr);

	for (int row = 0; row < 9; ++row) {
		for (int horizontal = 0; horizontal < 3; ++horizontal) {
			node.setProperty(
			    Identifier("amount_" + std::to_string(horizontal) + "_row_" + std::to_string(row)), 0, nullptr);
		}
	}

	node = m_value_tree_osc;
	node.setProperty("osc1_analog_wave", 0, nullptr);
	node.setProperty("osc2_analog_wave", 0, nullptr);
	node.setProperty("osc3_analog_wave", 0, nullptr);
	node.setProperty("osc1_type", OSC_TYPE_ANALOG, nullptr);
	node.setProperty("osc2_type", 1, nullptr);
	node.setProperty("osc3_type", 1, nullptr);
	node.setProperty("osc1_wavetable", 1, nullptr);
	node.setProperty("osc2_wavetable", 1, nullptr);
	node.setProperty("osc3_wavetable", 1, nullptr);
	node.setProperty("osc1_vec_a", 101, nullptr);
	node.setProperty("osc2_vec_a", 101, nullptr);
	node.setProperty("osc3_vec_a", 101, nullptr);
	node.setProperty("osc1_vec_b", 102, nullptr);
	node.setProperty("osc2_vec_b", 102, nullptr);
	node.setProperty("osc3_vec_b", 102, nullptr);
	node.setProperty("osc1_vec_c", 103, nullptr);
	node.setProperty("osc2_vec_c", 103, nullptr);
	node.setProperty("osc3_vec_c", 103, nullptr);
	node.setProperty("osc1_vec_d", 104, nullptr);
	node.setProperty("osc2_vec_d", 104, nullptr);
	node.setProperty("osc3_vec_d", 104, nullptr);
	node.setProperty("osc1_chipwave", 1, nullptr);
	node.setProperty("osc2_chipwave", 1, nullptr);
	node.setProperty("osc3_chipwave", 1, nullptr);
	node.setProperty("osc1_modulator_wave", 1, nullptr);
	node.setProperty("osc2_modulator_wave", 1, nullptr);
	node.setProperty("osc3_modulator_wave", 1, nullptr);
	node.setProperty("osc1_carrier_wave", 1, nullptr);
	node.setProperty("osc2_carrier_wave", 1, nullptr);
	node.setProperty("osc3_carrier_wave", 1, nullptr);
	node.setProperty("osc1_mod_source", 1, nullptr);
	node.setProperty("osc2_mod_source", 1, nullptr);
	node.setProperty("osc3_mod_source", 1, nullptr);
	node.setProperty("osc1_carrier_ratio", 1, nullptr);
	node.setProperty("osc2_carrier_ratio", 1, nullptr);
	node.setProperty("osc3_carrier_ratio", 1, nullptr);
	node.setProperty("osc1_modulator_ratio", 1, nullptr);
	node.setProperty("osc2_modulator_ratio", 1, nullptr);
	node.setProperty("osc3_modulator_ratio", 1, nullptr);
}

void OdinAudioProcessor::writeDefaultWavedrawValuesToTree(int p_osc) {

	//p_osc = {1,2,3}
	std::string osc_index_string = std::to_string(p_osc);

	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
		float val = sin(2 * M_PI * i / (float)WAVEDRAW_STEPS_X) * 0.9;
		m_value_tree_draw.setProperty(
		    String("osc" + osc_index_string + "_wavedraw_values_" + std::to_string(i)), val, nullptr);
	}
}

void OdinAudioProcessor::writeDefaultChipdrawValuesToTree(int p_osc) {

	//p_osc = {1,2,3}
	std::string osc_index_string = std::to_string(p_osc);

	for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
		float val = i < CHIPDRAW_STEPS_X / 2 ? 0.875f : -0.875f;
		m_value_tree_draw.setProperty(
		    String("osc" + osc_index_string + "_chipdraw_values_" + std::to_string(i)), val, nullptr);
	}
}

void OdinAudioProcessor::writeDefaultSpecdrawValuesToTree(int p_osc) {

	//p_osc = {1,2,3}
	std::string osc_index_string = std::to_string(p_osc);

	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		float val = i == 0 ? 1 : 0;
		m_value_tree_draw.setProperty(
		    String("osc" + osc_index_string + "_specdraw_values_" + std::to_string(i)), val, nullptr);
	}
}