/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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

#pragma once
for (int osc = 0; osc < 3; ++osc) {
	m_osc_oct[osc]            = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_oct");
	m_osc_semi[osc]           = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_semi");
	m_osc_fine[osc]           = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_fine");
	m_osc_vol[osc]            = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_vol");
	m_osc_position[osc]       = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_position");
	m_osc_detune[osc]         = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_detune");
	m_osc_pos_env[osc]         = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_pos_mod");
	m_osc_multi_position[osc] = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_multi_position");
	m_osc_spread[osc]         = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_spread");
	m_osc_pulsewidth[osc]     = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_pulsewidth");
	m_osc_drift[osc]          = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_drift");
	m_osc_arp_speed[osc]      = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_arp_speed");
	m_osc_step_1[osc]         = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_step_1");
	m_osc_step_2[osc]         = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_step_2");
	m_osc_step_3[osc]         = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_step_3");
	m_osc_fm[osc]             = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_fm");
	//m_osc_carrier_ratio[osc]  = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_carrier_ratio");
	//m_osc_modulator_ratio[osc] =
	//    m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_modulator_ratio");
	m_osc_lp[osc]    = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_lp");
	m_osc_hp[osc]    = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_hp");
	m_osc_reset[osc] = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_reset");
	if (osc != 0) {
		m_osc_sync[osc] = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_sync");
	}
	m_osc_arp_on[osc]    = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_arp_on");
	m_osc_step_3_on[osc] = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_step_3_on");
	m_osc_chipnoise[osc] = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_chipnoise");
	m_osc_exp_fm[osc]    = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_exp_fm");
	m_osc_vec_x[osc]     = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_vec_x");
	m_osc_vec_y[osc]     = m_value_tree.getRawParameterValue("osc" + std::to_string(osc + 1) + "_vec_y");
}

for (int fil = 0; fil < 3; ++fil) {
	m_fil_vel[fil]        = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_vel");
	m_fil_env[fil]        = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_env");
	m_fil_kbd[fil]        = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_kbd");
	m_fil_gain[fil]       = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_gain");
	m_fil_freq[fil]       = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_freq");
	m_fil_res[fil]        = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_res");
	m_fil_saturation[fil] = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_saturation");
	m_fil_formant_transition[fil] =
	    m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_formant_transition");
	m_fil_ring_mod_amount[fil] =
	    m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_ring_mod_amount");
	m_fil_sem_transition[fil] = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_sem_transition");
}

m_amp_pan        = m_value_tree.getRawParameterValue("amp_pan");
m_amp_gain       = m_value_tree.getRawParameterValue("amp_gain");
m_amp_velocity        = m_value_tree.getRawParameterValue("amp_velocity");
m_dist_threshold = m_value_tree.getRawParameterValue("dist_boost");
m_dist_drywet    = m_value_tree.getRawParameterValue("dist_drywet");
m_fil1_to_amp    = m_value_tree.getRawParameterValue("fil1_to_amp");
m_fil2_to_amp    = m_value_tree.getRawParameterValue("fil2_to_amp");

m_env1_attack  = m_value_tree.getRawParameterValue("env1_attack");
m_env1_decay   = m_value_tree.getRawParameterValue("env1_decay");
m_env1_sustain = m_value_tree.getRawParameterValue("env1_sustain");
m_env1_release = m_value_tree.getRawParameterValue("env1_release");
m_env1_loop    = m_value_tree.getRawParameterValue("env1_loop");

m_env2_attack  = m_value_tree.getRawParameterValue("env2_attack");
m_env2_decay   = m_value_tree.getRawParameterValue("env2_decay");
m_env2_sustain = m_value_tree.getRawParameterValue("env2_sustain");
m_env2_release = m_value_tree.getRawParameterValue("env2_release");
m_env2_loop    = m_value_tree.getRawParameterValue("env2_loop");

m_env3_attack  = m_value_tree.getRawParameterValue("env3_attack");
m_env3_decay   = m_value_tree.getRawParameterValue("env3_decay");
m_env3_sustain = m_value_tree.getRawParameterValue("env3_sustain");
m_env3_release = m_value_tree.getRawParameterValue("env3_release");
m_env3_loop    = m_value_tree.getRawParameterValue("env3_loop");

m_env4_attack  = m_value_tree.getRawParameterValue("env4_attack");
m_env4_decay   = m_value_tree.getRawParameterValue("env4_decay");
m_env4_sustain = m_value_tree.getRawParameterValue("env4_sustain");
m_env4_release = m_value_tree.getRawParameterValue("env4_release");
m_env4_loop    = m_value_tree.getRawParameterValue("env4_loop");

m_lfo1_freq  = m_value_tree.getRawParameterValue("lfo1_freq");
m_lfo1_reset = m_value_tree.getRawParameterValue("lfo1_reset");

m_lfo2_freq  = m_value_tree.getRawParameterValue("lfo2_freq");
m_lfo2_reset = m_value_tree.getRawParameterValue("lfo2_reset");

m_lfo3_freq  = m_value_tree.getRawParameterValue("lfo3_freq");
m_lfo3_reset = m_value_tree.getRawParameterValue("lfo3_reset");

m_lfo4_freq  = m_value_tree.getRawParameterValue("lfo4_freq");
m_lfo4_reset = m_value_tree.getRawParameterValue("lfo4_reset");

m_phaser_on  = m_value_tree.getRawParameterValue("phaser_on");
m_flanger_on = m_value_tree.getRawParameterValue("flanger_on");
m_delay_on   = m_value_tree.getRawParameterValue("delay_on");
m_chorus_on  = m_value_tree.getRawParameterValue("chorus_on");

for (int fil = 0; fil < 2; ++fil) {
	m_fil_osc1[fil] = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_osc1");
	m_fil_osc2[fil] = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_osc2");
	m_fil_osc3[fil] = m_value_tree.getRawParameterValue("fil" + std::to_string(fil + 1) + "_osc3");
}

m_fil2_fil1        = m_value_tree.getRawParameterValue("fil2_fil1");
m_xy_x             = m_value_tree.getRawParameterValue("xy_x");
m_xy_y             = m_value_tree.getRawParameterValue("xy_y");
m_glide            = m_value_tree.getRawParameterValue("glide");
m_master           = m_value_tree.getRawParameterValue("master");
m_modwheel         = m_value_tree.getRawParameterValue("modwheel");
m_pitchbend        = m_value_tree.getRawParameterValue("pitchbend");
//m_pitchbend_amount = m_value_tree.getRawParameterValue("pitchbend_amount");

m_delay_time     = m_value_tree.getRawParameterValue("delay_time");
m_delay_feedback = m_value_tree.getRawParameterValue("delay_feedback");
m_delay_hp       = m_value_tree.getRawParameterValue("delay_hp");
m_delay_ducking  = m_value_tree.getRawParameterValue("delay_ducking");
m_delay_dry      = m_value_tree.getRawParameterValue("delay_dry");
m_delay_wet      = m_value_tree.getRawParameterValue("delay_wet");
//m_delay_sync     = m_value_tree.getRawParameterValue("delay_sync");
m_delay_pingpong = m_value_tree.getRawParameterValue("delay_pingpong");

m_phaser_rate     = m_value_tree.getRawParameterValue("phaser_rate");
m_phaser_freq     = m_value_tree.getRawParameterValue("phaser_freq");
m_phaser_feedback = m_value_tree.getRawParameterValue("phaser_feedback");
m_phaser_mod      = m_value_tree.getRawParameterValue("phaser_mod");
m_phaser_drywet   = m_value_tree.getRawParameterValue("phaser_drywet");
//m_phaser_sync     = m_value_tree.getRawParameterValue("phaser_sync");
m_phaser_reset    = m_value_tree.getRawParameterValue("phaser_reset");

m_flanger_rate     = m_value_tree.getRawParameterValue("flanger_rate");
m_flanger_amount   = m_value_tree.getRawParameterValue("flanger_amount");
m_flanger_drywet   = m_value_tree.getRawParameterValue("flanger_drywet");
//m_flanger_sync     = m_value_tree.getRawParameterValue("flanger_sync");
m_flanger_reset    = m_value_tree.getRawParameterValue("flanger_reset");
m_flanger_feedback = m_value_tree.getRawParameterValue("flanger_feedback");

m_chorus_rate     = m_value_tree.getRawParameterValue("chorus_rate");
m_chorus_amount   = m_value_tree.getRawParameterValue("chorus_amount");
m_chorus_drywet   = m_value_tree.getRawParameterValue("chorus_drywet");
//m_chorus_sync     = m_value_tree.getRawParameterValue("chorus_sync");
m_chorus_reset    = m_value_tree.getRawParameterValue("chorus_reset");
m_chorus_feedback = m_value_tree.getRawParameterValue("chorus_feedback");

// for (int i = 0; i < 9; ++i) {
// 	m_amount_1[i] = m_value_tree.getRawParameterValue("amount_1_row_" + std::to_string(i));
// 	m_amount_2[i] = m_value_tree.getRawParameterValue("amount_2_row_" + std::to_string(i));
// 	m_amount_3[i] = m_value_tree.getRawParameterValue("amount_3_row_" + std::to_string(i));
// }

//============================================================================
//============================================================================
//============================================================================
//============================================================================
//======================== ATTACH TO LISTENER ================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================

// attach subtrees 
m_value_tree.state.appendChild(m_value_tree_draw, nullptr);
m_value_tree.state.appendChild(m_value_tree_fx, nullptr);
m_value_tree.state.appendChild(m_value_tree_lfo, nullptr);
m_value_tree.state.appendChild(m_value_tree_misc, nullptr);
m_value_tree.state.appendChild(m_value_tree_mod, nullptr);
m_value_tree.state.appendChild(m_value_tree_osc, nullptr);
m_value_tree.state.appendChild(m_value_tree_midi_learn, nullptr);
// attach non param listeners
m_value_tree_fx.addListener(&m_non_param_listener_fx);
m_value_tree_lfo.addListener(&m_non_param_listener_lfo);
m_value_tree_misc.addListener(&m_non_param_listener_misc);
m_value_tree_mod.addListener(&m_non_param_listener_mod);
m_value_tree_osc.addListener(&m_non_param_listener_osc);

for (int osc = 0; osc < 3; ++osc) {
	//osc pitch listener:
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_oct", &m_tree_listener_osc_pitch);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_semi", &m_tree_listener_osc_pitch);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_fine", &m_tree_listener_osc_pitch);

	//osc misc listener:
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_vol", &m_tree_listener_osc_misc);
	if (osc != 0) {
		m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_sync", &m_tree_listener_osc_misc);
	}
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_reset", &m_tree_listener_osc_misc);

	//osc analog listener
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_pulsewidth", &m_tree_listener_osc_analog);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_drift", &m_tree_listener_osc_analog);

	//osc multi listener
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_position", &m_tree_listener_osc_multi);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_detune", &m_tree_listener_osc_multi);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_pos_mod", &m_tree_listener_osc_multi);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_multi_position", &m_tree_listener_osc_multi);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_spread", &m_tree_listener_osc_multi);

	//osc xy listener
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_vec_x", &m_tree_listener_osc_xy);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_vec_y", &m_tree_listener_osc_xy);

	//osc chip listener
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_chipnoise", &m_tree_listener_osc_chip);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_arp_speed", &m_tree_listener_osc_chip);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_arp_on", &m_tree_listener_osc_chip);

	//osc arp listener
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_step_1", &m_tree_listener_osc_arp);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_step_2", &m_tree_listener_osc_arp);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_step_3", &m_tree_listener_osc_arp);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_step_3_on", &m_tree_listener_osc_arp);

	//osc fm listener
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_fm", &m_tree_listener_osc_fm);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_exp_fm", &m_tree_listener_osc_fm);
	//m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_carrier_ratio", &m_tree_listener_osc_fm);
	//m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_modulator_ratio", &m_tree_listener_osc_fm);

	//osc noiselistener
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_lp", &m_tree_listener_osc_noise);
	m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_hp", &m_tree_listener_osc_noise);
}

for (int fil = 0; fil < 3; ++fil) {
	//fil top listener
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_vel", &m_tree_listener_fil_top);
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_env", &m_tree_listener_fil_top);
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_kbd", &m_tree_listener_fil_top);
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_gain", &m_tree_listener_fil_top);

	//fil center listener
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_freq", &m_tree_listener_fil_center);
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_res", &m_tree_listener_fil_center);
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_saturation", &m_tree_listener_fil_center);

	//fil misc listener
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_ring_mod_amount", &m_tree_listener_fil_misc);
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_sem_transition", &m_tree_listener_fil_misc);
	//m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_comb_polarity", &m_tree_listener_fil_misc);

	//fil formant listener
	m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_formant_transition",
	                                  &m_tree_listener_fil_formant);
	//m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_vowel_left", &m_tree_listener_fil_formant);
	//m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_vowel_right", &m_tree_listener_fil_formant);
}

//amp listener
m_value_tree.addParameterListener("amp_pan", &m_tree_listener_amp);
m_value_tree.addParameterListener("amp_gain", &m_tree_listener_amp);
m_value_tree.addParameterListener("amp_velocity", &m_tree_listener_amp);
m_value_tree.addParameterListener("dist_boost", &m_tree_listener_amp);
m_value_tree.addParameterListener("dist_drywet", &m_tree_listener_amp);

//delay listener
m_value_tree.addParameterListener("delay_time", &m_tree_listener_delay);
m_value_tree.addParameterListener("delay_feedback", &m_tree_listener_delay);
m_value_tree.addParameterListener("delay_hp", &m_tree_listener_delay);
m_value_tree.addParameterListener("delay_ducking", &m_tree_listener_delay);
m_value_tree.addParameterListener("delay_dry", &m_tree_listener_delay);
m_value_tree.addParameterListener("delay_wet", &m_tree_listener_delay);
m_value_tree.addParameterListener("delay_pingpong", &m_tree_listener_delay);
m_value_tree.addParameterListener("delay_on", &m_tree_listener_delay);

//chorus listener
m_value_tree.addParameterListener("chorus_rate", &m_tree_listener_chorus);
m_value_tree.addParameterListener("chorus_amount", &m_tree_listener_chorus);
m_value_tree.addParameterListener("chorus_drywet", &m_tree_listener_chorus);
m_value_tree.addParameterListener("chorus_feedback", &m_tree_listener_chorus);
m_value_tree.addParameterListener("chorus_reset", &m_tree_listener_chorus);
m_value_tree.addParameterListener("chorus_on", &m_tree_listener_chorus);

//phaser listener
m_value_tree.addParameterListener("phaser_rate", &m_tree_listener_phaser);
m_value_tree.addParameterListener("phaser_freq", &m_tree_listener_phaser);
m_value_tree.addParameterListener("phaser_feedback", &m_tree_listener_phaser);
m_value_tree.addParameterListener("phaser_mod", &m_tree_listener_phaser);
m_value_tree.addParameterListener("phaser_drywet", &m_tree_listener_phaser);
m_value_tree.addParameterListener("phaser_reset", &m_tree_listener_phaser);
m_value_tree.addParameterListener("phaser_on", &m_tree_listener_phaser);

//flanger listener
m_value_tree.addParameterListener("flanger_rate", &m_tree_listener_flanger);
m_value_tree.addParameterListener("flanger_amount", &m_tree_listener_flanger);
m_value_tree.addParameterListener("flanger_drywet", &m_tree_listener_flanger);
m_value_tree.addParameterListener("flanger_feedback", &m_tree_listener_flanger);
m_value_tree.addParameterListener("flanger_reset", &m_tree_listener_flanger);
m_value_tree.addParameterListener("flanger_on", &m_tree_listener_flanger);

//adsr1 listener
m_value_tree.addParameterListener("env1_attack", &m_tree_listener_adsr1);
m_value_tree.addParameterListener("env1_decay", &m_tree_listener_adsr1);
m_value_tree.addParameterListener("env1_sustain", &m_tree_listener_adsr1);
m_value_tree.addParameterListener("env1_release", &m_tree_listener_adsr1);
m_value_tree.addParameterListener("env1_loop", &m_tree_listener_adsr1);

//adsr2 listener
m_value_tree.addParameterListener("env2_attack", &m_tree_listener_adsr2);
m_value_tree.addParameterListener("env2_decay", &m_tree_listener_adsr2);
m_value_tree.addParameterListener("env2_sustain", &m_tree_listener_adsr2);
m_value_tree.addParameterListener("env2_release", &m_tree_listener_adsr2);
m_value_tree.addParameterListener("env2_loop", &m_tree_listener_adsr2);

//adsr3 listener
m_value_tree.addParameterListener("env3_attack", &m_tree_listener_adsr3);
m_value_tree.addParameterListener("env3_decay", &m_tree_listener_adsr3);
m_value_tree.addParameterListener("env3_sustain", &m_tree_listener_adsr3);
m_value_tree.addParameterListener("env3_release", &m_tree_listener_adsr3);
m_value_tree.addParameterListener("env3_loop", &m_tree_listener_adsr3);

//adsr4 listener
m_value_tree.addParameterListener("env4_attack", &m_tree_listener_adsr4);
m_value_tree.addParameterListener("env4_decay", &m_tree_listener_adsr4);
m_value_tree.addParameterListener("env4_sustain", &m_tree_listener_adsr4);
m_value_tree.addParameterListener("env4_release", &m_tree_listener_adsr4);
m_value_tree.addParameterListener("env4_loop", &m_tree_listener_adsr4);

//lfo1 listener
m_value_tree.addParameterListener("lfo1_freq", &m_tree_listener_lfo1);
m_value_tree.addParameterListener("lfo1_reset", &m_tree_listener_lfo1);

//lfo2 listener
m_value_tree.addParameterListener("lfo2_freq", &m_tree_listener_lfo2);
m_value_tree.addParameterListener("lfo2_reset", &m_tree_listener_lfo2);

//lfo3 listener
m_value_tree.addParameterListener("lfo3_freq", &m_tree_listener_lfo3);
m_value_tree.addParameterListener("lfo3_reset", &m_tree_listener_lfo3);

//lfo4 listener
m_value_tree.addParameterListener("lfo4_freq", &m_tree_listener_lfo4);
m_value_tree.addParameterListener("lfo4_reset", &m_tree_listener_lfo4);

//general misc
m_value_tree.addParameterListener("glide", &m_tree_listener_general_misc);
m_value_tree.addParameterListener("master", &m_tree_listener_general_misc);

m_value_tree.addParameterListener("unison_detune", &m_tree_listener_general_misc);
m_value_tree.addParameterListener("unison_width", &m_tree_listener_general_misc);

//arp listener
m_value_tree.addParameterListener("step_0_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_1_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_2_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_3_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_4_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_5_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_6_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_7_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_8_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_9_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_10_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_11_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_12_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_13_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_14_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_15_on", &m_tree_listener_arp);

m_value_tree.addParameterListener("step_0_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_1_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_2_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_3_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_4_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_5_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_6_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_7_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_8_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_9_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_10_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_11_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_12_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_13_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_14_mod_1", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_15_mod_1", &m_tree_listener_arp);

m_value_tree.addParameterListener("step_0_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_1_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_2_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_3_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_4_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_5_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_6_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_7_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_8_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_9_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_10_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_11_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_12_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_13_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_14_mod_2", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_15_mod_2", &m_tree_listener_arp);

m_value_tree.addParameterListener("step_0_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_1_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_2_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_3_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_4_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_5_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_6_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_7_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_8_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_9_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_10_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_11_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_12_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_13_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_14_transpose", &m_tree_listener_arp);
m_value_tree.addParameterListener("step_15_transpose", &m_tree_listener_arp);

m_value_tree.addParameterListener("arp_on", &m_tree_listener_arp);
m_value_tree.addParameterListener("arp_one_shot", &m_tree_listener_arp);