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
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"


ModAmountComponentExtended::ModAmountComponentExtended() {
	m_input_limit = 3.f;
}

void ModAmountComponentExtended::setColorBarsExtended(
    juce::Colour p_col_pos_1, juce::Colour p_col_pos_2, juce::Colour p_col_pos_3, juce::Colour p_col_neg_1, juce::Colour p_col_neg_2, juce::Colour p_col_neg_3) {
	m_cols_pos[0] = p_col_pos_1;
	m_cols_pos[1] = p_col_pos_2;
	m_cols_pos[2] = p_col_pos_3;
	m_cols_neg[0] = p_col_neg_1;
	m_cols_neg[1] = p_col_neg_2;
	m_cols_neg[2] = p_col_neg_3;
}

void ModAmountComponentExtended::paint(Graphics &g) {

	SET_INTERPOLATION_QUALITY(g)

	auto bounds       = getLocalBounds().toFloat();
	const auto corner = float(getHeight()) * 0.1f;

	const auto scaleCompensation = float(ConfigFileManager::getInstance().getOptionGuiScale()) / float(GuiScale::Z200);
	juce::BorderSize<float> border_size(3.0f * scaleCompensation, 3.0f * scaleCompensation, 1.0f * scaleCompensation, 2.0f * scaleCompensation);
	border_size.subtractFrom(bounds);

	if (m_value > 0 && m_value <= 1) {
		g.setColour(m_cols_pos[0].withAlpha(0.3f));
		bounds = bounds.removeFromLeft(bounds.proportionOfWidth(m_value));
		g.fillRoundedRectangle(bounds, corner);
	} else if (m_value > 1 && m_value <= 2) {
		g.setColour(m_cols_pos[0].withAlpha(0.3f));
		const auto bounds2 = bounds.removeFromLeft(bounds.proportionOfWidth(m_value - 1.0f));
		g.fillRoundedRectangle(bounds, corner);

		g.setColour(m_cols_pos[1].withAlpha(0.3f));
		g.fillRoundedRectangle(bounds2, corner);
	} else if (m_value > 2 && m_value <= 3) {
		g.setColour(m_cols_pos[1].withAlpha(0.3f));
		const auto bounds2 = bounds.removeFromLeft(bounds.proportionOfWidth(m_value - 2.0f));
		g.fillRoundedRectangle(bounds, corner);

		g.setColour(m_cols_pos[2].withAlpha(0.3f));
		g.fillRoundedRectangle(bounds2, corner);
	}

	else if (m_value < 0 && m_value >= -1) {
		g.setColour(m_cols_neg[0].withAlpha(0.3f));
		bounds = bounds.removeFromRight(bounds.proportionOfWidth(-m_value));
		g.fillRoundedRectangle(bounds, corner);
	} else if (m_value < -1 && m_value >= -2) {
		g.setColour(m_cols_neg[1].withAlpha(0.3f));
		const auto bounds2 = bounds.removeFromLeft(bounds.proportionOfWidth(2.0f + m_value));
		g.fillRoundedRectangle(bounds, corner);
		g.setColour(m_cols_neg[0].withAlpha(0.3f));
		g.fillRoundedRectangle(bounds2, corner);
	} else if (m_value < -2 && m_value >= -3) {
		g.setColour(m_cols_neg[2].withAlpha(0.3f));
		const auto bounds2 = bounds.removeFromLeft(bounds.proportionOfWidth(3.0f + m_value));
		g.fillRoundedRectangle(bounds, corner);

		g.setColour(m_cols_neg[1].withAlpha(0.3f));
		g.fillRoundedRectangle(bounds2, corner);
	}

	g.setFont(Helpers::getAldrichFont(H / 1.8f));
	g.setColour(COL_TEXT_BLUE);
	g.drawText(juce::String(juce::roundToInt(m_value * 100.0f)), getLocalBounds(), Justification::centred, true);
}