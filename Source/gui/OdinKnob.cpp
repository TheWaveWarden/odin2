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

#include "../PluginProcessor.h"
#include "OdinKnob.h"
#include "UIAssetManager.h"
#include "../ConfigFileManager.h"

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

	//drawGuides (g, isEnabled());

	const auto value01      = valueToProportionOfLength(getValue());
	const auto image_offset = juce::roundToInt(value01 * double(N_KNOB_FRAMES - 1));
	const auto asset        = UIAssets::Indices(int(m_ui_asset_base) + int(image_offset));
	juce::Image graphic     = UIAssetManager::getInstance()->getUIAsset(asset, ConfigFileManager::getInstance().getOptionGuiScale());
	g.drawImageAt(graphic, 0, 0);
}
