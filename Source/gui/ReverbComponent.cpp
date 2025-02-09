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

#include "ReverbComponent.h"
#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"

ReverbComponent::ReverbComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone) :
    m_value_tree(vts),
    m_is_standalone_plugin(p_is_standalone),
    m_delay_label("Delay"),
    m_EQ_gain_label("EQ Gain"),
    m_EQ_freq_label("EQ Freq"),
    m_dry_wet_label("DryWet"),
    m_mid_hall_label("Decay"),
    m_hf_damp_label("HF Damp"),
    m_delay(OdinKnob::Type::knob_8x8a),
    m_mid_hall(OdinKnob::Type::knob_8x8a),
    m_hf_damp(OdinKnob::Type::knob_8x8a),
    m_EQ_freq(OdinKnob::Type::knob_5x5a),
    m_EQ_gain(OdinKnob::Type::knob_5x5a),
    m_dry_wet(OdinKnob::Type::knob_8x8b) {

	addAndMakeVisible(m_delay_label);
	addAndMakeVisible(m_EQ_gain_label);
	addAndMakeVisible(m_EQ_freq_label);
	addAndMakeVisible(m_dry_wet_label);
	addAndMakeVisible(m_mid_hall_label);
	addAndMakeVisible(m_hf_damp_label);

	m_reverb_delay_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_delay", m_delay));
	m_reverb_EQ_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqgain", m_EQ_gain));
	m_reverb_EQ_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqfreq", m_EQ_freq));
	m_reverb_drywet_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_drywet", m_dry_wet));
	m_reverb_mid_hall_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_mid_hall", m_mid_hall));
	m_reverb_hf_damp_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_hf_damp", m_hf_damp));

	m_delay.setSliderStyle(Slider::RotaryVerticalDrag);
	m_delay.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_delay.setTextValueSuffix(" ms");
	m_delay.setKnobTooltip("Sets the time of the initial delay before reverberation starts");
	addAndMakeVisible(m_delay);

	m_mid_hall.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mid_hall.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mid_hall.setTextValueSuffix(" s");
	m_mid_hall.setKnobTooltip("Controls the time the reverberation takes to attenuate");
	m_mid_hall.setNumDecimalPlacesToDisplay(2);
	addAndMakeVisible(m_mid_hall);

	m_hf_damp.setSliderStyle(Slider::RotaryVerticalDrag);
	m_hf_damp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_hf_damp.setTextValueSuffix(" Hz");
	m_hf_damp.setKnobTooltip("Controls the frequency where higher frequencies start to be attenuated");
	m_hf_damp.setNumDecimalPlacesToDisplay(0);
	addAndMakeVisible(m_hf_damp);

	m_EQ_gain.setSliderStyle(Slider::RotaryVerticalDrag);
	m_EQ_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_EQ_gain.setTextValueSuffix(" dB");
	m_EQ_gain.setNumDecimalPlacesToDisplay(1);
	m_EQ_gain.setKnobTooltip("Sets the gain of the built in equalizer");
	addAndMakeVisible(m_EQ_gain);

	m_EQ_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_EQ_freq.setTextValueSuffix(" Hz");
	m_EQ_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_EQ_freq.setKnobTooltip("Sets the frequency of the built in equalizer");
	addAndMakeVisible(m_EQ_freq);

	m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry_wet.setKnobTooltip("Controls the mix of processed and unprocessed signals");
	addAndMakeVisible(m_dry_wet);

	m_delay.setNumDecimalPlacesToDisplay(3);
	m_EQ_freq.setNumDecimalPlacesToDisplay(3);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_EQ_gain);
	SET_CTR_KEY(m_EQ_freq);
	SET_CTR_KEY(m_delay);
	SET_CTR_KEY(m_mid_hall);
	SET_CTR_KEY(m_hf_damp);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

ReverbComponent::~ReverbComponent() {
}

void ReverbComponent::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::FX_Reverb, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void ReverbComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
}

void ReverbComponent::resized() {
	GET_LOCAL_AREA(m_delay_label, "RevDelayLabel");
	GET_LOCAL_AREA(m_EQ_gain_label, "RevEqGainLabel");
	GET_LOCAL_AREA(m_EQ_freq_label, "RevEqFreqLabel");
	GET_LOCAL_AREA(m_dry_wet_label, "RevDryWetLabel");
	GET_LOCAL_AREA(m_mid_hall_label, "RevMidHallLabel");
	GET_LOCAL_AREA(m_hf_damp_label, "RevHfDampLabel");

	GET_LOCAL_AREA(m_delay, "RevDelay");
	GET_LOCAL_AREA(m_EQ_gain, "RevEqGain");
	GET_LOCAL_AREA(m_EQ_freq, "RevEqFreq");
	GET_LOCAL_AREA(m_dry_wet, "RevDryWet");
	GET_LOCAL_AREA(m_mid_hall, "RevMidHall");
	GET_LOCAL_AREA(m_hf_damp, "RevHfDamp");
}
