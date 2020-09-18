
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FormantSelector.h"
#include "../GlobalIncludes.h"
#include "Knob.h"
#include "LeftRightButton.h"
#include "OdinControlAttachments.h"

#define VEL_POS_X 9
#define VEL_POS_Y 8
#define ENV_POS_X 37
#define ENV_POS_Y VEL_POS_Y
#define KBD_POS_X 65
#define KBD_POS_Y VEL_POS_Y
#define GAIN_POS_X 93
#define GAIN_POS_Y VEL_POS_Y
#define FILTER_FREQ_POS_X 120
#define FILTER_FREQ_POS_Y 39
#define RES_POS_X 55
#define RES_POS_Y 71
#define SATURATION_POS_X 174
#define SATURATION_POS_Y 91
#define SEM_FREQ_POS_X 104
#define SEM_FREQ_POS_Y 48
#define SEM_RES_POS_X 49
#define SEM_RES_POS_Y 73
#define COMB_FREQ_POS_X 130
#define COMB_RES_POS_X 61
#define FORMANT_VOW_LEFT_POS_X 44
#define FORMANT_VOW_LEFT_POS_Y 66
#define FORMANT_VOW_RIGHT_POS_X 161
#define FORMANT_VOW_RIGHT_POS_Y FORMANT_VOW_LEFT_POS_Y
#define FORMANT_TRANSITION_X 102
#define FORMANT_TRANSITION_Y 56
#define TRANSITION_POS_X 166
#define TRANSITION_POS_Y (RES_POS_Y + 2)
#define COMB_PLUS_POS_X 152
#define COMB_PLUS_POS_Y 105

#define RINGMOD_AMOUNT_X 148
#define RINGMOD_AMOUNT_Y FORMANT_TRANSITION_Y
#define RINGMOD_FREQ_X 58
#define RINGMOD_FREQ_Y RINGMOD_AMOUNT_Y

#define FREQ_MIN 20
#define FREQ_MAX 20000
#define FREQ_MID 640
#define FREQ_DEFAULT 2000

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
	FilterComponent(AudioProcessorValueTreeState &vts, std::string p_filter_number);
	~FilterComponent();

	void paint(Graphics &) override;

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

	void setGUIBig();
	void setGUISmall();

protected:
	bool m_GUI_big = false;

	Knob m_vel;
	Knob m_kbd;
	Knob m_env;
	DecibelKnob m_gain;
	Knob m_freq;
	Knob m_res;
	Knob m_saturation;
	Knob m_sem_transition;
	Knob m_formant_transition;
	Knob m_ring_mod_amount;

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
	std::unique_ptr<OdinKnobAttachment> m_saturation_attach;
	std::unique_ptr<OdinKnobAttachment> m_formant_transition_attach;
	std::unique_ptr<OdinKnobAttachment> m_ring_mod_amount_attach;
	std::unique_ptr<OdinKnobAttachment> m_sem_transition_attach;

	//std::unique_ptr<OdinButtonAttachment> m_comb_polarity_attach;

	Identifier m_vowel_left_identifier;
	Identifier m_vowel_right_identifier;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterComponent)
};
