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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinMidiLearnBase.h"

class OdinAudioProcessor;

class OdinButton : public juce::Button, public OdinMidiLearnBase {
public:
	OdinButton(const String &buttonName, const String &p_button_text) :
	    juce::Button(buttonName), m_button_text(p_button_text) {
	}

	void paintButton(juce::Graphics &g, bool p_highlight, bool p_pressed) override {
		static constexpr auto stroke = 1.0f;

		g.setColour(COL_LIGHT);
		g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(stroke / 2.0f), 4.0f, stroke);

		if (p_highlight) {
			g.setColour(COL_LIGHT.withAlpha(0.3f));
			g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(stroke / 2.0f), 4.0f);
		} else if (getToggleState()) {
			g.setColour(COL_LIGHT.withAlpha(0.15f));
			g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(stroke / 2.0f), 4.0f);
		}

		g.setColour(COL_LIGHT);
		g.setFont(H * 0.6f);
		g.drawText(m_button_text, getLocalBounds(), juce::Justification::centred);
	}

	static void setOdinPointer(OdinAudioProcessor *p_pointer) {
		m_processor = p_pointer;
	}

	void stopMidiLearn() override {
		m_midi_learn   = false;
		m_midi_control = false;
		repaint();
	}

	void setMidiControlActive() override {
		m_midi_learn   = false;
		m_midi_control = true;
		repaint();
	}

private:
	juce::String m_button_text;
	static OdinAudioProcessor *m_processor;
};
