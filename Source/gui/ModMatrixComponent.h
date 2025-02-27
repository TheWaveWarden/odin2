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
#include "GlassDropdown.h"
#include "ModAmountComponentExtended.h"
#include "OdinButton.h"

#define N_ROWS 9

#define MODMATRIX_COL_POS_1 juce::Colour(46, 136, 174)
#define MODMATRIX_COL_POS_2 juce::Colour(0xff1ff153)
#define MODMATRIX_COL_POS_3 juce::Colour(0xffa0cde7)
#define MODMATRIX_COL_NEG_1 juce::Colour(252, 58, 97)
#define MODMATRIX_COL_NEG_2 juce::Colour(0xffa000ff)
#define MODMATRIX_COL_NEG_3 juce::Colour(0xffe79118)

class ModMatrixComponent : public Component {
public:
	ModMatrixComponent(AudioProcessorValueTreeState &vts);
	~ModMatrixComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void paint(Graphics &) override;
	void resized() override;

private:
	void clearRow(int p_row);

	void createMenu(PopupMenu *p_menu);
	void setStandardMenu(PopupMenu *p_menu);

	GlassDropdown m_source[N_ROWS];
	ModAmountComponentExtended m_amount_1[N_ROWS];
	GlassDropdown m_dest_1[N_ROWS];
	ModAmountComponentExtended m_amount_2[N_ROWS];
	GlassDropdown m_dest_2[N_ROWS];
	ModAmountComponent m_amount_3[N_ROWS];
	GlassDropdown m_scale[N_ROWS];

	Identifier m_source_identifier0;
	Identifier m_source_identifier1;
	Identifier m_source_identifier2;
	Identifier m_source_identifier3;
	Identifier m_source_identifier4;
	Identifier m_source_identifier5;
	Identifier m_source_identifier6;
	Identifier m_source_identifier7;
	Identifier m_source_identifier8;

	Identifier m_dest_1_identifier0;
	Identifier m_dest_1_identifier1;
	Identifier m_dest_1_identifier2;
	Identifier m_dest_1_identifier3;
	Identifier m_dest_1_identifier4;
	Identifier m_dest_1_identifier5;
	Identifier m_dest_1_identifier6;
	Identifier m_dest_1_identifier7;
	Identifier m_dest_1_identifier8;

	Identifier m_dest_2_identifier0;
	Identifier m_dest_2_identifier1;
	Identifier m_dest_2_identifier2;
	Identifier m_dest_2_identifier3;
	Identifier m_dest_2_identifier4;
	Identifier m_dest_2_identifier5;
	Identifier m_dest_2_identifier6;
	Identifier m_dest_2_identifier7;
	Identifier m_dest_2_identifier8;

	Identifier m_scale_identifier0;
	Identifier m_scale_identifier1;
	Identifier m_scale_identifier2;
	Identifier m_scale_identifier3;
	Identifier m_scale_identifier4;
	Identifier m_scale_identifier5;
	Identifier m_scale_identifier6;
	Identifier m_scale_identifier7;
	Identifier m_scale_identifier8;

	Identifier m_amount_1_identifier0;
	Identifier m_amount_2_identifier0;
	Identifier m_amount_3_identifier0;
	Identifier m_amount_1_identifier1;
	Identifier m_amount_2_identifier1;
	Identifier m_amount_3_identifier1;
	Identifier m_amount_1_identifier2;
	Identifier m_amount_2_identifier2;
	Identifier m_amount_3_identifier2;
	Identifier m_amount_1_identifier3;
	Identifier m_amount_2_identifier3;
	Identifier m_amount_3_identifier3;
	Identifier m_amount_1_identifier4;
	Identifier m_amount_2_identifier4;
	Identifier m_amount_3_identifier4;
	Identifier m_amount_1_identifier5;
	Identifier m_amount_2_identifier5;
	Identifier m_amount_3_identifier5;
	Identifier m_amount_1_identifier6;
	Identifier m_amount_2_identifier6;
	Identifier m_amount_3_identifier6;
	Identifier m_amount_1_identifier7;
	Identifier m_amount_2_identifier7;
	Identifier m_amount_3_identifier7;
	Identifier m_amount_1_identifier8;
	Identifier m_amount_2_identifier8;
	Identifier m_amount_3_identifier8;

	OdinButton m_clear_button0;
	OdinButton m_clear_button1;
	OdinButton m_clear_button2;
	OdinButton m_clear_button3;
	OdinButton m_clear_button4;
	OdinButton m_clear_button5;
	OdinButton m_clear_button6;
	OdinButton m_clear_button7;
	OdinButton m_clear_button8;

	AudioProcessorValueTreeState &m_value_tree;

	PopupMenu m_analog_osc_menu[3];
	PopupMenu m_wavetable_osc_menu[3];
	PopupMenu m_multi_osc_menu[3];
	PopupMenu m_vector_osc_menu[3];
	PopupMenu m_fm_osc_menu[3];
	PopupMenu m_pm_osc_menu[3];
	PopupMenu m_noise_osc_menu[3];
	PopupMenu m_draw_osc_menu[3];
	PopupMenu m_chiptune_osc_menu[3];
	PopupMenu m_standard_fil_menu[3];
	PopupMenu m_ringmod_fil_menu[3];
	PopupMenu m_SEM_fil_menu[3];
	PopupMenu m_comb_fil_menu[3];
	PopupMenu m_formant_fil_menu[3];
	PopupMenu m_adsr_menu[4];
	PopupMenu m_lfo_menu[4];
	PopupMenu m_amp_menu;
	PopupMenu m_distortion_menu;
	PopupMenu m_delay_menu;
	PopupMenu m_phaser_menu;
	PopupMenu m_flanger_menu;
	PopupMenu m_chorus_menu;
	PopupMenu m_xy_menu;
	PopupMenu m_arp_menu;

	PopupMenu m_sources_menu;

	int m_panel_height;
	int m_small_panel_width;
	int m_big_panel_width_left;
	int m_big_panel_width_mid;
	int m_big_panel_width_right;

	juce::Colour m_color;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModMatrixComponent)
};
