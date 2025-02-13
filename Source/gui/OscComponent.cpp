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
#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../PluginProcessor.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"

OscComponent::OscComponent(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts, const std::string &p_osc_number) :
    m_value_tree(vts),
    m_reset("reset_button", "Reset", OdinButton::Type::button_7x4),
    m_sync("osc_sync_button", "Sync", OdinButton::Type::button_7x4),
    m_LED_saw("LED_Saw", "Saw", OdinButton::Type::button_9x4),
    m_LED_pulse("LED_pulse", "PWM", OdinButton::Type::button_9x4),
    m_LED_triangle("LED_triangle", "Tri", OdinButton::Type::button_9x4),
    m_LED_sine("LED_sine", "Sine", OdinButton::Type::button_9x4),
    m_arp("arp", "", OdinButton::Type::power_4x4),
    m_step_button("step", "3", OdinButton::Type::button_7x4),
    m_noise("noise", "Noise", OdinButton::Type::button_7x4),
    m_carrier_label("Carrier"),
    m_modulator_label("Modulator"),
    m_FM_label("FM"),
    m_PM_label("PM"),
    m_exponential_label("Exponential"),
    m_linear_label("Linear"),
    m_chiptune_waveselector(true),
    m_carrier_waveselector(false),
    m_modulator_waveselector(true),
    m_wavetable_waveselector(true),
    m_modulation_source(true),
    m_carrier_ratio(false, NumberSelector::Type::selector_12x4),
    m_modulator_ratio(true, NumberSelector::Type::selector_12x4),
    m_fm_exp("fm_exp", LeftRightButton::Type::osc_fm_exp),
    m_xy(vts, "osc" + p_osc_number + "_vec_", m_xy_x, m_xy_y, true),
    m_HP_label("HighPass"),
    m_LP_label("LowPass"),
    m_osc_number(p_osc_number),
    m_wavetable_identifier("osc" + p_osc_number + "_wavetable"),
    m_modulation_source_identifier("osc" + p_osc_number + "_mod_source"),
    m_chipwave_identifier("osc" + p_osc_number + "_chipwave"),
    m_modulator_wave_identifier("osc" + p_osc_number + "_modulator_wave"),
    m_carrier_wave_identifier("osc" + p_osc_number + "_carrier_wave"),
    m_modulator_ratio_identifier("osc" + p_osc_number + "_modulator_ratio"),
    m_carrier_ratio_identifier("osc" + p_osc_number + "_carrier_ratio"),
    m_analog_wave_identifier("osc" + p_osc_number + "_analog_wave"),
    m_vec_a_identifier("osc" + p_osc_number + "_vec_a"),
    m_vec_b_identifier("osc" + p_osc_number + "_vec_b"),
    m_vec_c_identifier("osc" + p_osc_number + "_vec_c"),
    m_vec_d_identifier("osc" + p_osc_number + "_vec_d"),
    m_pos_mod_identifier("osc" + p_osc_number + "_pos_mod"),
    m_oct_label("Oct"),
    m_semi_label("Semi"),
    m_fine_label("Fine"),
    m_vol_label("Vol"),
    m_drift_label("Drift"),
    m_pw_label("PW"),
    m_chip1_label("1"),
    m_chip2_label("2"),
    m_chip_speed_label("Speed"),
    m_wt_select_label("Wavetable"),
    m_wt_mod_label("Modulation"),
    m_wt_pos_label("WT-Position"),
    m_pos_mod(OdinKnob::Type::knob_8x8a),
    m_wt_amount_label("Amount"),
    m_detune_spread_label("WT-Spread"),
    m_detune_pos_label("WT-Position"),
    m_detune_label("Detune"),
    m_detune_wt_label("Wavetable"),
    m_vec_x_label("X"),
    m_vec_y_label("Y"),
    m_vec_a_label("A"),
    m_vec_b_label("B"),
    m_vec_c_label("C"),
    m_vec_d_label("D"),
    m_oct(OdinKnob::Type::knob_4x4a),
    m_semi(OdinKnob::Type::knob_4x4a),
    m_fine(OdinKnob::Type::knob_4x4a),
    m_vol(OdinKnob::Type::knob_5x5a),
    m_pw(OdinKnob::Type::knob_8x8a),
    m_drift(OdinKnob::Type::knob_8x8a),
    m_position(OdinKnob::Type::knob_8x8a),
    m_detune(OdinKnob::Type::knob_8x8a),
    m_spread(OdinKnob::Type::knob_5x5a),
    m_position_multi(OdinKnob::Type::knob_5x5a),
    m_xy_x(OdinKnob::Type::knob_5x5a),
    m_xy_y(OdinKnob::Type::knob_5x5a),
    m_vec_a(GlassDropdown::Type::dropdown_14x4),
    m_vec_b(GlassDropdown::Type::dropdown_14x4),
    m_vec_c(GlassDropdown::Type::dropdown_14x4),
    m_vec_d(GlassDropdown::Type::dropdown_14x4),
    m_step_1(OdinKnob::Type::knob_4x4a),
    m_step_2(OdinKnob::Type::knob_4x4a),
    m_step_3(OdinKnob::Type::knob_4x4a),
    m_speed(OdinKnob::Type::knob_8x8a),
    m_arp_label("ARPEGGIATOR"),
    m_step_label("Steps:"),
    m_fm(OdinKnob::Type::knob_8x8a),
    m_lp(OdinKnob::Type::knob_8x8a),
    m_hp(OdinKnob::Type::knob_8x8a) {

	addAndMakeVisible(m_HP_label);
	addAndMakeVisible(m_LP_label);
	addAndMakeVisible(m_carrier_label);
	addAndMakeVisible(m_modulator_label);
	addAndMakeVisible(m_FM_label);
	addAndMakeVisible(m_PM_label);
	addAndMakeVisible(m_exponential_label);
	addAndMakeVisible(m_linear_label);

	addAndMakeVisible(m_chip1_label);
	addAndMakeVisible(m_chip2_label);
	addAndMakeVisible(m_chip_speed_label);
	addAndMakeVisible(m_arp_label);
	addAndMakeVisible(m_step_label);

	addAndMakeVisible(m_wt_select_label);
	addAndMakeVisible(m_wt_mod_label);
	addAndMakeVisible(m_wt_pos_label);
	addAndMakeVisible(m_wt_amount_label);
	addAndMakeVisible(m_detune_spread_label);
	addAndMakeVisible(m_detune_pos_label);
	addAndMakeVisible(m_detune_label);
	addAndMakeVisible(m_detune_wt_label);
	addAndMakeVisible(m_vec_x_label);
	addAndMakeVisible(m_vec_y_label);
	addAndMakeVisible(m_vec_a_label);
	addAndMakeVisible(m_vec_b_label);
	addAndMakeVisible(m_vec_c_label);
	addAndMakeVisible(m_vec_d_label);

	m_linear_label.setJustification(Justification::centredLeft);
	m_exponential_label.setJustification(Justification::centredLeft);

	m_WT_container = p_processor.getWavetableContainerPointer();

	m_oct_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_oct", m_oct));
	m_semi_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_semi", m_semi));
	m_fine_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_fine", m_fine));
	m_vol_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_vol", m_vol));
	m_position_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_position", m_position));
	m_detune_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_detune", m_detune));
	m_pos_mod_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_pos_mod", m_pos_mod));
	m_multi_position_attach.reset(new OdinKnobAttachment(m_value_tree, "osc" + m_osc_number + "_multi_position", m_position_multi));
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
	m_step_3_on_attach.reset(new OdinButtonAttachment(m_value_tree, "osc" + m_osc_number + "_step_3_on", m_step_button));
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
	m_oct.setNumGuides(9);
	addChildComponent(m_oct);

	m_semi.setSliderStyle(Slider::RotaryVerticalDrag);
	m_semi.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_semi.setRange(-SEMI_RANGE_MAX, SEMI_RANGE_MAX);
	m_semi.setNumDecimalPlacesToDisplay(0);
	m_semi.setKnobTooltip("The pitch of\nthe oscillator in semitones");
	m_semi.setNumGuides(13);
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
	m_pw.setNumGuides(0);
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
	m_arp.onClick = [&]() { enableChipArpComponents(); };

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
	m_step_button.onClick = [&]() { enableChipArpComponents(); };

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

	m_wavedraw.onMouseUp = [&] { createWavedrawTables(); };
	m_chipdraw.onMouseUp = [&] { createChipdrawTables(); };
	m_specdraw.onMouseUp = [&] { createSpecdrawTables(); };

	juce::Colour chip_color(93, 81, 63);

	m_chiptune_waveselector.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("osc").setProperty(m_chipwave_identifier, (float)p_new_value, nullptr); };
	m_chiptune_waveselector.setTooltip("Selects the wave for the oscillator");
	addChildComponent(m_chiptune_waveselector);

	juce::PopupMenu fm_menu;
	juce::PopupMenu chip_menu;
	juce::PopupMenu vector_menu;
	juce::PopupMenu wavetable_submenu;
	juce::PopupMenu wavetable_recycle_menu;

#define ADD_WAVETABLE_SUB_MENU(name, number)                                                                                                                                       \
	wavetable_recycle_menu.clear();                                                                                                                                                \
	wavetable_recycle_menu.addItem(number + 1, (std::string)name + (std::string) " 1");                                                                                            \
	wavetable_recycle_menu.addItem(number + 2, (std::string)name + (std::string) " 2");                                                                                            \
	wavetable_recycle_menu.addItem(number + 3, (std::string)name + (std::string) " 3");                                                                                            \
	wavetable_recycle_menu.addItem(number + 4, (std::string)name + (std::string) " 4");                                                                                            \
	wavetable_submenu.addSubMenu(name, wavetable_recycle_menu);

#define ADD_MAP_ENTRY(name, number, selector)                                                                                                                                      \
	selector.addMapEntry((std::string)name + (std::string) " 1", number + 1, number + 2, number - 6);                                                                              \
	selector.addMapEntry((std::string)name + (std::string) " 2", number + 2, number + 3, number + 1);                                                                              \
	selector.addMapEntry((std::string)name + (std::string) " 3", number + 3, number + 4, number + 2);                                                                              \
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

	m_carrier_waveselector.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("osc").setProperty(m_carrier_wave_identifier, (float)p_new_value, nullptr); };

	m_carrier_waveselector.setTooltip("Selects the wave for the carrier osc");
	addChildComponent(m_carrier_waveselector);

	m_wavetable_waveselector.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("osc").setProperty(m_wavetable_identifier, (float)p_new_value, nullptr); };
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

	m_modulation_source.addWave(1, "ModEnvelope");
	m_modulation_source.addWave(10, "LFO 1");
	//just toggle for this one
	m_modulation_source.setIncrementValue(1, 10);
	m_modulation_source.setIncrementValue(10, 1);
	m_modulation_source.setDecrementValue(1, 10);
	m_modulation_source.setDecrementValue(10, 1);
	m_modulation_source.setTooltip("Select which source modulates the wavetable position");
	m_modulation_source.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("osc").setProperty(m_modulation_source_identifier, (float)p_new_value, nullptr);
	};
	addChildComponent(m_modulation_source);

	m_modulator_waveselector.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("osc").setProperty(m_modulator_wave_identifier, (float)p_new_value, nullptr);
	};
	m_modulator_waveselector.setTooltip("Selects the waveform for the modulator osc");
	addChildComponent(m_modulator_waveselector);

	m_carrier_ratio.OnValueChange = [&](int p_new_value) {
		//SETAUDIOFULLRANGE(m_carrier_ratio_identifier, p_new_value);
		m_value_tree.state.getChildWithName("osc").setProperty(m_carrier_ratio_identifier, p_new_value, nullptr);
	};
	m_carrier_ratio.setRange(1, 16);
	m_carrier_ratio.setTooltip("The pitch ratio of the carrier to base frequency");
	m_carrier_ratio.setMouseDragDivisor(20.f);

	addChildComponent(m_carrier_ratio);

	m_modulator_ratio.OnValueChange = [&](int p_new_value) {
		//SETAUDIOFULLRANGE(m_modulator_ratio_identifier, p_new_value);
		m_value_tree.state.getChildWithName("osc").setProperty(m_modulator_ratio_identifier, p_new_value, nullptr);
	};
	m_modulator_ratio.setRange(1, 16);
	m_modulator_ratio.setTooltip("The pitch ratio of the modulator to base frequency");
	m_modulator_ratio.setMouseDragDivisor(20.f);
	addChildComponent(m_modulator_ratio);

	m_fm_exp.onStateChange = [&]() {};
	m_fm_exp.setTooltip("Whether to use exponential or linear FM.\nExponential FM has a more "
	                    "musical character to it, as it preserves the perceived note pitch.");
	addChildComponent(m_fm_exp);

	m_chipdraw.setDrawColor(juce::Colour(238, 230, 217));
	m_chipdraw.setTooltip("Draw a custom 4Bit waveform.");
	addChildComponent(m_chipdraw);

	juce::Colour wave_color(71, 92, 108);
	juce::Colour wave_draw_color(99, 165, 236);

	m_wavedraw.setDrawColor(wave_draw_color);
	m_wavedraw.setTooltip("Become the Picasso of music production and draw your own waveform.");
	addChildComponent(m_wavedraw);

	juce::Colour spec_color(61, 80, 70);
	juce::Colour spec_draw_color(116, 180, 147);

	m_specdraw.setDrawColor(spec_draw_color);
	m_specdraw.setTooltip("Draw the spectrum of the oscillator. A single peak corresponds to a "
	                      "sine function.");
	addChildComponent(m_specdraw);

	juce::Colour vector_color(25, 50, 60);

	m_xy.setInlay(1);
	m_xy.setTooltip("Transition seamlessly through four waveforms. Each corner contains a "
	                "waveform, which can be selected by the dropdowns to the right.");
	addAndMakeVisible(m_xy);

	m_vec_a.setInlay(1);
	m_vec_a.setEditableText(false);
	m_vec_a.setSelectedId(101, dontSendNotification);
	m_vec_a.setTooltip("Select the waveform to the bottom left of the XY pad");
	m_vec_a.onChange = [&]() { m_value_tree.state.getChildWithName("osc").setProperty(m_vec_a_identifier, (float)m_vec_a.getSelectedId(), nullptr); };
	addChildComponent(m_vec_a);

	m_vec_b.setInlay(1);
	m_vec_b.setEditableText(false);
	m_vec_b.setSelectedId(102, dontSendNotification);
	m_vec_b.setTooltip("Select the waveform to the top left of the XY pad");
	m_vec_b.onChange = [&]() { m_value_tree.state.getChildWithName("osc").setProperty(m_vec_b_identifier, (float)m_vec_b.getSelectedId(), nullptr); };
	addChildComponent(m_vec_b);

	m_vec_c.setInlay(1);
	m_vec_c.setEditableText(false);
	m_vec_c.setSelectedId(103, dontSendNotification);
	m_vec_c.setTooltip("Select the waveform to the top right of the XY pad");
	m_vec_c.onChange = [&]() { m_value_tree.state.getChildWithName("osc").setProperty(m_vec_c_identifier, (float)m_vec_c.getSelectedId(), nullptr); };
	addChildComponent(m_vec_c);

	m_vec_d.setInlay(1);
	m_vec_d.setEditableText(false);
	m_vec_d.setSelectedId(104, dontSendNotification);
	m_vec_d.setTooltip("Select the waveform to the bottom right of the XY pad");
	m_vec_d.onChange = [&]() { m_value_tree.state.getChildWithName("osc").setProperty(m_vec_d_identifier, (float)m_vec_d.getSelectedId(), nullptr); };
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

	auto asset = UIAssets::Indices::Oscillator_Analog;
	switch (m_osc_type) {
	case 1:
		return;
		break;
	case OSC_TYPE_ANALOG:
		asset = UIAssets::Indices::Oscillator_Analog;
		break;
	case OSC_TYPE_WAVETABLE:
		asset = UIAssets::Indices::Oscillator_Wavetable;
		break;
	case OSC_TYPE_MULTI:
		asset = UIAssets::Indices::Oscillator_Multi;
		break;
	case OSC_TYPE_VECTOR:
		asset = UIAssets::Indices::Oscillator_Vector;
		break;
	case OSC_TYPE_CHIPTUNE:
		asset = UIAssets::Indices::Oscillator_Chiptune;
		break;
	case OSC_TYPE_FM:
		asset = UIAssets::Indices::Oscillator_FM;
		break;
	case OSC_TYPE_PM:
		asset = UIAssets::Indices::Oscillator_PM;
		break;
	case OSC_TYPE_NOISE:
		asset = UIAssets::Indices::Oscillator_Noise;
		break;
	case OSC_TYPE_WAVEDRAW:
		asset = UIAssets::Indices::Oscillator_WaveDraw;
		break;
	case OSC_TYPE_CHIPDRAW:
		asset = UIAssets::Indices::Oscillator_ChipDraw;
		break;
	case OSC_TYPE_SPECDRAW:
		asset = UIAssets::Indices::Oscillator_SpecDraw;
		break;
	default:
		return;
		break;
	}

	auto background = UIAssetManager::getInstance()->getUIAsset(asset, ConfigFileManager::getInstance().getOptionGuiScale());
	jassert(background.getWidth() == getWidth());
	jassert(background.getHeight() == getHeight());
	g.drawImageAt(background, 0, 0);
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

	resized();
	repaint();
}

void OscComponent::setOscTypeBypass() {
	m_background = m_background_bypass;
}

void OscComponent::setOscTypeAnalog() {
	showAnalogComponents();
}
void OscComponent::setOscTypeChiptune() {
	showChiptuneComponents();
}

void OscComponent::setOscTypeFM() {
	showFMComponents();
}

void OscComponent::setOscTypePM() {
	showPMComponents();
}

void OscComponent::setOscTypeChipdraw() {
	showChipdrawComponents();
}

void OscComponent::setOscTypeWavedraw() {
	showWavedrawComponents();
}

void OscComponent::setOscTypeSpecdraw() {
	showSpecdrawComponents();
}

void OscComponent::setOscTypeNoise() {
	showNoiseComponents();
}

void OscComponent::setOscTypeVector() {
	showVectorComponents();
}

void OscComponent::setOscTypeWavetable() {
	showWavetableComponents();
}

void OscComponent::setOscTypeMulti() {
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
	m_wavedraw.setVisible(false);
	m_specdraw.setVisible(false);
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
	m_oct_label.setVisible(false);
	m_semi_label.setVisible(false);
	m_fine_label.setVisible(false);
	m_vol_label.setVisible(false);
	m_drift_label.setVisible(false);
	m_pw_label.setVisible(false);
	m_wt_select_label.setVisible(false);
	m_wt_mod_label.setVisible(false);
	m_wt_pos_label.setVisible(false);
	m_wt_amount_label.setVisible(false);
	m_detune_spread_label.setVisible(false);
	m_detune_pos_label.setVisible(false);
	m_detune_label.setVisible(false);
	m_detune_wt_label.setVisible(false);
	m_vec_x_label.setVisible(false);
	m_vec_y_label.setVisible(false);
	m_vec_a_label.setVisible(false);
	m_vec_b_label.setVisible(false);
	m_vec_c_label.setVisible(false);
	m_vec_d_label.setVisible(false);
	m_chip1_label.setVisible(false);
	m_chip2_label.setVisible(false);
	m_chip_speed_label.setVisible(false);
	m_arp_label.setVisible(false);
	m_step_label.setVisible(false);
	m_carrier_label.setVisible(false);
	m_modulator_label.setVisible(false);
	m_FM_label.setVisible(false);
	m_PM_label.setVisible(false);
	m_exponential_label.setVisible(false);
	m_linear_label.setVisible(false);
	m_HP_label.setVisible(false);
	m_LP_label.setVisible(false);
}
void OscComponent::showVolComponent() {
	m_vol.setVisible(true);
	m_vol_label.setVisible(true);
}

void OscComponent::showPitchComponents() {
	m_oct.setVisible(true);
	m_semi.setVisible(true);
	m_fine.setVisible(true);
	m_reset.setVisible(true);
	m_oct_label.setVisible(true);
	m_semi_label.setVisible(true);
	m_fine_label.setVisible(true);
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
	m_drift_label.setVisible(true);
	m_sync.setVisible(true);
	m_pw_label.setVisible(true);
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
	m_chip1_label.setVisible(true);
	m_chip2_label.setVisible(true);
	m_chip_speed_label.setVisible(true);
	m_arp_label.setVisible(true);
	m_step_label.setVisible(true);
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

	m_carrier_label.setVisible(true);
	m_modulator_label.setVisible(true);
	m_FM_label.setVisible(true);
	m_exponential_label.setVisible(true);
	m_linear_label.setVisible(true);
}

void OscComponent::showPMComponents() {
	showVolComponent();
	showPitchComponents();
	m_carrier_waveselector.setVisible(true);
	m_modulator_waveselector.setVisible(true);
	m_carrier_ratio.setVisible(true);
	m_modulator_ratio.setVisible(true);
	m_fm.setVisible(true);
	m_carrier_label.setVisible(true);
	m_modulator_label.setVisible(true);
	m_PM_label.setVisible(true);
}

void OscComponent::showChipdrawComponents() {
	showVolComponent();
	showPitchComponents();
	m_chipdraw.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showWavedrawComponents() {
	showVolComponent();
	showPitchComponents();
	m_wavedraw.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showSpecdrawComponents() {
	showVolComponent();
	showPitchComponents();
	m_specdraw.setVisible(true);
	m_sync.setVisible(true);
}

void OscComponent::showVectorComponents() {
	showVolComponent();
	showPitchComponents();
	m_vec_x_label.setVisible(true);
	m_vec_y_label.setVisible(true);
	m_vec_a_label.setVisible(true);
	m_vec_b_label.setVisible(true);
	m_vec_c_label.setVisible(true);
	m_vec_d_label.setVisible(true);
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
	m_wt_select_label.setVisible(true);
	m_wt_mod_label.setVisible(true);
	m_wt_pos_label.setVisible(true);
	m_wt_amount_label.setVisible(true);
	m_position.setVisible(true);
	m_wavetable_waveselector.setVisible(true);
	m_sync.setVisible(true);
	m_pos_mod.setVisible(true);
	m_modulation_source.setVisible(true);
}

void OscComponent::showMultiComponents() {
	showVolComponent();
	showPitchComponents();
	m_detune_spread_label.setVisible(true);
	m_detune_pos_label.setVisible(true);
	m_detune_label.setVisible(true);
	m_detune_wt_label.setVisible(true);
	m_position_multi.setVisible(true);
	m_spread.setVisible(true);
	m_detune.setVisible(true);
	m_wavetable_waveselector.setVisible(true);
}

void OscComponent::showNoiseComponents() {
	showVolComponent();
	m_lp.setVisible(true);
	m_hp.setVisible(true);
	m_HP_label.setVisible(true);
	m_LP_label.setVisible(true);
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
	m_chiptune_waveselector.setValue(m_value_tree.state.getChildWithName("osc")[String("osc" + m_osc_number + "_chipwave")]);
	auto node = m_value_tree.state;

	node = m_value_tree.state.getChildWithName("draw");

	// wavedraw
	float wavedraw_values[WAVEDRAW_STEPS_X];
	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
		wavedraw_values[i] = (float)node[String("osc" + m_osc_number + "_wavedraw_values_" + std::to_string(i))];
	}

	m_wavedraw.setDrawnTable(wavedraw_values);

	// chipdraw
	for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
		wavedraw_values[i] = (float)node[String("osc" + m_osc_number + "_chipdraw_values_" + std::to_string(i))];
	}
	m_chipdraw.setDrawnTable(wavedraw_values);

	// specdraw
	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		wavedraw_values[i] = (float)node[String("osc" + m_osc_number + "_specdraw_values_" + std::to_string(i))];
	}
	m_specdraw.setDrawnTable(wavedraw_values);

	if (p_create_wavetables) {
		createWavedrawTables();
		createChipdrawTables();
		createSpecdrawTables();
	}

	enableChipArpComponents();
}

void OscComponent::resized() {

	GET_LOCAL_AREA(m_wt_select_label, "OscWtSelectLabel");
	GET_LOCAL_AREA(m_wt_mod_label, "OscWtModLabel");
	GET_LOCAL_AREA(m_wt_pos_label, "OscWtPosLabel");
	GET_LOCAL_AREA(m_wt_amount_label, "OscWtAmountLabel");
	GET_LOCAL_AREA(m_detune_spread_label, "OscDetuneSpreadLabel");
	GET_LOCAL_AREA(m_detune_pos_label, "OscDetunePosLabel");
	GET_LOCAL_AREA(m_detune_label, "OscDetuneLabel");
	GET_LOCAL_AREA(m_detune_wt_label, "OscDetuneWtLabel");
	GET_LOCAL_AREA(m_vec_x_label, "OscVecXLabel");
	GET_LOCAL_AREA(m_vec_y_label, "OscVecYLabel");
	GET_LOCAL_AREA(m_vec_a_label, "OscVecALabel");
	GET_LOCAL_AREA(m_vec_b_label, "OscVecBLabel");
	GET_LOCAL_AREA(m_vec_c_label, "OscVecCLabel");
	GET_LOCAL_AREA(m_vec_d_label, "OscVecDLabel");
	GET_LOCAL_AREA(m_chip1_label, "Chip1Label");
	GET_LOCAL_AREA(m_chip2_label, "Chip2Label");
	GET_LOCAL_AREA(m_chip_speed_label, "ChipSpeedLabel");
	GET_LOCAL_AREA(m_arp_label, "ChipArpLabel");
	GET_LOCAL_AREA(m_step_label, "ChipStepLabel");
	GET_LOCAL_AREA(m_carrier_label, "CarrierLabel");
	GET_LOCAL_AREA(m_modulator_label, "ModulatorLabel");
	GET_LOCAL_AREA(m_exponential_label, "FmExpLabel");
	GET_LOCAL_AREA(m_linear_label, "FmLinLabel");
	GET_LOCAL_AREA(m_FM_label, "FmLabel");
	GET_LOCAL_AREA(m_PM_label, "FmLabel");
	GET_LOCAL_AREA(m_HP_label, "NoiseHpLabel");
	GET_LOCAL_AREA(m_LP_label, "NoiseLpLabel");

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

	// note: the 4x4 arp knobs were faultily placed at 0.5 grid cells now we need to align the step button below
	// by offsetting it half a grid cell to the left
	const auto grid = ConfigFileManager::getInstance().getOptionGuiScale();
	GET_LOCAL_AREA(m_step_button, "OscStep");
	m_step_button.setBounds(m_step_button.getBounds().translated(-grid / 2, 0));

	GET_LOCAL_AREA(m_step_1, "OscStep1");
	GET_LOCAL_AREA(m_step_2, "OscStep2");
	GET_LOCAL_AREA(m_step_3, "OscStep3");
	GET_LOCAL_AREA(m_fm, "OscFM");
	GET_LOCAL_AREA(m_lp, "OscLP");
	GET_LOCAL_AREA(m_hp, "OscHP");
	GET_LOCAL_AREA(m_position, "OscPos");
	GET_LOCAL_AREA(m_pos_mod, "OscPosMod");
	GET_LOCAL_AREA(m_detune, "OscDetune");
	GET_LOCAL_AREA(m_position_multi, "OscPositionMulti");
	GET_LOCAL_AREA(m_spread, "OscSpread");
	GET_LOCAL_AREA(m_speed, "OscSpeed");
	GET_LOCAL_AREA(m_modulator_ratio, "OscModRatio");
	GET_LOCAL_AREA(m_carrier_ratio, "OscCarrRatio");
	GET_LOCAL_AREA(m_fm_exp, "OscFmExp");

	GET_LOCAL_AREA(m_chiptune_waveselector, "ChipWave");
	GET_LOCAL_AREA(m_carrier_waveselector, "CarrWave");
	GET_LOCAL_AREA(m_modulator_waveselector, "ModWave");
	GET_LOCAL_AREA(m_modulation_source, "OscModSource");

	GET_LOCAL_AREA(m_chipdraw, "Wavedraw");
	GET_LOCAL_AREA(m_wavedraw, "Wavedraw");
	GET_LOCAL_AREA(m_specdraw, "Wavedraw");

	GET_LOCAL_AREA(m_modulation_source, "OscModSource");

	GET_LOCAL_AREA(m_xy, "OscXy");
	GET_LOCAL_AREA(m_xy_x, "OscXyX");
	GET_LOCAL_AREA(m_xy_y, "OscXyY");

	GET_LOCAL_AREA(m_vec_a, "OscVecA");
	GET_LOCAL_AREA(m_vec_b, "OscVecB");
	GET_LOCAL_AREA(m_vec_c, "OscVecC");
	GET_LOCAL_AREA(m_vec_d, "OscVecD");

	if (m_osc_type == OSC_TYPE_MULTI) {
		GET_LOCAL_AREA(m_wavetable_waveselector, "OscWaveMulti");
	} else {
		GET_LOCAL_AREA(m_wavetable_waveselector, "OscWaveWT");
	}
}

void OscComponent::enableChipArpComponents() {
	const auto arp_enabled = m_arp.getToggleState();

	m_step_1.setEnabled(arp_enabled);
	m_step_2.setEnabled(arp_enabled);
	m_step_label.setEnabled(arp_enabled);
	m_chip1_label.setEnabled(arp_enabled);
	m_chip2_label.setEnabled(arp_enabled);
	m_step_button.setEnabled(arp_enabled);
	m_speed.setEnabled(arp_enabled);

	m_step_3.setEnabled(arp_enabled && m_step_button.getToggleState());
}