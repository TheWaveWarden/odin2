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

#include <iomanip> // setprecision
#include <sstream> // stringstream

#include "OdinKnob.h"
#include "../PluginProcessor.h"

OdinAudioProcessor *OdinKnob::m_processor;

void OdinKnob::mouseDown(const MouseEvent &event) {
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
	Slider::mouseDown(event);
}

String OdinKnob::getTextFromValue(double value) {
	//apparently there's a better solution....
	//https://forum.juce.com/t/setnumdecimalplacestodisplay-not-behaving-solved/33686/2
	//DBG(String(value, getNumDecimalPlacesToDisplay()) + getTextValueSuffix());
	return String(value, getNumDecimalPlacesToDisplay()) + getTextValueSuffix();
}
