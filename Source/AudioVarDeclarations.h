/*
  ==============================================================================

    AudioVarDeclarations.h
    Created: 5 Apr 2019 9:30:01pm
    Author:  frot

  ==============================================================================
*/

#pragma once
//osc1
    float* m_osc_type[3];
    float* m_osc_oct[3];
    float* m_osc_semi[3];
    float* m_osc_fine[3];
    float* m_osc_analog_wave[3];
    float* m_osc_vol[3];
    float* m_osc_position[3];
    float* m_osc_detune[3];
    float* m_osc_multi_position[3];
    float* m_osc_spread[3];
    float* m_osc_pulsewidth[3];
    float* m_osc_drift[3];
    float* m_osc_arp_speed[3];
    float* m_osc_step_1[3];
    float* m_osc_step_2[3];
    float* m_osc_step_3[3];
    float* m_osc_fm[3];
    float* m_osc_carrier_ratio[3];
    float* m_osc_modulator_ratio[3];
    float* m_osc_lp[3];
    float* m_osc_hp[3];
    float* m_osc_reset[3];
    float* m_osc_arp_on[3];
    float* m_osc_step_3_on[3];
    float* m_osc_chipnoise[3];
    float* m_osc_exp_fm[3];
    float* m_osc_vec_a[3];
    float* m_osc_vec_b[3];
    float* m_osc_vec_c[3];
    float* m_osc_vec_d[3];
    float* m_osc_vec_x[3];
    float* m_osc_vec_y[3];
    float* m_osc_wavetable[3];
    
    //filters
    float* m_fil_type[3];
    float* m_fil_vel[3];
    float* m_fil_kbd[3];
    float* m_fil_gain[3];
    float* m_fil_freq[3];
    float* m_fil_res[3];
    float* m_fil_saturation[3];
    float* m_fil_formant_transition[3];
    float* m_fil_sem_transition[3];
    float* m_fil_vowel_left[3];
    float* m_fil_vowel_right[3];
    float* m_fil_comb_polarity[3];

    //midsection
    float* m_amp_pan;
    float* m_amp_gain;
    float* m_amp_vel;
    float* m_dist_threshold;
    float* m_dist_drywet;
    float* m_dist_on;
    float* m_dist_algo;
    float* m_fil1_to_amp;
    float* m_fil2_to_amp;
    //env1
    float* m_env1_attack;
    float* m_env1_decay;
    float* m_env1_sustain;
    float* m_env1_release;
    float* m_env1_loop;
    //env2
    float* m_env2_attack;
    float* m_env2_decay;
    float* m_env2_sustain;
    float* m_env2_release;
    float* m_env2_loop;
    //env3
    float* m_env3_attack;
    float* m_env3_decay;
    float* m_env3_sustain;
    float* m_env3_release;
    float* m_env3_loop;
    //env4
    float* m_env4_attack;
    float* m_env4_decay;
    float* m_env4_sustain;
    float* m_env4_release;
    float* m_env4_loop;
    //lfo1
    float* m_lfo1_freq;
    float* m_lfo1_reset;
    float* m_lfo1_sync;
    float* m_lfo1_wave;
    float* m_lfo1_synctime_numerator;
    float* m_lfo1_synctime_denominator;
    //lfo2
    float* m_lfo2_freq;
    float* m_lfo2_reset;
    float* m_lfo2_sync;
    float* m_lfo2_wave;
    float* m_lfo2_synctime_numerator;
    float* m_lfo2_synctime_denominator;
    //lfo3
    float* m_lfo3_freq;
    float* m_lfo3_reset;
    float* m_lfo3_sync;
    float* m_lfo3_wave;
    float* m_lfo3_synctime_numerator;
    float* m_lfo3_synctime_denominator;
    //lfo4
    float* m_lfo4_freq;
    float* m_lfo4_reset;
    float* m_lfo4_sync;
    float* m_lfo4_wave;
    float* m_lfo4_synctime_numerator;
    float* m_lfo4_synctime_denominator;
    //main
    float* m_glide;
    float* m_master;
    float* m_modwheel;
    float* m_pitchbend;
    float* m_pitchbend_amount;
    float* m_xy_x;
    float* m_xy_y;
    float* m_delay_on;
    float* m_phaser_on;
    float* m_flanger_on;
    float* m_chorus_on;
    float* m_fil_osc1[2];
    float* m_fil_osc2[2];
    float* m_fil_osc3[2];

    float* m_fil2_fil1;
    float* m_delay_position;
    float* m_flanger_position;
    float* m_phaser_position;
    float* m_chorus_position;
    //delay
    float* m_delay_time;
    float* m_delay_feedback;
    float* m_delay_hp;
    float* m_delay_ducking;
    float* m_delay_dry;
    float* m_delay_wet;
    float* m_delay_sync;
    float* m_delay_synctime_numerator;
    float* m_delay_synctime_denominator;
    //phaser
    float* m_phaser_reset;
    float* m_phaser_sync;
    float* m_phaser_frequency;
    float* m_phaser_amount;
    float* m_phaser_drywet;
    float* m_phaser_synctime_numerator;
    float* m_phaser_synctime_denominator;
    //flanger
    float* m_flanger_reset;
    float* m_flanger_sync;
    float* m_flanger_frequency;
    float* m_flanger_amount;
    float* m_flanger_drywet;
    float* m_flanger_synctime_numerator;
    float* m_flanger_synctime_denominator;
    //chorus
    float* m_chorus_reset;
    float* m_chorus_sync;
    float* m_chorus_frequency;
    float* m_chorus_amount;
    float* m_chorus_drywet;
    float* m_chorus_synctime_numerator;
    float* m_chorus_synctime_denominator;
    //modmatrix
    float* m_amount_1[9];
    float* m_amount_2[9];
    float* m_amount_3[9];
    float* m_source[9];
    float* m_dest_1[9];
    float* m_dest_2[9];
    float* m_scale[9];