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

#include "DelayComponent.h"
#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"

DelayComponent::DelayComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone) :
    m_sync("sync", "Sync", OdinButton::Type::button_10x4),
    m_pingpong("pingpong", "PingPong", OdinButton::Type::button_12x4),
    m_value_tree(vts),
    m_delay_synctime_denominator_identifier("delay_synctime_denominator"),
    m_delay_synctime_numerator_identifier("delay_synctime_numerator"),
    m_is_standalone_plugin(p_is_standalone),
    m_time_label("Time"),
    m_feedback_label("Feedback"),
    m_hp_label("HP"),
    m_ducking_label("Ducking"),
    m_dry_label("Dry"),
    m_wet_label("Wet"),
    m_feedback(OdinKnob::Type::knob_8x8a),
    m_HP(OdinKnob::Type::knob_5x5a),
    m_dry(OdinKnob::Type::knob_5x5a),
    m_wet(OdinKnob::Type::knob_5x5a),
    m_ducking(OdinKnob::Type::knob_5x5a),
    m_time(OdinKnob::Type::knob_8x8b),
    m_sync_time(UIAssets::Indices::screen_up_down_14x4_LR) {

	addAndMakeVisible(m_time_label);
	addAndMakeVisible(m_feedback_label);
	addAndMakeVisible(m_hp_label);
	addAndMakeVisible(m_ducking_label);
	addAndMakeVisible(m_dry_label);
	addAndMakeVisible(m_wet_label);

	m_delay_time_attach.reset(new OdinKnobAttachment(m_value_tree, "delay_time", m_time));
	m_delay_feedback_attach.reset(new OdinKnobAttachment(m_value_tree, "delay_feedback", m_feedback));
	m_delay_hp_attach.reset(new OdinKnobAttachment(m_value_tree, "delay_hp", m_HP));
	m_delay_ducking_attach.reset(new OdinKnobAttachment(m_value_tree, "delay_ducking", m_ducking));
	m_delay_dry_attach.reset(new OdinKnobAttachment(m_value_tree, "delay_dry", m_dry));
	m_delay_wet_attach.reset(new OdinKnobAttachment(m_value_tree, "delay_wet", m_wet));

	m_sync.setClickingTogglesState(true);
	m_sync.setTriggeredOnMouseDown(true);
	m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_sync.setTooltip("Syncs the delay time to your track");
	addAndMakeVisible(m_sync);
	m_sync.onClick = [&]() {
		setSync(m_sync.getToggleState());
		m_value_tree.state.getChildWithName("fx").setProperty((Identifier)("delay_sync"), m_sync.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage((Identifier)("delay_sync"));
	};

	m_pingpong.setClickingTogglesState(true);
	m_pingpong.setTriggeredOnMouseDown(true);
	m_pingpong.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_pingpong.setTooltip("Makes the left and right delay channels cross their feedback");
	addAndMakeVisible(m_pingpong);

	m_time.setSliderStyle(Slider::RotaryVerticalDrag);
	m_time.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_time.setRange(TIME_MIN, TIME_MAX);
	m_time.setTextValueSuffix(" s");
	m_time.setNumDecimalPlacesToDisplay(3);
	m_time.setKnobTooltip("The time for the signal to echo back");
	addAndMakeVisible(m_time);

	m_feedback.setSliderStyle(Slider::RotaryVerticalDrag);
	m_feedback.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_feedback.setKnobTooltip("How much to echo back");
	addAndMakeVisible(m_feedback);

	m_HP.setSliderStyle(Slider::RotaryVerticalDrag);
	m_HP.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_HP.setTextValueSuffix(" Hz");
	m_HP.setNumDecimalPlacesToDisplay(1);
	m_HP.setKnobTooltip("The cutoff frequency\n for a highpass filter\n which is "
	                    "applied\nto the signals");
	addAndMakeVisible(m_HP);

	m_ducking.setSliderStyle(Slider::RotaryVerticalDrag);
	m_ducking.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_ducking.setKnobTooltip("Lowers the wet volume\nwhen a signal is at the\ninput of the delay\nUse "
	                         "this to declutter\nyour sounds");
	addAndMakeVisible(m_ducking);

	m_dry.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry.setKnobTooltip("Volume of the input signal");
	addAndMakeVisible(m_dry);

	m_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_wet.setKnobTooltip("Volume of the delayed signal only");
	addAndMakeVisible(m_wet);

	m_pingpong_attach.reset(new OdinButtonAttachment(m_value_tree, "delay_pingpong", m_pingpong));

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		m_value_tree.state.getChildWithName("fx").setProperty(m_delay_synctime_numerator_identifier, p_left, nullptr);
		m_value_tree.state.getChildWithName("fx").setProperty(m_delay_synctime_denominator_identifier, p_right, nullptr);
	};
	m_sync_time.setTooltip("Set the delay time in sync to your track");
	addChildComponent(m_sync_time);

	m_feedback.setNumDecimalPlacesToDisplay(3);
	m_ducking.setNumDecimalPlacesToDisplay(3);
	m_dry.setNumDecimalPlacesToDisplay(3);
	m_wet.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_time);
	SET_CTR_KEY(m_feedback);
	SET_CTR_KEY(m_HP);
	SET_CTR_KEY(m_ducking);
	SET_CTR_KEY(m_dry);
	SET_CTR_KEY(m_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

DelayComponent::~DelayComponent() {
}

void DelayComponent::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::FX_Delay, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void DelayComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	m_sync_time.setValues(m_value_tree.state.getChildWithName("fx")[m_delay_synctime_numerator_identifier],
	                      m_value_tree.state.getChildWithName("fx")[m_delay_synctime_denominator_identifier]);
	m_sync.setToggleState((float)m_value_tree.state.getChildWithName("fx")["delay_sync"] > 0.5f, dontSendNotification);
	setSync((float)m_value_tree.state.getChildWithName("fx")["delay_sync"] > 0.5f);
	//send change message to set member in processor
	m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage((Identifier)("delay_sync"));
}

void DelayComponent::resized() {
	GET_LOCAL_AREA(m_time_label, "DelayTimeLabel");
	GET_LOCAL_AREA(m_feedback_label, "DelayFeedbackLabel");
	GET_LOCAL_AREA(m_hp_label, "DelayHpLabel");
	GET_LOCAL_AREA(m_ducking_label, "DelayDuckingLabel");
	GET_LOCAL_AREA(m_dry_label, "DelayDryLabel");
	GET_LOCAL_AREA(m_wet_label, "DelayWetLabel");

	GET_LOCAL_AREA(m_sync, "DelaySync");
	GET_LOCAL_AREA(m_pingpong, "DelayPingpong");
	GET_LOCAL_AREA(m_time, "DelayTime");
	GET_LOCAL_AREA(m_feedback, "DelayFeedback");
	GET_LOCAL_AREA(m_HP, "DelayHp");
	GET_LOCAL_AREA(m_ducking, "DelayDucking");
	GET_LOCAL_AREA(m_dry, "DelayDry");
	GET_LOCAL_AREA(m_wet, "DelayWet");
	GET_LOCAL_AREA(m_sync_time, "DelaySyncTime");
}

void DelayComponent::setSync(bool p_sync_on) {
	if (p_sync_on != m_sync_on) {
		m_sync_on = p_sync_on;

		if (p_sync_on) {
			m_time.setVisible(false);
			m_sync_time.setVisible(true);
		} else {
			m_time.setVisible(true);
			m_sync_time.setVisible(false);
		}
		repaint();
	}
}