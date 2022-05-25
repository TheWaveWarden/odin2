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

{
	// Osc 1
	std::make_unique<AudioParameterInt>("osc1_oct", "Osc1 Octave", -4, 4, 0),
	    std::make_unique<AudioParameterInt>("osc1_semi", "Osc1 Semitones", -7, 7, 0),
	    std::make_unique<AudioParameterFloat>("osc1_fine", "Osc1 Finetune", NormalisableRange<float>(-50.0f, 50.0f), 0),
	    std::make_unique<AudioParameterFloat>("osc1_vol", "Osc1 Volume", NormalisableRange<float>(-60.0f, 12.0f), 0),
	    std::make_unique<AudioParameterInt>("osc1_reset", "Osc1 Reset", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc1_position", "Osc1 Wavetble Position", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>("osc1_detune", "Osc1 Detune", NormalisableRange<float>(0.f, 1.f), 0.2),
	    std::make_unique<AudioParameterFloat>(
	        "osc1_pos_mod", "Osc1 Position Modulation", NormalisableRange<float>(-1.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc1_multi_position", "Osc1 Multi Position", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc1_spread", "Osc1 Wavetable Spread", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc1_pulsewidth", "Osc1 PulseWidth", NormalisableRange<float>(0.f, 1.f), 0.5f),
	    std::make_unique<AudioParameterFloat>("osc1_drift", "Osc1 Drift", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc1_vec_x", "Osc1 Vector X", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc1_vec_y", "Osc1 Vector Y", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc1_arp_speed",
	                                          "Osc1 Arpeggiator Speed",
	                                          NormalisableRange<float>(1.000000, 100.000000, 0, 0.354358f),
	                                          20.f),
	    std::make_unique<AudioParameterInt>("osc1_step_1", "Osc1 Step1", -12, 12, 0),
	    std::make_unique<AudioParameterInt>("osc1_step_2", "Osc1 Step2", -12, 12, 12),
	    std::make_unique<AudioParameterInt>("osc1_step_3", "Osc1 Step3", -12, 12, 7),
	    std::make_unique<AudioParameterInt>("osc1_arp_on", "Osc1 Arpeggiator On", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc1_step_3_on", "Osc1 Step3 On", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc1_chipnoise", "Osc1 Chipnoise", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc1_exp_fm", "Osc1 Exponential FM", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>("osc1_fm", "Osc1 FM/PM amount", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc1_lp",
	                                          "Osc1 Noise Lowpass",
	                                          NormalisableRange<float>(80.f, 18000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          18000.f),
	    std::make_unique<AudioParameterFloat>(
	        "osc1_hp", "Osc1 Noise Highpass", NormalisableRange<float>(80.f, 18000.f, 0.f, FILTER_SKEW_DEFAULT), 80.f),
	    // Osc 2
	    std::make_unique<AudioParameterInt>("osc2_oct", "Osc2 Octave", -4, 4, 0),
	    std::make_unique<AudioParameterInt>("osc2_semi", "Osc2 Semitones", -7, 7, 0),
	    std::make_unique<AudioParameterFloat>("osc2_fine", "Osc2 Finetune", NormalisableRange<float>(-50.0f, 50.0f), 0),
	    std::make_unique<AudioParameterFloat>("osc2_vol", "Osc2 Volume", NormalisableRange<float>(-60.0f, 12.0f), 0),
	    std::make_unique<AudioParameterInt>("osc2_reset", "Osc2 Reset", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc2_sync", "Osc2 Sync", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc2_position", "Osc2 Wavetable Position", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>("osc2_detune", "Osc2 Detune", NormalisableRange<float>(0.f, 1.f), 0.2),
	    std::make_unique<AudioParameterFloat>(
	        "osc2_pos_mod", "Osc2 Position Modulation", NormalisableRange<float>(-1.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc2_multi_position", "Osc2 Multi Position", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc2_spread", "Osc2 Wavetable Spread", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc2_pulsewidth", "Osc2 PulseWidth", NormalisableRange<float>(0.f, 1.f), 0.5f),
	    std::make_unique<AudioParameterFloat>("osc2_drift", "Osc2 Drift", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc2_vec_x", "Osc2 Vector X", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc2_vec_y", "Osc2 Vector Y", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc2_arp_speed",
	                                          "Osc2 Arpeggiator Speed",
	                                          NormalisableRange<float>(1.000000, 100.000000, 0, 0.354358f),
	                                          20.f),
	    std::make_unique<AudioParameterInt>("osc2_step_1", "Osc2 Step1", -12, 12, 0),
	    std::make_unique<AudioParameterInt>("osc2_step_2", "Osc2 Step2", -12, 12, 12),
	    std::make_unique<AudioParameterInt>("osc2_step_3", "Osc2 Step3", -12, 12, 7),
	    std::make_unique<AudioParameterInt>("osc2_arp_on", "Osc2 Arpeggiator On", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc2_step_3_on", "Osc2 Step3 On", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc2_chipnoise", "Osc2 Chipnoise", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc2_exp_fm", "Osc2 Exponential FM", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>("osc2_fm", "Osc2 FM/PM amount", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc2_lp",
	                                          "Osc2 Noise Lowpass",
	                                          NormalisableRange<float>(80.f, 18000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          18000.f),
	    std::make_unique<AudioParameterFloat>(
	        "osc2_hp", "Osc2 Noise Highpass", NormalisableRange<float>(80.f, 18000.f, 0.f, FILTER_SKEW_DEFAULT), 80.f),
	    // Osc 3
	    std::make_unique<AudioParameterInt>("osc3_oct", "Osc3 Octave", -4, 4, 0),
	    std::make_unique<AudioParameterInt>("osc3_semi", "Osc3 Semitones", -7, 7, 0),
	    std::make_unique<AudioParameterFloat>("osc3_fine", "Osc3 Finetune", NormalisableRange<float>(-50.0f, 50.0f), 0),
	    std::make_unique<AudioParameterFloat>("osc3_vol", "Osc3 Volume", NormalisableRange<float>(-60.0f, 12.0f), 0),
	    std::make_unique<AudioParameterInt>("osc3_reset", "Osc3 Reset", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc3_sync", "Osc3 Sync", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc3_position", "Osc3 Wavetable Position", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>("osc3_detune", "Osc3 Detune", NormalisableRange<float>(0.f, 1.f), 0.2),
	    std::make_unique<AudioParameterFloat>(
	        "osc3_pos_mod", "Osc3 Position Modulation", NormalisableRange<float>(-1.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc3_multi_position", "Osc3 Multi Position", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc3_spread", "Osc3 Wavetable Spread", NormalisableRange<float>(0.f, 1.f), 0),
	    std::make_unique<AudioParameterFloat>(
	        "osc3_pulsewidth", "Osc3 PulseWidth", NormalisableRange<float>(0.f, 1.f), 0.5f),
	    std::make_unique<AudioParameterFloat>("osc3_drift", "Osc3 Drift", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc3_vec_x", "Osc3 Vector X", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc3_vec_y", "Osc3 Vector Y", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc3_arp_speed",
	                                          "Osc3 Arpeggiator Speed",
	                                          NormalisableRange<float>(1.000000, 100.000000, 0, 0.354358f),
	                                          20.f),
	    std::make_unique<AudioParameterInt>("osc3_step_1", "Osc3 Step1", -12, 12, 0),
	    std::make_unique<AudioParameterInt>("osc3_step_2", "Osc3 Step2", -12, 12, 12),
	    std::make_unique<AudioParameterInt>("osc3_step_3", "Osc3 Step3", -12, 12, 7),
	    std::make_unique<AudioParameterInt>("osc3_arp_on", "Osc3 Arpeggiator On", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc3_step_3_on", "Osc3 Step3 On", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc3_chipnoise", "Osc3 Chipnoise", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("osc3_exp_fm", "Osc3 Exponential FM", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>("osc3_fm", "Osc3 FM/PM amount", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("osc3_lp",
	                                          "Osc3 Noise Lowpass",
	                                          NormalisableRange<float>(80.f, 18000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          18000.f),
	    std::make_unique<AudioParameterFloat>(
	        "osc3_hp", "Osc3 Noise Highpass", NormalisableRange<float>(80.f, 18000.f, 0.f, FILTER_SKEW_DEFAULT), 80.f),
	    // Filter1
	    std::make_unique<AudioParameterInt>("fil1_osc1", "Filter1 Osc1 Input", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("fil1_osc2", "Filter1 Osc2 Input", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("fil1_osc3", "Filter1 Osc3 Input", 0, 1, 1),
	    std::make_unique<AudioParameterFloat>(
	        "fil1_vel", "Filter1 Velocity Amount", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil1_env", "Filter1 Envelope Amount", NormalisableRange<float>(-1.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil1_kbd", "Filter1 Keyboard Follow", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("fil1_gain", "Filter1 Gain", NormalisableRange<float>(-60.f, 12.f), 0.f),
	    std::make_unique<AudioParameterFloat>("fil1_freq",
	                                          "Filter1 Frequency",
	                                          NormalisableRange<float>(20.f, 20000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          20000.f),
	    std::make_unique<AudioParameterFloat>("fil1_res", "Filter1 Resonance", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil1_saturation", "Filter1 Saturation", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil1_sem_transition", "Filter1 SEM Transition", NormalisableRange<float>(-1.f, 1.f), -1.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil1_formant_transition", "Filter1 Formant Transition", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil1_ring_mod_amount", "Filter1 RingMod Amount", NormalisableRange<float>(0.f, 1.f), 1.f),
	    std::make_unique<AudioParameterInt>("fil2_osc1", "Filter2 Osc1 Input", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("fil2_osc2", "Filter2 Osc2 Input", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("fil2_osc3", "Filter2 Osc3 Input", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("fil2_fil1", "Filter2 Filter1 Input", 0, 1, 1),
	    std::make_unique<AudioParameterFloat>(
	        "fil2_vel", "Filter2 Velocity Amount", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil2_env", "Filter2 Envelope Amount", NormalisableRange<float>(-1.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil2_kbd", "Filter2 Keyboard Follow", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("fil2_gain", "Filter2 Gain", NormalisableRange<float>(-60.f, 12.f), 0.f),
	    std::make_unique<AudioParameterFloat>("fil2_freq",
	                                          "Filter2 Frequency",
	                                          NormalisableRange<float>(20.f, 20000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          2000.f),
	    std::make_unique<AudioParameterFloat>("fil2_res", "Filter2 Resonance", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil2_saturation", "Filter2 Saturation", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil2_sem_transition", "Filter2 SEM Transition", NormalisableRange<float>(-1.f, 1.f), -1.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil2_formant_transition", "Filter2 Formant Transition", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil2_ring_mod_amount", "Filter2 RingMod Amount", NormalisableRange<float>(0.f, 1.f), 1.f),
	    // Filter3
	    std::make_unique<AudioParameterFloat>(
	        "fil3_vel", "Filter3 Velocity Amount", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil3_env", "Filter3 Envelope Amount", NormalisableRange<float>(-1.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil3_kbd", "Filter3 Keyboard Follow", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("fil3_gain", "Filter3 Gain", NormalisableRange<float>(-60.f, 12.f), 0.f),
	    std::make_unique<AudioParameterFloat>("fil3_freq",
	                                          "Filter3 Frequency",
	                                          NormalisableRange<float>(20.f, 20000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          2000.f),
	    std::make_unique<AudioParameterFloat>("fil3_res", "Filter3 Resonance", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil3_saturation", "Filter3 Saturation", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil3_sem_transition", "Filter3 SEM Transition", NormalisableRange<float>(-1.f, 1.f), -1.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil3_formant_transition", "Filter3 Formant Transition", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "fil3_ring_mod_amount", "Filter3 RingMod Amount", NormalisableRange<float>(0.f, 1.f), 1.f),
	    // Amp
	    std::make_unique<AudioParameterFloat>(
	        "amp_velocity", "Amplifier Velocity", NormalisableRange<float>(0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "amp_pan", "Amplifier Panorama", NormalisableRange<float>(-1.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("amp_gain", "Amplifier Gain", NormalisableRange<float>(-30.f, 12.f), 0.f),
	    // Distortion
	    std::make_unique<AudioParameterFloat>(
	        "dist_boost", "Distortion Boost", NormalisableRange<float>(0.f, 1.f), 0.3f),
	    std::make_unique<AudioParameterFloat>(
	        "dist_drywet", "Distortion DryWet", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 1.f),
	    // ADSR1
	    std::make_unique<AudioParameterFloat>(
	        "env1_attack", "AmpEnv Attack", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterFloat>(
	        "env1_decay", "AmpEnv Decay", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 1.f),
	    std::make_unique<AudioParameterFloat>(
	        "env1_sustain", "AmpEnv Sustain", NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717f), 0.5f),
	    std::make_unique<AudioParameterFloat>(
	        "env1_release", "AmpEnv Release", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterInt>("env1_loop", "AmpEnv Loop", 0, 1, 0),
	    // ADSR2
	    std::make_unique<AudioParameterFloat>(
	        "env2_attack", "FilterEnv Attack", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterFloat>(
	        "env2_decay", "FilterEnv Decay", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.8f),
	    std::make_unique<AudioParameterFloat>(
	        "env2_sustain", "FilterEnv Sustain", NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "env2_release", "FilterEnv Release", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterInt>("env2_loop", "FilterEnv Loop", 0, 1, 0),
	    // ADSR3
	    std::make_unique<AudioParameterFloat>(
	        "env3_attack", "ModEnv Attack", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterFloat>(
	        "env3_decay", "ModEnv Decay", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.8f),
	    std::make_unique<AudioParameterFloat>(
	        "env3_sustain", "ModEnv Sustain", NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "env3_release", "ModEnv Release", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterInt>("env3_loop", "ModEnv Loop", 0, 1, 0),
	    // ADSR4
	    std::make_unique<AudioParameterFloat>(
	        "env4_attack", "GlobalEnv Attack", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterFloat>(
	        "env4_decay", "GlobalEnv Decay", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 1.f),
	    std::make_unique<AudioParameterFloat>(
	        "env4_sustain", "GlobalEnv Sustain", NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717f), 0.5f),
	    std::make_unique<AudioParameterFloat>(
	        "env4_release", "GlobalEnv Release", NormalisableRange<float>(0.001f, 30.f, 0.f, 0.300912f), 0.01f),
	    std::make_unique<AudioParameterInt>("env4_loop", "GlobalEnv Loop", 0, 1, 0),
	    // LFO1
	    std::make_unique<AudioParameterInt>("lfo1_reset", "LFO1 Reset", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "lfo1_freq", "LFO1 Frequency", NormalisableRange<float>(0.01f, 100.f, 0.f, 0.196757f), 1),
	    // LFO2
	    std::make_unique<AudioParameterInt>("lfo2_reset", "LFO2 Reset", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "lfo2_freq", "LFO2 Frequency", NormalisableRange<float>(0.01f, 100.f, 0.f, 0.196757f), 1),
	    // LFO3
	    std::make_unique<AudioParameterInt>("lfo3_reset", "LFO3 Reset", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "lfo3_freq", "LFO3 Frequency", NormalisableRange<float>(0.01f, 100.f, 0.f, 0.196757f), 1),
	    // LFO4
	    std::make_unique<AudioParameterInt>("lfo4_reset", "LFO4 Reset", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "lfo4_freq", "LFO4 Frequency", NormalisableRange<float>(0.01f, 100.f, 0.f, 0.196757f), 1),
	    // Misc
	    std::make_unique<AudioParameterInt>("fil1_to_amp", "Filter1 To Amplifier", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("fil2_to_amp", "Filter2 To Amplifier", 0, 1, 1),
	    std::make_unique<AudioParameterFloat>("glide", "Glide", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("master", "Master", -30.f, 12.f, -7.f),
	    std::make_unique<AudioParameterFloat>(
	        "modwheel", "Modwheel", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "pitchbend", "Pitchbend", NormalisableRange<float>(-1.f, 1.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("xy_x", "XY Pad X", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>("xy_y", "XY Pad Y", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterInt>("delay_pingpong", "Delay PingPong", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "delay_time", "Delay Time", NormalisableRange<float>(0.01f, 3.f, 0.f, 0.627099), 1.f),
	    std::make_unique<AudioParameterFloat>(
	        "delay_feedback", "Delay Feedback", NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717), 0.45f),
	    std::make_unique<AudioParameterFloat>("delay_hp",
	                                          "Delay Highpass Frequency",
	                                          NormalisableRange<float>(80.f, 18000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          80.f),
	    std::make_unique<AudioParameterFloat>(
	        "delay_ducking", "Delay Ducking", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "delay_dry", "Delay Dry", NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717), 1.f),
	    std::make_unique<AudioParameterFloat>(
	        "delay_wet", "Delay Wet", NormalisableRange<float>(0.f, 1.f, 0.f, 0.575717), 0.7f),
	    std::make_unique<AudioParameterInt>("delay_on", "Delay On", 0, 1, 0),

	    std::make_unique<AudioParameterFloat>(
	        "rev_delay", "Reverb PreDelay", NormalisableRange<float>(20.f, 80.f, 0.f, 1), 40.f),
	    std::make_unique<AudioParameterFloat>(
	        "rev_eqgain", "Reverb EQ Gain", NormalisableRange<float>(-12.f, 12.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "rev_eqfreq", "Reverb EQ Freq", NormalisableRange<float>(100.f, 10000.f, 0.f, FILTER_SKEW_DEFAULT), 1000.f),
	    //std::make_unique<AudioParameterFloat>(
	    //    "rev_ducking", "Reverb Ducking", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "rev_drywet", "Reverb DryWet", NormalisableRange<float>(0.f, 1.f, 0.f, 0.7f), 0.25f),
	    std::make_unique<AudioParameterFloat>(
	        "rev_mid_hall", "Reverb Mid Hall", NormalisableRange<float>(1.f, 8.f, 0.f, 1.f), 2.f),
	    std::make_unique<AudioParameterFloat>("rev_hf_damp",
	                                          "Reverb High Freq Damp",
	                                          NormalisableRange<float>(1500.f, 24000.f, 0.f, FILTER_SKEW_DEFAULT),
	                                          6000.f),
	    std::make_unique<AudioParameterInt>("reverb_on", "Reverb On", 0, 1, 0),

	    // Phaser
	    std::make_unique<AudioParameterInt>("phaser_reset", "Phaser Reset", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("phaser_on", "Phaser On", 0, 1, 0),
	    std::make_unique<AudioParameterFloat>(
	        "phaser_freq", "Phaser Frequency", NormalisableRange<float>(40.000000, 8000.000000, 0, 0.444854), 4000),
	    std::make_unique<AudioParameterFloat>(
	        "phaser_feedback", "Phaser Feedback", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.25f),
	    std::make_unique<AudioParameterFloat>(
	        "phaser_rate", "Phaser Rate", NormalisableRange<float>(0.050000, 20.000000, 0, 0.211267), 0.25f),
	    std::make_unique<AudioParameterFloat>(
	        "phaser_mod", "Phaser Amount", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.3f),
	    std::make_unique<AudioParameterFloat>(
	        "phaser_drywet", "Phaser DryWet", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 1.f),
	    // Flanger
	    std::make_unique<AudioParameterInt>("flanger_reset", "Flanger Reset", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("flanger_on", "Flanger On", 0, 1, 0),

	    std::make_unique<AudioParameterFloat>(
	        "flanger_rate", "Flanger Rate", NormalisableRange<float>(0.05f, 20.f, 0, 0.211267), 0.15f),
	    std::make_unique<AudioParameterFloat>("flanger_feedback", "Flanger Feedback", -1.f, 1.f, 0.6f),
	    std::make_unique<AudioParameterFloat>(
	        "flanger_amount", "Flanger Amount", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.3f),
	    std::make_unique<AudioParameterFloat>(
	        "flanger_drywet", "Flanger DryWet", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 1.f),
	    std::make_unique<AudioParameterInt>("chorus_reset", "Chorus Reset", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("chorus_on", "Chorus On", 0, 1, 0),

	    std::make_unique<AudioParameterFloat>(
	        "chorus_rate", "Chorus Rate", NormalisableRange<float>(0.05f, 20.f, 0, 0.211267), 0.15f),
	    std::make_unique<AudioParameterFloat>(
	        "chorus_amount", "Chorus Amount", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.4f),
	    std::make_unique<AudioParameterFloat>("chorus_feedback", "Chorus Feedback", -1.f, 1.f, 0.f),
	    std::make_unique<AudioParameterFloat>(
	        "chorus_drywet", "Chorus DryWet", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 1.f),
	    std::make_unique<AudioParameterFloat>(
	        "unison_detune", "Unison Detune", NormalisableRange<float>(0.f, 0.6f, 0.f, 0.5f), 0.08f),
	    std::make_unique<AudioParameterFloat>(
	        "unison_width", "Unison Width", NormalisableRange<float>(0.f, 1.f, 0.f, 1.f), 0.7f),
	    // Arpeggiator / StepSequencer
	    std::make_unique<AudioParameterInt>("step_0_on", "Arp Step 1 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_1_on", "Arp Step 2 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_2_on", "Arp Step 3 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_3_on", "Arp Step 4 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_4_on", "Arp Step 5 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_5_on", "Arp Step 6 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_6_on", "Arp Step 7 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_7_on", "Arp Step 8 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_8_on", "Arp Step 9 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_9_on", "Arp Step 10 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_10_on", "Arp Step 11 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_11_on", "Arp Step 12 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_12_on", "Arp Step 13 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_13_on", "Arp Step 14 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_14_on", "Arp Step 15 On", 0, 1, 1),
	    std::make_unique<AudioParameterInt>("step_15_on", "Arp Step 16 On", 0, 1, 1),

	    std::make_unique<AudioParameterFloat>("step_0_mod_1", "Arp Step 1 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_1_mod_1", "Arp Step 2 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_2_mod_1", "Arp Step 3 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_3_mod_1", "Arp Step 4 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_4_mod_1", "Arp Step 5 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_5_mod_1", "Arp Step 6 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_6_mod_1", "Arp Step 7 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_7_mod_1", "Arp Step 8 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_8_mod_1", "Arp Step 9 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_9_mod_1", "Arp Step 10 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_10_mod_1", "Arp Step 11 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_11_mod_1", "Arp Step 12 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_12_mod_1", "Arp Step 13 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_13_mod_1", "Arp Step 14 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_14_mod_1", "Arp Step 15 Mod 1", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_15_mod_1", "Arp Step 16 Mod 1", -1, 1, 0),

	    std::make_unique<AudioParameterFloat>("step_0_mod_2", "Arp Step 1 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_1_mod_2", "Arp Step 2 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_2_mod_2", "Arp Step 3 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_3_mod_2", "Arp Step 4 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_4_mod_2", "Arp Step 5 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_5_mod_2", "Arp Step 6 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_6_mod_2", "Arp Step 7 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_7_mod_2", "Arp Step 8 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_8_mod_2", "Arp Step 9 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_9_mod_2", "Arp Step 10 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_10_mod_2", "Arp Step 11 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_11_mod_2", "Arp Step 12 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_12_mod_2", "Arp Step 13 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_13_mod_2", "Arp Step 14 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_14_mod_2", "Arp Step 15 Mod 2", -1, 1, 0),
	    std::make_unique<AudioParameterFloat>("step_15_mod_2", "Arp Step 16 Mod 2", -1, 1, 0),

	    std::make_unique<AudioParameterInt>("step_0_transpose", "Arp Step 1 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_1_transpose", "Arp Step 2 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_2_transpose", "Arp Step 3 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_3_transpose", "Arp Step 4 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_4_transpose", "Arp Step 5 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_5_transpose", "Arp Step 6 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_6_transpose", "Arp Step 7 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_7_transpose", "Arp Step 8 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_8_transpose", "Arp Step 9 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_9_transpose", "Arp Step 10 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_10_transpose", "Arp Step 11 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_11_transpose", "Arp Step 12 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_12_transpose", "Arp Step 13 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_13_transpose", "Arp Step 14 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_14_transpose", "Arp Step 15 Transpose", -24, 24, 0),
	    std::make_unique<AudioParameterInt>("step_15_transpose", "Arp Step 16 Transpose", -24, 24, 0),

	    std::make_unique<AudioParameterFloat>("arp_mod_transpose", "Arp Mod Transpose", 0, 1, 0),

	    std::make_unique<AudioParameterInt>("arp_on", "Arpeggiator On", 0, 1, 0),
	    std::make_unique<AudioParameterInt>("arp_one_shot", "Arpeggiator OneShot", 0, 1, 0)
}
