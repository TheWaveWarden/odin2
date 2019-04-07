#define DEBUG_VARIABLES

void OdinAudioProcessor::treeValueChanged(const String &p_ID,
                                          float p_new_value) {

  // create identifier once, then compare it against everything
  Identifier id(p_ID);

#ifdef DEBUG_VARIABLES
  DBG(p_ID + ": " + std::to_string(p_new_value));
#endif

  if (id == m_osc1_analog_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[0].selectWavetable((int)p_new_value - 1);
    }
  }
  else if (id == m_osc2_analog_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[1].selectWavetable((int)p_new_value - 1);
    }
  }
  else if (id == m_osc3_analog_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[2].selectWavetable((int)p_new_value - 1);
    }
  }
  else if (id == m_osc1_oct_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setOctave(p_new_value, 0);
    }
  }
  else if (id == m_osc2_oct_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setOctave(p_new_value, 1);      
    }
  }
  else if (id == m_osc3_oct_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setOctave(p_new_value, 2);      
    }
  }
  else if (id == m_osc1_semi_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setSemitones(p_new_value, 0);
    }
  }
  else if (id == m_osc2_semi_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setSemitones(p_new_value, 1);      
    }
  }
  else if (id == m_osc3_semi_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setSemitones(p_new_value, 2);      
    }
  }
  else if (id == m_osc1_fine_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setFinetune(p_new_value, 0);
    }
  }
  else if (id == m_osc2_fine_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setFinetune(p_new_value, 1);      
    }
  }
  else if (id == m_osc3_fine_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setFinetune(p_new_value, 2);      
    }
  }
  else if (id == m_osc1_reset_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setReset(p_new_value, 0);
    }
  }
  else if (id == m_osc2_reset_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setReset(p_new_value, 1);      
    }
  }
  else if (id == m_osc3_reset_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].setReset(p_new_value, 2);      
    }
  }
  else if (id == m_osc1_pulsewidth_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[0].setPWMDuty(p_new_value);
    }
  }
  else if (id == m_osc2_pulsewidth_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[1].setPWMDuty(p_new_value);
    }
  }
  else if (id == m_osc3_pulsewidth_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[2].setPWMDuty(p_new_value);
    }
  }
  else if (id == m_osc1_drift_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[0].setDrift(p_new_value);
    }
  }
  else if (id == m_osc2_drift_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[1].setDrift(p_new_value);
    }
  }
  else if (id == m_osc3_drift_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[2].setDrift(p_new_value);
    }
  }
  else if (id == m_osc1_position_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].wavetable_osc[0].setPosition(p_new_value);
    }
  }
  else if (id == m_osc2_position_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].wavetable_osc[1].setPosition(p_new_value);
    }
  }
  else if (id == m_osc3_position_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].wavetable_osc[2].setPosition(p_new_value);
    }
  }
  else if (id == m_osc1_wavetable_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].wavetable_osc[0].selectWavetable(p_new_value);
    }
  }
  else if (id == m_osc2_wavetable_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].wavetable_osc[1].selectWavetable(p_new_value);
    }
  }
  else if (id == m_osc3_wavetable_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].wavetable_osc[2].selectWavetable(p_new_value);
    }
  }
  else if (id == m_osc1_detune_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[0].setDetune(p_new_value);
    }
  }
  else if (id == m_osc2_detune_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[1].setDetune(p_new_value);
    }
  }
  else if (id == m_osc3_detune_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[2].setDetune(p_new_value);
    }
  }
  else if (id == m_osc1_multi_position_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[0].setPosition(p_new_value);
    }
  }
  else if (id == m_osc2_multi_position_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[1].setPosition(p_new_value);
    }
  }
  else if (id == m_osc3_multi_position_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[2].setPosition(p_new_value);
    }
  }
  else if (id == m_osc1_spread_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[0].setWavetableMultiSpread(p_new_value);
    }
  }
  else if (id == m_osc2_spread_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[1].setWavetableMultiSpread(p_new_value);
    }
  }
  else if (id == m_osc3_spread_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].multi_osc[2].setWavetableMultiSpread(p_new_value);
    }
  }

  else if (id == m_osc1_vec_a_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[0].selectWavetable(p_new_value, 0);
    }
  }
  else if (id == m_osc2_vec_a_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[1].selectWavetable(p_new_value, 0);
    }
  }
  else if (id == m_osc3_vec_a_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[2].selectWavetable(p_new_value, 0);
    }
  }
  else if (id == m_osc1_vec_b_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[0].selectWavetable(p_new_value, 1);
    }
  }
  else if (id == m_osc2_vec_b_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[1].selectWavetable(p_new_value, 1);
    }
  }
  else if (id == m_osc3_vec_b_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[2].selectWavetable(p_new_value, 1);
    }
  }
  else if (id == m_osc1_vec_c_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[0].selectWavetable(p_new_value, 2);
    }
  }
  else if (id == m_osc2_vec_c_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[1].selectWavetable(p_new_value, 2);
    }
  }
  else if (id == m_osc3_vec_c_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[2].selectWavetable(p_new_value, 2);
    }
  }
  else if (id == m_osc1_vec_d_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[0].selectWavetable(p_new_value, 3);
    }
  }
  else if (id == m_osc2_vec_d_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[1].selectWavetable(p_new_value, 3);
    }
  }
  else if (id == m_osc3_vec_d_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[2].selectWavetable(p_new_value, 3);
    }
  }
  else if (id == m_osc1_vec_x_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[0].setX(p_new_value);
    }
  }
  else if (id == m_osc2_vec_x_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[1].setX(p_new_value);
    }
  }
  else if (id == m_osc3_vec_x_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[2].setX(p_new_value);
    }
  }
  else if (id == m_osc1_vec_y_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[0].setY(p_new_value);
    }
  }
  else if (id == m_osc2_vec_y_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[1].setY(p_new_value);
    }
  }
  else if (id == m_osc3_vec_y_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].vector_osc[2].setY(p_new_value);
    }
  }
  else if (id == m_osc1_chipwave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].selectWavetable(p_new_value);
    }
  }
  else if (id == m_osc2_chipwave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].selectWavetable(p_new_value);
    }
  }
  else if (id == m_osc3_chipwave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].selectWavetable(p_new_value);
    }
  }
  else if (id == m_osc1_chipnoise_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].setNoiseEnabled(p_new_value);
    }
  }
  else if (id == m_osc2_chipnoise_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].setNoiseEnabled(p_new_value);
    }
  }
  else if (id == m_osc3_chipnoise_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].setNoiseEnabled(p_new_value);
    }
  }
  else if (id == m_osc1_step_1_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].setArpSemitone(0, p_new_value);
    }
  }
  else if (id == m_osc2_step_1_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].setArpSemitone(0, p_new_value);
    }
  }
  else if (id == m_osc3_step_1_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].setArpSemitone(0, p_new_value);
    }
  }
  else if (id == m_osc1_step_2_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].setArpSemitone(1, p_new_value);
    }
  }
  else if (id == m_osc2_step_2_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].setArpSemitone(1, p_new_value);
    }
  }
  else if (id == m_osc3_step_2_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].setArpSemitone(1, p_new_value);
    }
  }
  else if (id == m_osc1_step_3_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].setArpSemitone(2, p_new_value);
    }
  }
  else if (id == m_osc2_step_3_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].setArpSemitone(2, p_new_value);
    }
  }
  else if (id == m_osc3_step_3_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].setArpSemitone(2, p_new_value);
    }
  }
  else if (id == m_osc1_step_3_on_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].setArpStepThreeOn(p_new_value);
    }
  }
  else if (id == m_osc2_step_3_on_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].setArpStepThreeOn(p_new_value);
    }
  }
  else if (id == m_osc3_step_3_on_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].setArpStepThreeOn(p_new_value);
    }
  }
  else if (id == m_osc1_arp_on_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].setArpEnabled(p_new_value);
    }
  }
  else if (id == m_osc2_arp_on_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].setArpEnabled(p_new_value);
    }
  }
  else if (id == m_osc3_arp_on_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].setArpEnabled(p_new_value);
    }
  }
  else if (id == m_osc1_arp_speed_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[0].setArpSpeed(p_new_value);
    }
  }
  else if (id == m_osc2_arp_speed_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[1].setArpSpeed(p_new_value);
    }
  }
  else if (id == m_osc3_arp_speed_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].chiptune_osc[2].setArpSpeed(p_new_value);
    }
  }
  else if (id == m_osc1_fm_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[0].setFMAmount(p_new_value);
    }
  }
  else if (id == m_osc2_fm_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[1].setFMAmount(p_new_value);
    }
  }
  else if (id == m_osc3_fm_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[2].setFMAmount(p_new_value);
    }
  }
  else if (id == m_osc1_carrier_ratio_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[0].setCarrierRatio(p_new_value);
    }
  }
  else if (id == m_osc2_carrier_ratio_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[1].setCarrierRatio(p_new_value);
    }
  }
  else if (id == m_osc3_carrier_ratio_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[2].setCarrierRatio(p_new_value);
    }
  }
  else if (id == m_osc1_modulator_ratio_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[0].setModulatorRatio(p_new_value);
    }
  }
  else if (id == m_osc2_modulator_ratio_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[1].setModulatorRatio(p_new_value);
    }
  }
  else if (id == m_osc3_modulator_ratio_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[2].setModulatorRatio(p_new_value);
    }
  }
  else if (id == m_osc1_carrier_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[0].selectCarrierWavetable(p_new_value);
    }
  }
  else if (id == m_osc2_carrier_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[1].selectCarrierWavetable(p_new_value);
    }
  }
  else if (id == m_osc3_carrier_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[2].selectCarrierWavetable(p_new_value);
    }
  }
  else if (id == m_osc1_modulator_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[0].selectModulatorWavetable(p_new_value);
    }
  }
  else if (id == m_osc2_modulator_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[1].selectModulatorWavetable(p_new_value);
    }
  }
  else if (id == m_osc3_modulator_wave_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[2].selectModulatorWavetable(p_new_value);
    }
  }
  else if (id == m_osc1_exp_fm_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[0].setFMExponential(p_new_value);
    }
  }
  else if (id == m_osc2_exp_fm_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[1].setFMExponential(p_new_value);
    }
  }
  else if (id == m_osc3_exp_fm_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].fm_osc[2].setFMExponential(p_new_value);
    }
  }
  else if (id == m_osc1_hp_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].noise_osc[0].setHPFreq(p_new_value);
    }
  }
  else if (id == m_osc2_hp_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].noise_osc[1].setHPFreq(p_new_value);
    }
  }
  else if (id == m_osc3_hp_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].noise_osc[2].setHPFreq(p_new_value);
    }
  }
  else if (id == m_osc1_lp_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].noise_osc[0].setLPFreq(p_new_value);
    }
  }
  else if (id == m_osc2_lp_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].noise_osc[1].setLPFreq(p_new_value);
    }
  }
  else if (id == m_osc3_lp_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].noise_osc[2].setLPFreq(p_new_value);
    }
  }
  
  
  

}