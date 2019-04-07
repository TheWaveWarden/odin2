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
  
  
  /*else if (id == m_osc1_semi_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[0].m_semitones = p_new_value;
    }
  }
  else if (id == m_osc2_semi_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[1].m_semitones = p_new_value;
    }
  }
  else if (id == m_osc3_semi_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[2].m_semitones = p_new_value;
    }
  }
  else if (id == m_osc1_fine_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[0].m_cent = p_new_value;
    }
  }
  else if (id == m_osc2_fine_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[1].m_cent = p_new_value;
    }
  }
  else if (id == m_osc3_fine_identifier) {
    for(int voice = 0; voice < VOICES; ++voice){
      m_voice[voice].analog_osc[2].m_cent = p_new_value;
    }
  }*/

}