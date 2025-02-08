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

#include "NumberSelector.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "UIAssetManager.h"

NumberSelector::NumberSelector(bool p_buttons_right, Type p_type) : m_type(p_type), m_up("up", "", OdinButton::Type::up), m_down("down", "", OdinButton::Type::down) {
	switch (m_type) {
	case Type::selector_12x4:
		m_asset = UIAssets::Indices::screen_up_down_24x4_R;
		break;
	}

	m_up.setClickingTogglesState(false);
	m_down.setClickingTogglesState(false);

	m_display.setInlay(1);
	m_display.setText(std::to_string(m_value));

	m_display.toParentMouseDown = [&](const MouseEvent e) {
		mouse_reference_value = e.getScreenY();
		m_drag_initial_value  = m_value;
		// Component::mouseDown(e);
	};

	m_display.toParentMouseDrag = [&](const MouseEvent e) {
		float mouse_moved = mouse_reference_value - e.getScreenY();

		int new_value = m_drag_initial_value + mouse_moved / m_mouse_drag_divisor;
		new_value     = new_value > m_max ? m_max : new_value;
		new_value     = new_value < m_min ? m_min : new_value;

		setValue(new_value);

		// Component::mouseDrag(e);
	};

	m_display.toParentMouseUp = [&](const MouseEvent e) {};

	addAndMakeVisible(m_display);

	m_up.setClickingTogglesState(true);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { increment(); };

	m_down.setClickingTogglesState(true);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { decrement(); };

	m_display.onMouseDown = [&]() {
		// do nothing
	};

	m_buttons_right = p_buttons_right;

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	setValue(1);
}

NumberSelector::~NumberSelector() {
}

void NumberSelector::paint(Graphics &g) {
	if (m_type == Type::unassigned) {
		g.setColour(COL_LIGHT);
		static constexpr auto stroke = 1.0f;
		g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(stroke / 2.0f), 4.0f, stroke);
		return;
	}

	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(m_asset, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void NumberSelector::resized() {
	auto bounds        = getLocalBounds();
	auto button_bounds = bounds.removeFromRight(H);
	m_up.setBounds(button_bounds.removeFromTop(proportionOfHeight(0.5f)));
	m_down.setBounds(button_bounds);
	m_display.setBounds(bounds);
}
