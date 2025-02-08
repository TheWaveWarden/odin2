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
#define INLAY_DEFAULT_WAVEDRAW 2

//==============================================================================
class WavedrawDisplay : public Component, public SettableTooltipClient {
public:
	WavedrawDisplay();
	~WavedrawDisplay();

	void paint(Graphics &) override;
	void setInlay(int p_inlay) {
		m_inlay = p_inlay;
	}
	void setColor(juce::Colour p_color) {
		m_color = p_color;
	}
	void setDrawColor(juce::Colour p_draw_color) {
		m_draw_color = p_draw_color;
	}

	void mouseDrag(const MouseEvent &event) override;
	void mouseDown(const MouseEvent &event) override;
	void mouseUp(const MouseEvent &event) override;
	void mouseInteraction();

	float *getDrawnTable();

	void setDrawnTable(float p_table[WAVEDRAW_STEPS_X]) {
		for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
			//DBG(p_table[i]);
			m_draw_values[i] = p_table[i];
		}
		repaint();
	}

	std::function<void()> onMouseUp = []() {};
private:
	static constexpr auto DRAW_INLAY_HORZ_PROPORTION = 0.015f;
	static constexpr auto DRAW_INLAY_VERT_PROPORION  = 0.06f;
	static constexpr auto DRAW_STROKE_PROPORION      = 0.01f;

	bool m_mouse_was_down = false;
	int m_last_x_value;
	float m_last_y_value;

	int m_inlay = INLAY_DEFAULT_WAVEDRAW;
	juce::Image m_glaspanel;
	juce::Colour m_color      = juce::Colours::black;
	juce::Colour m_draw_color = juce::Colours::white;
	juce::Colour m_fill_color = juce::Colour(0x473c9bc7);

	float m_draw_values[WAVEDRAW_STEPS_X] = {0};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WavedrawDisplay)
};
