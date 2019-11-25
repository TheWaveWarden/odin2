/*
  ==============================================================================

    XYPadComponent.cpp
    Created: 27 Feb 2019 8:45:18pm
    Author:  frot

  ==============================================================================
*/

#include "XYPadComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
XYPadComponent::XYPadComponent(Knob &p_x, Knob &p_y, bool p_vector_pad) :
    m_knob_x(p_x), m_knob_y(p_y), m_vector_pad(p_vector_pad) {
}

XYPadComponent::~XYPadComponent() {
}

void XYPadComponent::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

	g.setColour(m_color);
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + 1, m_inlay);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay, -m_inlay);
	g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //

	if(m_draw_logo){
		g.drawImageAt(m_logo, 0, -5);
	}

	if (m_vector_pad) {
		g.setColour(juce::Colours::lightgrey);
		g.setFont(12.0f);
		g.drawText("A", VECTOR_LEFT, VECTOR_DOWN, 8, 15, Justification::topLeft);
		g.drawText("B", VECTOR_LEFT, VECTOR_UP, 8, 15, Justification::topLeft);
		g.drawText("C", VECTOR_RIGHT, VECTOR_UP, 8, 15, Justification::topLeft);
		g.drawText("D", VECTOR_RIGHT, VECTOR_DOWN, 8, 15, Justification::topLeft);
	}

	float x_handle = HANDLE_INLAY + m_value_x * (getWidth() - HANDLE_DIAMETER - 2 * HANDLE_INLAY);
	float y_handle =
	    getHeight() - HANDLE_DIAMETER - (HANDLE_INLAY + m_value_y * (getHeight() - HANDLE_DIAMETER - 2 * HANDLE_INLAY));
	// if (!m_vector_pad) {
	// 	g.setColour(Colour(0, 10, 30));
	// 	g.drawLine(m_inlay, y_handle + HANDLE_DIAMETER / 2, getWidth() - m_inlay, y_handle + HANDLE_DIAMETER / 2);
	// 	g.drawLine(x_handle + HANDLE_DIAMETER / 2, m_inlay, x_handle + HANDLE_DIAMETER / 2, getHeight() - m_inlay);
	// }
	g.setColour(Colours::white);
	g.fillEllipse(x_handle, y_handle, HANDLE_DIAMETER, HANDLE_DIAMETER);

	g.drawImageAt(m_panel, 0, 0);
}

void XYPadComponent::mouseDrag(const MouseEvent &event) {
	mouseInteraction();
}

void XYPadComponent::mouseDown(const MouseEvent &event) {
	mouseInteraction();
	m_lock_set_XY_while_drawing = true;
}

void XYPadComponent::mouseInteraction() {
	juce::Point<int> mouse_pos = getMouseXYRelative();
	m_value_x                  = (float)(mouse_pos.getX() - HANDLE_INLAY - HANDLE_DIAMETER / 2) /
	            (float)(getWidth() - HANDLE_DIAMETER - 2 * HANDLE_INLAY);
	m_value_x = m_value_x < 0 ? 0 : m_value_x;
	m_value_x = m_value_x > 1 ? 1 : m_value_x;

	m_value_y = (float)(mouse_pos.getY() + HANDLE_DIAMETER / 2 - getHeight() + HANDLE_INLAY) /
	            ((float)(2 * HANDLE_INLAY + HANDLE_DIAMETER - getHeight()));
	m_value_y = m_value_y < 0 ? 0 : m_value_y;
	m_value_y = m_value_y > 1 ? 1 : m_value_y;

	m_knob_x.setValue(m_value_x);
	m_knob_y.setValue(m_value_y);

	repaint();
}

void XYPadComponent::mouseUp(const MouseEvent &event) {
	m_lock_set_XY_while_drawing = false;
	Component::mouseUp(event);
}
