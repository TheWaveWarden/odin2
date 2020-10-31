/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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
	m_reverb_low_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_lowfreq", m_low_freq));
	m_reverb_low_time_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_lowtime", m_low_time));
	m_reverb_mid_time_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_midtime", m_mid_time));
	m_reverb_HF_damping_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_hfdamp", m_HF_damping));
	m_reverb_EQ_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqgain", m_EQ_gain));
	m_reverb_EQ_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqfreq", m_EQ_freq));
	m_reverb_ducking_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_ducking", m_ducking));
	m_reverb_drywet_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_drywet", m_dry_wet));

	m_delay.setSliderStyle(Slider::RotaryVerticalDrag);
	m_delay.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_delay.setTextValueSuffix(" ms");
	m_delay.setKnobTooltip("m_delay");
	addAndMakeVisible(m_delay);

	m_low_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_low_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_low_freq.setTextValueSuffix(" Hz");
	m_low_freq.setKnobTooltip("m_low_freq");
	addAndMakeVisible(m_low_freq);

	m_low_time.setSliderStyle(Slider::RotaryVerticalDrag);
	m_low_time.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_low_time.setTextValueSuffix(" s");
	m_low_time.setKnobTooltip("m_low_time");
	addAndMakeVisible(m_low_time);

	m_mid_time.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mid_time.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mid_time.setTextValueSuffix(" s");
	m_mid_time.setKnobTooltip("m_mid_time");
	addAndMakeVisible(m_mid_time);

	m_HF_damping.setSliderStyle(Slider::RotaryVerticalDrag);
	m_HF_damping.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_HF_damping.setTextValueSuffix(" Hz");
	m_HF_damping.setKnobTooltip("m_HF_damping");
	addAndMakeVisible(m_HF_damping);

	m_EQ_gain.setSliderStyle(Slider::RotaryVerticalDrag);
	m_EQ_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_EQ_gain.setTextValueSuffix(" dB");
	m_EQ_gain.setNumDecimalPlacesToDisplay(1);
	m_EQ_gain.setKnobTooltip("EQ GAIN");
	addAndMakeVisible(m_EQ_gain);

	m_EQ_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_EQ_freq.setTextValueSuffix(" Hz");
	m_EQ_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_EQ_freq.setKnobTooltip("EQ FREQ");
	addAndMakeVisible(m_EQ_freq);

	m_ducking.setSliderStyle(Slider::RotaryVerticalDrag);
	m_ducking.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_ducking.setKnobTooltip("DUCKING");
	addAndMakeVisible(m_ducking);

	m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry_wet.setKnobTooltip("DRYWET");
	addAndMakeVisible(m_dry_wet);

	m_delay.setNumDecimalPlacesToDisplay(3);
	m_low_freq.setNumDecimalPlacesToDisplay(3);
	m_low_time.setNumDecimalPlacesToDisplay(3);
	m_mid_time.setNumDecimalPlacesToDisplay(3);
	m_HF_damping.setNumDecimalPlacesToDisplay(3);
	m_ducking.setNumDecimalPlacesToDisplay(3);
	m_EQ_freq.setNumDecimalPlacesToDisplay(3);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_EQ_gain);
	SET_CTR_KEY(m_EQ_freq);
	SET_CTR_KEY(m_ducking);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

ReverbComponent::~ReverbComponent() {
}

void ReverbComponent::paint(Graphics &g) {

	//DEBUG_COMPONENT_OUTLINE
	SET_INTERPOLATION_QUALITY(g)

	g.drawImageAt(m_background, 0, 0);
}

void ReverbComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
}

void ReverbComponent::setGUIBig() {
	m_GUI_big = true;

	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_150_png, BinaryData::black_knob_mid_150_pngSize);
	
    m_delay.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_low_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_low_time.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_mid_time.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_HF_damping.setStrip(black_knob_mid, N_KNOB_FRAMES);
    m_EQ_gain.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_ducking.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_EQ_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_dry_wet.setStrip(black_knob_mid, N_KNOB_FRAMES);
    

	m_delay.setBounds(OdinHelper::c150(REVERB_DELAY_POS_X),
	                  OdinHelper::c150(REVERB_DELAY_POS_Y),
	                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_low_freq.setBounds(OdinHelper::c150(REVERB_LOW_FREQ_POS_X),
	                     OdinHelper::c150(REVERB_LOW_FREQ_POS_Y),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_low_time.setBounds(OdinHelper::c150(REVERB_LOW_TIME_POS_X),
	                     OdinHelper::c150(REVERB_LOW_TIME_POS_Y),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_mid_time.setBounds(OdinHelper::c150(REVERB_MID_TIME_POS_X),
	                     OdinHelper::c150(REVERB_MID_TIME_POS_Y),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_HF_damping.setBounds(OdinHelper::c150(REVERB_HF_DAMPING_POS_X),
	                       OdinHelper::c150(REVERB_HF_DAMPING_POS_Y),
	                       OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                       OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));

	m_EQ_gain.setBounds(OdinHelper::c150(REVERB_EQ_GAIN_POS_X) - 2,
	                    OdinHelper::c150(REVERB_EQ_GAIN_POS_Y) - 2,
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_ducking.setBounds(OdinHelper::c150(REVERB_DUCKING_POS_X) - 2,
	                    OdinHelper::c150(REVERB_DUCKING_POS_Y) - 2,
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_EQ_freq.setBounds(OdinHelper::c150(DRY_POS_X) - 2,
	                    OdinHelper::c150(DRY_POS_Y) - 2,
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_dry_wet.setBounds(OdinHelper::c150(WET_POS_X) - 2,
	                    OdinHelper::c150(WET_POS_Y) - 2,
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));

	juce::Image reverb_image;
	reverb_image = ImageCache::getFromMemory(BinaryData::reverb_150_png, BinaryData::reverb_150_pngSize);
	setImage(reverb_image);

	forceValueTreeOntoComponents(m_value_tree.state);
}

void ReverbComponent::setGUISmall() {
	m_GUI_big = false;

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_png, BinaryData::metal_knob_big_pngSize);
	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_png, BinaryData::black_knob_mid_pngSize);
	m_EQ_gain.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_ducking.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_EQ_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_dry_wet.setStrip(black_knob_mid, N_KNOB_FRAMES);

	m_EQ_gain.setBounds(REVERB_EQ_GAIN_POS_X, REVERB_EQ_GAIN_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
	m_ducking.setBounds(REVERB_DUCKING_POS_X, REVERB_DUCKING_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
	m_EQ_freq.setBounds(DRY_POS_X, DRY_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
	m_dry_wet.setBounds(WET_POS_X, WET_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);

	juce::Image reverb_image;
	//! todo
	reverb_image = ImageCache::getFromMemory(BinaryData::reverb_150_png, BinaryData::reverb_150_pngSize);
	setImage(reverb_image);

	forceValueTreeOntoComponents(m_value_tree.state);
}
