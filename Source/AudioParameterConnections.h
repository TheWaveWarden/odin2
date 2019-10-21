/*
  ==============================================================================

    AudioParameterConnections.h
    Created: 5 Apr 2019 10:15:10pm
    Author:  frot

  ==============================================================================
*/

#pragma once
for (int osc = 0; osc < 3; ++osc) {
  m_osc_oct[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_oct");
  m_osc_semi[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_semi");
  m_osc_fine[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_fine");
  m_osc_vol[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_vol");
  m_osc_position[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_position");
  m_osc_detune[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_detune");
  m_osc_multi_position[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_multi_position");
  m_osc_spread[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_spread");
  m_osc_pulsewidth[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_pulsewidth");
  m_osc_drift[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_drift");
  m_osc_arp_speed[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_arp_speed");
  m_osc_step_1[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_step_1");
  m_osc_step_2[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_step_2");
  m_osc_step_3[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_step_3");
  m_osc_fm[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_fm");
  m_osc_carrier_ratio[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_carrier_ratio");
  m_osc_modulator_ratio[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_modulator_ratio");
  m_osc_lp[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_lp");
  m_osc_hp[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_hp");
  m_osc_reset[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_reset");
  if (osc != 0) {
    m_osc_sync[osc] = m_value_tree.getRawParameterValue(
        "osc" + std::to_string(osc + 1) + "_sync");
  }
  m_osc_arp_on[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_arp_on");
  m_osc_step_3_on[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_step_3_on");
  m_osc_chipnoise[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_chipnoise");
  m_osc_exp_fm[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_exp_fm");
  m_osc_vec_x[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_vec_x");
  m_osc_vec_y[osc] = m_value_tree.getRawParameterValue(
      "osc" + std::to_string(osc + 1) + "_vec_y");
}

for (int fil = 0; fil < 3; ++fil) {
  m_fil_vel[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_vel");
  m_fil_env[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_env");
  m_fil_kbd[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_kbd");
  m_fil_gain[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_gain");
  m_fil_freq[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_freq");
  m_fil_res[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_res");
  m_fil_saturation[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_saturation");
  m_fil_formant_transition[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_formant_transition");
  m_fil_ring_mod_amount[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_ring_mod_amount");
  m_fil_sem_transition[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_sem_transition");
  m_fil_vowel_left[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_vowel_left");
  m_fil_vowel_right[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_vowel_right");
  m_fil_comb_polarity[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_comb_polarity");
}

m_amp_pan = m_value_tree.getRawParameterValue("amp_pan");
m_amp_gain = m_value_tree.getRawParameterValue("amp_gain");
m_amp_vel = m_value_tree.getRawParameterValue("amp_vel");
m_dist_threshold = m_value_tree.getRawParameterValue("dist_boost");
m_dist_drywet = m_value_tree.getRawParameterValue("dist_drywet");
m_dist_on = m_value_tree.getRawParameterValue("dist_on");
m_fil1_to_amp = m_value_tree.getRawParameterValue("fil1_to_amp");
m_fil2_to_amp = m_value_tree.getRawParameterValue("fil2_to_amp");

m_env1_attack = m_value_tree.getRawParameterValue("env1_attack");
m_env1_decay = m_value_tree.getRawParameterValue("env1_decay");
m_env1_sustain = m_value_tree.getRawParameterValue("env1_sustain");
m_env1_release = m_value_tree.getRawParameterValue("env1_release");
m_env1_loop = m_value_tree.getRawParameterValue("env1_loop");

m_env2_attack = m_value_tree.getRawParameterValue("env2_attack");
m_env2_decay = m_value_tree.getRawParameterValue("env2_decay");
m_env2_sustain = m_value_tree.getRawParameterValue("env2_sustain");
m_env2_release = m_value_tree.getRawParameterValue("env2_release");
m_env2_loop = m_value_tree.getRawParameterValue("env2_loop");

m_env3_attack = m_value_tree.getRawParameterValue("env3_attack");
m_env3_decay = m_value_tree.getRawParameterValue("env3_decay");
m_env3_sustain = m_value_tree.getRawParameterValue("env3_sustain");
m_env3_release = m_value_tree.getRawParameterValue("env3_release");
m_env3_loop = m_value_tree.getRawParameterValue("env3_loop");

m_env4_attack = m_value_tree.getRawParameterValue("env4_attack");
m_env4_decay = m_value_tree.getRawParameterValue("env4_decay");
m_env4_sustain = m_value_tree.getRawParameterValue("env4_sustain");
m_env4_release = m_value_tree.getRawParameterValue("env4_release");
m_env4_loop = m_value_tree.getRawParameterValue("env4_loop");

m_lfo1_freq = m_value_tree.getRawParameterValue("lfo1_freq");
m_lfo1_reset = m_value_tree.getRawParameterValue("lfo1_reset");
m_lfo1_sync = m_value_tree.getRawParameterValue("lfo1_sync");
m_lfo1_wave = m_value_tree.getRawParameterValue("lfo1_wave");

m_lfo2_freq = m_value_tree.getRawParameterValue("lfo2_freq");
m_lfo2_reset = m_value_tree.getRawParameterValue("lfo2_reset");
m_lfo2_sync = m_value_tree.getRawParameterValue("lfo2_sync");
m_lfo2_wave = m_value_tree.getRawParameterValue("lfo2_wave");

m_lfo3_freq = m_value_tree.getRawParameterValue("lfo3_freq");
m_lfo3_reset = m_value_tree.getRawParameterValue("lfo3_reset");
m_lfo3_sync = m_value_tree.getRawParameterValue("lfo3_sync");
m_lfo3_wave = m_value_tree.getRawParameterValue("lfo3_wave");

m_lfo4_freq = m_value_tree.getRawParameterValue("lfo4_freq");
m_lfo4_reset = m_value_tree.getRawParameterValue("lfo4_reset");
m_lfo4_sync = m_value_tree.getRawParameterValue("lfo4_sync");
m_lfo4_wave = m_value_tree.getRawParameterValue("lfo4_wave");

m_phaser_on = m_value_tree.getRawParameterValue("phaser_on");
m_flanger_on = m_value_tree.getRawParameterValue("flanger_on");
m_delay_on = m_value_tree.getRawParameterValue("delay_on");
m_chorus_on = m_value_tree.getRawParameterValue("chorus_on");

for (int fil = 0; fil < 2; ++fil) {
  m_fil_osc1[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_osc1");
  m_fil_osc2[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_osc2");
  m_fil_osc3[fil] = m_value_tree.getRawParameterValue(
      "fil" + std::to_string(fil + 1) + "_osc3");
}

m_fil2_fil1 = m_value_tree.getRawParameterValue("fil2_fil1");
m_xy_x = m_value_tree.getRawParameterValue("xy_x");
m_xy_y = m_value_tree.getRawParameterValue("xy_y");
m_glide = m_value_tree.getRawParameterValue("glide");
m_master = m_value_tree.getRawParameterValue("master");
m_modwheel = m_value_tree.getRawParameterValue("modwheel");
m_pitchbend = m_value_tree.getRawParameterValue("pitchbend");
m_pitchbend_amount = m_value_tree.getRawParameterValue("pitchbend_amount");

m_delay_time = m_value_tree.getRawParameterValue("delay_time");
m_delay_feedback = m_value_tree.getRawParameterValue("delay_feedback");
m_delay_hp = m_value_tree.getRawParameterValue("delay_hp");
m_delay_ducking = m_value_tree.getRawParameterValue("delay_ducking");
m_delay_dry = m_value_tree.getRawParameterValue("delay_dry");
m_delay_wet = m_value_tree.getRawParameterValue("delay_wet");
m_delay_sync = m_value_tree.getRawParameterValue("delay_sync");
m_delay_pingpong = m_value_tree.getRawParameterValue("delay_pingpong");

m_phaser_rate = m_value_tree.getRawParameterValue("phaser_rate");
m_phaser_freq = m_value_tree.getRawParameterValue("phaser_freq");
m_phaser_feedback = m_value_tree.getRawParameterValue("phaser_feedback");
m_phaser_mod = m_value_tree.getRawParameterValue("phaser_mod");
m_phaser_drywet = m_value_tree.getRawParameterValue("phaser_drywet");
m_phaser_sync = m_value_tree.getRawParameterValue("phaser_sync");
m_phaser_reset = m_value_tree.getRawParameterValue("phaser_reset");

m_flanger_rate = m_value_tree.getRawParameterValue("flanger_rate");
m_flanger_amount = m_value_tree.getRawParameterValue("flanger_amount");
m_flanger_drywet = m_value_tree.getRawParameterValue("flanger_drywet");
m_flanger_sync = m_value_tree.getRawParameterValue("flanger_sync");
m_flanger_reset = m_value_tree.getRawParameterValue("flanger_reset");
m_flanger_feedback = m_value_tree.getRawParameterValue("flanger_feedback");

m_chorus_rate = m_value_tree.getRawParameterValue("chorus_rate");
m_chorus_amount = m_value_tree.getRawParameterValue("chorus_amount");
m_chorus_drywet = m_value_tree.getRawParameterValue("chorus_drywet");
m_chorus_sync = m_value_tree.getRawParameterValue("chorus_sync");
m_chorus_reset = m_value_tree.getRawParameterValue("chorus_reset");
m_chorus_feedback = m_value_tree.getRawParameterValue("chorus_feedback");

for (int i = 0; i < 9; ++i) {
  m_amount_1[i] =
      m_value_tree.getRawParameterValue("amount_1_row_" + std::to_string(i));
  m_amount_2[i] =
      m_value_tree.getRawParameterValue("amount_2_row_" + std::to_string(i));
  m_amount_3[i] =
      m_value_tree.getRawParameterValue("amount_3_row_" + std::to_string(i));
}

//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//======================== ATTACH TO LISTENER ================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================

// first attach non param listener
m_value_tree.state.addListener(&m_non_param_listener);


//osc base listener:
for (int osc = 0; osc < 3; ++osc) {
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_oct",
                                    &m_tree_listener_osc_base);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_semi",
                                    &m_tree_listener_osc_base);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_fine",
                                    &m_tree_listener_osc_base);




                                    
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_vol",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_position", &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_detune",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_multi_position", &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_spread",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_pulsewidth", &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_drift",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_arp_speed", &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_step_1",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_step_2",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_step_3",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_fm",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_carrier_ratio", &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_modulator_ratio", &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_lp",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_hp",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_reset",
                                    &m_tree_listener);
  if (osc != 0) {
    m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_sync",
                                      &m_tree_listener);
  }
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_arp_on",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_step_3_on", &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_chipnoise", &m_tree_listener);
  m_value_tree.addParameterListener(
      "osc" + std::to_string(osc + 1) + "_chipwave", &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_exp_fm",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_vec_x",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("osc" + std::to_string(osc + 1) + "_vec_y",
                                    &m_tree_listener);
}
for (int fil = 0; fil < 3; ++fil) {
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_vel",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_env",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_kbd",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_gain",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_freq",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_res",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "fil" + std::to_string(fil + 1) + "_saturation", &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) +
                                        "_formant_transition",
                                    &m_tree_listener);
  m_value_tree.addParameterListener(
      "fil" + std::to_string(fil + 1) + "_ring_mod_amount", &m_tree_listener);
  m_value_tree.addParameterListener(
      "fil" + std::to_string(fil + 1) + "_sem_transition", &m_tree_listener);
  m_value_tree.addParameterListener(
      "fil" + std::to_string(fil + 1) + "_vowel_left", &m_tree_listener);
  m_value_tree.addParameterListener(
      "fil" + std::to_string(fil + 1) + "_vowel_right", &m_tree_listener);
  m_value_tree.addParameterListener(
      "fil" + std::to_string(fil + 1) + "_comb_polarity", &m_tree_listener);
}

m_value_tree.addParameterListener("amp_pan", &m_tree_listener);
m_value_tree.addParameterListener("amp_gain", &m_tree_listener);
m_value_tree.addParameterListener("amp_vel", &m_tree_listener);
m_value_tree.addParameterListener("dist_boost", &m_tree_listener);
m_value_tree.addParameterListener("dist_drywet", &m_tree_listener);
m_value_tree.addParameterListener("dist_on", &m_tree_listener);
m_value_tree.addParameterListener("fil1_to_amp", &m_tree_listener);
m_value_tree.addParameterListener("fil2_to_amp", &m_tree_listener);

m_value_tree.addParameterListener("env1_attack", &m_tree_listener);
m_value_tree.addParameterListener("env1_decay", &m_tree_listener);
m_value_tree.addParameterListener("env1_sustain", &m_tree_listener);
m_value_tree.addParameterListener("env1_release", &m_tree_listener);
m_value_tree.addParameterListener("env1_loop", &m_tree_listener);

m_value_tree.addParameterListener("env2_attack", &m_tree_listener);
m_value_tree.addParameterListener("env2_decay", &m_tree_listener);
m_value_tree.addParameterListener("env2_sustain", &m_tree_listener);
m_value_tree.addParameterListener("env2_release", &m_tree_listener);
m_value_tree.addParameterListener("env2_loop", &m_tree_listener);

m_value_tree.addParameterListener("env3_attack", &m_tree_listener);
m_value_tree.addParameterListener("env3_decay", &m_tree_listener);
m_value_tree.addParameterListener("env3_sustain", &m_tree_listener);
m_value_tree.addParameterListener("env3_release", &m_tree_listener);
m_value_tree.addParameterListener("env3_loop", &m_tree_listener);

m_value_tree.addParameterListener("env4_attack", &m_tree_listener);
m_value_tree.addParameterListener("env4_decay", &m_tree_listener);
m_value_tree.addParameterListener("env4_sustain", &m_tree_listener);
m_value_tree.addParameterListener("env4_release", &m_tree_listener);
m_value_tree.addParameterListener("env4_loop", &m_tree_listener);

m_value_tree.addParameterListener("lfo1_freq", &m_tree_listener);
m_value_tree.addParameterListener("lfo1_reset", &m_tree_listener);
m_value_tree.addParameterListener("lfo1_sync", &m_tree_listener);
m_value_tree.addParameterListener("lfo1_wave", &m_tree_listener);

m_value_tree.addParameterListener("lfo2_freq", &m_tree_listener);
m_value_tree.addParameterListener("lfo2_reset", &m_tree_listener);
m_value_tree.addParameterListener("lfo2_sync", &m_tree_listener);
m_value_tree.addParameterListener("lfo2_wave", &m_tree_listener);

m_value_tree.addParameterListener("lfo3_freq", &m_tree_listener);
m_value_tree.addParameterListener("lfo3_reset", &m_tree_listener);
m_value_tree.addParameterListener("lfo3_sync", &m_tree_listener);
m_value_tree.addParameterListener("lfo3_wave", &m_tree_listener);

m_value_tree.addParameterListener("lfo4_freq", &m_tree_listener);
m_value_tree.addParameterListener("lfo4_reset", &m_tree_listener);
m_value_tree.addParameterListener("lfo4_sync", &m_tree_listener);
m_value_tree.addParameterListener("lfo4_wave", &m_tree_listener);

m_value_tree.addParameterListener("phaser_on", &m_tree_listener);
m_value_tree.addParameterListener("flanger_on", &m_tree_listener);
m_value_tree.addParameterListener("delay_on", &m_tree_listener);
m_value_tree.addParameterListener("chorus_on", &m_tree_listener);

for (int fil = 0; fil < 2; ++fil) {
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_osc1",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_osc2",
                                    &m_tree_listener);
  m_value_tree.addParameterListener("fil" + std::to_string(fil + 1) + "_osc3",
                                    &m_tree_listener);
}

m_value_tree.addParameterListener("fil2_fil1", &m_tree_listener);
m_value_tree.addParameterListener("xy_x", &m_tree_listener);
m_value_tree.addParameterListener("xy_y", &m_tree_listener);
m_value_tree.addParameterListener("glide", &m_tree_listener);
m_value_tree.addParameterListener("master", &m_tree_listener);
m_value_tree.addParameterListener("modwheel", &m_tree_listener);
m_value_tree.addParameterListener("pitchbend", &m_tree_listener);
m_value_tree.addParameterListener("pitchbend_amount", &m_tree_listener);

m_value_tree.addParameterListener("delay_time", &m_tree_listener);
m_value_tree.addParameterListener("delay_feedback", &m_tree_listener);
m_value_tree.addParameterListener("delay_hp", &m_tree_listener);
m_value_tree.addParameterListener("delay_ducking", &m_tree_listener);
m_value_tree.addParameterListener("delay_dry", &m_tree_listener);
m_value_tree.addParameterListener("delay_wet", &m_tree_listener);
m_value_tree.addParameterListener("delay_sync", &m_tree_listener);
m_value_tree.addParameterListener("delay_pingpong", &m_tree_listener);

m_value_tree.addParameterListener("phaser_rate", &m_tree_listener);
m_value_tree.addParameterListener("phaser_freq", &m_tree_listener);
m_value_tree.addParameterListener("phaser_feedback", &m_tree_listener);
m_value_tree.addParameterListener("phaser_mod", &m_tree_listener);
m_value_tree.addParameterListener("phaser_drywet", &m_tree_listener);
m_value_tree.addParameterListener("phaser_sync", &m_tree_listener);
m_value_tree.addParameterListener("phaser_reset", &m_tree_listener);

m_value_tree.addParameterListener("flanger_rate", &m_tree_listener);
m_value_tree.addParameterListener("flanger_amount", &m_tree_listener);
m_value_tree.addParameterListener("flanger_drywet", &m_tree_listener);
m_value_tree.addParameterListener("flanger_feedback", &m_tree_listener);
m_value_tree.addParameterListener("flanger_sync", &m_tree_listener);
m_value_tree.addParameterListener("flanger_reset", &m_tree_listener);

m_value_tree.addParameterListener("chorus_rate", &m_tree_listener);
m_value_tree.addParameterListener("chorus_amount", &m_tree_listener);
m_value_tree.addParameterListener("chorus_drywet", &m_tree_listener);
m_value_tree.addParameterListener("chorus_feedback", &m_tree_listener);
m_value_tree.addParameterListener("chorus_sync", &m_tree_listener);
m_value_tree.addParameterListener("chorus_reset", &m_tree_listener);

for (int i = 0; i < 9; ++i) {
  m_value_tree.addParameterListener("amount_1_row_" + std::to_string(i) ,
                                    &m_tree_listener);
  m_value_tree.addParameterListener("amount_2_row_" + std::to_string(i),
                                    &m_tree_listener);
  m_value_tree.addParameterListener("amount_3_row_" + std::to_string(i),
                                    &m_tree_listener);
}