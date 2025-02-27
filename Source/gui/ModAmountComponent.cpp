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
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"

ModAmountComponent::ModAmountComponent() {
}

ModAmountComponent::~ModAmountComponent() {
}

void ModAmountComponent::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

	auto bounds       = getLocalBounds().toFloat();
	const auto corner = float(getHeight()) * 0.1f;

	const auto scaleCompensation = float(ConfigFileManager::getInstance().getOptionGuiScale()) / float(GuiScale::Z200);
	juce::BorderSize<float> border_size(3.0f * scaleCompensation, 3.0f * scaleCompensation, 1.0f * scaleCompensation, 2.0f * scaleCompensation);
	border_size.subtractFrom(bounds);

	if (m_value > 0) {
		g.setColour(m_color_bar.withAlpha(0.3f));
		bounds = bounds.removeFromLeft(bounds.proportionOfWidth(m_value));
		g.fillRoundedRectangle(bounds, corner);
	} else if (m_value < 0) {
		g.setColour(m_color_bar_negative.withAlpha(0.3f));
		bounds = bounds.removeFromRight(bounds.proportionOfWidth(-m_value));
		g.fillRoundedRectangle(bounds, corner);
	}

	g.setFont(Helpers::getAldrichFont(H / 1.8f));
	g.setColour(COL_TEXT_BLUE);
	g.drawText(juce::String(juce::roundToInt(m_value * 100.0f)), getLocalBounds(), Justification::centred, true);
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
