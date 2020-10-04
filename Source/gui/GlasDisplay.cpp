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

#include "GlasDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

GlasDisplay::GlasDisplay() {
	setLookAndFeel(&m_menu_feels);
}

GlasDisplay::~GlasDisplay() {
	setLookAndFeel(nullptr);
}

void GlasDisplay::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

	g.setColour(m_color);
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + 1, m_inlay  + m_inlay_top);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay, -m_inlay);
	g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //pmai
	g.setColour(Colours::white);
	Font current_font = g.getCurrentFont();
	current_font.setStyleFlags(1); //bold
	g.setFont(current_font);
	if (m_GUI_big) {
		g.setFont(18.0f);
	} else {
		g.setFont(12.0f);
	}
	juce::Rectangle<int> text_area = getLocalBounds();
	text_area.setY(text_area.getY() + m_text_offset_top);
	text_area.setX(text_area.getX() + m_text_offset_left);
	g.drawText(m_text, text_area, Justification::centred,
	           true); // draw some placeholder text

	g.drawImageAt(m_glas_panel, 0, 0);
}

void GlasDisplay::mouseDown(const MouseEvent &event) {
	onMouseDown();
	toParentMouseDown(event);
	Component::mouseDown(event);
}

void GlasDisplay::mouseDrag(const MouseEvent &event) {
	toParentMouseDrag(event);
	Component::mouseDrag(event);
}

void GlasDisplay::mouseUp(const MouseEvent &event) {
	toParentMouseUp(event);
	Component::mouseUp(event);
}
