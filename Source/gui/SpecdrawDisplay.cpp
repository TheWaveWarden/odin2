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

#include "SpecdrawDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

SpecdrawDisplay::SpecdrawDisplay() {
	setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
}

SpecdrawDisplay::~SpecdrawDisplay() {
}

void SpecdrawDisplay::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)
	g.setColour(m_color);
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + 1, m_inlay);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay - 1, -m_inlay);
	g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //

	g.setColour(m_draw_color);


	int draw_inlay_left_spec = DRAW_INLAY_LEFT_SPEC;
	if(m_GUI_big){
		draw_inlay_left_spec = 11;
	}

	float height = (float)(getHeight() - DRAW_INLAY_UP_SPEC - DRAW_INLAY_DOWN_SPEC);

	float width = SPECDRAW_THICCNESS;
	float draw_width = 2.8f;
	if(m_GUI_big){
		width = 6;
		float draw_width = 4.2f;
	}

	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		g.drawLine(draw_inlay_left_spec + i * width,
		           getHeight() - DRAW_INLAY_DOWN_SPEC - m_draw_values[i] * height,
		           draw_inlay_left_spec + i * width,
		           getHeight() - DRAW_INLAY_DOWN_SPEC,
		           draw_width);
	}

	g.drawImageAt(m_glaspanel, 0, 0);
}

void SpecdrawDisplay::mouseDrag(const MouseEvent &event) {
	mouseInteraction();
}

void SpecdrawDisplay::mouseDown(const MouseEvent &event) {
	mouseInteraction();
}

int max_int(int a, int b) {
	return a < b ? b : a;
}
int min_int(int a, int b) {
	return a > b ? b : a;
}

void SpecdrawDisplay::mouseInteraction() {
	int draw_inlay_left_spec = DRAW_INLAY_LEFT_SPEC;
	if(m_GUI_big){
		draw_inlay_left_spec = 10;
	}

	juce::Point<int> mouse_pos = getMouseXYRelative();

	float specdraw_thiccness = SPECDRAW_THICCNESS;
	float specdraw_x_offset = 6;
	if(m_GUI_big){
		specdraw_x_offset = 8;
		specdraw_thiccness = 6.f;
	}

	int x                      = (mouse_pos.getX() - specdraw_x_offset)/ specdraw_thiccness;
	float y                    = mouse_pos.getY();

	y = y < DRAW_INLAY_UP_SPEC ? DRAW_INLAY_UP_SPEC : y;
	y = y > getHeight() - DRAW_INLAY_DOWN_SPEC ? getHeight() - DRAW_INLAY_DOWN_SPEC : y;
	x = x < 0 ? 0 : x;
	x = x >= SPECDRAW_STEPS_X ? SPECDRAW_STEPS_X - 1 : x;

	float float_y =
	    (getHeight() - DRAW_INLAY_DOWN_SPEC - y) / ((float)getHeight() - (float)DRAW_INLAY_UP_SPEC - (float)DRAW_INLAY_DOWN_SPEC);
	if (m_mouse_was_down) {

		//DBG(std::to_string(x) + ", " + std::to_string(m_last_x_value));
		int max_x   = max_int(x, m_last_x_value);
		int min_x   = min_int(x, m_last_x_value);
		int range_x = max_x - min_x;

		float min_y   = x > m_last_x_value ? m_last_y_value : float_y;
		float max_y   = x >= m_last_x_value ? float_y : m_last_y_value;
		float range_y = max_y - min_y;

		if (range_x > 0) {
			for (int i = min_x; i <= max_x; ++i) {
				m_draw_values[i] = min_y + (range_y) * (float)(i - min_x) / (float)range_x;
			}
		} else {
			m_draw_values[x] = float_y;
		}
	} else {
		m_draw_values[x] = float_y;
	}
	m_last_x_value = x;
	m_last_y_value = float_y;

	onDraw();
	repaint();

	m_mouse_was_down = true;
}

void SpecdrawDisplay::mouseUp(const MouseEvent &event) {
	m_mouse_was_down = false;
}

float *SpecdrawDisplay::getDrawnTable() {
	return m_draw_values;
}

void SpecdrawDisplay::setGUIBig(){
	m_GUI_big = true;
	m_glaspanel = ImageCache::getFromMemory(BinaryData::drawpanel_150_png, BinaryData::drawpanel_150_pngSize);

	setSize(m_glaspanel.getWidth(), m_glaspanel.getHeight());
}
void SpecdrawDisplay::setGUISmall(){
	m_GUI_big = false;
	m_glaspanel = ImageCache::getFromMemory(BinaryData::drawpanel_png, BinaryData::drawpanel_pngSize);

	setSize(m_glaspanel.getWidth(), m_glaspanel.getHeight());
}