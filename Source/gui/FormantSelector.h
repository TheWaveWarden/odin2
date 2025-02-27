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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "NumberSelector.h"

class FormantSelector : public NumberSelector {
public:
  FormantSelector(bool p_buttons_right = true)
      : NumberSelector(p_buttons_right, NumberSelector::Type::selector_8x4) {
    m_min = 0;
    m_max = (int)m_formant_vector.size() - 1;
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
