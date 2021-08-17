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

#include "LeftRightButton.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"

OdinAudioProcessor *LeftRightButton::m_processor;

LeftRightButton::LeftRightButton(const String &buttonName) : juce::Button(buttonName) {
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
}

LeftRightButton::~LeftRightButton() {
	// setClickingTogglesState(true);
}

void LeftRightButton::paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
	g.fillAll(Colours::transparentBlack);

	if (getToggleState()) {
		g.drawImageAt(m_image_left, 0, 0);
	} else {
		g.drawImageAt(m_image_right, 0, 0);
	}
	if (m_midi_learn) {
		g.setColour(Colours::red);
		g.drawRoundedRectangle(getLocalBounds().getX(),
		                       getLocalBounds().getY(),
		                       getLocalBounds().getWidth(),
		                       getLocalBounds().getHeight(),
		                       5,
		                       2); // draw an outline around the component
	} else if (m_midi_control) {
		g.setColour(Colours::green);
		g.drawRoundedRectangle(getLocalBounds().getX(),
		                       getLocalBounds().getY(),
		                       getLocalBounds().getWidth(),
		                       getLocalBounds().getHeight(),
		                       5,
		                       2); // draw an outline around the component
	}
}

void LeftRightButton::mouseDown(const MouseEvent &p_event) {
	if (p_event.mods.isRightButtonDown() && m_midi_learn_possible) {

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
	juce::Point<int> mouse_position = getMouseXYRelative();
	bool left                       = mouse_position.getX() < getWidth() / 2.f ? true : false;
	if (left) {
		// Button::mouseDown(p_event);
		setToggleState(true, sendNotification);
	} else {
		// Button::mouseDown(p_event);
		setToggleState(false, sendNotification);
	}
}
