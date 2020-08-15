#include "OdinButton.h"
#include "PluginProcessor.h"

OdinAudioProcessor *OdinButton::m_processor;

void OdinButton::mouseDown(const MouseEvent &event) {
	if (event.mods.isRightButtonDown() && m_midi_learn_possible) {
		//DBG("RIGHT");
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
					m_processor->midiForget(m_parameter_ID, this);
				}
				m_processor->startMidiLearn(m_parameter_ID, this);
				m_midi_learn   = true;
				m_midi_control = false;
				repaint();
			} else if (menu == 3) {
				m_processor->midiForget(m_parameter_ID, this);
				m_midi_control = false;
				repaint();
			}
		}
		return;
	}
	DrawableButton::mouseDown(event);
}