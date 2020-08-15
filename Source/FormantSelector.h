#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "NumberSelector.h"

class FormantSelector : public NumberSelector {
public:
  FormantSelector(bool p_buttons_right = true)
      : NumberSelector(p_buttons_right) {
    m_min = 0;
    m_max = m_formant_vector.size() - 1;
  }

  ~FormantSelector() {}

  void setValueNotifyingHost(int p_value){
    if (p_value >= m_min && p_value <= m_max) {
      m_value = p_value;
      m_display.setText(m_formant_vector[p_value]);
      OnValueChange(p_value);
    }
  }

  void setValue(int p_value) override {
    if(p_value == m_value){
      return;//avoid infinite loop
    }
    if (p_value >= m_min && p_value <= m_max) {
      m_value = p_value;
      m_display.setText(m_formant_vector[p_value]);
      OnValueChange(p_value);
    }
  }

private:
  std::vector<std::string> m_formant_vector = {"A", "E", "I", "O",
                                               "U", "Ä", "Ö", "Ü"};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FormantSelector)
};
