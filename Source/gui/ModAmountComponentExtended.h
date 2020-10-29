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

#pragma once

#include "ModAmountComponent.h"

class ModAmountComponentExtended : public ModAmountComponent {
public:
	ModAmountComponentExtended();

	void paint(Graphics &) override;
	void setColorBarsExtended(juce::Colour p_col_pos_1,
	                          juce::Colour p_col_pos_2,
	                          juce::Colour p_col_pos_3,
	                          juce::Colour p_col_neg_1,
	                          juce::Colour p_col_neg_2,
	                          juce::Colour p_col_neg_3);

protected:

    juce::Colour m_cols_pos[3];
    juce::Colour m_cols_neg[3];

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModAmountComponentExtended)
};
