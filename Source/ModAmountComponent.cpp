/*
  ==============================================================================

    ModAmountComponent.cpp
    Created: 3 Mar 2019 2:02:02am
    Author:  frot

  ==============================================================================
*/

#include "ModAmountComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream

//==============================================================================
ModAmountComponent::ModAmountComponent() {
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	// setDoubleClickReturnValue(true, 0, ModifierKeys::ctrlModifier);
}

ModAmountComponent::~ModAmountComponent() {
}

void ModAmountComponent::paint(Graphics &g) {
    MAC_INTERPOLATION_QUALITY(g)
	g.setColour(m_color);
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay, m_inlay);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay, -m_inlay);
	g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); // pmai

	if (m_value > 0) {
		g.setColour(m_color_bar);
		bottom_right.addXY(-(getWidth() - m_inlay * 2) * (1.f - m_value), -m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	} else {
		g.setColour(m_color_bar_negative);
		top_left.addXY((getWidth() - m_inlay * 2) * (1 + m_value), m_inlay);
		g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
	}

	Font current_font = g.getCurrentFont();
	current_font.setStyleFlags(1); //bold
	g.setFont(current_font);
	g.setFont(12.0f);
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << m_value * 100;
	std::string value_string = stream.str();
	g.setColour(Colours::white);
	if (value_string == "0") {
		g.setColour(juce::Colours::lightgrey);
	}
	g.drawText(value_string, getLocalBounds(), Justification::centred, true);

	g.drawImageAt(m_glas_panel, 0, 0);
}


void ModAmountComponent::mouseDrag(const MouseEvent &event) {
	m_value = m_drag_start_value + (m_drag_start_y - getMouseXYRelative().getY()) * DRAG_SCALAR;
	m_value = m_value > 1 ? 1 : m_value;
	m_value = m_value < -1 ? -1 : m_value;
	onValueChange(m_value);
	repaint();
}

void ModAmountComponent::parameterChanged(const String &parameterID, float newValue) {
	if (parameterID == m_parameter_id) {
		setValue(newValue);
	}
}
