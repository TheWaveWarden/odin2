#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinMidiLearnBase.h"
#include "GlobalIncludes.h"

class OdinAudioProcessor;

class OdinButton : public DrawableButton, public OdinMidiLearnBase {
public:
  OdinButton(const String &buttonName, ButtonStyle buttonStyle)
      : DrawableButton(buttonName, buttonStyle) {}

  void paint(juce::Graphics &g) override {
    SET_INTERPOLATION_QUALITY(g)

    DrawableButton::paint(g);
    if (m_midi_learn) {
      g.setColour(Colours::red);
      g.drawRoundedRectangle(getLocalBounds().getX(), getLocalBounds().getY(),
                             getLocalBounds().getWidth(),
                             getLocalBounds().getHeight(), 5,
                             2); // draw an outline around the component
    } else if (m_midi_control) {
      g.setColour(Colours::green);
      g.drawRoundedRectangle(getLocalBounds().getX(), getLocalBounds().getY(),
                             getLocalBounds().getWidth(),
                             getLocalBounds().getHeight(), 5,
                             2); // draw an outline around the component
    }
  }

  void mouseDown(const MouseEvent &event) override;
  static void setOdinPointer(OdinAudioProcessor *p_pointer) {
    m_processor = p_pointer;
  }

  void stopMidiLearn() override {
    m_midi_learn = false;
    m_midi_control = false;
    repaint();
  }

  void setMidiControlActive() override {
    m_midi_learn = false;
    m_midi_control = true;
    repaint();
  }

private:

  static OdinAudioProcessor *m_processor;
};
