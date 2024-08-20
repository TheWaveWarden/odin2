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

#include "GlassDropdown.h"
#include "../JuceLibraryCode/JuceHeader.h"

GlassDropdown::GlassDropdown() {
	setLookAndFeel(&m_menu_feels);
}

GlassDropdown::~GlassDropdown() {
	setLookAndFeel(nullptr);
}

void GlassDropdown::paint(Graphics &g) {
	g.setColour(COL_LIGHT);
	g.drawRect(getLocalBounds(), 1);
	g.setFont(H * 0.6f);

	auto text_bounds = getLocalBounds();
	text_bounds.removeFromRight(H).reduced(proportionOfHeight(0.4f));

	paintTriangle(g);

	if (getSelectedId() == 0) {
		g.drawText(m_default_text, text_bounds, Justification::centred, true);
	} else {
		g.drawText(getText(), text_bounds, Justification::centred, true);
	}
	g.drawImageAt(m_glaspanel, 0, 0);
}

void GlassDropdown::paintTriangle(juce::Graphics &g) {
	auto triangle_bounds = getLocalBounds().removeFromRight(H).reduced(proportionOfHeight(0.4f));

	// draw the triangle
	juce::Path triangle;
	triangle.addTriangle(triangle_bounds.getX(),
	                     triangle_bounds.getY(),
	                     triangle_bounds.getRight(),
	                     triangle_bounds.getY(),
	                     triangle_bounds.getX() + triangle_bounds.getWidth() / 2,
	                     triangle_bounds.getBottom());
	g.fillPath(triangle);
}
