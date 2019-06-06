/*
  ==============================================================================

    DrawableSlider.h
    Created: 26 Feb 2019 9:46:41pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
/*
 */

#include "../JuceLibraryCode/JuceHeader.h"

#define ADSR_LABEL_OFFSET_Y -8

class ADSRFeels : public juce::LookAndFeel_V4 {
public:
  void drawBubble(Graphics &g, BubbleComponent &b,
                  const Point<float> &positionOfTip,
                  const Rectangle<float> &body) override {
    g.setColour(MENU_BACKGROUND_COLOR);
    g.fillRect(body); // pmai
    g.setColour(Colour(50, 50, 50));
    g.drawRect(body); // pmai
  }
  int getSliderPopupPlacement(Slider &slider) { return 2; }
  /*
    drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos, const
    Slider::SliderStyle, Slider &) {

                          }*/
};

class OdinAudioProcessor;

class DrawableSlider : public juce::Slider {
public:
  DrawableSlider();
  ~DrawableSlider();

  //TODO slider "wobble" when moved to very low params...

  String getTextFromValue(double value) override;

  void setTextValueSuffix(const String &suffix) {
    //setNumDecimalPlacesToDisplay(20);
    Slider::setTextValueSuffix(suffix);
  }

  void paint(Graphics &) override;
  // void resized() override;

  void setImage(juce::Image p_handle) { m_handle = p_handle; }

  static void setOdinPointer(OdinAudioProcessor *p_pointer) {
    m_processor = p_pointer;
  }

  void stopMidiLearn() {
    m_midi_learn = false;
    repaint();
  }

  void setMidiControlActive() {
    m_midi_learn = false;
    m_midi_control = true;
    repaint();
  }

  void mouseDown(const MouseEvent &event) override;
  

private:
  bool m_midi_learn = false;
  bool m_midi_control = false;

  static OdinAudioProcessor *m_processor;
  ADSRFeels m_feels;
  juce::Image m_handle;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableSlider)
};
