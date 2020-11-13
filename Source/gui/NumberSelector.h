/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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
#include "GlasDisplay.h"
#include "../GlobalIncludes.h"

class NumberSelector : public Component,
                       public AudioProcessorValueTreeState::Listener {
public:
                           NumberSelector() = delete;
  NumberSelector(bool p_buttons_right);
  ~NumberSelector();

  void parameterChanged(const String &parameterID, float newValue) override {
    if (parameterID == m_parameter_id) {
      setValue(newValue);
    }
  }

  void paint(Graphics &) override;

  void setColor(juce::Colour p_color) { m_display.setColor(p_color); }

  void setRange(int p_min, int p_max) {
    m_min = p_min;
    m_max = p_max;
  }

  
  virtual void setValue(int p_value) {
    if(p_value == m_value){
      return;//avoid infinite loop
    }
    if (p_value >= m_min && p_value <= m_max) {
      m_value = p_value;

      m_display.setText(std::to_string(m_value));
      OnValueChange(p_value);
    }
  }

  void setTooltip(const String p_text) { m_display.setTooltip(p_text); }

  std::function<void(int)> OnValueChange = [](int) {DBG("not set......");};

  void setParameterId(String p_id) { m_parameter_id = p_id; }

  void setMouseDragDivisor(float p_divisor) {
    m_mouse_drag_divisor = p_divisor;
  }
  virtual void setGUIBig();
  virtual void setGUISmall();
protected:
  bool m_GUI_big = true;


  float m_mouse_drag_divisor = 7.f;
  int mouse_reference_value = 0;
  int m_drag_initial_value = 0;

  String m_parameter_id;

  virtual void increment() { setValue(m_value + 1); }

  virtual void decrement() { setValue(m_value - 1); }

  GlasDisplay m_display;
  int m_min = 1;
  int m_max = 1;

  juce::DrawableButton m_up;
  juce::DrawableButton m_down;

  int m_value = 1;
  bool m_buttons_right;
  int m_display_width;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumberSelector)
};
