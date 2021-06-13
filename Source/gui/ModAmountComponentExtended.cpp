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

#include "ModAmountComponentExtended.h"
#include "../GlobalIncludes.h"

ModAmountComponentExtended::ModAmountComponentExtended() {
	m_input_limit = 3.f;
}

void ModAmountComponentExtended::setColorBarsExtended(juce::Colour p_col_pos_1,
                                                      juce::Colour p_col_pos_2,
                                                      juce::Colour p_col_pos_3,
                                                      juce::Colour p_col_neg_1,
                                                      juce::Colour p_col_neg_2,
                                                      juce::Colour p_col_neg_3) {
	m_cols_pos[0] = p_col_pos_1;
	m_cols_pos[1] = p_col_pos_2;
	m_cols_pos[2] = p_col_pos_3;
	m_cols_neg[0] = p_col_neg_1;
	m_cols_neg[1] = p_col_neg_2;
	m_cols_neg[2] = p_col_neg_3;
}

void ModAmountComponentExtended::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)
	g.setColour(m_color);
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay, m_inlay + m_inlay_top);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay, -m_inlay - m_inlay_bottom);
	g.fillRect(juce::Rectangle<int>(top_left, bottom_right));

	if (m_value > 0 && m_value <= 1) {
		g.setColour(m_cols_pos[0]);
		bottom_right.addXY(-(getWidth() - m_inlay * 2) * (1.f - m_value), -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	} else if (m_value > 1 && m_value <= 2) {
		g.setColour(m_cols_pos[0]);
		bottom_right.addXY(0, -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
		g.setColour(m_cols_pos[1]);
		bottom_right.addXY(-(getWidth() - m_inlay * 2) * (2.f - m_value), 0);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	} else if (m_value > 2 && m_value <= 3) {
		g.setColour(m_cols_pos[1]);
		bottom_right.addXY(0, -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
		g.setColour(m_cols_pos[2]);
		bottom_right.addXY(-(getWidth() - m_inlay * 2) * (3.f - m_value), 0);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	}

	else if (m_value < 0 && m_value >= -1) {
		g.setColour(m_cols_neg[0]);
		top_left.addXY((getWidth() - m_inlay * 2) * (1 + m_value), m_inlay - m_inlay_bottom);
		bottom_right.addXY(0, -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	} else if (m_value < -1 && m_value >= -2) {
		g.setColour(m_cols_neg[0]);
		top_left.addXY(0, m_inlay - m_inlay_bottom);
		bottom_right.addXY(0, -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
		g.setColour(m_cols_neg[1]);
		top_left.addXY((getWidth() - m_inlay * 2) * (2 + m_value), 0);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	} else if (m_value < -2 && m_value >= -3) {
		g.setColour(m_cols_neg[1]);
		top_left.addXY(0, m_inlay - m_inlay_bottom);
		bottom_right.addXY(0, -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
		g.setColour(m_cols_neg[2]);
		top_left.addXY((getWidth() - m_inlay * 2) * (3 + m_value), 0);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	}

	Font current_font = g.getCurrentFont();
	current_font.setStyleFlags(1); //bold
	g.setFont(current_font);
	if (m_GUI_big) {
		g.setFont(18.0f);
	} else {
		g.setFont(12.0f);
	}
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << m_value * 100;
	std::string value_string = stream.str();
	g.setColour(Colours::white);
	if (value_string == "0") {
		g.setColour(juce::Colours::lightgrey);
	}
	g.drawText(value_string, getLocalBounds(), Justification::centred, true);

	g.drawImageAt(m_glas_panel, 0, 0);
}