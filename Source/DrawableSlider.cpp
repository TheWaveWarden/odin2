/*
  ==============================================================================

    DrawableSlider.cpp
    Created: 26 Feb 2019 9:46:41pm
    Author:  frot

  ==============================================================================
*/

#include "DrawableSlider.h"
#include "GlobalIncludes.h"
#include "PluginProcessor.h"

OdinAudioProcessor *DrawableSlider::m_processor;

//==============================================================================
DrawableSlider::DrawableSlider()
{
  setLookAndFeel(&m_feels);

  setPopupDisplayEnabled(true, false, nullptr);
  setNumDecimalPlacesToDisplay(3);

  m_handle = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/slider_handle.png"));
  //setRange(1,2);
}

DrawableSlider::~DrawableSlider() { setLookAndFeel(nullptr); }

void DrawableSlider::paint(Graphics &g)
{
  //g.setColour(Colours::grey);
  //g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.drawImageAt(m_handle, 0, (1.f - valueToProportionOfLength(getValue())) * (getHeight() - m_handle.getHeight()));

  //DBG(getValue());
  if (m_midi_learn) {
      g.setColour(Colours::red);
      g.drawRoundedRectangle(getLocalBounds().getX(), getLocalBounds().getY(),
                             getLocalBounds().getWidth(),
                             getLocalBounds().getHeight(), 5,
                             2); // draw an outline around the component
    }
    else if (m_midi_control) {
      g.setColour(Colours::green);
      g.drawRoundedRectangle(getLocalBounds().getX(), getLocalBounds().getY(),
                             getLocalBounds().getWidth(),
                             getLocalBounds().getHeight(), 5,
                             2); // draw an outline around the component
    }
}

void DrawableSlider::mouseDown(const MouseEvent &event) {
  if (event.mods.isRightButtonDown()) {
    DBG("RIGHT");
    PopupMenu midi_learn_menu;
    if (m_midi_learn) {
      midi_learn_menu.addItem(2, "Stop MIDI learn");
      if (midi_learn_menu.show() == 2) {
        stopMidiLearn();
        m_processor->stopMidiLearn();
      }
    } else {
      midi_learn_menu.addItem(2, "MIDI learn");
      if (m_midi_control) {
        midi_learn_menu.addItem(3, "MIDI forget");
      }
      int menu = midi_learn_menu.show();
      if (menu == 2) {
        if (m_midi_control) {
          m_processor->midiForget(this);
        }
        m_processor->startMidiLearn(this);
        m_midi_learn = true;
        m_midi_control = false;
        repaint();
      } else if (menu == 3) {
        m_processor->midiForget(this);
        m_midi_control = false;
        repaint();
      }
    }
    return;
  }
  Slider::mouseDown(event);
}

String DrawableSlider::getTextFromValue(double value) {
  //todo?
  //https://forum.juce.com/t/setnumdecimalplacestodisplay-not-behaving-solved/33686/2
  return String(value, getNumDecimalPlacesToDisplay()) + getTextValueSuffix();
}