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

#include "StepComponent.h"
#include "JsonGuiProvider.h"
#include <JuceHeader.h>

StepComponent::StepComponent(AudioProcessorValueTreeState &vts, int p_step_index) :
    m_value_tree(vts),
    m_step_on("step_" + std::to_string(p_step_index + 1) + "_on", std::to_string(p_step_index + 1), OdinButton::Type::button_5x5),
    m_step_index(p_step_index),
    m_mod_1(OdinKnob::Type::knob_5x5a),
    m_mod_2(OdinKnob::Type::knob_5x5a),
    m_transpose(OdinKnob::Type::knob_5x5a) {

	addAndMakeVisible(m_mod_1);
	m_mod_1.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mod_1.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mod_1.setKnobTooltip("This value can be used as modulation source \"Arp Mod 1\" in the modmatrix");
	m_mod_1_attach.reset(new OdinKnobAttachment(m_value_tree, "step_" + std::to_string(m_step_index) + "_mod_1", m_mod_1));

	addAndMakeVisible(m_mod_2);
	m_mod_2.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mod_2.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mod_2.setKnobTooltip("This value can be used as modulation source \"Arp Mod 2\" in the modmatrix.\nThe entire row of knobs "
	                       "can be replaced with \"transpose\" by the button to the left.");
	m_mod_2_attach.reset(new OdinKnobAttachment(m_value_tree, "step_" + std::to_string(m_step_index) + "_mod_2", m_mod_2));

	addChildComponent(m_transpose);
	m_transpose.setSliderStyle(Slider::RotaryVerticalDrag);
	m_transpose.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_transpose.setKnobTooltip("Transposes this step in the arp in semitones.\nSet \"octaves\" to one to use the arp as a step sequencer");
	m_transpose_attach.reset(new OdinKnobAttachment(m_value_tree, "step_" + std::to_string(m_step_index) + "_transpose", m_transpose));

	addAndMakeVisible(m_step_on);
	m_step_on.setClickingTogglesState(true);
	m_step_on.setTooltip("Turns this step in the sequence on or off");
	m_step_on.setTriggeredOnMouseDown(true);
	m_step_on.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_step_on_attach.reset(new OdinButtonAttachment(m_value_tree, "step_" + std::to_string(m_step_index) + "_on", m_step_on));

	addAndMakeVisible(m_led);

	m_mod_1.setNumDecimalPlacesToDisplay(3);
	m_mod_2.setNumDecimalPlacesToDisplay(3);
	m_transpose.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_mod_1);
	SET_CTR_KEY(m_mod_2);
	SET_CTR_KEY(m_transpose);
}

StepComponent::~StepComponent() {
}

void StepComponent::resized() {
	GET_LOCAL_AREA(m_mod_1, "ArpStepMod_1");
	GET_LOCAL_AREA(m_mod_2, "ArpStepMod_2");
	GET_LOCAL_AREA(m_transpose, "ArpStepTranspose");
	GET_LOCAL_AREA(m_step_on, "ArpStepStepOn");
	GET_LOCAL_AREA(m_led, "ArpStepLed");
}

void StepComponent::setLEDActive(int p_step_index) {
	m_led.setLEDOn(p_step_index == m_step_index);
}

void StepComponent::setShowLED(int p_highest_led) {
	if ((p_highest_led > m_step_index) != m_show_led) {
		m_show_led = (p_highest_led > m_step_index);
		m_led.setVisible(m_show_led);
	}
}

void StepComponent::setModTranspose(bool p_is_mod) {
	if (p_is_mod) {
		m_mod_2.setVisible(true);
		m_transpose.setVisible(false);
	} else {
		m_mod_2.setVisible(false);
		m_transpose.setVisible(true);
	}
}