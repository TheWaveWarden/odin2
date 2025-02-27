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
#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"

//==============================================================================
ADSRComponent::ADSRComponent(AudioProcessorValueTreeState &vts, const std::string &p_adsr_number) :
    m_loop("loop_button", "", OdinButton::Type::loop),
    m_value_tree(vts),
    m_adsr_number(p_adsr_number),
    m_attack_label("A"),
    m_decay_label("D"),
    m_sustain_label("S"),
    m_release_label("R") {

	addAndMakeVisible(m_attack_label);
	addAndMakeVisible(m_decay_label);
	addAndMakeVisible(m_sustain_label);
	addAndMakeVisible(m_release_label);

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
}

ADSRComponent::~ADSRComponent() {
}

void ADSRComponent::resized() {
	GET_LOCAL_AREA(m_attack_label, "AttackLabel");
	GET_LOCAL_AREA(m_decay_label, "DecayLabel");
	GET_LOCAL_AREA(m_sustain_label, "SustainLabel");
	GET_LOCAL_AREA(m_release_label, "ReleaseLabel");

	GET_LOCAL_AREA(m_attack, "ADSRAttack");
	GET_LOCAL_AREA(m_decay, "ADSRDecay");
	GET_LOCAL_AREA(m_sustain, "ADSRSustain");
	GET_LOCAL_AREA(m_release, "ADSRRelease");

	// this one was position not conforming to the grid, so we need to move it half a cell to center align it with "D"
	GET_LOCAL_AREA(m_loop, "ADSRLoop");
	const auto grid = ConfigFileManager::getInstance().getOptionGuiScale();
	m_loop.setBounds(m_loop.getBounds().translated(grid / 2, 0));
}