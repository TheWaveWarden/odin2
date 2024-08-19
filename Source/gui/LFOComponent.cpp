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
#include "LFOComponent.h"

LFOComponent::LFOComponent(AudioProcessorValueTreeState &vts, const std::string &p_lfo_number, bool p_is_standalone) :
    m_value_tree(vts), m_lfo_number(p_lfo_number), m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_sync("sync", juce::DrawableButton::ButtonStyle::ImageRaw), m_lfo_wave_identifier("lfo" + p_lfo_number + "_wave"),
    m_lfo_synctime_denominator_identifier("lfo" + p_lfo_number + "_synctime_denominator"),
    m_lfo_synctime_numerator_identifier("lfo" + p_lfo_number + "_synctime_numerator"),
    m_is_standalone_plugin(p_is_standalone) {

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
		m_value_tree.state.getChildWithName("lfo").setProperty(
		    (Identifier)("lfo" + m_lfo_number + "_sync"), m_sync.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("lfo").sendPropertyChangeMessage(
		    (Identifier)("lfo" + m_lfo_number + "_sync"));
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
	m_selector.setTopLeftPosition(SELECTOR_POS_X, SELECTOR_POS_Y);
	m_selector.setTooltip("The waveform to be used for this LFO. WD 1-3 use "
	                      "the WaveDraw tables drawn in oscillators 1-3. CD 1-3 use the ChipDraw tables.");
	addAndMakeVisible(m_selector);

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		m_value_tree.state.getChildWithName("lfo").setProperty(m_lfo_synctime_numerator_identifier, p_left, nullptr);
		m_value_tree.state.getChildWithName("lfo").setProperty(m_lfo_synctime_denominator_identifier, p_right, nullptr);
	};
	m_sync_time.setTopLeftPosition(SYNC_TIME_POS_X, SYNC_TIME_POS_Y);
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

void LFOComponent::paint(Graphics &g) {
	g.setColour(COL_LIGHT);
	g.drawRect(getLocalBounds(), 1);
}

void LFOComponent::forceValueTreeOntoComponents(ValueTree p_tree) {

	m_selector.setValueGUIOnly(m_value_tree.state.getChildWithName("lfo")[m_lfo_wave_identifier]);

	m_sync_time.setValues(m_value_tree.state.getChildWithName("lfo")[m_lfo_synctime_numerator_identifier],
	                      m_value_tree.state.getChildWithName("lfo")[m_lfo_synctime_denominator_identifier]);
	setSync((float)m_value_tree.state.getChildWithName("lfo")[(Identifier)("lfo" + m_lfo_number + "_sync")] > 0.5f);
	//m_value_tree.state.getChildWithName("lfo").sendPropertyChangeMessage((Identifier)("lfo" + m_lfo_number + "_sync"));
	m_sync.setToggleState(
	    (float)m_value_tree.state.getChildWithName("lfo")[(Identifier)("lfo" + m_lfo_number + "_sync")] > 0.5f,
	    dontSendNotification);
}

void LFOComponent::setGUIBig() {
	m_GUI_big = true;

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
	m_reset.setBounds(
	    OdinHelper::c150(LFO_RESET_POS_X), OdinHelper::c150(LFO_RESET_POS_Y), reset_1.getWidth(), reset_1.getHeight());

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
	m_sync.setBounds(OdinHelper::c150(SYNC_POS_X), OdinHelper::c150(SYNC_POS_Y), sync_1.getWidth(), sync_1.getHeight());

	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_small_150_png, BinaryData::black_knob_small_150_pngSize);

	m_freq.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_selector.setTopLeftPosition(OdinHelper::c150(SELECTOR_POS_X), OdinHelper::c150(SELECTOR_POS_Y));
	m_sync_time.setTopLeftPosition(OdinHelper::c150(SYNC_TIME_POS_X), OdinHelper::c150(SYNC_TIME_POS_Y));

	m_freq.setBounds(OdinHelper::c150(LFO_FREQ_POS_X),
	                 OdinHelper::c150(LFO_FREQ_POS_Y),
	                 OdinHelper::c150(BLACK_KNOB_SMALL_SIZE_X),
	                 OdinHelper::c150(BLACK_KNOB_SMALL_SIZE_Y));

	m_selector.setGUIBig();
	m_sync_time.setGUIBig();

	m_knob_guide =
	    ImageCache::getFromMemory(BinaryData::arp_knob_guide_150_png, BinaryData::arp_knob_guide_150_pngSize);
}

void LFOComponent::setGUISmall() {
	m_GUI_big = false;

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
	m_reset.setBounds(LFO_RESET_POS_X, LFO_RESET_POS_Y, reset_1.getWidth(), reset_1.getHeight());

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
	m_sync.setBounds(SYNC_POS_X, SYNC_POS_Y, sync_1.getWidth(), sync_1.getHeight());

	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_small_png, BinaryData::black_knob_small_pngSize);

	m_freq.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_selector.setTopLeftPosition(SELECTOR_POS_X, SELECTOR_POS_Y);
	m_sync_time.setTopLeftPosition(SYNC_TIME_POS_X, SYNC_TIME_POS_Y);

	m_selector.setGUISmall();
	m_sync_time.setGUISmall();

	m_freq.setBounds(LFO_FREQ_POS_X, LFO_FREQ_POS_Y, BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);

	m_knob_guide = ImageCache::getFromMemory(BinaryData::arp_knob_guide_png, BinaryData::arp_knob_guide_pngSize);
}