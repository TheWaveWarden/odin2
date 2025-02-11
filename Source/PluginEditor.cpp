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

#include "PluginEditor.h"
#include "ConfigFileManager.h"
#include "PluginProcessor.h"
#include "gui/UIAssetManager.h"

//==============================================================================
OdinEditor::OdinEditor(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts, bool p_is_standalone) :
    AudioProcessorEditor(&p_processor),
    m_live_constrainer(*this),
    processor(p_processor),
    m_fx_buttons_section(vts, p_processor),
    m_value_tree(vts),
    m_detune_label("Detune"),
    m_width_label("Width"),
    m_master_label("Master"),
    m_master(OdinKnob::Type::knob_4x4a),
    m_unison_detune(OdinKnob::Type::knob_4x4a),
    m_unison_width(OdinKnob::Type::knob_4x4a),
    m_glide_label("Glide"),
    m_osc1_dropdown("osc1_dropdown_button", "", OdinButton::Type::button_dropdown),
    m_osc2_dropdown("osc2_dropdown_button", "", OdinButton::Type::button_dropdown),
    m_osc3_dropdown("osc3_dropdown_button", "", OdinButton::Type::button_dropdown),
    m_filter1_dropdown("filter1_dropdown_button", "", OdinButton::Type::button_dropdown),
    m_filter2_dropdown("filter2_dropdown_button", "", OdinButton::Type::button_dropdown),
    m_filter3_dropdown("filter3_dropdown_button", "", OdinButton::Type::button_dropdown),
    m_filleft_button1("filter_left_button1", "1", OdinButton::Type::button_5x5),
    m_filleft_button2("filter_left_button2", "2", OdinButton::Type::button_5x5),
    m_filleft_button3("filter_left_button3", "3", OdinButton::Type::button_5x5),
    m_filright_button1("filter_right_button1", "1", OdinButton::Type::button_5x5),
    m_filright_button2("filter_right_button2", "2", OdinButton::Type::button_5x5),
    m_filright_button3("filter_right_button3", "3", OdinButton::Type::button_5x5),
    m_filright_buttonf1("filter_left_buttonf1", "F1", OdinButton::Type::button_5x5),
    m_flanger_on_button("flanger_button", "", OdinButton::Type::fx_enabled),
    m_phaser_on_button("phaser_button", "", OdinButton::Type::fx_enabled),
    m_chorus_on_button("chorus_button", "", OdinButton::Type::fx_enabled),
    m_delay_on_button("delay_button", "", OdinButton::Type::fx_enabled),
    m_reverb_on_button("reverb_button", "", OdinButton::Type::fx_enabled),
    m_burger_button("burger_button", "", OdinButton::Type::burger),
    m_select_arp_button("select_arpeggiator_button", "Arpeggiator", OdinButton::Type::button_41x5),
    m_select_modmatrix_button("select_modmatrix_button", "ModMatrix", OdinButton::Type::button_40x5),
    m_select_presets_button("select_presets_button", "Presets", OdinButton::Type::button_41x5),
    m_reset("reset", "Reset", OdinButton::Type::button_9x4),
    m_env_1_button("env1_button", "Amp Env", OdinButton::Type::button_13x4EnvLFO),
    m_env_2_button("env2_button", "Filter Env", OdinButton::Type::button_13x4EnvLFO),
    m_env_3_button("env3_button", "Mod Env", OdinButton::Type::button_13x4EnvLFO),
    m_env_4_button("env4_button", "Global Env", OdinButton::Type::button_13x4EnvLFO),
    m_lfo_1_button("lfo1_button", "LFO 1", OdinButton::Type::button_13x4EnvLFO),
    m_lfo_2_button("lfo2_button", "LFO 2", OdinButton::Type::button_13x4EnvLFO),
    m_lfo_3_button("lfo3_button", "LFO 3", OdinButton::Type::button_13x4EnvLFO),
    m_lfo_4_button("lfo4_button", "Global LFO", OdinButton::Type::button_13x4EnvLFO),
    m_pitch_amount(true, NumberSelector::Type::selector_8x4),
    m_osc1(p_processor, vts, "1"),
    m_osc2(p_processor, vts, "2"),
    m_osc3(p_processor, vts, "3"),
    m_fil1_component(vts, "1"),
    m_fil2_component(vts, "2"),
    m_fil3_component(vts, "3"),
    m_midsection(vts),
    m_adsr_1(vts, "1"),
    m_adsr_2(vts, "2"),
    m_adsr_3(vts, "3"),
    m_adsr_4(vts, "4"),
    m_lfo_1(vts, "1", p_is_standalone),
    m_lfo_2(vts, "2", p_is_standalone),
    m_lfo_3(vts, "3", p_is_standalone),
    m_lfo_4(vts, "4", p_is_standalone),
    m_delay(vts, p_is_standalone),
    m_reverb(vts, p_is_standalone),
    m_phaser(vts, "phaser", p_is_standalone),
    m_flanger(vts, "flanger", p_is_standalone, FXComponent::Type::flanger),
    m_chorus(vts, "chorus", p_is_standalone, FXComponent::Type::chorus),
    m_xy_section(vts, "xy"),
    m_osc1_type_identifier("osc1_type"),
    m_osc2_type_identifier("osc2_type"),
    m_osc3_type_identifier("osc3_type"),
    m_fil1_type_identifier("fil1_type"),
    m_fil2_type_identifier("fil2_type"),
    m_fil3_type_identifier("fil3_type"),
    m_pitchbend_amount_identifier("pitchbend_amount"),
    m_unison_voices_identifier("unison_voices"),
    m_delay_position_identifier("delay_position"),
    m_flanger_position_identifier("flanger_position"),
    m_phaser_position_identifier("phaser_position"),
    m_chorus_position_identifier("chorus_position"),
    m_reverb_position_identifier("reverb_position"),
    m_mod_matrix(vts),
    m_tooltip(nullptr, 2047483647),
    m_is_standalone_plugin(p_is_standalone),
    /*m_save_load(vts, p_processor),*/ m_arp(p_processor, vts),
    m_processor(p_processor),
    m_patch_browser(p_processor, vts),
    m_tuning(p_processor),
    m_mono_poly_legato_dropdown(GlassDropdown::Type::dropdown_14x4),
    m_modwheel(OdinKnob::Type::wheel),
    m_glide(OdinKnob::Type::knob_6x6b) {

	setResizable(false, false);

#ifdef ODIN_MAC
	setBufferedToImage(true);
#endif

	if (m_is_standalone_plugin) {
		addKeyListener(this);
	}

	//these functions need to be deleted in the destructor, since after editor
	//destruction the pointers are invalid
	p_processor.onSetStateInformation = [&]() { forceValueTreeOntoComponents(true); };
	p_processor.updatePitchWheelGUI   = [&](float p_value) { updatePitchWheel(p_value); };
	p_processor.updateModWheelGUI     = [&](float p_value) { updateModWheel(p_value); };

	m_patch_browser.forceValueTreeLambda = [&]() { forceValueTreeOntoComponents(true); };

	m_reset.onClick = [&]() {
		if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon,
		                                 "Reset Synth",
		                                 "This will reset the synth to its initial state and you will lose "
		                                 "your patch!",
		                                 {},
		                                 {},
		                                 {})) {

			// replace stream with patch from binary data
			MemoryInputStream init_stream(BinaryData::init_patch_odin, BinaryData::init_patch_odinSize, false);
			processor.readPatch(ValueTree::readFromStream(init_stream));

			//reset pitchbend and modwheel, since they are not loaded with patches
			SETAUDIOFULLRANGESAFE("modwheel", 0.f);
			SETAUDIOFULLRANGESAFE("pitchbend", 0.f);

			//force modmatrix to show
			m_value_tree.state.getChildWithName("misc").setProperty("arp_mod_selected", MATRIX_SECTION_INDEX_MATRIX, nullptr);

			//this forces values onto the GUI (patch label as well)
			forceValueTreeOntoComponents(true);

			DBG("Loaded init patch");
		}
	};

	OdinKnob::setOdinPointer(&p_processor);
	DrawableSlider::setOdinPointer(&p_processor);
	LeftRightButton::setOdinPointer(&p_processor);
	OdinButton::setOdinPointer(&p_processor);

	m_osc_dropdown_menu.addItem(1, "None");
	m_osc_dropdown_menu.addSeparator();
	m_osc_dropdown_menu.addItem(OSC_TYPE_ANALOG, "Analog Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_WAVETABLE, "Wavetable Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_MULTI, "Multi Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_VECTOR, "Vector Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_CHIPTUNE, "Chiptune Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_FM, "FM Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_PM, "PhaseMod Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_NOISE, "Noise Oscillator");
	m_osc_dropdown_menu.addSeparator();
	m_osc_dropdown_menu.addItem(OSC_TYPE_WAVEDRAW, "Wavedraw Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_CHIPDRAW, "Chipdraw Oscillator");
	m_osc_dropdown_menu.addItem(OSC_TYPE_SPECDRAW, "Specdraw Oscillator");

	m_filter_dropdown_menu.addItem(1, "Bypass");
	m_filter_dropdown_menu.addSeparator();
	m_filter_dropdown_menu.addItem(FILTER_TYPE_LP24, "Lowpass 24");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_LP12, "Lowpass 12");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_BP24, "Bandpass 24");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_BP12, "Bandpass 12");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_HP24, "Highpass 24");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_HP12, "Highpass 12");
	m_filter_dropdown_menu.addSeparator();
	m_filter_dropdown_menu.addItem(FILTER_TYPE_SEM12, "SEM-12");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_DIODE, "Diode Ladder");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_KORG_LP, "KRG-35 LP");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_KORG_HP, "KRG-35 HP");
	m_filter_dropdown_menu.addSeparator();
	m_filter_dropdown_menu.addItem(FILTER_TYPE_COMB, "Comb Filter");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_FORMANT, "Formant Filter");
	m_filter_dropdown_menu.addSeparator();
	m_filter_dropdown_menu.addItem(FILTER_TYPE_RINGMOD, "Ring Modulator");

	m_osc1_dropdown.setClickingTogglesState(true);

	m_osc1_dropdown.setToggleState(true, dontSendNotification);
	m_osc1_dropdown.setTriggeredOnMouseDown(false);
	m_osc1_dropdown.onClick = [&]() {
		m_osc1_dropdown.setState(Button::ButtonState::buttonNormal);
		m_osc1_dropdown.setToggleState(false, dontSendNotification);
		setOsc1Plate(m_osc_dropdown_menu.show());
	};
	addAndMakeVisible(m_title_button);

	addAndMakeVisible(m_detune_label);
	addAndMakeVisible(m_width_label);
	addAndMakeVisible(m_master_label);
	addAndMakeVisible(m_glide_label);

	addAndMakeVisible(m_osc1_dropdown);
	m_osc1_dropdown.setTooltip("Select the module to\nbe used for oscillator 1");
	m_osc1_dropdown.setState(Button::ButtonState::buttonNormal);
	m_osc1_dropdown.setToggleState(false, dontSendNotification);
	m_osc1_dropdown.setClickingTogglesState(true);

	m_osc2_dropdown.setToggleState(true, dontSendNotification);
	m_osc2_dropdown.setTriggeredOnMouseDown(false);
	m_osc2_dropdown.onClick = [&]() {
		m_osc2_dropdown.setState(Button::ButtonState::buttonNormal);
		m_osc2_dropdown.setToggleState(false, dontSendNotification);
		setOsc2Plate(m_osc_dropdown_menu.show());
	};
	m_osc2_dropdown.setTooltip("Select the module to\nbe used for oscillator 2");
	addAndMakeVisible(m_osc2_dropdown);
	m_osc2_dropdown.setState(Button::ButtonState::buttonNormal);
	m_osc2_dropdown.setToggleState(false, dontSendNotification);

	m_osc3_dropdown.setClickingTogglesState(true);
	m_osc3_dropdown.setToggleState(true, dontSendNotification);
	m_osc3_dropdown.setTriggeredOnMouseDown(false);
	m_osc3_dropdown.onClick = [&]() {
		m_osc3_dropdown.setState(Button::ButtonState::buttonNormal);
		m_osc3_dropdown.setToggleState(false, dontSendNotification);
		setOsc3Plate(m_osc_dropdown_menu.show());
	};
	m_osc3_dropdown.setTooltip("Select the module to\nbe used for oscillator 3");
	addAndMakeVisible(m_osc3_dropdown);
	m_osc3_dropdown.setState(Button::ButtonState::buttonNormal);
	m_osc3_dropdown.setToggleState(false, dontSendNotification);

	m_filter1_dropdown.setClickingTogglesState(true);
	m_filter1_dropdown.setToggleState(true, dontSendNotification);
	m_filter1_dropdown.setTriggeredOnMouseDown(false);
	m_filter1_dropdown.onClick = [&]() {
		m_filter1_dropdown.setState(Button::ButtonState::buttonNormal);
		m_filter1_dropdown.setToggleState(false, dontSendNotification);
		setFilter1Plate(m_filter_dropdown_menu.show());
	};
	m_filter1_dropdown.setTooltip("Select the module to\nbe used for filter 1");
	addAndMakeVisible(m_filter1_dropdown);
	m_filter1_dropdown.setState(Button::ButtonState::buttonNormal);
	m_filter1_dropdown.setToggleState(false, dontSendNotification);

	m_filter2_dropdown.setClickingTogglesState(true);
	m_filter2_dropdown.setToggleState(true, dontSendNotification);
	m_filter2_dropdown.setTriggeredOnMouseDown(false);
	m_filter2_dropdown.onClick = [&]() {
		m_filter2_dropdown.setState(Button::ButtonState::buttonNormal);
		m_filter2_dropdown.setToggleState(false, dontSendNotification);
		setFilter2Plate(m_filter_dropdown_menu.show());
	};
	m_filter2_dropdown.setTooltip("Select the module to\nbe used for filter 2");
	addAndMakeVisible(m_filter2_dropdown);
	m_filter2_dropdown.setState(Button::ButtonState::buttonNormal);
	m_filter2_dropdown.setToggleState(false, dontSendNotification);

	m_filter3_dropdown.setClickingTogglesState(true);
	m_filter3_dropdown.setToggleState(true, dontSendNotification);
	m_filter3_dropdown.setTriggeredOnMouseDown(false);
	m_filter3_dropdown.onClick = [&]() {
		m_filter3_dropdown.setState(Button::ButtonState::buttonNormal);
		m_filter3_dropdown.setToggleState(false, dontSendNotification);
		setFilter3Plate(m_filter_dropdown_menu.show());
	};
	m_filter3_dropdown.setTooltip("Select the module to\nbe used for filter 3");
	addAndMakeVisible(m_filter3_dropdown);
	m_filter3_dropdown.setState(Button::ButtonState::buttonNormal);
	m_filter3_dropdown.setToggleState(false, dontSendNotification);

	m_fx_buttons_section.onButtonArrange = [&](std::map<std::string, int> p_map) { arrangeFXOnButtons(p_map); };
	m_fx_buttons_section.setHighlighted  = [&](const std::string &p_name) { setActiveFXPanel(p_name); };
	addAndMakeVisible(m_fx_buttons_section);

	m_phaser_on_button.setClickingTogglesState(true);
	addAndMakeVisible(m_phaser_on_button);
	m_phaser_on_button.setTriggeredOnMouseDown(true);
	m_phaser_on_button.setTooltip("Enables the phaser module");
	m_phaser_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_flanger_on_button.setClickingTogglesState(true);
	addAndMakeVisible(m_flanger_on_button);
	m_flanger_on_button.setTriggeredOnMouseDown(true);
	m_flanger_on_button.setTooltip("Enables the flanger module");
	m_flanger_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_chorus_on_button.setClickingTogglesState(true);
	addAndMakeVisible(m_chorus_on_button);
	m_chorus_on_button.setTriggeredOnMouseDown(true);
	m_chorus_on_button.setTooltip("Enables the chorus module");
	m_chorus_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_delay_on_button.setClickingTogglesState(true);
	addAndMakeVisible(m_delay_on_button);
	m_delay_on_button.setTriggeredOnMouseDown(true);
	m_delay_on_button.setTooltip("Enables the delay module");
	m_delay_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_reverb_on_button.setClickingTogglesState(true);
	addAndMakeVisible(m_reverb_on_button);
	m_reverb_on_button.setTriggeredOnMouseDown(true);
	m_reverb_on_button.setTooltip("Enables the reverb module");
	m_reverb_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_reset.setTooltip("Reset the synth to its initial state");
	addAndMakeVisible(m_reset);
	m_reset.setTriggeredOnMouseDown(false);
	m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_filleft_button1.setClickingTogglesState(true);
	addAndMakeVisible(m_filleft_button1);
	m_filleft_button1.setTriggeredOnMouseDown(true);
	m_filleft_button1.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_filleft_button1.setTooltip("Routes oscillator 1\ninto the filter");

	m_filright_button1.setClickingTogglesState(true);
	addAndMakeVisible(m_filright_button1);
	m_filright_button1.setTriggeredOnMouseDown(true);
	m_filright_button1.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_filright_button1.setTooltip("Routes oscillator 1\ninto the filter");

	addAndMakeVisible(m_burger_button);
	m_burger_button.onClick = [&]() {
		// open popup menu
		PopupMenu menu;
		PopupMenu zoomMenu;
		const auto current_zoom = ConfigFileManager::getInstance().getOptionGuiScale();
		zoomMenu.addItem(int(GuiScale::Z100), (current_zoom == int(GuiScale::Z100) ? juce::String("* ") : juce::String("")) + juce::String("100%"));
		zoomMenu.addItem(int(GuiScale::Z125), (current_zoom == int(GuiScale::Z125) ? juce::String("* ") : juce::String("")) + juce::String("125%"));
		zoomMenu.addItem(int(GuiScale::Z150), (current_zoom == int(GuiScale::Z150) ? juce::String("* ") : juce::String("")) + juce::String("150%"));
		zoomMenu.addItem(int(GuiScale::Z175), (current_zoom == int(GuiScale::Z175) ? juce::String("* ") : juce::String("")) + juce::String("175%"));
		zoomMenu.addItem(int(GuiScale::Z200), (current_zoom == int(GuiScale::Z200) ? juce::String("* ") : juce::String("")) + juce::String("200%"));

		menu.addSubMenu("Zoom", zoomMenu);
		menu.addSeparator();
		if (ConfigFileManager::getInstance().getOptionShowTooltip())
			menu.addItem(1000, "Hide Tooltips");
		else
			menu.addItem(1000, "Show Tooltips");

		menu.addSeparator(), menu.addItem(1050, "Open Main Storage Path");

#ifdef ODIN_DEBUG
		menu.addSeparator();
		menu.addItem(1100, "Save GuiData.json");
#endif

		const auto ret = menu.show();

		if (ret == 0)
			return;

		if (ret <= int(GuiScale::Z200)) {
			const auto threadWasStarted = UIAssetManager::getInstance()->launchImageCreationThreads(ret);
			if (!threadWasStarted)
				setGuiSize((GuiScale)(ret), true);
			return;
		}

		if (ret == 1000) {
			const auto new_tooltip_state = !ConfigFileManager::getInstance().getOptionShowTooltip();
			ConfigFileManager::getInstance().setOptionShowTooltip(new_tooltip_state);
			ConfigFileManager::getInstance().saveDataToFile();
			setTooltipEnabled(new_tooltip_state);
			return;
		}

		if (ret == 1050) {
			juce::URL(ODIN_STORAGE_PATH).launchInDefaultBrowser();
			return;
		}

		if (ret == 1100) {
			JsonGuiProvider::getInstance().saveToFile();
			return;
		}
	};

	addAndMakeVisible(m_tuning);

	m_filleft_button2.setClickingTogglesState(true);
	addAndMakeVisible(m_filleft_button2);
	m_filleft_button2.setTooltip("Routes oscillator 2 into the filter");
	m_filleft_button2.setTriggeredOnMouseDown(true);
	m_filleft_button2.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_filright_button2.setClickingTogglesState(true);
	addAndMakeVisible(m_filright_button2);
	m_filright_button2.setTooltip("Routes oscillator 2 into the filter");
	m_filright_button2.setTriggeredOnMouseDown(true);
	m_filright_button2.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_filleft_button3.setClickingTogglesState(true);
	m_filleft_button3.setTooltip("Routes oscillator 3 into the filter");
	addAndMakeVisible(m_filleft_button3);
	m_filleft_button3.setTriggeredOnMouseDown(true);
	m_filleft_button3.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_filright_button3.setClickingTogglesState(true);
	addAndMakeVisible(m_filright_button3);
	m_filright_button3.setTooltip("Routes oscillator 3\ninto the filter");
	m_filright_button3.setTriggeredOnMouseDown(true);
	m_filright_button3.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_filright_buttonf1.setClickingTogglesState(true);
	addAndMakeVisible(m_filright_buttonf1);
	m_filright_buttonf1.setTooltip("Routes filter 1\ninto filter 2");
	m_filright_buttonf1.setTriggeredOnMouseDown(true);
	m_filright_buttonf1.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_glide.setSliderStyle(Slider::RotaryVerticalDrag);
	m_glide.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_glide.setKnobTooltip("Makes the pitch glide\nfrom the\nlast key pressed to\nthe current one");
	addAndMakeVisible(m_glide);

	m_master.setSliderStyle(Slider::RotaryVerticalDrag);
	m_master.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_master.setKnobTooltip("The master volume\nof the synth");
	addAndMakeVisible(m_master);

	m_unison_detune.setSliderStyle(Slider::RotaryVerticalDrag);
	m_unison_detune.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_unison_detune.setKnobTooltip("Maximum detune in unison, measured in semitones");
	addAndMakeVisible(m_unison_detune);
	m_unison_width.setSliderStyle(Slider::RotaryVerticalDrag);
	m_unison_width.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_unison_width.setKnobTooltip("Spreads the unison voices over the stereo field");
	addAndMakeVisible(m_unison_width);

	m_pitchwheel.setSliderStyle(Slider::RotaryVerticalDrag);
	m_pitchwheel.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_pitchwheel.setTooltip("Bends the pitch of all\noscillators up or down.\nYou can specify the "
	                        "amount\nby the selector below");
	addAndMakeVisible(m_pitchwheel);

	m_modwheel.setSliderStyle(Slider::RotaryVerticalDrag);
	m_modwheel.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_modwheel.setTooltip("Modwheel to be used\nin the modulation matrix");
	addAndMakeVisible(m_modwheel);

	m_osc1.setOscType(OSC_TYPE_ANALOG); // analog
	addAndMakeVisible(m_osc1);
	m_osc2.setOscType(1); // bypass
	addAndMakeVisible(m_osc2);
	m_osc3.setOscType(1); // bypass
	addAndMakeVisible(m_osc3);

	m_fil1_component.setFilterType(FILTER_TYPE_LP24); // LP
	addAndMakeVisible(m_fil1_component);
	m_fil2_component.setFilterType(1); // bypass
	addAndMakeVisible(m_fil2_component);
	m_fil3_component.setFilterType(1); // bypass
	addAndMakeVisible(m_fil3_component);

	addAndMakeVisible(m_midsection);
	addAndMakeVisible(m_adsr_1);
	addAndMakeVisible(m_adsr_2);
	addChildComponent(m_adsr_3);
	addChildComponent(m_adsr_4);

	m_flanger.setSyncTimeColor(FLANGER_DISPLAY_COLOR);
	addChildComponent(m_flanger);

	m_phaser.setSyncTimeColor(PHASER_DISPLAY_COLOR);
	addChildComponent(m_phaser);

	m_chorus.setSyncTimeColor(CHORUS_DISPLAY_COLOR);
	addChildComponent(m_chorus);

	addAndMakeVisible(m_delay);

	addChildComponent(m_reverb);

	addAndMakeVisible(m_mod_matrix);
	addChildComponent(m_patch_browser);
	addChildComponent(m_arp);

	addAndMakeVisible(m_xy_section);

	m_env_1_button.setToggleState(true, dontSendNotification);
	m_env_1_button.setRadioGroupId(RADIO_GROUP_ENV13);
	m_env_1_button.onClick = [&]() {
		setEnv13(m_env_1_button.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty("env_left_selected", (int)m_env_1_button.getToggleState(), nullptr);
	};
	m_env_1_button.setTooltip("Shows the amplifier\nenvelope");
	addAndMakeVisible(m_env_1_button);
	m_env_1_button.disableMidiLearn();
	m_env_1_button.setClickingTogglesState(true);

	m_env_3_button.setToggleState(false, dontSendNotification);
	m_env_3_button.setRadioGroupId(RADIO_GROUP_ENV13);
	m_env_3_button.onClick = [&]() {
		setEnv13(!m_env_3_button.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty("env_left_selected", (int)!m_env_3_button.getToggleState(), nullptr);
	};
	m_env_3_button.setTooltip("Shows the mod envelope");
	addAndMakeVisible(m_env_3_button);
	m_env_3_button.disableMidiLearn();
	m_env_3_button.setClickingTogglesState(true);

	m_env_2_button.setClickingTogglesState(true);
	m_env_2_button.setRadioGroupId(RADIO_GROUP_ENV24);
	m_env_2_button.setToggleState(true, dontSendNotification);
	m_env_2_button.onClick = [&]() {
		setEnv24(m_env_2_button.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty("env_right_selected", (int)m_env_2_button.getToggleState(), nullptr);
	};
	m_env_2_button.setTooltip("Shows the filter envelope");
	addAndMakeVisible(m_env_2_button);
	m_env_2_button.disableMidiLearn();

	m_env_4_button.setClickingTogglesState(true);
	m_env_4_button.setRadioGroupId(RADIO_GROUP_ENV24);
	m_env_4_button.setToggleState(false, dontSendNotification);
	m_env_4_button.onClick = [&]() {
		setEnv24(!m_env_4_button.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty("env_right_selected", (int)!m_env_4_button.getToggleState(), nullptr);
	};
	m_env_4_button.setTooltip("Shows the global envelope");
	addAndMakeVisible(m_env_4_button);
	m_env_4_button.disableMidiLearn();

	m_lfo_1_button.setClickingTogglesState(true);
	m_lfo_1_button.setRadioGroupId(RADIO_GROUP_LFO12);
	m_lfo_1_button.setToggleState(true, dontSendNotification);
	m_lfo_1_button.onClick = [&]() {
		setLfo12(m_lfo_1_button.getToggleState());
		m_value_tree.state.getChildWithName("lfo").setProperty("lfo_left_selected", (int)m_lfo_1_button.getToggleState(), nullptr);
	};
	m_lfo_1_button.setTooltip("Shows LFO 1");
	addAndMakeVisible(m_lfo_1_button);
	m_lfo_1_button.disableMidiLearn();

	m_lfo_2_button.setClickingTogglesState(true);
	m_lfo_2_button.setRadioGroupId(RADIO_GROUP_LFO12);
	m_lfo_2_button.setToggleState(false, dontSendNotification);
	m_lfo_2_button.onClick = [&]() {
		setLfo12(!m_lfo_2_button.getToggleState());
		m_value_tree.state.getChildWithName("lfo").setProperty("lfo_left_selected", (int)!m_lfo_2_button.getToggleState(), nullptr);
	};
	m_lfo_2_button.setTooltip("Shows LFO 2");
	addAndMakeVisible(m_lfo_2_button);
	m_lfo_2_button.disableMidiLearn();

	m_lfo_3_button.setClickingTogglesState(true);
	m_lfo_3_button.setRadioGroupId(RADIO_GROUP_LFO34);
	m_lfo_3_button.setToggleState(true, dontSendNotification);
	m_lfo_3_button.onClick = [&]() {
		setLfo34(m_lfo_3_button.getToggleState());
		m_value_tree.state.getChildWithName("lfo").setProperty("lfo_right_selected", (int)m_lfo_3_button.getToggleState(), nullptr);
	};
	m_lfo_3_button.setTooltip("Shows LFO 3");
	addAndMakeVisible(m_lfo_3_button);
	m_lfo_3_button.disableMidiLearn();

	m_lfo_4_button.setClickingTogglesState(true);
	m_lfo_4_button.setRadioGroupId(RADIO_GROUP_LFO34);
	m_lfo_4_button.setToggleState(false, dontSendNotification);
	m_lfo_4_button.onClick = [&]() {
		setLfo34(!m_lfo_4_button.getToggleState());
		m_value_tree.state.getChildWithName("lfo").setProperty("lfo_right_selected", (int)!m_lfo_4_button.getToggleState(), nullptr);
	};
	m_lfo_4_button.setTooltip("Shows the Global LFO");
	addAndMakeVisible(m_lfo_4_button);
	m_lfo_4_button.disableMidiLearn();

	m_select_arp_button.setToggleState(false, dontSendNotification);
	m_select_arp_button.setClickingTogglesState(true);
	m_select_arp_button.setRadioGroupId(RADIO_GROUP_ARP_MODMATRIX_PRESETS);
	m_select_arp_button.onClick = [&]() {
		setMatrixSectionModule(MATRIX_SECTION_INDEX_ARP);
		m_value_tree.state.getChildWithName("misc").setProperty("arp_mod_selected", MATRIX_SECTION_INDEX_ARP, nullptr);
	};
	m_select_arp_button.setTooltip("Shows the arpeggiator or the mod-matrix");
	addAndMakeVisible(m_select_arp_button);
	m_select_arp_button.disableMidiLearn();

	m_select_modmatrix_button.setToggleState(false, dontSendNotification);
	m_select_modmatrix_button.setClickingTogglesState(true);
	m_select_modmatrix_button.setRadioGroupId(RADIO_GROUP_ARP_MODMATRIX_PRESETS);
	m_select_modmatrix_button.onClick = [&]() {
		setMatrixSectionModule(MATRIX_SECTION_INDEX_MATRIX);
		m_value_tree.state.getChildWithName("misc").setProperty("arp_mod_selected", MATRIX_SECTION_INDEX_MATRIX, nullptr);
	};
	m_select_modmatrix_button.setTooltip("Shows the arpeggiator or the mod-matrix");
	addAndMakeVisible(m_select_modmatrix_button);
	m_select_modmatrix_button.disableMidiLearn();

	m_select_presets_button.setToggleState(false, dontSendNotification);
	m_select_presets_button.setClickingTogglesState(true);
	m_select_presets_button.setRadioGroupId(RADIO_GROUP_ARP_MODMATRIX_PRESETS);
	m_select_presets_button.onClick = [&]() {
		setMatrixSectionModule(MATRIX_SECTION_INDEX_PRESETS);
		m_value_tree.state.getChildWithName("misc").setProperty("arp_mod_selected", MATRIX_SECTION_INDEX_PRESETS, nullptr);
	};
	m_select_presets_button.setTooltip("Shows the arpeggiator or the mod-matrix");
	addAndMakeVisible(m_select_presets_button);
	m_select_presets_button.disableMidiLearn();

	m_mono_poly_legato_dropdown.setInlay(1);
	m_mono_poly_legato_dropdown.addItem("Legato", 1);
	m_mono_poly_legato_dropdown.addItem("Poly", 2);
	m_mono_poly_legato_dropdown.addItem("Retrig", 3);
	m_mono_poly_legato_dropdown.setEditableText(false);
	m_mono_poly_legato_dropdown.setSelectedId(2, dontSendNotification);
	m_mono_poly_legato_dropdown.showTriangle();

	m_mono_poly_legato_dropdown.setTooltip("Selects the Playmode:\n\nPoly: Play multiple voices at once.\n\nLegato: Only one voice available. Envelopes "
	                                       "are not restarted, so legato notes will blend over seemlessly.\n\nRetrig: Like Legato, but the envelopes are "
	                                       "restarted on every note start.");
	m_mono_poly_legato_dropdown.onChange = [&]() {
		m_value_tree.state.getChildWithName("misc").setProperty("legato", PLAYMODETOVALUETREE(m_mono_poly_legato_dropdown.getSelectedId()), nullptr);
		processor.setMonoPolyLegato((PlayModes)m_mono_poly_legato_dropdown.getSelectedId());
	};
	addAndMakeVisible(m_mono_poly_legato_dropdown);

	addAndMakeVisible(m_lfo_1);
	addChildComponent(m_lfo_2);
	addAndMakeVisible(m_lfo_3);
	addChildComponent(m_lfo_4);

	m_pitch_amount.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("misc").setProperty("pitchbend_amount", p_new_value, nullptr); };
	addAndMakeVisible(m_pitch_amount);
	m_pitch_amount.setMouseDragDivisor(20.f);
	m_pitch_amount.setRange(0, 24);
	m_pitch_amount.setTooltip("The amount of pitchbend for the pitchwheel in semitones");

	m_unison_selector.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("misc").setProperty("unison_voices", p_new_value, nullptr); };
	m_unison_selector.valueToText   = [](int p_value) { return "Unison: " + std::to_string(p_value); };
	m_unison_selector.setLegalValues({1, 2, 3, 4, 6});

	addAndMakeVisible(m_unison_selector);
	m_unison_selector.setMouseDragDivisor(20.f);
	m_unison_selector.setTooltip("Number of voices to trigger simultaneously\nThis limits the polyphony to 12 / "
	                             "N\nBeware: N voices means N times the CPU load, so use with care!");

	m_osc_dropdown_menu.setLookAndFeel(&m_menu_feels);
	m_filter_dropdown_menu.setLookAndFeel(&m_menu_feels);

	m_tooltip.setLookAndFeel(&m_tooltip_feels);

	m_phaser_on_attachment.reset(new OdinButtonAttachment(m_value_tree, "phaser_on", m_phaser_on_button));
	m_flanger_on_attachment.reset(new OdinButtonAttachment(m_value_tree, "flanger_on", m_flanger_on_button));
	m_delay_on_attachment.reset(new OdinButtonAttachment(m_value_tree, "delay_on", m_delay_on_button));
	m_reverb_on_attachment.reset(new OdinButtonAttachment(m_value_tree, "reverb_on", m_reverb_on_button));
	m_chorus_on_attachment.reset(new OdinButtonAttachment(m_value_tree, "chorus_on", m_chorus_on_button));
	m_fil1_osc1_attachment.reset(new OdinButtonAttachment(m_value_tree, "fil1_osc1", m_filleft_button1));
	m_fil1_osc2_attachment.reset(new OdinButtonAttachment(m_value_tree, "fil1_osc2", m_filleft_button2));
	m_fil1_osc3_attachment.reset(new OdinButtonAttachment(m_value_tree, "fil1_osc3", m_filleft_button3));
	m_fil2_osc1_attachment.reset(new OdinButtonAttachment(m_value_tree, "fil2_osc1", m_filright_button1));
	m_fil2_osc2_attachment.reset(new OdinButtonAttachment(m_value_tree, "fil2_osc2", m_filright_button2));
	m_fil2_osc3_attachment.reset(new OdinButtonAttachment(m_value_tree, "fil2_osc3", m_filright_button3));
	m_fil2_fil1_attachment.reset(new OdinButtonAttachment(m_value_tree, "fil2_fil1", m_filright_buttonf1));

	m_glide_attachment.reset(new OdinKnobAttachment(m_value_tree, "glide", m_glide));
	m_master_attachment.reset(new OdinKnobAttachment(m_value_tree, "master", m_master));
	m_unison_width_attachment.reset(new OdinKnobAttachment(m_value_tree, "unison_width", m_unison_width));
	m_unison_detune_attachment.reset(new OdinKnobAttachment(m_value_tree, "unison_detune", m_unison_detune));
	m_modwheel_attachment.reset(new OdinKnobAttachment(m_value_tree, "modwheel", m_modwheel));
	m_pitchbend_attachment.reset(new OdinKnobAttachment(m_value_tree, "pitchbend", m_pitchwheel));

	m_master.setNumDecimalPlacesToDisplay(2);
	m_glide.setNumDecimalPlacesToDisplay(3);
	m_modwheel.setNumDecimalPlacesToDisplay(3);
	m_pitchwheel.setNumDecimalPlacesToDisplay(3);
	m_unison_detune.setNumDecimalPlacesToDisplay(3);
	m_unison_width.setNumDecimalPlacesToDisplay(3);

	m_pitch_amount.setParameterId("pitchbend_amount");
	m_value_tree.addParameterListener("pitchbend_amount", &m_pitch_amount);

	m_value_input.setBounds(0, 0, INPUT_LABEL_SIZE_X, INPUT_LABEL_SIZE_Y);
	m_value_input.setComponentID("value_input");
	m_value_input.onFocusLost = [&]() { m_value_input.setVisible(false); };
	m_value_input.onEscapeKey = [&]() { m_value_input.setVisible(false); };
	m_value_input.onReturnKey = [&]() {
		m_value_input.applyValue();
		m_value_input.setVisible(false);
	};
	m_value_input.setLookAndFeel(&m_input_feels);
	setComponentID("editor");
	addChildComponent(m_value_input);

	setOsc1Plate(m_value_tree.state.getChildWithName("osc")["osc1_type"]);
	setOsc2Plate(m_value_tree.state.getChildWithName("osc")["osc2_type"]);
	setOsc3Plate(m_value_tree.state.getChildWithName("osc")["osc3_type"]);
	setFilter1Plate(m_value_tree.state.getChildWithName("misc")["fil1_type"]);
	setFilter2Plate(m_value_tree.state.getChildWithName("misc")["fil2_type"]);
	setFilter3Plate(m_value_tree.state.getChildWithName("misc")["fil3_type"]);

	SET_CTR_KEY(m_glide);
	SET_CTR_KEY(m_master);
	SET_CTR_KEY(m_modwheel);
	SET_CTR_KEY(m_unison_detune);
	SET_CTR_KEY(m_unison_width);

	m_master.setRange(MASTER_MIN, MASTER_MAX);
	m_master.setTextValueSuffix(" dB");
	m_master.setNumDecimalPlacesToDisplay(1);

	m_tooltip.setBounds(900, 100, 100, 100);
	setTooltipEnabled(false);
	addAndMakeVisible(m_tooltip);

	forceValueTreeOntoComponents(false);

	bool set_GUI_big;
	readOrCreateConfigFile(set_GUI_big);
	auto scale = ConfigFileManager::getInstance().getOptionGuiScale();
	setSize(GUI_BASE_WIDTH * scale, GUI_BASE_HEIGHT * scale);

	setTooltipEnabled(ConfigFileManager::getInstance().getOptionShowTooltip());

	addAndMakeVisible(m_resize_dragger);
	m_resize_dragger.onIncrease = [&]() {
		auto new_size = int(ConfigFileManager::getInstance().getOptionGuiScale()) + 1;
		if (new_size <= int(GuiScale::Z200)) {
			setGuiScale(new_size);
		}
	};
	m_resize_dragger.onDecrease = [&]() {
		auto new_size = int(ConfigFileManager::getInstance().getOptionGuiScale()) - 1;
		if (new_size >= int(GuiScale::Z100)) {
			setGuiScale(new_size);
		}
	};
	m_resize_dragger.setTooltip("Drag here to change the size of the user interface");

	m_osc1_dropdown.toFront(false);
	m_osc2_dropdown.toFront(false);
	m_osc3_dropdown.toFront(false);
	m_filter1_dropdown.toFront(false);
	m_filter2_dropdown.toFront(false);
	m_filter3_dropdown.toFront(false);
	m_phaser_on_button.toFront(false);
	m_flanger_on_button.toFront(false);
	m_chorus_on_button.toFront(false);
	m_delay_on_button.toFront(false);
	m_reverb_on_button.toFront(false);
	m_filleft_button1.toFront(false);
	m_filright_button1.toFront(false);
	m_filleft_button2.toFront(false);
	m_filright_button2.toFront(false);
	m_filleft_button3.toFront(false);
	m_filright_button3.toFront(false);
	m_filright_buttonf1.toFront(false);
	m_value_input.toFront(false);
	m_tooltip.toFront(false);

	//DBG("Display_Scale: " + std::to_string(Desktop::getInstance().getDisplays().getMainDisplay().scale));
	ConfigFileManager::getInstance().incrementNumGuiOpens();
	ConfigFileManager::getInstance().saveDataToFile();

	addChildComponent(m_spline_ad);
	// todo todo todo add preference when user has clicked this away and also second time
	//m_spline_ad.setVisible(ConfigFileManager::getInstance().getNumGuiOpens() > 10);
	addChildComponent(m_rescale_component);

	UIAssetManager::getInstance()->registerEditor(this);

	// start resizing thread
	const auto threadWasStarted = UIAssetManager::getInstance()->launchImageCreationThreads(ConfigFileManager::getInstance().getOptionGuiScale());
	if (threadWasStarted)
		setGuiSize(GuiScale::Z100, false); // if the assets don't exist, the thread will set the size in a few seconds set this size to ensure call of resized
	else
		setGuiSize((GuiScale)ConfigFileManager::getInstance().getOptionGuiScale(), false);
}

OdinEditor::~OdinEditor() {
	UIAssetManager::getInstance()->unregisterEditor(this);

	//remove lookandfeels
	m_osc_dropdown_menu.setLookAndFeel(nullptr);
	m_filter_dropdown_menu.setLookAndFeel(nullptr);
	m_tooltip.setLookAndFeel(nullptr);
	m_value_input.setLookAndFeel(nullptr);

	// todo what is the reasoning behind this???
	m_value_tree.removeParameterListener("pitchbend_amount", &m_pitch_amount);

	// remove lambdas which are invalid after the editor was closed
	m_processor.onSetStateInformation = [&]() {};
	m_processor.updatePitchWheelGUI   = [&](float p_value) {};
	m_processor.updateModWheelGUI     = [&](float p_value) {};

	// tell processor that the editor was destroyed (needs to invalidate pointers)
	m_processor.onEditorDestruction();
}

//==============================================================================
void OdinEditor::paint(Graphics &g) {
	juce::Image background = UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::background, ConfigFileManager::getInstance().getOptionGuiScale());
	g.drawImageAt(background, 0, 0);
}

void OdinEditor::resized() {
	GET_LOCAL_AREA(m_resize_dragger, "ResizeDragger");

	GET_LOCAL_AREA(m_detune_label, "DetuneLabel");
	GET_LOCAL_AREA(m_width_label, "WidthLabel");
	GET_LOCAL_AREA(m_master_label, "MasterLabel");
	GET_LOCAL_AREA(m_glide_label, "GlideLabel");

	GET_LOCAL_AREA(m_osc1, "Osc1");
	GET_LOCAL_AREA(m_osc2, "Osc2");
	GET_LOCAL_AREA(m_osc3, "Osc3");
	GET_LOCAL_AREA(m_fil1_component, "Fil1");
	GET_LOCAL_AREA(m_fil2_component, "Fil2");
	GET_LOCAL_AREA(m_fil3_component, "Fil3");
	GET_LOCAL_AREA(m_osc1_dropdown, "Osc1DD");
	GET_LOCAL_AREA(m_osc2_dropdown, "Osc2DD");
	GET_LOCAL_AREA(m_osc3_dropdown, "Osc3DD");
	GET_LOCAL_AREA(m_filter1_dropdown, "Fil1DD");
	GET_LOCAL_AREA(m_filter2_dropdown, "Fil2DD");
	GET_LOCAL_AREA(m_filter3_dropdown, "Fil3DD");

	GET_LOCAL_AREA(m_mod_matrix, "MatrixArpPreset");
	GET_LOCAL_AREA(m_arp, "MatrixArpPreset");
	GET_LOCAL_AREA(m_patch_browser, "MatrixArpPreset");

	GET_LOCAL_AREA(m_select_presets_button, "SelectPresets");
	GET_LOCAL_AREA(m_select_modmatrix_button, "SelectModMatrix");
	GET_LOCAL_AREA(m_select_arp_button, "SelectArp");

	GET_LOCAL_AREA(m_midsection, "AmpDist");

	GET_LOCAL_AREA(m_env_1_button, "ADSRButton1");
	GET_LOCAL_AREA(m_env_2_button, "ADSRButton2");
	GET_LOCAL_AREA(m_env_3_button, "ADSRButton3");
	GET_LOCAL_AREA(m_env_4_button, "ADSRButton4");

	GET_LOCAL_AREA(m_adsr_1, "ADSRLeft");
	GET_LOCAL_AREA(m_adsr_3, "ADSRLeft");
	GET_LOCAL_AREA(m_adsr_2, "ADSRRight");
	GET_LOCAL_AREA(m_adsr_4, "ADSRRight");

	GET_LOCAL_AREA(m_lfo_1_button, "LFOButton1");
	GET_LOCAL_AREA(m_lfo_2_button, "LFOButton2");
	GET_LOCAL_AREA(m_lfo_3_button, "LFOButton3");
	GET_LOCAL_AREA(m_lfo_4_button, "LFOButton4");

	GET_LOCAL_AREA(m_lfo_1, "LFOLeft");
	GET_LOCAL_AREA(m_lfo_2, "LFOLeft");
	GET_LOCAL_AREA(m_lfo_3, "LFORight");
	GET_LOCAL_AREA(m_lfo_4, "LFORight");

	GET_LOCAL_AREA(m_flanger, "FX");
	GET_LOCAL_AREA(m_phaser, "FX");
	GET_LOCAL_AREA(m_chorus, "FX");
	GET_LOCAL_AREA(m_delay, "FX");
	GET_LOCAL_AREA(m_reverb, "FX");

	GET_LOCAL_AREA(m_burger_button, "BurgerButton");

	GET_LOCAL_AREA(m_reset, "Reset");
	GET_LOCAL_AREA(m_glide, "Glide");
	GET_LOCAL_AREA(m_master, "Master");
	GET_LOCAL_AREA(m_unison_detune, "UnisonDetune");
	GET_LOCAL_AREA(m_unison_width, "UnisonWidth");
	GET_LOCAL_AREA(m_xy_section, "XY_SECTION");

	GET_LOCAL_AREA(m_fx_buttons_section, "FXButtons");
	GET_LOCAL_AREA(m_delay_on_button, "DelayOn");
	GET_LOCAL_AREA(m_phaser_on_button, "PhaserOn");
	GET_LOCAL_AREA(m_chorus_on_button, "ChorusOn");
	GET_LOCAL_AREA(m_flanger_on_button, "FlangerOn");
	GET_LOCAL_AREA(m_reverb_on_button, "ReverbOn");

	GET_LOCAL_AREA(m_filleft_button1, "FilLeft1");
	GET_LOCAL_AREA(m_filleft_button2, "FilLeft2");
	GET_LOCAL_AREA(m_filleft_button3, "FilLeft3");
	GET_LOCAL_AREA(m_filright_button1, "FilRight1");
	GET_LOCAL_AREA(m_filright_button2, "FilRight2");
	GET_LOCAL_AREA(m_filright_button3, "FilRight3");
	GET_LOCAL_AREA(m_filright_buttonf1, "FilRightF1");

	GET_LOCAL_AREA(m_tuning, "Tuning");
	GET_LOCAL_AREA(m_pitchwheel, "PitchBend");
	GET_LOCAL_AREA(m_modwheel, "ModWheel");
	GET_LOCAL_AREA(m_mono_poly_legato_dropdown, "Playmode");
	GET_LOCAL_AREA(m_pitch_amount, "PitchBend Amount");
	GET_LOCAL_AREA(m_unison_selector, "UnisonAmount");
	GET_LOCAL_AREA(m_value_input, "ValueInput");

	m_spline_ad.setBounds(getLocalBounds());
	m_rescale_component.setBounds(getLocalBounds());

	//forceValueTreeOntoComponents(false);
}

void OdinEditor::setGuiSize(GuiScale p_size, bool p_set_config) {
	if (p_set_config) {
		ConfigFileManager::getInstance().setOptionGuiScale(int(p_size));
		ConfigFileManager::getInstance().saveDataToFile();
	}

	setSize(GUI_BASE_WIDTH * (int)p_size, GUI_BASE_HEIGHT * (int)p_size);

	forceValueTreeOntoComponents();
}

void OdinEditor::setOsc1Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_osc1.setOscType(p_osc_type);
	m_value_tree.state.getChildWithName("osc").setProperty(m_osc1_type_identifier, p_osc_type, nullptr);
}

void OdinEditor::setOsc2Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_osc2.setOscType(p_osc_type);
	m_value_tree.state.getChildWithName("osc").setProperty(m_osc2_type_identifier, p_osc_type, nullptr);
}

void OdinEditor::setOsc3Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_osc3.setOscType(p_osc_type);
	m_value_tree.state.getChildWithName("osc").setProperty(m_osc3_type_identifier, p_osc_type, nullptr);
}

void OdinEditor::setFilter1Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_fil1_component.setFilterType(p_osc_type);
	m_value_tree.state.getChildWithName("misc").setProperty(m_fil1_type_identifier, p_osc_type, nullptr);
}

void OdinEditor::setFilter2Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_fil2_component.setFilterType(p_osc_type);
	m_value_tree.state.getChildWithName("misc").setProperty(m_fil2_type_identifier, p_osc_type, nullptr);
}

void OdinEditor::setFilter3Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_fil3_component.setFilterType(p_osc_type);
	m_value_tree.state.getChildWithName("misc").setProperty(m_fil3_type_identifier, p_osc_type, nullptr);
}

void OdinEditor::setEnv13(bool p_env1) {
	if (p_env1) {
		m_adsr_1.setVisible(true);
		m_adsr_3.setVisible(false);
	} else {
		m_adsr_1.setVisible(false);
		m_adsr_3.setVisible(true);
	}
}

void OdinEditor::setEnv24(bool p_env2) {
	if (p_env2) {
		m_adsr_2.setVisible(true);
		m_adsr_4.setVisible(false);
	} else {
		m_adsr_2.setVisible(false);
		m_adsr_4.setVisible(true);
	}
}

void OdinEditor::setLfo12(bool p_lfo1) {
	if (p_lfo1) {
		m_lfo_1.setVisible(true);
		m_lfo_2.setVisible(false);
	} else {
		m_lfo_1.setVisible(false);
		m_lfo_2.setVisible(true);
	}
}

void OdinEditor::setLfo34(bool p_lfo2) {
	if (p_lfo2) {
		m_lfo_3.setVisible(true);
		m_lfo_4.setVisible(false);
	} else {
		m_lfo_3.setVisible(false);
		m_lfo_4.setVisible(true);
	}
}

void OdinEditor::setMatrixSectionModule(int p_module) {
	switch (p_module) {
	case MATRIX_SECTION_INDEX_ARP:
		m_mod_matrix.setVisible(false);
		m_patch_browser.setVisible(false);
		m_arp.setVisibleAndStartTimer(true);
		break;
	case MATRIX_SECTION_INDEX_MATRIX:
		m_mod_matrix.setVisible(true);
		m_patch_browser.setVisible(false);
		m_arp.setVisibleAndStartTimer(false);
		break;
	case MATRIX_SECTION_INDEX_PRESETS:
		m_mod_matrix.setVisible(false);
		m_patch_browser.setVisible(true);
		m_arp.setVisibleAndStartTimer(false);
		break;
	default:
		//fallback to matrix
		m_mod_matrix.setVisible(true);
		m_patch_browser.setVisible(false);
		m_arp.setVisibleAndStartTimer(false);
		break;
	}
}

void OdinEditor::arrangeFXOnButtons(std::map<std::string, int> p_map) {

	const auto width = m_delay_on_button.getWidth();
	const auto x     = m_fx_buttons_section.getX();
	const auto y     = m_delay_on_button.getY();

	m_flanger_on_button.setTopLeftPosition(x + p_map.find("flanger")->second * width, y);
	m_phaser_on_button.setTopLeftPosition(x + p_map.find("phaser")->second * width, y);
	m_chorus_on_button.setTopLeftPosition(x + p_map.find("chorus")->second * width, y);
	m_delay_on_button.setTopLeftPosition(x + p_map.find("delay")->second * width, y);
	m_reverb_on_button.setTopLeftPosition(x + p_map.find("reverb")->second * width, y);

	m_value_tree.state.getChildWithName("fx").setProperty(m_delay_position_identifier, (float)p_map.find("delay")->second, nullptr);
	m_value_tree.state.getChildWithName("fx").setProperty(m_phaser_position_identifier, (float)p_map.find("phaser")->second, nullptr);
	m_value_tree.state.getChildWithName("fx").setProperty(m_flanger_position_identifier, (float)p_map.find("flanger")->second, nullptr);
	m_value_tree.state.getChildWithName("fx").setProperty(m_chorus_position_identifier, (float)p_map.find("chorus")->second, nullptr);
	m_value_tree.state.getChildWithName("fx").setProperty(m_reverb_position_identifier, (float)p_map.find("reverb")->second, nullptr);

	processor.setFXButtonsPosition((float)p_map.find("delay")->second,
	                               (float)p_map.find("phaser")->second,
	                               (float)p_map.find("flanger")->second,
	                               (float)p_map.find("chorus")->second,
	                               (float)p_map.find("reverb")->second);
}

void OdinEditor::setActiveFXPanel(const std::string &p_name) {
	m_phaser.setVisible(false);
	m_flanger.setVisible(false);
	m_chorus.setVisible(false);
	m_delay.setVisible(false);
	m_reverb.setVisible(false);

	int fx = 0;
	fx     = p_name == "phaser" ? 1 : fx;
	fx     = p_name == "flanger" ? 2 : fx;
	fx     = p_name == "chorus" ? 3 : fx;
	fx     = p_name == "delay" ? 4 : fx;
	fx     = p_name == "reverb" ? 5 : fx;

	switch (fx) {
	case 1:
		m_phaser.setVisible(true);
		break;
	case 2:
		m_flanger.setVisible(true);
		break;
	case 3:
		m_chorus.setVisible(true);
		break;
	case 4:
		m_delay.setVisible(true);
		break;
	case 5:
		m_reverb.setVisible(true);
		break;
	default:
		m_delay.setVisible(true);
		break;
	}
}

void OdinEditor::setTooltipEnabled(bool p_enabled) {
	m_tooltip.activate(p_enabled);
	if (p_enabled) {
		m_tooltip.setMillisecondsBeforeTipAppears(0);
	} else {
		//unelegant solution
		m_tooltip.setMillisecondsBeforeTipAppears(2047483647);
		m_tooltip.hideTip();
	}
}

void OdinEditor::forceValueTreeOntoComponentsOnlyMainPanel() {

	m_unison_selector.setValue(m_value_tree.state.getChildWithName("misc")["unison_voices"]);
	m_pitch_amount.setValue(m_value_tree.state.getChildWithName("misc")["pitchbend_amount"]);
	m_mono_poly_legato_dropdown.setSelectedId((int)VALUETREETOPLAYMODE((int)m_value_tree.state.getChildWithName("misc")["legato"]), dontSendNotification);

	// ugly fix to set highlighted fx panel
	std::string fx_name = "delay";
	if ((float)m_value_tree.state.getChildWithName("fx")["phaser_selected"] > 0.5) {
		fx_name = "phaser";
	} else if ((float)m_value_tree.state.getChildWithName("fx")["flanger_selected"] > 0.5) {
		fx_name = "flanger";
	} else if ((float)m_value_tree.state.getChildWithName("fx")["chorus_selected"] > 0.5) {
		fx_name = "chorus";
	} else if ((float)m_value_tree.state.getChildWithName("fx")["reverb_selected"] > 0.5) {
		fx_name = "reverb";
	}
	setActiveFXPanel(fx_name);

	const auto env_1_selected = (float)m_value_tree.state.getChildWithName("misc")["env_left_selected"] > 0.5;
	m_env_1_button.setToggleState(env_1_selected, dontSendNotification);
	m_env_3_button.setToggleState(!env_1_selected, dontSendNotification);
	setEnv13(env_1_selected);
	const auto env_2_selected = (float)m_value_tree.state.getChildWithName("misc")["env_right_selected"] > 0.5;
	m_env_2_button.setToggleState(env_2_selected, dontSendNotification);
	m_env_4_button.setToggleState(!env_2_selected, dontSendNotification);
	setEnv24(env_2_selected);

	const auto lfo_1_selected = (float)m_value_tree.state.getChildWithName("lfo")["lfo_left_selected"] > 0.5;
	m_lfo_1_button.setToggleState(lfo_1_selected, dontSendNotification);
	m_lfo_2_button.setToggleState(!lfo_1_selected, dontSendNotification);
	setLfo12(lfo_1_selected);
	const auto lfo_3_selected = (float)m_value_tree.state.getChildWithName("lfo")["lfo_right_selected"] > 0.5;
	m_lfo_3_button.setToggleState(lfo_3_selected, dontSendNotification);
	m_lfo_4_button.setToggleState(!lfo_3_selected, dontSendNotification);
	setLfo34(lfo_3_selected);

	const float bottom_section_selection = (float)m_value_tree.state.getChildWithName("misc")["arp_mod_selected"];
	m_select_arp_button.setToggleState(fabs(bottom_section_selection - (float)MATRIX_SECTION_INDEX_ARP) < 0.1f, dontSendNotification);
	m_select_modmatrix_button.setToggleState(fabs(bottom_section_selection - (float)MATRIX_SECTION_INDEX_MATRIX) < 0.1f, dontSendNotification);
	m_select_presets_button.setToggleState(fabs(bottom_section_selection - (float)MATRIX_SECTION_INDEX_PRESETS) < 0.1f, dontSendNotification);
	setMatrixSectionModule((int)(bottom_section_selection + 0.5f));
}

void OdinEditor::forceValueTreeOntoComponents(bool p_reset_audio) {

	// reset audioengine
	if (p_reset_audio) {
		processor.resetAudioEngine();
	}

	forceValueTreeOntoComponentsOnlyMainPanel();

	// constructor, these seems to have no influence
	setOsc1Plate(m_value_tree.state.getChildWithName("osc")["osc1_type"]);
	setOsc2Plate(m_value_tree.state.getChildWithName("osc")["osc2_type"]);
	setOsc3Plate(m_value_tree.state.getChildWithName("osc")["osc3_type"]);
	setFilter1Plate(m_value_tree.state.getChildWithName("misc")["fil1_type"]);
	setFilter2Plate(m_value_tree.state.getChildWithName("misc")["fil2_type"]);
	setFilter3Plate(m_value_tree.state.getChildWithName("misc")["fil3_type"]);

	m_osc1.forceValueTreeOntoComponents(m_value_tree.state, 1, true);
	m_osc2.forceValueTreeOntoComponents(m_value_tree.state, 2, true);
	m_osc3.forceValueTreeOntoComponents(m_value_tree.state, 3, true);
	m_fil1_component.forceValueTreeOntoComponents(m_value_tree.state, 1);
	m_fil2_component.forceValueTreeOntoComponents(m_value_tree.state, 2);
	m_fil3_component.forceValueTreeOntoComponents(m_value_tree.state, 3);
	m_mod_matrix.forceValueTreeOntoComponents(m_value_tree.state);
	m_lfo_1.forceValueTreeOntoComponents(m_value_tree.state);
	m_lfo_2.forceValueTreeOntoComponents(m_value_tree.state);
	m_lfo_3.forceValueTreeOntoComponents(m_value_tree.state);
	m_lfo_4.forceValueTreeOntoComponents(m_value_tree.state);
	m_flanger.forceValueTreeOntoComponents(m_value_tree.state);
	m_phaser.forceValueTreeOntoComponents(m_value_tree.state);
	m_chorus.forceValueTreeOntoComponents(m_value_tree.state);
	m_delay.forceValueTreeOntoComponents(m_value_tree.state);
	m_midsection.forceValueTreeOntoComponents(m_value_tree.state);
	m_fx_buttons_section.forceValueTreeOntoComponents(m_value_tree.state);
	m_arp.forceValueTreeOntoComponents(m_value_tree.state);

	m_patch_browser.setSelectedEntriesFromValueTree();
}

bool OdinEditor::keyStateChanged(bool isKeyDown, Component *originatingComponent) {

	if (m_is_standalone_plugin) {

		if (KeyPress::isKeyCurrentlyDown(65) != m_A_down) {
			m_A_down = KeyPress::isKeyCurrentlyDown(65);
			if (m_A_down) {
				processor.handleMidiNoteOn(48 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(48 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(87) != m_W_down) {
			m_W_down = KeyPress::isKeyCurrentlyDown(87);
			if (m_W_down) {
				processor.handleMidiNoteOn(49 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(49 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(83) != m_S_down) {
			m_S_down = KeyPress::isKeyCurrentlyDown(83);
			if (m_S_down) {
				processor.handleMidiNoteOn(50 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(50 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(69) != m_E_down) {
			m_E_down = KeyPress::isKeyCurrentlyDown(69);
			if (m_E_down) {
				processor.handleMidiNoteOn(51 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(51 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(68) != m_D_down) {
			m_D_down = KeyPress::isKeyCurrentlyDown(68);
			if (m_D_down) {
				processor.handleMidiNoteOn(52 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(52 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(70) != m_F_down) {
			m_F_down = KeyPress::isKeyCurrentlyDown(70);
			if (m_F_down) {
				processor.handleMidiNoteOn(53 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(53 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(84) != m_T_down) {
			m_T_down = KeyPress::isKeyCurrentlyDown(84);
			if (m_T_down) {
				processor.handleMidiNoteOn(54 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(54 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(71) != m_G_down) {
			m_G_down = KeyPress::isKeyCurrentlyDown(71);
			if (m_G_down) {
				processor.handleMidiNoteOn(55 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(55 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(90) != m_Z_down) {
			m_Z_down = KeyPress::isKeyCurrentlyDown(90);
			if (m_Z_down) {
				processor.handleMidiNoteOn(56 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(56 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(72) != m_H_down) {
			m_H_down = KeyPress::isKeyCurrentlyDown(72);
			if (m_H_down) {
				processor.handleMidiNoteOn(57 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(57 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(85) != m_U_down) {
			m_U_down = KeyPress::isKeyCurrentlyDown(85);
			if (m_U_down) {
				processor.handleMidiNoteOn(58 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(58 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(74) != m_J_down) {
			m_J_down = KeyPress::isKeyCurrentlyDown(74);
			if (m_J_down) {
				processor.handleMidiNoteOn(59 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(59 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(75) != m_K_down) {
			m_K_down = KeyPress::isKeyCurrentlyDown(75);
			if (m_K_down) {
				processor.handleMidiNoteOn(60 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(60 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(79) != m_O_down) {
			m_O_down = KeyPress::isKeyCurrentlyDown(79);
			if (m_O_down) {
				processor.handleMidiNoteOn(61 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(61 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(76) != m_L_down) {
			m_L_down = KeyPress::isKeyCurrentlyDown(76);
			if (m_L_down) {
				processor.handleMidiNoteOn(62 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(62 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(80) != m_P_down) {
			m_P_down = KeyPress::isKeyCurrentlyDown(80);
			if (m_P_down) {
				processor.handleMidiNoteOn(63 + m_octave_shift * 12, 100);
			} else {
				processor.handleMidiNoteOff(63 + m_octave_shift * 12);
			}
		}
	}
	return false;
}

void OdinEditor::allMidiKeysOff() {
	// this is the SHITTIEST implementation ever
	// but it gets the job done and is only called every blue moon :-)
	// (gets called only for (PC-)keyboard octave change)
	for (int note = 0; note < 127; ++note) {
		processor.handleMidiNoteOff(note);
	}
}

void OdinEditor::updatePitchWheel(float p_value) {
	auto sp = SafePointer<Component>(this);

	MessageManager::callAsync([p_value, sp, this] {
		if (sp == nullptr) {
			return;
		}
		m_pitchwheel.setValue(p_value);
	});
}

void OdinEditor::updateModWheel(float p_value) {
	auto sp = SafePointer<Component>(this);

	MessageManager::callAsync([p_value, sp, this] {
		if (sp == nullptr) {
			return;
		}
		m_modwheel.setValue(p_value);
	});
}

void OdinEditor::readOrCreateConfigFile(bool &p_GUI_big) {
	ConfigFileManager::getInstance().saveDataToFile();
}

void OdinEditor::paintOverChildren(Graphics &g) {
	if (m_live_constrainer.isConstraining())
		paintGrid(g);

	m_live_constrainer.paintOverlay(g);
}

void OdinEditor::paintGrid(Graphics &g) {
	constexpr auto grid_alpha       = 0.05f;
	constexpr auto grid_alpha_major = 0.2f;
	constexpr auto grid_alpha_mid   = 0.1f;
	const auto grid_size            = int(ConfigFileManager::getInstance().getOptionGuiScale());

	auto grid_base_colour = juce::Colour(0x0088ffff);

	for (int x = 0; x < getWidth(); x += grid_size) {
		if (x % (grid_size * 10) == 0)
			g.setColour(grid_base_colour.withAlpha(grid_alpha_major));
		else if (x % (grid_size * 5) == 0)
			g.setColour(grid_base_colour.withAlpha(grid_alpha_mid));
		else
			g.setColour(grid_base_colour.withAlpha(grid_alpha));

		g.drawLine(x, 0, x, getHeight(), 1);
	}

	for (int y = 0; y < getHeight(); y += grid_size) {
		if (y % (grid_size * 10) == 0)
			g.setColour(grid_base_colour.withAlpha(grid_alpha_major));
		else if (y % (grid_size * 5) == 0)
			g.setColour(grid_base_colour.withAlpha(grid_alpha_mid));
		else
			g.setColour(grid_base_colour.withAlpha(grid_alpha));

		g.drawLine(0, y, getWidth(), y, 1);
	}
}

void OdinEditor::setGuiScale(int scale) {
	ConfigFileManager::getInstance().setOptionGuiScale(scale);
	ConfigFileManager::getInstance().saveDataToFile();

	setSize(GUI_BASE_WIDTH * scale, GUI_BASE_HEIGHT * scale);
}