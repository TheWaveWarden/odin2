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

#include "WavedrawDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

WavedrawDisplay::WavedrawDisplay() {
	setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
}

WavedrawDisplay::~WavedrawDisplay() {
}

void WavedrawDisplay::paint(Graphics &g) {
	int draw_inlay_left    = DRAW_INLAY_LEFT;
	int draw_inlay_up      = DRAW_INLAY_UP;
	int draw_inlay_down    = DRAW_INLAY_DOWN;
	int wavedraw_thiccness = WAVEDRAW_THICCNESS;

	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + 1, m_inlay);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay - 1, -m_inlay);
	g.setColour(COL_LIGHT);
	g.drawRect(juce::Rectangle<int>(top_left, bottom_right), 1);

	float width  = (float)(getWidth() - draw_inlay_left - DRAW_INLAY_RIGHT) / (float)WAVEDRAW_STEPS_X;
	float height = (float)(getHeight() - draw_inlay_up - draw_inlay_down) / 2.f;
	float mid    = (float)getHeight() / 2.f;

	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {

		if (i != WAVEDRAW_STEPS_X - 1) {

			//draw backgorund shade as path
			Path path;
			path.startNewSubPath(draw_inlay_left + (i)*width, mid);
			path.lineTo(draw_inlay_left + (i)*width, mid - m_draw_values[i] * height);
			path.lineTo(draw_inlay_left + (i + 1) * width, mid - m_draw_values[i + 1] * height);
			path.lineTo(draw_inlay_left + (i + 1) * width, mid);
			path.closeSubPath();

			g.setColour(COL_LIGHT.withAlpha(0.3f));
			g.fillPath(path);

			g.setColour(COL_LIGHT);

			g.drawLine(draw_inlay_left + (i)*width,
			           mid - m_draw_values[i] * height,
			           draw_inlay_left + (i + 1) * width,
			           mid - m_draw_values[i + 1] * height,
			           wavedraw_thiccness);
		} else {
			g.drawLine(draw_inlay_left + (i)*width,
			           mid - m_draw_values[i] * height,
			           draw_inlay_left + (i + 1) * width,
			           mid - m_draw_values[0] * height,
			           wavedraw_thiccness);
		}
	}
}

void WavedrawDisplay::mouseDrag(const MouseEvent &event) {
	mouseInteraction();
}

void WavedrawDisplay::mouseDown(const MouseEvent &event) {
	mouseInteraction();
}

int max_int_wavedraw(int a, int b) {
	return a < b ? b : a;
} // shouldnt be here
int min_int_wavedraw(int a, int b) {
	return a > b ? b : a;
} // shouldnt be here

void WavedrawDisplay::mouseInteraction() {

	// get Mouse data
	juce::Point<int> mouse_pos = getMouseXYRelative();
	float x                    = mouse_pos.getX();
	float y                    = mouse_pos.getY();

	int draw_inlay_left = DRAW_INLAY_LEFT;
	int draw_inlay_up   = DRAW_INLAY_UP;
	int draw_inlay_down = DRAW_INLAY_DOWN;

	if (m_GUI_big) {
		draw_inlay_left = 5;
		draw_inlay_up   = 7;
		draw_inlay_down = 5;
	}

	// clamp values
	y = y < draw_inlay_up ? draw_inlay_up : y;
	y = y > getHeight() - draw_inlay_down ? getHeight() - draw_inlay_down : y;
	x = x > draw_inlay_left ? x : draw_inlay_left + 1;
	x = x < getWidth() - DRAW_INLAY_RIGHT ? x : getWidth() - DRAW_INLAY_RIGHT - 1;

	float float_x = (x - draw_inlay_left) / (getWidth() - draw_inlay_left - DRAW_INLAY_RIGHT);
	int step_x    = (int)(floor(float_x * WAVEDRAW_STEPS_X));

	float float_y = (y - draw_inlay_up) / (getHeight() - draw_inlay_up - draw_inlay_down);
	float_y       = 2 * (0.5 - float_y);

	if (m_mouse_was_down) {
		int min_x     = min_int_wavedraw(step_x, m_last_x_value);
		int max_x     = max_int_wavedraw(step_x, m_last_x_value);
		float range_x = max_x - min_x;

		float min_y   = step_x > m_last_x_value ? m_last_y_value : float_y;
		float max_y   = step_x >= m_last_x_value ? float_y : m_last_y_value;
		float range_y = max_y - min_y;

		if (range_x > 0) {
			for (int i = min_x; i <= max_x; ++i) {
				// m_draw_values[i] = float_y;
				m_draw_values[i] = min_y + (range_y) * (float)(i - min_x) / (float)range_x;
			}
		} else {
			m_draw_values[step_x] = float_y;
		}
	} else {
		m_draw_values[step_x] = float_y;
	}
	m_last_x_value = step_x;
	m_last_y_value = float_y;

	onDraw();
	repaint();

	m_mouse_was_down = true;
}

void WavedrawDisplay::mouseUp(const MouseEvent &event) {
	m_mouse_was_down = false;
}

float *WavedrawDisplay::getDrawnTable() {
	return m_draw_values;
}

void WavedrawDisplay::setGUIBig() {
	m_GUI_big   = true;
	m_glaspanel = ImageCache::getFromMemory(BinaryData::drawpanel_150_png, BinaryData::drawpanel_150_pngSize);

	setSize(m_glaspanel.getWidth(), m_glaspanel.getHeight());
}
void WavedrawDisplay::setGUISmall() {
	m_GUI_big   = false;
	m_glaspanel = ImageCache::getFromMemory(BinaryData::drawpanel_png, BinaryData::drawpanel_pngSize);

	setSize(m_glaspanel.getWidth(), m_glaspanel.getHeight());
}