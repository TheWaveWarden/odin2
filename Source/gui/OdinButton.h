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
	enum class Type {
		unassigned,
		button_7x4,
		button_9x4,
		power_4x4,
		button_dropdown,
		up,
		down,
		burger,
	};

	OdinButton(const String &buttonName, const String &p_button_text, Type p_type = Type::unassigned) : juce::Button(buttonName), m_button_text(p_button_text), m_type(p_type) {

		switch (m_type) {
		case Type::button_7x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_7x4_off);
			break;
		case Type::button_9x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_9x4_off);
			break;
		case Type::button_dropdown:
			m_ui_asset_base = int(UIAssets::Indices::bttn_dropdown_5x2_off);
			break;
		case Type::up:
			m_ui_asset_base = int(UIAssets::Indices::bttn_4x2_arrow_up_off);
			break;
		case Type::down:
			m_ui_asset_base = int(UIAssets::Indices::bttn_4x2_arrow_down_off);
			break;
		case Type::burger:
			m_ui_asset_base = int(UIAssets::Indices::bttn_4x4_burger_off);
			break;
		case Type::power_4x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_4x4_power_off);
			break;
		default:
			break;
		}
	}

	void paintButton(juce::Graphics &g, bool p_highlight, bool p_pressed) override;

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

	void setButtonText(const juce::String &p_text) {
		m_button_text = p_text;
		repaint();
	}

private:
	Type m_type;
	int m_ui_asset_base;
	juce::String m_button_text;
	static OdinAudioProcessor *m_processor;
};
