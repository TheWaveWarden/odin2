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

#include "FilterComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"

FilterComponent::FilterComponent(AudioProcessorValueTreeState &vts, const std::string &p_filter_number) :
    m_value_tree(vts), m_filter_number(p_filter_number), m_vowel_left(false), m_vowel_right(true),
    m_vowel_left_identifier("fil" + p_filter_number + "_vowel_left"),
    m_vowel_right_identifier("fil" + p_filter_number + "_vowel_right"), m_comb_plus_minus("comb_plus_minus") {

	m_vel_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_vel", m_vel));
	m_env_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_env", m_env));
	m_kbd_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_kbd", m_kbd));
	m_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_gain", m_gain));
	m_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_freq", m_freq));
	m_res_attach.reset(new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_res", m_res));
	m_saturation_attach.reset(
	    new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_saturation", m_saturation));
	m_formant_transition_attach.reset(
	    new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_formant_transition", m_formant_transition));
	m_ring_mod_amount_attach.reset(
	    new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_ring_mod_amount", m_ring_mod_amount));
	m_sem_transition_attach.reset(
	    new OdinKnobAttachment(m_value_tree, "fil" + m_filter_number + "_sem_transition", m_sem_transition));

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

	m_res.setSliderStyle(Slider::RotaryVerticalDrag);
	m_res.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_res.setKnobTooltip("Creates a resonant peak at the cutoff "
	                     "frequency.\nFilters will not\n self oscillate");
	addChildComponent(m_res);

	m_saturation.setSliderStyle(Slider::RotaryVerticalDrag);
	m_saturation.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_saturation.setKnobTooltip("Saturates the filter\nand introduces a\nslight distortion");
	addChildComponent(m_saturation);

	m_sem_transition.setSliderStyle(Slider::RotaryVerticalDrag);
	m_sem_transition.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_sem_transition.setTopLeftPosition(TRANSITION_POS_X, TRANSITION_POS_Y);
	m_sem_transition.setRange(TRANSITION_MIN, TRANSITION_MAX);
	m_sem_transition.setNumDecimalPlacesToDisplay(3);
	m_sem_transition.setKnobTooltip("Sets the filter characteristic\nfrom a "
	                                "lowpass over a\nnotch to a highpass");
	addChildComponent(m_sem_transition);

	m_formant_transition.setSliderStyle(Slider::RotaryVerticalDrag);
	m_formant_transition.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_formant_transition.setTopLeftPosition(FORMANT_TRANSITION_X, FORMANT_TRANSITION_Y);
	m_formant_transition.setKnobTooltip("Transitions from the vowel on the\nleft to the one on the right");
	addChildComponent(m_formant_transition);

	m_ring_mod_amount.setSliderStyle(Slider::RotaryVerticalDrag);
	m_ring_mod_amount.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_ring_mod_amount.setTopLeftPosition(RINGMOD_AMOUNT_X, RINGMOD_AMOUNT_Y);
	m_ring_mod_amount.setKnobTooltip("How much ring modulation is applied to the signal");
	addChildComponent(m_ring_mod_amount);

	m_vowel_left.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("misc").setProperty(m_vowel_left_identifier, p_new_value, nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage(m_vowel_left_identifier);
	};
	m_vowel_left.setTopLeftPosition(FORMANT_VOW_LEFT_POS_X, FORMANT_VOW_LEFT_POS_Y);
	m_vowel_left.setTooltip("The vowel to\nthe left side of\nthe transition knob");
	m_vowel_left.setMouseDragDivisor(40.f);
	addChildComponent(m_vowel_left);
	m_vowel_left.setColor(Colour(90, 40, 40));

	m_vowel_right.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("misc").setProperty(m_vowel_right_identifier, p_new_value, nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage(m_vowel_right_identifier);
	};
	m_vowel_right.setTopLeftPosition(FORMANT_VOW_RIGHT_POS_X, FORMANT_VOW_RIGHT_POS_Y);
	m_vowel_right.setTooltip("The vowel to\nthe right side of\nthe transition knob");
	m_vowel_right.setMouseDragDivisor(40.f);
	addChildComponent(m_vowel_right);
	m_vowel_right.setColor(Colour(90, 40, 40));

	m_comb_plus_minus.onClick = [&]() {
		m_value_tree.state.getChildWithName("misc").setProperty(
		    (Identifier)("fil" + m_filter_number + "_comb_polarity"), m_comb_plus_minus.getToggleState(), nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage(
		    (Identifier)("fil" + m_filter_number + "_comb_polarity"));
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
	SET_INTERPOLATION_QUALITY(g)
	g.drawImageAt(m_background, 0, 0);
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
	m_saturation.setVisible(false);
	m_sem_transition.setVisible(false);
	m_formant_transition.setVisible(false);
	m_ring_mod_amount.setVisible(false);
	m_vowel_left.setVisible(false);
	m_vowel_right.setVisible(false);
	m_comb_plus_minus.setVisible(false);
}

void FilterComponent::setFilterBypass() {
	m_background = m_background_bypass;
}

void FilterComponent::setFilterLP24() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::LP24_backdrop_150_png, BinaryData::LP24_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::LP24_backdrop_png, BinaryData::LP24_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterLP12() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::LP12_backdrop_150_png, BinaryData::LP12_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::LP12_backdrop_png, BinaryData::LP12_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterBP24() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::BP24_backdrop_150_png, BinaryData::BP24_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::BP24_backdrop_png, BinaryData::BP24_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterBP12() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::BP12_backdrop_150_png, BinaryData::BP12_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::BP12_backdrop_png, BinaryData::BP12_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterHP24() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::HP24_backdrop_150_png, BinaryData::HP24_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::HP24_backdrop_png, BinaryData::HP24_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterHP12() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::HP12_backdrop_150_png, BinaryData::HP12_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::HP12_backdrop_png, BinaryData::HP12_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterSEM24() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::SEM24_backdrop_150_png, BinaryData::SEM24_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::SEM24_backdrop_png, BinaryData::SEM24_backdrop_pngSize);
	}
	showSEMFilterComponents();
}

void FilterComponent::setFilterSEM12() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::SEM12_backdrop_150_png, BinaryData::SEM12_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::SEM12_backdrop_png, BinaryData::SEM12_backdrop_pngSize);
	}
	showSEMFilterComponents();
}

void FilterComponent::setFilterDiode() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::diode_backdrop_150_png, BinaryData::diode_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::diode_backdrop_png, BinaryData::diode_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterKorgLP() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::korg_backdrop_150_png, BinaryData::korg_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::korg_backdrop_png, BinaryData::korg_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterKorgHP() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::korgHP_backdrop_150_png, BinaryData::korgHP_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::korgHP_backdrop_png, BinaryData::korgHP_backdrop_pngSize);
	}
	showStandardFilterComponents();
}

void FilterComponent::setFilterComb() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::comb_backdrop_150_png, BinaryData::comb_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::comb_backdrop_png, BinaryData::comb_backdrop_pngSize);
	}
	showCombFilterComponents();
}

void FilterComponent::setFilterFormant() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::formant_backdrop_150_png, BinaryData::formant_backdrop_150_pngSize);
	} else {
		m_background =
		    ImageCache::getFromMemory(BinaryData::formant_backdrop_png, BinaryData::formant_backdrop_pngSize);
	}
	showFormantFilterComponents();
}

void FilterComponent::setFilterRingMod() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::ringmod_backdrop_150_png, BinaryData::ringmod_backdrop_150_pngSize);
	} else {
		m_background =
		    ImageCache::getFromMemory(BinaryData::ringmod_backdrop_png, BinaryData::ringmod_backdrop_pngSize);
	}
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

	if (m_GUI_big) {
		m_freq.setTopLeftPosition(OdinHelper::c150(SEM_FREQ_POS_X)-1, OdinHelper::c150(SEM_FREQ_POS_Y)-1);
		m_res.setTopLeftPosition(OdinHelper::c150(SEM_RES_POS_X)-1, OdinHelper::c150(SEM_RES_POS_Y)-1);
	} else {
		m_freq.setTopLeftPosition(SEM_FREQ_POS_X, SEM_FREQ_POS_Y);
		m_res.setTopLeftPosition(SEM_RES_POS_X, SEM_RES_POS_Y);
	}
}

void FilterComponent::showStandardFilterComponents() {
	m_vel.setVisible(true);
	m_kbd.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_freq.setVisible(true);
	m_res.setVisible(true);
	m_saturation.setVisible(true);

	if (m_GUI_big) {
		m_freq.setTopLeftPosition(OdinHelper::c150(FILTER_FREQ_POS_X) - 1, OdinHelper::c150(FILTER_FREQ_POS_Y) - 1);
		m_res.setTopLeftPosition(OdinHelper::c150(RES_POS_X) - 1, OdinHelper::c150(RES_POS_Y) - 1);
		m_saturation.setTopLeftPosition(OdinHelper::c150(SATURATION_POS_X), OdinHelper::c150(SATURATION_POS_Y));
	} else {
		m_freq.setTopLeftPosition(FILTER_FREQ_POS_X, FILTER_FREQ_POS_Y);
		m_res.setTopLeftPosition(RES_POS_X, RES_POS_Y);
		m_saturation.setTopLeftPosition(SATURATION_POS_X, SATURATION_POS_Y);
	}
}

void FilterComponent::showCombFilterComponents() {
	m_vel.setVisible(true);
	m_kbd.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_freq.setVisible(true);
	m_res.setVisible(true);
	m_comb_plus_minus.setVisible(true);

	if (m_GUI_big) {
		m_freq.setTopLeftPosition(OdinHelper::c150(COMB_FREQ_POS_X)-1, OdinHelper::c150(FILTER_FREQ_POS_Y)-1);
		m_res.setTopLeftPosition(OdinHelper::c150(COMB_RES_POS_X)-2, OdinHelper::c150(RES_POS_Y)-1);
	} else {
		m_freq.setTopLeftPosition(COMB_FREQ_POS_X, FILTER_FREQ_POS_Y);
		m_res.setTopLeftPosition(COMB_RES_POS_X, RES_POS_Y);
	}
}

void FilterComponent::showFormantFilterComponents() {
	m_vel.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_vowel_right.setVisible(true);
	m_vowel_left.setVisible(true);
	m_formant_transition.setVisible(true);
}

void FilterComponent::showRingModFilterComponents() {
	m_kbd.setVisible(true);
	m_vel.setVisible(true);
	m_env.setVisible(true);
	m_gain.setVisible(true);
	m_freq.setVisible(true);
	m_ring_mod_amount.setVisible(true);
	if (m_GUI_big) {
		m_freq.setTopLeftPosition(OdinHelper::c150(RINGMOD_FREQ_X)-1, OdinHelper::c150(RINGMOD_FREQ_Y)-1);
	} else {
		m_freq.setTopLeftPosition(RINGMOD_FREQ_X, RINGMOD_FREQ_Y);
	}
}

void FilterComponent::forceValueTreeOntoComponents(ValueTree p_tree, int p_index) {
	m_comb_plus_minus.setValue(
	    m_value_tree.state.getChildWithName("misc")[(Identifier)("fil" + m_filter_number + "_comb_polarity")]);

	m_vowel_left.setValue(m_value_tree.state.getChildWithName("misc")[m_vowel_left_identifier]);
	m_vowel_right.setValue(m_value_tree.state.getChildWithName("misc")[m_vowel_right_identifier]);
}

void FilterComponent::setGUIBig() {
	m_GUI_big = true;

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_150_png, BinaryData::metal_knob_big_150_pngSize);
	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_150_png, BinaryData::metal_knob_mid_150_pngSize);
	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_small_150_png, BinaryData::black_knob_small_150_pngSize);

	m_vel.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_vel.setBounds(OdinHelper::c150(VEL_POS_X),
	                OdinHelper::c150(VEL_POS_Y),
	                black_knob_small.getWidth(),
	                black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_kbd.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_kbd.setBounds(OdinHelper::c150(KBD_POS_X),
	                OdinHelper::c150(KBD_POS_Y),
	                black_knob_small.getWidth(),
	                black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_env.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_env.setBounds(OdinHelper::c150(ENV_POS_X),
	                OdinHelper::c150(ENV_POS_Y),
	                black_knob_small.getWidth(),
	                black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_gain.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_gain.setBounds(OdinHelper::c150(GAIN_POS_X),
	                 OdinHelper::c150(GAIN_POS_Y),
	                 black_knob_small.getWidth(),
	                 black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_freq.setStrip(metal_knob_big, N_KNOB_FRAMES);
	m_freq.setBounds(OdinHelper::c150(FILTER_FREQ_POS_X) - 1,
	                 OdinHelper::c150(FILTER_FREQ_POS_Y) - 1,
	                 metal_knob_big.getWidth(),
	                 metal_knob_big.getHeight() / N_KNOB_FRAMES);

	m_res.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_res.setBounds(OdinHelper::c150(RES_POS_X) - 1,
	                OdinHelper::c150(RES_POS_Y) - 1,
	                metal_knob_mid.getWidth(),
	                metal_knob_mid.getHeight() / N_KNOB_FRAMES);

	m_saturation.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_saturation.setBounds(OdinHelper::c150(SATURATION_POS_X),
	                       OdinHelper::c150(SATURATION_POS_Y),
	                       black_knob_small.getWidth(),
	                       black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_sem_transition.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_sem_transition.setBounds(OdinHelper::c150(TRANSITION_POS_X)-1,
	                           OdinHelper::c150(TRANSITION_POS_Y)-1,
	                           metal_knob_mid.getWidth(),
	                           metal_knob_mid.getHeight() / N_KNOB_FRAMES);

	m_formant_transition.setStrip(metal_knob_big, N_KNOB_FRAMES);
	m_formant_transition.setBounds(OdinHelper::c150(FORMANT_TRANSITION_X)-1,
	                               OdinHelper::c150(FORMANT_TRANSITION_Y)-1,
	                               metal_knob_big.getWidth(),
	                               metal_knob_big.getHeight() / N_KNOB_FRAMES);

	m_ring_mod_amount.setStrip(metal_knob_big, N_KNOB_FRAMES);
	m_ring_mod_amount.setBounds(OdinHelper::c150(RINGMOD_AMOUNT_X)-1,
	                            OdinHelper::c150(RINGMOD_AMOUNT_Y)-1,
	                            metal_knob_big.getWidth(),
	                            metal_knob_big.getHeight() / N_KNOB_FRAMES);

	m_vowel_left.setTopLeftPosition(OdinHelper::c150(FORMANT_VOW_LEFT_POS_X), OdinHelper::c150(FORMANT_VOW_LEFT_POS_Y));
	m_vowel_right.setTopLeftPosition(OdinHelper::c150(FORMANT_VOW_RIGHT_POS_X),
	                                 OdinHelper::c150(FORMANT_VOW_RIGHT_POS_Y));

	juce::Image comb_plus =
	    ImageCache::getFromMemory(BinaryData::buttonplusminus_1_150_png, BinaryData::buttonplusminus_1_150_pngSize);
	juce::Image comb_minus =
	    ImageCache::getFromMemory(BinaryData::buttonplusminus_3_150_png, BinaryData::buttonplusminus_3_150_pngSize);
	m_comb_plus_minus.setImage(comb_plus, 1);
	m_comb_plus_minus.setImage(comb_minus, 2);
	m_comb_plus_minus.setBounds(OdinHelper::c150(COMB_PLUS_POS_X),
	                            OdinHelper::c150(COMB_PLUS_POS_Y),
	                            comb_plus.getWidth(),
	                            comb_plus.getHeight());

	m_vowel_left.setGUIBig();
	m_vowel_right.setGUIBig();

	forceValueTreeOntoComponents(m_value_tree.state, std::stoi(m_filter_number));
}
void FilterComponent::setGUISmall() {
	m_GUI_big = false;

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_png, BinaryData::metal_knob_big_pngSize);
	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_png, BinaryData::metal_knob_mid_pngSize);
	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_small_png, BinaryData::black_knob_small_pngSize);

	m_vel.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_vel.setBounds(VEL_POS_X, VEL_POS_Y, black_knob_small.getWidth(), black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_kbd.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_kbd.setBounds(KBD_POS_X, KBD_POS_Y, black_knob_small.getWidth(), black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_env.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_env.setBounds(ENV_POS_X, ENV_POS_Y, black_knob_small.getWidth(), black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_gain.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_gain.setBounds(GAIN_POS_X, GAIN_POS_Y, black_knob_small.getWidth(), black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_freq.setStrip(metal_knob_big, N_KNOB_FRAMES);
	m_freq.setBounds(
	    FILTER_FREQ_POS_X, FILTER_FREQ_POS_Y, metal_knob_big.getWidth(), metal_knob_big.getHeight() / N_KNOB_FRAMES);

	m_res.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_res.setBounds(RES_POS_X, RES_POS_Y, metal_knob_mid.getWidth(), metal_knob_mid.getHeight() / N_KNOB_FRAMES);

	m_saturation.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_saturation.setBounds(
	    SATURATION_POS_X, SATURATION_POS_Y, black_knob_small.getWidth(), black_knob_small.getHeight() / N_KNOB_FRAMES);

	m_sem_transition.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_sem_transition.setBounds(
	    TRANSITION_POS_X, TRANSITION_POS_Y, metal_knob_mid.getWidth(), metal_knob_mid.getHeight() / N_KNOB_FRAMES);

	m_formant_transition.setStrip(metal_knob_big, N_KNOB_FRAMES);
	m_formant_transition.setBounds(FORMANT_TRANSITION_X,
	                               FORMANT_TRANSITION_Y,
	                               metal_knob_big.getWidth(),
	                               metal_knob_big.getHeight() / N_KNOB_FRAMES);

	m_ring_mod_amount.setStrip(metal_knob_big, N_KNOB_FRAMES);
	m_ring_mod_amount.setBounds(
	    RINGMOD_AMOUNT_X, RINGMOD_AMOUNT_Y, metal_knob_big.getWidth(), metal_knob_big.getHeight() / N_KNOB_FRAMES);

	m_vowel_left.setTopLeftPosition(FORMANT_VOW_LEFT_POS_X, FORMANT_VOW_LEFT_POS_Y);
	m_vowel_right.setTopLeftPosition(FORMANT_VOW_RIGHT_POS_X, FORMANT_VOW_RIGHT_POS_Y);

	juce::Image comb_plus =
	    ImageCache::getFromMemory(BinaryData::buttonplusminus_1_png, BinaryData::buttonplusminus_1_pngSize);
	juce::Image comb_minus =
	    ImageCache::getFromMemory(BinaryData::buttonplusminus_3_png, BinaryData::buttonplusminus_3_pngSize);
	m_comb_plus_minus.setImage(comb_plus, 1);
	m_comb_plus_minus.setImage(comb_minus, 2);
	m_comb_plus_minus.setBounds(COMB_PLUS_POS_X, COMB_PLUS_POS_Y, comb_plus.getWidth(), comb_plus.getHeight());

	m_vowel_left.setGUISmall();
	m_vowel_right.setGUISmall();

	forceValueTreeOntoComponents(m_value_tree.state, std::stoi(m_filter_number));
}