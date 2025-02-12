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

#include "SyncTimeSelector.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "UIAssetManager.h"

SyncTimeSelector::SyncTimeSelector(UIAssets::Indices p_asset) :
    m_asset(p_asset),
    m_up_left("up_left", "", OdinButton::Type::up),
    m_down_left("down_left", "", OdinButton::Type::down),
    m_up_right("up_right", "", OdinButton::Type::up),
    m_down_right("down_right", "", OdinButton::Type::down) {

	m_dropdown.addItem(10, "8 / 1");
	m_dropdown.addItem(20, "4 / 1");
	m_dropdown.addItem(30, "2 / 1");
	m_dropdown.addItem(40, "1 / 1");
	m_dropdown.addItem(50, "1 / 2");
	m_dropdown.addItem(60, "1 / 4");
	m_dropdown.addItem(70, "3 / 16");
	m_dropdown.addItem(80, "1 / 8");
	m_dropdown.addItem(90, "1 / 8T");
	m_dropdown.addItem(100, "1 / 16");
	m_dropdown.addItem(110, "1 / 16T");
	m_dropdown.addItem(120, "1 / 32");
	m_dropdown.addItem(130, "1 / 32T");

	m_display.onMouseDown = [&]() { setValuesFromDropdown(m_dropdown.show()); };

	m_dropdown.setLookAndFeel(&m_menu_feels);

	m_display.setInlay(1);
	m_display.setColor(STANDARD_DISPLAY_COLOR);
	addAndMakeVisible(m_display);

	m_up_left.setClickingTogglesState(false);
	addAndMakeVisible(m_up_left);
	m_up_left.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up_left.onClick = [&]() { incrementLeft(); };
	m_up_left.setAlwaysOnTop(true);

	m_up_right.setClickingTogglesState(false);
	addAndMakeVisible(m_up_right);
	m_up_right.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up_right.onClick = [&]() { incrementRight(); };
	m_up_right.setAlwaysOnTop(true);

	m_down_left.setClickingTogglesState(false);
	addAndMakeVisible(m_down_left);
	m_down_left.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down_left.onClick = [&]() { decrementLeft(); };
	m_down_left.setAlwaysOnTop(true);

	m_down_right.setClickingTogglesState(false);
	addAndMakeVisible(m_down_right);
	m_down_right.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down_right.onClick = [&]() { decrementRight(); };
	m_down_right.setAlwaysOnTop(true);

	updateDisplay();
}

SyncTimeSelector::~SyncTimeSelector() {
	m_dropdown.setLookAndFeel(nullptr);
}

void SyncTimeSelector::paint(Graphics &g) {
	g.setColour(juce::Colours::white.withAlpha(isEnabled() ? 1.0f : MODULE_DISABLED_ALPHA));
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(m_asset, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void SyncTimeSelector::resized() {

	auto bounds       = getLocalBounds();
	auto button_left  = bounds.removeFromLeft(H);
	auto button_right = bounds.removeFromRight(H);

	m_up_left.setBounds(button_left.removeFromTop(proportionOfHeight(0.5f)));
	m_down_left.setBounds(button_left);
	m_up_right.setBounds(button_right.removeFromTop(proportionOfHeight(0.5f)));
	m_down_right.setBounds(button_right);

	m_display.setBounds(bounds);
}

void SyncTimeSelector::setValuesFromDropdown(int p_dropdown_return) {
	switch (p_dropdown_return) {
	case 0:
		return;
		break;

		//std::vector<std::string> m_left_values  = {"1", "2", "3", "4", "5", "6", "7", "8"};
		//std::vector<std::string> m_right_values = {"1", "2", "4", "8", "8T", "16", "16T", "32", "32T"};

	case 10:
		//"8 / 1";
		m_value_left  = 7;
		m_value_right = 0;
		break;
	case 20:
		//"4 / 1";
		m_value_left  = 3;
		m_value_right = 0;
		break;
	case 30:
		//"2 / 1";
		m_value_left  = 1;
		m_value_right = 0;
		break;
	case 40:
		//"1 / 1";
		m_value_left  = 0;
		m_value_right = 0;
		break;
	case 50:
		//"1 / 2";
		m_value_left  = 0;
		m_value_right = 1;
		break;
	case 60:
		//"1 / 4";
		m_value_left  = 0;
		m_value_right = 2;
		break;
	case 70:
		//"3 / 16";
		m_value_left  = 2;
		m_value_right = 5;
		break;
	case 80:
		//"1 / 8";
		m_value_left  = 0;
		m_value_right = 3;
		break;
	case 90:
		//"1 / 8T";
		m_value_left  = 0;
		m_value_right = 4;
		break;
	case 100:
		//"1 / 16";
		m_value_left  = 0;
		m_value_right = 5;
		break;
	case 110:
		//"1 / 16T";
		m_value_left  = 0;
		m_value_right = 6;
		break;
	case 120:
		//"1 / 32";
		m_value_left  = 0;
		m_value_right = 7;
		break;
	case 130:
		//"1 / 32T";
		m_value_left  = 0;
		m_value_right = 8;
		break;
	default:
		return;
	}
	updateDisplay();
	OnValueChange(m_value_left, m_value_right);
}
