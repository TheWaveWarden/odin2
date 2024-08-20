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

#include "OscComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "JsonGuiProvider.h"

OscComponent::OscComponent(OdinAudioProcessor &p_processor,
                           AudioProcessorValueTreeState &vts,
                           const std::string &p_osc_number) :
    m_value_tree(vts),
    m_reset("reset_button", "Reset"), m_sync("osc_sync_button", "Sync"), m_LED_saw("LED_Saw", "Saw"),
    m_LED_pulse("LED_pulse", "PWM"), m_LED_triangle("LED_triangle", "Tri"), m_LED_sine("LED_sine", "Sine"),
    m_arp("arp", "Arp"), m_step_button("step", "Step"), m_noise("noise", "Noise"),
    m_chipdraw_convert("convert_chipdraw", "ok"), m_wavedraw_convert("convert_wavedraw", "ok"),
    m_specdraw_convert("convert_wavedraw", "ok"), m_chiptune_waveselector(true), m_carrier_waveselector(false),
    m_modulator_waveselector(true), m_wavetable_waveselector(true), m_modulation_source(true), m_carrier_ratio(false),
    m_modulator_ratio(true), m_fm_exp("fm_exp"), m_xy(vts, "osc" + p_osc_number + "_vec_", m_xy_x, m_xy_y, true),
    m_osc_number(p_osc_number), m_wavetable_identifier("osc" + p_osc_number + "_wavetable"),
    m_modulation_source_identifier("osc" + p_osc_number + "_mod_source"),
    m_chipwave_identifier("osc" + p_osc_number + "_chipwave"),
    m_modulator_wave_identifier("osc" + p_osc_number + "_modulator_wave"),
    m_carrier_wave_identifier("osc" + p_osc_number + "_carrier_wave"),
    m_modulator_ratio_identifier("osc" + p_osc_number + "_modulator_ratio"),
    m_carrier_ratio_identifier("osc" + p_osc_number + "_carrier_ratio"),
    m_analog_wave_identifier("osc" + p_osc_number + "_analog_wave"),
    m_vec_a_identifier("osc" + p_osc_number + "_vec_a"), m_vec_b_identifier("osc" + p_osc_number + "_vec_b"),
    m_vec_c_identifier("osc" + p_osc_number + "_vec_c"), m_vec_d_identifier("osc" + p_osc_number + "_vec_d"),
    m_pos_mod_identifier("osc" + p_osc_number + "_pos_mod"), m_osc_label("AnalogOsc"), m_oct_label("Oct"),
    m_semi_label("Semi"), m_fine_label("Fine"), m_vol_label("Vol"), m_drift_label("Drift"), m_pw_label("PW") {

	m_WT_container = p_processor.getWavetableContainerPointer();

	m_oct_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_oct", m_oct));
	m_semi_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_semi", m_semi));
	m_fine_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_fine", m_fine));
	m_vol_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_vol", m_vol));
	m_position_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_position", m_position));
	m_detune_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_detune", m_detune));
	m_pos_mod_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_pos_mod", m_pos_mod));
	m_multi_position_attach.reset(
	    new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_multi_position", m_position_multi));
	m_spread_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_spread", m_spread));
	m_pulsewidth_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_pulsewidth", m_pw));
	m_drift_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_drift", m_drift));
	m_arp_speed_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_arp_speed", m_speed));
	m_step_1_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_step_1", m_step_1));
	m_step_2_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_step_2", m_step_2));
	m_step_3_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_step_3", m_step_3));
	m_fm_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_fm", m_fm));
	m_lp_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_lp", m_lp));
	m_hp_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_hp", m_hp));
	m_x_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_vec_x", m_xy_x));
	m_y_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_vec_y", m_xy_y));

	m_reset_attach.reset(new OdinButtonAttachment(m_value_tree, "osc" + m_osc_number + "_reset", m_reset));
	if (std::stoi(m_osc_number) != 1) {
		m_sync_attach.reset(new OdinButtonAttachment(m_value_tree, "osc" + m_osc_number + "_sync", m_sync));
	}
	m_arp_on_attach.reset(new OdinButtonAttachment(m_value_tree, "osc" + m_osc_number + "_arp_on", m_arp));
	m_step_3_on_attach.reset(
	    new OdinButtonAttachment(m_value_tree, "osc" + m_osc_number + "_step_3_on", m_step_button));
	m_chipnoise_attach.reset(new OdinButtonAttachment(m_value_tree, "osc" + m_osc_number + "_chipnoise", m_noise));
	m_exp_fm_attach.reset(new OdinButtonAttachment(m_value_tree, "osc" + m_osc_number + "_exp_fm", m_fm_exp));

	m_vol.setSliderStyle(Slider::RotaryVerticalDrag);
	m_vol.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_vol.setRange(VOL_MIN, VOL_MAX);
	m_vol.setTextValueSuffix(" dB");
	m_vol.setNumDecimalPlacesToDisplay(1);
	m_vol.setKnobTooltip("The volume of the oscillator");
	addChildComponent(m_vol);

	m_reset.setClickingTogglesState(true);
	m_reset.setTooltip("Resets the oscillator to\nthe start of its waveform\nfor a new note");
	addChildComponent(m_reset);
	m_reset.setAlwaysOnTop(true);
	m_reset.setTriggeredOnMouseDown(true);
	m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	// sync button only for osc 2 & 3
	if (std::stoi(m_osc_number) != 1) {
		m_sync.setClickingTogglesState(true);
		m_sync.setTooltip("Syncs this osc to osc1, so it will start over when osc1 "
		                  "completes a cycle.");
		addChildComponent(m_sync);
		m_sync.setAlwaysOnTop(true);
		m_sync.setTriggeredOnMouseDown(true);
		m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	}

	m_oct.setSliderStyle(Slider::RotaryVerticalDrag);
	m_oct.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_oct.setRange(-OCT_RANGE_MAX, OCT_RANGE_MAX);
	m_oct.setNumDecimalPlacesToDisplay(0);
	m_oct.setKnobTooltip("The pitch of\nthe oscillator in octaves");
	addChildComponent(m_oct);

	m_semi.setSliderStyle(Slider::RotaryVerticalDrag);
	m_semi.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_semi.setRange(-SEMI_RANGE_MAX, SEMI_RANGE_MAX);
	m_semi.setNumDecimalPlacesToDisplay(0);
	m_semi.setKnobTooltip("The pitch of\nthe oscillator in semitones");
	addChildComponent(m_semi);

	m_fine.setSliderStyle(Slider::RotaryVerticalDrag);
	m_fine.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_fine.setKnobTooltip("The pitch of\nthe oscillator in cents");
	addChildComponent(m_fine);

	m_LED_saw.setClickingTogglesState(true);
	m_LED_saw.setRadioGroupId(23456);
	m_LED_saw.setTriggeredOnMouseDown(false);
	m_LED_saw.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_LED_saw.setTooltip("Select the saw wave");
	m_LED_saw.onStateChange = [&]() {
		if (m_LED_saw.getToggleState()) {
			m_value_tree.state.getChildWithName("osc").setProperty(m_analog_wave_identifier, 0, nullptr);
		}
	};
	addChildComponent(m_LED_saw);

	m_LED_pulse.setClickingTogglesState(true);
	m_LED_pulse.setRadioGroupId(23456);
	m_LED_pulse.setTriggeredOnMouseDown(false);
	m_LED_pulse.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_LED_pulse.setTooltip("Select the pulse wave");
	m_LED_pulse.onStateChange = [&]() {
		if (m_LED_pulse.getToggleState()) {
			m_value_tree.state.getChildWithName("osc").setProperty(m_analog_wave_identifier, 1, nullptr);
		}
	};
	addChildComponent(m_LED_pulse);

	m_LED_triangle.setClickingTogglesState(true);
	m_LED_triangle.setRadioGroupId(23456);
	m_LED_triangle.setTriggeredOnMouseDown(false);
	m_LED_triangle.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_LED_triangle.setTooltip("Select the triangle wave");
	m_LED_triangle.onStateChange = [&]() {
		if (m_LED_triangle.getToggleState()) {
			m_value_tree.state.getChildWithName("osc").setProperty(m_analog_wave_identifier, 2, nullptr);
		}
	};
	addChildComponent(m_LED_triangle);

	m_LED_sine.setClickingTogglesState(true);
	m_LED_sine.setRadioGroupId(23456);
	m_LED_sine.setTriggeredOnMouseDown(false);
	m_LED_sine.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_LED_sine.setTooltip("Select the sine wave");
	m_LED_sine.onStateChange = [&]() {
		if (m_LED_sine.getToggleState()) {
			m_value_tree.state.getChildWithName("osc").setProperty(m_analog_wave_identifier, 3, nullptr);
		}
	};
	addChildComponent(m_LED_sine);

	m_pw.setSliderStyle(Slider::RotaryVerticalDrag);
	m_pw.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_pw.setKnobTooltip("The pulse width if\nthe pulse wave is selected");
	addChildComponent(m_pw);

	m_drift.setSliderStyle(Slider::RotaryVerticalDrag);
	m_drift.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_drift.setKnobTooltip("Slightly detunes the oscillator\nover time, like an "
	                       "analog\noscillator would");
	addChildComponent(m_drift);

	m_arp.setClickingTogglesState(true);
	addChildComponent(m_arp);
	m_arp.setTooltip("Enables the arpeggiator");
	m_arp.setAlwaysOnTop(true);
	m_arp.setTriggeredOnMouseDown(true);
	m_arp.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_noise.setClickingTogglesState(true);
	m_noise.setTooltip("Switches the oscillator\nto 4Bit noise mode");
	addChildComponent(m_noise);
	m_noise.setAlwaysOnTop(true);
	m_noise.setTriggeredOnMouseDown(true);
	m_noise.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_step_button.setClickingTogglesState(true);
	addChildComponent(m_step_button);
	m_step_button.setTooltip("Enables step 3 in the arpeggiator");
	m_step_button.setAlwaysOnTop(true);
	m_step_button.setTriggeredOnMouseDown(true);
	m_step_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_step_1.setSliderStyle(Slider::RotaryVerticalDrag);
	m_step_1.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_step_1.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
	m_step_1.setNumDecimalPlacesToDisplay(0);
	m_step_1.setKnobTooltip("The pitch of the\nfirst step of the\narpeggiator in semitones");
	addChildComponent(m_step_1);

	m_step_2.setSliderStyle(Slider::RotaryVerticalDrag);
	m_step_2.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_step_2.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
	m_step_2.setNumDecimalPlacesToDisplay(0);
	m_step_2.setKnobTooltip("The pitch of the\nsecond step of the\narpeggiator in semitones");
	addChildComponent(m_step_2);

	m_step_3.setSliderStyle(Slider::RotaryVerticalDrag);
	m_step_3.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_step_3.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
	m_step_3.setNumDecimalPlacesToDisplay(0);
	m_step_3.setKnobTooltip("The pitch of the\nthird step of the\narpeggiator in semitones");
	addChildComponent(m_step_3);

	m_fm.setSliderStyle(Slider::RotaryVerticalDrag);
	m_fm.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_fm.setKnobTooltip("How much the modulator modulates the pitch (FM) or the "
	                    "phase (PM) of the carrier wave");
	addChildComponent(m_fm);

	m_lp.setSliderStyle(Slider::RotaryVerticalDrag);
	m_lp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_lp.setTextValueSuffix(" Hz");
	m_lp.setNumDecimalPlacesToDisplay(0);
	m_lp.setKnobTooltip("The frequency of\nthe lowpass filter which\nis applied to the noise");
	addChildComponent(m_lp);

	m_hp.setSliderStyle(Slider::RotaryVerticalDrag);
	m_hp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_hp.setTextValueSuffix(" Hz");
	m_hp.setNumDecimalPlacesToDisplay(0);
	m_hp.setKnobTooltip("The frequency of\nthe highpass filter which\nis applied to the noise");
	addChildComponent(m_hp);

	m_position.setSliderStyle(Slider::RotaryVerticalDrag);
	m_position.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_position.setKnobTooltip("The position in the wavetable used. There "
	                          "are four tables through which you can sweep");
	addChildComponent(m_position);

	m_detune.setSliderStyle(Slider::RotaryVerticalDrag);
	m_detune.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_detune.setKnobTooltip("How much the individual\noscillators are detuned\n against each other");
	addChildComponent(m_detune);

	m_pos_mod.setSliderStyle(Slider::RotaryVerticalDrag);
	m_pos_mod.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_pos_mod.setKnobTooltip("Controls the depth of modulation from the selected modsource to the osc position");
	addChildComponent(m_pos_mod);

	m_spread.setSliderStyle(Slider::RotaryVerticalDrag);
	m_spread.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_spread.setKnobTooltip("Spreads the sub-oscillators to different positions in the wavetable");
	addChildComponent(m_spread);

	m_position_multi.setSliderStyle(Slider::RotaryVerticalDrag);
	m_position_multi.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_position_multi.setKnobTooltip("The position in the\nwavetable used. There are\nfour tables through "
	                                "which\nyou can sweep");
	addChildComponent(m_position_multi);

	m_speed.setSliderStyle(Slider::RotaryVerticalDrag);
	m_speed.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_speed.setTextValueSuffix(" Hz");
	m_speed.setNumDecimalPlacesToDisplay(1);
	m_speed.setKnobTooltip("Speed of the arpeggiator");
	addChildComponent(m_speed);

	m_chipdraw_convert.setAlwaysOnTop(true);
	m_chipdraw_convert.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_chipdraw_convert.setToggleState(true, sendNotification);
	m_chipdraw_convert.disableMidiLearn();
	m_chipdraw_convert.setClickingTogglesState(true);
	m_chipdraw_convert.setTriggeredOnMouseDown(false);
	m_chipdraw_convert.onClick = [&]() {
		if (m_chipdraw_convert.getToggleState()) {
			createChipdrawTables();
		}
		m_chipdraw_convert.setToggleState(true, dontSendNotification);
	};

	m_chipdraw_convert.setTooltip("Converts the waveform drawn\nin the window. You won't hear\nany changes "
	                              "before you press\nthis button");
	addChildComponent(m_chipdraw_convert);

	m_wavedraw_convert.setClickingTogglesState(true);
	m_wavedraw_convert.setAlwaysOnTop(true);
	m_wavedraw_convert.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_wavedraw_convert.setToggleState(true, sendNotification);
	m_wavedraw_convert.setClickingTogglesState(true);
	m_wavedraw_convert.setTriggeredOnMouseDown(false);
	m_wavedraw_convert.onClick = [&]() {
		if (m_wavedraw_convert.getToggleState()) {
			createWavedrawTables();
		}
		m_wavedraw_convert.setToggleState(true, dontSendNotification);
	};
	m_wavedraw_convert.setTooltip("Converts the waveform drawn\nin the window. You won't hear\nany changes "
	                              "before you press\nthis button");
	addChildComponent(m_wavedraw_convert);
	m_wavedraw_convert.disableMidiLearn();

	m_specdraw_convert.setClickingTogglesState(true);
	m_specdraw_convert.setAlwaysOnTop(true);
	m_specdraw_convert.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_specdraw_convert.setToggleState(true, sendNotification);
	m_specdraw_convert.setClickingTogglesState(true);
	m_specdraw_convert.setTriggeredOnMouseDown(false);
	m_specdraw_convert.onClick = [&]() {
		if (m_specdraw_convert.getToggleState()) {
			createSpecdrawTables();
		}
		m_specdraw_convert.setToggleState(true, dontSendNotification);
	};
	m_specdraw_convert.setTooltip("Converts the waveform drawn\nin the window. You won't hear\nany changes "
	                              "before you press\nthis button");
	addChildComponent(m_specdraw_convert);
	m_specdraw_convert.disableMidiLearn();

	juce::Colour chip_color(93, 81, 63);

	m_chiptune_waveselector.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("osc").setProperty(m_chipwave_identifier, (float)p_new_value, nullptr);
	};
	m_chiptune_waveselector.setTopLeftPosition(WAVE_CHIPTUNE_POS_X, WAVE_CHIPTUNE_POS_Y);
	m_chiptune_waveselector.setColor(chip_color);
	m_chiptune_waveselector.setTooltip("Selects the wave for the oscillator");
	addChildComponent(m_chiptune_waveselector);

	juce::PopupMenu fm_menu;
	juce::PopupMenu chip_menu;
	juce::PopupMenu vector_menu;
	juce::PopupMenu wavetable_submenu;
	juce::PopupMenu wavetable_recycle_menu;

#define ADD_WAVETABLE_SUB_MENU(name, number)                                                                           \
	wavetable_recycle_menu.clear();                                                                                    \
	wavetable_recycle_menu.addItem(number + 1, (std::string)name + (std::string) " 1");                                \
	wavetable_recycle_menu.addItem(number + 2, (std::string)name + (std::string) " 2");                                \
	wavetable_recycle_menu.addItem(number + 3, (std::string)name + (std::string) " 3");                                \
	wavetable_recycle_menu.addItem(number + 4, (std::string)name + (std::string) " 4");                                \
	wavetable_submenu.addSubMenu(name, wavetable_recycle_menu);

#define ADD_MAP_ENTRY(name, number, selector)                                                                          \
	selector.addMapEntry((std::string)name + (std::string) " 1", number + 1, number + 2, number - 6);                  \
	selector.addMapEntry((std::string)name + (std::string) " 2", number + 2, number + 3, number + 1);                  \
	selector.addMapEntry((std::string)name + (std::string) " 3", number + 3, number + 4, number + 2);                  \
	selector.addMapEntry((std::string)name + (std::string) " 4", number + 4, number + 11, number + 3);

	juce::PopupMenu chiptune_submenu;
	chiptune_submenu.addItem(501, "Square 50");
	chiptune_submenu.addItem(502, "Square 25");
	chiptune_submenu.addItem(503, "Square 12.5");
	chiptune_submenu.addItem(504, "ChipTriangle");

	chiptune_submenu.addItem(505, "ChipSaw");
	chiptune_submenu.addItem(506, "ChipSine");

	juce::PopupMenu wavedraw_submenu;
	wavedraw_submenu.addItem(601, "WaveDraw Osc1");
	wavedraw_submenu.addItem(602, "WaveDraw Osc2");
	wavedraw_submenu.addItem(603, "WaveDraw Osc3");
	juce::PopupMenu chipdraw_submenu;
	chipdraw_submenu.addItem(701, "ChipDraw Osc1");
	chipdraw_submenu.addItem(702, "ChipDraw Osc2");
	chipdraw_submenu.addItem(703, "ChipDraw Osc3");
	juce::PopupMenu specdraw_submenu;
	specdraw_submenu.addItem(801, "SpecDraw Osc1");
	specdraw_submenu.addItem(802, "SpecDraw Osc2");
	specdraw_submenu.addItem(803, "SpecDraw Osc3");

	// now add all menus
	wavetable_recycle_menu.addItem(101, "Saw");
	wavetable_recycle_menu.addItem(102, "Square");
	wavetable_recycle_menu.addItem(103, "Triangle");
	wavetable_recycle_menu.addItem(104, "Sine");

	wavetable_submenu.addSubMenu("Classic Analog", wavetable_recycle_menu);

	ADD_WAVETABLE_SUB_MENU("Nasal", 120);
	ADD_WAVETABLE_SUB_MENU("Reed", 130);
	ADD_WAVETABLE_SUB_MENU("Glass", 140);
	ADD_WAVETABLE_SUB_MENU("FM", 150);
	ADD_WAVETABLE_SUB_MENU("BrokenSine", 160);
	ADD_WAVETABLE_SUB_MENU("Harsh", 170);
	ADD_WAVETABLE_SUB_MENU("Mean", 180);
	ADD_WAVETABLE_SUB_MENU("Rectangular", 190);
	ADD_WAVETABLE_SUB_MENU("Bitreduced", 200);
	ADD_WAVETABLE_SUB_MENU("Strings", 210);
	ADD_WAVETABLE_SUB_MENU("Piano", 220);
	ADD_WAVETABLE_SUB_MENU("Organ", 230);
	ADD_WAVETABLE_SUB_MENU("Oboe", 240);
	ADD_WAVETABLE_SUB_MENU("Trumpet", 250);
	ADD_WAVETABLE_SUB_MENU("MixedWaves A", 260);
	ADD_WAVETABLE_SUB_MENU("MixedWaves B", 270);
	ADD_WAVETABLE_SUB_MENU("MixedWaves C", 280);
	ADD_WAVETABLE_SUB_MENU("MixedWaves D", 290);
	ADD_WAVETABLE_SUB_MENU("Voice A", 300);
	ADD_WAVETABLE_SUB_MENU("Voice B", 310);
	ADD_WAVETABLE_SUB_MENU("Voice C", 320);
	ADD_WAVETABLE_SUB_MENU("Voice D", 330);
	ADD_WAVETABLE_SUB_MENU("Additive A", 340);
	ADD_WAVETABLE_SUB_MENU("Additive B", 350);
	ADD_WAVETABLE_SUB_MENU("Additive C", 360);
	ADD_WAVETABLE_SUB_MENU("Additive D", 370);
	ADD_WAVETABLE_SUB_MENU("Overtones A", 380);
	ADD_WAVETABLE_SUB_MENU("Overtones B", 390);
	ADD_WAVETABLE_SUB_MENU("Harmonics A", 400);
	ADD_WAVETABLE_SUB_MENU("Harmonics B", 410);
	ADD_WAVETABLE_SUB_MENU("MutantSaw A", 420);
	ADD_WAVETABLE_SUB_MENU("MutantSaw B", 430);
	ADD_WAVETABLE_SUB_MENU("MutantSquare A", 440);
	ADD_WAVETABLE_SUB_MENU("MutantSquare B", 450);

	fm_menu.addItem(1, "Sine");
	fm_menu.addSeparator();
	fm_menu.addSubMenu("Wavetables", wavetable_submenu);
	fm_menu.addSeparator();
	fm_menu.addSubMenu("Chiptune", chiptune_submenu);
	fm_menu.addSeparator();
	fm_menu.addSubMenu("WaveDraw", wavedraw_submenu);
	fm_menu.addSubMenu("ChipDraw", chipdraw_submenu);
	fm_menu.addSubMenu("SpecDraw", specdraw_submenu);

	fm_menu.setLookAndFeel(&m_menu_feels);

	vector_menu.addSubMenu("Wavetables", wavetable_submenu);
	vector_menu.addSeparator();
	vector_menu.addSubMenu("Chiptune", chiptune_submenu);
	vector_menu.addSeparator();
	vector_menu.addSubMenu("WaveDraw", wavedraw_submenu);
	vector_menu.addSubMenu("ChipDraw", chipdraw_submenu);
	vector_menu.addSubMenu("SpecDraw", specdraw_submenu);

	chip_menu.addItem(1, "Pulse 50");
	chip_menu.addItem(2, "Pulse 25");
	chip_menu.addItem(3, "Pulse 12.5");
	chip_menu.addItem(4, "ChipTriangle");
	chip_menu.addItem(5, "ChipSaw");
	chip_menu.addItem(6, "ChipSine");
	chip_menu.addSeparator();
	chip_menu.addItem(701, "ChipDraw Osc1");
	chip_menu.addItem(702, "ChipDraw Osc2");
	chip_menu.addItem(703, "ChipDraw Osc3");

	//====================
	//=== COPY & PASTA ===
	//====================

	ADD_MAP_ENTRY("Nasal", 120, m_carrier_waveselector)
	ADD_MAP_ENTRY("Reed", 130, m_carrier_waveselector)
	ADD_MAP_ENTRY("Glass", 140, m_carrier_waveselector)
	ADD_MAP_ENTRY("FM", 150, m_carrier_waveselector)
	ADD_MAP_ENTRY("BrokenSine", 160, m_carrier_waveselector)
	ADD_MAP_ENTRY("Harsh", 170, m_carrier_waveselector)
	ADD_MAP_ENTRY("Mean", 180, m_carrier_waveselector)
	ADD_MAP_ENTRY("Rectangular", 190, m_carrier_waveselector)
	ADD_MAP_ENTRY("Bitreduced", 200, m_carrier_waveselector)
	ADD_MAP_ENTRY("Strings", 210, m_carrier_waveselector)
	ADD_MAP_ENTRY("Piano", 220, m_carrier_waveselector)
	ADD_MAP_ENTRY("Organ", 230, m_carrier_waveselector)
	ADD_MAP_ENTRY("Oboe", 240, m_carrier_waveselector)
	ADD_MAP_ENTRY("Trumpet", 250, m_carrier_waveselector)
	ADD_MAP_ENTRY("MixedWaves A", 260, m_carrier_waveselector)
	ADD_MAP_ENTRY("MixedWaves B", 270, m_carrier_waveselector)
	ADD_MAP_ENTRY("MixedWaves C", 280, m_carrier_waveselector)
	ADD_MAP_ENTRY("MixedWaves D", 290, m_carrier_waveselector)
	ADD_MAP_ENTRY("Voice A", 300, m_carrier_waveselector)
	ADD_MAP_ENTRY("Voice B", 310, m_carrier_waveselector)
	ADD_MAP_ENTRY("Voice C", 320, m_carrier_waveselector)
	ADD_MAP_ENTRY("Voice D", 330, m_carrier_waveselector)
	ADD_MAP_ENTRY("Additive A", 340, m_carrier_waveselector)
	ADD_MAP_ENTRY("Additive B", 350, m_carrier_waveselector)
	ADD_MAP_ENTRY("Additive C", 360, m_carrier_waveselector)
	ADD_MAP_ENTRY("Additive D", 370, m_carrier_waveselector)
	ADD_MAP_ENTRY("Overtones A", 380, m_carrier_waveselector)
	ADD_MAP_ENTRY("Overtones B", 390, m_carrier_waveselector)
	ADD_MAP_ENTRY("Harmonics A", 400, m_carrier_waveselector)
	ADD_MAP_ENTRY("Harmonics B", 410, m_carrier_waveselector)
	ADD_MAP_ENTRY("MutantSaw A", 420, m_carrier_waveselector)
	ADD_MAP_ENTRY("MutantSaw B", 430, m_carrier_waveselector)
	ADD_MAP_ENTRY("MutantSquare A", 440, m_carrier_waveselector)
	ADD_MAP_ENTRY("MutantSquare B", 450, m_carrier_waveselector)

	m_carrier_waveselector.setDecrementValue(121, 104);
	m_carrier_waveselector.setIncrementValue(454, 501);
	m_carrier_waveselector.addMapEntry("Sine", 1, 101, 1);
	m_carrier_waveselector.addMapEntry("Saw", 101, 102, 1);
	m_carrier_waveselector.addMapEntry("Square", 102, 103, 101);
	m_carrier_waveselector.addMapEntry("Triangle", 103, 104, 102);
	m_carrier_waveselector.addMapEntry("Sine", 104, 121, 103);

	m_carrier_waveselector.addMapEntry("Pulse 50", 501, 502, 454);
	m_carrier_waveselector.addMapEntry("Pulse 25", 502, 503, 501);
	m_carrier_waveselector.addMapEntry("Pulse 12.5", 503, 504, 502);
	m_carrier_waveselector.addMapEntry("ChipTriangle", 504, 505, 503);
	m_carrier_waveselector.addMapEntry("ChipSaw", 505, 506, 504);
	m_carrier_waveselector.addMapEntry("ChipSine", 506, 601, 505);

	m_carrier_waveselector.addMapEntry("WaveDraw Osc1", 601, 602, 506);
	m_carrier_waveselector.addMapEntry("WaveDraw Osc2", 602, 603, 601);
	m_carrier_waveselector.addMapEntry("WaveDraw Osc3", 603, 701, 602);

	m_carrier_waveselector.addMapEntry("ChipDraw Osc1", 701, 702, 603);
	m_carrier_waveselector.addMapEntry("ChipDraw Osc2", 702, 703, 701);
	m_carrier_waveselector.addMapEntry("ChipDraw Osc3", 703, 801, 702);

	m_carrier_waveselector.addMapEntry("SpecDraw Osc1", 801, 802, 703);
	m_carrier_waveselector.addMapEntry("SpecDraw Osc2", 802, 803, 801);
	m_carrier_waveselector.addMapEntry("SpecDraw Osc3", 803, 803, 802);

	m_carrier_waveselector.m_menu = fm_menu;

	ADD_MAP_ENTRY("Nasal", 120, m_modulator_waveselector)
	ADD_MAP_ENTRY("Reed", 130, m_modulator_waveselector)
	ADD_MAP_ENTRY("Glass", 140, m_modulator_waveselector)
	ADD_MAP_ENTRY("FM", 150, m_modulator_waveselector)
	ADD_MAP_ENTRY("BrokenSine", 160, m_modulator_waveselector)
	ADD_MAP_ENTRY("Harsh", 170, m_modulator_waveselector)
	ADD_MAP_ENTRY("Mean", 180, m_modulator_waveselector)
	ADD_MAP_ENTRY("Rectangular", 190, m_modulator_waveselector)
	ADD_MAP_ENTRY("Bitreduced", 200, m_modulator_waveselector)
	ADD_MAP_ENTRY("Strings", 210, m_modulator_waveselector)
	ADD_MAP_ENTRY("Piano", 220, m_modulator_waveselector)
	ADD_MAP_ENTRY("Organ", 230, m_modulator_waveselector)
	ADD_MAP_ENTRY("Oboe", 240, m_modulator_waveselector)
	ADD_MAP_ENTRY("Trumpet", 250, m_modulator_waveselector)
	ADD_MAP_ENTRY("MixedWaves A", 260, m_modulator_waveselector)
	ADD_MAP_ENTRY("MixedWaves B", 270, m_modulator_waveselector)
	ADD_MAP_ENTRY("MixedWaves C", 280, m_modulator_waveselector)
	ADD_MAP_ENTRY("MixedWaves D", 290, m_modulator_waveselector)
	ADD_MAP_ENTRY("Voice A", 300, m_modulator_waveselector)
	ADD_MAP_ENTRY("Voice B", 310, m_modulator_waveselector)
	ADD_MAP_ENTRY("Voice C", 320, m_modulator_waveselector)
	ADD_MAP_ENTRY("Voice D", 330, m_modulator_waveselector)
	ADD_MAP_ENTRY("Additive A", 340, m_modulator_waveselector)
	ADD_MAP_ENTRY("Additive B", 350, m_modulator_waveselector)
	ADD_MAP_ENTRY("Additive C", 360, m_modulator_waveselector)
	ADD_MAP_ENTRY("Additive D", 370, m_modulator_waveselector)
	ADD_MAP_ENTRY("Overtones A", 380, m_modulator_waveselector)
	ADD_MAP_ENTRY("Overtones B", 390, m_modulator_waveselector)
	ADD_MAP_ENTRY("Harmonics A", 400, m_modulator_waveselector)
	ADD_MAP_ENTRY("Harmonics B", 410, m_modulator_waveselector)
	ADD_MAP_ENTRY("MutantSaw A", 420, m_modulator_waveselector)
	ADD_MAP_ENTRY("MutantSaw B", 430, m_modulator_waveselector)
	ADD_MAP_ENTRY("MutantSquare A", 440, m_modulator_waveselector)
	ADD_MAP_ENTRY("MutantSquare B", 450, m_modulator_waveselector)

	addAndMakeVisible(m_osc_label);
	addAndMakeVisible(m_oct_label);
	addAndMakeVisible(m_semi_label);
	addAndMakeVisible(m_fine_label);
	addAndMakeVisible(m_vol_label);
	addAndMakeVisible(m_drift_label);
	addAndMakeVisible(m_pw_label);

	m_modulator_waveselector.setDecrementValue(121, 104);
	m_modulator_waveselector.setIncrementValue(454, 501);
	m_modulator_waveselector.addMapEntry("Sine", 1, 101, 1);
	m_modulator_waveselector.addMapEntry("Saw", 101, 102, 1);
	m_modulator_waveselector.addMapEntry("Square", 102, 103, 101);
	m_modulator_waveselector.addMapEntry("Triangle", 103, 104, 102);
	m_modulator_waveselector.addMapEntry("Sine", 104, 121, 103);
	m_modulator_waveselector.addMapEntry("Pulse 50", 501, 502, 454);
	m_modulator_waveselector.addMapEntry("Pulse 25", 502, 503, 501);
	m_modulator_waveselector.addMapEntry("Pulse 12.5", 503, 504, 502);
	m_modulator_waveselector.addMapEntry("ChipTriangle", 504, 505, 503);
	m_modulator_waveselector.addMapEntry("ChipSaw", 505, 506, 504);
	m_modulator_waveselector.addMapEntry("ChipSine", 506, 601, 505);

	m_modulator_waveselector.addMapEntry("WaveDraw Osc1", 601, 602, 506);
	m_modulator_waveselector.addMapEntry("WaveDraw Osc2", 602, 603, 601);
	m_modulator_waveselector.addMapEntry("WaveDraw Osc3", 603, 701, 602);

	m_modulator_waveselector.addMapEntry("ChipDraw Osc1", 701, 702, 603);
	m_modulator_waveselector.addMapEntry("ChipDraw Osc2", 702, 703, 701);
	m_modulator_waveselector.addMapEntry("ChipDraw Osc3", 703, 801, 702);

	m_modulator_waveselector.addMapEntry("SpecDraw Osc1", 801, 802, 703);
	m_modulator_waveselector.addMapEntry("SpecDraw Osc2", 802, 803, 801);
	m_modulator_waveselector.addMapEntry("SpecDraw Osc3", 803, 803, 802);

	m_modulator_waveselector.m_menu = fm_menu;

	*(m_vec_a.getRootMenu()) = vector_menu;
	*(m_vec_b.getRootMenu()) = vector_menu;
	*(m_vec_c.getRootMenu()) = vector_menu;
	*(m_vec_d.getRootMenu()) = vector_menu;

	m_chiptune_waveselector.setDecrementValue(121, 104);
	m_chiptune_waveselector.setIncrementValue(454, 601);

	m_chiptune_waveselector.addMapEntry("Pulse 50", 1, 2, 1);
	m_chiptune_waveselector.addMapEntry("Pulse 25", 2, 3, 1);
	m_chiptune_waveselector.addMapEntry("Pulse 12.5", 3, 4, 2);
	m_chiptune_waveselector.addMapEntry("ChipTriangle", 4, 5, 3);
	m_chiptune_waveselector.addMapEntry("ChipSaw", 5, 6, 4);
	m_chiptune_waveselector.addMapEntry("ChipSine", 6, 701, 5);
	m_chiptune_waveselector.addMapEntry("ChipDraw Osc1", 701, 702, 6);
	m_chiptune_waveselector.addMapEntry("ChipDraw Osc2", 702, 703, 701);
	m_chiptune_waveselector.addMapEntry("ChipDraw Osc3", 703, 703, 702);

	chip_menu.setLookAndFeel(&m_menu_feels);
	m_chiptune_waveselector.m_menu = chip_menu;

	m_carrier_waveselector.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("osc").setProperty(m_carrier_wave_identifier, (float)p_new_value, nullptr);
	};
	m_carrier_waveselector.setTopLeftPosition(WAVE_CARRIER_POS_X, WAVE_CARRIER_POS_Y);

	m_carrier_waveselector.setColor(FM_COLOR);
	m_carrier_waveselector.setTooltip("Selects the wave for the carrier osc");
	addChildComponent(m_carrier_waveselector);

	m_wavetable_waveselector.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("osc").setProperty(m_wavetable_identifier, (float)p_new_value, nullptr);
	};
	m_wavetable_waveselector.setTopLeftPosition(WAVE_CARRIER_POS_X, WAVE_CARRIER_POS_Y);
	m_wavetable_waveselector.addWave(1, "Classic Analog");
	m_wavetable_waveselector.addWave(2, "Nasal");
	m_wavetable_waveselector.addWave(3, "Reed");
	m_wavetable_waveselector.addWave(4, "Glass");
	m_wavetable_waveselector.addWave(5, "FM");
	m_wavetable_waveselector.addWave(6, "BrokenSine");
	m_wavetable_waveselector.addWave(7, "Harsh");
	m_wavetable_waveselector.addWave(8, "Mean");
	m_wavetable_waveselector.addWave(9, "Rectangular");
	m_wavetable_waveselector.addWave(10, "Bitreduced");
	m_wavetable_waveselector.addWave(11, "Strings");
	m_wavetable_waveselector.addWave(12, "Piano");
	m_wavetable_waveselector.addWave(13, "Organ");
	m_wavetable_waveselector.addWave(14, "Oboe");
	m_wavetable_waveselector.addWave(15, "Trumpet");
	m_wavetable_waveselector.addWave(16, "MixedWaves A");
	m_wavetable_waveselector.addWave(17, "MixedWaves B");
	m_wavetable_waveselector.addWave(18, "MixedWaves C");
	m_wavetable_waveselector.addWave(19, "MixedWaves D");
	m_wavetable_waveselector.addWave(20, "Voice A");
	m_wavetable_waveselector.addWave(21, "Voice B");
	m_wavetable_waveselector.addWave(22, "Voice C");
	m_wavetable_waveselector.addWave(23, "Voice D");
	m_wavetable_waveselector.addWave(24, "Additive A");
	m_wavetable_waveselector.addWave(25, "Additive B");
	m_wavetable_waveselector.addWave(26, "Additive C");
	m_wavetable_waveselector.addWave(27, "Additive D");
	m_wavetable_waveselector.addWave(28, "Overtones A");
	m_wavetable_waveselector.addWave(29, "Overtones B");
	m_wavetable_waveselector.addWave(30, "Harmonics A");
	m_wavetable_waveselector.addWave(31, "Harmonics B");
	m_wavetable_waveselector.addWave(32, "MutantSaw A");
	m_wavetable_waveselector.addWave(33, "MutantSaw B");
	m_wavetable_waveselector.addWave(34, "MutantSquare A");
	m_wavetable_waveselector.addWave(35, "MutantSquare B");

	m_wavetable_waveselector.setTooltip("Selects the waveform for the oscillator");
	addChildComponent(m_wavetable_waveselector);

	m_modulation_source.setTopLeftPosition(POS_MODULATION_X, POS_MODULATION_Y);
	m_modulation_source.addWave(1, "ModEnvelope");
	m_modulation_source.addWave(10, "LFO 1");
	//just toggle for this one
	m_modulation_source.setIncrementValue(1, 10);
	m_modulation_source.setIncrementValue(10, 1);
	m_modulation_source.setDecrementValue(1, 10);
	m_modulation_source.setDecrementValue(10, 1);
	m_modulation_source.setTooltip("Select which source modulates the wavetable position");
	m_modulation_source.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("osc").setProperty(
		    m_modulation_source_identifier, (float)p_new_value, nullptr);
	};
	m_modulation_source.setColor(WAVETABLE_DROPDOWN_COLOR);
	addChildComponent(m_modulation_source);

	m_modulator_waveselector.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("osc").setProperty(
		    m_modulator_wave_identifier, (float)p_new_value, nullptr);
	};
	m_modulator_waveselector.setTopLeftPosition(WAVE_MODULATOR_POS_X, WAVE_MODULATOR_POS_Y);
	m_modulator_waveselector.setColor(FM_COLOR);
	m_modulator_waveselector.setTooltip("Selects the waveform for the modulator osc");
	addChildComponent(m_modulator_waveselector);

	m_carrier_ratio.OnValueChange = [&](int p_new_value) {
		//SETAUDIOFULLRANGE(m_carrier_ratio_identifier, p_new_value);
		m_value_tree.state.getChildWithName("osc").setProperty(m_carrier_ratio_identifier, p_new_value, nullptr);
	};
	m_carrier_ratio.setTopLeftPosition(RATIO_CARRIER_POS_X, RATIO_CARRIER_POS_Y);
	m_carrier_ratio.setRange(1, 16);
	m_carrier_ratio.setColor(FM_COLOR);
	m_carrier_ratio.setTooltip("The pitch ratio of the carrier to base frequency");
	m_carrier_ratio.setMouseDragDivisor(20.f);

	addChildComponent(m_carrier_ratio);

	m_modulator_ratio.OnValueChange = [&](int p_new_value) {
		//SETAUDIOFULLRANGE(m_modulator_ratio_identifier, p_new_value);
		m_value_tree.state.getChildWithName("osc").setProperty(m_modulator_ratio_identifier, p_new_value, nullptr);
	};
	m_modulator_ratio.setTopLeftPosition(RATIO_MODULATOR_POS_X, RATIO_MODULATOR_POS_Y);
	m_modulator_ratio.setRange(1, 16);
	m_modulator_ratio.setColor(FM_COLOR);
	m_modulator_ratio.setTooltip("The pitch ratio of the modulator to base frequency");
	m_modulator_ratio.setMouseDragDivisor(20.f);
	addChildComponent(m_modulator_ratio);

	m_fm_exp.onStateChange = [&]() {};
	m_fm_exp.setTooltip("Whether to use exponential or linear FM.\nExponential FM has a more "
	                    "musical character to it, as it preserves the perceived note pitch.");
	addChildComponent(m_fm_exp);

	m_chipdraw.setTopLeftPosition(CHIPDRAW_POSITION_X, CHIPDRAW_POSITION_Y);
	m_chipdraw.setColor(chip_color);
	m_chipdraw.setDrawColor(juce::Colour(238, 230, 217));
	m_chipdraw.onDraw = [&]() { m_chipdraw_convert.setToggleState(false, dontSendNotification); };
	m_chipdraw.setTooltip("Draw a custom 4Bit waveform.\n\nDon't forget to apply "
	                      "your waveform with the button on the bottom right.");
	addChildComponent(m_chipdraw);

	juce::Colour wave_color(71, 92, 108);
	juce::Colour wave_draw_color(99, 165, 236);

	m_wavedraw.setTopLeftPosition(CHIPDRAW_POSITION_X, CHIPDRAW_POSITION_Y);
	m_wavedraw.setColor(wave_color);
	m_wavedraw.setDrawColor(wave_draw_color);
	m_wavedraw.onDraw = [&]() { m_wavedraw_convert.setToggleState(false, dontSendNotification); };
	m_wavedraw.setTooltip("Become the Picasso of music production.\n\nDon't forget to apply your "
	                      "waveform with the button on the bottom right.");
	addChildComponent(m_wavedraw);

	juce::Colour spec_color(61, 80, 70);
	juce::Colour spec_draw_color(116, 180, 147);

	m_specdraw.setTopLeftPosition(CHIPDRAW_POSITION_X, CHIPDRAW_POSITION_Y);
	m_specdraw.setColor(spec_color);
	m_specdraw.setDrawColor(spec_draw_color);
	m_specdraw.onDraw = [&]() { m_specdraw_convert.setToggleState(false, dontSendNotification); };
	m_specdraw.setTooltip("Draw the spectrum of the oscillator. A single peak corresponds to a "
	                      "sine function.\n\nDon't forget to apply your waveform with the button "
	                      "on the bottom right.");
	addChildComponent(m_specdraw);

	juce::Colour vector_color(25, 50, 60);

	m_xy.setInlay(1);
	m_xy.setColor(vector_color);
	m_xy.setTooltip("Transition seamlessly through four waveforms. Each corner contains a "
	                "waveform, which can be selected by the dropdowns on the right.");
	addAndMakeVisible(m_xy);

	m_vec_a.setInlay(1);
	m_vec_a.setEditableText(false);
	m_vec_a.setSelectedId(101, dontSendNotification);
	m_vec_a.setColor(vector_color);
	m_vec_a.setTooltip("Select the waveform to the bottom left of the XY pad");
	m_vec_a.onChange = [&]() {
		m_value_tree.state.getChildWithName("osc").setProperty(
		    m_vec_a_identifier, (float)m_vec_a.getSelectedId(), nullptr);
	};
	addChildComponent(m_vec_a);

	m_vec_b.setInlay(1);
	m_vec_b.setEditableText(false);
	m_vec_b.setSelectedId(102, dontSendNotification);
	m_vec_b.setColor(vector_color);
	m_vec_b.setTooltip("Select the waveform to the top left of the XY pad");
	m_vec_b.onChange = [&]() {
		m_value_tree.state.getChildWithName("osc").setProperty(
		    m_vec_b_identifier, (float)m_vec_b.getSelectedId(), nullptr);
	};
	addChildComponent(m_vec_b);

	m_vec_c.setInlay(1);
	m_vec_c.setEditableText(false);
	m_vec_c.setSelectedId(103, dontSendNotification);
	m_vec_c.setColor(vector_color);
	m_vec_c.setTooltip("Select the waveform to the top right of the XY pad");
	m_vec_c.onChange = [&]() {
		m_value_tree.state.getChildWithName("osc").setProperty(
		    m_vec_c_identifier, (float)m_vec_c.getSelectedId(), nullptr);
	};
	addChildComponent(m_vec_c);

	m_vec_d.setInlay(1);
	m_vec_d.setEditableText(false);
	m_vec_d.setSelectedId(104, dontSendNotification);
	m_vec_d.setColor(vector_color);
	m_vec_d.setTooltip("Select the waveform to the bottom right of the XY pad");
	m_vec_d.onChange = [&]() {
		m_value_tree.state.getChildWithName("osc").setProperty(
		    m_vec_d_identifier, (float)m_vec_d.getSelectedId(), nullptr);
	};
	addChildComponent(m_vec_d);

	m_xy_x.setSliderStyle(Slider::RotaryVerticalDrag);
	m_xy_x.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_xy_x.setNumDecimalPlacesToDisplay(3);
	m_xy_x.setKnobTooltip("X part of the XY pad");
	m_xy_x.onValueChange = [&] { m_xy.setX(m_xy_x.getValue()); };
	addChildComponent(m_xy_x);

	m_xy_y.setSliderStyle(Slider::RotaryVerticalDrag);
	m_xy_y.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_xy_y.setNumDecimalPlacesToDisplay(3);
	m_xy_y.setKnobTooltip("X part of the XY pad");
	m_xy_y.onValueChange = [&] { m_xy.setY(m_xy_y.getValue()); };
	addChildComponent(m_xy_y);

	m_vol.setNumDecimalPlacesToDisplay(2);
	m_pw.setNumDecimalPlacesToDisplay(3);
	m_fine.setNumDecimalPlacesToDisplay(2);
	m_drift.setNumDecimalPlacesToDisplay(3);
	m_position.setNumDecimalPlacesToDisplay(3);
	m_position_multi.setNumDecimalPlacesToDisplay(3);
	m_spread.setNumDecimalPlacesToDisplay(3);
	m_detune.setNumDecimalPlacesToDisplay(3);
	m_pos_mod.setNumDecimalPlacesToDisplay(3);
	m_speed.setNumDecimalPlacesToDisplay(2);
	m_fm.setNumDecimalPlacesToDisplay(3);
	m_lp.setNumDecimalPlacesToDisplay(1);
	m_hp.setNumDecimalPlacesToDisplay(1);

	forceValueTreeOntoComponents(m_value_tree.state, std::stoi(m_osc_number), false);

	SET_CTR_KEY(m_oct);
	SET_CTR_KEY(m_semi);
	SET_CTR_KEY(m_fine);
	SET_CTR_KEY(m_vol);
	SET_CTR_KEY(m_pw);
	SET_CTR_KEY(m_drift);
	SET_CTR_KEY(m_step_1);
	SET_CTR_KEY(m_step_2);
	SET_CTR_KEY(m_step_3);
	SET_CTR_KEY(m_fm);
	SET_CTR_KEY(m_speed);
	SET_CTR_KEY(m_lp);
	SET_CTR_KEY(m_hp);
	SET_CTR_KEY(m_position);
	SET_CTR_KEY(m_position_multi);
	SET_CTR_KEY(m_spread);
	SET_CTR_KEY(m_detune);
	SET_CTR_KEY(m_pos_mod);

	resetVectorWaves();
}

OscComponent::~OscComponent() {
	m_carrier_waveselector.m_menu.setLookAndFeel(nullptr);
	m_modulator_waveselector.m_menu.setLookAndFeel(nullptr);
	m_chiptune_waveselector.m_menu.setLookAndFeel(nullptr);

	//m_value_tree.removeParameterListener("osc" + m_osc_number + "_carrier_ratio", &m_carrier_ratio);
	//m_value_tree.removeParameterListener("osc" + m_osc_number + "_modulator_ratio", &m_modulator_ratio);
}

void OscComponent::resetVectorWaves() {
	int store = m_vec_a.getSelectedId();
	m_vec_a.setSelectedId(1);
	m_vec_a.setSelectedId(store);

	store = m_vec_b.getSelectedId();
	m_vec_b.setSelectedId(1);
	m_vec_b.setSelectedId(store);

	store = m_vec_c.getSelectedId();
	m_vec_c.setSelectedId(1);
	m_vec_c.setSelectedId(store);

	store = m_vec_d.getSelectedId();
	m_vec_d.setSelectedId(1);
	m_vec_d.setSelectedId(store);
}

void OscComponent::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)
	//g.drawImageAt(m_background, 0, 0);
	g.setColour(COL_LIGHT);
	g.drawRect(getLocalBounds(), 1);
}

void OscComponent::setOscType(int p_osc_type, bool p_force) {
	if (p_osc_type == 0) {
		return;
	}
	if (m_osc_type == p_osc_type && !p_force) {
		return;
	}
	m_osc_type = p_osc_type;
	hideAllComponents();
	switch (p_osc_type) {
	case 1:
		setOscTypeBypass();
		break;
	case OSC_TYPE_ANALOG:
		setOscTypeAnalog();
		break;
	case OSC_TYPE_WAVETABLE:
		setOscTypeWavetable();
		break;
	case OSC_TYPE_MULTI:
		setOscTypeMulti();
		break;
	case OSC_TYPE_VECTOR:
		setOscTypeVector();
		break;
	case OSC_TYPE_CHIPTUNE:
		setOscTypeChiptune();
		break;
	case OSC_TYPE_FM:
		setOscTypeFM();
		break;
	case OSC_TYPE_PM:
		setOscTypePM();
		break;
	case OSC_TYPE_NOISE:
		setOscTypeNoise();
		break;
	case OSC_TYPE_WAVEDRAW:
		setOscTypeWavedraw();
		break;
	case OSC_TYPE_CHIPDRAW:
		setOscTypeChipdraw();
		break;
	case OSC_TYPE_SPECDRAW:
		setOscTypeSpecdraw();
		break;
	default:
		setOscTypeBypass();
		break;
	}
	repaint();
}
void OscComponent::setOscTypeBypass() {
	m_background = m_background_bypass;
}

void OscComponent::setOscTypeAnalog() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::analog_backdrop_150_png, BinaryData::analog_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::analog_backdrop_png, BinaryData::analog_backdrop_pngSize);
	}
	showAnalogComponents();
}
void OscComponent::setOscTypeChiptune() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::chiptune_backdrop_150_png, BinaryData::chiptune_backdrop_150_pngSize);
	} else {
		m_background =
		    ImageCache::getFromMemory(BinaryData::chiptune_backdrop_png, BinaryData::chiptune_backdrop_pngSize);
	}
	showChiptuneComponents();
}

void OscComponent::setOscTypeFM() {
	if (m_GUI_big) {
		m_background = ImageCache::getFromMemory(BinaryData::fm_backdrop_150_png, BinaryData::fm_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::fm_backdrop_png, BinaryData::fm_backdrop_pngSize);
	}
	showFMComponents();
}

void OscComponent::setOscTypePM() {
	if (m_GUI_big) {
		m_background = ImageCache::getFromMemory(BinaryData::pm_backdrop_150_png, BinaryData::pm_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::pm_backdrop_png, BinaryData::pm_backdrop_pngSize);
	}
	showPMComponents();
}

void OscComponent::setOscTypeChipdraw() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::chipdraw_backdrop_150_png, BinaryData::chipdraw_backdrop_150_pngSize);
	} else {
		m_background =
		    ImageCache::getFromMemory(BinaryData::chipdraw_backdrop_png, BinaryData::chipdraw_backdrop_pngSize);
	}
	showChipdrawComponents();
}

void OscComponent::setOscTypeWavedraw() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::wavedraw_backdrop_150_png, BinaryData::wavedraw_backdrop_150_pngSize);
	} else {
		m_background =
		    ImageCache::getFromMemory(BinaryData::wavedraw_backdrop_png, BinaryData::wavedraw_backdrop_pngSize);
	}
	showWavedrawComponents();
}

void OscComponent::setOscTypeSpecdraw() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::specdraw_backdrop_150_png, BinaryData::specdraw_backdrop_150_pngSize);
	} else {
		m_background =
		    ImageCache::getFromMemory(BinaryData::specdraw_backdrop_png, BinaryData::specdraw_backdrop_pngSize);
	}
	showSpecdrawComponents();
}

void OscComponent::setOscTypeNoise() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::noise_backdrop_150_png, BinaryData::noise_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::noise_backdrop_png, BinaryData::noise_backdrop_pngSize);
	}
	showNoiseComponents();
}

void OscComponent::setOscTypeVector() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::vector_backdrop_150_png, BinaryData::vector_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::vector_backdrop_png, BinaryData::vector_backdrop_pngSize);
	}
	showVectorComponents();
}

void OscComponent::setOscTypeWavetable() {
	if (m_GUI_big) {
		m_background = ImageCache::getFromMemory(BinaryData::wavetable_backdrop_150_png,
		                                         BinaryData::wavetable_backdrop_150_pngSize);
	} else {
		m_background =
		    ImageCache::getFromMemory(BinaryData::wavetable_backdrop_png, BinaryData::wavetable_backdrop_pngSize);
	}
	showWavetableComponents();
}

void OscComponent::setOscTypeMulti() {
	if (m_GUI_big) {
		m_background =
		    ImageCache::getFromMemory(BinaryData::multi_backdrop_150_png, BinaryData::multi_backdrop_150_pngSize);
	} else {
		m_background = ImageCache::getFromMemory(BinaryData::multi_backdrop_png, BinaryData::multi_backdrop_pngSize);
	}
	showMultiComponents();
}

void OscComponent::hideAllComponents() {
	m_oct.setVisible(false);
	m_semi.setVisible(false);
	m_fine.setVisible(false);
	m_vol.setVisible(false);
	m_pw.setVisible(false);
	m_drift.setVisible(false);
	m_step_1.setVisible(false);
	m_step_2.setVisible(false);
	m_step_3.setVisible(false);
	m_speed.setVisible(false);
	m_arp.setVisible(false);
	m_step_button.setVisible(false);
	m_noise.setVisible(false);
	m_LED_saw.setVisible(false);
	m_LED_pulse.setVisible(false);
	m_LED_triangle.setVisible(false);
	m_LED_sine.setVisible(false);
	m_reset.setVisible(false);
	m_chiptune_waveselector.setVisible(false);
	m_carrier_waveselector.setVisible(false);
	m_modulator_waveselector.setVisible(false);
	m_carrier_ratio.setVisible(false);
	m_modulator_ratio.setVisible(false);
	m_fm.setVisible(false);
	m_fm_exp.setVisible(false);
	m_chipdraw.setVisible(false);
	m_chipdraw_convert.setVisible(false);
	m_wavedraw.setVisible(false);
	m_wavedraw_convert.setVisible(false);
	m_specdraw.setVisible(false);
	m_specdraw_convert.setVisible(false);
	m_lp.setVisible(false);
	m_hp.setVisible(false);
	m_xy.setVisible(false);
	m_xy_x.setVisible(false);
	m_xy_y.setVisible(false);
	m_vec_a.setVisible(false);
	m_vec_b.setVisible(false);
	m_vec_c.setVisible(false);
	m_vec_d.setVisible(false);
	m_position.setVisible(false);
	m_position_multi.setVisible(false);
	m_spread.setVisible(false);
	m_detune.setVisible(false);
	m_wavetable_waveselector.setVisible(false);
	m_modulation_source.setVisible(false);
	m_sync.setVisible(false);
	m_pos_mod.setVisible(false);
}
void OscComponent::showVolComponent() {
	m_vol.setVisible(true);
}

void OscComponent::showPitchComponents() {
	m_oct.setVisible(true);
	m_semi.setVisible(true);
	m_fine.setVisible(true);
	m_reset.setVisible(true);
}

void OscComponent::showAnalogComponents() {
	showVolComponent();
	showPitchComponents();
	m_pw.setVisible(true);
	m_drift.setVisible(true);
	m_LED_saw.setVisible(true);
	m_LED_pulse.setVisible(true);
	m_LED_triangle.setVisible(true);
	m_LED_sine.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showChiptuneComponents() {
	showVolComponent();
	showPitchComponents();
	m_step_1.setVisible(true);
	m_step_2.setVisible(true);
	m_step_3.setVisible(true);
	m_speed.setVisible(true);
	m_arp.setVisible(true);
	m_step_button.setVisible(true);
	m_noise.setVisible(true);
	m_chiptune_waveselector.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showFMComponents() {
	showVolComponent();
	showPitchComponents();
	m_carrier_waveselector.setVisible(true);
	m_modulator_waveselector.setVisible(true);
	m_carrier_ratio.setVisible(true);
	m_modulator_ratio.setVisible(true);
	m_fm.setVisible(true);
	m_fm_exp.setVisible(true);

	m_carrier_waveselector.setColor(FM_COLOR);
	m_modulator_waveselector.setColor(FM_COLOR);
	m_carrier_ratio.setColor(FM_COLOR);
	m_modulator_ratio.setColor(FM_COLOR);
}

void OscComponent::showPMComponents() {
	showVolComponent();
	showPitchComponents();
	m_carrier_waveselector.setVisible(true);
	m_modulator_waveselector.setVisible(true);
	m_carrier_ratio.setVisible(true);
	m_modulator_ratio.setVisible(true);
	m_fm.setVisible(true);

	m_carrier_waveselector.setColor(PM_COLOR);
	m_modulator_waveselector.setColor(PM_COLOR);
	m_carrier_ratio.setColor(PM_COLOR);
	m_modulator_ratio.setColor(PM_COLOR);
}

void OscComponent::showChipdrawComponents() {
	showVolComponent();
	showPitchComponents();
	m_chipdraw.setVisible(true);
	m_chipdraw_convert.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showWavedrawComponents() {
	showVolComponent();
	showPitchComponents();
	m_wavedraw.setVisible(true);
	m_wavedraw_convert.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showSpecdrawComponents() {
	showVolComponent();
	showPitchComponents();
	m_specdraw.setVisible(true);
	m_specdraw_convert.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showVectorComponents() {
	showVolComponent();
	showPitchComponents();
	m_xy.setVisible(true);
	m_vec_a.setVisible(true);
	m_vec_b.setVisible(true);
	m_vec_c.setVisible(true);
	m_vec_d.setVisible(true);
	m_xy_x.setVisible(true);
	m_xy_y.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showWavetableComponents() {
	showVolComponent();
	showPitchComponents();
	m_wavetable_waveselector.setColor(WAVETABLE_DROPDOWN_COLOR);
	if (m_GUI_big) {
		m_wavetable_waveselector.setTopLeftPosition(OdinHelper::c150(WAVETABLE_WAVE_X),
		                                            OdinHelper::c150(WAVETABLE_WAVE_Y));
	} else {
		m_wavetable_waveselector.setTopLeftPosition(WAVETABLE_WAVE_X, WAVETABLE_WAVE_Y);
	}
	m_position.setVisible(true);
	m_wavetable_waveselector.setVisible(true);
	m_sync.setVisible(true);
	m_pos_mod.setVisible(true);
	m_modulation_source.setVisible(true);
}

void OscComponent::showMultiComponents() {
	showVolComponent();
	showPitchComponents();
	m_position_multi.setVisible(true);
	m_spread.setVisible(true);
	m_detune.setVisible(true);

	m_wavetable_waveselector.setColor(MULTI_DROPDOWN_COLOR);
	if (m_GUI_big) {
		m_wavetable_waveselector.setTopLeftPosition(OdinHelper::c150(MULTI_WAVE_X), OdinHelper::c150(MULTI_WAVE_Y));
	} else {
		m_wavetable_waveselector.setTopLeftPosition(MULTI_WAVE_X, MULTI_WAVE_Y);
	}
	m_wavetable_waveselector.setVisible(true);
}

void OscComponent::showNoiseComponents() {
	showVolComponent();
	m_lp.setVisible(true);
	m_hp.setVisible(true);
}

void OscComponent::createWavedrawTables() {
	m_WT_container->createWavedrawTable(std::stoi(m_osc_number) - 1, m_wavedraw.getDrawnTable(), 44100.f);

	// write values to audiovaluetree
	float *table = m_wavedraw.getDrawnTable();
	auto node    = m_value_tree.state.getChildWithName("draw");
	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
		node.setProperty(String("osc" + m_osc_number + "_wavedraw_values_" + std::to_string(i)), table[i], nullptr);
	}
}

void OscComponent::createChipdrawTables() {

	m_WT_container->createChipdrawTable(std::stoi(m_osc_number) - 1, m_chipdraw.getDrawnTable(), 44100.f);

	// write values to audiovaluetree
	float *table = m_chipdraw.getDrawnTable();
	auto node    = m_value_tree.state.getChildWithName("draw");
	for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
		node.setProperty(String("osc" + m_osc_number + "_chipdraw_values_" + std::to_string(i)), table[i], nullptr);
	}
}

void OscComponent::createSpecdrawTables() {
	m_WT_container->createSpecdrawTable(std::stoi(m_osc_number) - 1, m_specdraw.getDrawnTable(), 44100.f);
	// write values to audiovaluetree
	float *table = m_specdraw.getDrawnTable();
	auto node    = m_value_tree.state.getChildWithName("draw");
	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		node.setProperty(String("osc" + m_osc_number + "_specdraw_values_" + std::to_string(i)), table[i], nullptr);
	}
}

#ifdef WTGEN
void OscComponent::writeWavedrawTableToFile() {
	Utilities::getInstance().writeWavedrawTable(m_wavedraw.getDrawnTable(), "WAVEDRAW_TABLE");
}

void OscComponent::writeSpecdrawTableToFile() {
	Utilities::getInstance().writeSpecdrawTable(m_specdraw.getDrawnTable(), "SPECDRAW_TABLE");
}

void OscComponent::writeChipdrawTableToFile() {
	Utilities::getInstance().writeChipdrawTable(m_chipdraw.getDrawnTable(), "CHIPDRAW_TABLE");
}
#endif

void OscComponent::forceValueTreeOntoComponents(ValueTree p_tree, int p_index, bool p_create_wavetables) {
	std::string index = std::to_string(p_index);

	// analog waveform
	switch ((int)m_value_tree.state.getChildWithName("osc")[m_analog_wave_identifier]) {
	case 0:
		m_LED_saw.setToggleState(true, sendNotification);
		break;
	case 1:
		m_LED_pulse.setToggleState(true, sendNotification);
		break;
	case 2:
		m_LED_triangle.setToggleState(true, sendNotification);
		break;
	case 3:
		m_LED_sine.setToggleState(true, sendNotification);
		break;
	default:
		break;
	}

	// wavetable waveselctor
	m_wavetable_waveselector.setValue(m_value_tree.state.getChildWithName("osc")[m_wavetable_identifier]);
	m_modulation_source.setValue(m_value_tree.state.getChildWithName("osc")[m_modulation_source_identifier]);

	// vector
	m_vec_a.setSelectedId(m_value_tree.state.getChildWithName("osc")[m_vec_a_identifier]);
	m_vec_b.setSelectedId(m_value_tree.state.getChildWithName("osc")[m_vec_b_identifier]);
	m_vec_c.setSelectedId(m_value_tree.state.getChildWithName("osc")[m_vec_c_identifier]);
	m_vec_d.setSelectedId(m_value_tree.state.getChildWithName("osc")[m_vec_d_identifier]);

	m_xy.setX(GETAUDIO("osc" + m_osc_number + "_vec_x"));
	m_xy.setY(GETAUDIO("osc" + m_osc_number + "_vec_y"));

	// fm
	m_carrier_waveselector.setValue(m_value_tree.state.getChildWithName("osc")[m_carrier_wave_identifier]);
	m_modulator_waveselector.setValue(m_value_tree.state.getChildWithName("osc")[m_modulator_wave_identifier]);

	//m_carrier_ratio.setValue(GETAUDIO(m_carrier_ratio_identifier));
	//m_modulator_ratio.setValue(GETAUDIO(m_modulator_ratio_identifier));

	m_carrier_ratio.setValue(m_value_tree.state.getChildWithName("osc")[m_carrier_ratio_identifier]);
	m_modulator_ratio.setValue(m_value_tree.state.getChildWithName("osc")[m_modulator_ratio_identifier]);

	m_fm_exp.setValue(m_value_tree.getParameterAsValue("osc" + m_osc_number + "_exp_fm").getValue());

	// chiptune
	m_chiptune_waveselector.setValue(
	    m_value_tree.state.getChildWithName("osc")[String("osc" + m_osc_number + "_chipwave")]);
	auto node = m_value_tree.state;

	node = m_value_tree.state.getChildWithName("draw");

	// wavedraw
	float wavedraw_values[WAVEDRAW_STEPS_X];
	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
		wavedraw_values[i] = (float)node[String("osc" + m_osc_number + "_wavedraw_values_" + std::to_string(i))];
	}

	m_wavedraw.setDrawnTable(wavedraw_values);
	m_wavedraw_convert.setToggleState(true, dontSendNotification);

	// chipdraw
	for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
		wavedraw_values[i] = (float)node[String("osc" + m_osc_number + "_chipdraw_values_" + std::to_string(i))];
	}
	m_chipdraw.setDrawnTable(wavedraw_values);
	m_chipdraw_convert.setToggleState(true, dontSendNotification);

	// specdraw
	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		wavedraw_values[i] = (float)node[String("osc" + m_osc_number + "_specdraw_values_" + std::to_string(i))];
	}
	m_specdraw.setDrawnTable(wavedraw_values);
	m_specdraw_convert.setToggleState(true, dontSendNotification);

	if (p_create_wavetables) {
		createWavedrawTables();
		createChipdrawTables();
		createSpecdrawTables();
	}
}

void OscComponent::setGUIBig() {
	m_GUI_big = true;

	m_xy.setGUIBig();
	m_vec_a.setGUIBig();
	m_vec_b.setGUIBig();
	m_vec_c.setGUIBig();
	m_vec_d.setGUIBig();
	m_vec_a.setInlayLeft(1);
	m_vec_b.setInlayLeft(1);
	m_vec_c.setInlayLeft(1);
	m_vec_d.setInlayLeft(1);
	m_wavedraw.setGUIBig();
	m_chipdraw.setGUIBig();
	m_specdraw.setGUIBig();
	m_chiptune_waveselector.setGUIBig();
	m_carrier_waveselector.setGUIBig();
	m_modulator_waveselector.setGUIBig();
	m_wavetable_waveselector.setGUIBig();
	m_modulation_source.setGUIBig();
	m_menu_feels.setGUIBig();
}

void OscComponent::setGUISmall() {
	m_GUI_big = false;

	m_xy.setGUISmall();
	m_vec_a.setGUISmall();
	m_vec_b.setGUISmall();
	m_vec_c.setGUISmall();
	m_vec_d.setGUISmall();
	m_vec_a.setInlayLeft(0);
	m_vec_b.setInlayLeft(0);
	m_vec_c.setInlayLeft(0);
	m_vec_d.setInlayLeft(0);
	m_wavedraw.setGUISmall();
	m_chipdraw.setGUISmall();
	m_specdraw.setGUISmall();
	m_chiptune_waveselector.setGUISmall();
	m_carrier_waveselector.setGUISmall();
	m_modulator_waveselector.setGUISmall();
	m_wavetable_waveselector.setGUISmall();
	m_modulation_source.setGUISmall();
	m_menu_feels.setGUISmall();
}

void OscComponent::resized() {
	GET_LOCAL_AREA(m_osc_label, "Osc_osc_label");
	GET_LOCAL_AREA(m_oct_label, "Osc_oct_label");
	GET_LOCAL_AREA(m_semi_label, "Osc_semi_label");
	GET_LOCAL_AREA(m_fine_label, "Osc_fine_label");
	GET_LOCAL_AREA(m_vol_label, "Osc_vol_label");
	GET_LOCAL_AREA(m_drift_label, "Osc_drift_label");
	GET_LOCAL_AREA(m_pw_label, "Osc_pw_label");
	GET_LOCAL_AREA(m_vol, "OscVol");
	GET_LOCAL_AREA(m_reset, "OscReset");
	GET_LOCAL_AREA(m_sync, "OscSync");
	GET_LOCAL_AREA(m_oct, "OscOct");
	GET_LOCAL_AREA(m_semi, "OscSemi");
	GET_LOCAL_AREA(m_fine, "OscFine");
	GET_LOCAL_AREA(m_LED_saw, "OscSawLED");
	GET_LOCAL_AREA(m_LED_pulse, "OscPulseLED");
	GET_LOCAL_AREA(m_LED_triangle, "OscTriangleLED");
	GET_LOCAL_AREA(m_LED_sine, "OscSineLED");
	GET_LOCAL_AREA(m_pw, "OscPW");
	GET_LOCAL_AREA(m_drift, "OscDrift");
	GET_LOCAL_AREA(m_arp, "OscArpeggiator");
	GET_LOCAL_AREA(m_noise, "OscNoise");
	GET_LOCAL_AREA(m_step_button, "OscStep");
	GET_LOCAL_AREA(m_step_1, "OscStep1");
	GET_LOCAL_AREA(m_step_2, "OscStep2");
	GET_LOCAL_AREA(m_step_3, "OscStep3");
	GET_LOCAL_AREA(m_fm, "OscFM");
	GET_LOCAL_AREA(m_lp, "OscLP");
	GET_LOCAL_AREA(m_hp, "OscHP");
	GET_LOCAL_AREA(m_position, "OscPos");
	GET_LOCAL_AREA(m_detune, "OscDetune");

	GET_LOCAL_AREA(m_chiptune_waveselector, "ChipWave");
	GET_LOCAL_AREA(m_carrier_waveselector, "CarrWave");
	GET_LOCAL_AREA(m_modulator_waveselector, "ModWave");
	GET_LOCAL_AREA(m_wavetable_waveselector, "OscWave");
	GET_LOCAL_AREA(m_modulation_source, "OscModSource");
}