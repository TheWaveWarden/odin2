/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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

#include "ModAmountComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream

ModAmountComponent::ModAmountComponent() {
}

ModAmountComponent::~ModAmountComponent() {
}

void ModAmountComponent::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)
	g.setColour(m_color);
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay, m_inlay + m_inlay_top);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay, -m_inlay - m_inlay_bottom);
	g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); // pmai

	if (m_value > 0) {
		g.setColour(m_color_bar);
		bottom_right.addXY(-(getWidth() - m_inlay * 2) * (1.f - m_value), -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	} else if (m_value < 0) {
		g.setColour(m_color_bar_negative);
		top_left.addXY((getWidth() - m_inlay * 2) * (1 + m_value), m_inlay - m_inlay_bottom);
		bottom_right.addXY(0, -m_inlay);
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

void ModAmountComponent::mouseDrag(const MouseEvent &event) {
	float drag_scalar = DRAG_SCALAR;
	if (m_GUI_big) {
		drag_scalar *= 0.66f;
	}
	m_value = m_drag_start_value + (m_drag_start_y - getMouseXYRelative().getY()) * drag_scalar;
	m_value = m_value > 1 ? 1 : m_value;
	m_value = m_value < -1 ? -1 : m_value;
	onValueChange(m_value);
	repaint();
}

void ModAmountComponent::parameterChanged(const String &parameterID, float newValue) {
	if (parameterID == m_parameter_id) {
		setValue(newValue);
	}
}
