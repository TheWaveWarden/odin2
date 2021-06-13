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
#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"

PhaserComponent::PhaserComponent(AudioProcessorValueTreeState &vts, const std::string &p_fx_name, bool p_is_standalone) :
    m_value_tree(vts), m_fx_name(p_fx_name), m_is_standalone_plugin(p_is_standalone),
    m_sync("sync", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_fx_synctime_denominator_identifier(p_fx_name + "_synctime_denominator"),
    m_fx_synctime_numerator_identifier(p_fx_name + "_synctime_numerator") {

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
		m_value_tree.state.getChildWithName("fx").setProperty(
		    (Identifier)("phaser_sync"), m_sync.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage("phaser_sync");
	};

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		m_value_tree.state.getChildWithName("fx").setProperty(m_fx_synctime_numerator_identifier, p_left, nullptr);
		m_value_tree.state.getChildWithName("fx").setProperty(m_fx_synctime_denominator_identifier, p_right, nullptr);
	};
	m_sync_time.setTopLeftPosition(PHASER_SYNC_TIME_POS_X, PHASER_SYNC_TIME_POS_Y);
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
	SET_INTERPOLATION_QUALITY(g)
	if (m_sync_enabled) {
		g.drawImageAt(m_background_sync, 0, 0);
	} else {
		g.drawImageAt(m_background_no_sync, 0, 0);
	}
}

void PhaserComponent::setSyncEnabled(bool p_sync) {
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

void PhaserComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	m_sync_time.setValues(m_value_tree.state.getChildWithName("fx")[m_fx_synctime_numerator_identifier],
	                      m_value_tree.state.getChildWithName("fx")[m_fx_synctime_denominator_identifier]);

	m_sync.setToggleState((float)m_value_tree.state.getChildWithName("fx")["phaser_sync"] > 0.5f, dontSendNotification);
	setSyncEnabled((float)m_value_tree.state.getChildWithName("fx")["phaser_sync"] > 0.5f);
	//send change message to set member in processor
	m_value_tree.state.getChildWithName("fx").sendPropertyChangeMessage((Identifier)("phaser_sync"));
}

void PhaserComponent::setGUIBig() {
	m_GUI_big = true;

	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_150_png, BinaryData::metal_knob_mid_150_pngSize);
	m_mod.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_rate.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_freq.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_feedback.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_dry_wet.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	juce::Image reset_1 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_1_150_png, BinaryData::buttonreset_lfo_1_150_pngSize);
	juce::Image reset_2 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_2_150_png, BinaryData::buttonreset_lfo_2_150_pngSize);
	juce::Image reset_3 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_3_150_png, BinaryData::buttonreset_lfo_3_150_pngSize);
	juce::Image reset_4 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_4_150_png, BinaryData::buttonreset_lfo_4_150_pngSize);

	juce::DrawableImage reset_draw1;
	juce::DrawableImage reset_draw2;
	juce::DrawableImage reset_draw3;
	juce::DrawableImage reset_draw4;

	reset_draw1.setImage(reset_1);
	reset_draw2.setImage(reset_2);
	reset_draw3.setImage(reset_3);
	reset_draw4.setImage(reset_4);

	m_reset.setImages(
	    &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1, &reset_draw4, &reset_draw4, &reset_draw3, &reset_draw3);
	m_reset.setBounds(OdinHelper::c150(PHASER_RESET_POS_X),
	                  OdinHelper::c150(PHASER_RESET_POS_Y),
	                  reset_1.getWidth(),
	                  reset_1.getHeight());

	juce::Image sync_1 =
	    ImageCache::getFromMemory(BinaryData::buttonsync_1_150_png, BinaryData::buttonsync_1_150_pngSize);
	juce::Image sync_2 =
	    ImageCache::getFromMemory(BinaryData::buttonsync_2_150_png, BinaryData::buttonsync_2_150_pngSize);
	juce::Image sync_3 =
	    ImageCache::getFromMemory(BinaryData::buttonsync_3_150_png, BinaryData::buttonsync_3_150_pngSize);
	juce::Image sync_4 =
	    ImageCache::getFromMemory(BinaryData::buttonsync_4_150_png, BinaryData::buttonsync_4_150_pngSize);

	juce::DrawableImage sync_draw1;
	juce::DrawableImage sync_draw2;
	juce::DrawableImage sync_draw3;
	juce::DrawableImage sync_draw4;

	sync_draw1.setImage(sync_1);
	sync_draw2.setImage(sync_2);
	sync_draw3.setImage(sync_3);
	sync_draw4.setImage(sync_4);

	m_sync.setImages(
	    &sync_draw2, &sync_draw2, &sync_draw1, &sync_draw1, &sync_draw4, &sync_draw4, &sync_draw3, &sync_draw3);
	m_sync.setBounds(OdinHelper::c150(PHASER_SYNC_POS_X),
	                 OdinHelper::c150(PHASER_SYNC_POS_Y),
	                 sync_1.getWidth(),
	                 sync_1.getHeight());
	m_sync_time.setTopLeftPosition(OdinHelper::c150(PHASER_SYNC_TIME_POS_X), OdinHelper::c150(PHASER_SYNC_TIME_POS_Y));

	m_mod.setBounds(OdinHelper::c150(PHASER_MOD_POS_X)-2,
	                OdinHelper::c150(PHASER_MOD_POS_Y)-1,
	                metal_knob_mid.getWidth(),
	                metal_knob_mid.getWidth());
	m_rate.setBounds(OdinHelper::c150(PHASER_RATE_POS_X)-2,
	                 OdinHelper::c150(PHASER_RATE_POS_Y)-1,
	                 metal_knob_mid.getWidth(),
	                 metal_knob_mid.getWidth());
	m_dry_wet.setBounds(OdinHelper::c150(PHASER_DRY_WET_POS_X)-2,
	                    OdinHelper::c150(PHASER_DRY_WET_POS_Y)-1,
	                    metal_knob_mid.getWidth(),
	                    metal_knob_mid.getWidth());
	m_freq.setBounds(OdinHelper::c150(PHASER_FREQ_POS_X)-2,
	                 OdinHelper::c150(PHASER_FREQ_POS_Y)-1,
	                 metal_knob_mid.getWidth(),
	                 metal_knob_mid.getWidth());
	m_feedback.setBounds(OdinHelper::c150(PHASER_FEEDBACK_POS_X)-2,
	                     OdinHelper::c150(PHASER_FEEDBACK_POS_Y)-1,
	                     metal_knob_mid.getWidth(),
	                     metal_knob_mid.getWidth());

	m_background_sync = ImageCache::getFromMemory(BinaryData::phasersync_150_png, BinaryData::phasersync_150_pngSize);
	m_background_no_sync =
	    ImageCache::getFromMemory(BinaryData::phasernosync_150_png, BinaryData::phasernosync_150_pngSize);
	m_sync_time.setGUIBig();

	forceValueTreeOntoComponents(m_value_tree.state);
}
void PhaserComponent::setGUISmall() {
	m_GUI_big = false;

	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_png, BinaryData::metal_knob_mid_pngSize);
	m_mod.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_rate.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_freq.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_feedback.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_dry_wet.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	juce::Image reset_1 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_1_png, BinaryData::buttonreset_lfo_1_pngSize);
	juce::Image reset_2 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_2_png, BinaryData::buttonreset_lfo_2_pngSize);
	juce::Image reset_3 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_3_png, BinaryData::buttonreset_lfo_3_pngSize);
	juce::Image reset_4 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_lfo_4_png, BinaryData::buttonreset_lfo_4_pngSize);

	juce::DrawableImage reset_draw1;
	juce::DrawableImage reset_draw2;
	juce::DrawableImage reset_draw3;
	juce::DrawableImage reset_draw4;

	reset_draw1.setImage(reset_1);
	reset_draw2.setImage(reset_2);
	reset_draw3.setImage(reset_3);
	reset_draw4.setImage(reset_4);

	m_reset.setImages(
	    &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1, &reset_draw4, &reset_draw4, &reset_draw3, &reset_draw3);
	m_reset.setBounds(PHASER_RESET_POS_X, PHASER_RESET_POS_Y, reset_1.getWidth(), reset_1.getHeight());

	juce::Image sync_1 = ImageCache::getFromMemory(BinaryData::buttonsync_1_png, BinaryData::buttonsync_1_pngSize);
	juce::Image sync_2 = ImageCache::getFromMemory(BinaryData::buttonsync_2_png, BinaryData::buttonsync_2_pngSize);
	juce::Image sync_3 = ImageCache::getFromMemory(BinaryData::buttonsync_3_png, BinaryData::buttonsync_3_pngSize);
	juce::Image sync_4 = ImageCache::getFromMemory(BinaryData::buttonsync_4_png, BinaryData::buttonsync_4_pngSize);

	juce::DrawableImage sync_draw1;
	juce::DrawableImage sync_draw2;
	juce::DrawableImage sync_draw3;
	juce::DrawableImage sync_draw4;

	sync_draw1.setImage(sync_1);
	sync_draw2.setImage(sync_2);
	sync_draw3.setImage(sync_3);
	sync_draw4.setImage(sync_4);

	m_sync.setImages(
	    &sync_draw2, &sync_draw2, &sync_draw1, &sync_draw1, &sync_draw4, &sync_draw4, &sync_draw3, &sync_draw3);
	m_sync.setBounds(PHASER_SYNC_POS_X, PHASER_SYNC_POS_Y, sync_1.getWidth(), sync_1.getHeight());
	m_sync_time.setTopLeftPosition(PHASER_SYNC_TIME_POS_X, PHASER_SYNC_TIME_POS_Y);

	m_mod.setBounds(PHASER_MOD_POS_X, PHASER_MOD_POS_Y, metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
	m_rate.setBounds(PHASER_RATE_POS_X, PHASER_RATE_POS_Y, metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
	m_dry_wet.setBounds(
	    PHASER_DRY_WET_POS_X, PHASER_DRY_WET_POS_Y, metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
	m_freq.setBounds(PHASER_FREQ_POS_X, PHASER_FREQ_POS_Y, metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
	m_feedback.setBounds(
	    PHASER_FEEDBACK_POS_X, PHASER_FEEDBACK_POS_Y, metal_knob_mid.getWidth(), metal_knob_mid.getWidth());

	m_background_sync    = ImageCache::getFromMemory(BinaryData::phasersync_png, BinaryData::phasersync_pngSize);
	m_background_no_sync = ImageCache::getFromMemory(BinaryData::phasernosync_png, BinaryData::phasernosync_pngSize);

	m_sync_time.setGUISmall();
	forceValueTreeOntoComponents(m_value_tree.state);
}