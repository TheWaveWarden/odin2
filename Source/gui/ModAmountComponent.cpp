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

#include "ModAmountComponent.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream

ModAmountComponent::ModAmountComponent() {
}

ModAmountComponent::~ModAmountComponent() {
}

void ModAmountComponent::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)
	g.setColour(m_color);
	juce::Point<float> top_left = getLocalBounds().getTopLeft().toFloat();
	top_left.addXY(m_inlay, m_inlay + m_inlay_top);
	juce::Point<float> bottom_right = getLocalBounds().getBottomRight().toFloat();
	bottom_right.addXY(-m_inlay, -m_inlay - m_inlay_bottom);

	const auto corner = float(getHeight()) * 0.1f;

	if (m_value > 0) {
		g.setColour(m_color_bar.withAlpha(0.3f));
		bottom_right.addXY(-(getWidth() - m_inlay * 2) * (1.f - m_value), -m_inlay);
		g.fillRoundedRectangle(juce::Rectangle<float>(top_left, bottom_right), corner);
	} else if (m_value < 0) {
		g.setColour(m_color_bar_negative.withAlpha(0.3f));
		top_left.addXY((getWidth() - m_inlay * 2) * (1 + m_value), m_inlay - m_inlay_bottom);
		bottom_right.addXY(0, -m_inlay);
		g.fillRoundedRectangle(juce::Rectangle<float>(top_left, bottom_right), corner);
	}

	g.setFont(H / 1.8f);
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << m_value * 100;
	std::string value_string = stream.str();
	g.setColour(COL_TEXT_BLUE);
	g.drawText(value_string, getLocalBounds(), Justification::centred, true);
}

void ModAmountComponent::mouseDrag(const MouseEvent &event) {
	float drag_scalar = H * 0.0002f;
	m_value           = m_drag_start_value + (m_drag_start_y - getMouseXYRelative().getY()) * drag_scalar;
	m_value           = m_value > m_input_limit ? m_input_limit : m_value;
	m_value           = m_value < -m_input_limit ? -m_input_limit : m_value;
	onValueChange(m_value);
	repaint();
}

void ModAmountComponent::parameterChanged(const String &parameterID, float newValue) {
	if (parameterID == m_parameter_id) {
		setValue(newValue);
	}
}
