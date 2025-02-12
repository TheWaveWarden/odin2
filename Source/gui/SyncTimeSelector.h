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

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlasDisplay.h"
#include "OdinButton.h"
#include "OdinFeels.h"

class SyncTimeSelector : public Component {
public:
	SyncTimeSelector(UIAssets::Indices p_asset);
	~SyncTimeSelector();

	void paint(Graphics &g) override;

	void setColor(juce::Colour p_color) {
		m_display.setColor(p_color);
	}

	void setValueLeft(int p_value) {
		if (p_value < m_left_values.size() && p_value >= 0) {
			m_value_left = p_value;
			updateDisplay();
			OnValueChange(m_value_left, m_value_right);
		}
	}

	void setValueRight(int p_value) {
		if (p_value < m_right_values.size() && p_value >= 0) {
			DBG("SETVALUERIGHT " + String(p_value));
			m_value_right = p_value;
			updateDisplay();
			OnValueChange(m_value_left, m_value_right);
		}
	}

	void setValues(int p_left, int p_right) {
		m_value_left  = p_left;
		m_value_right = p_right;
		updateDisplay();
		OnValueChange(m_value_left, m_value_right);
	}

	void setTooltip(const String p_text) {
		m_display.setTooltip(p_text);
	}

	std::function<void(int, int)> OnValueChange = [](int, int) {};

	void resized() override;

	void enablementChanged() override {
		repaint();
	}

private:
	void updateDisplay() {
		m_display.setText(m_left_values[m_value_left] + "/" + m_right_values[m_value_right]);
	}

	void incrementLeft() {
		if (m_value_left + 1 < m_left_values.size()) {
			++m_value_left;
			updateDisplay();
			OnValueChange(m_value_left, m_value_right);
		}
	}

	void incrementRight() {
		if (m_value_right + 1 < m_right_values.size()) {
			++m_value_right;
			updateDisplay();
			OnValueChange(m_value_left, m_value_right);
		}
	}

	void decrementLeft() {
		if (m_value_left > 0) {
			--m_value_left;
			updateDisplay();
			OnValueChange(m_value_left, m_value_right);
		}
	}

	void decrementRight() {
		if (m_value_right > 0) {
			--m_value_right;
			updateDisplay();
			OnValueChange(m_value_left, m_value_right);
		}
	}

	std::vector<std::string> m_right_values = {"1", "2", "4", "8", "8T", "16", "16T", "32", "32T"};
	std::vector<std::string> m_left_values  = {"1", "2", "3", "4", "5", "6", "7", "8"};

	void setValuesFromDropdown(int p_dropdown_return);

	GlasDisplay m_display;
	OdinMenuFeels m_menu_feels;

	PopupMenu m_dropdown;

	OdinButton m_up_left;
	OdinButton m_down_left;
	OdinButton m_up_right;
	OdinButton m_down_right;

	int m_value_left  = 2;
	int m_value_right = 5;

	const UIAssets::Indices m_asset;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SyncTimeSelector)
};
