/*
  ==============================================================================

    Knob.cpp
    Created: 5 May 2019 7:23:25pm
    Author:  frot

  ==============================================================================
*/

#include "Knob.h"
#include "PluginProcessor.h"

OdinAudioProcessor *Knob::m_processor;

void Knob::mouseDown(const MouseEvent &event) {
  if (event.mods.isRightButtonDown()) {
    DBG("RIGHT");
    PopupMenu midi_learn_menu;
    if (m_midi_learn) {
      midi_learn_menu.addItem(2, "Stop MIDI learn");
      if (midi_learn_menu.show() == 2) {
        stopMidiLearn();
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
