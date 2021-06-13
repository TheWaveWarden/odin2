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

class InputFeels : public juce::LookAndFeel_V4 {
public:
  void fillTextEditorBackground(Graphics &g, int width, int height,
                                TextEditor &t) override {
    g.setColour(MENU_BACKGROUND_COLOR);
    Rectangle<float> Rect(0, 0, width, height);
    g.fillRect(Rect); // pmai
  }

  void drawTextEditorOutline(Graphics &g, int width, int height,
                             TextEditor &t) override {
    g.setColour(Colour(80, 80, 80));
    Rectangle<float> Rect(0, 0, width, height);
    g.drawRect(Rect); // pmai
  }
};

class InputField : public TextEditor {
public:
  void applyValue() {
    //DBG("APPLYVALUE!!!!");

    // val to store parsed floating type number
    try {
      float val = std::stof(getText().toStdString());
      m_attached_slider->setValue(val);
    } catch (...) {
    }
  }

  void setAttachedSlider(Slider *p_slider) { m_attached_slider = p_slider; }

protected:
  Slider *m_attached_slider;
};