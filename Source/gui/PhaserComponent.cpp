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

#include "PhaserComponent.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"

PhaserComponent::PhaserComponent(AudioProcessorValueTreeState &vts, const std::string &p_fx_name, bool p_is_standalone) :
    m_value_tree(vts),
    m_fx_name(p_fx_name),
    m_is_standalone_plugin(p_is_standalone),
    m_sync("sync", "Sync", OdinButton::Type::button_10x4),
    m_reset("reset", "Reset", OdinButton::Type::button_10x4),
    m_fx_synctime_denominator_identifier(p_fx_name + "_synctime_denominator"),
    m_fx_synctime_numerator_identifier(p_fx_name + "_synctime_numerator"),
    m_rate_label("Rate"),
    m_rate_label_sync("Rate"),
    m_mod_label("Amount"),
    m_freq_label("Freq"),
    m_feedback_label("Feedback"),
    m_dry_wet_label("DryWet"),
    m_sync_time(UIAssets::Indices::screen_up_down_14x4_LR),
    m_rate(OdinKnob::Type::knob_8x8b),
    m_mod(OdinKnob::Type::knob_8x8a),
    m_freq(OdinKnob::Type::knob_8x8a),
    m_feedback(OdinKnob::Type::knob_8x8a),
    m_dry_wet(OdinKnob::Type::knob_8x8b)

{
	addAndMakeVisible(m_rate_label);
	addChildComponent(m_rate_label_sync);
	addAndMakeVisible(m_mod_label);
	addAndMakeVisible(m_freq_label);
	addAndMakeVisible(m_feedback_label);
	addAndMakeVisible(m_dry_wet_label);

	m_rate_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_rate", m_rate));
	m_freq_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_freq", m_freq));
	m_feedback_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_feedback", m_feedback));
	m_mod_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_mod", m_mod));
	m_drywet_attach.reset(new OdinKnobAttachment(m_value_tree, m_fx_name + "_drywet", m_dry_wet));

	m_reset_attach.reset(new OdinButtonAttachment(m_value_tree, m_fx_name + "_reset", m_reset));

	m_mod.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mod.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mod.setKnobTooltip("How much the internal\nLFO modulates the\nfrequency of this effect");
	addAndMakeVisible(m_mod);

	m_rate.setSliderStyle(Slider::RotaryVerticalDrag);
	m_rate.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_rate.setRange(PHASER_FREQ_MIN, PHASER_FREQ_MAX);

	m_rate.setTextValueSuffix(" Hz");
	m_rate.setNumDecimalPlacesToDisplay(2);
	m_rate.setKnobTooltip("The frequency of the internal LFO");
	addAndMakeVisible(m_rate);

	m_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_freq.setTextValueSuffix(" Hz");
	m_freq.setNumDecimalPlacesToDisplay(2);
	m_freq.setKnobTooltip("The base frequency of the internal allpass filters");
	addAndMakeVisible(m_freq);

	m_feedback.setSliderStyle(Slider::RotaryVerticalDrag);
	m_feedback.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_feedback.setRange(0, 1);
	m_feedback.setNumDecimalPlacesToDisplay(2);
	m_feedback.setKnobTooltip("How much filtered signal is fed back to the input");
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
		m_value_tree.state.getChildWithName("fx").setProperty((Identifier)("phaser_sync"), m_sync.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage("phaser_sync");
	};

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		m_value_tree.state.getChildWithName("fx").setProperty(m_fx_synctime_numerator_identifier, p_left, nullptr);
		m_value_tree.state.getChildWithName("fx").setProperty(m_fx_synctime_denominator_identifier, p_right, nullptr);
	};
	m_sync_time.setTooltip("Set the delay time in sync to your track");
	addChildComponent(m_sync_time);

	m_rate.setNumDecimalPlacesToDisplay(3);
	m_mod.setNumDecimalPlacesToDisplay(3);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);
	m_feedback.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_rate);
	SET_CTR_KEY(m_mod);
	SET_CTR_KEY(m_freq);
	SET_CTR_KEY(m_feedback);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

PhaserComponent::~PhaserComponent() {
}

void PhaserComponent::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::FX_Phaser, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void PhaserComponent::setSyncEnabled(bool p_sync) {
	if (m_sync_enabled != p_sync) {
		m_sync_enabled = p_sync;
		if (m_sync_enabled) {
			m_rate_label.setVisible(false);
			m_rate_label_sync.setVisible(true);
			m_rate.setVisible(false);
			m_sync_time.setVisible(true);
		} else {
			m_rate_label.setVisible(true);
			m_rate_label_sync.setVisible(false);
			m_rate.setVisible(true);
			m_sync_time.setVisible(false);
		}
		repaint();
	}
}

void PhaserComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	m_sync_time.setValues(m_value_tree.state.getChildWithName("fx")[m_fx_synctime_numerator_identifier],
	                      m_value_tree.state.getChildWithName("fx")[m_fx_synctime_denominator_identifier]);

	m_sync.setToggleState((float)m_value_tree.state.getChildWithName("fx")["phaser_sync"] > 0.5f, dontSendNotification);
	setSyncEnabled((float)m_value_tree.state.getChildWithName("fx")["phaser_sync"] > 0.5f);
	//send change message to set member in processor
	m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage((Identifier)("phaser_sync"));
}

void PhaserComponent::resized() {
	GET_LOCAL_AREA(m_rate_label, "PhaserRateLabel");
	GET_LOCAL_AREA(m_rate_label_sync, "PhaserRateLabelSync");
	GET_LOCAL_AREA(m_mod_label, "PhaserModLabel");
	GET_LOCAL_AREA(m_freq_label, "PhaserFreqLabel");
	GET_LOCAL_AREA(m_feedback_label, "PhaserFeedbackLabel");
	GET_LOCAL_AREA(m_dry_wet_label, "PhaserDryWetLabel");

	GET_LOCAL_AREA(m_rate, "PhaserRate");
	GET_LOCAL_AREA(m_mod, "PhaserMod");
	GET_LOCAL_AREA(m_freq, "PhaserFreq");
	GET_LOCAL_AREA(m_feedback, "PhaserFeedback");
	GET_LOCAL_AREA(m_dry_wet, "PhaserDryWet");
	GET_LOCAL_AREA(m_sync, "PhaserSync");
	GET_LOCAL_AREA(m_reset, "PhaserReset");
	GET_LOCAL_AREA(m_sync_time, "PhaserSyncTime");
}