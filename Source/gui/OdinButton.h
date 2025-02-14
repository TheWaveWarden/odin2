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

class OdinAudioProcessor;

class OdinButton : public juce::Button {
public:
	enum class Type {
		unassigned,
		button_7x4,
		button_7x5,
		button_9x4,
		button_10x4,
		button_12x4,
		button_13x4,
		button_13x4EnvLFO,
		button_5x5,
		button_40x5,
		button_41x5,
		power_4x4,
		button_dropdown,
		button_left,
		button_right,
		fx_enabled,
		up,
		down,
		burger,
		loop,
		matrix_clear,
		preset_import,
		preset_export,
		preset_export_small,
		preset_new,
		preset_save
	};

	OdinButton(const String &buttonName, const String &p_button_text, Type p_type = Type::unassigned) : juce::Button(buttonName), m_button_text(p_button_text), m_type(p_type) {

		switch (m_type) {
		case Type::button_7x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_7x4_off);
			break;
		case Type::button_7x5:
			m_ui_asset_base = int(UIAssets::Indices::bttn_7x5_off);
			break;
		case Type::button_9x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_9x4_off);
			break;
		case Type::button_13x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_13x4_off);
			break;
		case Type::button_12x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_12x4_off);
			break;
		case Type::button_10x4:
			m_ui_asset_base = int(UIAssets::Indices::bttn_10x4_off);
			break;
		case Type::button_13x4EnvLFO:
			m_ui_asset_base = int(UIAssets::Indices::bttn_13x4_EnvLFO_off);
			break;
		case Type::button_dropdown:
			m_ui_asset_base = int(UIAssets::Indices::bttn_dropdown_5x2_off);
			break;
		case Type::button_left:
			m_ui_asset_base = int(UIAssets::Indices::bttn_5x5_arrow_L_off);
			break;
		case Type::button_right:
			m_ui_asset_base = int(UIAssets::Indices::bttn_5x5_arrow_R_off);
			break;
		case Type::fx_enabled:
			m_ui_asset_base = int(UIAssets::Indices::bttn_12x2_power_off);
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
		case Type::button_5x5:
			m_ui_asset_base = int(UIAssets::Indices::bttn_5x5_off);
			break;
		case Type::button_40x5:
			m_ui_asset_base = int(UIAssets::Indices::bttn_select_bot_40x5_off);
			break;
		case Type::button_41x5:
			m_ui_asset_base = int(UIAssets::Indices::bttn_select_bot_41x5_off);
			break;
		case Type::loop:
			m_ui_asset_base = int(UIAssets::Indices::bttn_loop_off);
			break;
		case Type::matrix_clear:
			m_ui_asset_base = int(UIAssets::Indices::bttn_2x2_close);
			break;
		case Type::preset_import:
			m_ui_asset_base = int(UIAssets::Indices::bttn_preset_a);
			break;
		case Type::preset_export:
			m_ui_asset_base = int(UIAssets::Indices::bttn_preset_b);
			break;
		case Type::preset_export_small:
			m_ui_asset_base = int(UIAssets::Indices::bttn_preset_export2);
			break;
		case Type::preset_new:
			m_ui_asset_base = int(UIAssets::Indices::bttn_preset_c);
			break;
		case Type::preset_save:
			m_ui_asset_base = int(UIAssets::Indices::bttn_preset_d);
			break;
		default:
			break;
		}
	}

	void paintButton(juce::Graphics &g, bool p_highlight, bool p_pressed) override;

	static void setOdinPointer(OdinAudioProcessor *p_pointer) {
		m_processor = p_pointer;
	}

	void setButtonText(const juce::String &p_text) {
		m_button_text = p_text;
		repaint();
	}

	void setFontHeightRelative(float p_font_height_relative) {
		m_font_height_relative = p_font_height_relative;
		repaint();
	}

private:
	Type m_type;
	int m_ui_asset_base;
	juce::String m_button_text;
	static OdinAudioProcessor *m_processor;

	float m_font_height_relative = 0.5f;
};

class DoubleTextButton : public OdinButton {
public:
	DoubleTextButton(const juce::String &buttonName, const juce::String &p_on_text, const juce::String &p_off_text) :
	    OdinButton(buttonName, ""),
	    m_on_text(p_on_text),
	    m_off_text(p_off_text) {
	}

	void paintButton(juce::Graphics &g, bool isHighlighted, bool isPressed) override;

private:
	juce::String m_on_text;
	juce::String m_off_text;
};