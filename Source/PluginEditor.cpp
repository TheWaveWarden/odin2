/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

#include <ctime> //todo ifdef debug

bool writeComponentImageToFile(Component &comp) {
	time_t rawtime;
	struct tm *timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	//strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
	std::string date(buffer);

	juce::File file("/home/frot/odinvst/screenshot.png");
	juce::File file2("/home/frot/gui_saves_odin/screenshot" + date + ".png");
	Rectangle<int> subArea = comp.getBounds();
	if (ImageFileFormat *format = ImageFileFormat::findImageFormatForFileExtension(file)) {
		FileOutputStream out(file);
		FileOutputStream out2(file2);
		if (out2.openedOk()) {
			format->writeImageToStream(comp.createComponentSnapshot(subArea), out2);
		}

		if (out.openedOk()) {
			out.setPosition(0);
			out.truncate();
			return format->writeImageToStream(comp.createComponentSnapshot(subArea), out);
		}
	}
	return false;
}

void writeValueTreeToFile(const ValueTree &tree) {
	File file("/home/frot/odinvst/ValueTree.txt");
	String text = tree.toXmlString();
	file.replaceWithText(text);
}

//==============================================================================
OdinAudioProcessorEditor::OdinAudioProcessorEditor(OdinAudioProcessor &p_processor,
                                                   AudioProcessorValueTreeState &vts,
                                                   bool p_is_standalone) :
    m_value_tree(vts),
    m_fx_buttons_section(vts, p_processor), AudioProcessorEditor(&p_processor), processor(p_processor),
    m_osc1_dropdown("osc1_dropdown_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_osc2_dropdown("osc2_dropdown_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_osc3_dropdown("osc3_dropdown_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filter1_dropdown("filter1_dropdown_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filter2_dropdown("filter2_dropdown_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filter3_dropdown("filter3_dropdown_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filleft_button1("filter_left_button1", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filleft_button2("filter_left_button2", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filleft_button3("filter_left_button3", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filright_button1("filter_right_button1", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filright_button2("filter_right_button2", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filright_button3("filter_right_button3", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_filright_buttonf1("filter_left_buttonf1", juce::DrawableButton::ButtonStyle::ImageRaw),

    m_phaser_on_button("phaser_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_flanger_on_button("flanger_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_chorus_on_button("chorus_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_delay_on_button("delay_button", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_question_button("question_button", juce::DrawableButton::ButtonStyle::ImageRaw), m_env_13_button("env13_button"),
    m_env_24_button("env24_button"), m_lfo_13_button("lfo13_button"), m_lfo_24_button("lfo24_button"),
    m_pitch_amount(true), m_BPM_selector(true), m_osc1(p_processor, vts, "1"), m_osc2(p_processor, vts, "2"),
    m_osc3(p_processor, vts, "3"), m_fil1_component(vts, "1"), m_fil2_component(vts, "2"), m_fil3_component(vts, "3"),
    m_midsection(vts), m_adsr_1(vts, "1"), m_adsr_2(vts, "2"), m_adsr_3(vts, "3"), m_adsr_4(vts, "4"),
    m_lfo_1(vts, "1", p_is_standalone), m_lfo_2(vts, "2", p_is_standalone), m_lfo_3(vts, "3", p_is_standalone),
    m_lfo_4(vts, "4", p_is_standalone), m_delay(vts, p_is_standalone), m_phaser(vts, "phaser", p_is_standalone),
    m_flanger(vts, "flanger", p_is_standalone), m_chorus(vts, "chorus", p_is_standalone), m_xy_section(vts, "xy"),
    m_osc1_type_indentifier("osc1_type"), m_osc2_type_indentifier("osc2_type"), m_osc3_type_indentifier("osc3_type"),
    m_fil1_type_indentifier("fil1_type"), m_fil2_type_indentifier("fil2_type"), m_fil3_type_indentifier("fil3_type"),
    m_pitchbend_amount_identifier("pitchbend_amount"), m_delay_position_identifier("delay_position"),
    m_flanger_position_identifier("flanger_position"), m_phaser_position_identifier("phaser_position"),
    m_chorus_position_identifier("chorus_position"), m_mod_matrix(vts), m_legato_button("legato"),
    m_tooltip(nullptr, 2047483647), m_is_standalone_plugin(p_is_standalone), m_save_load(vts) {

	if (m_is_standalone_plugin) {
		addKeyListener(this);
	}

	p_processor.onSetStateInformation = [&]() {
		// DBG("ONSETSTATEINFORMATION!\n\");
		forceValueTreeOntoComponents(true);
	};

	p_processor.updatePitchWheelGUI = [&](float p_value) { updatePitchWheel(p_value); };

	p_processor.updateModWheelGUI = [&](float p_value) { updateModWheel(p_value); };

	m_save_load.forceValueTreeLambda = [&]() { forceValueTreeOntoComponents(true); };

	Knob::setOdinPointer(&p_processor);
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
	m_filter_dropdown_menu.addItem(FILTER_TYPE_SEM12, "Oberheim 12");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_DIODE, "Diode Ladder");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_KORG_LP, "Korg 35 LP");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_KORG_HP, "Korg 35 HP");
	m_filter_dropdown_menu.addSeparator();
	m_filter_dropdown_menu.addItem(FILTER_TYPE_COMB, "Comb Filter");
	m_filter_dropdown_menu.addItem(FILTER_TYPE_FORMANT, "Formant Filter");
	m_filter_dropdown_menu.addSeparator();
	m_filter_dropdown_menu.addItem(FILTER_TYPE_RINGMOD, "Ring Modulator");

	juce::Image dropdown_button1 =
	    ImageCache::getFromMemory(BinaryData::buttondropdown_1_png, BinaryData::buttondropdown_1_pngSize);
	juce::Image dropdown_button2 =
	    ImageCache::getFromMemory(BinaryData::buttondropdown_2_png, BinaryData::buttondropdown_2_pngSize);

	juce::DrawableImage dropdown_draw1;
	juce::DrawableImage dropdown_draw2;

	dropdown_draw1.setImage(dropdown_button1);
	dropdown_draw2.setImage(dropdown_button2);

	m_osc1_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);
	m_osc1_dropdown.setClickingTogglesState(true);

	m_osc1_dropdown.setBounds(
	    DROPDOWN_OSC1_POS_X, DROPDOWN_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_osc1_dropdown.setToggleState(true, dontSendNotification);
	m_osc1_dropdown.setTriggeredOnMouseDown(false);
	m_osc1_dropdown.onClick = [&]() {
		m_osc1_dropdown.setState(Button::ButtonState::buttonNormal);
		m_osc1_dropdown.setToggleState(false, dontSendNotification);
		setOsc1Plate(m_osc_dropdown_menu.show());
	};
	addAndMakeVisible(m_osc1_dropdown);
	m_osc1_dropdown.setTooltip("Select the module to\nbe used for oscillator 1");
	m_osc1_dropdown.setAlwaysOnTop(true);
	m_osc1_dropdown.setState(Button::ButtonState::buttonNormal);
	m_osc1_dropdown.setToggleState(false, dontSendNotification);

	m_osc2_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);
	m_osc1_dropdown.setClickingTogglesState(true);

	m_osc2_dropdown.setBounds(
	    DROPDOWN_OSC2_POS_X, DROPDOWN_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_osc2_dropdown.setToggleState(true, dontSendNotification);
	m_osc2_dropdown.setTriggeredOnMouseDown(false);
	m_osc2_dropdown.onClick = [&]() {
		m_osc2_dropdown.setState(Button::ButtonState::buttonNormal);
		m_osc2_dropdown.setToggleState(false, dontSendNotification);
		setOsc2Plate(m_osc_dropdown_menu.show());
	};
	m_osc2_dropdown.setTooltip("Select the module to\nbe used for oscillator 2");
	addAndMakeVisible(m_osc2_dropdown);
	m_osc2_dropdown.setAlwaysOnTop(true);
	m_osc2_dropdown.setState(Button::ButtonState::buttonNormal);
	m_osc2_dropdown.setToggleState(false, dontSendNotification);

	m_osc3_dropdown.setImages(&dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw1,
	                          &dropdown_draw2);
	m_osc3_dropdown.setClickingTogglesState(true);

	m_osc3_dropdown.setBounds(
	    DROPDOWN_OSC3_POS_X, DROPDOWN_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_osc3_dropdown.setToggleState(true, dontSendNotification);
	m_osc3_dropdown.setTriggeredOnMouseDown(false);
	m_osc3_dropdown.onClick = [&]() {
		m_osc3_dropdown.setState(Button::ButtonState::buttonNormal);
		m_osc3_dropdown.setToggleState(false, dontSendNotification);
		setOsc3Plate(m_osc_dropdown_menu.show());
	};
	m_osc3_dropdown.setTooltip("Select the module to\nbe used for oscillator 3");
	addAndMakeVisible(m_osc3_dropdown);
	m_osc3_dropdown.setAlwaysOnTop(true);
	m_osc3_dropdown.setState(Button::ButtonState::buttonNormal);
	m_osc3_dropdown.setToggleState(false, dontSendNotification);

	m_filter1_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter1_dropdown.setClickingTogglesState(true);

	m_filter1_dropdown.setBounds(
	    DROPDOWN_FILTER1_POS_X, DROPDOWN_FILTER1_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_filter1_dropdown.setToggleState(true, dontSendNotification);
	m_filter1_dropdown.setTriggeredOnMouseDown(false);
	m_filter1_dropdown.onClick = [&]() {
		m_filter1_dropdown.setState(Button::ButtonState::buttonNormal);
		m_filter1_dropdown.setToggleState(false, dontSendNotification);
		setFilter1Plate(m_filter_dropdown_menu.show());
	};
	m_filter1_dropdown.setTooltip("Select the module to\nbe used for filter 1");
	addAndMakeVisible(m_filter1_dropdown);
	m_filter1_dropdown.setAlwaysOnTop(true);
	m_filter1_dropdown.setState(Button::ButtonState::buttonNormal);
	m_filter1_dropdown.setToggleState(false, dontSendNotification);

	m_filter2_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter2_dropdown.setClickingTogglesState(true);

	m_filter2_dropdown.setBounds(
	    DROPDOWN_FILTER2_POS_X, DROPDOWN_FILTER2_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_filter2_dropdown.setToggleState(true, dontSendNotification);
	m_filter2_dropdown.setTriggeredOnMouseDown(false);
	m_filter2_dropdown.onClick = [&]() {
		m_filter2_dropdown.setState(Button::ButtonState::buttonNormal);
		m_filter2_dropdown.setToggleState(false, dontSendNotification);
		setFilter2Plate(m_filter_dropdown_menu.show());
	};
	m_filter2_dropdown.setTooltip("Select the module to\nbe used for filter 2");
	addAndMakeVisible(m_filter2_dropdown);
	m_filter2_dropdown.setAlwaysOnTop(true);
	m_filter2_dropdown.setState(Button::ButtonState::buttonNormal);
	m_filter2_dropdown.setToggleState(false, dontSendNotification);

	m_filter3_dropdown.setImages(&dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw1,
	                             &dropdown_draw2);
	m_filter3_dropdown.setClickingTogglesState(true);

	m_filter3_dropdown.setBounds(
	    DROPDOWN_FILTER3_POS_X, DROPDOWN_FILTER3_POS_Y, dropdown_button1.getWidth(), dropdown_button1.getHeight());
	m_filter3_dropdown.setToggleState(true, dontSendNotification);
	m_filter3_dropdown.setTriggeredOnMouseDown(false);
	m_filter3_dropdown.onClick = [&]() {
		m_filter3_dropdown.setState(Button::ButtonState::buttonNormal);
		m_filter3_dropdown.setToggleState(false, dontSendNotification);
		setFilter3Plate(m_filter_dropdown_menu.show());
	};
	m_filter3_dropdown.setTooltip("Select the module to\nbe used for filter 3");
	addAndMakeVisible(m_filter3_dropdown);
	m_filter3_dropdown.setAlwaysOnTop(true);
	m_filter3_dropdown.setState(Button::ButtonState::buttonNormal);
	m_filter3_dropdown.setToggleState(false, dontSendNotification);

	m_fx_buttons_section.setTopLeftPosition(FX_BUTTON_X, FX_BUTTON_Y);
	m_fx_buttons_section.onButtonArrange = [&](std::map<std::string, int> p_map) { arrangeFXOnButtons(p_map); };
	m_fx_buttons_section.setHighlighted  = [&](std::string p_name) { setActiveFXPanel(p_name); };

	addAndMakeVisible(m_fx_buttons_section);

	juce::Image fx_on_1 = ImageCache::getFromMemory(BinaryData::buttonfxon_1_png, BinaryData::buttonfxon_1_pngSize);
	juce::Image fx_on_2 = ImageCache::getFromMemory(BinaryData::buttonfxon_2_png, BinaryData::buttonfxon_2_pngSize);
	juce::Image fx_on_3 = ImageCache::getFromMemory(BinaryData::buttonfxon_3_png, BinaryData::buttonfxon_3_pngSize);
	juce::Image fx_on_4 = ImageCache::getFromMemory(BinaryData::buttonfxon_4_png, BinaryData::buttonfxon_4_pngSize);

	juce::DrawableImage fx_on_draw1;
	juce::DrawableImage fx_on_draw2;
	juce::DrawableImage fx_on_draw3;
	juce::DrawableImage fx_on_draw4;

	fx_on_draw1.setImage(fx_on_1);
	fx_on_draw2.setImage(fx_on_2);
	fx_on_draw3.setImage(fx_on_3);
	fx_on_draw4.setImage(fx_on_4);

	m_phaser_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_phaser_on_button.setClickingTogglesState(true);
	m_phaser_on_button.setBounds(FX_ON_BUTTON_X, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());
	addAndMakeVisible(m_phaser_on_button);
	m_phaser_on_button.setAlwaysOnTop(true);
	m_phaser_on_button.setTriggeredOnMouseDown(true);
	m_phaser_on_button.setTooltip("Enables the phaser");
	m_phaser_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_flanger_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_flanger_on_button.setClickingTogglesState(true);
	m_flanger_on_button.setBounds(
	    FX_ON_BUTTON_X + FX_BUTTON_OFFSET, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());
	addAndMakeVisible(m_flanger_on_button);
	m_flanger_on_button.setAlwaysOnTop(true);
	m_flanger_on_button.setTriggeredOnMouseDown(true);
	m_flanger_on_button.setTooltip("Enables the flanger");
	m_flanger_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_chorus_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_chorus_on_button.setClickingTogglesState(true);
	m_chorus_on_button.setBounds(
	    FX_ON_BUTTON_X + 2 * FX_BUTTON_OFFSET, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());
	addAndMakeVisible(m_chorus_on_button);
	m_chorus_on_button.setAlwaysOnTop(true);
	m_chorus_on_button.setTriggeredOnMouseDown(true);
	m_chorus_on_button.setTooltip("Enables the chorus");
	m_chorus_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_delay_on_button.setImages(
	    &fx_on_draw2, &fx_on_draw2, &fx_on_draw1, &fx_on_draw1, &fx_on_draw4, &fx_on_draw4, &fx_on_draw3, &fx_on_draw3);
	m_delay_on_button.setClickingTogglesState(true);
	m_delay_on_button.setBounds(
	    FX_ON_BUTTON_X + 3 * FX_BUTTON_OFFSET, FX_ON_BUTTON_Y, fx_on_1.getWidth(), fx_on_1.getHeight());
	addAndMakeVisible(m_delay_on_button);
	m_delay_on_button.setAlwaysOnTop(true);
	m_delay_on_button.setTriggeredOnMouseDown(true);
	m_delay_on_button.setTooltip("Enables the delay");
	m_delay_on_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	juce::Image filter_button1_1 = ImageCache::getFromMemory(BinaryData::button1_1_png, BinaryData::button1_1_pngSize);
	juce::Image filter_button1_2 = ImageCache::getFromMemory(BinaryData::button1_2_png, BinaryData::button1_2_pngSize);
	juce::Image filter_button1_3 = ImageCache::getFromMemory(BinaryData::button1_3_png, BinaryData::button1_3_pngSize);
	juce::Image filter_button1_4 = ImageCache::getFromMemory(BinaryData::button1_4_png, BinaryData::button1_4_pngSize);

	juce::DrawableImage filter_button1_draw1;
	juce::DrawableImage filter_button1_draw2;
	juce::DrawableImage filter_button1_draw3;
	juce::DrawableImage filter_button1_draw4;

	filter_button1_draw1.setImage(filter_button1_1);
	filter_button1_draw2.setImage(filter_button1_2);
	filter_button1_draw3.setImage(filter_button1_3);
	filter_button1_draw4.setImage(filter_button1_4);

	m_filleft_button1.setImages(&filter_button1_draw2,
	                            &filter_button1_draw2,
	                            &filter_button1_draw1,
	                            &filter_button1_draw1,
	                            &filter_button1_draw4,
	                            &filter_button1_draw4,
	                            &filter_button1_draw3,
	                            &filter_button1_draw3);
	m_filleft_button1.setClickingTogglesState(true);
	m_filleft_button1.setBounds(
	    BUTTON_1_LEFT_POS_X, BUTTON_1_LEFT_POS_Y, filter_button1_1.getWidth(), filter_button1_1.getHeight());
	addAndMakeVisible(m_filleft_button1);
	m_filleft_button1.setAlwaysOnTop(true);
	m_filleft_button1.setTriggeredOnMouseDown(true);
	m_filleft_button1.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_filleft_button1.setTooltip("Routes oscillator 1\ninto the filter");

	m_filright_button1.setImages(&filter_button1_draw2,
	                             &filter_button1_draw2,
	                             &filter_button1_draw1,
	                             &filter_button1_draw1,
	                             &filter_button1_draw4,
	                             &filter_button1_draw4,
	                             &filter_button1_draw3,
	                             &filter_button1_draw3);
	m_filright_button1.setClickingTogglesState(true);
	m_filright_button1.setBounds(BUTTON_2_RIGHT_POS_X,
	                             BUTTON_2_RIGHT_POS_Y - filter_button1_1.getHeight(),
	                             filter_button1_1.getWidth(),
	                             filter_button1_1.getHeight());
	addAndMakeVisible(m_filright_button1);
	m_filright_button1.setAlwaysOnTop(true);
	m_filright_button1.setTriggeredOnMouseDown(true);
	m_filright_button1.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_filright_button1.setTooltip("Routes oscillator 1\ninto the filter");

	juce::Image question_button_1 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_1_png, BinaryData::buttonpleb_1_pngSize);
	juce::Image question_button_2 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_2_png, BinaryData::buttonpleb_2_pngSize);
	juce::Image question_button_3 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_3_png, BinaryData::buttonpleb_3_pngSize);
	juce::Image question_button_4 =
	    ImageCache::getFromMemory(BinaryData::buttonpleb_4_png, BinaryData::buttonpleb_4_pngSize);

	juce::DrawableImage question_button_draw1;
	juce::DrawableImage question_button_draw2;
	juce::DrawableImage question_button_draw3;
	juce::DrawableImage question_button_draw4;

	question_button_draw1.setImage(question_button_1);
	question_button_draw2.setImage(question_button_2);
	question_button_draw3.setImage(question_button_3);
	question_button_draw4.setImage(question_button_4);

	m_question_button.setImages(&question_button_draw2,
	                            &question_button_draw2,
	                            &question_button_draw1,
	                            &question_button_draw1,
	                            &question_button_draw4,
	                            &question_button_draw4,
	                            &question_button_draw3,
	                            &question_button_draw3);
	m_question_button.setClickingTogglesState(true);
	m_question_button.setBounds(
	    QUESTION_POS_X, QUESTION_POS_Y, question_button_1.getWidth(), question_button_1.getHeight());
	m_question_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_question_button.onStateChange = [&] {
		// this function is reason for tooltip subclass, else the tooltip will show
		// after window creation for some reason...
		m_tooltip.activate();
		setTooltipEnabled(m_question_button.getToggleState());
	};
	m_question_button.setTooltip(std::string("Shows a tooltip for every\nparameter in the synth. Simply\n hover your "
	                                         "mouse over it!\n\nGENERAL TIPS:\n\nHold shift to finetune knobs\n\nCtr "
	                                         "+ click to reset knobs\n\nDouble click to enter values\n\nRight click "
	                                         "to access MIDI-learn\n\nThe order of FX can be rearranged\nby "
	                                         "dragging and dropping\n the FX selection buttons.\n\n") +
	                             ODIN_VERSION_STRING);
	addAndMakeVisible(m_question_button);

	juce::Image filter_button2_1 = ImageCache::getFromMemory(BinaryData::button2_1_png, BinaryData::button2_1_pngSize);
	juce::Image filter_button2_2 = ImageCache::getFromMemory(BinaryData::button2_2_png, BinaryData::button2_2_pngSize);
	juce::Image filter_button2_3 = ImageCache::getFromMemory(BinaryData::button2_3_png, BinaryData::button2_3_pngSize);
	juce::Image filter_button2_4 = ImageCache::getFromMemory(BinaryData::button2_4_png, BinaryData::button2_4_pngSize);

	juce::DrawableImage filter_button2_draw1;
	juce::DrawableImage filter_button2_draw2;
	juce::DrawableImage filter_button2_draw3;
	juce::DrawableImage filter_button2_draw4;

	filter_button2_draw1.setImage(filter_button2_1);
	filter_button2_draw2.setImage(filter_button2_2);
	filter_button2_draw3.setImage(filter_button2_3);
	filter_button2_draw4.setImage(filter_button2_4);

	m_filleft_button2.setImages(&filter_button2_draw2,
	                            &filter_button2_draw2,
	                            &filter_button2_draw1,
	                            &filter_button2_draw1,
	                            &filter_button2_draw4,
	                            &filter_button2_draw4,
	                            &filter_button2_draw3,
	                            &filter_button2_draw3);
	m_filleft_button2.setClickingTogglesState(true);
	m_filleft_button2.setBounds(BUTTON_1_LEFT_POS_X,
	                            BUTTON_1_LEFT_POS_Y + filter_button2_1.getHeight(),
	                            filter_button2_1.getWidth(),
	                            filter_button2_1.getHeight());
	addAndMakeVisible(m_filleft_button2);
	m_filleft_button2.setTooltip("Routes oscillator 2\ninto the filter");
	m_filleft_button2.setAlwaysOnTop(true);
	m_filleft_button2.setTriggeredOnMouseDown(true);
	m_filleft_button2.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_filright_button2.setImages(&filter_button2_draw2,
	                             &filter_button2_draw2,
	                             &filter_button2_draw1,
	                             &filter_button2_draw1,
	                             &filter_button2_draw4,
	                             &filter_button2_draw4,
	                             &filter_button2_draw3,
	                             &filter_button2_draw3);
	m_filright_button2.setClickingTogglesState(true);
	m_filright_button2.setBounds(
	    BUTTON_2_RIGHT_POS_X, BUTTON_2_RIGHT_POS_Y, filter_button2_1.getWidth(), filter_button2_1.getHeight());
	addAndMakeVisible(m_filright_button2);
	m_filright_button2.setTooltip("Routes oscillator 2\ninto the filter");
	m_filright_button2.setAlwaysOnTop(true);
	m_filright_button2.setTriggeredOnMouseDown(true);
	m_filright_button2.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	juce::Image filter_button3_1 = ImageCache::getFromMemory(BinaryData::button3_1_png, BinaryData::button3_1_pngSize);
	juce::Image filter_button3_2 = ImageCache::getFromMemory(BinaryData::button3_2_png, BinaryData::button3_2_pngSize);
	juce::Image filter_button3_3 = ImageCache::getFromMemory(BinaryData::button3_3_png, BinaryData::button3_3_pngSize);
	juce::Image filter_button3_4 = ImageCache::getFromMemory(BinaryData::button3_4_png, BinaryData::button3_4_pngSize);

	juce::DrawableImage filter_button3_draw1;
	juce::DrawableImage filter_button3_draw2;
	juce::DrawableImage filter_button3_draw3;
	juce::DrawableImage filter_button3_draw4;

	filter_button3_draw1.setImage(filter_button3_1);
	filter_button3_draw2.setImage(filter_button3_2);
	filter_button3_draw3.setImage(filter_button3_3);
	filter_button3_draw4.setImage(filter_button3_4);

	m_filleft_button3.setImages(&filter_button3_draw2,
	                            &filter_button3_draw2,
	                            &filter_button3_draw1,
	                            &filter_button3_draw1,
	                            &filter_button3_draw4,
	                            &filter_button3_draw4,
	                            &filter_button3_draw3,
	                            &filter_button3_draw3);
	m_filleft_button3.setClickingTogglesState(true);
	m_filleft_button3.setBounds(BUTTON_1_LEFT_POS_X,
	                            BUTTON_1_LEFT_POS_Y + 2 * filter_button3_1.getHeight(),
	                            filter_button3_1.getWidth(),
	                            filter_button3_1.getHeight());
	m_filleft_button3.setTooltip("Routes oscillator 3\ninto the filter");
	addAndMakeVisible(m_filleft_button3);
	m_filleft_button3.setAlwaysOnTop(true);
	m_filleft_button3.setTriggeredOnMouseDown(true);
	m_filleft_button3.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_filright_button3.setImages(&filter_button3_draw2,
	                             &filter_button3_draw2,
	                             &filter_button3_draw1,
	                             &filter_button3_draw1,
	                             &filter_button3_draw4,
	                             &filter_button3_draw4,
	                             &filter_button3_draw3,
	                             &filter_button3_draw3);
	m_filright_button3.setClickingTogglesState(true);
	m_filright_button3.setBounds(BUTTON_2_RIGHT_POS_X,
	                             BUTTON_2_RIGHT_POS_Y + filter_button3_1.getHeight(),
	                             filter_button3_1.getWidth(),
	                             filter_button3_1.getHeight());
	addAndMakeVisible(m_filright_button3);
	m_filright_button3.setTooltip("Routes oscillator 3\ninto the filter");
	m_filright_button3.setAlwaysOnTop(true);
	m_filright_button3.setTriggeredOnMouseDown(true);
	m_filright_button3.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	juce::Image filter_buttonf1_1 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_1_png, BinaryData::buttonf1_1_pngSize);
	juce::Image filter_buttonf1_2 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_2_png, BinaryData::buttonf1_2_pngSize);
	juce::Image filter_buttonf1_3 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_3_png, BinaryData::buttonf1_3_pngSize);
	juce::Image filter_buttonf1_4 =
	    ImageCache::getFromMemory(BinaryData::buttonf1_4_png, BinaryData::buttonf1_4_pngSize);

	juce::DrawableImage filter_buttonf1_draw1;
	juce::DrawableImage filter_buttonf1_draw2;
	juce::DrawableImage filter_buttonf1_draw3;
	juce::DrawableImage filter_buttonf1_draw4;

	filter_buttonf1_draw1.setImage(filter_buttonf1_1);
	filter_buttonf1_draw2.setImage(filter_buttonf1_2);
	filter_buttonf1_draw3.setImage(filter_buttonf1_3);
	filter_buttonf1_draw4.setImage(filter_buttonf1_4);

	m_filright_buttonf1.setImages(&filter_buttonf1_draw2,
	                              &filter_buttonf1_draw2,
	                              &filter_buttonf1_draw1,
	                              &filter_buttonf1_draw1,
	                              &filter_buttonf1_draw4,
	                              &filter_buttonf1_draw4,
	                              &filter_buttonf1_draw3,
	                              &filter_buttonf1_draw3);
	m_filright_buttonf1.setClickingTogglesState(true);
	m_filright_buttonf1.setBounds(BUTTON_2_RIGHT_POS_X,
	                              BUTTON_2_RIGHT_POS_Y + 2 * filter_button3_1.getHeight(),
	                              filter_buttonf1_1.getWidth(),
	                              filter_buttonf1_1.getHeight());
	addAndMakeVisible(m_filright_buttonf1);
	m_filright_buttonf1.setTooltip("Routes filter 1\ninto filter 2");
	m_filright_buttonf1.setAlwaysOnTop(true);
	m_filright_buttonf1.setTriggeredOnMouseDown(true);
	m_filright_buttonf1.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	// load background image
	juce::Image odin_backplate =
	    ImageCache::getFromMemory(BinaryData::odin_backplate_png, BinaryData::odin_backplate_pngSize);

	// load the knob strips
	juce::Image metal_knob_small =
	    ImageCache::getFromMemory(BinaryData::metal_knob_small_png, BinaryData::metal_knob_small_pngSize);

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_png, BinaryData::metal_knob_big_pngSize);

	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_png, BinaryData::black_knob_mid_pngSize);

	// load backplates for osc and filters

	juce::Image bypass_osc1_plate =
	    ImageCache::getFromMemory(BinaryData::osc1_bypass_png, BinaryData::osc1_bypass_pngSize);
	juce::Image bypass_osc2_plate =
	    ImageCache::getFromMemory(BinaryData::osc2_bypass_png, BinaryData::osc2_bypass_pngSize);
	juce::Image bypass_osc3_plate =
	    ImageCache::getFromMemory(BinaryData::osc3_bypass_png, BinaryData::osc3_bypass_pngSize);

	juce::Image bypass_fil1_plate =
	    ImageCache::getFromMemory(BinaryData::filter1_bypass_png, BinaryData::filter1_bypass_pngSize);
	juce::Image bypass_fil2_plate =
	    ImageCache::getFromMemory(BinaryData::filter2_bypass_png, BinaryData::filter2_bypass_pngSize);
	juce::Image bypass_fil3_plate =
	    ImageCache::getFromMemory(BinaryData::filter3_bypass_png, BinaryData::filter3_bypass_pngSize);

	m_glide.setStrip(metal_knob_small, N_KNOB_FRAMES);
	m_glide.setSliderStyle(Slider::RotaryVerticalDrag);
	m_glide.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_glide.setKnobTooltip("Makes the pitch glide\nfrom the\nlast key pressed to\nthe current one");
	addAndMakeVisible(m_glide);

	m_master.setStrip(metal_knob_small, N_KNOB_FRAMES);
	m_master.setSliderStyle(Slider::RotaryVerticalDrag);
	m_master.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_master.setKnobTooltip("The master volume\nof the synth");
	addAndMakeVisible(m_master);

	juce::Image pitchwheel = ImageCache::getFromMemory(BinaryData::modwheel_png, BinaryData::modwheel_pngSize);

	m_pitchwheel.setStrip(pitchwheel, N_KNOB_FRAMES, false);
	m_pitchwheel.setSliderStyle(Slider::RotaryVerticalDrag);
	m_pitchwheel.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_pitchwheel.setBounds(PITCHWHEEL_X, WHEEL_Y, pitchwheel.getWidth() / N_KNOB_FRAMES, pitchwheel.getHeight());
	m_pitchwheel.setTooltip("Bends the pitch of all\noscillators up or down.\nYou can specify the "
	                        "amount\nby the selector below");
	addAndMakeVisible(m_pitchwheel);

	m_modwheel.setStrip(pitchwheel, N_KNOB_FRAMES, false);
	m_modwheel.setSliderStyle(Slider::RotaryVerticalDrag);
	m_modwheel.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_modwheel.setBounds(MODWHEEL_X, WHEEL_Y, pitchwheel.getWidth() / N_KNOB_FRAMES, pitchwheel.getHeight());
	m_modwheel.setTooltip("Modwheel to be used\nin the modulation matrix");
	addAndMakeVisible(m_modwheel);

	m_osc1.setBackgroundBypass(bypass_osc1_plate);
	m_osc1.setOscType(OSC_TYPE_ANALOG); // analog
	addAndMakeVisible(m_osc1);
	m_osc2.setBackgroundBypass(bypass_osc2_plate);
	m_osc2.setOscType(1); // chiptune
	addAndMakeVisible(m_osc2);
	m_osc3.setBackgroundBypass(bypass_osc3_plate);
	m_osc3.setOscType(1);
	addAndMakeVisible(m_osc3);

	m_fil1_component.setBackgroundBypass(bypass_fil1_plate);
	m_fil1_component.setFilterType(FILTER_TYPE_LP24); // LP
	addAndMakeVisible(m_fil1_component);
	m_fil2_component.setBackgroundBypass(bypass_fil2_plate);
	m_fil2_component.setFilterType(1); // bypass
	addAndMakeVisible(m_fil2_component);
	m_fil3_component.setBackgroundBypass(bypass_fil3_plate);
	m_fil3_component.setFilterType(1); // bypass
	addAndMakeVisible(m_fil3_component);

	addAndMakeVisible(m_midsection);
	addAndMakeVisible(m_adsr_1);
	addAndMakeVisible(m_adsr_2);
	addChildComponent(m_adsr_3);
	addChildComponent(m_adsr_4);

	juce::Image flanger_image =
	    ImageCache::getFromMemory(BinaryData::flangernosync_png, BinaryData::flangernosync_pngSize);
	juce::Image flanger_image_sync =
	    ImageCache::getFromMemory(BinaryData::flangersync_png, BinaryData::flangersync_pngSize);
	m_flanger.setImage(flanger_image_sync, true);
	m_flanger.setImage(flanger_image, false);
	m_flanger.setSyncTimeColor(FLANGER_DISPLAY_COLOR);
	addChildComponent(m_flanger);

	juce::Image phaser_image =
	    ImageCache::getFromMemory(BinaryData::phasernosync_png, BinaryData::phasernosync_pngSize);
	juce::Image phaser_image_sync =
	    ImageCache::getFromMemory(BinaryData::phasersync_png, BinaryData::phasersync_pngSize);
	m_phaser.setImage(phaser_image_sync, true);
	m_phaser.setImage(phaser_image, false);
	m_phaser.setSyncTimeColor(PHASER_DISPLAY_COLOR);
	addChildComponent(m_phaser);

	juce::Image chorus_image =
	    ImageCache::getFromMemory(BinaryData::chorusnosync_png, BinaryData::chorusnosync_pngSize);
	juce::Image chorus_image_sync =
	    ImageCache::getFromMemory(BinaryData::chorussync_png, BinaryData::chorussync_pngSize);
	m_chorus.setImage(chorus_image_sync, true);
	m_chorus.setImage(chorus_image, false);
	m_chorus.setSyncTimeColor(CHORUS_DISPLAY_COLOR);
	addChildComponent(m_chorus);

	juce::Image delay_image;
	if ((float)m_value_tree.state.getChildWithName("fx")["delay_sync"] < 0.5f) {
		delay_image = ImageCache::getFromMemory(BinaryData::delaynosync_png, BinaryData::delaynosync_pngSize);
	} else {
		delay_image = ImageCache::getFromMemory(BinaryData::delaysync_png, BinaryData::delaysync_pngSize);
	}
	m_delay.setImage(delay_image);
	addAndMakeVisible(m_delay);

	addAndMakeVisible(m_mod_matrix);
	addAndMakeVisible(m_save_load);
	addAndMakeVisible(m_xy_section);

	juce::Image env13_left =
	    ImageCache::getFromMemory(BinaryData::buttonenv13_1_png, BinaryData::buttonenv13_1_pngSize);
	juce::Image env13_right =
	    ImageCache::getFromMemory(BinaryData::buttonenv13_3_png, BinaryData::buttonenv13_3_pngSize);
	m_env_13_button.setImage(env13_left, 1);
	m_env_13_button.setImage(env13_right, 2);
	m_env_13_button.setBounds(ENV13_POS_X, ENV13_POS_Y, env13_left.getWidth(), env13_left.getHeight());
	m_env_13_button.setToggleState(true, dontSendNotification);
	m_env_13_button.onStateChange = [&]() {
		setEnv13(m_env_13_button.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty(
		    "env_left_selected", (int)m_env_13_button.getToggleState(), nullptr);
	};
	m_env_13_button.setTooltip("Shows the amplifier\nenvelope or envelope 3");
	addAndMakeVisible(m_env_13_button);

	juce::Image env24_left =
	    ImageCache::getFromMemory(BinaryData::buttonenv24_1_png, BinaryData::buttonenv24_1_pngSize);
	juce::Image env24_right =
	    ImageCache::getFromMemory(BinaryData::buttonenv24_3_png, BinaryData::buttonenv24_3_pngSize);
	m_env_24_button.setImage(env24_left, 1);
	m_env_24_button.setImage(env24_right, 2);
	m_env_24_button.setBounds(ENV24_POS_X, ENV24_POS_Y, env24_left.getWidth(), env24_left.getHeight());
	m_env_24_button.setToggleState(true, dontSendNotification);
	m_env_24_button.onStateChange = [&]() {
		setEnv24(m_env_24_button.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty(
		    "env_right_selected", (int)m_env_24_button.getToggleState(), nullptr);
	};
	m_env_24_button.setTooltip("Shows the filter\nenvelope or envelope 4");
	addAndMakeVisible(m_env_24_button);

	juce::Image lfo13_left =
	    ImageCache::getFromMemory(BinaryData::buttonlfo13_1_png, BinaryData::buttonlfo13_1_pngSize);
	juce::Image lfo13_right =
	    ImageCache::getFromMemory(BinaryData::buttonlfo13_3_png, BinaryData::buttonlfo13_3_pngSize);
	m_lfo_13_button.setImage(lfo13_left, 1);
	m_lfo_13_button.setImage(lfo13_right, 2);
	m_lfo_13_button.setBounds(LFO13_POS_X, LFO13_POS_Y, lfo13_left.getWidth(), lfo13_left.getHeight());
	m_lfo_13_button.setToggleState(true, dontSendNotification);
	m_lfo_13_button.onStateChange = [&]() {
		setLfo12(m_lfo_13_button.getToggleState());
		m_value_tree.state.getChildWithName("lfo").setProperty(
		    "lfo_left_selected", (int)m_lfo_13_button.getToggleState(), nullptr);
	};
	m_lfo_13_button.setTooltip("Shows LFO 1 or LFO 3");
	addAndMakeVisible(m_lfo_13_button);

	juce::Image lfo24_left =
	    ImageCache::getFromMemory(BinaryData::buttonlfo24_1_png, BinaryData::buttonlfo24_1_pngSize);
	juce::Image lfo24_right =
	    ImageCache::getFromMemory(BinaryData::buttonlfo24_3_png, BinaryData::buttonlfo24_3_pngSize);
	m_lfo_24_button.setImage(lfo24_left, 1);
	m_lfo_24_button.setImage(lfo24_right, 2);
	m_lfo_24_button.setBounds(LFO24_POS_X, LFO24_POS_Y, lfo24_left.getWidth(), lfo24_left.getHeight());
	m_lfo_24_button.setToggleState(true, dontSendNotification);
	m_lfo_24_button.onStateChange = [&]() {
		setLfo34(m_lfo_24_button.getToggleState());
		m_value_tree.state.getChildWithName("lfo").setProperty(
		    "lfo_right_selected", (int)m_lfo_24_button.getToggleState(), nullptr);
	};
	m_lfo_24_button.setTooltip("Shows LFO 2 or LFO 4");
	addAndMakeVisible(m_lfo_24_button);

	juce::Image legato_left =
	    ImageCache::getFromMemory(BinaryData::buttonlegato_1_png, BinaryData::buttonlegato_1_pngSize);
	juce::Image legato_right =
	    ImageCache::getFromMemory(BinaryData::buttonlegato_3_png, BinaryData::buttonlegato_3_pngSize);
	m_legato_button.setImage(legato_left, 1);
	m_legato_button.setImage(legato_right, 2);
	m_legato_button.setBounds(LEGATO_POS_X, LEGATO_POS_Y, legato_left.getWidth(), legato_left.getHeight());
	m_legato_button.setToggleState(true, sendNotification);
	m_legato_button.onClick = [&]() {
		// set value in audiovaluetree
		m_value_tree.state.getChildWithName("misc").setProperty("legato", m_legato_button.getToggleState(), nullptr);
		// notify voice manager this will reset synth
		processor.setPolyLegato(m_legato_button.getToggleState());
	};
	m_legato_button.setTooltip("Sets the synth to legato or polyphonic mode. In poly mode you have 12 "
	                           "voices, in legato you only have one and the envelopes tied together. "
	                           "This can be usefull for melodies, since notes don't overlap and there's "
	                           "no hard envelope reset when transitioning from one note to another.");
	addAndMakeVisible(m_legato_button);

	juce::Image lfo13_sync_background =
	    ImageCache::getFromMemory(BinaryData::lfo13_sync_background_png, BinaryData::lfo13_sync_background_pngSize);

	juce::Image lfo24_sync_background =
	    ImageCache::getFromMemory(BinaryData::lfo24_sync_background_png, BinaryData::lfo24_sync_background_pngSize);
	m_lfo_1.setBounds(LFO_LEFT_POS_X, LFO_LEFT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_1.setSyncOverdraw(lfo13_sync_background);
	addAndMakeVisible(m_lfo_1);
	m_lfo_2.setBounds(LFO_LEFT_POS_X, LFO_LEFT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_2.setSyncOverdraw(lfo24_sync_background);
	addChildComponent(m_lfo_2);
	m_lfo_3.setBounds(LFO_RIGHT_POS_X, LFO_RIGHT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_3.setSyncOverdraw(lfo13_sync_background);
	addAndMakeVisible(m_lfo_3);
	m_lfo_4.setBounds(LFO_RIGHT_POS_X, LFO_RIGHT_POS_Y, LFO_SIZE_X, LFO_SIZE_Y);
	m_lfo_4.setSyncOverdraw(lfo24_sync_background);
	addChildComponent(m_lfo_4);

	m_pitch_amount.OnValueChange = [&](int p_new_value) {
		m_value_tree.getParameter(m_pitchbend_amount_identifier)->setValueNotifyingHost(((float)p_new_value) / 24.f);
	};
	m_pitch_amount.setTopLeftPosition(PITCH_AMOUNT_X, PITCH_AMOUNT_Y);
	addAndMakeVisible(m_pitch_amount);
	m_pitch_amount.setMouseDragDivisor(20.f);
	m_pitch_amount.setRange(0, 24);
	// m_pitch_amount.setValue(12);
	m_pitch_amount.setColor(Colour(10, 40, 50));
	m_pitch_amount.setTooltip("The amount of pitchbend for the pitchwheel in semitones");

	m_BPM_selector.OnValueChange = [&](int p_new_value) {
		m_value_tree.state.getChildWithName("misc").setProperty("BPM", p_new_value, nullptr);
	};
	m_BPM_selector.setTopLeftPosition(BPM_POS_X, BPM_POS_Y);
	m_BPM_selector.setRange(10, 240);
	// m_BPM_selector.setValue(120);
	m_BPM_selector.setColor(Colour(10, 40, 50));
	m_BPM_selector.setTooltip("A BPM value for all time synced components in Odin standalone");
	if (m_is_standalone_plugin) {
		addAndMakeVisible(m_BPM_selector);
	}

	m_osc_dropdown_menu.setLookAndFeel(&m_menu_feels);
	m_filter_dropdown_menu.setLookAndFeel(&m_menu_feels);

	m_tooltip.setLookAndFeel(&m_tooltip_feels);

	m_phaser_on_attachment.reset(new ButtonAttachment(m_value_tree, "phaser_on", m_phaser_on_button));
	m_flanger_on_attachment.reset(new ButtonAttachment(m_value_tree, "flanger_on", m_flanger_on_button));
	m_delay_on_attachment.reset(new ButtonAttachment(m_value_tree, "delay_on", m_delay_on_button));
	m_chorus_on_attachment.reset(new ButtonAttachment(m_value_tree, "chorus_on", m_chorus_on_button));
	m_fil1_osc1_attachment.reset(new ButtonAttachment(m_value_tree, "fil1_osc1", m_filleft_button1));
	m_fil1_osc2_attachment.reset(new ButtonAttachment(m_value_tree, "fil1_osc2", m_filleft_button2));
	m_fil1_osc3_attachment.reset(new ButtonAttachment(m_value_tree, "fil1_osc3", m_filleft_button3));
	m_fil2_osc1_attachment.reset(new ButtonAttachment(m_value_tree, "fil2_osc1", m_filright_button1));
	m_fil2_osc2_attachment.reset(new ButtonAttachment(m_value_tree, "fil2_osc2", m_filright_button2));
	m_fil2_osc3_attachment.reset(new ButtonAttachment(m_value_tree, "fil2_osc3", m_filright_button3));
	m_fil2_fil1_attachment.reset(new ButtonAttachment(m_value_tree, "fil2_fil1", m_filright_buttonf1));

	m_glide_attachment.reset(new SliderAttachment(m_value_tree, "glide", m_glide));
	m_master_attachment.reset(new SliderAttachment(m_value_tree, "master", m_master));
	m_modwheel_attachment.reset(new SliderAttachment(m_value_tree, "modwheel", m_modwheel));
	m_pitchbend_attachment.reset(new SliderAttachment(m_value_tree, "pitchbend", m_pitchwheel));

	m_master.setNumDecimalPlacesToDisplay(2);
	m_glide.setNumDecimalPlacesToDisplay(3);
	m_modwheel.setNumDecimalPlacesToDisplay(3);
	m_pitchwheel.setNumDecimalPlacesToDisplay(3);

	m_pitch_amount.setParameterId("pitchbend_amount");
	m_value_tree.addParameterListener("pitchbend_amount", &m_pitch_amount);

	m_value_input.setBounds(0, 0, INPUT_LABEL_SIZE_X, INPUT_LABEL_SIZE_Y);
	m_value_input.setComponentID("value_input");
	m_value_input.setAlwaysOnTop(true);
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

	//m_color_picker.setTopLeftPosition(ADSR_LEFT_POS_X, ADSR_LEFT_POS_Y);
	//m_color_picker.setSize(3 * ADSR_SIZE_X, 3 * ADSR_SIZE_Y);

	//addAndMakeVisible(m_color_picker);
	//m_color_picker.setAlwaysOnTop(true);
	//m_color_picker.setSwatchColour(1, juce::Colour(71, 92, 108));

	//m_osc1.setColorPickerPointer(&m_color_picker);
	//m_osc2.setColorPickerPointer(&m_color_picker);
	//m_osc3.setColorPickerPointer(&m_color_picker);

#ifdef WTGEN
	m_spectrum_display.setTopLeftPosition(800 - 512 - 2 * 6, 400 - 2 * 6);
	addAndMakeVisible(m_wavetable_display);
	m_wavetable_display.setTopLeftPosition(800 - 512 - 2 * 6, 400 - 4 * 6 - 200);
	addAndMakeVisible(m_spectrum_display);
	m_wavetable_display.setAlwaysOnTop(true);
	m_spectrum_display.setAlwaysOnTop(true);
#endif

	Rectangle<int> area_osc_1(OSC_SIZE_X, OSC_SIZE_Y);
	Rectangle<int> area_osc_2(OSC_SIZE_X, OSC_SIZE_Y);
	Rectangle<int> area_osc_3(OSC_SIZE_X, OSC_SIZE_Y);
	Rectangle<int> area_fil_1(FILTER_SIZE_X, FILTER_SIZE_Y);
	Rectangle<int> area_midsection(MIDSECTION_SIZE_X, MIDSECTION_SIZE_Y);

	area_osc_1.setPosition(OSC1_POS_X, OSC_POS_Y);
	area_osc_2.setPosition(OSC2_POS_X, OSC_POS_Y);
	area_osc_3.setPosition(OSC3_POS_X, OSC_POS_Y);
	area_fil_1.setPosition(FIL1_POS_X, FIL1_POS_Y);
	area_midsection.setPosition(MIDSECTION_POS_X, MIDSECTION_POS_Y);

	m_glide.setBounds(GLIDE_POS_X - METAL_KNOB_SMALL_OFFSET_X,
	                  GLIDE_POS_Y - METAL_KNOB_SMALL_OFFSET_Y,
	                  METAL_KNOB_SMALL_SIZE_X,
	                  METAL_KNOB_SMALL_SIZE_Y);
	m_master.setRange(MASTER_MIN, MASTER_MAX);
	m_master.setTextValueSuffix(" dB");
	m_master.setNumDecimalPlacesToDisplay(1);

	m_master.setBounds(MASTER_POS_X - METAL_KNOB_SMALL_OFFSET_X,
	                   MASTER_POS_Y - METAL_KNOB_SMALL_OFFSET_Y,
	                   METAL_KNOB_SMALL_SIZE_X,
	                   METAL_KNOB_SMALL_SIZE_Y);

	m_osc1.setBounds(area_osc_1);
	m_osc2.setBounds(area_osc_2);
	m_osc3.setBounds(area_osc_3);

	m_fil1_component.setBounds(area_fil_1);
	m_fil2_component.setBounds(FIL2_POS_X, FIL2_POS_Y, FILTER_SIZE_X, FILTER_SIZE_Y);
	m_fil3_component.setBounds(FIL3_POS_X, FIL3_POS_Y, FILTER_SIZE_X, FILTER_SIZE_Y);
	m_midsection.setBounds(area_midsection);

	m_adsr_1.setBounds(ADSR_LEFT_POS_X, ADSR_LEFT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);
	m_adsr_2.setBounds(ADSR_RIGHT_POS_X, ADSR_RIGHT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);
	m_adsr_3.setBounds(ADSR_LEFT_POS_X, ADSR_LEFT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);
	m_adsr_4.setBounds(ADSR_RIGHT_POS_X, ADSR_RIGHT_POS_Y, ADSR_SIZE_X, ADSR_SIZE_Y);

	m_flanger.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);
	m_phaser.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);
	m_chorus.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);
	m_delay.setBounds(FX_AREA_POS_X, FX_AREA_POS_Y, FX_AREA_SIZE_X, FX_AREA_SIZE_Y);

	m_mod_matrix.setBounds(MATRIX_POS_X, MATRIX_POS_Y, MATRIX_SIZE_X, MATRIX_SIZE_Y);
	m_save_load.setBounds(SAVE_LOAD_POS_X, SAVE_LOAD_POS_Y, SAVE_LOAD_SIZE_X, SAVE_LOAD_SIZE_Y);
	m_xy_section.setBounds(XY_COMPONENT_POS_X, XY_COMPONENT_POS_Y, XY_COMPONENT_SIZE_X, XY_COMPONENT_SIZE_Y);

	m_tooltip.setBounds(900, 100, 100, 100);
	m_tooltip.setAlwaysOnTop(true);
	setTooltipEnabled(false);
	addAndMakeVisible(m_tooltip);

	setSize(800, 600);

	forceValueTreeOntoComponentsOnlyMainPanel();

	if (!writeComponentImageToFile(*this)) {
		DBG("Failed to create GUI screenshot");
	}
	writeValueTreeToFile(m_value_tree.state);
}

OdinAudioProcessorEditor::~OdinAudioProcessorEditor() {
	m_osc_dropdown_menu.setLookAndFeel(nullptr);
	m_filter_dropdown_menu.setLookAndFeel(nullptr);
	m_tooltip.setLookAndFeel(nullptr);
	m_value_input.setLookAndFeel(nullptr);

	m_value_tree.removeParameterListener("pitchbend_amount", &m_pitch_amount);
}

//==============================================================================
void OdinAudioProcessorEditor::paint(Graphics &g) {
	g.drawImageAt(ImageCache::getFromMemory(BinaryData::odin_backdrop_png, BinaryData::odin_backdrop_pngSize), 0, 0);
}

void OdinAudioProcessorEditor::resized() {
	//this is the only valid call to resized, since this window is not allowed to change size
	setSize(800, 600);
}

void OdinAudioProcessorEditor::setOsc1Plate(int p_osc_type) {
	// DBG(p_osc_type);
	if (p_osc_type == 0) {
		return;
	}
	m_osc1.setOscType(p_osc_type);
	m_value_tree.state.getChildWithName("osc").setProperty(m_osc1_type_indentifier, p_osc_type, nullptr);
}

void OdinAudioProcessorEditor::setOsc2Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_osc2.setOscType(p_osc_type);
	m_value_tree.state.getChildWithName("osc").setProperty(m_osc2_type_indentifier, p_osc_type, nullptr);
}

void OdinAudioProcessorEditor::setOsc3Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_osc3.setOscType(p_osc_type);
	m_value_tree.state.getChildWithName("osc").setProperty(m_osc3_type_indentifier, p_osc_type, nullptr);
}

void OdinAudioProcessorEditor::setFilter1Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_fil1_component.setFilterType(p_osc_type);
	m_value_tree.state.getChildWithName("misc").setProperty(m_fil1_type_indentifier, p_osc_type, nullptr);
}

void OdinAudioProcessorEditor::setFilter2Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_fil2_component.setFilterType(p_osc_type);
	m_value_tree.state.getChildWithName("misc").setProperty(m_fil2_type_indentifier, p_osc_type, nullptr);
}

void OdinAudioProcessorEditor::setFilter3Plate(int p_osc_type) {
	if (p_osc_type == 0) {
		return;
	}
	m_fil3_component.setFilterType(p_osc_type);
	m_value_tree.state.getChildWithName("misc").setProperty(m_fil3_type_indentifier, p_osc_type, nullptr);
}

void OdinAudioProcessorEditor::setEnv13(bool p_env1) {
	if (p_env1) {
		m_adsr_1.setVisible(true);
		m_adsr_3.setVisible(false);
	} else {
		m_adsr_1.setVisible(false);
		m_adsr_3.setVisible(true);
	}
}

void OdinAudioProcessorEditor::setEnv24(bool p_env2) {
	if (p_env2) {
		m_adsr_2.setVisible(true);
		m_adsr_4.setVisible(false);
	} else {
		m_adsr_2.setVisible(false);
		m_adsr_4.setVisible(true);
	}
}

void OdinAudioProcessorEditor::setLfo12(bool p_lfo1) {
	if (p_lfo1) {
		m_lfo_1.setVisible(true);
		m_lfo_2.setVisible(false);
	} else {
		m_lfo_1.setVisible(false);
		m_lfo_2.setVisible(true);
	}
}

void OdinAudioProcessorEditor::setLfo34(bool p_lfo2) {
	if (p_lfo2) {
		m_lfo_3.setVisible(true);
		m_lfo_4.setVisible(false);
	} else {
		m_lfo_3.setVisible(false);
		m_lfo_4.setVisible(true);
	}
}

void OdinAudioProcessorEditor::arrangeFXOnButtons(std::map<std::string, int> p_map) {

	m_flanger_on_button.setTopLeftPosition(FX_ON_BUTTON_X + p_map.find("flanger")->second * FX_BUTTON_OFFSET,
	                                       FX_ON_BUTTON_Y);
	m_phaser_on_button.setTopLeftPosition(FX_ON_BUTTON_X + p_map.find("phaser")->second * FX_BUTTON_OFFSET,
	                                      FX_ON_BUTTON_Y);
	m_chorus_on_button.setTopLeftPosition(FX_ON_BUTTON_X + p_map.find("chorus")->second * FX_BUTTON_OFFSET,
	                                      FX_ON_BUTTON_Y);
	m_delay_on_button.setTopLeftPosition(FX_ON_BUTTON_X + p_map.find("delay")->second * FX_BUTTON_OFFSET,
	                                     FX_ON_BUTTON_Y);

	m_value_tree.state.getChildWithName("fx").setProperty(
	    m_delay_position_identifier, (float)p_map.find("delay")->second, nullptr);
	m_value_tree.state.getChildWithName("fx").setProperty(
	    m_phaser_position_identifier, (float)p_map.find("phaser")->second, nullptr);
	m_value_tree.state.getChildWithName("fx").setProperty(
	    m_flanger_position_identifier, (float)p_map.find("flanger")->second, nullptr);
	m_value_tree.state.getChildWithName("fx").setProperty(
	    m_chorus_position_identifier, (float)p_map.find("chorus")->second, nullptr);

	processor.setFXButtonsPosition((float)p_map.find("delay")->second,
	                               (float)p_map.find("phaser")->second,
	                               (float)p_map.find("flanger")->second,
	                               (float)p_map.find("chorus")->second);
}

void OdinAudioProcessorEditor::setActiveFXPanel(std::string p_name) {
	m_phaser.setVisible(false);
	m_flanger.setVisible(false);
	m_chorus.setVisible(false);
	m_delay.setVisible(false);

	int fx = 0;
	fx     = p_name == "phaser" ? 1 : fx;
	fx     = p_name == "flanger" ? 2 : fx;
	fx     = p_name == "chorus" ? 3 : fx;
	fx     = p_name == "delay" ? 4 : fx;

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
	default:
		m_delay.setVisible(true);
		break;
	}
}

void OdinAudioProcessorEditor::setTooltipEnabled(bool p_enabled) {
	if (p_enabled) {
		m_tooltip.setMillisecondsBeforeTipAppears(0);
	} else {
		//unelegant solution
		m_tooltip.setMillisecondsBeforeTipAppears(2047483647);
		m_tooltip.hideTip();
	}
}

void OdinAudioProcessorEditor::forceValueTreeOntoComponentsOnlyMainPanel() {
	m_pitch_amount.setValue(m_value_tree.getParameterAsValue("pitchbend_amount").getValue());

	// ugly fix to set highlighted fx panel
	std::string fx_name = "delay";
	if ((float)m_value_tree.state.getChildWithName("fx")["phaser_selected"] > 0.5) {
		fx_name = "phaser";
	} else if ((float)m_value_tree.state.getChildWithName("fx")["flanger_selected"] > 0.5) {
		fx_name = "flanger";
	} else if ((float)m_value_tree.state.getChildWithName("fx")["chorus_selected"] > 0.5) {
		fx_name = "chorus";
	}
	setActiveFXPanel(fx_name);

	m_legato_button.setToggleState((float)m_value_tree.state.getChildWithName("misc")["legato"] > 0.5,
	                               dontSendNotification);
	processor.setPolyLegato(m_legato_button.getToggleState());

	m_BPM_selector.setValue(m_value_tree.state.getChildWithName("misc")["BPM"]);

	m_env_13_button.setToggleState((float)m_value_tree.state.getChildWithName("misc")["env_left_selected"] > 0.5,
	                               dontSendNotification);
	setEnv13(m_env_13_button.getToggleState());
	m_env_24_button.setToggleState((float)m_value_tree.state.getChildWithName("misc")["env_right_selected"] > 0.5,
	                               dontSendNotification);
	setEnv24(m_env_24_button.getToggleState());

	m_lfo_13_button.setToggleState((float)m_value_tree.state.getChildWithName("lfo")["lfo_left_selected"] > 0.5,
	                               dontSendNotification);
	setLfo12(m_lfo_13_button.getToggleState());
	m_lfo_24_button.setToggleState((float)m_value_tree.state.getChildWithName("lfo")["lfo_right_selected"] > 0.5,
	                               dontSendNotification);
	setLfo34(m_lfo_24_button.getToggleState());
}

void OdinAudioProcessorEditor::forceValueTreeOntoComponents(bool p_reset_audio) {

	// reset audioengine
	if (p_reset_audio) {
		processor.resetAudioEngine();
	}

	DBG("FORCEVALUETREEONTOCOMPONENTS");
	// DBG(m_value_tree.state.toXmlString());

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
}

bool OdinAudioProcessorEditor::keyStateChanged(bool isKeyDown, Component *originatingComponent) {

	if (m_is_standalone_plugin) {

		if (KeyPress::isKeyCurrentlyDown(65) != m_A_down) {
			m_A_down = KeyPress::isKeyCurrentlyDown(65);
			if (m_A_down) {
				processor.midiNoteOn(48 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(48 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(87) != m_W_down) {
			m_W_down = KeyPress::isKeyCurrentlyDown(87);
			if (m_W_down) {
				processor.midiNoteOn(49 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(49 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(83) != m_S_down) {
			m_S_down = KeyPress::isKeyCurrentlyDown(83);
			if (m_S_down) {
				processor.midiNoteOn(50 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(50 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(69) != m_E_down) {
			m_E_down = KeyPress::isKeyCurrentlyDown(69);
			if (m_E_down) {
				processor.midiNoteOn(51 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(51 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(68) != m_D_down) {
			m_D_down = KeyPress::isKeyCurrentlyDown(68);
			if (m_D_down) {
				processor.midiNoteOn(52 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(52 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(70) != m_F_down) {
			m_F_down = KeyPress::isKeyCurrentlyDown(70);
			if (m_F_down) {
				processor.midiNoteOn(53 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(53 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(84) != m_T_down) {
			m_T_down = KeyPress::isKeyCurrentlyDown(84);
			if (m_T_down) {
				processor.midiNoteOn(54 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(54 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(71) != m_G_down) {
			m_G_down = KeyPress::isKeyCurrentlyDown(71);
			if (m_G_down) {
				processor.midiNoteOn(55 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(55 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(90) != m_Z_down) {
			m_Z_down = KeyPress::isKeyCurrentlyDown(90);
			if (m_Z_down) {
				processor.midiNoteOn(56 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(56 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(72) != m_H_down) {
			m_H_down = KeyPress::isKeyCurrentlyDown(72);
			if (m_H_down) {
				processor.midiNoteOn(57 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(57 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(85) != m_U_down) {
			m_U_down = KeyPress::isKeyCurrentlyDown(85);
			if (m_U_down) {
				processor.midiNoteOn(58 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(58 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(74) != m_J_down) {
			m_J_down = KeyPress::isKeyCurrentlyDown(74);
			if (m_J_down) {
				processor.midiNoteOn(59 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(59 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(75) != m_K_down) {
			m_K_down = KeyPress::isKeyCurrentlyDown(75);
			if (m_K_down) {
				processor.midiNoteOn(60 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(60 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(79) != m_O_down) {
			m_O_down = KeyPress::isKeyCurrentlyDown(79);
			if (m_O_down) {
				processor.midiNoteOn(61 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(61 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(76) != m_L_down) {
			m_L_down = KeyPress::isKeyCurrentlyDown(76);
			if (m_L_down) {
				processor.midiNoteOn(62 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(62 + m_octave_shift * 12);
			}
		}
		if (KeyPress::isKeyCurrentlyDown(80) != m_P_down) {
			m_P_down = KeyPress::isKeyCurrentlyDown(80);
			if (m_P_down) {
				processor.midiNoteOn(63 + m_octave_shift * 12, 100);
			} else {
				processor.midiNoteOff(63 + m_octave_shift * 12);
			}
		}
	}
	return false;
}

void OdinAudioProcessorEditor::allMidiKeysOff() {
	// this is the SHITTIEST implementation ever
	// but it gets the job done and is called every moon :-)
	// (gets called only for (PC-)keyboard octave change)
	for (int note = 0; note < 127; ++note) {
		processor.midiNoteOff(note);
	}
}

void OdinAudioProcessorEditor::updatePitchWheel(float p_value) {
	const MessageManagerLock mmLock;
	m_pitchwheel.setValue(p_value);
}

void OdinAudioProcessorEditor::updateModWheel(float p_value) {
	const MessageManagerLock mmLock;
	m_modwheel.setValue(p_value, sendNotificationSync);
}