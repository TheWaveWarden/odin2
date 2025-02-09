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

#include <typeinfo>

#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "LFOComponent.h"

LFOComponent::LFOComponent(AudioProcessorValueTreeState &vts, const std::string &p_lfo_number, bool p_is_standalone) :
    m_value_tree(vts),
    m_lfo_number(p_lfo_number),
    m_reset("reset", "Reset", OdinButton::Type::button_13x4),
    m_sync("sync", "Sync", OdinButton::Type::button_13x4),
    m_lfo_wave_identifier("lfo" + p_lfo_number + "_wave"),
    m_lfo_synctime_denominator_identifier("lfo" + p_lfo_number + "_synctime_denominator"),
    m_lfo_synctime_numerator_identifier("lfo" + p_lfo_number + "_synctime_numerator"),
    m_is_standalone_plugin(p_is_standalone),
    m_freq(OdinKnob::Type::timeHz_13x4),
	m_sync_time(UIAssets::Indices::screen_up_down_13x4_LR) {

	m_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "lfo" + m_lfo_number + "_freq", m_freq));

	m_reset_attach.reset(new OdinButtonAttachment(m_value_tree, "lfo" + m_lfo_number + "_reset", m_reset));

	m_reset.setClickingTogglesState(true);
	m_reset.setTooltip("Resets the LFO on every new note");
	addAndMakeVisible(m_reset);
	m_reset.setTriggeredOnMouseDown(true);
	m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_sync.setClickingTogglesState(true);
	m_sync.setTriggeredOnMouseDown(true);
	m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	addAndMakeVisible(m_sync);
	m_sync.setTooltip("Enables syncing the LFO\nto the speed of your track");
	m_sync.onClick = [&]() {
		setSync(m_sync.getToggleState());
		m_value_tree.state.getChildWithName("lfo").setProperty((Identifier)("lfo" + m_lfo_number + "_sync"), m_sync.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("lfo").sendPropertyChangeMessage((Identifier)("lfo" + m_lfo_number + "_sync"));
	};

	m_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_freq.setRange(LFO_FREQ_MIN, LFO_FREQ_MAX);
	m_freq.setTextValueSuffix(" Hz");
	m_freq.setNumDecimalPlacesToDisplay(2);
	m_freq.setKnobTooltip("The frequency of the LFO");
	addAndMakeVisible(m_freq);

	m_selector.OnValueChange = [&](int p_new_value) {
		//m_value_tree.getParameter(m_lfo_wave_identifier)->setValueNotifyingHost(((float)p_new_value) / 20.f);
		m_value_tree.state.getChildWithName("lfo").setProperty(m_lfo_wave_identifier, p_new_value, nullptr);
	};
	m_selector.setTooltip("The waveform to be used for this LFO. WD 1-3 use "
	                      "the WaveDraw tables drawn in oscillators 1-3. CD 1-3 use the ChipDraw tables.");
	addAndMakeVisible(m_selector);

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		m_value_tree.state.getChildWithName("lfo").setProperty(m_lfo_synctime_numerator_identifier, p_left, nullptr);
		m_value_tree.state.getChildWithName("lfo").setProperty(m_lfo_synctime_denominator_identifier, p_right, nullptr);
	};
	m_sync_time.setTooltip("Set the frequency in sync to your track.");
	addChildComponent(m_sync_time);

	m_selector.setParameterId("lfo" + m_lfo_number + "_wave");
	m_value_tree.addParameterListener("lfo" + m_lfo_number + "_wave", &m_selector);

	SET_CTR_KEY(m_freq);

	forceValueTreeOntoComponents(m_value_tree.state);
}

LFOComponent::~LFOComponent() {
	m_value_tree.removeParameterListener("lfo" + m_lfo_number + "_wave", &m_selector);
}

void LFOComponent::forceValueTreeOntoComponents(ValueTree p_tree) {

	m_selector.setValueGUIOnly(m_value_tree.state.getChildWithName("lfo")[m_lfo_wave_identifier]);

	m_sync_time.setValues(m_value_tree.state.getChildWithName("lfo")[m_lfo_synctime_numerator_identifier],
	                      m_value_tree.state.getChildWithName("lfo")[m_lfo_synctime_denominator_identifier]);
	setSync((float)m_value_tree.state.getChildWithName("lfo")[(Identifier)("lfo" + m_lfo_number + "_sync")] > 0.5f);
	//m_value_tree.state.getChildWithName("lfo").sendPropertyChangeMessage((Identifier)("lfo" + m_lfo_number + "_sync"));
	m_sync.setToggleState((float)m_value_tree.state.getChildWithName("lfo")[(Identifier)("lfo" + m_lfo_number + "_sync")] > 0.5f, dontSendNotification);
}

void LFOComponent::resized() {
	GET_LOCAL_AREA(m_selector, "LFOSelector");
	GET_LOCAL_AREA(m_sync_time, "LFOSyncTime");
	GET_LOCAL_AREA(m_freq, "LFOFreq");
	GET_LOCAL_AREA(m_reset, "LFOReset");
	GET_LOCAL_AREA(m_sync, "LFOSync");
}
