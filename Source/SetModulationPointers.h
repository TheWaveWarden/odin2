
void OdinAudioProcessor::setModulationPointers() {
	//========================================
	//==============  SOURCES  ===============
	//========================================

	for (int voice = 0; voice < VOICES; ++voice) {
		m_mod_sources.voice[voice].osc[0]        = &(m_osc_output[voice][0]);
		m_mod_sources.voice[voice].osc[1]        = &(m_osc_output[voice][1]);
		m_mod_sources.voice[voice].osc[2]        = &(m_osc_output[voice][2]);
		m_mod_sources.voice[voice].filter[0]     = &(m_filter_output[voice][0]);
		m_mod_sources.voice[voice].filter[1]     = &(m_filter_output[voice][1]);
		m_mod_sources.voice[voice].adsr[0]       = &(m_adsr[voice][0]);
		m_mod_sources.voice[voice].adsr[1]       = &(m_adsr[voice][1]);
		m_mod_sources.voice[voice].adsr[2]       = &(m_adsr[voice][2]);
		m_mod_sources.voice[voice].lfo[0]        = &(m_lfo[voice][0]);
		m_mod_sources.voice[voice].lfo[1]        = &(m_lfo[voice][1]);
		m_mod_sources.voice[voice].lfo[2]        = &(m_lfo[voice][2]);
		m_mod_sources.voice[voice].MIDI_key      = &(m_voice[voice].MIDI_key_mod_source);
		m_mod_sources.voice[voice].MIDI_velocity = &(m_voice[voice].MIDI_velocity_mod_source);
		m_mod_sources.voice[voice].MIDI_aftertouch = &(m_voice[voice].MIDI_aftertouch_mod_source);
		m_mod_sources.voice[voice].unison_position = &(m_voice[voice].unison_pan_position);
		m_mod_sources.voice[voice].random        = &(m_voice[voice].random_modulation);
	}
	m_mod_sources.global_adsr           = &m_global_env_mod_source;
	m_mod_sources.global_lfo            = &m_global_lfo_mod_source;
	m_mod_sources.MIDI_channel_pressure = &(m_MIDI_channel_pressure);
	m_mod_sources.x                     = &m_x_smooth;
	m_mod_sources.y                     = &m_y_smooth;
	m_mod_sources.modwheel              = &m_modwheel_smooth;
	m_mod_sources.pitchwheel            = &m_pitch_bend_smooth;
	m_mod_sources.constant              = &(m_constant);
	m_mod_sources.sustain_pedal         = &(m_voice_manager.m_sustain_active_float);
	m_mod_sources.soft_pedal            = &(m_soft_pedal);

	//========================================
	//============= DESTINATIONS =============
	//========================================
	for (int voice = 0; voice < VOICES; ++voice) {
		for (int osc = 0; osc < 3; ++osc) {
			m_voice[voice].analog_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].wavetable_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].multi_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].vector_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].fm_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].pm_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].chiptune_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].wavedraw_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].chipdraw_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));
			m_voice[voice].specdraw_osc[osc].setPitchBendPointer(&(m_pitch_bend_smooth_and_applied));

			m_voice[voice].analog_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].wavetable_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].multi_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].vector_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].fm_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].pm_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].chiptune_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].wavedraw_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].chipdraw_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
			m_voice[voice].specdraw_osc[osc].setPitchModExpPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));

			m_voice[voice].analog_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].wavetable_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].multi_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].vector_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].fm_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].pm_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].chiptune_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].wavedraw_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].chipdraw_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));
			m_voice[voice].specdraw_osc[osc].setGlidePointer(&(m_mod_destinations.misc.glide));

			m_voice[voice].analog_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].wavetable_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].multi_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].vector_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].fm_osc[osc].setPitchModLinPointer(&(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].pm_osc[osc].setPitchModLinPointer(&(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].chiptune_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].wavedraw_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].chipdraw_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
			m_voice[voice].specdraw_osc[osc].setPitchModLinPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));

			m_voice[voice].analog_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].wavetable_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].multi_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].vector_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].fm_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].pm_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].chiptune_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].wavedraw_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].chipdraw_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].specdraw_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));
			m_voice[voice].noise_osc[osc].setVolModPointer(&(m_mod_destinations.voice[voice].osc[osc].vol));

			m_voice[voice].analog_osc[osc].setPWMModPointer(&(m_mod_destinations.voice[voice].osc[osc].pulse_width));

			m_voice[voice].wavetable_osc[osc].setPosModPointer(&(m_mod_destinations.voice[voice].osc[osc].position));
			m_voice[voice].multi_osc[osc].setPosModPointer(&(m_mod_destinations.voice[voice].osc[osc].position));

			m_voice[voice].multi_osc[osc].setDetuneModPointer(&(m_mod_destinations.voice[voice].osc[osc].detune));

			m_voice[voice].multi_osc[osc].setSpreadModPointer(&(m_mod_destinations.voice[voice].osc[osc].spread));

			m_voice[voice].vector_osc[osc].setXModPointer(&(m_mod_destinations.voice[voice].osc[osc].x));
			m_voice[voice].vector_osc[osc].setYModPointer(&(m_mod_destinations.voice[voice].osc[osc].y));

			m_voice[voice].chiptune_osc[osc].setArpSpeedModPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].arp_speed));

			m_voice[voice].fm_osc[osc].setFMModPointer(&(m_mod_destinations.voice[voice].osc[osc].fm_amount));

			m_voice[voice].pm_osc[osc].setPMModPointer(&(m_mod_destinations.voice[voice].osc[osc].fm_amount));

			m_voice[voice].fm_osc[osc].setCarrierRatioModPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].carrier_ratio));
			m_voice[voice].fm_osc[osc].setModulatorRatioModPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].modulator_ratio));

			m_voice[voice].pm_osc[osc].setCarrierRatioModPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].carrier_ratio));
			m_voice[voice].pm_osc[osc].setModulatorRatioModPointer(
			    &(m_mod_destinations.voice[voice].osc[osc].modulator_ratio));

			m_voice[voice].noise_osc[osc].setHPModPointer(&(m_mod_destinations.voice[voice].osc[osc].hp_freq));
			m_voice[voice].noise_osc[osc].setLPModPointer(&(m_mod_destinations.voice[voice].osc[osc].lp_freq));
		}
		for (int fil = 0; fil < 2; ++fil) {
			m_voice[voice].ladder_filter[fil].setFreqModPointer(&(m_mod_destinations.voice[voice].filter[fil].freq));
			m_voice[voice].diode_filter[fil].setFreqModPointer(&(m_mod_destinations.voice[voice].filter[fil].freq));
			m_voice[voice].korg_filter[fil].setFreqModPointer(&(m_mod_destinations.voice[voice].filter[fil].freq));
			m_voice[voice].comb_filter[fil].setFreqModPointer(&(m_mod_destinations.voice[voice].filter[fil].freq));
			m_voice[voice].SEM_filter_12[fil].setFreqModPointer(&(m_mod_destinations.voice[voice].filter[fil].freq));
			m_voice[voice].ring_mod[fil].setPitchModExpPointer(&(m_mod_destinations.voice[voice].filter[fil].freq));

			m_voice[voice].ladder_filter[fil].setResModPointer(&(m_mod_destinations.voice[voice].filter[fil].res));
			m_voice[voice].diode_filter[fil].setResModPointer(&(m_mod_destinations.voice[voice].filter[fil].res));
			m_voice[voice].korg_filter[fil].setResModPointer(&(m_mod_destinations.voice[voice].filter[fil].res));
			m_voice[voice].SEM_filter_12[fil].setResModPointer(&(m_mod_destinations.voice[voice].filter[fil].res));
			m_voice[voice].comb_filter[fil].setResModPointer(&(m_mod_destinations.voice[voice].filter[fil].res));

			m_voice[voice].ladder_filter[fil].setVolModPointer(&(m_mod_destinations.voice[voice].filter[fil].gain));
			m_voice[voice].diode_filter[fil].setVolModPointer(&(m_mod_destinations.voice[voice].filter[fil].gain));
			m_voice[voice].korg_filter[fil].setVolModPointer(&(m_mod_destinations.voice[voice].filter[fil].gain));
			m_voice[voice].SEM_filter_12[fil].setVolModPointer(&(m_mod_destinations.voice[voice].filter[fil].gain));
			m_voice[voice].comb_filter[fil].setVolModPointer(&(m_mod_destinations.voice[voice].filter[fil].gain));
			m_voice[voice].formant_filter[fil].setVolModPointer(&(m_mod_destinations.voice[voice].filter[fil].gain));
			m_voice[voice].ring_mod[fil].setVolModPointer(&(m_mod_destinations.voice[voice].filter[fil].gain));

			m_voice[voice].ladder_filter[fil].setEnvModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].env_amount));
			m_voice[voice].diode_filter[fil].setEnvModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].env_amount));
			m_voice[voice].korg_filter[fil].setEnvModPointer(&(m_mod_destinations.voice[voice].filter[fil].env_amount));
			m_voice[voice].SEM_filter_12[fil].setEnvModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].env_amount));
			m_voice[voice].comb_filter[fil].setEnvModPointer(&(m_mod_destinations.voice[voice].filter[fil].env_amount));
			m_voice[voice].formant_filter[fil].setEnvModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].env_amount));
			m_voice[voice].ring_mod[fil].setEnvModPointer(&(m_mod_destinations.voice[voice].filter[fil].env_amount));

			m_voice[voice].ladder_filter[fil].setVelModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
			m_voice[voice].diode_filter[fil].setVelModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
			m_voice[voice].korg_filter[fil].setVelModPointer(&(m_mod_destinations.voice[voice].filter[fil].vel_amount));
			m_voice[voice].SEM_filter_12[fil].setVelModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
			m_voice[voice].comb_filter[fil].setVelModPointer(&(m_mod_destinations.voice[voice].filter[fil].vel_amount));
			m_voice[voice].formant_filter[fil].setVelModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
			m_voice[voice].ring_mod[fil].setVelModPointer(&(m_mod_destinations.voice[voice].filter[fil].vel_amount));

			m_voice[voice].ladder_filter[fil].setKbdModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
			m_voice[voice].diode_filter[fil].setKbdModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
			m_voice[voice].korg_filter[fil].setKbdModPointer(&(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
			m_voice[voice].SEM_filter_12[fil].setKbdModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
			m_voice[voice].comb_filter[fil].setKbdModPointer(&(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
			m_voice[voice].formant_filter[fil].setKbdModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
			m_voice[voice].ring_mod[fil].setKbdModPointer(&(m_mod_destinations.voice[voice].filter[fil].kbd_amount));

			m_voice[voice].ladder_filter[fil].setSaturationModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].saturation));
			m_voice[voice].diode_filter[fil].setSaturationModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].saturation));
			m_voice[voice].korg_filter[fil].setSaturationModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].saturation));

			m_voice[voice].SEM_filter_12[fil].setTransitionModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].SEM_transition));

			m_voice[voice].formant_filter[fil].setTransitionModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].formant_transition));

			m_voice[voice].ring_mod[fil].setRingModAmountModPointer(
			    &(m_mod_destinations.voice[voice].filter[fil].ringmod_amount));
		}

		for (int mod = 0; mod < 3; ++mod) {
			m_voice[voice].lfo[mod].setPitchModExpPointer(&(m_mod_destinations.voice[voice].lfo[mod].freq));

			m_voice[voice].env[mod].setAttackModPointer(&(m_mod_destinations.voice[voice].adsr[mod].attack));
			m_voice[voice].env[mod].setDecayModPointer(&(m_mod_destinations.voice[voice].adsr[mod].decay));
			m_voice[voice].env[mod].setSustainModPointer(&(m_mod_destinations.voice[voice].adsr[mod].sustain));
			m_voice[voice].env[mod].setReleaseModPointer(&(m_mod_destinations.voice[voice].adsr[mod].release));
		}
		m_global_lfo.setPitchModExpPointer(&(m_mod_destinations.global_lfo.freq));
		m_global_env.setAttackModPointer(&(m_mod_destinations.global_adsr.attack));
		m_global_env.setDecayModPointer(&(m_mod_destinations.global_adsr.decay));
		m_global_env.setSustainModPointer(&(m_mod_destinations.global_adsr.sustain));
		m_global_env.setReleaseModPointer(&(m_mod_destinations.global_adsr.release));

		m_voice[voice].amp.setGainModPointer(&(m_mod_destinations.voice[voice].amp.gain));
		m_voice[voice].amp.setPanModPointer(&(m_mod_destinations.voice[voice].amp.pan));
		m_voice[voice].amp.setVelModPointer(&(m_mod_destinations.voice[voice].amp.vel));

		m_voice[voice].distortion[0].setThresholdModPointer(&(m_mod_destinations.voice[voice].distortion.boost));
		m_voice[voice].distortion[0].setDryWetModPointer(&(m_mod_destinations.voice[voice].distortion.drywet));
		m_voice[voice].distortion[1].setThresholdModPointer(&(m_mod_destinations.voice[voice].distortion.boost));
		m_voice[voice].distortion[1].setDryWetModPointer(&(m_mod_destinations.voice[voice].distortion.drywet));
	}

	for (int stereo = 0; stereo < 2; ++stereo) {

		m_ladder_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
		m_diode_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
		m_korg_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
		m_comb_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
		m_SEM_filter_12[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
		m_ring_mod[stereo].setPitchModExpPointer(&(m_mod_destinations.filter3.freq));

		m_ladder_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
		m_diode_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
		m_korg_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
		m_SEM_filter_12[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
		m_comb_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));

		m_ladder_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
		m_diode_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
		m_korg_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
		m_SEM_filter_12[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
		m_comb_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
		m_formant_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
		m_ring_mod[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));

		m_ladder_filter[stereo].setEnvModPointer(&(m_mod_destinations.filter3.env_amount));
		m_diode_filter[stereo].setEnvModPointer(&(m_mod_destinations.filter3.env_amount));
		m_korg_filter[stereo].setEnvModPointer(&(m_mod_destinations.filter3.env_amount));
		m_SEM_filter_12[stereo].setEnvModPointer(&(m_mod_destinations.filter3.env_amount));
		m_comb_filter[stereo].setEnvModPointer(&(m_mod_destinations.filter3.env_amount));
		m_formant_filter[stereo].setEnvModPointer(&(m_mod_destinations.filter3.env_amount));
		m_ring_mod[stereo].setEnvModPointer(&(m_mod_destinations.filter3.env_amount));

		m_ladder_filter[stereo].setVelModPointer(&(m_mod_destinations.filter3.vel_amount));
		m_diode_filter[stereo].setVelModPointer(&(m_mod_destinations.filter3.vel_amount));
		m_korg_filter[stereo].setVelModPointer(&(m_mod_destinations.filter3.vel_amount));
		m_SEM_filter_12[stereo].setVelModPointer(&(m_mod_destinations.filter3.vel_amount));
		m_comb_filter[stereo].setVelModPointer(&(m_mod_destinations.filter3.vel_amount));
		m_ring_mod[stereo].setVelModPointer(&(m_mod_destinations.filter3.vel_amount));

		m_ladder_filter[stereo].setKbdModPointer(&(m_mod_destinations.filter3.kbd_amount));
		m_diode_filter[stereo].setKbdModPointer(&(m_mod_destinations.filter3.kbd_amount));
		m_korg_filter[stereo].setKbdModPointer(&(m_mod_destinations.filter3.kbd_amount));
		m_SEM_filter_12[stereo].setKbdModPointer(&(m_mod_destinations.filter3.kbd_amount));
		m_comb_filter[stereo].setKbdModPointer(&(m_mod_destinations.filter3.kbd_amount));
		m_formant_filter[stereo].setKbdModPointer(&(m_mod_destinations.filter3.kbd_amount));
		m_ring_mod[stereo].setKbdModPointer(&(m_mod_destinations.filter3.kbd_amount));

		m_ladder_filter[stereo].setSaturationModPointer(&(m_mod_destinations.filter3.saturation));
		m_diode_filter[stereo].setSaturationModPointer(&(m_mod_destinations.filter3.saturation));
		m_korg_filter[stereo].setSaturationModPointer(&(m_mod_destinations.filter3.saturation));

		m_SEM_filter_12[stereo].setTransitionModPointer(&(m_mod_destinations.filter3.SEM_transition));

		m_formant_filter[stereo].setTransitionModPointer(&(m_mod_destinations.filter3.formant_transition));

		m_ring_mod[stereo].setRingModAmountModPointer(&(m_mod_destinations.filter3.ringmod_amount));

		m_delay.setTimeModPointer(&(m_mod_destinations.delay.time));
		m_delay.setFeedbackModPointer(&(m_mod_destinations.delay.feedback));
		m_delay.setHPFreqModPointer(&(m_mod_destinations.delay.hp_freq));
		m_delay.setDryModPointer(&(m_mod_destinations.delay.dry));
		m_delay.setWetModPointer(&(m_mod_destinations.delay.wet));

		m_flanger[stereo].setFreqModPointer(&(m_mod_destinations.flanger.freq));
		m_flanger[stereo].setAmountModPointer(&(m_mod_destinations.flanger.amount));
		m_flanger[stereo].setFeedbackModPointer(&(m_mod_destinations.flanger.feedback));
		m_flanger[stereo].setDryWetModPointer(&(m_mod_destinations.flanger.drywet));

		m_chorus[stereo].setFreqModPointer(&(m_mod_destinations.chorus.freq));
		m_chorus[stereo].setAmountModPointer(&(m_mod_destinations.chorus.amount));
		m_chorus[stereo].setFeedbackModPointer(&(m_mod_destinations.chorus.feedback));
		m_chorus[stereo].setDryWetModPointer(&(m_mod_destinations.chorus.drywet));

		m_master_mod = &(m_mod_destinations.misc.master);
	}
	m_phaser.setRateModPointer(&(m_mod_destinations.phaser.rate));
	m_phaser.setAmountModPointer(&(m_mod_destinations.phaser.amount));
	m_phaser.setDryWetModPointer(&(m_mod_destinations.phaser.drywet));
	m_phaser.setFreqModPointer(&(m_mod_destinations.phaser.freq));
	m_phaser.setFeedbackModPointer(&(m_mod_destinations.phaser.feedback));
}
