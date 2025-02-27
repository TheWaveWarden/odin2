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
#include "GlasDisplay.h"
#include "OdinButton.h"

class NumberSelector : public Component, public AudioProcessorValueTreeState::Listener {
public:
	enum class Type {
		unassigned,
		selector_16x4,
		selector_14x4,
		selector_12x4,
		selector_8x4,
	};

	NumberSelector(bool p_buttons_right, Type p_type = Type::unassigned);
	~NumberSelector();

	void enablementChanged() override {
		repaint();
	}

	void parameterChanged(const String &parameterID, float newValue) override {
		if (parameterID == m_parameter_id) {
			setValue(newValue);
		}
	}

	void paint(Graphics &) override;
	void resized() override;

	void setRange(int p_min, int p_max) {
		m_min = p_min;
		m_max = p_max;
	}

	virtual void setValue(int p_value) {
		if (p_value == m_value) {
			return; //avoid infinite loop
		}
		if (p_value >= m_min && p_value <= m_max) {
			m_value = p_value;

			m_display.setText(std::to_string(m_value));
			OnValueChange(p_value);
		}
	}

	void setTooltip(const String p_text) {
		m_display.setTooltip(p_text);
	}

	std::function<void(int)> OnValueChange = [](int) { DBG("not set......"); };

	void setParameterId(String p_id) {
		m_parameter_id = p_id;
	}

	void setMouseDragDivisor(float p_divisor) {
		m_mouse_drag_divisor = p_divisor;
	}

protected:
	float m_mouse_drag_divisor = 7.f;
	int mouse_reference_value  = 0;
	int m_drag_initial_value   = 0;

	String m_parameter_id;

	virtual void increment() {
		setValue(m_value + 1);
	}

	virtual void decrement() {
		setValue(m_value - 1);
	}

	GlasDisplay m_display;
	int m_min = 1;
	int m_max = 1;

	OdinButton m_up;
	OdinButton m_down;

	Type m_type;
	UIAssets::Indices m_asset;

	int m_value = 1;
	bool m_buttons_right;
	int m_display_width;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumberSelector)
};
