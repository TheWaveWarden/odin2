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

#include "GlasDropdown.h"
#include "../JuceLibraryCode/JuceHeader.h"

GlasDropdown::GlasDropdown() {
	setLookAndFeel(&m_menu_feels);
}

GlasDropdown::~GlasDropdown() {
	setLookAndFeel(nullptr);
}

void GlasDropdown::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

	if (getSelectedId() == 0 && m_grey_first_element) {
		g.setColour(m_grey_background_color);
	} else {
		g.setColour(m_color);
	}
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + m_inlay_left, m_inlay - m_inlay_top);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay - 1, -m_inlay);
	g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //

	g.setColour(m_font_color);

	if (getSelectedId() == 0 && m_grey_first_element) {
		g.setColour(m_grey_color);
	}

	// g.setFont(bfont);Font current_font = g.getCurrentFont();
	Font current_font = g.getCurrentFont();
	current_font.setStyleFlags(1); // bold
	g.setFont(current_font);
	if (m_GUI_big) {
		g.setFont(18.0f);
	} else {
		g.setFont(12.0f);
	}

	if (getSelectedId() == 0) {
		g.drawText(m_default_text, getLocalBounds(), Justification::centred, true);
	} else {

		g.drawText(getText(), getLocalBounds(), Justification::centred, true);
	}
	g.drawImageAt(m_glaspanel, 0, 0);
}
