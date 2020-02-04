void OdinAudioProcessor::addNonAudioParametersToTree() {

	auto node = m_value_tree_draw;

	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
		float val = sin(2 * M_PI * i / (float)WAVEDRAW_STEPS_X) * 0.9;
		node.setProperty(String("osc1_wavedraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc2_wavedraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc3_wavedraw_values_" + std::to_string(i)), val, nullptr);
	}
	for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
		float val = i < CHIPDRAW_STEPS_X / 2 ? 0.875f : -0.875f;
		node.setProperty(String("osc1_chipdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc2_chipdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc3_chipdraw_values_" + std::to_string(i)), val, nullptr);
	}
	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		float val = i == 0 ? 1 : 0;
		node.setProperty(String("osc1_specdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc2_specdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc3_specdraw_values_" + std::to_string(i)), val, nullptr);
		//		node.setProperty(String("osc3_specdraw_values_" + std::to_string(i)), 0.5f-i*0.005+cos(i*0.2f)*0.5f*(1.f-i*0.02), nullptr);
	}

	node = m_value_tree_fx;
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
	node.setProperty("delay_position", 0, nullptr);
	node.setProperty("phaser_position", 1, nullptr);
	node.setProperty("chorus_position", 2, nullptr);
	node.setProperty("flanger_position", 3, nullptr);

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

	node = m_value_tree_misc;
	node.setProperty("legato", 1, nullptr); // this is actually "poly" or !legato"
	node.setProperty("dist_algo", 1.f, nullptr);
	node.setProperty("BPM", 120, nullptr);
	node.setProperty("env_left_selected", 1, nullptr);
	node.setProperty("env_right_selected", 1, nullptr);
	node.setProperty("fil1_type", FILTER_TYPE_LP24, nullptr);
	node.setProperty("fil2_type", 1, nullptr);
	node.setProperty("fil3_type", 1, nullptr);
	node.setProperty("patch_name", String("init_patch"), nullptr);
	// these are synth version
	node.setProperty("version_minor", ODIN_MINOR_VERSION, nullptr);
	node.setProperty("version_patch", ODIN_PATCH_VERSION, nullptr);
	// this is patch version
	node.setProperty("patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);

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
			node.setProperty(Identifier("amount_" + std::to_string(horizontal) + "_row_" + std::to_string(row)), 0, nullptr);
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
}