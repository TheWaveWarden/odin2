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
#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "UIAssetManager.h"

OdinAudioProcessor *LeftRightButton::m_processor;

LeftRightButton::LeftRightButton(const String &buttonName, Type p_type) : juce::Button(buttonName), m_type(p_type) {
	switch (m_type) {
	case Type::osc_fm_exp:
		m_asset      = UIAssets::Indices::switch_v_down;
		m_is_up_down = true;
		break;
	case Type::filter_comb_polarity:
		m_asset = UIAssets::Indices::switch_comb_neg;
		break;
	}
}

LeftRightButton::~LeftRightButton() {
}

void LeftRightButton::paintButton(Graphics &g, bool p_highlight, bool /*p_pressed*/) {
	auto asset = int(m_asset);
	if (getToggleState())
		asset += 2;
	if (p_highlight)
		asset += 1;

	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices(asset), ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void LeftRightButton::mouseDown(const MouseEvent &p_event) {
	juce::Point<int> mouse_position = getMouseXYRelative();
	bool new_toggle_state           = false;

	if (m_is_up_down)
		new_toggle_state = mouse_position.getY() < getHeight() / 2 ? true : false;
	else
		new_toggle_state = mouse_position.getX() > getWidth() / 2 ? true : false;

	if (new_toggle_state)
		setToggleState(true, sendNotification);
	else
		setToggleState(false, sendNotification);
}

void LeftRightButton::mouseDrag(const MouseEvent &p_event) {
	// for the up down buttons in the FM osc, also allow dragging

	if (!m_is_up_down)
		return;

	juce::Point<int> mouse_position = getMouseXYRelative();
	bool new_toggle_state           = mouse_position.getY() < getHeight() / 2 ? true : false;

	if (getToggleState() != new_toggle_state)
		setToggleState(new_toggle_state, sendNotification);
}
