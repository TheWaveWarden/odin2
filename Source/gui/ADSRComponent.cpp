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

#include "ADSRComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
ADSRComponent::ADSRComponent(AudioProcessorValueTreeState &vts, const std::string &p_adsr_number) :
    m_loop("loop_button", juce::DrawableButton::ButtonStyle::ImageRaw), m_value_tree(vts),
    m_adsr_number(p_adsr_number) {

	m_attack_attach.reset(new OdinSliderAttachment(m_value_tree, ("env" + m_adsr_number + "_attack"), m_attack));
	m_decay_attach.reset(new OdinSliderAttachment(m_value_tree, "env" + m_adsr_number + "_decay", m_decay));
	m_sustain_attach.reset(new OdinSliderAttachment(m_value_tree, "env" + m_adsr_number + "_sustain", m_sustain));
	m_release_attach.reset(new OdinSliderAttachment(m_value_tree, "env" + m_adsr_number + "_release", m_release));

	m_loop.setClickingTogglesState(true);
	addAndMakeVisible(m_loop);
	m_loop.setAlwaysOnTop(true);
	m_loop.setTriggeredOnMouseDown(true);
	m_loop.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_loop.setTooltip("Loops the envelopes attack\n and decay sections");

	m_attack.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	addAndMakeVisible(m_attack);
	m_decay.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	addAndMakeVisible(m_decay);
	m_sustain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	addAndMakeVisible(m_sustain);
	m_release.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	addAndMakeVisible(m_release);

	m_attack.setRange(A_LOW_LIMIT, A_HIGH_LIMIT);
	m_attack.setTooltip("Attack\nDefines how long the envelope\ntakes to reach the top peak");
	m_attack.setTextValueSuffix(" s");

	m_decay.setTextValueSuffix(" s");
	m_decay.setTooltip("Decay\nDefines how long the\n envelope takes to fall "
	                   "from the top\n peak to the sustain level");

	m_sustain.setNumDecimalPlacesToDisplay(3);
	m_sustain.setTooltip("Sustain\nDefines the height of the evelope\nafter the "
	                     "decay section is finished");

	m_release.setTextValueSuffix(" s");
	m_release.setTooltip("Release\nDefines how long the envelope takes\n to fall "
	                     "back to zero after\nthe key is released");

	m_loop_attach.reset(new OdinButtonAttachment(m_value_tree, "env" + m_adsr_number + "_loop", m_loop));

	m_attack.setNumDecimalPlacesToDisplay(3);
	m_decay.setNumDecimalPlacesToDisplay(3);
	m_sustain.setNumDecimalPlacesToDisplay(3);
	m_release.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_attack);
	SET_CTR_KEY(m_decay);
	SET_CTR_KEY(m_sustain);
	SET_CTR_KEY(m_release);

	m_attack.setBounds(SLIDER_POS_X, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
	m_decay.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 1 - 1, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
	m_sustain.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 2, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
	m_release.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 3, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
}

ADSRComponent::~ADSRComponent() {
}

void ADSRComponent::paint(Graphics &g) {
	g.setColour(COL_LIGHT);
	g.drawRect(getLocalBounds(), 1);
}

void ADSRComponent::setGUIBig() {
	juce::Image loop_1 =
	    ImageCache::getFromMemory(BinaryData::buttonloop_1_150_png, BinaryData::buttonloop_1_150_pngSize);
	juce::Image loop_2 =
	    ImageCache::getFromMemory(BinaryData::buttonloop_2_150_png, BinaryData::buttonloop_2_150_pngSize);
	juce::Image loop_3 =
	    ImageCache::getFromMemory(BinaryData::buttonloop_3_150_png, BinaryData::buttonloop_3_150_pngSize);
	juce::Image loop_4 =
	    ImageCache::getFromMemory(BinaryData::buttonloop_4_150_png, BinaryData::buttonloop_4_150_pngSize);

	juce::DrawableImage loop_draw1;
	juce::DrawableImage loop_draw2;
	juce::DrawableImage loop_draw3;
	juce::DrawableImage loop_draw4;

	loop_draw1.setImage(loop_1);
	loop_draw2.setImage(loop_2);
	loop_draw3.setImage(loop_3);
	loop_draw4.setImage(loop_4);

	m_loop.setImages(
	    &loop_draw2, &loop_draw2, &loop_draw1, &loop_draw1, &loop_draw4, &loop_draw4, &loop_draw3, &loop_draw3);

	m_loop.setBounds(OdinHelper::c150(LOOP_POS_X), OdinHelper::c150(LOOP_POS_Y), loop_1.getWidth(), loop_1.getHeight());

	m_attack.setBounds(OdinHelper::c150(SLIDER_POS_X) - 3,
	                   OdinHelper::c150(SLIDER_POS_Y),
	                   OdinHelper::c150(SLIDER_SIZE_X),
	                   OdinHelper::c150(SLIDER_SIZE_Y));
	m_decay.setBounds(OdinHelper::c150(SLIDER_POS_X) + OdinHelper::c150(SLIDER_OFFSET) * 1 - 3,
	                  OdinHelper::c150(SLIDER_POS_Y),
	                  OdinHelper::c150(SLIDER_SIZE_X),
	                  OdinHelper::c150(SLIDER_SIZE_Y));
	m_sustain.setBounds(OdinHelper::c150(SLIDER_POS_X) + OdinHelper::c150(SLIDER_OFFSET) * 2 - 3,
	                    OdinHelper::c150(SLIDER_POS_Y),
	                    OdinHelper::c150(SLIDER_SIZE_X),
	                    OdinHelper::c150(SLIDER_SIZE_Y));
	m_release.setBounds(OdinHelper::c150(SLIDER_POS_X) + OdinHelper::c150(SLIDER_OFFSET) * 3 - 3,
	                    OdinHelper::c150(SLIDER_POS_Y),
	                    OdinHelper::c150(SLIDER_SIZE_X),
	                    OdinHelper::c150(SLIDER_SIZE_Y));

	m_attack.setHandle(
	    ImageCache::getFromMemory(BinaryData::slider_handle_150_png, BinaryData::slider_handle_150_pngSize));
	m_decay.setHandle(
	    ImageCache::getFromMemory(BinaryData::slider_handle_150_png, BinaryData::slider_handle_150_pngSize));
	m_sustain.setHandle(
	    ImageCache::getFromMemory(BinaryData::slider_handle_150_png, BinaryData::slider_handle_150_pngSize));
	m_release.setHandle(
	    ImageCache::getFromMemory(BinaryData::slider_handle_150_png, BinaryData::slider_handle_150_pngSize));
}
void ADSRComponent::setGUISmall() {

	juce::Image loop_1 = ImageCache::getFromMemory(BinaryData::buttonloop_1_png, BinaryData::buttonloop_1_pngSize);
	juce::Image loop_2 = ImageCache::getFromMemory(BinaryData::buttonloop_2_png, BinaryData::buttonloop_2_pngSize);
	juce::Image loop_3 = ImageCache::getFromMemory(BinaryData::buttonloop_3_png, BinaryData::buttonloop_3_pngSize);
	juce::Image loop_4 = ImageCache::getFromMemory(BinaryData::buttonloop_4_png, BinaryData::buttonloop_4_pngSize);

	juce::DrawableImage loop_draw1;
	juce::DrawableImage loop_draw2;
	juce::DrawableImage loop_draw3;
	juce::DrawableImage loop_draw4;

	loop_draw1.setImage(loop_1);
	loop_draw2.setImage(loop_2);
	loop_draw3.setImage(loop_3);
	loop_draw4.setImage(loop_4);

	m_loop.setImages(
	    &loop_draw2, &loop_draw2, &loop_draw1, &loop_draw1, &loop_draw4, &loop_draw4, &loop_draw3, &loop_draw3);

	m_loop.setBounds(LOOP_POS_X, LOOP_POS_Y, loop_1.getWidth(), loop_1.getHeight());

	m_attack.setBounds(SLIDER_POS_X, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
	m_decay.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 1 - 1, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
	m_sustain.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 2, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
	m_release.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 3, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);

	m_attack.setHandle(ImageCache::getFromMemory(BinaryData::slider_handle_png, BinaryData::slider_handle_pngSize));
	m_decay.setHandle(ImageCache::getFromMemory(BinaryData::slider_handle_png, BinaryData::slider_handle_pngSize));
	m_sustain.setHandle(ImageCache::getFromMemory(BinaryData::slider_handle_png, BinaryData::slider_handle_pngSize));
	m_release.setHandle(ImageCache::getFromMemory(BinaryData::slider_handle_png, BinaryData::slider_handle_pngSize));
}