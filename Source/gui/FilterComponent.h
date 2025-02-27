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

#pragma once

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "FormantSelector.h"
#include "LeftRightButton.h"
#include "OdinControlAttachments.h"
#include "OdinKnob.h"
#include "TextLabel.h"

#define FREQ_MIN 20
#define FREQ_MAX 20000
#define FREQ_MID 640

#define ENV_MIN -1
#define ENV_MAX 1
#define ENV_DEFAULT 0
#define ENV_SKEW 0.7

#define GAIN_MIN -60
#define GAIN_MAX 12
#define GAIN_DEFAULT 0

#define TRANSITION_MIN -1
#define TRANSITION_MAX 1
#define TRANSITION_DEFAULT -1

//==============================================================================
/*
 */
class FilterComponent : public Component {
public:
	FilterComponent(AudioProcessorValueTreeState &vts, const std::string &p_filter_number);
	~FilterComponent();

	void paint(Graphics &) override;
	void resized() override;

	void setBackground(juce::Image p_background) {
		m_background = p_background;
	}
	void setBackgroundBypass(juce::Image p_background) {
		m_background_bypass = p_background;
	}

	void hideAllComponents();
	int getFilterType() {
		return m_filter_type;
	}
	void setFilterType(int p_filter_type, bool p_force = true);
	void setFilterBypass();
	void setFilterLP24();
	void setFilterLP12();
	void setFilterBP24();
	void setFilterBP12();
	void setFilterHP24();
	void setFilterHP12();
	void setFilterSEM24();
	void setFilterSEM12();
	void setFilterDiode();
	void setFilterKorgLP();
	void setFilterKorgHP();
	void setFilterComb();
	void setFilterFormant();
	void setFilterRingMod();

	void showStandardFilterComponents();
	void showSEMFilterComponents();
	void showCombFilterComponents();
	void showFormantFilterComponents();
	void showRingModFilterComponents();

	void forceValueTreeOntoComponents(ValueTree p_tree, int index);

protected:
	TextLabel m_vel_label;
	TextLabel m_kbd_label;
	TextLabel m_env_label;
	TextLabel m_gain_label;
	TextLabel m_freq_label;
	TextLabel m_res_label;
	TextLabel m_freq_comb_label;
	TextLabel m_res_comb_label;
	TextLabel m_saturation_label;
	TextLabel m_sem_notch_label;
	TextLabel m_sem_lp_label;
	TextLabel m_sem_hp_label;
	TextLabel m_formant_transition_label;
	TextLabel m_ring_mod_amount_label;
	TextLabel m_vowel1_label;
	TextLabel m_vowel2_label;

	OdinKnob m_vel;
	OdinKnob m_kbd;
	OdinKnob m_env;
	DecibelKnob m_gain;
	OdinKnob m_freq;
	OdinKnob m_res;
	OdinKnob m_freq_comb;
	OdinKnob m_res_comb;
	OdinKnob m_saturation;
	OdinKnob m_sem_transition;
	OdinKnob m_formant_transition;
	OdinKnob m_ring_mod_amount;

	FormantSelector m_vowel_left;
	FormantSelector m_vowel_right;

	LeftRightButton m_comb_plus_minus;

	juce::Image m_background;
	juce::Image m_background_bypass;
	int m_filter_type = 0;

	std::string m_filter_number;
	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_vel_attach;
	std::unique_ptr<OdinKnobAttachment> m_kbd_attach;
	std::unique_ptr<OdinKnobAttachment> m_env_attach;
	std::unique_ptr<OdinKnobAttachment> m_gain_attach;
	std::unique_ptr<OdinKnobAttachment> m_freq_attach;
	std::unique_ptr<OdinKnobAttachment> m_res_attach;
	std::unique_ptr<OdinKnobAttachment> m_freq_comb_attach;
	std::unique_ptr<OdinKnobAttachment> m_res_comb_attach;
	std::unique_ptr<OdinKnobAttachment> m_saturation_attach;
	std::unique_ptr<OdinKnobAttachment> m_formant_transition_attach;
	std::unique_ptr<OdinKnobAttachment> m_ring_mod_amount_attach;
	std::unique_ptr<OdinKnobAttachment> m_sem_transition_attach;

	//std::unique_ptr<OdinButtonAttachment> m_comb_polarity_attach;

	Identifier m_vowel_left_identifier;
	Identifier m_vowel_right_identifier;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};
