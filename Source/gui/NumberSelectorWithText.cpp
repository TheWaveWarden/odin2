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

#include "NumberSelectorWithText.h"
#include <JuceHeader.h>

//==============================================================================
NumberSelectorWithText::NumberSelectorWithText(NumberSelector::Type p_type) : NumberSelector(true, p_type) {
	m_display.setInlay(1);

	m_display.toParentMouseDown = [&](const MouseEvent e) {};
	m_display.toParentMouseDrag = [&](const MouseEvent e) {};
	m_display.setMouseCursor(juce::MouseCursor::StandardCursorType::NormalCursor);

	// m_display.toParentMouseUp = [&](const MouseEvent e) {};
	m_display.onMouseDown = [&]() {
		m_dropdown.clear();
		for (int index = m_legal_values.size() - 1; index >= 0; --index) {
			//ugly hack to
			// a) avoid 0 being used as value (reserved for "nothing selected")
			// b) make ordering inside menu the same as up/down buttons
			m_dropdown.addItem(9999 - m_legal_values[index], valueToText(m_legal_values[index]));
		}
		int selected = 9999 - m_dropdown.show();
		if (selected != 9999) {
			DBG(selected);
			setValue(selected);
		}
	};

	addAndMakeVisible(m_display);

	m_up.setClickingTogglesState(false);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { increment(); };

	m_down.setClickingTogglesState(false);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { decrement(); };

	m_buttons_right = true;

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	m_dropdown.setLookAndFeel(&m_menu_feels);

	m_value = 1;
}

NumberSelectorWithText::~NumberSelectorWithText() {
	m_dropdown.setLookAndFeel(nullptr);
}

void NumberSelectorWithText::increment() {
	setValue(m_increment_map[m_value]);
}

void NumberSelectorWithText::decrement() {
	setValue(m_decrement_map[m_value]);
}

void NumberSelectorWithText::setValue(int p_value) {
	//just some safety...
	bool value_legal = false;
	for (auto value : m_legal_values) {
		value_legal = (p_value == value) ? true : value_legal;
	}
	if (!value_legal) {
		//this should never happen (unless patch with unison = 12 is loaded)
		int nearest_value  = 0;
		float nearest_dist = 999;
		for (auto value : m_legal_values) {
			if (fabs(value - p_value) < nearest_dist) {
				nearest_value = value;
				nearest_dist  = fabs(value - p_value);
			}
		}
		m_value = nearest_value;
	} else {
		m_value = p_value;
	}
	//m_display.setText("Unison: " + std::to_string(m_value));
	m_display.setText(valueToText(m_value));
	OnValueChange(m_value);
}