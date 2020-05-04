/*
  ==============================================================================

    ArpComponent.cpp
    Created: 4 May 2020 1:23:25am
    Author:  frederik_siepe

  ==============================================================================
*/

#include "ArpComponent.h"
#include <JuceHeader.h>

//==============================================================================
ArpComponent::ArpComponent() {
	for (int step = 0; step < NUMBER_OF_STEPS; ++step) {
		addAndMakeVisible(m_steps[step]);
	}
}

ArpComponent::~ArpComponent() {
}

void ArpComponent::paint(Graphics &g) {

	// g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

	 g.setColour (Colours::grey);
	 g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

	// g.setColour (Colours::white);
	// g.setFont (14.0f);
	// g.drawText ("ArpComponent", getLocalBounds(),
	//             Justification::centred, true);   // draw some placeholder text
}

void ArpComponent::resized() {
	// This method is where you should set the bounds of any child
	// components that your component contains..
}

void ArpComponent::setGUIBig() {
	m_GUI_big = true;
	for (int step = 0; step < NUMBER_OF_STEPS; ++step) {

	m_steps[step].setBounds(STEP_COMPONENT_POS_X + step * STEP_COMPONENT_WIDTH,
		                       STEP_COMPONENT_POS_Y,
		                       STEP_COMPONENT_WIDTH,
		                       STEP_COMPONENT_HEIGHT);
    m_steps[step].setGUIBig();
	}
}
void ArpComponent::setGUISmall() {
	m_GUI_big = false;
}