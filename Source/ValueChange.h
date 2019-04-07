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
  

}