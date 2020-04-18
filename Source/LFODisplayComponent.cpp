/*
  ==============================================================================

    LFODisplayComponent.cpp
    Created: 27 Feb 2019 6:43:33pm
    Author:  frot

  ==============================================================================
*/

#include "LFODisplayComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
LFODisplayComponent::LFODisplayComponent() {
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
}

LFODisplayComponent::~LFODisplayComponent() {
}

void LFODisplayComponent::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

	g.setColour(STANDARD_DISPLAY_COLOR);
	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + 1 + m_inlay_top, m_inlay + m_inlay_left);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay - 1, -m_inlay);

	g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); // pmai

	if (m_value < m_waves.size()) {
		g.drawImageAt((m_waves[m_value]), 0, 1);
	}

	g.drawImageAt(m_panel, 0, 0);
}

void LFODisplayComponent::addWave(juce::Image p_wave) {
	m_waves.push_back(p_wave.createCopy());
}

void LFODisplayComponent::mouseDown(const MouseEvent &event) {
	//onMouseDown();
	toParentMouseDown(event);
	Component::mouseDown(event);
}

void LFODisplayComponent::mouseDrag(const MouseEvent &event) {
	toParentMouseDrag(event);
	Component::mouseDrag(event);
}

void LFODisplayComponent::mouseUp(const MouseEvent &event) {
	toParentMouseUp(event);
	Component::mouseUp(event);
}