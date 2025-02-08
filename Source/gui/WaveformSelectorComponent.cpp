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

#include "WaveformSelectorComponent.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "UIAssetManager.h"

WaveformSelectorComponent::WaveformSelectorComponent(bool p_buttons_right) : m_up("up", "", OdinButton::Type::up), m_down("down", "", OdinButton::Type::down) {

	m_display.setInlay(1);
	addAndMakeVisible(m_display);

	m_up.setClickingTogglesState(false);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { decrement(); };

	m_down.setClickingTogglesState(false);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { increment(); };

	m_display.onMouseDown = [&]() { setValue(m_menu.show()); };

	m_buttons_right = p_buttons_right;

	m_menu.setLookAndFeel(&m_menu_feels);

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	setValue(1);
}

WaveformSelectorComponent::~WaveformSelectorComponent() {
	m_menu.setLookAndFeel(nullptr);
}

void WaveformSelectorComponent::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::screen_up_down_24x4_R, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void WaveformSelectorComponent::resized() {
	auto bounds        = getLocalBounds();
	auto button_bounds = bounds.removeFromRight(H);
	m_up.setBounds(button_bounds.removeFromTop(proportionOfHeight(0.5f)));
	m_down.setBounds(button_bounds);
	m_display.setBounds(bounds);
}