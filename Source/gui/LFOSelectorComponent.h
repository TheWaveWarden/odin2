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
#include "LFODisplayComponent.h"
#include "LFOPopupLookAndFeel.h"
#include "OdinButton.h"


class LFOSelectorComponent : public Component, public AudioProcessorValueTreeState::Listener {
public:
	LFOSelectorComponent();
	~LFOSelectorComponent();

	void parameterChanged(const String &parameterID, float newValue) override {
		if (parameterID == m_parameter_id) {
			setValueGUIOnly(newValue);
		}
	}

	void resized() override;
	void paint(Graphics &) override;

	void setValueGUIOnly(int p_value) {
		if (p_value < m_display.getNrOfWaves() && p_value >= 0) {
			m_value = p_value;
			m_display.setValue(p_value);
		}
	}

	void setValue(int p_value) {
		if (p_value < m_display.getNrOfWaves() && p_value >= 0) {
			m_value = p_value;
			m_display.setValue(p_value);
			OnValueChange(p_value);
		}
	}

	void setTooltip(const String p_text) {
		m_display.setTooltip(p_text);
	}

	std::function<void(int)> OnValueChange = [](int) {};
	void setParameterId(String p_id) {
		m_parameter_id = p_id;
	}

protected:
	int mouse_reference_value = 0;
	int m_drag_initial_value  = 0;

	String m_parameter_id;
	void increment() {
		if (m_value + 1 < m_display.getNrOfWaves()) {
			m_display.setValue(++m_value);
			OnValueChange(m_value);
		}
	}

	void decrement() {
		if (m_value > 0) {
			m_display.setValue(--m_value);
			OnValueChange(m_value);
		}
	}

	LFODisplayComponent m_display;
	LFOPopupLookAndFeel m_popup_look_and_feel;
	PopupMenu m_popup;

	OdinButton m_up;
	OdinButton m_down;

	int m_value = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOSelectorComponent)
};
