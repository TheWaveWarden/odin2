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
#include "../JuceLibraryCode/JuceHeader.h"

ReverbComponent::ReverbComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone) :
    m_value_tree(vts), m_is_standalone_plugin(p_is_standalone) {

	m_reverb_delay_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_delay", m_delay));
	m_reverb_EQ_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqgain", m_EQ_gain));
	m_reverb_EQ_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqfreq", m_EQ_freq));
	//m_reverb_ducking_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_ducking", m_ducking));
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

	//m_ducking.setSliderStyle(Slider::RotaryVerticalDrag);
	//m_ducking.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	//m_ducking.setKnobTooltip("DUCKING");
	//addAndMakeVisible(m_ducking);

	m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry_wet.setKnobTooltip("Controls the mix of processed and unprocessed signals");
	addAndMakeVisible(m_dry_wet);

	m_delay.setNumDecimalPlacesToDisplay(3);
	//m_ducking.setNumDecimalPlacesToDisplay(3);
	m_EQ_freq.setNumDecimalPlacesToDisplay(3);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_EQ_gain);
	SET_CTR_KEY(m_EQ_freq);
	SET_CTR_KEY(m_delay);
	SET_CTR_KEY(m_mid_hall);
	SET_CTR_KEY(m_hf_damp);
	//SET_CTR_KEY(m_ducking);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

ReverbComponent::~ReverbComponent() {
}

void ReverbComponent::paint(Graphics &g) {
	g.setColour(COL_LIGHT);
	g.drawRect(getLocalBounds(), 1);
}

void ReverbComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
}

void ReverbComponent::setGUIBig() {
	m_GUI_big = true;

	setImage(ImageCache::getFromMemory(BinaryData::reverb_zita_150_png, BinaryData::reverb_zita_150_pngSize));

	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_150_png, BinaryData::black_knob_mid_150_pngSize);
	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_150_png, BinaryData::metal_knob_mid_150_pngSize);
	juce::Image black_knob_big =
	    ImageCache::getFromMemory(BinaryData::black_knob_big_150_png, BinaryData::black_knob_big_150_pngSize);

	m_delay.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_EQ_gain.setStrip(black_knob_mid, N_KNOB_FRAMES);
	//m_ducking.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_EQ_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_dry_wet.setStrip(black_knob_big, N_KNOB_FRAMES);
	m_mid_hall.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_hf_damp.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_mid_hall.setBounds(OdinHelper::c150(REVERB_MID_HALL_POS_X),
	                     OdinHelper::c150(REVERB_MID_HALL_POS_Y),
	                     OdinHelper::c150(METAL_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(METAL_KNOB_MID_SIZE_Y));
	m_hf_damp.setBounds(OdinHelper::c150(REVERB_HF_DAMP_POS_X),
	                    OdinHelper::c150(REVERB_HF_DAMP_POS_Y),
	                    OdinHelper::c150(METAL_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(METAL_KNOB_MID_SIZE_Y));

	m_delay.setBounds(OdinHelper::c150(REVERB_DELAY_POS_X),
	                  OdinHelper::c150(REVERB_DELAY_POS_Y),
	                  OdinHelper::c150(METAL_KNOB_MID_SIZE_X),
	                  OdinHelper::c150(METAL_KNOB_MID_SIZE_Y));

	m_EQ_gain.setBounds(OdinHelper::c150(REVERB_EQ_GAIN_POS_X),
	                    OdinHelper::c150(REVERB_EQ_GAIN_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	//	m_ducking.setBounds(OdinHelper::c150(REVERB_WIDTH_POS_X),
	//	                    OdinHelper::c150(REVERB_WIDTH_POS_Y),
	//	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	//	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_EQ_freq.setBounds(OdinHelper::c150(REVERB_DRY_POS_X),
	                    OdinHelper::c150(REVERB_DRY_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_dry_wet.setBounds(OdinHelper::c150(REVERB_WET_POS_X),
	                    OdinHelper::c150(REVERB_WET_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_BIG_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_BIG_SIZE_Y));

	forceValueTreeOntoComponents(m_value_tree.state);
}

void ReverbComponent::setGUISmall() {
	m_GUI_big = false;

	setImage(ImageCache::getFromMemory(BinaryData::reverb_zita_png, BinaryData::reverb_zita_pngSize));

	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_png, BinaryData::black_knob_mid_pngSize);
	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_png, BinaryData::metal_knob_mid_pngSize);
	juce::Image black_knob_big =
	    ImageCache::getFromMemory(BinaryData::black_knob_big_png, BinaryData::black_knob_big_pngSize);

	m_delay.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_EQ_gain.setStrip(black_knob_mid, N_KNOB_FRAMES);
	//m_ducking.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_EQ_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_dry_wet.setStrip(black_knob_big, N_KNOB_FRAMES);
	m_mid_hall.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_hf_damp.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_mid_hall.setBounds(
	    (REVERB_MID_HALL_POS_X) + 2, (REVERB_MID_HALL_POS_Y) + 1, (METAL_KNOB_MID_SIZE_X), (METAL_KNOB_MID_SIZE_Y));
	m_hf_damp.setBounds(
	    (REVERB_HF_DAMP_POS_X) + 2, (REVERB_HF_DAMP_POS_Y) + 1, (METAL_KNOB_MID_SIZE_X), (METAL_KNOB_MID_SIZE_Y));

	m_delay.setBounds(
	    (REVERB_DELAY_POS_X) + 2, (REVERB_DELAY_POS_Y) + 1, (METAL_KNOB_MID_SIZE_X), (METAL_KNOB_MID_SIZE_Y));

	m_EQ_gain.setBounds(
	    (REVERB_EQ_GAIN_POS_X) + 1, (REVERB_EQ_GAIN_POS_Y) + 1, (BLACK_KNOB_MID_SIZE_X), (BLACK_KNOB_MID_SIZE_Y));
	//	m_ducking.setBounds((REVERB_WIDTH_POS_X),
	//	                    (REVERB_WIDTH_POS_Y),
	//	                    (BLACK_KNOB_MID_SIZE_X),
	//	                    (BLACK_KNOB_MID_SIZE_Y));
	m_EQ_freq.setBounds(
	    (REVERB_DRY_POS_X) + 1, (REVERB_DRY_POS_Y) + 1, (BLACK_KNOB_MID_SIZE_X), (BLACK_KNOB_MID_SIZE_Y));
	m_dry_wet.setBounds((REVERB_WET_POS_X) + 1, (REVERB_WET_POS_Y), (BLACK_KNOB_BIG_SIZE_X), (BLACK_KNOB_BIG_SIZE_Y));

	forceValueTreeOntoComponents(m_value_tree.state);
}
