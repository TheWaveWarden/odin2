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

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "GlobalIncludes.h"
#include "gui/ADSRComponent.h"
#include "gui/AmpDistortionFlowComponent.h"
#include "gui/ArpComponent.h"
#include "gui/DelayComponent.h"
#include "gui/DragButton.h"
#include "gui/FXButtonsSection.h"
#include "gui/FXComponent.h"
#include "gui/FilterComponent.h"
#include "gui/FixedTextGlassDropdown.h"
#include "gui/InputField.h"
#include "gui/OdinKnob.h"
#include "gui/LFOComponent.h"
#include "gui/LeftRightButton.h"
#include "gui/ModMatrixComponent.h"
#include "gui/NumberSelector.h"
#include "gui/NumberSelectorWithText.h"
#include "gui/OdinButton.h"
#include "gui/OdinTooltipWindow.h"
#include "gui/OscComponent.h"
#include "gui/PatchBrowser.h"
#include "gui/PhaserComponent.h"
#include "gui/PitchWheel.h"
#include "gui/ReverbComponent.h"
#include "gui/TuningComponent.h"
#include "gui/XYSectionComponent.h"
#include "gui/TooltipFeels.h"
#include "gui/OdinFeels.h"

#ifdef WTGEN
#include "SpectrumDisplay.h"
#include "WavetableDisplay.h"
#endif

#define RADIO_GROUP_ARP_MODMATRIX_PRESETS 98765

#define GLIDE_POS_X 117
#define GLIDE_POS_Y 536
#define MASTER_POS_X GLIDE_POS_X
#define MASTER_POS_Y 582

#define OSC1_POS_X 29
#define OSC2_POS_X 277
#define OSC3_POS_X 525
#define OSC_POS_Y 29
#define OSC_SIZE_X 247
#define OSC_SIZE_Y 145

#define FILTER_SIZE_X 247
#define FILTER_SIZE_Y 134
#define FIL1_POS_X 29
#define FIL1_POS_Y 177
#define FIL2_POS_X 525
#define FIL2_POS_Y FIL1_POS_Y
#define FIL3_POS_X OSC2_POS_X
#define FIL3_POS_Y 317

#define DROPDOWN_OSC1_POS_X 245
#define DROPDOWN_OSC2_POS_X 493
#define DROPDOWN_OSC3_POS_X 741
#define DROPDOWN_POS_Y 36

#define DROPDOWN_FILTER1_POS_X 245
#define DROPDOWN_FILTER1_POS_Y 182
#define DROPDOWN_FILTER2_POS_X 741
#define DROPDOWN_FILTER2_POS_Y DROPDOWN_FILTER1_POS_Y
#define DROPDOWN_FILTER3_POS_X DROPDOWN_OSC2_POS_X
#define DROPDOWN_FILTER3_POS_Y 323

#define BUTTON_1_LEFT_POS_X 33
#define BUTTON_1_LEFT_POS_Y 239
#define BUTTON_2_RIGHT_POS_X 742
#define BUTTON_2_RIGHT_POS_Y BUTTON_1_LEFT_POS_Y

#define MIDSECTION_SIZE_X 252
#define MIDSECTION_SIZE_Y 142
#define MIDSECTION_POS_X 275
#define MIDSECTION_POS_Y 175

#define ADSR_SIZE_X 108
#define ADSR_SIZE_Y 96
#define ADSR_LEFT_POS_X 33
#define ADSR_LEFT_POS_Y 330
#define ADSR_RIGHT_POS_X 162
#define ADSR_RIGHT_POS_Y 330

#define FX_BUTTON_X 527
#define FX_BUTTON_Y 310

#define FX_ON_BUTTON_X 528
#define FX_ON_BUTTON_Y 330

#define LEFT_FX_POS_X 526
#define LEFT_FX_POS_Y 343
#define RIGHT_FX_POS_X 746
#define RIGHT_FX_POS_Y LEFT_FX_POS_Y

#define FX_AREA_POS_X 527
#define FX_AREA_POS_Y 343
#define FX_AREA_SIZE_X 243
#define FX_AREA_SIZE_Y 107

#define MATRIX_SIZE_X 500
#define MATRIX_SIZE_Y 144
#define MATRIX_POS_X_100 274
#define MATRIX_POS_Y_100 470

#define MATRIX_POS_X_150 411
#define MATRIX_POS_Y_150 708

#define ARP_MATRIX_BUTTON_POS_X MATRIX_POS_X_100
#define ARP_MATRIX_BUTTON_POS_Y 450
#define SELECT_BUTTON_WIDTH_150 249
#define SELECT_BUTTON_WIDTH 166

#define ARPEGGIATOR_POS_X MATRIX_POS_X_100
#define ARPEGGIATOR_POS_Y 470

#define SAVE_LOAD_POS_X 23
#define SAVE_LOAD_POS_Y 2
#define SAVE_LOAD_SIZE_X 288
#define SAVE_LOAD_SIZE_Y 25

#define LEGATO_POS_X 225
#define LEGATO_POS_Y 8

#define TUNING_POS_X 530
#define TUNING_POS_Y 7

#define UNISON_OFFSET 80
//define UNISON_OFFSET_150 567

#define UNISON_SELECTOR_X 112 - UNISON_OFFSET
#define UNISON_SELECTOR_Y 6
#define UNISON_DETUNE_X 223 - UNISON_OFFSET
#define UNISON_DETUNE_Y 5
#define UNISON_STEREO_X 268 - UNISON_OFFSET
#define UNISON_STEREO_Y UNISON_DETUNE_Y

#define GUI_SIZE_POS_X 674
#define GUI_SIZE_POS_Y 7

#define RESET_SYNTH_POS_X_100 604
#define RESET_SYNTH_POS_Y_100 8
#define RESET_SYNTH_POS_X_150 904
#define RESET_SYNTH_POS_Y_150 12

#define ENV13_POS_X 30
#define ENV13_POS_Y 313
#define ENV24_POS_X 158
#define ENV24_POS_Y ENV13_POS_Y

#define LFO13_POS_X 30
#define LFO13_POS_Y 430
#define LFO24_POS_X 156
#define LFO24_POS_Y LFO13_POS_Y

#define LFO_SIZE_X 116
#define LFO_SIZE_Y 71
#define LFO_RIGHT_POS_X 157
#define LFO_RIGHT_POS_Y 448
#define LFO_LEFT_POS_X 30
#define LFO_LEFT_POS_Y LFO_RIGHT_POS_Y

#define XY_COMPONENT_POS_X 143
#define XY_COMPONENT_POS_Y 516
#define XY_COMPONENT_SIZE_X 127
#define XY_COMPONENT_SIZE_Y 93

#define QUESTION_POS_X 745
#define QUESTION_POS_Y 5

#define PITCHWHEEL_X 42
#define MODWHEEL_X 72
#define WHEEL_Y 511

#define PITCH_AMOUNT_X 35
#define PITCH_AMOUNT_Y 592

#define MASTER_MAX 12
#define MASTER_MIN -30
#define MASTER_DEFAULT -7

#define FONT_SIZE_Y 16
#define FONT_SIZE_X 10
#define FONT_SPACE_Y 4
#define TOOLTIP_INLAY 10

#define BPM_POS_X 500
#define BPM_POS_Y 6

#define MIDI_KEYBOARD_SIZE 85

#define ODIN_EDITOR_SIZE_X 800
#define ODIN_EDITOR_SIZE_Y 614

#define ODIN_EDITOR_SIZE_150_X 1200
#define ODIN_EDITOR_SIZE_150_Y 921

class OdinAudioProcessorEditor : public AudioProcessorEditor, public KeyListener {
public:
	OdinAudioProcessorEditor(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts, bool p_is_standalone);
	~OdinAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics &) override;
	void resized() override;
	void arrangeFXOnButtons(std::map<std::string, int> p_map);
	void setActiveFXPanel(const std::string &name);

	void forceValueTreeOntoComponents(bool p_reset_audio);
	void forceValueTreeOntoComponentsOnlyMainPanel();

	bool keyPressed(const KeyPress &key, Component *) override {
		if (key.getKeyCode() == 120) {
			++m_octave_shift;
			allMidiKeysOff();
		} else if (key == 121) {
			--m_octave_shift;
			allMidiKeysOff();
		}
		return false;
	}
	bool keyStateChanged(bool isKeyDown, Component *originatingComponent) override;
	void allMidiKeysOff();

	InputField m_value_input;
	InputFeels m_input_feels;

	//ColourSelector m_color_picker;

#ifdef WTGEN
	WavetableDisplay m_wavetable_display;
	SpectrumDisplay m_spectrum_display;
#endif

private:
	bool m_A_down = false; // C
	bool m_W_down = false; // C#
	bool m_S_down = false; // D
	bool m_E_down = false; // D#
	bool m_D_down = false; // E
	bool m_F_down = false; // F
	bool m_T_down = false; // F#
	bool m_G_down = false; // G
	bool m_Z_down = false; // G#
	bool m_H_down = false; // A
	bool m_U_down = false; // A#
	bool m_J_down = false; // B
	bool m_K_down = false; // C
	bool m_O_down = false; // C#
	bool m_L_down = false; // D
	bool m_P_down = false; // D#

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	OdinAudioProcessor &processor;

	void setTooltipEnabled(bool p_enabled);
	//==============================================================================
	// Your private member variables go here...
	OdinMenuFeels m_menu_feels;

	OdinKnob m_glide;
	OdinKnob m_master;
	OdinKnob m_modwheel;
	OdinKnob m_unison_detune;
	OdinKnob m_unison_width;

	PitchWheel m_pitchwheel;
	NumberSelector m_pitch_amount;

	NumberSelectorWithText m_unison_selector;

	OscComponent m_osc1;
	OscComponent m_osc2;
	OscComponent m_osc3;

	FilterComponent m_fil1_component;
	FilterComponent m_fil2_component;
	FilterComponent m_fil3_component;

	AmpDistortionFlowComponent m_midsection;

	ADSRComponent m_adsr_1;
	ADSRComponent m_adsr_2;
	ADSRComponent m_adsr_3;
	ADSRComponent m_adsr_4;

	LFOComponent m_lfo_1;
	LFOComponent m_lfo_2;
	LFOComponent m_lfo_3;
	LFOComponent m_lfo_4;

	FXComponent m_flanger;
	PhaserComponent m_phaser;
	FXComponent m_chorus;
	DelayComponent m_delay;
	ReverbComponent m_reverb;

	ModMatrixComponent m_mod_matrix;
	ArpComponent m_arp;
	PatchBrowser m_patch_browser;

	XYSectionComponent m_xy_section;
	TuningComponent m_tuning;

	juce::PopupMenu m_osc_dropdown_menu;
	juce::DrawableButton m_osc1_dropdown;
	juce::DrawableButton m_osc2_dropdown;
	juce::DrawableButton m_osc3_dropdown;

	juce::PopupMenu m_filter_dropdown_menu;
	juce::DrawableButton m_filter1_dropdown;
	juce::DrawableButton m_filter2_dropdown;
	juce::DrawableButton m_filter3_dropdown;

	OdinButton m_filleft_button1;
	OdinButton m_filleft_button2;
	OdinButton m_filleft_button3;

	OdinButton m_filright_button1;
	OdinButton m_filright_button2;
	OdinButton m_filright_button3;
	OdinButton m_filright_buttonf1;

	FXButtonsSection m_fx_buttons_section;

	OdinButton m_flanger_on_button;
	OdinButton m_phaser_on_button;
	OdinButton m_chorus_on_button;
	OdinButton m_delay_on_button;
	OdinButton m_reverb_on_button;

	OdinButton m_select_arp_button;
	OdinButton m_select_modmatrix_button;
	OdinButton m_select_presets_button;

	juce::DrawableButton m_question_button;

	juce::ComponentDragger m_dragger;

	LeftRightButton m_env_13_button;
	LeftRightButton m_env_24_button;

	LeftRightButton m_lfo_13_button;
	LeftRightButton m_lfo_24_button;

	GlassDropdown m_mono_poly_legato_dropdown;
	LeftRightButton m_gui_size_button;

	juce::DrawableButton m_reset;

	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinButtonAttachment> m_phaser_on_attachment;
	std::unique_ptr<OdinButtonAttachment> m_flanger_on_attachment;
	std::unique_ptr<OdinButtonAttachment> m_delay_on_attachment;
	std::unique_ptr<OdinButtonAttachment> m_reverb_on_attachment;
	std::unique_ptr<OdinButtonAttachment> m_chorus_on_attachment;
	std::unique_ptr<OdinButtonAttachment> m_fil1_osc1_attachment;
	std::unique_ptr<OdinButtonAttachment> m_fil1_osc2_attachment;
	std::unique_ptr<OdinButtonAttachment> m_fil1_osc3_attachment;
	std::unique_ptr<OdinButtonAttachment> m_fil2_osc1_attachment;
	std::unique_ptr<OdinButtonAttachment> m_fil2_osc2_attachment;
	std::unique_ptr<OdinButtonAttachment> m_fil2_osc3_attachment;
	std::unique_ptr<OdinButtonAttachment> m_fil2_fil1_attachment;
	std::unique_ptr<OdinButtonAttachment> m_legato_attachment;

	std::unique_ptr<OdinKnobAttachment> m_glide_attachment;
	std::unique_ptr<OdinKnobAttachment> m_master_attachment;
	std::unique_ptr<OdinKnobAttachment> m_modwheel_attachment;
	std::unique_ptr<OdinKnobAttachment> m_pitchbend_attachment;
	std::unique_ptr<OdinKnobAttachment> m_unison_detune_attachment;
	std::unique_ptr<OdinKnobAttachment> m_unison_width_attachment;

	Identifier m_osc1_type_identifier;
	Identifier m_osc2_type_identifier;
	Identifier m_osc3_type_identifier;
	Identifier m_fil1_type_identifier;
	Identifier m_fil2_type_identifier;
	Identifier m_fil3_type_identifier;
	Identifier m_pitchbend_amount_identifier;
	Identifier m_unison_voices_identifier;
	Identifier m_delay_position_identifier;
	Identifier m_phaser_position_identifier;
	Identifier m_flanger_position_identifier;
	Identifier m_chorus_position_identifier;
	Identifier m_reverb_position_identifier;

	OdinTooltipWindow m_tooltip;
	TooltipFeels m_tooltip_feels;

	OdinAudioProcessor &m_processor;

	juce::Image m_odin_backdrop;

	void setOsc1Plate(int p_osc_type);
	void setOsc2Plate(int p_osc_type);
	void setOsc3Plate(int p_osc_type);
	void setFilter1Plate(int p_osc_type);
	void setFilter2Plate(int p_osc_type);
	void setFilter3Plate(int p_osc_type);
	void setEnv13(bool p_env13);
	void setEnv24(bool p_env24);
	void setLfo12(bool p_lfo13);
	void setLfo34(bool p_lfo24);
	//void setArpMod(bool p_arp);
	void setMatrixSectionModule(int p_module);
	void updatePitchWheel(float p_value);
	void updateModWheel(float p_value);
	void setGUISizeBig(bool p_big, bool p_write_to_config);
	void setGUISmall();
	void setGUIBig();

	void readOrCreateConfigFile(bool &p_GUI_big);

	int m_octave_shift = 0;
	bool m_is_standalone_plugin;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OdinAudioProcessorEditor)
};
