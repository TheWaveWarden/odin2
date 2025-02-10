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
#define INLAY_DEF_MODAMOUNT 2

class ModAmountComponent : public Component,
                           public SettableTooltipClient,
                           public AudioProcessorValueTreeState::Listener {
public:
	ModAmountComponent();
	~ModAmountComponent();

	void paint(Graphics &) override;
	void setImage(juce::Image p_panel) {
		m_glas_panel = p_panel;
	}
	
    void setColor(juce::Colour p_color) {
		m_color = p_color;
	}
	void setColorBars(juce::Colour p_color, juce::Colour p_color_negative) {
		m_color_bar          = p_color;
		m_color_bar_negative = p_color_negative;
	}

	void setValue(float p_amount) {
		//using a lock here will crash win build on usage...
		//const MessageManagerLock mmLock;
		m_value = p_amount;
		repaint();
	}

	void reset() {
		m_value = 0;
		onValueChange(0);
		repaint();
	}

	void mouseDown(const MouseEvent &event) override {
		if (event.mods.isCtrlDown()) {
			reset();
		}
		m_drag_start_y     = getMouseXYRelative().getY();
		m_drag_start_value = m_value;
	}
	void mouseDrag(const MouseEvent &event) override;

	std::function<void()> onMouseDown        = []() {}; // overwriteable with lambda
	std::function<void(float)> onValueChange = [](float) {};

	void parameterChanged(const String &parameterID, float newValue) override;

	void setParameterId(String p_id) {
		m_parameter_id = p_id;
	}

protected:
	String m_parameter_id = "";

	juce::Colour m_color_bar;
	juce::Colour m_color_bar_negative;
	juce::Colour m_color = juce::Colours::black;
	int m_inlay          = INLAY_DEF_MODAMOUNT;
	int m_inlay_top      = 0;
	int m_inlay_bottom   = 0;
	juce::Image m_glas_panel;

	float m_value            = 0.f;
	float m_drag_start_value = 0.f;
	int m_drag_start_y       = 0;
	float m_input_limit      = 1.f;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModAmountComponent)
};
