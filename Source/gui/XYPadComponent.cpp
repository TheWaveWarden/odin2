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

#include "XYPadComponent.h"
#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "UIAssetManager.h"

XYPadComponent::XYPadComponent(AudioProcessorValueTreeState &vts, const std::string &p_param_prefix, OdinKnob &p_x, OdinKnob &p_y, bool p_vector_pad) :
    m_value_tree(vts),
    m_param_name_x(p_param_prefix + "x"),
    m_param_name_y(p_param_prefix + "y"),
    m_knob_x(p_x),
    m_knob_y(p_y),
    m_vector_pad(p_vector_pad) {
	m_color = juce::Colour(30, 30, 30);
}

XYPadComponent::~XYPadComponent() {
}

void XYPadComponent::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

	if (m_vector_pad) {
		g.setColour(COL_TEXT_BLUE);
		g.setFont(Helpers::getAldrichFont(H / 11.0f));
		const auto bounds = getLocalBounds().reduced(H / 20.0f);
		g.drawText("A", bounds, Justification::bottomLeft, false);
		g.drawText("B", bounds, Justification::topLeft, false);
		g.drawText("C", bounds, Justification::topRight, false);
		g.drawText("D", bounds, Justification::bottomRight, false);
	}

	const auto grid     = ConfigFileManager::getInstance().getOptionGuiScale();
	int handle_diameter = 2 * grid;
	int handle_inlay    = handle_diameter / 6.0f;

	float x_handle = handle_inlay + m_value_x * (getWidth() - handle_diameter - 2 * handle_inlay);
	float y_handle = getHeight() - handle_diameter - (handle_inlay + m_value_y * (getHeight() - handle_diameter - 2 * handle_inlay));
	if (!m_vector_pad) {
		g.setColour(Colour(60, 90, 120));
		g.drawLine(m_inlay, y_handle + handle_diameter / 2, getWidth() - m_inlay, y_handle + handle_diameter / 2);
		g.drawLine(x_handle + handle_diameter / 2, m_inlay, x_handle + handle_diameter / 2, getHeight() - m_inlay);
	}
	g.setColour(juce::Colours::white);
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::screen_dot, grid), x_handle, y_handle);
}

void XYPadComponent::mouseDrag(const MouseEvent &event) {
	mouseInteraction();
}

void XYPadComponent::mouseDown(const MouseEvent &event) {
	//needed so the host knows we are "touching" the knobs
	m_value_tree.getParameter(m_param_name_x)->beginChangeGesture();
	m_value_tree.getParameter(m_param_name_y)->beginChangeGesture();

	mouseInteraction();
	m_lock_set_XY_while_drawing = true;
}

void XYPadComponent::mouseInteraction() {
	juce::Point<int> mouse_pos = getMouseXYRelative();

	int handle_inlay    = proportionOfWidth(0.02f);
	int handle_diameter = proportionOfWidth(0.03f);

	m_value_x = (float)(mouse_pos.getX() - handle_inlay - handle_diameter / 2) / (float)(getWidth() - handle_diameter - 2 * handle_inlay);
	m_value_x = m_value_x < 0 ? 0 : m_value_x;
	m_value_x = m_value_x > 1 ? 1 : m_value_x;

	m_value_y = (float)(mouse_pos.getY() + handle_diameter / 2 - getHeight() + handle_inlay) / ((float)(2 * handle_inlay + handle_diameter - getHeight()));
	m_value_y = m_value_y < 0 ? 0 : m_value_y;
	m_value_y = m_value_y > 1 ? 1 : m_value_y;

	SETAUDIO0TO1(m_param_name_x, m_value_x);
	SETAUDIO0TO1(m_param_name_y, m_value_y);

	repaint();
}

void XYPadComponent::mouseUp(const MouseEvent &event) {
	m_lock_set_XY_while_drawing = false;
	Component::mouseUp(event);

	m_value_tree.getParameter(m_param_name_x)->endChangeGesture();
	m_value_tree.getParameter(m_param_name_y)->endChangeGesture();
}

void XYPadComponent::setImage(juce::Image p_panel) {
	m_panel = p_panel;
}

void XYPadComponent::setInlay(int p_inlay) {
	m_inlay = p_inlay;
}

void XYPadComponent::setX(float p_x) {
	if (!m_lock_set_XY_while_drawing) {
		m_value_x = p_x;
		repaint();
	}
}
void XYPadComponent::setY(float p_y) {
	if (!m_lock_set_XY_while_drawing) {
		m_value_y = p_y;
		repaint();
	}
}

void XYPadComponent::setColor(juce::Colour p_color) {
	m_color = p_color;
}

void XYPadComponent::setLogoImage(juce::Image p_image) {
	m_logo      = p_image;
	m_draw_logo = true;
}