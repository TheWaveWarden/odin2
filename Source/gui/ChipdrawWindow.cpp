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

#include "ChipdrawWindow.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>
//==============================================================================
ChipdrawWindow::ChipdrawWindow() {
	setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
}

ChipdrawWindow::~ChipdrawWindow() {
}

void ChipdrawWindow::paint(Graphics &g) {
	const int draw_inlay_left    = proportionOfWidth(DRAW_INLAY_HORZ_PROPORTION);
	const int draw_inlay_right   = draw_inlay_left;
	const int draw_inlay_up      = proportionOfHeight(DRAW_INLAY_VERT_PROPORION);
	const int draw_inlay_down    = proportionOfHeight(0.03f);
	const int chipdraw_thiccness = proportionOfWidth(DRAW_STROKE_PROPORION);

	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + 1, m_inlay);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay - 1, -m_inlay);

	const float width  = (float)(getWidth() - draw_inlay_left - draw_inlay_right) / float(CHIPDRAW_STEPS_X);
	const float height = (float)(getHeight() - draw_inlay_up - draw_inlay_down) / 2.f;
	const float mid    = draw_inlay_up + height;

	Path path;
	path.startNewSubPath(draw_inlay_left, mid - m_draw_values[0] * height);
	for (int i = 1; i < CHIPDRAW_STEPS_X; ++i) {
		path.lineTo(draw_inlay_left + i * width, mid - m_draw_values[i - 1] * height);
		path.lineTo(draw_inlay_left + i * width, mid - m_draw_values[i] * height);
	}
	path.lineTo(draw_inlay_left + CHIPDRAW_STEPS_X * width, mid - m_draw_values[CHIPDRAW_STEPS_X - 1] * height);

	const juce::Colour col(0xff86f6e8);

	// fill
	Path fill_path = path;
	fill_path.lineTo(getWidth() - draw_inlay_left, mid);
	fill_path.lineTo(draw_inlay_left, mid);
	fill_path.closeSubPath();
	g.setColour(col.withAlpha(0.15f));
	g.fillPath(fill_path);

	// stroke
	g.setColour(col.withAlpha(0.9f));
	g.strokePath(path, PathStrokeType(chipdraw_thiccness));
}

void ChipdrawWindow::mouseDrag(const MouseEvent &event) {
	mouseInteraction();
}

void ChipdrawWindow::mouseDown(const MouseEvent &event) {
	mouseInteraction();
}

float ChipdrawWindow::quantizeY(float p_y) {
	return std::round(p_y * CHIPDRAW_STEPS_Y) / float(CHIPDRAW_STEPS_Y);
}

void ChipdrawWindow::mouseInteraction() {
	int draw_inlay_left    = proportionOfWidth(DRAW_INLAY_HORZ_PROPORTION);
	int draw_inlay_right   = draw_inlay_left;
	int draw_inlay_up      = proportionOfHeight(DRAW_INLAY_VERT_PROPORION);
	int draw_inlay_down    = draw_inlay_up;
	int chipdraw_thiccness = proportionOfWidth(DRAW_STROKE_PROPORION);

	// get Mouse data
	juce::Point<int> mouse_pos = getMouseXYRelative();
	float x                    = mouse_pos.getX();
	float y                    = mouse_pos.getY();

	// clamp values
	y = y < draw_inlay_up ? draw_inlay_up : y;
	y = y > getHeight() - draw_inlay_down ? getHeight() - draw_inlay_down : y;
	x = x > draw_inlay_left ? x : draw_inlay_left + 1;
	x = x < getWidth() - draw_inlay_right ? x : getWidth() - draw_inlay_right - 1;

	float float_x = (x - draw_inlay_left) / (getWidth() - draw_inlay_left - draw_inlay_right);
	int step_x    = (int)(floor(float_x * CHIPDRAW_STEPS_X));

	float float_y = (y - draw_inlay_up) / (getHeight() - draw_inlay_up - draw_inlay_down);
	float_y       = 2 * (0.5 - float_y);

	//discretize y
	float_y = quantizeY(float_y);

	if (m_mouse_was_down) {
		int min_x     = juce::jmin(step_x, m_last_x_value);
		int max_x     = juce::jmax(step_x, m_last_x_value);
		float range_x = max_x - min_x;

		float min_y   = step_x > m_last_x_value ? m_last_y_value : float_y;
		float max_y   = step_x >= m_last_x_value ? float_y : m_last_y_value;
		float range_y = max_y - min_y;

		if (range_x > 0) {
			for (int i = min_x; i <= max_x; ++i) {
				m_draw_values[i] = quantizeY(min_y + (range_y) * (float)(i - min_x) / (float)range_x);
			}
		} else {
			m_draw_values[step_x] = float_y;
		}
	} else {
		m_draw_values[step_x] = float_y;
	}
	m_last_x_value = step_x;
	m_last_y_value = float_y;

	repaint();

	m_mouse_was_down = true;
}

void ChipdrawWindow::mouseUp(const MouseEvent &event) {
	m_mouse_was_down = false;
	onMouseUp();
}

float *ChipdrawWindow::getDrawnTable() {
	return m_draw_values;
}