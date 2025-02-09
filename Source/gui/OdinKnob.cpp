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

#include "../ConfigFileManager.h"
#include "../PluginProcessor.h"
#include "OdinKnob.h"
#include "UIAssetManager.h"

OdinAudioProcessor *OdinKnob::m_processor;

OdinKnob::OdinKnob(Type type) : m_type(type) {
	setLookAndFeel(&m_knob_feels);
	setRange(0, 1);

	if (m_type != Type::timeHz)
		setPopupDisplayEnabled(true, false, nullptr);

	setNumDecimalPlacesToDisplay(3);
	setVelocityModeParameters(1.0, 1, 0.0, true, ModifierKeys::shiftModifier);

	switch (m_type) {
	default:
	case Type::knob_4x4a:
		m_ui_asset_base = int(UIAssets::Indices::knob_4x4_a_0000);
		m_inlay_y       = 1;
		m_center_pos_y  = 24.0f / 56.0f;
		m_guide_radius  = 0.39f;
		break;
	case Type::knob_4x4b:
		m_ui_asset_base = int(UIAssets::Indices::knob_4x4_b_0000);
		m_inlay_y       = 1;
		m_center_pos_y  = 24.0f / 56.0f;
		m_guide_radius  = 0.39f;
		break;
	case Type::knob_5x5a:
		m_ui_asset_base = int(UIAssets::Indices::knob_5x5_a_0000);
		m_inlay_y       = 1;
		m_center_pos_y  = 28.0f / 64.0f;
		m_guide_radius  = 0.42f;
		break;
	case Type::knob_5x5b:
		m_ui_asset_base = int(UIAssets::Indices::knob_5x5_b_0000);
		m_inlay_y       = 1;
		m_center_pos_y  = 28.0f / 64.0f;
		m_guide_radius  = 0.42f;
		break;
	case Type::knob_6x6a:
		m_ui_asset_base = int(UIAssets::Indices::knob_6x6_a_0000);
		m_inlay_x       = 1;
		m_inlay_y       = 1;
		m_center_pos_y  = 32.0f / 72.0f;
		m_guide_radius  = 0.42f;
		break;
	case Type::knob_6x6b:
		m_ui_asset_base = int(UIAssets::Indices::knob_6x6_b_0000);
		m_inlay_x       = 1;
		m_inlay_y       = 1;
		m_center_pos_y  = 32.0f / 72.0f;
		m_guide_radius  = 0.42f;
		break;
	case Type::knob_8x8a:
		m_ui_asset_base = int(UIAssets::Indices::knob_8x8_a_0000);
		m_inlay_x       = 1;
		m_inlay_y       = 1;
		m_center_pos_y  = 40.0f / 88.0f;
		m_guide_radius  = 0.46f;
		break;
	case Type::knob_8x8b:
		m_ui_asset_base = int(UIAssets::Indices::knob_8x8_b_0000);
		m_inlay_x       = 1;
		m_inlay_y       = 1;
		m_center_pos_y  = 40.0f / 88.0f;
		m_guide_radius  = 0.46f;
		break;
	case Type::wheel:
		m_ui_asset_base = int(UIAssets::Indices::wheel_0000);
		break;
	case Type::timeHz:
		m_ui_asset_base = int(UIAssets::Indices::screen_dropdown_13x4);
		setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
		break;
	}
}

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

void OdinKnob::paint(juce::Graphics &g) {
	if (m_type == Type::unassigned) {
		g.setColour(COL_LIGHT);
		const auto val                 = valueToProportionOfLength(getValue());
		static constexpr auto deadzone = 30.0f;
		static constexpr auto stroke   = 2.0f;
		const auto angle               = (deadzone + val * (360.0f - 2 * deadzone)) * juce::MathConstants<float>::twoPi / 360.0f;
		const auto sin                 = std::sin(angle);
		const auto cos                 = std::cos(angle);
		const auto center_x            = getLocalBounds().getCentreX();
		const auto center_y            = getLocalBounds().getCentreY();

		g.drawEllipse(getLocalBounds().toFloat().reduced(stroke / 2.0f), 1.0f);
		g.drawLine(center_x, center_y, center_x * (1.0f - sin), center_y * (1.0f + cos));
		return;
	}

	if (m_type == Type::timeHz) {
		g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices(m_ui_asset_base), ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
		g.setColour(COL_TEXT_BLUE);
		g.setFont(getHeight() / 2.0f);
		g.drawText(getTextFromValue(getValue()), getLocalBounds(), juce::Justification::centred, false);
		return;
	}

	if (m_num_guides > 0)
		drawGuides(g, isEnabled());

	const auto value01      = valueToProportionOfLength(getValue());
	const auto num_frames   = m_type == Type::wheel ? N_KNOB_FRAMES_WHEEL : N_KNOB_FRAMES;
	const auto image_offset = juce::roundToInt(value01 * double(num_frames - 1));
	const auto asset        = UIAssets::Indices(int(m_ui_asset_base) + int(image_offset));

	const auto ui_scale = ConfigFileManager::getInstance().getOptionGuiScale();
	juce::Image graphic = UIAssetManager::getInstance()->getUIAsset(asset, ui_scale);
	g.setColour(juce::Colours::white);
	g.drawImageAt(graphic, ui_scale * m_inlay_x, ui_scale * m_inlay_y);
}

void OdinKnob::drawGuides(juce::Graphics &g, bool isEnabled) {
	g.setColour(juce::Colours::white.withAlpha(0.5f));

	const auto center_x = float(getWidth()) / 2.0f;
	const auto center_y = float(getHeight()) * m_center_pos_y;
	const auto radius   = float(getWidth()) * m_guide_radius;

	const auto angle_start = 42.0f / 360.0f * juce::MathConstants<float>::twoPi;
	const auto angle_range = juce::MathConstants<float>::twoPi - 2.0f * angle_start;

	const auto stroke = float(getHeight()) / 70.0f;

	for (int guide = 0; guide < m_num_guides; ++guide) {
		const auto angle = angle_start + angle_range * float(guide) / float(m_num_guides - 1);
		const auto sin   = std::sin(angle);
		const auto cos   = std::cos(angle);

		const auto x1 = center_x + radius * sin;
		const auto y1 = center_y + radius * cos;
		const auto x2 = center_x;
		const auto y2 = center_y;

		g.drawLine(x1, y1, x2, y2, stroke);
	}
}
