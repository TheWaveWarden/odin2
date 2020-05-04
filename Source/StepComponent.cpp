/*
  ==============================================================================

    StepComponent.cpp
    Created: 4 May 2020 1:23:41am
    Author:  frederik_siepe

  ==============================================================================
*/

#include "StepComponent.h"
#include <JuceHeader.h>

//==============================================================================
StepComponent::StepComponent() {
	addAndMakeVisible(m_mod);
	m_mod.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mod.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mod.setNumDecimalPlacesToDisplay(3);
	m_mod.setKnobTooltip("Pans the sound to the\nleft or the right");

  addAndMakeVisible(m_transpose);
	m_transpose.setSliderStyle(Slider::RotaryVerticalDrag);
	m_transpose.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_transpose.setNumDecimalPlacesToDisplay(3);
	m_transpose.setKnobTooltip("Pans the sound to the\nleft or the right");
}

StepComponent::~StepComponent() {
}

void StepComponent::paint(Graphics &g) {
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1); // draw an outline around the component
}

void StepComponent::resized() {
}

void StepComponent::setGUIBig() {
	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_small_150_png, BinaryData::black_knob_small_150_pngSize);
	m_mod.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_mod.setBounds(KNOB_POS_X, MOD_POS_Y, BLACK_KNOB_SMALL_SIZE_X * 1.5, BLACK_KNOB_SMALL_SIZE_Y * 1.5);

  m_transpose.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_transpose.setBounds(KNOB_POS_X, TRANSPOSE_POS_Y, BLACK_KNOB_MID_SIZE_X * 1.5, BLACK_KNOB_MID_SIZE_Y * 1.5);
}

void StepComponent::setGUISmall() {
}
