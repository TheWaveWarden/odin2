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
#include "../Utilities.h"
#include "../audio/Oscillators/WavetableContainer.h"
#include "ChipdrawWindow.h"
#include "GlassDropdown.h"
#include "IntegerKnob.h"
#include "LeftRightButton.h"
#include "NumberSelector.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "OdinKnob.h"
#include "SpecdrawDisplay.h"
#include "TextLabel.h"
#include "WavedrawDisplay.h"
#include "WaveformSelectorComponent.h"
#include "XYPadComponent.h"

#define OCT_RANGE_MAX 4
#define SEMI_RANGE_MAX 12
#define FINE_RANGE_MAX 50
#define PW_DEFAULT 0.5
#define STEP_RANGE_MAX 12
#define STEP_2_DEFAULT 12
#define STEP_3_DEFAULT 7

#define SPEED_MAX 60
#define SPEED_MIN 1
#define SPEED_DEFAULT 20
#define SPEED_MID 15

#define VOL_MAX 12
#define VOL_MIN -60
#define VOL_DEFAULT 0

#define HP_MIN 80
#define HP_MAX 18000
#define HP_DEFAULT 80
#define HP_MID 2000

#define LP_MIN 80
#define LP_MAX 18000
#define LP_DEFAULT 18000
#define LP_MID 2000

#define X_POS_X 30
#define X_POS_Y 27
#define Y_POS_X 75
#define Y_POS_Y X_POS_Y

#define FM_COLOR Colour(93, 41, 41)
#define PM_COLOR Colour(30, 30, 30)
//==============================================================================
/*
 */

class OscComponent : public Component {
public:
	OscComponent(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts, const std::string &p_osc_number);
	~OscComponent();

	void paint(Graphics &) override;
	void setBackground(juce::Image p_background) {
		m_background = p_background;
	}
	void setBackgroundBypass(juce::Image p_background) {
		m_background_bypass = p_background;
	}
	int getOscType() {
		return m_osc_type;
	}
	void setOscType(int p_osc_type, bool p_force = true);
	void setOscTypeBypass();
	void setOscTypeAnalog();
	void setOscTypeChiptune();
	void setOscTypeFM();
	void setOscTypePM();
	void setOscTypeChipdraw();
	void setOscTypeWavedraw();
	void setOscTypeSpecdraw();
	void setOscTypeNoise();
	void setOscTypeVector();
	void setOscTypeWavetable();
	void setOscTypeMulti();

	void hideAllComponents();

	void showVolComponent();
	void showPitchComponents();
	void showAnalogComponents();
	void showChiptuneComponents();
	void showFMComponents();
	void showPMComponents();
	void showChipdrawComponents();
	void showWavedrawComponents();
	void showSpecdrawComponents();
	void showNoiseComponents();
	void showVectorComponents();
	void showWavetableComponents();
	void showMultiComponents();

#ifdef WTGEN
	void writeWavedrawTableToFile();
	void writeChipdrawTableToFile();
	void writeSpecdrawTableToFile();
#endif

	void createWavedrawTables();
	void createChipdrawTables();
	void createSpecdrawTables();

	void forceValueTreeOntoComponents(ValueTree p_tree, int p_index, bool p_create_wavetables);

	void resetVectorWaves();

	void setWavetableContainer(WavetableContainer *p_WT_container) {
		m_WT_container = p_WT_container;
	}

	void resized();

protected:
	int m_osc_type = 0;

	juce::Image m_background;
	juce::Image m_background_bypass;

	TextLabel m_osc_label;
	TextLabel m_oct_label;
	TextLabel m_semi_label;
	TextLabel m_fine_label;
	TextLabel m_vol_label;
	TextLabel m_drift_label;
	TextLabel m_pw_label;

	TextLabel m_wt_select_label;
	TextLabel m_wt_mod_label;
	TextLabel m_wt_pos_label;
	TextLabel m_wt_amount_label;

	TextLabel m_detune_spread_label;
	TextLabel m_detune_pos_label;
	TextLabel m_detune_label;
	TextLabel m_detune_wt_label;

	TextLabel m_vec_x_label;
	TextLabel m_vec_y_label;
	TextLabel m_vec_a_label;
	TextLabel m_vec_b_label;
	TextLabel m_vec_c_label;
	TextLabel m_vec_d_label;

	TextLabel m_chip1_label;
	TextLabel m_chip2_label;
	TextLabel m_chip_speed_label;

	TextLabel m_carrier_label;
	TextLabel m_modulator_label;
	TextLabel m_FM_label;
	TextLabel m_PM_label;

	TextLabel m_HP_label;
	TextLabel m_LP_label;

	IntegerKnob m_oct;
	IntegerKnob m_semi;
	OdinKnob m_fine;
	DecibelKnob m_vol;
	OdinKnob m_pw;
	OdinKnob m_drift;
	IntegerKnob m_step_1;
	IntegerKnob m_step_2;
	IntegerKnob m_step_3;
	OdinKnob m_fm;
	OdinKnob m_speed;
	OdinKnob m_lp;
	OdinKnob m_hp;
	OdinKnob m_position;
	OdinKnob m_position_multi;
	OdinKnob m_spread;
	OdinKnob m_detune;
	OdinKnob m_pos_mod;

	OdinButton m_arp;
	OdinButton m_step_button;
	OdinButton m_noise;

	OdinButton m_LED_saw;
	OdinButton m_LED_pulse;
	OdinButton m_LED_triangle;
	OdinButton m_LED_sine;

	OdinButton m_reset;
	OdinButton m_sync;

	WaveformSelectorComponent m_chiptune_waveselector;
	WaveformSelectorComponent m_carrier_waveselector;
	WaveformSelectorComponent m_modulator_waveselector;
	WaveformSelectorComponent m_wavetable_waveselector;
	WaveformSelectorComponent m_modulation_source;

	NumberSelector m_carrier_ratio;
	NumberSelector m_modulator_ratio;

	LeftRightButton m_fm_exp;

	ChipdrawWindow m_chipdraw;
	WavedrawDisplay m_wavedraw;
	SpecdrawDisplay m_specdraw;

	XYPadComponent m_xy;
	OdinKnob m_xy_x;
	OdinKnob m_xy_y;
	GlassDropdown m_vec_a;
	GlassDropdown m_vec_b;
	GlassDropdown m_vec_c;
	GlassDropdown m_vec_d;

	std::string m_osc_number;
	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_oct_attach;
	std::unique_ptr<OdinKnobAttachment> m_semi_attach;
	std::unique_ptr<OdinKnobAttachment> m_fine_attach;
	std::unique_ptr<OdinKnobAttachment> m_vol_attach;
	std::unique_ptr<OdinKnobAttachment> m_position_attach;
	std::unique_ptr<OdinKnobAttachment> m_detune_attach;
	std::unique_ptr<OdinKnobAttachment> m_multi_position_attach;
	std::unique_ptr<OdinKnobAttachment> m_spread_attach;
	std::unique_ptr<OdinKnobAttachment> m_pulsewidth_attach;
	std::unique_ptr<OdinKnobAttachment> m_drift_attach;
	std::unique_ptr<OdinKnobAttachment> m_arp_speed_attach;
	std::unique_ptr<OdinKnobAttachment> m_step_1_attach;
	std::unique_ptr<OdinKnobAttachment> m_step_2_attach;
	std::unique_ptr<OdinKnobAttachment> m_step_3_attach;
	std::unique_ptr<OdinKnobAttachment> m_fm_attach;
	std::unique_ptr<OdinKnobAttachment> m_lp_attach;
	std::unique_ptr<OdinKnobAttachment> m_hp_attach;
	std::unique_ptr<OdinKnobAttachment> m_x_attach;
	std::unique_ptr<OdinKnobAttachment> m_y_attach;
	std::unique_ptr<OdinKnobAttachment> m_pos_mod_attach;

	std::unique_ptr<OdinButtonAttachment> m_reset_attach;
	std::unique_ptr<OdinButtonAttachment> m_sync_attach;
	std::unique_ptr<OdinButtonAttachment> m_arp_on_attach;
	std::unique_ptr<OdinButtonAttachment> m_step_3_on_attach;
	std::unique_ptr<OdinButtonAttachment> m_chipnoise_attach;
	std::unique_ptr<OdinButtonAttachment> m_exp_fm_attach;

	Identifier m_wavetable_identifier;
	Identifier m_modulation_source_identifier;
	Identifier m_chipwave_identifier;
	Identifier m_modulator_wave_identifier;
	Identifier m_carrier_wave_identifier;
	Identifier m_modulator_ratio_identifier;
	Identifier m_carrier_ratio_identifier;
	Identifier m_analog_wave_identifier;
	Identifier m_vec_a_identifier;
	Identifier m_vec_b_identifier;
	Identifier m_vec_c_identifier;
	Identifier m_vec_d_identifier;
	Identifier m_pos_mod_identifier;

	OdinMenuFeels m_menu_feels;

	WavetableContainer *m_WT_container;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent)
};
