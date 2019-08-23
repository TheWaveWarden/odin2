/*
  ==============================================================================

    InputField.h
    Created: 20 Jun 2019 1:17:51am
    Author:  frot

  ==============================================================================
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
                             TextEditor &t) {
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