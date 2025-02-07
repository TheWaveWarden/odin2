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

#include "OdinButton.h"
#include "../ConfigFileManager.h"
#include "../PluginProcessor.h"
#include "UIAssetManager.h"

OdinAudioProcessor *OdinButton::m_processor;

void OdinButton::paintButton(juce::Graphics &g, bool p_highlight, bool p_pressed) {

	if (m_type == Type::unassigned) {

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
		g.drawText(m_button_text, getLocalBounds(), juce::Justification::centred, false);

		return;
	}

	// ordering of assets:
	//bttn_7x4_off.png
	//bttn_7x4_off_hover.png
	//bttn_7x4_off_pressed.png
	//bttn_7x4_on.png
	//bttn_7x4_on_hover.png
	//bttn_7x4_on_pressed.png

	auto asset = m_ui_asset_base;
	if (getToggleState()) {
		asset += 3;
	}

	if (p_pressed) {
		asset += 2;
	} else if (p_highlight) {
		asset += 1;
	}

	auto background = UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices(asset), ConfigFileManager::getInstance().getOptionGuiScale());
	g.drawImageAt(background, 0, 0);

	const auto text_alpha = p_pressed ? 0.64f : 1.0f;
	auto colour_hex       = getToggleState() ? 0xff0084d7 : 0xff86cbf6;
	if (p_highlight)
		colour_hex = 0xffffffff;
	g.setColour(juce::Colour(colour_hex).withAlpha(text_alpha));
	g.setFont(H * 0.5f);
	g.drawText(m_button_text, getLocalBounds(), juce::Justification::centred, false);
}