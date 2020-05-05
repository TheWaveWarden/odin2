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
ArpComponent::ArpComponent(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts) :
    m_processor(p_processor), m_value_tree(vts), m_step_0(vts, 0), m_step_1(vts, 1), m_step_2(vts, 2), m_step_3(vts, 3),
    m_step_4(vts, 4), m_step_5(vts, 5), m_step_6(vts, 6), m_step_7(vts, 7), m_step_8(vts, 8), m_step_9(vts, 9),
    m_step_10(vts, 10), m_step_11(vts, 11), m_step_12(vts, 12), m_step_13(vts, 13), m_step_14(vts, 14),
    m_step_15(vts, 15)

{
	addAndMakeVisible(m_step_0);
	addAndMakeVisible(m_step_1);
	addAndMakeVisible(m_step_2);
	addAndMakeVisible(m_step_3);
	addAndMakeVisible(m_step_4);
	addAndMakeVisible(m_step_5);
	addAndMakeVisible(m_step_6);
	addAndMakeVisible(m_step_7);
	addAndMakeVisible(m_step_8);
	addAndMakeVisible(m_step_9);
	addAndMakeVisible(m_step_10);
	addAndMakeVisible(m_step_11);
	addAndMakeVisible(m_step_12);
	addAndMakeVisible(m_step_13);
	addAndMakeVisible(m_step_14);
	addAndMakeVisible(m_step_15);

	//todo start and stop timer when component is shown
	startTimer(10);
}

ArpComponent::~ArpComponent() {
}

void ArpComponent::paint(Graphics &g) {

	// g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1); // draw an outline around the component

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

		m_step_0.setBounds(STEP_COMPONENT_POS_X + 0 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_0.setGUIBig();

		m_step_1.setBounds(STEP_COMPONENT_POS_X + 1 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_1.setGUIBig();

		m_step_2.setBounds(STEP_COMPONENT_POS_X + 2 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_2.setGUIBig();

		m_step_3.setBounds(STEP_COMPONENT_POS_X + 3 * STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_3.setGUIBig();

		m_step_4.setBounds(STEP_COMPONENT_POS_X + 4 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_4.setGUIBig();

		m_step_5.setBounds(STEP_COMPONENT_POS_X + 5 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_5.setGUIBig();

		m_step_6.setBounds(STEP_COMPONENT_POS_X + 6 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_6.setGUIBig();

		m_step_7.setBounds(STEP_COMPONENT_POS_X + 7 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_7.setGUIBig();

		m_step_8.setBounds(STEP_COMPONENT_POS_X + 8 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*2,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_8.setGUIBig();

		m_step_9.setBounds(STEP_COMPONENT_POS_X + 9 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*2,
		                   STEP_COMPONENT_POS_Y,
		                   STEP_COMPONENT_WIDTH,
		                   STEP_COMPONENT_HEIGHT);
		m_step_9.setGUIBig();

		m_step_10.setBounds(STEP_COMPONENT_POS_X + 10 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*2,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_10.setGUIBig();

		m_step_11.setBounds(STEP_COMPONENT_POS_X + 11 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*2,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_11.setGUIBig();

		m_step_12.setBounds(STEP_COMPONENT_POS_X + 12 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_12.setGUIBig();

		m_step_13.setBounds(STEP_COMPONENT_POS_X + 13 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_13.setGUIBig();

		m_step_14.setBounds(STEP_COMPONENT_POS_X + 14 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_14.setGUIBig();

		m_step_15.setBounds(STEP_COMPONENT_POS_X + 15 * STEP_COMPONENT_WIDTH + STEP_COMPONENT_SPACING*3,
		                    STEP_COMPONENT_POS_Y,
		                    STEP_COMPONENT_WIDTH,
		                    STEP_COMPONENT_HEIGHT);
		m_step_15.setGUIBig();
	}
}
void ArpComponent::setGUISmall() {
	m_GUI_big = false;
}

void ArpComponent::timerCallback() {
	int led_active = m_processor.m_step_led_active.get();

	if (led_active != -1) {
		setLEDActive(led_active);
	} else {
		setNoLEDActive();
	}
}

void ArpComponent::setLEDActive(int p_LED) {
	m_step_0.setLEDActive(p_LED);
	m_step_1.setLEDActive(p_LED);
	m_step_2.setLEDActive(p_LED);
	m_step_3.setLEDActive(p_LED);
	m_step_4.setLEDActive(p_LED);
	m_step_5.setLEDActive(p_LED);
	m_step_6.setLEDActive(p_LED);
	m_step_7.setLEDActive(p_LED);
	m_step_8.setLEDActive(p_LED);
	m_step_9.setLEDActive(p_LED);
	m_step_10.setLEDActive(p_LED);
	m_step_11.setLEDActive(p_LED);
	m_step_12.setLEDActive(p_LED);
	m_step_13.setLEDActive(p_LED);
	m_step_14.setLEDActive(p_LED);
	m_step_15.setLEDActive(p_LED);
}

void ArpComponent::setNoLEDActive() {
	m_step_0.setLEDActive(-1);
	m_step_1.setLEDActive(-1);
	m_step_2.setLEDActive(-1);
	m_step_3.setLEDActive(-1);
	m_step_4.setLEDActive(-1);
	m_step_5.setLEDActive(-1);
	m_step_6.setLEDActive(-1);
	m_step_7.setLEDActive(-1);
	m_step_8.setLEDActive(-1);
	m_step_9.setLEDActive(-1);
	m_step_10.setLEDActive(-1);
	m_step_11.setLEDActive(-1);
	m_step_12.setLEDActive(-1);
	m_step_13.setLEDActive(-1);
	m_step_14.setLEDActive(-1);
	m_step_15.setLEDActive(-1);
}