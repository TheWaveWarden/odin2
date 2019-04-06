/*
  ==============================================================================

    AudioParameterConnections.h
    Created: 5 Apr 2019 10:15:10pm
    Author:  frot

  ==============================================================================
*/

#pragma once
  for(int osc = 0; osc < 3; ++osc){
    m_osc_type[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_type");
    m_osc_oct[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_oct");
    m_osc_semi[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_semi");
    m_osc_fine[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_fine");
    m_osc_vol[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_vol");
    m_osc_position[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_position");
    m_osc_detune[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_detune");
    m_osc_multi_position[osc] =
        m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_multi_position");
    m_osc_spread[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_spread");
    m_osc_pulsewidth[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_pulsewidth");
    m_osc_drift[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_drift");
    m_osc_arp_speed[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_arp_speed");
    m_osc_step_1[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_step_1");
    m_osc_step_2[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_step_2");
    m_osc_step_3[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_step_3");
    m_osc_fm[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_fm");
    m_osc_carrier_ratio[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_carrier_ratio");
    m_osc_modulator_ratio[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_modulator_ratio");
    m_osc_lp[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_lp");
    m_osc_hp[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_hp");
    m_osc_reset[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_reset");
    m_osc_arp_on[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_arp_on");
    m_osc_step_3_on[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_step_3_on");
    m_osc_chipnoise[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_chipnoise");
    m_osc_exp_fm[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_exp_fm");
    m_osc_vec_a[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_vec_a");
    m_osc_vec_b[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_vec_b");
    m_osc_vec_c[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_vec_c");
    m_osc_vec_d[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_vec_d");
    m_osc_vec_x[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_vec_x");
    m_osc_vec_y[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_vec_y");
    m_osc_wavetable[osc] = m_parameters.getRawParameterValue("osc" + std::to_string(osc+1) + "_wavetable");
  }
  

  m_fil1_type = m_parameters.getRawParameterValue("fil1_type");
  m_fil1_vel = m_parameters.getRawParameterValue("fil1_vel");
  m_fil1_kbd = m_parameters.getRawParameterValue("fil1_kbd");
  m_fil1_gain = m_parameters.getRawParameterValue("fil1_gain");
  m_fil1_freq = m_parameters.getRawParameterValue("fil1_freq");
  m_fil1_res = m_parameters.getRawParameterValue("fil1_res");
  m_fil1_saturation = m_parameters.getRawParameterValue("fil1_saturation");
  m_fil1_formant_transition =
      m_parameters.getRawParameterValue("fil1_formant_transition");
  m_fil1_sem_transition =
      m_parameters.getRawParameterValue("fil1_sem_transition");
  m_fil1_vowel_left = m_parameters.getRawParameterValue("fil1_vowel_left");
  m_fil1_vowel_right = m_parameters.getRawParameterValue("fil1_vowel_right");
  m_fil1_comb_polarity = m_parameters.getRawParameterValue("fil1_comb_polarity");

  m_fil2_type = m_parameters.getRawParameterValue("fil2_type");
  m_fil2_vel = m_parameters.getRawParameterValue("fil2_vel");
  m_fil2_kbd = m_parameters.getRawParameterValue("fil2_kbd");
  m_fil2_gain = m_parameters.getRawParameterValue("fil2_gain");
  m_fil2_freq = m_parameters.getRawParameterValue("fil2_freq");
  m_fil2_res = m_parameters.getRawParameterValue("fil2_res");
  m_fil2_saturation = m_parameters.getRawParameterValue("fil2_saturation");
  m_fil2_formant_transition =
      m_parameters.getRawParameterValue("fil2_formant_transition");
  m_fil2_sem_transition =
      m_parameters.getRawParameterValue("fil2_sem_transition");
  m_fil2_vowel_left = m_parameters.getRawParameterValue("fil2_vowel_left");
  m_fil2_vowel_right = m_parameters.getRawParameterValue("fil2_vowel_right");
  m_fil2_comb_polarity = m_parameters.getRawParameterValue("fil2_comb_polarity");

  m_fil3_type = m_parameters.getRawParameterValue("fil3_type");
  m_fil3_vel = m_parameters.getRawParameterValue("fil3_vel");
  m_fil3_kbd = m_parameters.getRawParameterValue("fil3_kbd");
  m_fil3_gain = m_parameters.getRawParameterValue("fil3_gain");
  m_fil3_freq = m_parameters.getRawParameterValue("fil3_freq");
  m_fil3_res = m_parameters.getRawParameterValue("fil3_res");
  m_fil3_saturation = m_parameters.getRawParameterValue("fil3_saturation");
  m_fil3_formant_transition =
      m_parameters.getRawParameterValue("fil3_formant_transition");
  m_fil3_sem_transition =
      m_parameters.getRawParameterValue("fil3_sem_transition");
  m_fil3_vowel_left = m_parameters.getRawParameterValue("fil3_vowel_left");
  m_fil3_vowel_right = m_parameters.getRawParameterValue("fil3_vowel_right");
  m_fil3_comb_polarity = m_parameters.getRawParameterValue("fil3_comb_polarity");

  m_amp_pan = m_parameters.getRawParameterValue("amp_pan");
  m_amp_gain = m_parameters.getRawParameterValue("amp_gain");
  m_amp_vel = m_parameters.getRawParameterValue("amp_vel");
  m_dist_threshold = m_parameters.getRawParameterValue("dist_threshold");
  m_dist_drywet = m_parameters.getRawParameterValue("dist_drywet");
  m_dist_on = m_parameters.getRawParameterValue("dist_on");
  m_dist_algo = m_parameters.getRawParameterValue("dist_algo");
  m_fil1_to_amp = m_parameters.getRawParameterValue("fil1_to_amp");
  m_fil2_to_amp = m_parameters.getRawParameterValue("fil2_to_amp");

  m_env1_attack = m_parameters.getRawParameterValue("env1_attack");
  m_env1_decay = m_parameters.getRawParameterValue("env1_decay");
  m_env1_sustain = m_parameters.getRawParameterValue("env1_sustain");
  m_env1_release = m_parameters.getRawParameterValue("env1_release");
  m_env1_loop = m_parameters.getRawParameterValue("env1_loop");

  m_env2_attack = m_parameters.getRawParameterValue("env2_attack");
  m_env2_decay = m_parameters.getRawParameterValue("env2_decay");
  m_env2_sustain = m_parameters.getRawParameterValue("env2_sustain");
  m_env2_release = m_parameters.getRawParameterValue("env2_release");
  m_env2_loop = m_parameters.getRawParameterValue("env2_loop");

  m_env3_attack = m_parameters.getRawParameterValue("env3_attack");
  m_env3_decay = m_parameters.getRawParameterValue("env3_decay");
  m_env3_sustain = m_parameters.getRawParameterValue("env3_sustain");
  m_env3_release = m_parameters.getRawParameterValue("env3_release");
  m_env3_loop = m_parameters.getRawParameterValue("env3_loop");

  m_env4_attack = m_parameters.getRawParameterValue("env4_attack");
  m_env4_decay = m_parameters.getRawParameterValue("env4_decay");
  m_env4_sustain = m_parameters.getRawParameterValue("env4_sustain");
  m_env4_release = m_parameters.getRawParameterValue("env4_release");
  m_env4_loop = m_parameters.getRawParameterValue("env4_loop");

  m_lfo1_freq = m_parameters.getRawParameterValue("lfo1_freq");
  m_lfo1_reset = m_parameters.getRawParameterValue("lfo1_reset");
  m_lfo1_sync = m_parameters.getRawParameterValue("lfo1_sync");
  m_lfo1_wave = m_parameters.getRawParameterValue("lfo1_wave");
  m_lfo1_synctime_numerator = m_parameters.getRawParameterValue("lfo1_synctime_numerator");
  m_lfo1_synctime_denominator = m_parameters.getRawParameterValue("lfo1_synctime_denominator");

  m_lfo2_freq = m_parameters.getRawParameterValue("lfo2_freq");
  m_lfo2_reset = m_parameters.getRawParameterValue("lfo2_reset");
  m_lfo2_sync = m_parameters.getRawParameterValue("lfo2_sync");
  m_lfo2_wave = m_parameters.getRawParameterValue("lfo2_wave");
  m_lfo2_synctime_numerator = m_parameters.getRawParameterValue("lfo2_synctime_numerator");
  m_lfo2_synctime_denominator = m_parameters.getRawParameterValue("lfo2_synctime_denominator");

  m_lfo3_freq = m_parameters.getRawParameterValue("lfo3_freq");
  m_lfo3_reset = m_parameters.getRawParameterValue("lfo3_reset");
  m_lfo3_sync = m_parameters.getRawParameterValue("lfo3_sync");
  m_lfo3_wave = m_parameters.getRawParameterValue("lfo3_wave");
  m_lfo3_synctime_numerator = m_parameters.getRawParameterValue("lfo3_synctime_numerator");
  m_lfo3_synctime_denominator = m_parameters.getRawParameterValue("lfo3_synctime_denominator");

  m_lfo4_freq = m_parameters.getRawParameterValue("lfo4_freq");
  m_lfo4_reset = m_parameters.getRawParameterValue("lfo4_reset");
  m_lfo4_sync = m_parameters.getRawParameterValue("lfo4_sync");
  m_lfo4_wave = m_parameters.getRawParameterValue("lfo4_wave");
  m_lfo4_synctime_numerator = m_parameters.getRawParameterValue("lfo4_synctime_numerator");
  m_lfo4_synctime_denominator = m_parameters.getRawParameterValue("lfo4_synctime_denominator");
  
  m_phaser_on = m_parameters.getRawParameterValue("phaser_on");
  m_flanger_on = m_parameters.getRawParameterValue("flanger_on");
  m_delay_on = m_parameters.getRawParameterValue("delay_on");
  m_chorus_on = m_parameters.getRawParameterValue("chorus_on");

  m_fil1_osc1 = m_parameters.getRawParameterValue("fil1_osc1");
  m_fil1_osc2 = m_parameters.getRawParameterValue("fil1_osc2");
  m_fil1_osc3 = m_parameters.getRawParameterValue("fil1_osc3");
  m_fil2_osc1 = m_parameters.getRawParameterValue("fil2_osc1");
  m_fil2_osc2 = m_parameters.getRawParameterValue("fil2_osc2");
  m_fil2_osc3 = m_parameters.getRawParameterValue("fil2_osc3");
  m_fil2_fil1 = m_parameters.getRawParameterValue("fil2_fil1");
  m_xy_x = m_parameters.getRawParameterValue("xy_x");
  m_xy_y = m_parameters.getRawParameterValue("xy_y");
  m_glide = m_parameters.getRawParameterValue("glide");
  m_master = m_parameters.getRawParameterValue("master");
  m_modwheel = m_parameters.getRawParameterValue("modwheel");
  m_pitchbend = m_parameters.getRawParameterValue("pitchbend");
  m_pitchbend_amount = m_parameters.getRawParameterValue("pitchbend_amount");

  m_delay_time = m_parameters.getRawParameterValue("delay_time");
  m_delay_feedback = m_parameters.getRawParameterValue("delay_feedback");
  m_delay_hp = m_parameters.getRawParameterValue("delay_hp");
  m_delay_ducking = m_parameters.getRawParameterValue("delay_ducking");
  m_delay_dry = m_parameters.getRawParameterValue("delay_dry");
  m_delay_wet = m_parameters.getRawParameterValue("delay_wet");
  m_delay_sync = m_parameters.getRawParameterValue("delay_sync");
  m_delay_synctime_numerator = m_parameters.getRawParameterValue("delay_synctime_numerator");
  m_delay_synctime_denominator = m_parameters.getRawParameterValue("delay_synctime_denominator");
  

  m_phaser_frequency = m_parameters.getRawParameterValue("phaser_frequency");
  m_phaser_amount = m_parameters.getRawParameterValue("phaser_amount");
  m_phaser_drywet = m_parameters.getRawParameterValue("phaser_drywet");
  m_phaser_sync = m_parameters.getRawParameterValue("phaser_sync");
  m_phaser_reset = m_parameters.getRawParameterValue("phaser_reset");
  m_phaser_synctime_numerator = m_parameters.getRawParameterValue("phaser_synctime_numerator");
  m_phaser_synctime_denominator = m_parameters.getRawParameterValue("phaser_synctime_denominator");

  m_flanger_frequency = m_parameters.getRawParameterValue("flanger_frequency");
  m_flanger_amount = m_parameters.getRawParameterValue("flanger_amount");
  m_flanger_drywet = m_parameters.getRawParameterValue("flanger_drywet");
  m_flanger_sync = m_parameters.getRawParameterValue("flanger_sync");
  m_flanger_reset = m_parameters.getRawParameterValue("flanger_reset");
  m_flanger_synctime_numerator = m_parameters.getRawParameterValue("flanger_synctime_numerator");
  m_flanger_synctime_denominator = m_parameters.getRawParameterValue("flanger_synctime_denominator");

  m_chorus_frequency = m_parameters.getRawParameterValue("chorus_frequency");
  m_chorus_amount = m_parameters.getRawParameterValue("chorus_amount");
  m_chorus_drywet = m_parameters.getRawParameterValue("chorus_drywet");
  m_chorus_sync = m_parameters.getRawParameterValue("chorus_sync");
  m_chorus_reset = m_parameters.getRawParameterValue("chorus_reset");
  m_chorus_synctime_numerator = m_parameters.getRawParameterValue("chorus_synctime_numerator");
  m_chorus_synctime_denominator = m_parameters.getRawParameterValue("chorus_synctime_denominator");

  m_delay_position = m_parameters.getRawParameterValue("delay_position");
  m_flanger_position = m_parameters.getRawParameterValue("flanger_position");
  m_phaser_position = m_parameters.getRawParameterValue("phaser_position");
  m_chorus_position = m_parameters.getRawParameterValue("chorus_position");
  
  for(int i = 0; i < 9; ++i){
    m_amount_1[i] = m_parameters.getRawParameterValue("amount_1_[" + std::to_string(i) + "]");
    m_amount_2[i] = m_parameters.getRawParameterValue("amount_2_[" + std::to_string(i) + "]");
    m_amount_3[i] = m_parameters.getRawParameterValue("amount_3_[" + std::to_string(i) + "]");
    m_source[i] = m_parameters.getRawParameterValue("source_[" + std::to_string(i) + "]");
    m_dest_1[i] = m_parameters.getRawParameterValue("dest_1_[" + std::to_string(i) + "]");
    m_dest_2[i] = m_parameters.getRawParameterValue("dest_2_[" + std::to_string(i) + "]");
    m_scale[i] = m_parameters.getRawParameterValue("scale_[" + std::to_string(i) + "]");
  }