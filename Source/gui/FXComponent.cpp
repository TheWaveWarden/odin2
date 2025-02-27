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

#include "FXComponent.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"

FXComponent::FXComponent(AudioProcessorValueTreeState &vts, const std::string &p_fx_name, bool p_is_standalone, Type p_type) :
    m_type(p_type),
    m_value_tree(vts),
    m_fx_name(p_fx_name),
    m_is_standalone_plugin(p_is_standalone),
    m_sync("sync", "Sync", OdinButton::Type::button_10x4),
    m_reset("reset", "Reset", OdinButton::Type::button_10x4),
    m_fx_synctime_denominator_identifier(p_fx_name + "_synctime_denominator"),
    m_fx_synctime_numerator_identifier(p_fx_name + "_synctime_numerator"),
    m_rate_label("Rate"),
    m_amount_label("Amount"),
    m_feedback_label("Feedback"),
    m_dry_wet_label("DryWet"),
    m_sync_time(UIAssets::Indices::screen_up_down_14x4_LR),
    m_rate(OdinKnob::Type::knob_8x8b),
    m_amount(OdinKnob::Type::knob_8x8a),
    m_feedback(OdinKnob::Type::knob_8x8a),
    m_dry_wet(OdinKnob::Type::knob_8x8b) {

	addAndMakeVisible(m_rate_label);
	addAndMakeVisible(m_amount_label);
	addAndMakeVisible(m_feedback_label);
	addAndMakeVisible(m_dry_wet_label);

	m_rate_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_rate", m_rate));
	m_amount_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_amount", m_amount));
	m_drywet_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_drywet", m_dry_wet));
	m_feedback_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_feedback", m_feedback));

	m_reset_attach.reset(new OdinButtonAttachment(m_value_tree, m_fx_name + "_reset", m_reset));

	m_amount.setSliderStyle(Slider::RotaryVerticalDrag);
	m_amount.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	m_amount.setKnobTooltip("How much the internal\nLFO modulates the\nfrequency of this effect");
	addAndMakeVisible(m_amount);

	m_rate.setSliderStyle(Slider::RotaryVerticalDrag);
	m_rate.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	//DBG(m_rate.getSkewFactor());
	m_rate.setTextValueSuffix(" Hz");
	m_rate.setNumDecimalPlacesToDisplay(2);
	m_rate.setKnobTooltip("The frequency of the internal LFO");
	addAndMakeVisible(m_rate);

	m_feedback.setSliderStyle(Slider::RotaryVerticalDrag);
	m_feedback.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_feedback.setRange(-1, 1);
	m_feedback.setKnobTooltip("How much of the delay line is fed back onto itself");
	addAndMakeVisible(m_feedback);

	m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry_wet.setKnobTooltip("The mix of unprocessed and\n processed signals");
	addAndMakeVisible(m_dry_wet);

	m_reset.setClickingTogglesState(true);
	m_reset.setTriggeredOnMouseDown(true);
	m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_reset.setTooltip("Resets the internal\n LFO on every new note");
	addAndMakeVisible(m_reset);

	m_sync.setClickingTogglesState(true);
	m_sync.setTriggeredOnMouseDown(true);
	m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_sync.setTooltip("Syncs the internal LFOs\nspeed to your track");
	addAndMakeVisible(m_sync);
	m_sync.onClick = [&]() {
		setSyncEnabled(m_sync.getToggleState());
		m_value_tree.state.getChildWithName("fx").setProperty((Identifier)(m_fx_name + "_sync"), m_sync.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage((Identifier)(m_fx_name + "_sync"));
	};

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		m_value_tree.state.getChildWithName("fx").setProperty(m_fx_synctime_numerator_identifier, p_left, nullptr);
		m_value_tree.state.getChildWithName("fx").setProperty(m_fx_synctime_denominator_identifier, p_right, nullptr);
	};

	m_sync_time.setTooltip("Set the delay time in sync to your track");
	addChildComponent(m_sync_time);

	m_rate.setNumDecimalPlacesToDisplay(3);
	m_amount.setNumDecimalPlacesToDisplay(3);
	m_feedback.setNumDecimalPlacesToDisplay(3);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_rate);
	SET_CTR_KEY(m_amount);
	SET_CTR_KEY(m_feedback);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

FXComponent::~FXComponent() {
}

void FXComponent::paint(Graphics &g) {
	auto asset = m_type == Type::chorus ? UIAssets::Indices::FX_Chorus : UIAssets::Indices::FX_Flanger;

	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(asset, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void FXComponent::setSyncEnabled(bool p_sync) {
	if (m_sync_enabled != p_sync) {
		m_sync_enabled = p_sync;
		if (m_sync_enabled) {
			m_rate.setVisible(false);
			m_sync_time.setVisible(true);
		} else {
			m_rate.setVisible(true);
			m_sync_time.setVisible(false);
		}
		repaint();
	}
}

void FXComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	m_sync_time.setValues(m_value_tree.state.getChildWithName("fx")[m_fx_synctime_numerator_identifier],
	                      m_value_tree.state.getChildWithName("fx")[m_fx_synctime_denominator_identifier]);

	m_sync.setToggleState((float)m_value_tree.state.getChildWithName("fx")[(Identifier)(m_fx_name + "_sync")] > 0.5f, dontSendNotification);
	setSyncEnabled((float)m_value_tree.state.getChildWithName("fx")[(Identifier)(m_fx_name + "_sync")] > 0.5f);
	//send change message to set member in processor
	m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage(((Identifier)(m_fx_name + "_sync")));
}

void FXComponent::resized() {

	GET_LOCAL_AREA(m_rate_label, "FXRateLabel");
	GET_LOCAL_AREA(m_amount_label, "FXAmountLabel");
	GET_LOCAL_AREA(m_feedback_label, "FXFeedbackLabel");
	GET_LOCAL_AREA(m_dry_wet_label, "FXDryWetLabel");

	GET_LOCAL_AREA(m_rate, "FXRate");
	GET_LOCAL_AREA(m_amount, "FXAmount");
	GET_LOCAL_AREA(m_feedback, "FXFeedback");
	GET_LOCAL_AREA(m_dry_wet, "FXDryWet");
	GET_LOCAL_AREA(m_sync, "FXSync");
	GET_LOCAL_AREA(m_reset, "FXReset");
	GET_LOCAL_AREA(m_sync_time, "FXSyncTime");
}