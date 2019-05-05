/*
  ==============================================================================

    LeftRightButton.h
    Created: 27 Feb 2019 3:35:54pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
/*
 */

class OdinAudioProcessor;

class LeftRightButton : public juce::Button {
public:
  LeftRightButton(const String &buttonName);
  ~LeftRightButton();

  void paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted,
                   bool shouldDrawButtonAsDown) override;
  void resized() override;
  void setImage(juce::Image p_image, int p_frame) {
    if (p_frame == 1) {
      m_image_left = p_image;
    } else {
      m_image_right = p_image;
    }
  }

  void mouseDown(const MouseEvent &) override;
  void mouseExit(const MouseEvent &) override {}
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

private:
  bool m_midi_learn = false;
  bool m_midi_control = false;

  static OdinAudioProcessor *m_processor;
  juce::Image m_image_left;
  juce::Image m_image_right;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftRightButton)
};
