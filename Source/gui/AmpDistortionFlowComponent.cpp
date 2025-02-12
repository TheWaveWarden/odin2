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

#include "AmpDistortionFlowComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"

AmpDistortionFlowComponent::AmpDistortionFlowComponent(AudioProcessorValueTreeState &vts) :
    m_flow_left("flow_left", "", OdinButton::Type::button_left),
    m_flow_right("flow_right", "", OdinButton::Type::button_right),
    m_distortion("distortion", "", OdinButton::Type::fx_enabled),
    m_value_tree(vts),
    m_gain_label("Gain"),
    m_pan_label("Pan"),
    m_velocity_label("Velocity"),
    m_boost_label("Boost"),
    m_drywet_label("DryWet"),
    m_amp_gain(OdinKnob::Type::knob_8x8a),
    m_amp_pan(OdinKnob::Type::knob_5x5a),
    m_amp_velocity(OdinKnob::Type::knob_5x5a),
    m_boost(OdinKnob::Type::knob_8x8b),
    m_dry_wet(OdinKnob::Type::knob_8x8b),
    m_distortion_algo(GlassDropdown::Type::dropdown_12x4),
    m_distortion_label("DISTORTION") {

	addAndMakeVisible(m_gain_label);
	addAndMakeVisible(m_pan_label);
	addAndMakeVisible(m_velocity_label);
	addAndMakeVisible(m_boost_label);
	addAndMakeVisible(m_drywet_label);
	addAndMakeVisible(m_distortion_label);

	m_flow_left.setClickingTogglesState(true);
	addAndMakeVisible(m_flow_left);
	m_flow_left.setTooltip("Routes filter 2 into the amplifier");
	m_flow_left.setAlwaysOnTop(true);
	m_flow_left.setTriggeredOnMouseDown(true);
	m_flow_left.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_flow_right.setClickingTogglesState(true);
	addAndMakeVisible(m_flow_right);
	m_flow_right.setTooltip("Routes filter 1 into the amplifier.\n Note that "
	                        "filter 1 can also\nbe routed into filter 2");
	m_flow_right.setAlwaysOnTop(true);
	m_flow_right.setTriggeredOnMouseDown(true);
	m_flow_right.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_distortion.setClickingTogglesState(true);
	addAndMakeVisible(m_distortion);
	m_distortion.setTooltip("Enables the distortion section");
	m_distortion.setAlwaysOnTop(true);
	m_distortion.setTriggeredOnMouseDown(true);
	m_distortion.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_distortion.onClick = [&]() {
		setDistortionPanelActive(m_distortion.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty("dist_on", m_distortion.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage("dist_on");
	};

	m_amp_gain.setSliderStyle(Slider::RotaryVerticalDrag);
	m_amp_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_amp_gain.setRange(AMP_GAIN_MIN, AMP_GAIN_MAX);
	m_amp_gain.setTextValueSuffix(" dB");
	m_amp_gain.setNumDecimalPlacesToDisplay(1);
	m_amp_gain.setKnobTooltip("The volume of\nthe amplifier");
	addAndMakeVisible(m_amp_gain);

	m_amp_pan.setSliderStyle(Slider::RotaryVerticalDrag);
	m_amp_pan.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_amp_pan.setRange(PAN_MIN, PAN_MAX);
	m_amp_pan.setNumDecimalPlacesToDisplay(3);
	m_amp_pan.setKnobTooltip("Pans the sound to the\nleft or the right");
	addAndMakeVisible(m_amp_pan);

	m_amp_velocity.setSliderStyle(Slider::RotaryVerticalDrag);
	m_amp_velocity.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_amp_velocity.setKnobTooltip("Modulates the amp-gain by MIDI-key velocity");
	addAndMakeVisible(m_amp_velocity);

	m_boost.setSliderStyle(Slider::RotaryVerticalDrag);
	m_boost.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_boost.setKnobTooltip("Boosts the signal to apply more damage");
	m_boost.setNumDecimalPlacesToDisplay(3);
	addAndMakeVisible(m_boost);

	m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry_wet.setKnobTooltip("The ratio of unprocessed\nand processed sound");
	addAndMakeVisible(m_dry_wet);

	m_distortion_algo.setInlay(1);
	m_distortion_algo.addItem("Clamp", 1);
	m_distortion_algo.addItem("Fold", 2);
	m_distortion_algo.addItem("Zero", 3);
	//m_distortion_algo.addItem("Sine", 4);
	//m_distortion_algo.addItem("Cube", 5);
	m_distortion_algo.setEditableText(false);
	m_distortion_algo.showTriangle();
	m_distortion_algo.setSelectedId(1, dontSendNotification);
	m_distortion_algo.setColor(juce::STANDARD_DISPLAY_COLOR);
	m_distortion_algo.setTooltip("Selects the distortion algorithm to be used");
	m_distortion_algo.onChange = [&]() { m_value_tree.state.getChildWithName("misc").setProperty("dist_algo", m_distortion_algo.getSelectedId(), nullptr); };
	addAndMakeVisible(m_distortion_algo);

	m_amp_velocity_attach.reset(new OdinKnobAttachment(m_value_tree, "amp_velocity", m_amp_velocity));
	m_amp_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "amp_gain", m_amp_gain));
	m_amp_pan_attach.reset(new OdinKnobAttachment(m_value_tree, "amp_pan", m_amp_pan));
	m_dist_threshold_attach.reset(new OdinKnobAttachment(m_value_tree, "dist_boost", m_boost));
	m_dist_drywet_attach.reset(new OdinKnobAttachment(m_value_tree, "dist_drywet", m_dry_wet));

	m_fil1_to_amp_attach.reset(new OdinButtonAttachment(m_value_tree, "fil1_to_amp", m_flow_right));
	m_fil2_to_amp_attach.reset(new OdinButtonAttachment(m_value_tree, "fil2_to_amp", m_flow_left));

	// m_boost.textFromValueFunction = nullptr;
	m_boost.setNumDecimalPlacesToDisplay(3);
	m_amp_gain.setNumDecimalPlacesToDisplay(2);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);
	m_amp_velocity.setNumDecimalPlacesToDisplay(3);
	m_amp_pan.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_amp_gain);
	SET_CTR_KEY(m_amp_pan);
	SET_CTR_KEY(m_amp_velocity);
	SET_CTR_KEY(m_boost);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

AmpDistortionFlowComponent::~AmpDistortionFlowComponent() {
}

void AmpDistortionFlowComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	m_distortion_algo.setValue(m_value_tree.state.getChildWithName("misc")["dist_algo"]);
	setDistortionPanelActive((float)m_value_tree.state.getChildWithName("misc")["dist_on"] > 0.5f);
	m_distortion.setToggleState((float)m_value_tree.state.getChildWithName("misc")["dist_on"] > 0.5f, dontSendNotification);
}

void AmpDistortionFlowComponent::setDistortionPanelActive(bool p_active) {
	m_distortion_algo.setEnabled(p_active);
	m_boost.setEnabled(p_active);
	m_boost_label.setEnabled(p_active);
	m_dry_wet.setEnabled(p_active);
	m_drywet_label.setEnabled(p_active);
	m_distortion_on = p_active;
	repaint();
}

void AmpDistortionFlowComponent::paint(Graphics &g) {
}

void AmpDistortionFlowComponent::resized() {

	GET_LOCAL_AREA(m_gain_label, "GainLabel");
	GET_LOCAL_AREA(m_pan_label, "PanLabel");
	GET_LOCAL_AREA(m_velocity_label, "VelocityLabel");
	GET_LOCAL_AREA(m_boost_label, "BoostLabel");
	GET_LOCAL_AREA(m_drywet_label, "DistDryWetLabel");
	GET_LOCAL_AREA(m_distortion_label, "DistModuleLabel");

	GET_LOCAL_AREA(m_flow_left, "FlowLeft");
	GET_LOCAL_AREA(m_flow_right, "FlowRight");
	GET_LOCAL_AREA(m_distortion, "Distortion");
	GET_LOCAL_AREA(m_distortion_algo, "DistortionAlgo");
	GET_LOCAL_AREA(m_amp_gain, "AmpGain");
	GET_LOCAL_AREA(m_amp_pan, "AmpPan");
	GET_LOCAL_AREA(m_amp_velocity, "AmpVelocity");
	GET_LOCAL_AREA(m_boost, "Boost");
	GET_LOCAL_AREA(m_dry_wet, "DryWet");
}