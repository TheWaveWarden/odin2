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
#include "gui/LFOComponent.h"
#include "gui/LeftRightButton.h"
#include "gui/LiveConstrainer.h"
#include "gui/ModMatrixComponent.h"
#include "gui/NumberSelector.h"
#include "gui/NumberSelectorWithText.h"
#include "gui/OdinButton.h"
#include "gui/OdinFeels.h"
#include "gui/OdinKnob.h"
#include "gui/OdinTooltipWindow.h"
#include "gui/OscComponent.h"
#include "gui/PatchBrowser.h"
#include "gui/PhaserComponent.h"
#include "gui/PitchWheel.h"
#include "gui/RescaleProgressComponent.h"
#include "gui/ResizeDragger.h"
#include "gui/ReverbComponent.h"
#include "gui/SplineAdComponent.h"
#include "gui/TextLabel.h"
#include "gui/TitleButton.h"
#include "gui/TooltipFeels.h"
#include "gui/TuningComponent.h"
#include "gui/XYSectionComponent.h"

#ifdef WTGEN
#include "SpectrumDisplay.h"
#include "WavetableDisplay.h"
#endif

#define RADIO_GROUP_ARP_MODMATRIX_PRESETS 98765
#define RADIO_GROUP_ENV13 13
#define RADIO_GROUP_ENV24 24
#define RADIO_GROUP_LFO12 12
#define RADIO_GROUP_LFO34 34
#define MASTER_MIN -50.0f
#define MASTER_MAX 12.0f

class OdinEditor : public AudioProcessorEditor, public KeyListener {
public:
	OdinEditor(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts, bool p_is_standalone);
	~OdinEditor();

	//==============================================================================
	void paint(Graphics &) override;
	void paintOverChildren(juce::Graphics &g) override;

	void resized() override;
	void arrangeFXOnButtons(std::map<std::string, int> p_map);
	void setActiveFXPanel(const std::string &name);

	void forceValueTreeOntoComponents(bool p_reset_audio = false);
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

	RescaleProgressComponent &getRescaleOverlayComponent() {
		return m_rescale_component;
	}

	void setGuiSize(GuiScale p_size, bool p_set_config);

	InputField m_value_input;
	InputFeels m_input_feels;

	//ColourSelector m_color_picker;

#ifdef WTGEN
	WavetableDisplay m_wavetable_display;
	SpectrumDisplay m_spectrum_display;
#endif

private:
	void paintGrid(Graphics &g);

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

	LiveConstrainer m_live_constrainer;

	void setGuiScale(int scale);
	void setTooltipEnabled(bool p_enabled);

	//==============================================================================
	// Your private member variables go here...
	OdinMenuFeels m_menu_feels;

	OdinKnob m_glide;
	OdinKnob m_master;
	OdinKnob m_modwheel;
	OdinKnob m_unison_detune;
	OdinKnob m_unison_width;

	TextLabel m_detune_label;
	TextLabel m_width_label;
	TextLabel m_master_label;
	TextLabel m_glide_label;

	PitchWheel m_pitchwheel;
	NumberSelector m_pitch_amount;

	NumberSelectorWithText m_unison_selector;

	OscComponent m_osc1;
	OscComponent m_osc2;
	OscComponent m_osc3;

	SplineAdComponent m_spline_ad;

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
	OdinButton m_osc1_dropdown;
	OdinButton m_osc2_dropdown;
	OdinButton m_osc3_dropdown;

	juce::PopupMenu m_filter_dropdown_menu;
	OdinButton m_filter1_dropdown;
	OdinButton m_filter2_dropdown;
	OdinButton m_filter3_dropdown;

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

	TitleButton m_title_button;

	OdinButton m_burger_button;

	juce::ComponentDragger m_dragger;

	OdinButton m_env_1_button;
	OdinButton m_env_2_button;
	OdinButton m_env_3_button;
	OdinButton m_env_4_button;

	OdinButton m_lfo_1_button;
	OdinButton m_lfo_2_button;
	OdinButton m_lfo_3_button;
	OdinButton m_lfo_4_button;

	GlassDropdown m_mono_poly_legato_dropdown;
	RescaleProgressComponent m_rescale_component;

	OdinButton m_reset;

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
	void setFXModulesEnablements();

	void readOrCreateConfigFile(bool &p_GUI_big);

	int m_octave_shift = 0;
	bool m_is_standalone_plugin;

	JUCE_DECLARE_WEAK_REFERENCEABLE(OdinEditor)
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OdinEditor)
};
