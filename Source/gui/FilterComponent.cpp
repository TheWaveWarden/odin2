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

#include "FilterComponent.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"

FilterComponent::FilterComponent(AudioProcessorValueTreeState &vts, const std::string &p_filter_number) :
    m_value_tree(vts),
    m_filter_number(p_filter_number),
    m_vowel_left(false),
    m_vowel_right(true),
    m_vowel_left_identifier("fil" + p_filter_number + "_vowel_left"),
    m_vowel_right_identifier("fil" + p_filter_number + "_vowel_right"),
    m_comb_plus_minus("comb_plus_minus", LeftRightButton::Type::filter_comb_polarity),
    m_vel_label("Vel"),
    m_kbd_label("Kbd"),
    m_env_label("Env"),
    m_gain_label("Gain"),
    m_freq_label("Frequency"),
    m_res_label("Resonance"),
    m_freq_comb_label("Frequency"),
    m_res_comb_label("Resonance"),
    m_saturation_label("Saturation"),
    m_formant_transition_label("Transition"),
    m_ring_mod_amount_label("Amount"),
    m_sem_notch_label("Notch"),
    m_sem_lp_label("LP"),
    m_sem_hp_label("HP"),
    m_vowel1_label("Vowel 1"),
    m_vowel2_label("Vowel 2"),
    m_vel(OdinKnob::Type::knob_4x4a),
    m_env(OdinKnob::Type::knob_4x4a),
    m_kbd(OdinKnob::Type::knob_4x4a),
    m_gain(OdinKnob::Type::knob_4x4a),
    m_res(OdinKnob::Type::knob_5x5a),
    m_sem_transition(OdinKnob::Type::knob_5x5a),
    m_saturation(OdinKnob::Type::knob_5x5a),
    m_freq(OdinKnob::Type::knob_8x8a),
    m_freq_comb(OdinKnob::Type::knob_8x8a),
    m_res_comb(OdinKnob::Type::knob_8x8a),
    m_ring_mod_amount(OdinKnob::Type::knob_8x8a),
    m_formant_transition(OdinKnob::Type::knob_8x8a) {

	addAndMakeVisible(m_vel_label);
	addAndMakeVisible(m_kbd_label);
	addAndMakeVisible(m_env_label);
	addAndMakeVisible(m_gain_label);
	addAndMakeVisible(m_freq_label);
	addAndMakeVisible(m_res_label);
	addAndMakeVisible(m_freq_comb_label);
	addAndMakeVisible(m_res_comb_label);
	addAndMakeVisible(m_saturation_label);
	addAndMakeVisible(m_formant_transition_label);
	addAndMakeVisible(m_ring_mod_amount_label);
	addAndMakeVisible(m_sem_notch_label);
	addAndMakeVisible(m_sem_lp_label);
	addAndMakeVisible(m_sem_hp_label);
	addAndMakeVisible(m_vowel1_label);
	addAndMakeVisible(m_vowel2_label);

	m_vel_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_vel", m_vel));
	m_env_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_env", m_env));
	m_kbd_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_kbd", m_kbd));
	m_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_gain", m_gain));
	m_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_freq", m_freq));
	m_res_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_res", m_res));
	m_freq_comb_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_freq", m_freq_comb));
	m_res_comb_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_res", m_res_comb));
	m_saturation_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_saturation", m_saturation));
	m_formant_transition_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_formant_transition", m_formant_transition));
	m_ring_mod_amount_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_ring_mod_amount", m_ring_mod_amount));
	m_sem_transition_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_sem_transition", m_sem_transition));

	m_vel.setSliderStyle(Slider::RotaryVerticalDrag);
	m_vel.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_vel.setKnobTooltip("Velocity sensitivity. Sets how much the MIDI velocity "
	                     "value is added to the filter freqency");
	addChildComponent(m_vel);

	m_kbd.setSliderStyle(Slider::RotaryVerticalDrag);
	m_kbd.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_kbd.setKnobTooltip("Keyboard follow.\nSets how much the MIDI note\n is "
	                     "added to the\nfilter frequency");
	addChildComponent(m_kbd);

	m_env.setSliderStyle(Slider::RotaryVerticalDrag);
	m_env.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_env.setRange(ENV_MIN, ENV_MAX);
	m_env.setSkewFactor(ENV_SKEW, true);
	m_env.setNumDecimalPlacesToDisplay(3);
	m_env.setKnobTooltip("Filter envelope amount.\nSets how much the "
	                     "filter\nenvelope modulates the filter");
	addChildComponent(m_env);

	m_gain.setSliderStyle(Slider::RotaryVerticalDrag);
	m_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_gain.setRange(GAIN_MIN, GAIN_MAX);
	m_gain.setTextValueSuffix(" dB");
	m_gain.setNumDecimalPlacesToDisplay(1);
	m_gain.setKnobTooltip("The volume of\n the filter");
	addChildComponent(m_gain);

	m_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_freq.setRange(FREQ_MIN, FREQ_MAX);
	m_freq.setKnobTooltip("The filter cutoff frequency");
	m_freq.setTextValueSuffix(" Hz");
	m_freq.setNumDecimalPlacesToDisplay(1);
	addChildComponent(m_freq);

	m_freq_comb.setSliderStyle(Slider::RotaryVerticalDrag);
	m_freq_comb.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_freq_comb.setRange(FREQ_MIN, FREQ_MAX);
	m_freq_comb.setKnobTooltip("The filter cutoff frequency");
	m_freq_comb.setTextValueSuffix(" Hz");
	m_freq_comb.setNumDecimalPlacesToDisplay(1);
	addChildComponent(m_freq_comb);

	m_res.setSliderStyle(Slider::RotaryVerticalDrag);
	m_res.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_res.setKnobTooltip("Creates a resonant peak at the cutoff "
	                     "frequency.\nFilters will not\n self oscillate");
	addChildComponent(m_res);

	m_res_comb.setSliderStyle(Slider::RotaryVerticalDrag);
	m_res_comb.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_res_comb.setKnobTooltip("Creates a resonant peak at the cutoff "
	                          "frequency.\nFilters will not\n self oscillate");
	addChildComponent(m_res_comb);

	m_saturation.setSliderStyle(Slider::RotaryVerticalDrag);
	m_saturation.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_saturation.setKnobTooltip("Saturates the filter\nand introduces a\nslight distortion");
	addChildComponent(m_saturation);

	m_sem_transition.setSliderStyle(Slider::RotaryVerticalDrag);
	m_sem_transition.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_sem_transition.setRange(TRANSITION_MIN, TRANSITION_MAX);
	m_sem_transition.setNumDecimalPlacesToDisplay(3);
	m_sem_transition.setKnobTooltip("Sets the filter characteristic\nfrom a "
	                                "lowpass over a\nnotch to a highpass");
	addChildComponent(m_sem_transition);

	m_formant_transition.setSliderStyle(Slider::RotaryVerticalDrag);
	m_formant_transition.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_formant_transition.setKnobTooltip("Transitions from the vowel on the\nleft to the one on the right");
	addChildComponent(m_formant_transition);

	m_ring_mod_amount.setSliderStyle(Slider::RotaryVerticalDrag);
	m_ring_mod_amount.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_ring_mod_amount.setKnobTooltip("How much ring modulation is applied to the signal");
	addChildComponent(m_ring_mod_amount);

	m_vowel_left.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("misc").setProperty(m_vowel_left_identifier, p_new_value, nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage(m_vowel_left_identifier);
	};
	m_vowel_left.setTooltip("The vowel to\nthe left side of\nthe transition knob");
	m_vowel_left.setMouseDragDivisor(40.f);
	addChildComponent(m_vowel_left);

	m_vowel_right.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("misc").setProperty(m_vowel_right_identifier, p_new_value, nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage(m_vowel_right_identifier);
	};
	m_vowel_right.setTooltip("The vowel to\nthe right side of\nthe transition knob");
	m_vowel_right.setMouseDragDivisor(40.f);
	addChildComponent(m_vowel_right);

	m_comb_plus_minus.onClick = [&]() {
		m_value_tree.state.getChildWithName("misc").setProperty((Identifier)("fil" + m_filter_number + "_comb_polarity"), m_comb_plus_minus.getToggleState(), nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage((Identifier)("fil" + m_filter_number + "_comb_polarity"));
	};

	m_comb_plus_minus.setTooltip("Whether to add or subtract the feedback\n in the internal delay line");
	addChildComponent(m_comb_plus_minus);

	m_res.setNumDecimalPlacesToDisplay(3);
	m_saturation.setNumDecimalPlacesToDisplay(3);
	m_gain.setNumDecimalPlacesToDisplay(2);
	m_vel.setNumDecimalPlacesToDisplay(3);
	m_kbd.setNumDecimalPlacesToDisplay(3);
	m_formant_transition.setNumDecimalPlacesToDisplay(3);
	m_ring_mod_amount.setNumDecimalPlacesToDisplay(3);

	m_vowel_left.setParameterId("fil" + m_filter_number + "_vowel_left");
	m_vowel_right.setParameterId("fil" + m_filter_number + "_vowel_right");

	SET_CTR_KEY(m_vel);
	SET_CTR_KEY(m_kbd);
	SET_CTR_KEY(m_env);
	SET_CTR_KEY(m_gain);
	SET_CTR_KEY(m_freq);
	SET_CTR_KEY(m_res);
	SET_CTR_KEY(m_freq_comb);
	SET_CTR_KEY(m_res_comb);
	SET_CTR_KEY(m_saturation);
	SET_CTR_KEY(m_sem_transition);
	SET_CTR_KEY(m_formant_transition);
	SET_CTR_KEY(m_ring_mod_amount);

	forceValueTreeOntoComponents(m_value_tree.state, std::stoi(m_filter_number));

	setWantsKeyboardFocus(true);
}

FilterComponent::~FilterComponent() {
	m_value_tree.removeParameterListener("fil" + m_filter_number + "_vowel_left", &m_vowel_left);
	m_value_tree.removeParameterListener("fil" + m_filter_number + "_vowel_right", &m_vowel_right);
}

void FilterComponent::paint(Graphics &g) {

	auto asset = UIAssets::Indices::Filter_Lowpass24;
	switch (m_filter_type) {
	case 1:
		return;
	case FILTER_TYPE_LP24:
		asset = UIAssets::Indices::Filter_Lowpass24;
		break;
	case FILTER_TYPE_LP12:
		asset = UIAssets::Indices::Filter_Lowpass12;
		break;
	case FILTER_TYPE_BP24:
		asset = UIAssets::Indices::Filter_Bandpass24;
		break;
	case FILTER_TYPE_BP12:
		asset = UIAssets::Indices::Filter_Bandpass12;
		break;
	case FILTER_TYPE_HP24:
		asset = UIAssets::Indices::Filter_Highpass24;
		break;
	case FILTER_TYPE_HP12:
		asset = UIAssets::Indices::Filter_Highpass12;
		break;
	case FILTER_TYPE_SEM12:
		asset = UIAssets::Indices::Filter_SEM12;
		break;
	case FILTER_TYPE_DIODE:
		asset = UIAssets::Indices::Filter_DiodeLadder;
		break;
	case FILTER_TYPE_KORG_LP:
		asset = UIAssets::Indices::Filter_KRG35LP;
		break;
	case FILTER_TYPE_KORG_HP:
		asset = UIAssets::Indices::Filter_KRG35HP;
		break;
	case FILTER_TYPE_COMB:
		asset = UIAssets::Indices::Filter_Combfilter;
		break;
	case FILTER_TYPE_FORMANT:
		asset = UIAssets::Indices::Filter_Formant;
		break;
	case FILTER_TYPE_RINGMOD:
		asset = UIAssets::Indices::Filter_Ringmod;
		break;
	default:
		break;
	}

	auto background = UIAssetManager::getInstance()->getUIAsset(asset, ConfigFileManager::getInstance().getOptionGuiScale());
	jassert(background.getWidth() == getWidth());
	jassert(background.getHeight() == getHeight());
	g.drawImageAt(background, 0, 0);
}

void FilterComponent::setFilterType(int p_filter_type, bool p_force) {
	if (p_filter_type == 0) {
		return;
	}
	if (m_filter_type == p_filter_type && !p_force) {
		return;
	}
	m_filter_type = p_filter_type;
	hideAllComponents();
	switch (p_filter_type) {
	case 1:
		setFilterBypass();
		break;
	case FILTER_TYPE_LP24:
		setFilterLP24();
		break;
	case FILTER_TYPE_LP12:
		setFilterLP12();
		break;
	case FILTER_TYPE_BP24:
		setFilterBP24();
		break;
	case FILTER_TYPE_BP12:
		setFilterBP12();
		break;
	case FILTER_TYPE_HP24:
		setFilterHP24();
		break;
	case FILTER_TYPE_HP12:
		setFilterHP12();
		break;
	case FILTER_TYPE_SEM12:
		setFilterSEM12();
		break;
	case FILTER_TYPE_DIODE:
		setFilterDiode();
		break;
	case FILTER_TYPE_KORG_LP:
		setFilterKorgLP();
		break;
	case FILTER_TYPE_KORG_HP:
		setFilterKorgHP();
		break;
	case FILTER_TYPE_COMB:
		setFilterComb();
		break;
	case FILTER_TYPE_FORMANT:
		setFilterFormant();
		break;
	case FILTER_TYPE_RINGMOD:
		setFilterRingMod();
		break;
	default:
		setFilterBypass();
		break;
	}

	repaint();
}

void FilterComponent::hideAllComponents() {
	m_vel.setVisible(false);
	m_kbd.setVisible(false);
	m_env.setVisible(false);
	m_gain.setVisible(false);
	m_freq.setVisible(false);
	m_res.setVisible(false);
	m_freq_comb.setVisible(false);
	m_res_comb.setVisible(false);
	m_saturation.setVisible(false);
	m_sem_transition.setVisible(false);
	m_formant_transition.setVisible(false);
	m_ring_mod_amount.setVisible(false);
	m_vowel_left.setVisible(false);
	m_vowel_right.setVisible(false);
	m_comb_plus_minus.setVisible(false);

	m_vel_label.setVisible(false);
	m_kbd_label.setVisible(false);
	m_env_label.setVisible(false);
	m_gain_label.setVisible(false);
	m_freq_label.setVisible(false);
	m_res_label.setVisible(false);
	m_freq_comb_label.setVisible(false);
	m_res_comb_label.setVisible(false);
	m_saturation_label.setVisible(false);

	m_vowel1_label.setVisible(false);
	m_vowel2_label.setVisible(false);
	m_formant_transition_label.setVisible(false);

	m_sem_notch_label.setVisible(false);
	m_sem_lp_label.setVisible(false);
	m_sem_hp_label.setVisible(false);

	m_ring_mod_amount_label.setVisible(false);
}

void FilterComponent::setFilterBypass() {
}

void FilterComponent::setFilterLP24() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterLP12() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterBP24() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterBP12() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterHP24() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterHP12() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterSEM24() {
	showSEMFilterComponents();
}

void FilterComponent::setFilterSEM12() {
	showSEMFilterComponents();
}

void FilterComponent::setFilterDiode() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterKorgLP() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterKorgHP() {
	showStandardFilterComponents();
}

void FilterComponent::setFilterComb() {
	showCombFilterComponents();
}

void FilterComponent::setFilterFormant() {
	showFormantFilterComponents();
}

void FilterComponent::setFilterRingMod() {
	showRingModFilterComponents();
}

void FilterComponent::showSEMFilterComponents() {
	m_vel.setVisible(true);
	m_kbd.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_freq.setVisible(true);
	m_res.setVisible(true);
	m_sem_transition.setVisible(true);

	m_vel_label.setVisible(true);
	m_kbd_label.setVisible(true);
	m_env_label.setVisible(true);
	m_gain_label.setVisible(true);
	m_freq_label.setVisible(true);
	m_res_label.setVisible(true);

	m_sem_notch_label.setVisible(true);
	m_sem_lp_label.setVisible(true);
	m_sem_hp_label.setVisible(true);
}

void FilterComponent::showStandardFilterComponents() {
	m_vel.setVisible(true);
	m_kbd.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_freq.setVisible(true);
	m_res.setVisible(true);
	m_saturation.setVisible(true);

	m_vel_label.setVisible(true);
	m_kbd_label.setVisible(true);
	m_env_label.setVisible(true);
	m_gain_label.setVisible(true);
	m_freq_label.setVisible(true);
	m_res_label.setVisible(true);
	m_saturation_label.setVisible(true);
}

void FilterComponent::showCombFilterComponents() {
	m_vel.setVisible(true);
	m_kbd.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_freq_comb.setVisible(true);
	m_res_comb.setVisible(true);

	m_vel_label.setVisible(true);
	m_kbd_label.setVisible(true);
	m_env_label.setVisible(true);
	m_gain_label.setVisible(true);
	m_freq_comb_label.setVisible(true);
	m_res_comb_label.setVisible(true);

	m_comb_plus_minus.setVisible(true);
}

void FilterComponent::showFormantFilterComponents() {
	m_vel.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_vel_label.setVisible(true);
	m_env_label.setVisible(true);
	m_gain_label.setVisible(true);
	m_vowel_right.setVisible(true);
	m_vowel_left.setVisible(true);
	m_formant_transition.setVisible(true);

	m_vowel1_label.setVisible(true);
	m_vowel2_label.setVisible(true);
	m_formant_transition_label.setVisible(true);
}

void FilterComponent::showRingModFilterComponents() {
	m_kbd.setVisible(true);
	m_vel.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_kbd_label.setVisible(true);
	m_vel_label.setVisible(true);
	m_env_label.setVisible(true);
	m_gain_label.setVisible(true);
	m_freq_comb.setVisible(true);
	m_freq_comb_label.setVisible(true);
	m_ring_mod_amount.setVisible(true);
	m_ring_mod_amount_label.setVisible(true);
}

void FilterComponent::forceValueTreeOntoComponents(ValueTree p_tree, int p_index) {
	m_comb_plus_minus.setValue(m_value_tree.state.getChildWithName("misc")[(Identifier)("fil" + m_filter_number + "_comb_polarity")]);

	m_vowel_left.setValue(m_value_tree.state.getChildWithName("misc")[m_vowel_left_identifier]);
	m_vowel_right.setValue(m_value_tree.state.getChildWithName("misc")[m_vowel_right_identifier]);
}

void FilterComponent::resized() {
	GET_LOCAL_AREA(m_vowel1_label, "FilVowel1Label");
	GET_LOCAL_AREA(m_vowel2_label, "FilVowel2Label");
	GET_LOCAL_AREA(m_sem_notch_label, "FilSemNotchLabel");
	GET_LOCAL_AREA(m_sem_lp_label, "FilSemLpLabel");
	GET_LOCAL_AREA(m_sem_hp_label, "FilSemHpLabel");
	GET_LOCAL_AREA(m_vel_label, "FilVelLabel");
	GET_LOCAL_AREA(m_kbd_label, "FilKbdLabel");
	GET_LOCAL_AREA(m_env_label, "FilEnvLabel");
	GET_LOCAL_AREA(m_gain_label, "FilGainLabel");
	GET_LOCAL_AREA(m_freq_label, "FilFreqLabel");
	GET_LOCAL_AREA(m_res_label, "FilResLabel");
	GET_LOCAL_AREA(m_freq_comb_label, "FilFreqCombLabel");
	GET_LOCAL_AREA(m_res_comb_label, "FilResCombLabel");
	GET_LOCAL_AREA(m_saturation_label, "FilSaturationLabel");
	GET_LOCAL_AREA(m_formant_transition_label, "FilFormantTransitionLabel");
	GET_LOCAL_AREA(m_ring_mod_amount_label, "FilRingModAmountLabel");

	GET_LOCAL_AREA(m_vel, "FilVel");
	GET_LOCAL_AREA(m_kbd, "FilKbd");
	GET_LOCAL_AREA(m_env, "FilEnv");
	GET_LOCAL_AREA(m_gain, "FilGain");
	GET_LOCAL_AREA(m_freq, "FilFreq");
	GET_LOCAL_AREA(m_res, "FilRes");
	GET_LOCAL_AREA(m_freq_comb, "FilFreqComb");
	GET_LOCAL_AREA(m_res_comb, "FilResComb");
	GET_LOCAL_AREA(m_saturation, "FilSaturation");
	GET_LOCAL_AREA(m_sem_transition, "FilSemTransition");
	GET_LOCAL_AREA(m_formant_transition, "FilFormantTransition");
	GET_LOCAL_AREA(m_ring_mod_amount, "FilRingModAmount");
	GET_LOCAL_AREA(m_vowel_left, "FilVowelLeft");
	GET_LOCAL_AREA(m_vowel_right, "FilVowelRight");
	GET_LOCAL_AREA(m_comb_plus_minus, "FilCombPlusMinus");
}