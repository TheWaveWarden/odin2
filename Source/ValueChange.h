#define DEBUG_VARIABLES

float valueToDenominator(int p_value) {
	switch (p_value) {
	case 0:
		return 1.f;
		break;
	case 1:
		return 2.f;
		break;
	case 2:
		return 4.f;
		break;
	case 3:
		return 8.f;
		break;
	case 4:
		return 12.f;
		break;
	case 5:
		return 16.f;
		break;
	case 6:
		return 24.f;
		break;
	case 7:
		return 32.f;
		break;
	case 8:
		return 48.f;
		break;
	default:
		return 16.f;
		break;
	}
	return 16.f;
}

void OdinAudioProcessor::treeValueChangedOscPitch(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_pitch: " + p_ID + ": " + std::to_string(p_new_value));
#endif

	StringRef id = StringRef(p_ID);

	if (id == m_osc1_oct_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setOctave(p_new_value, 0);
		}
	} else if (id == m_osc2_oct_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setOctave(p_new_value, 1);
		}
	} else if (id == m_osc3_oct_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setOctave(p_new_value, 2);
		}
	} else if (id == m_osc1_semi_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setSemitones(p_new_value, 0);
		}
	} else if (id == m_osc2_semi_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setSemitones(p_new_value, 1);
		}
	} else if (id == m_osc3_semi_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setSemitones(p_new_value, 2);
		}
	} else if (id == m_osc1_fine_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setFinetune(p_new_value, 0);
		}
	} else if (id == m_osc2_fine_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setFinetune(p_new_value, 1);
		}
	} else if (id == m_osc3_fine_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setFinetune(p_new_value, 2);
		}
	}
}

void OdinAudioProcessor::treeValueChangedOscMisc(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_misc: " + p_ID + ": " + std::to_string(p_new_value));
#endif

	StringRef id = StringRef(p_ID);
	if (id == m_osc1_reset_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setReset(p_new_value, 0);
		}
	} else if (id == m_osc2_reset_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setReset(p_new_value, 1);
		}
	} else if (id == m_osc3_reset_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setReset(p_new_value, 2);
		}
	} else if (id == m_osc2_sync_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setOscSyncEnabled(p_new_value, 1);
		}
	} else if (id == m_osc3_sync_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setOscSyncEnabled(p_new_value, 2);
		}
	} else if (id == m_osc1_vol_identifier) {
		m_osc_vol_control[0] = Decibels::decibelsToGain(p_new_value, -59.99f);
	} else if (id == m_osc2_vol_identifier) {
		m_osc_vol_control[1] = Decibels::decibelsToGain(p_new_value, -59.99f);
	} else if (id == m_osc3_vol_identifier) {
		m_osc_vol_control[2] = Decibels::decibelsToGain(p_new_value, -59.99f);
	}
}

void OdinAudioProcessor::treeValueChangedOscAnalog(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_analog: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);
	if (id == m_osc1_pulsewidth_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[0].setPWMDuty(p_new_value);
		}
	} else if (id == m_osc2_pulsewidth_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[1].setPWMDuty(p_new_value);
		}
	} else if (id == m_osc3_pulsewidth_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[2].setPWMDuty(p_new_value);
		}
	} else if (id == m_osc1_drift_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[0].setDrift(p_new_value);
		}
	} else if (id == m_osc2_drift_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[1].setDrift(p_new_value);
		}
	} else if (id == m_osc3_drift_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[2].setDrift(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedOscMulti(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_multi: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_osc1_position_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[0].setPosition(p_new_value);
		}
	} else if (id == m_osc2_position_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[1].setPosition(p_new_value);
		}
	} else if (id == m_osc3_position_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[2].setPosition(p_new_value);
		}
	} else if (id == m_osc1_detune_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[0].setDetune(p_new_value);
		}
	} else if (id == m_osc2_detune_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[1].setDetune(p_new_value);
		}
	} else if (id == m_osc3_detune_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[2].setDetune(p_new_value);
		}
	} else if (id == m_osc1_multi_position_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[0].setPosition(p_new_value);
		}
	} else if (id == m_osc2_multi_position_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[1].setPosition(p_new_value);
		}
	} else if (id == m_osc3_multi_position_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[2].setPosition(p_new_value);
		}
	} else if (id == m_osc1_spread_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[0].setWavetableMultiSpread(p_new_value);
		}
	} else if (id == m_osc2_spread_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[1].setWavetableMultiSpread(p_new_value);
		}
	} else if (id == m_osc3_spread_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].multi_osc[2].setWavetableMultiSpread(p_new_value);
		}
	} else if (id == m_osc1_pos_env_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[0].setPosModAmount(p_new_value);
		}
	} else if (id == m_osc2_pos_env_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[1].setPosModAmount(p_new_value);
		}
	} else if (id == m_osc3_pos_env_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[2].setPosModAmount(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedOscXY(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_xy: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_osc1_vec_x_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[0].setX(p_new_value);
		}
	} else if (id == m_osc2_vec_x_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[1].setX(p_new_value);
		}
	} else if (id == m_osc3_vec_x_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[2].setX(p_new_value);
		}
	} else if (id == m_osc1_vec_y_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[0].setY(p_new_value);
		}
	} else if (id == m_osc2_vec_y_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[1].setY(p_new_value);
		}
	} else if (id == m_osc3_vec_y_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[2].setY(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedOscChip(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_chip: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_osc1_chipnoise_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].setNoiseEnabled(p_new_value);
		}
	} else if (id == m_osc2_chipnoise_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].setNoiseEnabled(p_new_value);
		}
	} else if (id == m_osc3_chipnoise_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].setNoiseEnabled(p_new_value);
		}
	} else if (id == m_osc1_arp_on_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].setArpEnabled(p_new_value);
		}
	} else if (id == m_osc2_arp_on_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].setArpEnabled(p_new_value);
		}
	} else if (id == m_osc3_arp_on_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].setArpEnabled(p_new_value);
		}
	} else if (id == m_osc1_arp_speed_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].setArpSpeed(p_new_value);
		}
	} else if (id == m_osc2_arp_speed_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].setArpSpeed(p_new_value);
		}
	} else if (id == m_osc3_arp_speed_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].setArpSpeed(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedOscArp(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_arp: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_osc1_step_1_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].setArpSemitone(0, p_new_value);
		}
	} else if (id == m_osc2_step_1_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].setArpSemitone(0, p_new_value);
		}
	} else if (id == m_osc3_step_1_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].setArpSemitone(0, p_new_value);
		}
	} else if (id == m_osc1_step_2_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].setArpSemitone(1, p_new_value);
		}
	} else if (id == m_osc2_step_2_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].setArpSemitone(1, p_new_value);
		}
	} else if (id == m_osc3_step_2_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].setArpSemitone(1, p_new_value);
		}
	} else if (id == m_osc1_step_3_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].setArpSemitone(2, p_new_value);
		}
	} else if (id == m_osc2_step_3_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].setArpSemitone(2, p_new_value);
		}
	} else if (id == m_osc3_step_3_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].setArpSemitone(2, p_new_value);
		}
	} else if (id == m_osc1_step_3_on_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].setArpStepThreeOn(p_new_value);
		}
	} else if (id == m_osc2_step_3_on_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].setArpStepThreeOn(p_new_value);
		}
	} else if (id == m_osc3_step_3_on_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].setArpStepThreeOn(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedOscFM(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_fm: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_osc1_fm_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[0].setFMAmount(p_new_value);
			m_voice[voice].pm_osc[0].setPMAmount(p_new_value);
		}
	} else if (id == m_osc2_fm_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[1].setFMAmount(p_new_value);
			m_voice[voice].pm_osc[1].setPMAmount(p_new_value);
		}
	} else if (id == m_osc3_fm_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[2].setFMAmount(p_new_value);
			m_voice[voice].pm_osc[2].setPMAmount(p_new_value);
		}
	} else if (id == m_osc1_exp_fm_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[0].setFMExponential(p_new_value);
		}
	} else if (id == m_osc2_exp_fm_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[1].setFMExponential(p_new_value);
		}
	} else if (id == m_osc3_exp_fm_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[2].setFMExponential(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedOscNoise(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("osc_noise: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_osc1_hp_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].noise_osc[0].setHPFreq(p_new_value);
		}
	} else if (id == m_osc2_hp_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].noise_osc[1].setHPFreq(p_new_value);
		}
	} else if (id == m_osc3_hp_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].noise_osc[2].setHPFreq(p_new_value);
		}
	} else if (id == m_osc1_lp_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].noise_osc[0].setLPFreq(p_new_value);
		}
	} else if (id == m_osc2_lp_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].noise_osc[1].setLPFreq(p_new_value);
		}
	} else if (id == m_osc3_lp_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].noise_osc[2].setLPFreq(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedFilTop(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("fil_top: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_fil1_kbd_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setKbd(p_new_value, 0);
		}
	} else if (id == m_fil2_kbd_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setKbd(p_new_value, 1);
		}
	} else if (id == m_fil1_vel_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setVelModAmount(p_new_value, 0);
		}
	} else if (id == m_fil2_vel_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setVelModAmount(p_new_value, 1);
		}
	} else if (id == m_fil1_env_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setEnvModAmount(p_new_value, 0);
		}
	} else if (id == m_fil2_env_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setEnvModAmount(p_new_value, 1);
		}
	} else if (id == m_fil3_kbd_identifier) {
		m_ladder_filter[0].m_kbd_mod_amount = p_new_value;
		m_SEM_filter_12[0].m_kbd_mod_amount = p_new_value;
		m_korg_filter[0].m_kbd_mod_amount   = p_new_value;
		m_diode_filter[0].m_kbd_mod_amount  = p_new_value;
		m_comb_filter[0].m_kbd_mod_amount   = p_new_value;
		m_ring_mod[0].m_kbd_mod_amount      = p_new_value;
		m_ladder_filter[1].m_kbd_mod_amount = p_new_value;
		m_SEM_filter_12[1].m_kbd_mod_amount = p_new_value;
		m_korg_filter[1].m_kbd_mod_amount   = p_new_value;
		m_diode_filter[1].m_kbd_mod_amount  = p_new_value;
		m_comb_filter[1].m_kbd_mod_amount   = p_new_value;
		m_ring_mod[1].m_kbd_mod_amount      = p_new_value;
	} else if (id == m_fil3_vel_identifier) {
		m_ladder_filter[0].m_vel_mod_amount  = p_new_value;
		m_SEM_filter_12[0].m_vel_mod_amount  = p_new_value;
		m_korg_filter[0].m_vel_mod_amount    = p_new_value;
		m_diode_filter[0].m_vel_mod_amount   = p_new_value;
		m_comb_filter[0].m_vel_mod_amount    = p_new_value;
		m_ring_mod[0].m_vel_mod_amount       = p_new_value;
		m_formant_filter[0].m_vel_mod_amount = p_new_value;
		m_ladder_filter[1].m_vel_mod_amount  = p_new_value;
		m_SEM_filter_12[1].m_vel_mod_amount  = p_new_value;
		m_korg_filter[1].m_vel_mod_amount    = p_new_value;
		m_diode_filter[1].m_vel_mod_amount   = p_new_value;
		m_comb_filter[1].m_vel_mod_amount    = p_new_value;
		m_ring_mod[1].m_vel_mod_amount       = p_new_value;
		m_formant_filter[1].m_vel_mod_amount = p_new_value;
	} else if (id == m_fil3_env_identifier) {
		m_ladder_filter[0].m_env_mod_amount  = p_new_value;
		m_SEM_filter_12[0].m_env_mod_amount  = p_new_value;
		m_korg_filter[0].m_env_mod_amount    = p_new_value;
		m_diode_filter[0].m_env_mod_amount   = p_new_value;
		m_comb_filter[0].m_env_mod_amount    = p_new_value;
		m_ring_mod[0].m_env_mod_amount       = p_new_value;
		m_formant_filter[0].m_env_mod_amount = p_new_value;
		m_ladder_filter[1].m_env_mod_amount  = p_new_value;
		m_SEM_filter_12[1].m_env_mod_amount  = p_new_value;
		m_korg_filter[1].m_env_mod_amount    = p_new_value;
		m_diode_filter[1].m_env_mod_amount   = p_new_value;
		m_comb_filter[1].m_env_mod_amount    = p_new_value;
		m_ring_mod[1].m_env_mod_amount       = p_new_value;
		m_formant_filter[1].m_env_mod_amount = p_new_value;
	} else if (id == m_fil1_gain_identifier) {
		m_fil_gain_control[0] = Decibels::decibelsToGain(p_new_value, -59.99f);
	} else if (id == m_fil2_gain_identifier) {
		m_fil_gain_control[1] = Decibels::decibelsToGain(p_new_value, -59.99f);
	} else if (id == m_fil3_gain_identifier) {
		m_fil_gain_control[2] = Decibels::decibelsToGain(p_new_value, -59.99f);
	}
}

void OdinAudioProcessor::treeValueChangedFilCenter(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("fil_center: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_fil1_freq_identifier) {
		m_fil_freq_control[0] = p_new_value;
	} else if (id == m_fil2_freq_identifier) {
		m_fil_freq_control[1] = p_new_value;
	} else if (id == m_fil1_res_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setFilterRes(p_new_value, 0);
		}
	} else if (id == m_fil2_res_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setFilterRes(p_new_value, 1);
		}
	} else if (id == m_fil1_saturation_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setSaturation(p_new_value * 2, 0);
		}
	} else if (id == m_fil2_saturation_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setSaturation(p_new_value * 2, 1);
		}
	} else if (id == m_fil3_freq_identifier) {
		m_fil_freq_control[2] = p_new_value;
	} else if (id == m_fil3_res_identifier) {
		m_ladder_filter[0].setResControl(p_new_value);
		m_SEM_filter_12[0].setResControl(p_new_value);
		m_korg_filter[0].setResControl(p_new_value);
		m_diode_filter[0].setResControl(p_new_value);
		m_comb_filter[0].setResonance(p_new_value);
		m_ladder_filter[1].setResControl(p_new_value);
		m_SEM_filter_12[1].setResControl(p_new_value);
		m_korg_filter[1].setResControl(p_new_value);
		m_diode_filter[1].setResControl(p_new_value);
		m_comb_filter[1].setResonance(p_new_value);
	} else if (id == m_fil3_saturation_identifier) {
		m_ladder_filter[0].m_overdrive = p_new_value * 2;
		m_SEM_filter_12[0].m_overdrive = p_new_value * 2;
		m_korg_filter[0].m_overdrive   = p_new_value * 2;
		m_diode_filter[0].m_overdrive  = p_new_value * 2;
		m_ladder_filter[1].m_overdrive = p_new_value * 2;
		m_SEM_filter_12[1].m_overdrive = p_new_value * 2;
		m_korg_filter[1].m_overdrive   = p_new_value * 2;
		m_diode_filter[1].m_overdrive  = p_new_value * 2;
	}
}

void OdinAudioProcessor::treeValueChangedFilMisc(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("fil_misc: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_fil1_ring_mod_amount_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].ring_mod[0].setAmount(p_new_value);
		}
	} else if (id == m_fil2_ring_mod_amount_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].ring_mod[1].setAmount(p_new_value);
		}
	} else if (id == m_fil1_sem_transition_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].SEM_filter_12[0].m_transition = p_new_value;
		}
	} else if (id == m_fil2_sem_transition_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].SEM_filter_12[1].m_transition = p_new_value;
		}
	} else if (id == m_fil3_sem_transition_identifier) {
		m_SEM_filter_12[0].m_transition = p_new_value;
		m_SEM_filter_12[1].m_transition = p_new_value;
	}

	else if (id == m_fil3_ring_mod_amount_identifier) {
		m_ring_mod[0].setAmount(p_new_value);
		m_ring_mod[1].setAmount(p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedFilFormant(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("fil_formant: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_fil1_formant_transition_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].formant_filter[0].setTransition(p_new_value);
		}
	} else if (id == m_fil2_formant_transition_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].formant_filter[1].setTransition(p_new_value);
		}
	} else if (id == m_fil3_formant_transition_identifier) {
		m_formant_filter[0].setTransition(p_new_value);
		m_formant_filter[1].setTransition(p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedAmp(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("Amp: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_amp_pan_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].amp.setPan(p_new_value);
		}
	} else if (id == m_amp_gain_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].amp.setGainDecibels(p_new_value);
		}
	} else if (id == m_amp_velocity_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].amp.setVelocityAmount(p_new_value);
		}
	} else if (id == m_dist_drywet_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].distortion[0].setDryWet(p_new_value);
			m_voice[voice].distortion[1].setDryWet(p_new_value);
		}
	} else if (id == m_dist_threshold_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].distortion[0].setThreshold(p_new_value);
			m_voice[voice].distortion[1].setThreshold(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedDelay(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("Delay: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_delay_time_identifier) {
		m_delay.setDelayTime(p_new_value);
	} else if (id == m_delay_feedback_identifier) {
		m_delay.setFeedback(p_new_value);
	} else if (id == m_delay_hp_identifier) {
		m_delay.setHPFreq(p_new_value);
	} else if (id == m_delay_ducking_identifier) {
		m_delay.setDucking(p_new_value);
	} else if (id == m_delay_dry_identifier) {
		m_delay.setDry(p_new_value);
	} else if (id == m_delay_wet_identifier) {
		m_delay.setWet(p_new_value);
	} else if (id == m_delay_on_identifier) {
		m_delay.reset();
	} else if (id == m_delay_pingpong_identifier) {
		m_delay.setPingPong(*m_delay_pingpong > 0.5f);
	}
}

void OdinAudioProcessor::treeValueChangedChorus(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("Chorus: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_chorus_amount_identifier) {
		m_chorus[0].setAmount(p_new_value);
		m_chorus[1].setAmount(p_new_value);
	} else if (id == m_chorus_rate_identifier) {
		m_chorus[0].setLFOFreq(p_new_value);
		m_chorus[1].setLFOFreq(p_new_value);
	} else if (id == m_chorus_feedback_identifier) {
		m_chorus[0].setFeedback(p_new_value);
		m_chorus[1].setFeedback(p_new_value);
	} else if (id == m_chorus_drywet_identifier) {
		m_chorus[0].setDryWet(p_new_value);
		m_chorus[1].setDryWet(p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedPhaser(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("Phaser: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_phaser_mod_identifier) {
		m_phaser.setLFOAmplitude(p_new_value);
	} else if (id == m_phaser_feedback_identifier) {
		m_phaser.setFeedback(p_new_value);
	} else if (id == m_phaser_freq_identifier) {
		m_phaser.setBaseFreq(p_new_value);
	} else if (id == m_phaser_rate_identifier) {
		m_phaser.setLFOFreq(p_new_value);
	} else if (id == m_phaser_drywet_identifier) {
		m_phaser.setDryWet(p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedArp(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("Arp: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_arp_on_identifier) {
		m_arpeggiator.reset();
		allNotesOff();
		m_arpeggiator_on = p_new_value > 0.5f;
		m_voice_manager.setSustainActive(false);
		m_arpeggiator.setSustainActive(false);
		m_step_led_active.set(-1);
	} else if (id == m_arp_one_shot_identifier) {
		m_arpeggiator.setOneShotEnabled(p_new_value > 0.5f);
	} else if (id == m_step_0_on_identifier) {
		m_arpeggiator.setSequenceStepActive(0, p_new_value > 0.5f);
	} else if (id == m_step_1_on_identifier) {
		m_arpeggiator.setSequenceStepActive(1, p_new_value > 0.5f);
	} else if (id == m_step_2_on_identifier) {
		m_arpeggiator.setSequenceStepActive(2, p_new_value > 0.5f);
	} else if (id == m_step_3_on_identifier) {
		m_arpeggiator.setSequenceStepActive(3, p_new_value > 0.5f);
	} else if (id == m_step_4_on_identifier) {
		m_arpeggiator.setSequenceStepActive(4, p_new_value > 0.5f);
	} else if (id == m_step_5_on_identifier) {
		m_arpeggiator.setSequenceStepActive(5, p_new_value > 0.5f);
	} else if (id == m_step_6_on_identifier) {
		m_arpeggiator.setSequenceStepActive(6, p_new_value > 0.5f);
	} else if (id == m_step_7_on_identifier) {
		m_arpeggiator.setSequenceStepActive(7, p_new_value > 0.5f);
	} else if (id == m_step_8_on_identifier) {
		m_arpeggiator.setSequenceStepActive(8, p_new_value > 0.5f);
	} else if (id == m_step_9_on_identifier) {
		m_arpeggiator.setSequenceStepActive(9, p_new_value > 0.5f);
	} else if (id == m_step_10_on_identifier) {
		m_arpeggiator.setSequenceStepActive(10, p_new_value > 0.5f);
	} else if (id == m_step_11_on_identifier) {
		m_arpeggiator.setSequenceStepActive(11, p_new_value > 0.5f);
	} else if (id == m_step_12_on_identifier) {
		m_arpeggiator.setSequenceStepActive(12, p_new_value > 0.5f);
	} else if (id == m_step_13_on_identifier) {
		m_arpeggiator.setSequenceStepActive(13, p_new_value > 0.5f);
	} else if (id == m_step_14_on_identifier) {
		m_arpeggiator.setSequenceStepActive(14, p_new_value > 0.5f);
	} else if (id == m_step_15_on_identifier) {
		m_arpeggiator.setSequenceStepActive(15, p_new_value > 0.5f);
	}

	else if (id == m_step_0_mod_1_identifier) {
		m_arpeggiator.setStepMod1(0, p_new_value);
	} else if (id == m_step_1_mod_1_identifier) {
		m_arpeggiator.setStepMod1(1, p_new_value);
	} else if (id == m_step_2_mod_1_identifier) {
		m_arpeggiator.setStepMod1(2, p_new_value);
	} else if (id == m_step_3_mod_1_identifier) {
		m_arpeggiator.setStepMod1(3, p_new_value);
	} else if (id == m_step_4_mod_1_identifier) {
		m_arpeggiator.setStepMod1(4, p_new_value);
	} else if (id == m_step_5_mod_1_identifier) {
		m_arpeggiator.setStepMod1(5, p_new_value);
	} else if (id == m_step_6_mod_1_identifier) {
		m_arpeggiator.setStepMod1(6, p_new_value);
	} else if (id == m_step_7_mod_1_identifier) {
		m_arpeggiator.setStepMod1(7, p_new_value);
	} else if (id == m_step_8_mod_1_identifier) {
		m_arpeggiator.setStepMod1(8, p_new_value);
	} else if (id == m_step_9_mod_1_identifier) {
		m_arpeggiator.setStepMod1(9, p_new_value);
	} else if (id == m_step_10_mod_1_identifier) {
		m_arpeggiator.setStepMod1(10, p_new_value);
	} else if (id == m_step_11_mod_1_identifier) {
		m_arpeggiator.setStepMod1(11, p_new_value);
	} else if (id == m_step_12_mod_1_identifier) {
		m_arpeggiator.setStepMod1(12, p_new_value);
	} else if (id == m_step_13_mod_1_identifier) {
		m_arpeggiator.setStepMod1(13, p_new_value);
	} else if (id == m_step_14_mod_1_identifier) {
		m_arpeggiator.setStepMod1(14, p_new_value);
	} else if (id == m_step_15_mod_1_identifier) {
		m_arpeggiator.setStepMod1(15, p_new_value);
	}

	else if (id == m_step_0_mod_2_identifier) {
		m_arpeggiator.setStepMod2(0, p_new_value);
	} else if (id == m_step_1_mod_2_identifier) {
		m_arpeggiator.setStepMod2(1, p_new_value);
	} else if (id == m_step_2_mod_2_identifier) {
		m_arpeggiator.setStepMod2(2, p_new_value);
	} else if (id == m_step_3_mod_2_identifier) {
		m_arpeggiator.setStepMod2(3, p_new_value);
	} else if (id == m_step_4_mod_2_identifier) {
		m_arpeggiator.setStepMod2(4, p_new_value);
	} else if (id == m_step_5_mod_2_identifier) {
		m_arpeggiator.setStepMod2(5, p_new_value);
	} else if (id == m_step_6_mod_2_identifier) {
		m_arpeggiator.setStepMod2(6, p_new_value);
	} else if (id == m_step_7_mod_2_identifier) {
		m_arpeggiator.setStepMod2(7, p_new_value);
	} else if (id == m_step_8_mod_2_identifier) {
		m_arpeggiator.setStepMod2(8, p_new_value);
	} else if (id == m_step_9_mod_2_identifier) {
		m_arpeggiator.setStepMod2(9, p_new_value);
	} else if (id == m_step_10_mod_2_identifier) {
		m_arpeggiator.setStepMod2(10, p_new_value);
	} else if (id == m_step_11_mod_2_identifier) {
		m_arpeggiator.setStepMod2(11, p_new_value);
	} else if (id == m_step_12_mod_2_identifier) {
		m_arpeggiator.setStepMod2(12, p_new_value);
	} else if (id == m_step_13_mod_2_identifier) {
		m_arpeggiator.setStepMod2(13, p_new_value);
	} else if (id == m_step_14_mod_2_identifier) {
		m_arpeggiator.setStepMod2(14, p_new_value);
	} else if (id == m_step_15_mod_2_identifier) {
		m_arpeggiator.setStepMod2(15, p_new_value);
	}

	else if (id == m_step_0_transpose_identifier) {
		m_arpeggiator.setStepTranspose(0, p_new_value);
	} else if (id == m_step_1_transpose_identifier) {
		m_arpeggiator.setStepTranspose(1, p_new_value);
	} else if (id == m_step_2_transpose_identifier) {
		m_arpeggiator.setStepTranspose(2, p_new_value);
	} else if (id == m_step_3_transpose_identifier) {
		m_arpeggiator.setStepTranspose(3, p_new_value);
	} else if (id == m_step_4_transpose_identifier) {
		m_arpeggiator.setStepTranspose(4, p_new_value);
	} else if (id == m_step_5_transpose_identifier) {
		m_arpeggiator.setStepTranspose(5, p_new_value);
	} else if (id == m_step_6_transpose_identifier) {
		m_arpeggiator.setStepTranspose(6, p_new_value);
	} else if (id == m_step_7_transpose_identifier) {
		m_arpeggiator.setStepTranspose(7, p_new_value);
	} else if (id == m_step_8_transpose_identifier) {
		m_arpeggiator.setStepTranspose(8, p_new_value);
	} else if (id == m_step_9_transpose_identifier) {
		m_arpeggiator.setStepTranspose(9, p_new_value);
	} else if (id == m_step_10_transpose_identifier) {
		m_arpeggiator.setStepTranspose(10, p_new_value);
	} else if (id == m_step_11_transpose_identifier) {
		m_arpeggiator.setStepTranspose(11, p_new_value);
	} else if (id == m_step_12_transpose_identifier) {
		m_arpeggiator.setStepTranspose(12, p_new_value);
	} else if (id == m_step_13_transpose_identifier) {
		m_arpeggiator.setStepTranspose(13, p_new_value);
	} else if (id == m_step_14_transpose_identifier) {
		m_arpeggiator.setStepTranspose(14, p_new_value);
	} else if (id == m_step_15_transpose_identifier) {
		m_arpeggiator.setStepTranspose(15, p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedFlanger(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("Flanger: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_flanger_amount_identifier) {
		m_flanger[0].setLFOAmount(p_new_value);
		m_flanger[1].setLFOAmount(p_new_value);
	} else if (id == m_flanger_rate_identifier) {
		m_flanger[0].setLFOFreq(p_new_value);
		m_flanger[1].setLFOFreq(p_new_value);
	} else if (id == m_flanger_feedback_identifier) {
		m_flanger[0].setFeedback(p_new_value);
		m_flanger[1].setFeedback(p_new_value);
	} else if (id == m_flanger_drywet_identifier) {
		m_flanger[0].setDryWet(p_new_value);
		m_flanger[1].setDryWet(p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedADSR1(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("ADSR1: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);
	if (id == m_env1_attack_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[0].setAttack(p_new_value);
		}
	} else if (id == m_env1_decay_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[0].setDecay(p_new_value);
		}
	} else if (id == m_env1_sustain_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[0].setSustain(p_new_value);
		}
	} else if (id == m_env1_release_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[0].setRelease(p_new_value);
		}
	} else if (id == m_env1_loop_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[0].setLoop(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedADSR2(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("ADSR2: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);
	if (id == m_env2_attack_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[1].setAttack(p_new_value);
		}
	} else if (id == m_env2_decay_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[1].setDecay(p_new_value);
		}
	} else if (id == m_env2_sustain_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[1].setSustain(p_new_value);
		}
	} else if (id == m_env2_release_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[1].setRelease(p_new_value);
		}
	} else if (id == m_env2_loop_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[1].setLoop(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedADSR3(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("ADSR3: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);
	if (id == m_env3_attack_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[2].setAttack(p_new_value);
		}
	} else if (id == m_env3_decay_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[2].setDecay(p_new_value);
		}
	} else if (id == m_env3_sustain_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[2].setSustain(p_new_value);
		}
	} else if (id == m_env3_release_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[2].setRelease(p_new_value);
		}
	} else if (id == m_env3_loop_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].env[2].setLoop(p_new_value);
		}
	}
}

void OdinAudioProcessor::treeValueChangedADSR4(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("ADSR4: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);
	if (id == m_env4_attack_identifier) {
		m_global_env.setAttack(p_new_value);
	} else if (id == m_env4_decay_identifier) {
		m_global_env.setDecay(p_new_value);
	} else if (id == m_env4_sustain_identifier) {
		m_global_env.setSustain(p_new_value);
	} else if (id == m_env4_release_identifier) {
		m_global_env.setRelease(p_new_value);
	} else if (id == m_env4_loop_identifier) {
		m_global_env.setLoop(p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedLFO1(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("LFO1: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_lfo1_freq_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[0].setBaseFrequency(p_new_value);
		}
	} else if (id == m_lfo1_reset_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[0].setResetActive(p_new_value > 0.5f);
		}
	}
}

void OdinAudioProcessor::treeValueChangedLFO2(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("LFO2: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_lfo2_freq_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[1].setBaseFrequency(p_new_value);
		}
	} else if (id == m_lfo2_reset_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[1].setResetActive(p_new_value > 0.5f);
		}
	}
}

void OdinAudioProcessor::treeValueChangedLFO3(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("LFO3: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_lfo3_freq_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[2].setBaseFrequency(p_new_value);
		}
	} else if (id == m_lfo3_reset_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[2].setResetActive(p_new_value > 0.5f);
		}
	}
}

void OdinAudioProcessor::treeValueChangedLFO4(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("LFO4: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_lfo4_freq_identifier) {
		m_global_lfo.setBaseFrequency(p_new_value);
	} else if (id == m_lfo4_reset_identifier) {
		m_global_lfo.setResetActive(p_new_value > 0.5f);
	}
}

// void OdinAudioProcessor::treeValueChangedAmount1(const String &p_ID, float p_new_value) {
// #ifdef DEBUG_VARIABLES
// 	DBG("Amount1: " + p_ID + ": " + std::to_string(p_new_value));
// #endif
// 	StringRef id = StringRef(p_ID);

//
// }

void OdinAudioProcessor::treeValueChangedGeneralMisc(const String &p_ID, float p_new_value) {
#ifdef DEBUG_VARIABLES
	DBG("Misc: " + p_ID + ": " + std::to_string(p_new_value));
#endif
	StringRef id = StringRef(p_ID);

	if (id == m_glide_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setGlide(p_new_value);
		}
	} else if (id == m_master_identifier) {
		m_master_control = Decibels::decibelsToGain(p_new_value);
	} else if (id == m_unison_width_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setUnisonPanAmount(p_new_value);
		}
	} else if (id == m_unison_detune_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setUnisonDetuneAmount(p_new_value);
		}
	}
}

// void OdinAudioProcessor::treeValueChangedNonParam(ValueTree &tree, const Identifier &id) {

// 	float p_new_value = (float)tree[id];
// 	DBG("DELETE ME: " + id.toString().toStdString() + ": " + std::to_string(p_new_value));
// }

void OdinAudioProcessor::treeValueChangedNonParamFX(ValueTree &tree, const Identifier &id) {

	float p_new_value = (float)tree[id];
#ifdef DEBUG_VARIABLES
	DBG("nonparam_fx: " + id.toString().toStdString() + ": " + std::to_string(p_new_value));
#endif
	if (id == m_flanger_synctime_numerator_identifier) {
		for (int stereo = 0; stereo < 2; ++stereo) {
			m_flanger[stereo].setSynctimeNumerator(p_new_value + 1);
		}
	} else if (id == m_flanger_synctime_denominator_identifier) {
		for (int stereo = 0; stereo < 2; ++stereo) {
			m_flanger[stereo].setSynctimeDenominator(valueToDenominator(p_new_value));
		}
	} else if (id == m_chorus_synctime_numerator_identifier) {
		for (int stereo = 0; stereo < 2; ++stereo) {
			m_chorus[stereo].setSynctimeNumerator(p_new_value + 1);
		}
	} else if (id == m_chorus_synctime_denominator_identifier) {
		for (int stereo = 0; stereo < 2; ++stereo) {
			m_chorus[stereo].setSynctimeDenominator(valueToDenominator(p_new_value));
		}
	} else if (id == m_delay_synctime_numerator_identifier) {
		m_delay.setSynctimeNumerator(p_new_value + 1);
	} else if (id == m_delay_synctime_denominator_identifier) {
		m_delay.setSynctimeDenominator(valueToDenominator(p_new_value));
	} else if (id == m_phaser_synctime_numerator_identifier) {
		m_phaser.setSynctimeNumerator(p_new_value + 1);
	} else if (id == m_phaser_synctime_denominator_identifier) {
		m_phaser.setSynctimeDenominator(valueToDenominator(p_new_value));
	} else if (id == m_delay_sync_identifier) {
		m_delay_sync = p_new_value > 0.5;
		if (!p_new_value) {
			m_delay.setDelayTime(*m_delay_time);
		}
	} else if (id == m_chorus_sync_identifier) {
		m_chorus_sync = p_new_value > 0.5;
		if (!p_new_value) {
			m_chorus[0].setLFOFreq(*m_chorus_rate);
			m_chorus[1].setLFOFreq(*m_chorus_rate);
		}
	} else if (id == m_flanger_sync_identifier) {
		m_flanger_sync = p_new_value > 0.5;
		if (!p_new_value) {
			m_flanger[0].setLFOFreq(*m_flanger_rate);
			m_flanger[1].setLFOFreq(*m_flanger_rate);
		}
	} else if (id == m_phaser_sync_identifier) {
		m_phaser_sync = p_new_value > 0.5;
		if (!p_new_value) {
			m_phaser.setLFOFreq(*m_phaser_rate);
		}
	}
}

void OdinAudioProcessor::treeValueChangedNonParamLFO(ValueTree &tree, const Identifier &id) {
	float p_new_value = (float)tree[id];
#ifdef DEBUG_VARIABLES
	DBG("nonparam_lfo: " + id.toString().toStdString() + ": " + std::to_string(p_new_value));
#endif
	if (id == m_lfo1_synctime_numerator_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[0].setSynctimeNumerator(p_new_value + 1);
		}
	} else if (id == m_lfo1_synctime_denominator_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[0].setSynctimeDenominator(valueToDenominator(p_new_value));
		}
	} else if (id == m_lfo2_synctime_numerator_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[1].setSynctimeNumerator(p_new_value + 1);
		}
	} else if (id == m_lfo2_synctime_denominator_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[1].setSynctimeDenominator(valueToDenominator(p_new_value));
		}
	} else if (id == m_lfo3_synctime_numerator_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[2].setSynctimeNumerator(p_new_value + 1);
		}
	} else if (id == m_lfo3_synctime_denominator_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].lfo[2].setSynctimeDenominator(valueToDenominator(p_new_value));
		}
	} else if (id == m_lfo4_synctime_numerator_identifier) {
		m_global_lfo.setSynctimeNumerator(p_new_value + 1);
	} else if (id == m_lfo4_synctime_denominator_identifier) {
		m_global_lfo.setSynctimeDenominator(valueToDenominator(p_new_value));
	}

	else if (id == m_lfo1_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			if (p_new_value < 6.5f) {
				m_voice[voice].lfo[0].selectWavetable(p_new_value);
				m_voice[voice].lfo[0].setSHActive(false);
			} else if (p_new_value > 7.5f) {
				m_voice[voice].lfo[0].selectWavetable((int)p_new_value - 1);
				m_voice[voice].lfo[0].setSHActive(false);
			} else {
				m_voice[voice].lfo[0].setSHActive(true);
			}
		}
	} else if (id == m_lfo2_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			if (p_new_value < 6.5f) {
				m_voice[voice].lfo[1].selectWavetable(p_new_value);
				m_voice[voice].lfo[1].setSHActive(false);
			} else if (p_new_value > 7.5f) {
				m_voice[voice].lfo[1].selectWavetable((int)p_new_value - 1);
				m_voice[voice].lfo[1].setSHActive(false);
			} else {
				m_voice[voice].lfo[1].setSHActive(true);
			}
		}
	} else if (id == m_lfo3_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			if (p_new_value < 6.5f) {
				m_voice[voice].lfo[2].selectWavetable(p_new_value);
				m_voice[voice].lfo[2].setSHActive(false);
			} else if (p_new_value > 7.5f) {
				m_voice[voice].lfo[2].selectWavetable((int)p_new_value - 1);
				m_voice[voice].lfo[2].setSHActive(false);
			} else {
				m_voice[voice].lfo[2].setSHActive(true);
			}
		}
	} else if (id == m_lfo4_wave_identifier) {
		if (p_new_value < 6.5f) {
			m_global_lfo.selectWavetable(p_new_value);
			m_global_lfo.setSHActive(false);
		} else if (p_new_value > 7.5f) {
			m_global_lfo.selectWavetable((int)p_new_value - 1);
			m_global_lfo.setSHActive(false);
		} else {
			m_global_lfo.setSHActive(true);
		}
	} else if (id == m_lfo4_sync_identifier) {
		m_lfo4_sync = p_new_value > 0.5;
		if (!p_new_value) {
			m_global_lfo.setBaseFrequency(*m_lfo4_freq);
		}
	} else if (id == m_lfo3_sync_identifier) {
		m_lfo3_sync = p_new_value > 0.5;
		if (!p_new_value) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].lfo[2].setBaseFrequency(*m_lfo3_freq);
			}
		}
	} else if (id == m_lfo2_sync_identifier) {
		m_lfo2_sync = p_new_value > 0.5;
		if (!p_new_value) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].lfo[1].setBaseFrequency(*m_lfo2_freq);
			}
		}
	} else if (id == m_lfo1_sync_identifier) {
		m_lfo1_sync = p_new_value > 0.5;
		if (!p_new_value) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].lfo[0].setBaseFrequency(*m_lfo1_freq);
			}
		}
	}
}

void OdinAudioProcessor::treeValueChangedNonParamMod(ValueTree &tree, const Identifier &id) {
	float p_new_value = (float)tree[id];
#ifdef DEBUG_VARIABLES
	DBG("nonparam_mod: " + id.toString().toStdString() + ": " + std::to_string(p_new_value));
#endif
	if (id == m_source_row_1_identifier) {
		m_mod_matrix.setModSource(0, p_new_value);
	} else if (id == m_dest_1_row_1_identifier) {
		m_mod_matrix.setModDestination1(0, p_new_value);
	} else if (id == m_source_row_2_identifier) {
		m_mod_matrix.setModSource(1, p_new_value);
	} else if (id == m_dest_1_row_2_identifier) {
		m_mod_matrix.setModDestination1(1, p_new_value);
	} else if (id == m_source_row_3_identifier) {
		m_mod_matrix.setModSource(2, p_new_value);
	} else if (id == m_dest_1_row_3_identifier) {
		m_mod_matrix.setModDestination1(2, p_new_value);
	} else if (id == m_source_row_4_identifier) {
		m_mod_matrix.setModSource(3, p_new_value);
	} else if (id == m_dest_1_row_4_identifier) {
		m_mod_matrix.setModDestination1(3, p_new_value);
	} else if (id == m_source_row_5_identifier) {
		m_mod_matrix.setModSource(4, p_new_value);
	} else if (id == m_dest_1_row_5_identifier) {
		m_mod_matrix.setModDestination1(4, p_new_value);
	} else if (id == m_source_row_6_identifier) {
		m_mod_matrix.setModSource(5, p_new_value);
	} else if (id == m_dest_1_row_6_identifier) {
		m_mod_matrix.setModDestination1(5, p_new_value);
	} else if (id == m_source_row_7_identifier) {
		m_mod_matrix.setModSource(6, p_new_value);
	} else if (id == m_dest_1_row_7_identifier) {
		m_mod_matrix.setModDestination1(6, p_new_value);
	} else if (id == m_source_row_8_identifier) {
		m_mod_matrix.setModSource(7, p_new_value);
	} else if (id == m_dest_1_row_8_identifier) {
		m_mod_matrix.setModDestination1(7, p_new_value);
	} else if (id == m_source_row_9_identifier) {
		m_mod_matrix.setModSource(8, p_new_value);
	} else if (id == m_dest_1_row_9_identifier) {
		m_mod_matrix.setModDestination1(8, p_new_value);
	} else if (id == m_dest_2_row_1_identifier) {
		m_mod_matrix.setModDestination2(0, p_new_value);
	} else if (id == m_dest_2_row_2_identifier) {
		m_mod_matrix.setModDestination2(1, p_new_value);
	} else if (id == m_dest_2_row_3_identifier) {
		m_mod_matrix.setModDestination2(2, p_new_value);
	} else if (id == m_dest_2_row_4_identifier) {
		m_mod_matrix.setModDestination2(3, p_new_value);
	} else if (id == m_dest_2_row_5_identifier) {
		m_mod_matrix.setModDestination2(4, p_new_value);
	} else if (id == m_dest_2_row_6_identifier) {
		m_mod_matrix.setModDestination2(5, p_new_value);
	} else if (id == m_dest_2_row_7_identifier) {
		m_mod_matrix.setModDestination2(6, p_new_value);
	} else if (id == m_dest_2_row_8_identifier) {
		m_mod_matrix.setModDestination2(7, p_new_value);
	} else if (id == m_dest_2_row_9_identifier) {
		m_mod_matrix.setModDestination2(8, p_new_value);
	}

	else if (id == m_scale_row_1_identifier) {
		m_mod_matrix.setModScale(0, p_new_value);
	} else if (id == m_scale_row_2_identifier) {
		m_mod_matrix.setModScale(1, p_new_value);
	} else if (id == m_scale_row_3_identifier) {
		m_mod_matrix.setModScale(2, p_new_value);
	} else if (id == m_scale_row_4_identifier) {
		m_mod_matrix.setModScale(3, p_new_value);
	} else if (id == m_scale_row_5_identifier) {
		m_mod_matrix.setModScale(4, p_new_value);
	} else if (id == m_scale_row_6_identifier) {
		m_mod_matrix.setModScale(5, p_new_value);
	} else if (id == m_scale_row_7_identifier) {
		m_mod_matrix.setModScale(6, p_new_value);
	} else if (id == m_scale_row_8_identifier) {
		m_mod_matrix.setModScale(7, p_new_value);
	} else if (id == m_scale_row_9_identifier) {
		m_mod_matrix.setModScale(8, p_new_value);
	}

	else if (id == m_amount_1_row_1_identifier) {
		m_mod_matrix.setModAmount1(0, p_new_value);
	} else if (id == m_amount_1_row_2_identifier) {
		m_mod_matrix.setModAmount1(1, p_new_value);
	} else if (id == m_amount_1_row_3_identifier) {
		m_mod_matrix.setModAmount1(2, p_new_value);
	} else if (id == m_amount_1_row_4_identifier) {
		m_mod_matrix.setModAmount1(3, p_new_value);
	} else if (id == m_amount_1_row_5_identifier) {
		m_mod_matrix.setModAmount1(4, p_new_value);
	} else if (id == m_amount_1_row_6_identifier) {
		m_mod_matrix.setModAmount1(5, p_new_value);
	} else if (id == m_amount_1_row_7_identifier) {
		m_mod_matrix.setModAmount1(6, p_new_value);
	} else if (id == m_amount_1_row_8_identifier) {
		m_mod_matrix.setModAmount1(7, p_new_value);
	} else if (id == m_amount_1_row_9_identifier) {
		m_mod_matrix.setModAmount1(8, p_new_value);
	}

	else if (id == m_amount_2_row_1_identifier) {
		m_mod_matrix.setModAmount2(0, p_new_value);
	} else if (id == m_amount_2_row_2_identifier) {
		m_mod_matrix.setModAmount2(1, p_new_value);
	} else if (id == m_amount_2_row_3_identifier) {
		m_mod_matrix.setModAmount2(2, p_new_value);
	} else if (id == m_amount_2_row_4_identifier) {
		m_mod_matrix.setModAmount2(3, p_new_value);
	} else if (id == m_amount_2_row_5_identifier) {
		m_mod_matrix.setModAmount2(4, p_new_value);
	} else if (id == m_amount_2_row_6_identifier) {
		m_mod_matrix.setModAmount2(5, p_new_value);
	} else if (id == m_amount_2_row_7_identifier) {
		m_mod_matrix.setModAmount2(6, p_new_value);
	} else if (id == m_amount_2_row_8_identifier) {
		m_mod_matrix.setModAmount2(7, p_new_value);
	} else if (id == m_amount_2_row_9_identifier) {
		m_mod_matrix.setModAmount2(8, p_new_value);
	} else if (id == m_amount_3_row_1_identifier) {
		m_mod_matrix.setScaleAmount(0, p_new_value);
	} else if (id == m_amount_3_row_2_identifier) {
		m_mod_matrix.setScaleAmount(1, p_new_value);
	} else if (id == m_amount_3_row_3_identifier) {
		m_mod_matrix.setScaleAmount(2, p_new_value);
	} else if (id == m_amount_3_row_4_identifier) {
		m_mod_matrix.setScaleAmount(3, p_new_value);
	} else if (id == m_amount_3_row_5_identifier) {
		m_mod_matrix.setScaleAmount(4, p_new_value);
	} else if (id == m_amount_3_row_6_identifier) {
		m_mod_matrix.setScaleAmount(5, p_new_value);
	} else if (id == m_amount_3_row_7_identifier) {
		m_mod_matrix.setScaleAmount(6, p_new_value);
	} else if (id == m_amount_3_row_8_identifier) {
		m_mod_matrix.setScaleAmount(7, p_new_value);
	} else if (id == m_amount_3_row_9_identifier) {
		m_mod_matrix.setScaleAmount(8, p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedNonParamMisc(ValueTree &tree, const Identifier &id) {
	float p_new_value = (float)tree[id];
#ifdef DEBUG_VARIABLES
	DBG("nonparam_misc: " + id.toString().toStdString() + ": " + std::to_string(p_new_value));
#endif
	if (id == m_dist_algo_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].distortion[0].setAlgorithm((int)p_new_value);
			m_voice[voice].distortion[1].setAlgorithm((int)p_new_value);
		}
	} else if (id == m_fil1_type_identifier) {
		m_fil_type[0] = p_new_value;
		if (p_new_value < 7.5f && p_new_value > 1.5f) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].ladder_filter[0].setFilterType((int)p_new_value - 2);
			}
		} else if ((int)p_new_value == FILTER_TYPE_KORG_LP) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].korg_filter[0].setFilterType(true);
			}
		} else if ((int)p_new_value == FILTER_TYPE_KORG_HP) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].korg_filter[0].setFilterType(false);
			}
		}
	} else if (id == m_fil2_type_identifier) {
		m_fil_type[1] = p_new_value;

		if (p_new_value < 7.5f && p_new_value > 1.5f) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].ladder_filter[1].setFilterType((int)p_new_value - 2);
			}
		} else if ((int)p_new_value == FILTER_TYPE_KORG_LP) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].korg_filter[1].setFilterType(true);
			}
		} else if ((int)p_new_value == FILTER_TYPE_KORG_HP) {
			for (int voice = 0; voice < VOICES; ++voice) {
				m_voice[voice].korg_filter[1].setFilterType(false);
			}
		}
	} else if (id == m_fil3_type_identifier) {
		m_fil_type[2] = p_new_value;
		if (p_new_value < 7.5f && p_new_value > 1.5f) {
			m_ladder_filter[0].setFilterType((int)p_new_value - 2);
			m_ladder_filter[1].setFilterType((int)p_new_value - 2);
		} else if ((int)p_new_value == FILTER_TYPE_KORG_LP) {
			m_korg_filter[0].setFilterType(true);
			m_korg_filter[1].setFilterType(true);
		} else if ((int)p_new_value == FILTER_TYPE_KORG_HP) {
			m_korg_filter[0].setFilterType(false);
			m_korg_filter[1].setFilterType(false);
		} else if (id == m_BPM_identifier) {
			m_BPM = p_new_value;
		}
	} else if (id == m_dist_on_identifier) {
		m_dist_on = p_new_value > 0.5f;
	} else if (id == m_fil1_comb_polarity_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].comb_filter[0].setPositive(!((bool)p_new_value));
		}
	} else if (id == m_fil2_comb_polarity_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].comb_filter[1].setPositive(!((bool)p_new_value));
		}
	} else if (id == m_fil3_comb_polarity_identifier) {
		m_comb_filter[0].setPositive(!((bool)p_new_value));
		m_comb_filter[1].setPositive(!((bool)p_new_value));
	} else if (id == m_fil1_vowel_left_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].formant_filter[0].setVowelLeft((int)p_new_value);
		}
	} else if (id == m_fil2_vowel_left_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].formant_filter[1].setVowelLeft((int)p_new_value);
		}
	} else if (id == m_fil1_vowel_right_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].formant_filter[0].setVowelRight((int)p_new_value);
		}
	} else if (id == m_fil2_vowel_right_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].formant_filter[1].setVowelRight((int)p_new_value - 1);
		}
	} else if (id == m_fil3_vowel_left_identifier) {
		m_formant_filter[0].setVowelLeft((int)p_new_value);
		m_formant_filter[1].setVowelLeft((int)p_new_value);
	} else if (id == m_fil3_vowel_right_identifier) {
		m_formant_filter[0].setVowelRight((int)p_new_value);
		m_formant_filter[1].setVowelRight((int)p_new_value);
	} else if (id == m_pitchbend_amount_identifier) {
		m_pitchbend_amount = p_new_value;
	}

	if (id == m_arp_synctime_numerator_identifier) {
		m_arpeggiator.setSynctimeNumerator(p_new_value + 1);
	} else if (id == m_arp_synctime_denominator_identifier) {
		m_arpeggiator.setSynctimeDenominator(valueToDenominator(p_new_value));
	} else if (id == m_arp_octaves_identifier) {
		m_arpeggiator.setOctaves(p_new_value);
	} else if (id == m_arp_direction_identifier) {
		m_arpeggiator.setDirection(p_new_value);
	} else if (id == m_arp_steps_identifier) {
		m_arpeggiator.setSteps(p_new_value);
	} else if (id == m_arp_gate_identifier) {
		m_arpeggiator.setGatePercent(p_new_value);
	}
}

void OdinAudioProcessor::treeValueChangedNonParamOsc(ValueTree &tree, const Identifier &id) {
	float p_new_value = (float)tree[id];
#ifdef DEBUG_VARIABLES
	DBG("nonparam_osc: " + id.toString().toStdString() + ": " + std::to_string(p_new_value));
#endif
	if (id == m_osc1_analog_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[0].selectWavetable((int)p_new_value);
		}
	} else if (id == m_osc2_analog_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[1].selectWavetable((int)p_new_value);
		}
	} else if (id == m_osc3_analog_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].analog_osc[2].selectWavetable((int)p_new_value);
		}
	} else if (id == m_osc1_type_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].killGlide(0);
			m_osc_type[0] = p_new_value;
			// set new sync master osc
			switch (m_osc_type[0]) {
			case OSC_TYPE_ANALOG:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].analog_osc[0]));
				break;
			case OSC_TYPE_WAVETABLE:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].wavetable_osc[0]));
				break;
			case OSC_TYPE_MULTI:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].multi_osc[0]));
				break;
			case OSC_TYPE_VECTOR:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].vector_osc[0]));
				break;
			case OSC_TYPE_CHIPTUNE:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].chiptune_osc[0]));
				break;
			case OSC_TYPE_FM:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].fm_osc[0]));
				break;
			case OSC_TYPE_PM:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].pm_osc[0]));
				break;
			case OSC_TYPE_WAVEDRAW:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].wavedraw_osc[0]));
				break;
			case OSC_TYPE_CHIPDRAW:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].chipdraw_osc[0]));
				break;
			case OSC_TYPE_SPECDRAW:
				m_voice[voice].setOscSyncOscillator(&(m_voice[voice].specdraw_osc[0]));
				break;
			default:
				// default (= none or noise) set nullptr
				m_voice[voice].setOscSyncOscillator(nullptr);
				break;
			}
		}
		//check which sources to render for wavetable env mod:
		m_mod_matrix.checkWhichSourceToRender();
	} else if (id == m_osc2_type_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].killGlide(1);
			m_osc_type[1] = p_new_value;
		}
		//check which sources to render for wavetable env mod:
		m_mod_matrix.checkWhichSourceToRender();
	} else if (id == m_osc3_type_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].killGlide(2);
			m_osc_type[2] = p_new_value;
		}
		//check which sources to render for wavetable env mod:
		m_mod_matrix.checkWhichSourceToRender();
	} else if (id == m_osc1_wavetable_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[0].selectWavetable((int)p_new_value - 1);
			m_voice[voice].multi_osc[0].selectWavetable((int)p_new_value - 1);
		}
	} else if (id == m_osc2_wavetable_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[1].selectWavetable((int)p_new_value - 1);
			m_voice[voice].multi_osc[1].selectWavetable((int)p_new_value - 1);
		}
	} else if (id == m_osc3_wavetable_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].wavetable_osc[2].selectWavetable((int)p_new_value - 1);
			m_voice[voice].multi_osc[2].selectWavetable((int)p_new_value - 1);
		}
	} else if (id == m_osc1_vec_a_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[0].selectWavetableByMappingVector(p_new_value, 0);
		}
	} else if (id == m_osc2_vec_a_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[1].selectWavetableByMappingVector(p_new_value, 0);
		}
	} else if (id == m_osc3_vec_a_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[2].selectWavetableByMappingVector(p_new_value, 0);
		}
	} else if (id == m_osc1_vec_b_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[0].selectWavetableByMappingVector(p_new_value, 1);
		}
	} else if (id == m_osc2_vec_b_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[1].selectWavetableByMappingVector(p_new_value, 1);
		}
	} else if (id == m_osc3_vec_b_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[2].selectWavetableByMappingVector(p_new_value, 1);
		}
	} else if (id == m_osc1_vec_c_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[0].selectWavetableByMappingVector(p_new_value, 2);
		}
	} else if (id == m_osc2_vec_c_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[1].selectWavetableByMappingVector(p_new_value, 2);
		}
	} else if (id == m_osc3_vec_c_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[2].selectWavetableByMappingVector(p_new_value, 2);
		}
	} else if (id == m_osc1_vec_d_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[0].selectWavetableByMappingVector(p_new_value, 3);
		}
	} else if (id == m_osc2_vec_d_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[1].selectWavetableByMappingVector(p_new_value, 3);
		}
	} else if (id == m_osc3_vec_d_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].vector_osc[2].selectWavetableByMappingVector(p_new_value, 3);
		}
	} else if (id == m_osc1_chipwave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[0].selectWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc2_chipwave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[1].selectWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc3_chipwave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].chiptune_osc[2].selectWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc1_carrier_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[0].selectCarrierWavetableByMapping(p_new_value);
			m_voice[voice].pm_osc[0].selectCarrierWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc2_carrier_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[1].selectCarrierWavetableByMapping(p_new_value);
			m_voice[voice].pm_osc[1].selectCarrierWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc3_carrier_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[2].selectCarrierWavetableByMapping(p_new_value);
			m_voice[voice].pm_osc[2].selectCarrierWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc1_modulator_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[0].selectModulatorWavetableByMapping(p_new_value);
			m_voice[voice].pm_osc[0].selectModulatorWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc2_modulator_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[1].selectModulatorWavetableByMapping(p_new_value);
			m_voice[voice].pm_osc[1].selectModulatorWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc3_modulator_wave_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[2].selectModulatorWavetableByMapping(p_new_value);
			m_voice[voice].pm_osc[2].selectModulatorWavetableByMapping(p_new_value);
		}
	} else if (id == m_osc1_modulation_source_identifier) {
		m_osc_wavetable_source_lfo[0] = (p_new_value == 10);
		//check which sources to render for wavetable pos mod:
		m_mod_matrix.checkWhichSourceToRender();
	} else if (id == m_osc2_modulation_source_identifier) {
		m_osc_wavetable_source_lfo[1] = (p_new_value == 10);
		//check which sources to render for wavetable pos mod:
		m_mod_matrix.checkWhichSourceToRender();
	} else if (id == m_osc3_modulation_source_identifier) {
		m_osc_wavetable_source_lfo[2] = (p_new_value == 10);
		//check which sources to render for wavetable pos mod:
		m_mod_matrix.checkWhichSourceToRender();
	} else if (id == m_osc1_carrier_ratio_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[0].setCarrierRatio(p_new_value);
			m_voice[voice].pm_osc[0].setCarrierRatio(p_new_value);
		}
	} else if (id == m_osc2_carrier_ratio_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[1].setCarrierRatio(p_new_value);
			m_voice[voice].pm_osc[1].setCarrierRatio(p_new_value);
		}
	} else if (id == m_osc3_carrier_ratio_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[2].setCarrierRatio(p_new_value);
			m_voice[voice].pm_osc[2].setCarrierRatio(p_new_value);
		}
	} else if (id == m_osc1_modulator_ratio_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[0].setModulatorRatio(p_new_value);
			m_voice[voice].pm_osc[0].setModulatorRatio(p_new_value);
		}
	} else if (id == m_osc2_modulator_ratio_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[1].setModulatorRatio(p_new_value);
			m_voice[voice].pm_osc[1].setModulatorRatio(p_new_value);
		}
	} else if (id == m_osc3_modulator_ratio_identifier) {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].fm_osc[2].setModulatorRatio(p_new_value);
			m_voice[voice].pm_osc[2].setModulatorRatio(p_new_value);
		}
	}
}
