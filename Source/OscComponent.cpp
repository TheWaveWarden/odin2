/*
  ==============================================================================

    OscComponent.cpp
    Created: 25 Feb 2019 12:31:26am
    Author:  frot

  ==============================================================================
*/

#include "OscComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
OscComponent::OscComponent(OdinAudioProcessor &p_processor,
                           AudioProcessorValueTreeState &vts,
                           std::string p_osc_number)
    : m_value_tree(vts),
      m_reset("reset_button", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_LED_saw("LED_Saw", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_LED_pulse("LED_pulse", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_LED_triangle("LED_triangle",
                     juce::DrawableButton::ButtonStyle::ImageRaw),
      m_LED_sine("LED_sine", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_arp("arp", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_step_button("step", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_noise("noise", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_chipdraw_convert("convert_chipdraw",
                         juce::DrawableButton::ButtonStyle::ImageRaw),
      m_wavedraw_convert("convert_wavedraw",
                         juce::DrawableButton::ButtonStyle::ImageRaw),
      m_wavedraw_convert_REMOVE("convert_wavedrawREMOVE",
                                juce::DrawableButton::ButtonStyle::ImageRaw),
      m_chipdraw_convert_REMOVE("convert_wadsvedrawREMOVE",
                                juce::DrawableButton::ButtonStyle::ImageRaw),
      m_specdraw_convert_REMOVE("convert_wdavedrawREMOVE",
                                juce::DrawableButton::ButtonStyle::ImageRaw),
      m_specdraw_convert("convert_wavedraw",
                         juce::DrawableButton::ButtonStyle::ImageRaw),
      m_chiptune_waveselector(true), m_carrier_waveselector(false),
      m_modulator_waveselector(true), m_wavetable_waveselector(true),
      m_carrier_ratio(false), m_modulator_ratio(true), m_fm_exp("fm_exp"),
      m_xy(m_xy_x, m_xy_y, true), m_osc_number(p_osc_number),
      m_wavetable_identifier("osc" + p_osc_number + "_wavetable"),
      m_chipwave_identifier("osc" + p_osc_number + "_chipwave"),
      m_modulator_wave_identifier("osc" + p_osc_number + "_modulator_wave"),
      m_carrier_wave_identifier("osc" + p_osc_number + "_carrier_wave"),
      m_modulator_ratio_identifier("osc" + p_osc_number + "_modulator_ratio"),
      m_carrier_ratio_identifier("osc" + p_osc_number + "_carrier_ratio"),
      m_analog_wave_identifier("osc" + p_osc_number + "_analog_wave"),
      m_vec_a_identifier("osc" + p_osc_number + "_vec_a"),
      m_vec_b_identifier("osc" + p_osc_number + "_vec_b"),
      m_vec_c_identifier("osc" + p_osc_number + "_vec_c"),
      m_vec_d_identifier("osc" + p_osc_number + "_vec_d") {

  TIMESTART("oscconstructor");

  m_oct_attach.reset(
      new SliderAttachment(m_value_tree, "osc" + m_osc_number + "_oct", m_oct));
  m_semi_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_semi", m_semi));
  m_fine_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_fine", m_fine));
  m_vol_attach.reset(
      new SliderAttachment(m_value_tree, "osc" + m_osc_number + "_vol", m_vol));
  m_position_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_position", m_position));
  m_detune_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_detune", m_detune));
  m_multi_position_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_multi_position",
      m_position_multi));
  m_spread_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_spread", m_spread));
  m_pulsewidth_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_pulsewidth", m_pw));
  m_drift_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_drift", m_drift));
  m_arp_speed_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_arp_speed", m_speed));
  m_step_1_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_step_1", m_step_1));
  m_step_2_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_step_2", m_step_2));
  m_step_3_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_step_3", m_step_3));
  m_fm_attach.reset(
      new SliderAttachment(m_value_tree, "osc" + m_osc_number + "_fm", m_fm));
  m_lp_attach.reset(
      new SliderAttachment(m_value_tree, "osc" + m_osc_number + "_lp", m_lp));
  m_hp_attach.reset(
      new SliderAttachment(m_value_tree, "osc" + m_osc_number + "_hp", m_hp));
  m_x_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_vec_x", m_xy_x));
  m_y_attach.reset(new SliderAttachment(
      m_value_tree, "osc" + m_osc_number + "_vec_y", m_xy_y));

  m_reset_attach.reset(new ButtonAttachment(
      m_value_tree, "osc" + m_osc_number + "_reset", m_reset));
  m_arp_on_attach.reset(new ButtonAttachment(
      m_value_tree, "osc" + m_osc_number + "_arp_on", m_arp));
  m_step_3_on_attach.reset(new ButtonAttachment(
      m_value_tree, "osc" + m_osc_number + "_step_3_on", m_step_button));
  m_chipnoise_attach.reset(new ButtonAttachment(
      m_value_tree, "osc" + m_osc_number + "_chipnoise", m_noise));
  m_exp_fm_attach.reset(new ButtonAttachment(
      m_value_tree, "osc" + m_osc_number + "_exp_fm", m_fm_exp));

  // m_vec_a_attach.reset(new ComboBoxAttachment(
  //    m_value_tree, "osc" + m_osc_number + "_vec_a", m_vec_a));
  // m_vec_b_attach.reset(new ComboBoxAttachment(
  //    m_value_tree, "osc" + m_osc_number + "_vec_b", m_vec_b));
  // m_vec_c_attach.reset(new ComboBoxAttachment(
  //    m_value_tree, "osc" + m_osc_number + "_vec_c", m_vec_c));
  // m_vec_d_attach.reset(new ComboBoxAttachment(
  //    m_value_tree, "osc" + m_osc_number + "_vec_d", m_vec_d));

  // m_fine.setOdinPointer(&p_processor);

  m_vol.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      N_KNOB_FRAMES);
  m_vol.setSliderStyle(Slider::RotaryVerticalDrag);
  m_vol.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_vol.setRange(VOL_MIN, VOL_MAX);
  m_vol.setTextValueSuffix(" dB");
  // m_vol.setValue(GETAUDIO("osc" + m_osc_number + "_vol"));
  m_vol.setNumDecimalPlacesToDisplay(1);
  m_vol.setKnobTooltip("The volume of the oscillator");
  addChildComponent(m_vol);

  Rectangle<int> area_vol(BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);
  area_vol.setPosition(VOL_POS_X - BLACK_KNOB_SMALL_OFFSET_X,
                       VOL_POS_Y - BLACK_KNOB_SMALL_OFFSET_Y);
  m_vol.setBounds(area_vol);

  juce::Image reset_1 = ImageCache::getFromMemory(
      BinaryData::buttonreset_osc_1_png, BinaryData::buttonreset_osc_1_pngSize);
  juce::Image reset_2 = ImageCache::getFromMemory(
      BinaryData::buttonreset_osc_2_png, BinaryData::buttonreset_osc_2_pngSize);
  juce::Image reset_3 = ImageCache::getFromMemory(
      BinaryData::buttonreset_osc_3_png, BinaryData::buttonreset_osc_3_pngSize);
  juce::Image reset_4 = ImageCache::getFromMemory(
      BinaryData::buttonreset_osc_4_png, BinaryData::buttonreset_osc_4_pngSize);

  juce::DrawableImage reset_draw1;
  juce::DrawableImage reset_draw2;
  juce::DrawableImage reset_draw3;
  juce::DrawableImage reset_draw4;

  reset_draw1.setImage(reset_1);
  reset_draw2.setImage(reset_2);
  reset_draw3.setImage(reset_3);
  reset_draw4.setImage(reset_4);

  m_reset.setImages(&reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1,
                    &reset_draw4, &reset_draw4, &reset_draw3, &reset_draw3);
  m_reset.setClickingTogglesState(true);
  m_reset.setBounds(RESET_POS_X, RESET_POS_Y, reset_1.getWidth(),
                    reset_1.getHeight());
  m_reset.setTooltip(
      "Resets the oscillator to\nthe start of its waveform\nfor a new note");
  addChildComponent(m_reset);
  m_reset.setAlwaysOnTop(true);
  m_reset.setTriggeredOnMouseDown(true);
  m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());

  m_oct.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      N_KNOB_FRAMES);
  m_oct.setSliderStyle(Slider::RotaryVerticalDrag);
  m_oct.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  Rectangle<int> area_oct(BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);
  area_oct.setPosition(OCT_POS_X - BLACK_KNOB_SMALL_OFFSET_X,
                       PITCH_POS_Y - BLACK_KNOB_SMALL_OFFSET_Y);
  m_oct.setBounds(area_oct);
  m_oct.setRange(-OCT_RANGE_MAX, OCT_RANGE_MAX);
  m_oct.setNumDecimalPlacesToDisplay(0);
  m_oct.setKnobTooltip("The pitch of\nthe oscillator in octaves");

  addChildComponent(m_oct);

  m_semi.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      N_KNOB_FRAMES);
  m_semi.setSliderStyle(Slider::RotaryVerticalDrag);
  m_semi.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_semi.setRange(-SEMI_RANGE_MAX, SEMI_RANGE_MAX);
  m_semi.setNumDecimalPlacesToDisplay(0);
  m_semi.setKnobTooltip("The pitch of\nthe oscillator in semitones");
  addChildComponent(m_semi);

  m_fine.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      N_KNOB_FRAMES);
  m_fine.setSliderStyle(Slider::RotaryVerticalDrag);
  m_fine.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_fine.setKnobTooltip("The pitch of\nthe oscillator in cents");
  addChildComponent(m_fine);

  Rectangle<int> area_semi(BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);
  area_semi.setPosition(SEMI_POS_X - BLACK_KNOB_SMALL_OFFSET_X,
                        PITCH_POS_Y - BLACK_KNOB_SMALL_OFFSET_Y);
  m_semi.setBounds(area_semi);

  Rectangle<int> area_fine(BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);
  area_fine.setPosition(FINE_POS_X - BLACK_KNOB_SMALL_OFFSET_X,
                        PITCH_POS_Y - BLACK_KNOB_SMALL_OFFSET_Y);
  m_fine.setRange(-FINE_RANGE_MAX, FINE_RANGE_MAX);
  m_fine.setNumDecimalPlacesToDisplay(1);
  m_fine.setBounds(area_fine);

  juce::Image LED_1 = ImageCache::getFromMemory(BinaryData::LED_1_png,
                                                BinaryData::LED_1_pngSize);
  juce::Image LED_2 = ImageCache::getFromMemory(BinaryData::LED_2_png,
                                                BinaryData::LED_2_pngSize);
  juce::DrawableImage LED_draw1;
  juce::DrawableImage LED_draw2;
  LED_draw1.setImage(LED_1);
  LED_draw2.setImage(LED_2);

  m_LED_saw.setImages(&LED_draw2, &LED_draw2, &LED_draw2, &LED_draw2,
                      &LED_draw1, &LED_draw1, &LED_draw1);
  m_LED_saw.setClickingTogglesState(true);
  m_LED_saw.setRadioGroupId(23456);

  m_LED_saw.setBounds(LED_POS_X, LED_POS_Y - LED_1.getHeight(),
                      LED_1.getWidth(), LED_1.getHeight());
  //m_LED_saw.setToggleState(true, dontSendNotification);
  m_LED_saw.setTriggeredOnMouseDown(false);
  m_LED_saw.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                      juce::Colour());
  m_LED_saw.setTooltip("Select the saw wave");
  m_LED_saw.onStateChange = [&]() {
    if (m_LED_saw.getToggleState()) {
      m_value_tree.state.setProperty(m_analog_wave_identifier, 0, nullptr);
    }
  };
  addChildComponent(m_LED_saw);

  m_LED_pulse.setImages(&LED_draw2, &LED_draw2, &LED_draw2, &LED_draw2,
                        &LED_draw1, &LED_draw1, &LED_draw1);
  m_LED_pulse.setClickingTogglesState(true);
  m_LED_pulse.setRadioGroupId(23456);

  m_LED_pulse.setBounds(LED_POS_X, LED_POS_Y, LED_1.getWidth(),
                        LED_1.getHeight());
  // m_LED_pulse.setToggleState(true, dontSendNotification);
  m_LED_pulse.setTriggeredOnMouseDown(false);
  m_LED_pulse.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                        juce::Colour());
  m_LED_pulse.setTooltip("Select the pulse wave");
  m_LED_pulse.onStateChange = [&]() {
    if (m_LED_pulse.getToggleState()) {
      m_value_tree.state.setProperty(m_analog_wave_identifier, 1, nullptr);
    }
  };
  addChildComponent(m_LED_pulse);

  m_LED_triangle.setImages(&LED_draw2, &LED_draw2, &LED_draw2, &LED_draw2,
                           &LED_draw1, &LED_draw1, &LED_draw1);
  m_LED_triangle.setClickingTogglesState(true);
  m_LED_triangle.setRadioGroupId(23456);

  m_LED_triangle.setBounds(LED_POS_X, LED_POS_Y + LED_1.getHeight(),
                           LED_1.getWidth(), LED_1.getHeight());
  // m_LED_triangle.setToggleState(true, dontSendNotification);
  m_LED_triangle.setTriggeredOnMouseDown(false);
  m_LED_triangle.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
  m_LED_triangle.setTooltip("Select the triangle wave");
  // m_LED_triangle.setClickingTogglesState(true);
  m_LED_triangle.onStateChange = [&]() {
    if (m_LED_triangle.getToggleState()) {
      m_value_tree.state.setProperty(m_analog_wave_identifier, 2, nullptr);
    }
  };
  addChildComponent(m_LED_triangle);

  m_LED_sine.setImages(&LED_draw2, &LED_draw2, &LED_draw2, &LED_draw2,
                       &LED_draw1, &LED_draw1, &LED_draw1);
  m_LED_sine.setClickingTogglesState(true);
  m_LED_sine.setRadioGroupId(23456);

  m_LED_sine.setBounds(LED_POS_X, LED_POS_Y + 2 * LED_1.getHeight(),
                       LED_1.getWidth(), LED_1.getHeight());
  // m_LED_sine.setToggleState(true, dontSendNotification);
  m_LED_sine.setTriggeredOnMouseDown(false);
  m_LED_sine.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                       juce::Colour());
  m_LED_sine.setTooltip("Select the sine wave");
  m_LED_sine.onStateChange = [&]() {
    if (m_LED_sine.getToggleState()) {
      m_value_tree.state.setProperty(m_analog_wave_identifier, 3, nullptr);
    }
  };
  addChildComponent(m_LED_sine);

  m_pw.setStrip(ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                          BinaryData::black_knob_small_pngSize),
                N_KNOB_FRAMES);
  m_pw.setSliderStyle(Slider::RotaryVerticalDrag);
  m_pw.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  // m_pw.setValue(GETAUDIO("osc" + m_osc_number + "_pulsewidth"));
  m_pw.setDoubleClickReturnValue(true, PW_DEFAULT, ModifierKeys::ctrlModifier);
  m_pw.setKnobTooltip("The pulse width if\nthe pulse wave is selected");
  addChildComponent(m_pw);

  m_drift.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_big_png,
                                BinaryData::black_knob_big_pngSize),
      N_KNOB_FRAMES);
  m_drift.setSliderStyle(Slider::RotaryVerticalDrag);
  m_drift.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_drift.setKnobTooltip("Slightly detunes the oscillator\nover time, like an "
                         "analog\noscillator would");
  addChildComponent(m_drift);

  juce::Image arp_1 =
      ImageCache::getFromMemory(BinaryData::buttonarpeggiator_1_png,
                                BinaryData::buttonarpeggiator_1_pngSize);
  juce::Image arp_2 =
      ImageCache::getFromMemory(BinaryData::buttonarpeggiator_2_png,
                                BinaryData::buttonarpeggiator_2_pngSize);
  juce::Image arp_3 =
      ImageCache::getFromMemory(BinaryData::buttonarpeggiator_3_png,
                                BinaryData::buttonarpeggiator_3_pngSize);
  juce::Image arp_4 =
      ImageCache::getFromMemory(BinaryData::buttonarpeggiator_4_png,
                                BinaryData::buttonarpeggiator_4_pngSize);

  juce::DrawableImage arp_draw1;
  juce::DrawableImage arp_draw2;
  juce::DrawableImage arp_draw3;
  juce::DrawableImage arp_draw4;

  arp_draw1.setImage(arp_1);
  arp_draw2.setImage(arp_2);
  arp_draw3.setImage(arp_3);
  arp_draw4.setImage(arp_4);

  m_arp.setImages(&arp_draw2, &arp_draw2, &arp_draw1, &arp_draw1, &arp_draw4,
                  &arp_draw4, &arp_draw3, &arp_draw3);
  m_arp.setClickingTogglesState(true);
  m_arp.setBounds(ARP_POS_X, ARP_POS_Y, arp_1.getWidth(), arp_1.getHeight());
  addChildComponent(m_arp);
  m_arp.setTooltip("Enables the arpeggiator");
  m_arp.setAlwaysOnTop(true);
  m_arp.setTriggeredOnMouseDown(true);
  m_arp.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                  juce::Colour());

  juce::Image noise_1 = ImageCache::getFromMemory(
      BinaryData::buttonnoise_1_png, BinaryData::buttonnoise_1_pngSize);
  juce::Image noise_2 = ImageCache::getFromMemory(
      BinaryData::buttonnoise_2_png, BinaryData::buttonnoise_2_pngSize);
  juce::Image noise_3 = ImageCache::getFromMemory(
      BinaryData::buttonnoise_3_png, BinaryData::buttonnoise_3_pngSize);
  juce::Image noise_4 = ImageCache::getFromMemory(
      BinaryData::buttonnoise_4_png, BinaryData::buttonnoise_4_pngSize);

  juce::DrawableImage noise_draw1;
  juce::DrawableImage noise_draw2;
  juce::DrawableImage noise_draw3;
  juce::DrawableImage noise_draw4;

  noise_draw1.setImage(noise_1);
  noise_draw2.setImage(noise_2);
  noise_draw3.setImage(noise_3);
  noise_draw4.setImage(noise_4);

  m_noise.setImages(&noise_draw2, &noise_draw2, &noise_draw1, &noise_draw1,
                    &noise_draw4, &noise_draw4, &noise_draw3, &noise_draw3);
  m_noise.setClickingTogglesState(true);
  m_noise.setBounds(NOISE_POS_X, NOISE_POS_Y, noise_1.getWidth(),
                    noise_1.getHeight());
  m_noise.setTooltip("Switches the oscillator\nto 4Bit noise mode");
  addChildComponent(m_noise);
  m_noise.setAlwaysOnTop(true);
  m_noise.setTriggeredOnMouseDown(true);
  m_noise.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());

  juce::Image step_button_1 = ImageCache::getFromMemory(
      BinaryData::buttonstep_1_png, BinaryData::buttonstep_1_pngSize);
  juce::Image step_button_2 = ImageCache::getFromMemory(
      BinaryData::buttonstep_2_png, BinaryData::buttonstep_2_pngSize);
  juce::Image step_button_3 = ImageCache::getFromMemory(
      BinaryData::buttonstep_3_png, BinaryData::buttonstep_3_pngSize);
  juce::Image step_button_4 = ImageCache::getFromMemory(
      BinaryData::buttonstep_4_png, BinaryData::buttonstep_4_pngSize);

  juce::DrawableImage step_button_draw1;
  juce::DrawableImage step_button_draw2;
  juce::DrawableImage step_button_draw3;
  juce::DrawableImage step_button_draw4;

  step_button_draw1.setImage(step_button_1);
  step_button_draw2.setImage(step_button_2);
  step_button_draw3.setImage(step_button_3);
  step_button_draw4.setImage(step_button_4);

  m_step_button.setImages(&step_button_draw2, &step_button_draw2,
                          &step_button_draw1, &step_button_draw1,
                          &step_button_draw4, &step_button_draw4,
                          &step_button_draw3, &step_button_draw3);
  m_step_button.setClickingTogglesState(true);
  m_step_button.setBounds(STEP_BUTTON_POS_X, STEP_BUTTON_POS_Y,
                          step_button_1.getWidth(), step_button_1.getHeight());
  addChildComponent(m_step_button);
  m_step_button.setTooltip("Enables step 3 in the arpeggiator");
  m_step_button.setAlwaysOnTop(true);
  m_step_button.setTriggeredOnMouseDown(true);
  m_step_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                          juce::Colour());

  m_step_1.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      256);
  m_step_1.setBounds(STEP_1_POS_X, STEP_1_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                     BLACK_KNOB_SMALL_SIZE_Y);
  m_step_1.setSliderStyle(Slider::RotaryVerticalDrag);
  m_step_1.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_step_1.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
  m_step_1.setNumDecimalPlacesToDisplay(0);
  // m_step_1.setValue(GETAUDIO("osc" + m_osc_number + "_step_1"));
  m_step_1.setKnobTooltip(
      "The pitch of the\nfirst step of the\narpeggiator in semitones");
  addChildComponent(m_step_1);

  m_step_2.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      256);
  m_step_2.setBounds(STEP_2_POS_X, STEP_2_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                     BLACK_KNOB_SMALL_SIZE_Y);
  m_step_2.setSliderStyle(Slider::RotaryVerticalDrag);
  m_step_2.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_step_2.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
  // m_step_2.setValue(GETAUDIO("osc" + m_osc_number + "_step_2"));
  m_step_2.setNumDecimalPlacesToDisplay(0);
  m_step_2.setKnobTooltip(
      "The pitch of the\nsecond step of the\narpeggiator in semitones");
  m_step_2.setDoubleClickReturnValue(true, STEP_2_DEFAULT,
                                     ModifierKeys::ctrlModifier);
  addChildComponent(m_step_2);

  m_step_3.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      256);
  m_step_3.setBounds(STEP_3_POS_X, STEP_3_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                     BLACK_KNOB_SMALL_SIZE_Y);
  m_step_3.setSliderStyle(Slider::RotaryVerticalDrag);
  m_step_3.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_step_3.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
  // m_step_3.setValue(GETAUDIO("osc" + m_osc_number + "_step_3"));
  m_step_3.setNumDecimalPlacesToDisplay(0);
  m_step_3.setKnobTooltip(
      "The pitch of the\nthird step of the\narpeggiator in semitones");
  m_step_3.setDoubleClickReturnValue(true, STEP_2_DEFAULT,
                                     ModifierKeys::ctrlModifier);
  addChildComponent(m_step_3);

  m_fm.setStrip(ImageCache::getFromMemory(BinaryData::metal_knob_big_png,
                                          BinaryData::metal_knob_big_pngSize),
                256);
  m_fm.setBounds(FM_POS_X, FM_POS_Y, METAL_KNOB_BIG_SIZE_X,
                 METAL_KNOB_BIG_SIZE_Y);
  m_fm.setSliderStyle(Slider::RotaryVerticalDrag);
  m_fm.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_fm.setKnobTooltip(
      "How much the modulator modulates the pitch (FM) or the phase (PM) of the carrier wave");
  // m_fm.setValue(GETAUDIO("osc" + m_osc_number + "_fm"));
  addChildComponent(m_fm);

  m_lp.setStrip(ImageCache::getFromMemory(BinaryData::metal_knob_big_png,
                                          BinaryData::metal_knob_big_pngSize),
                256);
  m_lp.setBounds(LP_POS_X, LP_POS_Y, METAL_KNOB_BIG_SIZE_X,
                 METAL_KNOB_BIG_SIZE_Y);
  m_lp.setSliderStyle(Slider::RotaryVerticalDrag);
  m_lp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_lp.setRange(LP_MIN, LP_MAX);
  m_lp.setSkewFactorFromMidPoint(LP_MID);
  // m_lp.setValue(GETAUDIO("osc" + m_osc_number + "_lp"));
  m_lp.setDoubleClickReturnValue(true, LP_DEFAULT, ModifierKeys::ctrlModifier);
  m_lp.setTextValueSuffix(" Hz");
  m_lp.setNumDecimalPlacesToDisplay(0);
  m_lp.setKnobTooltip(
      "The frequency of\nthe lowpass filter which\nis applied to the noise");
  addChildComponent(m_lp);

  m_hp.setStrip(ImageCache::getFromMemory(BinaryData::metal_knob_big_png,
                                          BinaryData::metal_knob_big_pngSize),
                256);
  m_hp.setBounds(HP_POS_X, HP_POS_Y, METAL_KNOB_BIG_SIZE_X,
                 METAL_KNOB_BIG_SIZE_Y);
  m_hp.setSliderStyle(Slider::RotaryVerticalDrag);
  m_hp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_hp.setRange(HP_MIN, HP_MAX);
  m_hp.setSkewFactorFromMidPoint(HP_MID);
  m_hp.setTextValueSuffix(" Hz");
  // m_hp.setValue(GETAUDIO("osc" + m_osc_number + "_hp"));
  m_hp.setDoubleClickReturnValue(true, HP_DEFAULT, ModifierKeys::ctrlModifier);
  m_hp.setNumDecimalPlacesToDisplay(0);
  m_hp.setKnobTooltip(
      "The frequency of\nthe highspass filter which\nis applied to the noise");
  addChildComponent(m_hp);

  m_position.setStrip(
      ImageCache::getFromMemory(BinaryData::metal_knob_big_png,
                                BinaryData::metal_knob_big_pngSize),
      256);
  m_position.setBounds(POSITION_POS_X, POSITION_POS_Y, METAL_KNOB_BIG_SIZE_X,
                       METAL_KNOB_BIG_SIZE_Y);
  m_position.setSliderStyle(Slider::RotaryVerticalDrag);
  m_position.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_position.setKnobTooltip("The position in the\nwavetable used. There "
                            "are\nfour tables through which\nyou can sweep");
  addChildComponent(m_position);

  m_detune.setStrip(
      ImageCache::getFromMemory(BinaryData::metal_knob_big_png,
                                BinaryData::metal_knob_big_pngSize),
      256);
  m_detune.setBounds(DETUNE_POS_X, DETUNE_POS_Y, METAL_KNOB_BIG_SIZE_X,
                     METAL_KNOB_BIG_SIZE_Y);
  m_detune.setSliderStyle(Slider::RotaryVerticalDrag);
  m_detune.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_detune.setKnobTooltip(
      "How much the individual\noscillators are detuned\n against each other");
  m_detune.setSkewFactorFromMidPoint(0.3);
  addChildComponent(m_detune);

  m_spread.setStrip(
      ImageCache::getFromMemory(BinaryData::metal_knob_small_png,
                                BinaryData::metal_knob_small_pngSize),
      256);
  m_spread.setBounds(SPREAD_POS_X, SPREAD_POS_Y, METAL_KNOB_SMALL_SIZE_X,
                     METAL_KNOB_SMALL_SIZE_Y);
  m_spread.setSliderStyle(Slider::RotaryVerticalDrag);
  m_spread.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_spread.setKnobTooltip(
      "Spreads the oscillators to\n different positions in the\n wavetable");
  addChildComponent(m_spread);

  m_position_multi.setStrip(
      ImageCache::getFromMemory(BinaryData::metal_knob_small_png,
                                BinaryData::metal_knob_small_pngSize),
      256);
  m_position_multi.setBounds(POSITION_MULTI_POS_X, POSITION_MULTI_POS_Y,
                             METAL_KNOB_SMALL_SIZE_X, METAL_KNOB_SMALL_SIZE_Y);
  m_position_multi.setSliderStyle(Slider::RotaryVerticalDrag);
  m_position_multi.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_position_multi.setKnobTooltip(
      "The position in the\nwavetable used. There are\nfour tables through "
      "which\nyou can sweep");
  addChildComponent(m_position_multi);

  m_speed.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_small_png,
                                BinaryData::black_knob_small_pngSize),
      256);
  m_speed.setBounds(SPEED_POS_X, SPEED_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                    BLACK_KNOB_SMALL_SIZE_Y);
  m_speed.setSliderStyle(Slider::RotaryVerticalDrag);
  m_speed.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_speed.setTextValueSuffix(" Hz");
  m_speed.setRange(SPEED_MIN, SPEED_MAX);
  m_speed.setSkewFactorFromMidPoint(SPEED_MID);
  // m_speed./(GETAUDIO("osc" + m_osc_number + "_arp_speed"));
  m_speed.setDoubleClickReturnValue(true, SPEED_DEFAULT,
                                    ModifierKeys::ctrlModifier);
  m_speed.setNumDecimalPlacesToDisplay(1);
  m_speed.setKnobTooltip("Speed of the arpeggiator");
  addChildComponent(m_speed);

  juce::Image chipdraw_convert_1 = ImageCache::getFromMemory(
      BinaryData::buttonapply_1_png, BinaryData::buttonapply_1_pngSize);
  juce::Image chipdraw_convert_2 = ImageCache::getFromMemory(
      BinaryData::buttonapply_2_png, BinaryData::buttonapply_2_pngSize);
  juce::Image chipdraw_convert_3 = ImageCache::getFromMemory(
      BinaryData::buttonapply_3_png, BinaryData::buttonapply_3_pngSize);
  juce::Image chipdraw_convert_4 = ImageCache::getFromMemory(
      BinaryData::buttonapply_4_png, BinaryData::buttonapply_4_pngSize);

  juce::DrawableImage chipdraw_convert_draw1;
  juce::DrawableImage chipdraw_convert_draw2;
  juce::DrawableImage chipdraw_convert_draw3;
  juce::DrawableImage chipdraw_convert_draw4;

  chipdraw_convert_draw1.setImage(chipdraw_convert_1);
  chipdraw_convert_draw2.setImage(chipdraw_convert_2);
  chipdraw_convert_draw3.setImage(chipdraw_convert_3);
  chipdraw_convert_draw4.setImage(chipdraw_convert_4);

  m_chipdraw_convert.setImages(
      &chipdraw_convert_draw2, &chipdraw_convert_draw2, &chipdraw_convert_draw1,
      &chipdraw_convert_draw1, &chipdraw_convert_draw4, &chipdraw_convert_draw4,
      &chipdraw_convert_draw3, &chipdraw_convert_draw3);
  m_chipdraw_convert.setClickingTogglesState(true);
  m_chipdraw_convert.setBounds(CONVERT_POS_X, CONVERT_POS_Y,
                               chipdraw_convert_1.getWidth(),
                               chipdraw_convert_1.getHeight());
  m_chipdraw_convert.setAlwaysOnTop(true);
  m_chipdraw_convert.setTriggeredOnMouseDown(true);
  m_chipdraw_convert.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
  m_chipdraw_convert.setToggleState(true, sendNotification);
  m_chipdraw_convert.onClick = [&]() {
    m_chipdraw_convert.setToggleState(true, sendNotification);
    createChipdrawTables();
  };
  m_chipdraw_convert.setTooltip(
      "Converts the waveform drawn\nin the window. You won't hear\nany changes "
      "before you press\nthis button");
  addChildComponent(m_chipdraw_convert);

  m_wavedraw_convert.setImages(
      &chipdraw_convert_draw2, &chipdraw_convert_draw2, &chipdraw_convert_draw1,
      &chipdraw_convert_draw1, &chipdraw_convert_draw4, &chipdraw_convert_draw4,
      &chipdraw_convert_draw3, &chipdraw_convert_draw3);
  m_wavedraw_convert.setClickingTogglesState(true);
  m_wavedraw_convert.setBounds(CONVERT_POS_X, CONVERT_POS_Y,
                               chipdraw_convert_1.getWidth(),
                               chipdraw_convert_1.getHeight());
  m_wavedraw_convert.setAlwaysOnTop(true);
  m_wavedraw_convert.setTriggeredOnMouseDown(true);
  m_wavedraw_convert.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
  m_wavedraw_convert.setToggleState(true, sendNotification);
  m_wavedraw_convert.onClick = [&]() {
    m_wavedraw_convert.setToggleState(true, sendNotification);
    createWavedrawTables();
  };
  m_wavedraw_convert.setTooltip(
      "Converts the waveform drawn\nin the window. You won't hear\nany changes "
      "before you press\nthis button");
  addChildComponent(m_wavedraw_convert);

  m_wavedraw_convert_REMOVE.setImages(
      &chipdraw_convert_draw2, &chipdraw_convert_draw2, &chipdraw_convert_draw1,
      &chipdraw_convert_draw1, &chipdraw_convert_draw4, &chipdraw_convert_draw4,
      &chipdraw_convert_draw3, &chipdraw_convert_draw3);
  m_wavedraw_convert_REMOVE.setClickingTogglesState(true);
  m_wavedraw_convert_REMOVE.setBounds(
      CONVERT_POS_X, CONVERT_POS_Y - chipdraw_convert_1.getHeight() * 1.1,
      chipdraw_convert_1.getWidth(), chipdraw_convert_1.getHeight());
  m_wavedraw_convert_REMOVE.setAlwaysOnTop(true);
  m_wavedraw_convert_REMOVE.setTriggeredOnMouseDown(true);
  m_wavedraw_convert_REMOVE.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
  m_wavedraw_convert_REMOVE.onClick = [&]() {
    m_wavedraw_convert_REMOVE.setToggleState(true, sendNotification);
    writeWavedrawTableToFile();
  };
  m_wavedraw_convert_REMOVE.setTooltip("R E M O V E");
  addChildComponent(m_wavedraw_convert_REMOVE);

  m_chipdraw_convert_REMOVE.setImages(
      &chipdraw_convert_draw2, &chipdraw_convert_draw2, &chipdraw_convert_draw1,
      &chipdraw_convert_draw1, &chipdraw_convert_draw4, &chipdraw_convert_draw4,
      &chipdraw_convert_draw3, &chipdraw_convert_draw3);
  m_chipdraw_convert_REMOVE.setClickingTogglesState(true);
  m_chipdraw_convert_REMOVE.setBounds(
      CONVERT_POS_X, CONVERT_POS_Y - chipdraw_convert_1.getHeight() * 1.1,
      chipdraw_convert_1.getWidth(), chipdraw_convert_1.getHeight());
  m_chipdraw_convert_REMOVE.setAlwaysOnTop(true);
  m_chipdraw_convert_REMOVE.setTriggeredOnMouseDown(true);
  m_chipdraw_convert_REMOVE.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
  m_chipdraw_convert_REMOVE.onClick = [&]() {
    m_chipdraw_convert_REMOVE.setToggleState(true, sendNotification);
    writeChipdrawTableToFile();
  };
  m_chipdraw_convert_REMOVE.setTooltip("R E M O V E");
  addChildComponent(m_chipdraw_convert_REMOVE);

  TIMEADD("chipdraw");

  m_specdraw_convert_REMOVE.setImages(
      &chipdraw_convert_draw2, &chipdraw_convert_draw2, &chipdraw_convert_draw1,
      &chipdraw_convert_draw1, &chipdraw_convert_draw4, &chipdraw_convert_draw4,
      &chipdraw_convert_draw3, &chipdraw_convert_draw3);
  m_specdraw_convert_REMOVE.setClickingTogglesState(true);
  m_specdraw_convert_REMOVE.setBounds(
      CONVERT_POS_X, CONVERT_POS_Y - chipdraw_convert_1.getHeight() * 1.1,
      chipdraw_convert_1.getWidth(), chipdraw_convert_1.getHeight());
  m_specdraw_convert_REMOVE.setAlwaysOnTop(true);
  m_specdraw_convert_REMOVE.setTriggeredOnMouseDown(true);
  m_specdraw_convert_REMOVE.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
  m_specdraw_convert_REMOVE.onClick = [&]() {
    m_specdraw_convert_REMOVE.setToggleState(true, sendNotification);
    writeSpecdrawTableToFile();
  };
  m_specdraw_convert_REMOVE.setTooltip("R E M O V E");
  addChildComponent(m_specdraw_convert_REMOVE);

  m_specdraw_convert.setImages(
      &chipdraw_convert_draw2, &chipdraw_convert_draw2, &chipdraw_convert_draw1,
      &chipdraw_convert_draw1, &chipdraw_convert_draw4, &chipdraw_convert_draw4,
      &chipdraw_convert_draw3, &chipdraw_convert_draw3);
  m_specdraw_convert.setClickingTogglesState(true);
  m_specdraw_convert.setBounds(CONVERT_POS_X, CONVERT_POS_Y,
                               chipdraw_convert_1.getWidth(),
                               chipdraw_convert_1.getHeight());
  m_specdraw_convert.setAlwaysOnTop(true);
  m_specdraw_convert.setTriggeredOnMouseDown(true);
  m_specdraw_convert.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
  m_specdraw_convert.setToggleState(true, sendNotification);
  m_specdraw_convert.onClick = [&]() {
    m_specdraw_convert.setToggleState(true, sendNotification);
    createSpecdrawTables();
  };
  m_specdraw_convert.setTooltip(
      "Converts the waveform drawn\nin the window. You won't hear\nany changes "
      "before you press\nthis button");
  addChildComponent(m_specdraw_convert);

  juce::Colour chip_color(102, 93, 79);

  m_chiptune_waveselector.OnValueChange = [&](int p_new_value) {
    m_value_tree.state.setProperty(m_chipwave_identifier, (float)p_new_value,
                                   nullptr);
  };
  m_chiptune_waveselector.setTopLeftPosition(WAVE_CHIPTUNE_POS_X,
                                             WAVE_CHIPTUNE_POS_Y);
  m_chiptune_waveselector.addWave(1, "Pulse 50");
  m_chiptune_waveselector.setDecrementValue(1, 1);
  m_chiptune_waveselector.addWave(2, "Pulse 25");
  m_chiptune_waveselector.addWave(3, "Pulse 12.5");
  m_chiptune_waveselector.addWave(4, "ChipTriangle");
  m_chiptune_waveselector.addWave(5, "custom1");
  m_chiptune_waveselector.addWave(6, "custom2");
  m_chiptune_waveselector.addWave(7, "custom3");
  m_chiptune_waveselector.addWave(8, "custom4");
  m_chiptune_waveselector.addWave(9, "custom5");
  m_chiptune_waveselector.addSeparator();
  // normal wavetable submenu
  m_chiptune_waveselector.setIncrementValue(9, 100);
  m_chiptune_waveselector.addWaveToSubmenu(100, "henlo", 0);
  m_chiptune_waveselector.setDecrementValue(100, 9);
  m_chiptune_waveselector.addWaveToSubmenu(101, "henlo2", 0);
  m_chiptune_waveselector.setIncrementValue(101, 201);
  m_chiptune_waveselector.applySubmenu(0, "Wavetables");
  m_chiptune_waveselector.addSeparator();
  // chipdraw menu
  m_chiptune_waveselector.addWaveToSubmenu(201, "ChipDraw Osc1", 1);
  m_chiptune_waveselector.setDecrementValue(201, 101);
  m_chiptune_waveselector.addWaveToSubmenu(202, "ChipDraw Osc2", 1);
  m_chiptune_waveselector.addWaveToSubmenu(203, "ChipDraw Osc3", 1);
  m_chiptune_waveselector.setIncrementValue(203, 301);
  m_chiptune_waveselector.applySubmenu(1, "ChipDraw");
  // wavedraw menu
  m_chiptune_waveselector.addWaveToSubmenu(301, "WaveDraw Osc 1", 2);
  m_chiptune_waveselector.setDecrementValue(301, 203);
  m_chiptune_waveselector.addWaveToSubmenu(302, "WaveDraw Osc 2", 2);
  m_chiptune_waveselector.addWaveToSubmenu(303, "WaveDraw Osc 3", 2);
  m_chiptune_waveselector.setIncrementValue(303, 401);
  m_chiptune_waveselector.applySubmenu(2, "WaveDraw");
  // specdraw menu
  m_chiptune_waveselector.addWaveToSubmenu(401, "SpecDraw Osc 1", 3);
  m_chiptune_waveselector.setDecrementValue(401, 303);
  m_chiptune_waveselector.addWaveToSubmenu(402, "SpecDraw Osc 2", 3);
  m_chiptune_waveselector.addWaveToSubmenu(403, "SpecDraw Osc 3", 3);
  m_chiptune_waveselector.setIncrementValue(403, 403);
  m_chiptune_waveselector.applySubmenu(3, "WaveDraw");

  // m_chiptune_waveselector.setValue(GETAUDIO("osc" + m_osc_number +
  // "_chiptune_wave"));
  m_chiptune_waveselector.setColor(chip_color);
  m_chiptune_waveselector.setTooltip("Selects the wave for the oscillator");
  addChildComponent(m_chiptune_waveselector);

  //juce::Colour fm_color(90, 40, 40);

  juce::PopupMenu fm_menu;
  juce::PopupMenu vector_menu;
  juce::PopupMenu wavetable_submenu;
  juce::PopupMenu wavetable_recycle_menu;

#define ADD_WAVETABLE_SUB_MENU(name, number)                                   \
  wavetable_recycle_menu.clear();                                              \
  wavetable_recycle_menu.addItem(number + 1,                                   \
                                 (std::string)name + (std::string) " 1");      \
  wavetable_recycle_menu.addItem(number + 2,                                   \
                                 (std::string)name + (std::string) " 2");      \
  wavetable_recycle_menu.addItem(number + 3,                                   \
                                 (std::string)name + (std::string) " 3");      \
  wavetable_recycle_menu.addItem(number + 4,                                   \
                                 (std::string)name + (std::string) " 4");      \
  wavetable_submenu.addSubMenu(name, wavetable_recycle_menu);

#define ADD_MAP_ENTRY(name, number, selector)                                  \
  selector.addMapEntry((std::string)name + (std::string) " 1", number + 1,     \
                       number + 2, number - 6);                                \
  selector.addMapEntry((std::string)name + (std::string) " 2", number + 2,     \
                       number + 3, number + 1);                                \
  selector.addMapEntry((std::string)name + (std::string) " 3", number + 3,     \
                       number + 4, number + 2);                                \
  selector.addMapEntry((std::string)name + (std::string) " 4", number + 4,     \
                       number + 11, number + 3);

  juce::PopupMenu chiptune_submenu;
  chiptune_submenu.addItem(401, "Square 50");
  chiptune_submenu.addItem(402, "Square 25");
  chiptune_submenu.addItem(403, "Square 12.5");
  chiptune_submenu.addItem(404, "ChipTriangle");
  chiptune_submenu.addItem(405, "TODO");
  juce::PopupMenu wavedraw_submenu;
  wavedraw_submenu.addItem(501, "WaveDraw Osc1");
  wavedraw_submenu.addItem(502, "WaveDraw Osc2");
  wavedraw_submenu.addItem(503, "WaveDraw Osc3");
  juce::PopupMenu chipdraw_submenu;
  chipdraw_submenu.addItem(601, "ChipDraw Osc1");
  chipdraw_submenu.addItem(602, "ChipDraw Osc2");
  chipdraw_submenu.addItem(603, "ChipDraw Osc3");
  juce::PopupMenu specdraw_submenu;
  specdraw_submenu.addItem(701, "SpecDraw Osc1");
  specdraw_submenu.addItem(702, "SpecDraw Osc2");
  specdraw_submenu.addItem(703, "SpecDraw Osc3");

  // now add all menus
  wavetable_recycle_menu.addItem(101, "Saw");
  wavetable_recycle_menu.addItem(102, "Square");
  wavetable_recycle_menu.addItem(103, "Triangle");
  wavetable_recycle_menu.addItem(104, "Sine");

  wavetable_submenu.addSubMenu("Classic Analog", wavetable_recycle_menu);
  ADD_WAVETABLE_SUB_MENU("Additive A", 110)
  ADD_WAVETABLE_SUB_MENU("Additive B", 120)
  ADD_WAVETABLE_SUB_MENU("Additive C", 130)
  ADD_WAVETABLE_SUB_MENU("Additive D", 140)
  ADD_WAVETABLE_SUB_MENU("Harmonics A", 150)
  ADD_WAVETABLE_SUB_MENU("Harmonics B", 160)
  ADD_WAVETABLE_SUB_MENU("Harmonics C", 170)
  ADD_WAVETABLE_SUB_MENU("Harmonics D", 180)
  ADD_WAVETABLE_SUB_MENU("Organ", 190)
  ADD_WAVETABLE_SUB_MENU("BrokenSine", 200)
  ADD_WAVETABLE_SUB_MENU("Skyline", 210)
  ADD_WAVETABLE_SUB_MENU("Soft", 220)
  ADD_WAVETABLE_SUB_MENU("MultiSaw", 230)
  ADD_WAVETABLE_SUB_MENU("Rectangular", 240)
  ADD_WAVETABLE_SUB_MENU("Strings", 250)
  ADD_WAVETABLE_SUB_MENU("Piano", 260)
  ADD_WAVETABLE_SUB_MENU("Oboe", 270)
  ADD_WAVETABLE_SUB_MENU("Flute", 280)
  ADD_WAVETABLE_SUB_MENU("Trumpet", 290)
  ADD_WAVETABLE_SUB_MENU("MusicBox", 300)
  ADD_WAVETABLE_SUB_MENU("Wave22", 310)
  ADD_WAVETABLE_SUB_MENU("Wave23", 320)
  ADD_WAVETABLE_SUB_MENU("Last", 996)

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

  //====================
  //=== COPY & PASTA ===
  //====================

  ADD_MAP_ENTRY("Additive A", 110, m_carrier_waveselector)
  ADD_MAP_ENTRY("Additive B", 120, m_carrier_waveselector)
  ADD_MAP_ENTRY("Additive C", 130, m_carrier_waveselector)
  ADD_MAP_ENTRY("Additive D", 140, m_carrier_waveselector)
  ADD_MAP_ENTRY("Harmonics A", 150, m_carrier_waveselector)
  ADD_MAP_ENTRY("Harmonics B", 160, m_carrier_waveselector)
  ADD_MAP_ENTRY("Harmonics C", 170, m_carrier_waveselector)
  ADD_MAP_ENTRY("Harmonics D", 180, m_carrier_waveselector)
  ADD_MAP_ENTRY("Organ", 190, m_carrier_waveselector)
  ADD_MAP_ENTRY("BrokenSine", 200, m_carrier_waveselector)
  ADD_MAP_ENTRY("Skyline", 210, m_carrier_waveselector)
  ADD_MAP_ENTRY("Soft", 220, m_carrier_waveselector)
  ADD_MAP_ENTRY("MultiSaw", 230, m_carrier_waveselector)
  ADD_MAP_ENTRY("Wave15", 240, m_carrier_waveselector)
  ADD_MAP_ENTRY("Strings", 250, m_carrier_waveselector)
  ADD_MAP_ENTRY("Piano", 260, m_carrier_waveselector)
  ADD_MAP_ENTRY("Oboe", 270, m_carrier_waveselector)
  ADD_MAP_ENTRY("Flute", 280, m_carrier_waveselector)
  ADD_MAP_ENTRY("Trumpet", 290, m_carrier_waveselector)
  ADD_MAP_ENTRY("MusicBox", 300, m_carrier_waveselector)
  ADD_MAP_ENTRY("Wave22", 310, m_carrier_waveselector)
  ADD_MAP_ENTRY("Wave23", 320, m_carrier_waveselector)
  ADD_MAP_ENTRY("Last", 996, m_carrier_waveselector) // last entry 996-1000

  m_carrier_waveselector.setDecrementValue(101, 1);
  m_carrier_waveselector.setIncrementValue(334, 401);
  m_carrier_waveselector.addMapEntry("Sine", 1, 101, 1);
  m_carrier_waveselector.addMapEntry("Saw", 101, 102, 1);
  m_carrier_waveselector.addMapEntry("Square", 102, 103, 101);
  m_carrier_waveselector.addMapEntry("Triangle", 103, 104, 102);
  m_carrier_waveselector.addMapEntry("Sine", 104, 111, 103);

  m_carrier_waveselector.addMapEntry("Square 50", 401, 402, 334);
  m_carrier_waveselector.addMapEntry("Square 25", 402, 403, 401);
  m_carrier_waveselector.addMapEntry("Square 12.5", 403, 404, 402);
  m_carrier_waveselector.addMapEntry("ChipTriangle", 404, 405, 403);
  m_carrier_waveselector.addMapEntry("TODO", 405, 501, 404);

  m_carrier_waveselector.addMapEntry("WaveDraw Osc1", 501, 502, 405);
  m_carrier_waveselector.addMapEntry("WaveDraw Osc2", 502, 503, 501);
  m_carrier_waveselector.addMapEntry("WaveDraw Osc3", 503, 601, 502);

  m_carrier_waveselector.addMapEntry("ChipDraw Osc1", 601, 602, 503);
  m_carrier_waveselector.addMapEntry("ChipDraw Osc2", 602, 603, 601);
  m_carrier_waveselector.addMapEntry("ChipDraw Osc3", 603, 701, 602);

  m_carrier_waveselector.addMapEntry("SpecDraw Osc1", 701, 702, 603);
  m_carrier_waveselector.addMapEntry("SpecDraw Osc2", 702, 703, 701);
  m_carrier_waveselector.addMapEntry("SpecDraw Osc3", 703, 703, 702);

  m_carrier_waveselector.m_menu = fm_menu;

  ADD_MAP_ENTRY("Additive A", 110, m_modulator_waveselector)
  ADD_MAP_ENTRY("Additive B", 120, m_modulator_waveselector)
  ADD_MAP_ENTRY("Additive C", 130, m_modulator_waveselector)
  ADD_MAP_ENTRY("Additive D", 140, m_modulator_waveselector)
  ADD_MAP_ENTRY("Harmonics A", 150, m_modulator_waveselector)
  ADD_MAP_ENTRY("Harmonics B", 160, m_modulator_waveselector)
  ADD_MAP_ENTRY("Harmonics C", 170, m_modulator_waveselector)
  ADD_MAP_ENTRY("Harmonics D", 180, m_modulator_waveselector)
  ADD_MAP_ENTRY("Organ", 190, m_modulator_waveselector)
  ADD_MAP_ENTRY("BrokenSine", 200, m_modulator_waveselector)
  ADD_MAP_ENTRY("Skyline", 210, m_modulator_waveselector)
  ADD_MAP_ENTRY("Soft", 220, m_modulator_waveselector)
  ADD_MAP_ENTRY("MultiSaw", 230, m_modulator_waveselector)
  ADD_MAP_ENTRY("Wave15", 240, m_modulator_waveselector)
  ADD_MAP_ENTRY("Strings", 250, m_modulator_waveselector)
  ADD_MAP_ENTRY("Piano", 260, m_modulator_waveselector)
  ADD_MAP_ENTRY("Oboe", 270, m_modulator_waveselector)
  ADD_MAP_ENTRY("Flute", 280, m_modulator_waveselector)
  ADD_MAP_ENTRY("Trumpet", 290, m_modulator_waveselector)
  ADD_MAP_ENTRY("MusicBox", 300, m_modulator_waveselector)
  ADD_MAP_ENTRY("Wave22", 310, m_modulator_waveselector)
  ADD_MAP_ENTRY("Wave23", 320, m_modulator_waveselector)
  ADD_MAP_ENTRY("Last", 330, m_modulator_waveselector)
  ADD_MAP_ENTRY("Last", 996, m_carrier_waveselector) // last entry 996-1000

  m_modulator_waveselector.setDecrementValue(101, 1);
  m_modulator_waveselector.setIncrementValue(334, 401);
  m_modulator_waveselector.addMapEntry("Sine", 1, 101, 1);
  m_modulator_waveselector.addMapEntry("Saw", 101, 102, 1);
  m_modulator_waveselector.addMapEntry("Square", 102, 103, 101);
  m_modulator_waveselector.addMapEntry("Triangle", 103, 104, 102);
  m_modulator_waveselector.addMapEntry("Sine", 104, 111, 103);

  m_modulator_waveselector.addMapEntry("Square 50", 401, 402, 334);
  m_modulator_waveselector.addMapEntry("Square 25", 402, 403, 401);
  m_modulator_waveselector.addMapEntry("Square 12.5", 403, 404, 402);
  m_modulator_waveselector.addMapEntry("ChipTriangle", 404, 405, 403);
  m_modulator_waveselector.addMapEntry("TODO", 405, 501, 404);

  m_modulator_waveselector.addMapEntry("WaveDraw Osc1", 501, 502, 405);
  m_modulator_waveselector.addMapEntry("WaveDraw Osc2", 502, 503, 501);
  m_modulator_waveselector.addMapEntry("WaveDraw Osc3", 503, 601, 502);

  m_modulator_waveselector.addMapEntry("ChipDraw Osc1", 601, 602, 503);
  m_modulator_waveselector.addMapEntry("ChipDraw Osc2", 602, 603, 601);
  m_modulator_waveselector.addMapEntry("ChipDraw Osc3", 603, 701, 602);

  m_modulator_waveselector.addMapEntry("SpecDraw Osc1", 701, 702, 603);
  m_modulator_waveselector.addMapEntry("SpecDraw Osc2", 702, 703, 701);
  m_modulator_waveselector.addMapEntry("SpecDraw Osc3", 703, 703, 702);

  m_modulator_waveselector.m_menu = fm_menu;

  *(m_vec_a.getRootMenu()) = vector_menu;
  *(m_vec_b.getRootMenu()) = vector_menu;
  *(m_vec_c.getRootMenu()) = vector_menu;
  *(m_vec_d.getRootMenu()) = vector_menu;

  m_carrier_waveselector.OnValueChange = [&](int p_new_value) {
    m_value_tree.state.setProperty(m_carrier_wave_identifier,
                                   (float)p_new_value, nullptr);
  };
  m_carrier_waveselector.setTopLeftPosition(WAVE_CARRIER_POS_X,
                                            WAVE_CARRIER_POS_Y);

  // m_carrier_waveselector.setValue(1);
  m_carrier_waveselector.setColor(FM_COLOR);
  m_carrier_waveselector.setTooltip("Selects the wave for the carrier osc");
  addChildComponent(m_carrier_waveselector);

  m_wavetable_waveselector.OnValueChange = [&](int p_new_value) {
    m_value_tree.state.setProperty(m_wavetable_identifier, (float)p_new_value,
                                   nullptr);
  };
  m_wavetable_waveselector.setTopLeftPosition(WAVE_CARRIER_POS_X,
                                              WAVE_CARRIER_POS_Y);
  m_wavetable_waveselector.addWave(1, "Classic Analog");
  m_wavetable_waveselector.addWave(2, "Additive A");
  m_wavetable_waveselector.addWave(3, "Additive B");
  m_wavetable_waveselector.addWave(4, "Additive C");
  m_wavetable_waveselector.addWave(5, "Additive D");
  m_wavetable_waveselector.addWave(6, "Harmonics1");
  m_wavetable_waveselector.addWave(7, "Harmonics2");
  m_wavetable_waveselector.addWave(8, "Harmonics3");
  m_wavetable_waveselector.addWave(9, "Harmonics4");
  m_wavetable_waveselector.addWave(10, "Organ");
  m_wavetable_waveselector.addWave(11, "BrokenSine");
  m_wavetable_waveselector.addWave(12, "Skyline");
  m_wavetable_waveselector.addWave(13, "Soft");
  m_wavetable_waveselector.addWave(14, "MultiSaw");
  m_wavetable_waveselector.addWave(15, "Recangular");
  m_wavetable_waveselector.addWave(16, "Strings");
  m_wavetable_waveselector.addWave(17, "Piano");
  m_wavetable_waveselector.addWave(18, "Oboe");
  m_wavetable_waveselector.addWave(19, "Flute");
  m_wavetable_waveselector.addWave(20, "Trumpet");
  m_wavetable_waveselector.addWave(21, "MusicBox");
  m_wavetable_waveselector.addWave(22, "Wave22");
  m_wavetable_waveselector.addWave(23, "Wave23");
  // m_wavetable_waveselector.setValue(1);
  m_wavetable_waveselector.setTooltip("Selects the wave for the oscillator");
  addChildComponent(m_wavetable_waveselector);

  m_modulator_waveselector.OnValueChange = [&](int p_new_value) {
    m_value_tree.state.setProperty(m_modulator_wave_identifier,
                                   (float)p_new_value, nullptr);
  };
  m_modulator_waveselector.setTopLeftPosition(WAVE_MODULATOR_POS_X,
                                              WAVE_MODULATOR_POS_Y);
  m_modulator_waveselector.setColor(FM_COLOR);
  // m_modulator_waveselector.setValue(1);
  m_modulator_waveselector.setTooltip("Selects the wave for the modulator osc");
  addChildComponent(m_modulator_waveselector);

  m_carrier_ratio.OnValueChange = [&](int p_new_value) {
    //m_value_tree.state.setProperty(m_carrier_ratio_identifier,
    //                               (float)p_new_value, nullptr);
    DBG("setratio");
    DBG(p_new_value);
    SETAUDIO(m_carrier_ratio_identifier, (p_new_value - 1) / 11.f);
    // m_value_tree.getParameter(m_vowel_left_identifier)
    //     ->setValueNotifyingHost(((float)p_new_value) / 7.f);
  };
  m_carrier_ratio.setTopLeftPosition(RATIO_CARRIER_POS_X, RATIO_CARRIER_POS_Y);
  m_carrier_ratio.setRange(1, 12);
  // m_carrier_ratio.setValue(1);
  m_carrier_ratio.setColor(FM_COLOR);
  m_carrier_ratio.setTooltip(
      "The pitch ratio of the carrier to base frequency");
  m_carrier_ratio.setMouseDragDivisor(20.f);

  addChildComponent(m_carrier_ratio);

  m_modulator_ratio.OnValueChange = [&](int p_new_value) {
    //m_value_tree.state.setProperty(m_modulator_ratio_identifier,
    //                               (float)p_new_value, nullptr);
    SETAUDIO(m_modulator_ratio_identifier, (p_new_value - 1)/11.f);
  };
  m_modulator_ratio.setTopLeftPosition(RATIO_MODULATOR_POS_X,
                                       RATIO_MODULATOR_POS_Y);
  m_modulator_ratio.setRange(1, 12);
  // m_modulator_ratio.setValue(1);
  m_modulator_ratio.setColor(FM_COLOR);
  m_modulator_ratio.setTooltip(
      "The pitch ratio of the modulator to base frequency");
  m_modulator_ratio.setMouseDragDivisor(20.f);
  addChildComponent(m_modulator_ratio);

  juce::Image fm_exp_left = ImageCache::getFromMemory(
      BinaryData::buttonexplin_1_png, BinaryData::buttonexplin_1_pngSize);
  juce::Image fm_exp_right = ImageCache::getFromMemory(
      BinaryData::buttonexplin_3_png, BinaryData::buttonexplin_3_pngSize);
  m_fm_exp.setImage(fm_exp_left, 1);
  m_fm_exp.setImage(fm_exp_right, 2);
  m_fm_exp.setBounds(FM_EXP_POS_X, FM_EXP_POS_Y, fm_exp_left.getWidth(),
                     fm_exp_left.getHeight());
  //m_fm_exp.setToggleState(false, dontSendNotification);
  m_fm_exp.onStateChange = [&]() {
    // setLfo12(m_fm_exp_button.getToggleState());
  };
  m_fm_exp.setTooltip(
      "Whether to use exponential or linear FM.\nExponential FM has a more "
      "musical character to it, as it preserves the perceived note pitch.");
  addChildComponent(m_fm_exp);

  m_chipdraw.setTopLeftPosition(CHIPDRAW_POSITION_X, CHIPDRAW_POSITION_Y);
  m_chipdraw.setColor(chip_color);
  m_chipdraw.setDrawColor(juce::Colours::white);
  m_chipdraw.onDraw = [&]() {
    m_chipdraw_convert.setToggleState(false, dontSendNotification);
  };
  m_chipdraw.setTooltip("Draw a custom 4Bit waveform.\n\nDon't forget to apply "
                        "your waveform with the button on the bottom right.");
  addChildComponent(m_chipdraw);

  juce::Colour wave_color(71, 92, 108);
  juce::Colour wave_draw_color(99, 165, 236);

  m_wavedraw.setTopLeftPosition(CHIPDRAW_POSITION_X, CHIPDRAW_POSITION_Y);
  m_wavedraw.setColor(wave_color);
  m_wavedraw.setDrawColor(wave_draw_color);
  m_wavedraw.onDraw = [&]() {
    m_wavedraw_convert.setToggleState(false, dontSendNotification);
  };
  m_wavedraw.setTooltip(
      "Become the Picasso of music production.\n\nDon't forget to apply your "
      "waveform with the button on the bottom right.");
  addChildComponent(m_wavedraw);

  juce::Colour spec_color(61, 80, 70);
  juce::Colour spec_draw_color(116, 147, 147);

  m_specdraw.setTopLeftPosition(CHIPDRAW_POSITION_X, CHIPDRAW_POSITION_Y);
  m_specdraw.setColor(spec_color);
  m_specdraw.setDrawColor(spec_draw_color);
  m_specdraw.onDraw = [&]() {
    m_specdraw_convert.setToggleState(false, dontSendNotification);
  };
  m_specdraw.setTooltip(
      "Draw the spectrum of the oscillator. A single peak corresponds to a "
      "sine function.\n\nDon't forget to apply your waveform with the button "
      "on the bottom right.");
  addChildComponent(m_specdraw);

  juce::Colour vector_color(60, 60, 60);

  juce::Image glas_panel = ImageCache::getFromMemory(
      BinaryData::vectorpanel_png, BinaryData::vectorpanel_pngSize);
  m_xy.setTopLeftPosition(XY_POS_X, XY_POS_Y);
  m_xy.setSize(glas_panel.getWidth(), glas_panel.getHeight());
  m_xy.setImage(glas_panel);
  m_xy.setInlay(1);
  m_xy.setColor(vector_color);
  m_xy.setTooltip(
      "Transition seamlessly through four waveforms. Each corner contains a "
      "waveform, which can be selected by the dropdowns on the right.");
  addAndMakeVisible(m_xy);

  juce::Image glas_panel_vecwave = ImageCache::getFromMemory(
      BinaryData::glaspanel_big_png, BinaryData::glaspanel_big_pngSize);
  m_vec_a.setImage(glas_panel_vecwave);
  m_vec_a.setInlay(1);
  m_vec_a.setEditableText(false);
  m_vec_a.setSelectedId(101, dontSendNotification);
  m_vec_a.setBounds(VEC_WAVE_X, VEC_WAVE_Y, glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_a.setColor(vector_color);
  m_vec_a.setTooltip("Select the waveform to the bottom left of the XY pad");
  m_vec_a.onChange = [&]() {
    m_value_tree.state.setProperty(m_vec_a_identifier,
                                   (float)m_vec_a.getSelectedId(), nullptr);
  };
  addChildComponent(m_vec_a);

  m_vec_b.setImage(glas_panel_vecwave);
  m_vec_b.setInlay(1);
  m_vec_b.setEditableText(false);
  m_vec_b.setSelectedId(102, dontSendNotification);
  m_vec_b.setBounds(VEC_WAVE_X, VEC_WAVE_Y + 1 * VEC_WAVE_OFFSET,
                    glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_b.setColor(vector_color);
  m_vec_b.setTooltip("Select the waveform to the top left of the XY pad");
  m_vec_b.onChange = [&]() {
    m_value_tree.state.setProperty(m_vec_b_identifier,
                                   (float)m_vec_b.getSelectedId(), nullptr);
  };
  addChildComponent(m_vec_b);

  m_vec_c.setImage(glas_panel_vecwave);
  m_vec_c.setInlay(1);
  m_vec_c.setEditableText(false);
  m_vec_c.setSelectedId(103, dontSendNotification);
  m_vec_c.setBounds(VEC_WAVE_X, VEC_WAVE_Y + 2 * VEC_WAVE_OFFSET,
                    glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_c.setColor(vector_color);
  m_vec_c.setTooltip("Select the waveform to the top right of the XY pad");
  m_vec_c.onChange = [&]() {
    m_value_tree.state.setProperty(m_vec_c_identifier,
                                   (float)m_vec_c.getSelectedId(), nullptr);
  };
  addChildComponent(m_vec_c);

  m_vec_d.setImage(glas_panel_vecwave);
  m_vec_d.setInlay(1);
  m_vec_d.setEditableText(false);
  m_vec_d.setSelectedId(104, dontSendNotification);
  m_vec_d.setBounds(VEC_WAVE_X, VEC_WAVE_Y + 3 * VEC_WAVE_OFFSET,
                    glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_d.setColor(vector_color);
  m_vec_d.setTooltip("Select the waveform to the bottom right of the XY pad");
  m_vec_d.onChange = [&]() {
    m_value_tree.state.setProperty(m_vec_d_identifier,
                                   (float)m_vec_d.getSelectedId(), nullptr);
  };
  addChildComponent(m_vec_d);

  m_xy_x.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_very_small_png,
                                BinaryData::black_knob_very_small_pngSize),
      256);
  m_xy_x.setBounds(X_POS_X, X_POS_Y, BLACK_KNOB_VERY_SMALL_SIZE_X,
                   BLACK_KNOB_VERY_SMALL_SIZE_Y);
  m_xy_x.setSliderStyle(Slider::RotaryVerticalDrag);
  m_xy_x.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_xy_x.setDoubleClickReturnValue(true, m_xy_x.getDoubleClickReturnValue(), ModifierKeys::ctrlModifier);
  m_xy_x.setNumDecimalPlacesToDisplay(3);
  m_xy_x.setKnobTooltip("X part of the XY pad");
  m_xy_x.onValueChange = [&] { m_xy.setX(m_xy_x.getValue()); };

  addChildComponent(m_xy_x);

  m_xy_y.setStrip(
      ImageCache::getFromMemory(BinaryData::black_knob_very_small_png,
                                BinaryData::black_knob_very_small_pngSize),
      256);
  m_xy_y.setBounds(Y_POS_X, Y_POS_Y, BLACK_KNOB_VERY_SMALL_SIZE_X,
                   BLACK_KNOB_VERY_SMALL_SIZE_Y);
  m_xy_y.setSliderStyle(Slider::RotaryVerticalDrag);
  m_xy_y.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_xy_y.setDoubleClickReturnValue(true, 0, ModifierKeys::ctrlModifier);
  m_xy_y.setNumDecimalPlacesToDisplay(3);
  m_xy_y.setKnobTooltip("X part of the XY pad");
  m_xy_y.onValueChange = [&] { m_xy.setY(m_xy_y.getValue()); };
  addChildComponent(m_xy_y);

  // todo
  REMOVE_EDITOR.setBounds(0, 0, 100, 25);
  addChildComponent(REMOVE_EDITOR);

  m_vol.setNumDecimalPlacesToDisplay(2);
  m_pw.setNumDecimalPlacesToDisplay(3);
  m_drift.setNumDecimalPlacesToDisplay(3);
  m_position.setNumDecimalPlacesToDisplay(3);
  m_position_multi.setNumDecimalPlacesToDisplay(3);
  m_spread.setNumDecimalPlacesToDisplay(3);
  m_detune.setNumDecimalPlacesToDisplay(3);
  m_speed.setNumDecimalPlacesToDisplay(2);
  m_fm.setNumDecimalPlacesToDisplay(3);
  m_lp.setNumDecimalPlacesToDisplay(1);
  m_hp.setNumDecimalPlacesToDisplay(1);

  m_carrier_ratio.setParameterId("osc" + m_osc_number + "_carrier_ratio");
  m_value_tree.addParameterListener("osc" + m_osc_number + "_carrier_ratio",
                                    &m_carrier_ratio);
  m_modulator_ratio.setParameterId("osc" + m_osc_number + "_modulator_ratio");
  m_value_tree.addParameterListener("osc" + m_osc_number + "_modulator_ratio",
                                    &m_modulator_ratio);

  TIMEADD("beforeForceValue")

  forceValueTreeOntoComponents(m_value_tree.state, std::stoi(m_osc_number),
                               false);

  setSize(247, 145);

  TIMEEND
}

OscComponent::~OscComponent() {
  m_carrier_waveselector.m_menu.setLookAndFeel(nullptr);
  m_modulator_waveselector.m_menu.setLookAndFeel(nullptr);

  m_value_tree.removeParameterListener("osc" + m_osc_number + "_carrier_ratio",
                                       &m_carrier_ratio);
  m_value_tree.removeParameterListener(
      "osc" + m_osc_number + "_modulator_ratio", &m_modulator_ratio);
}

void OscComponent::paint(Graphics &g) { g.drawImageAt(m_background, 0, 0); }

void OscComponent::resized() {
  Rectangle<int> area_pw(BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);
  area_pw.setPosition(PW_POS_X - BLACK_KNOB_SMALL_OFFSET_X,
                      PW_POS_Y - BLACK_KNOB_SMALL_OFFSET_Y);
  m_pw.setBounds(area_pw);

  Rectangle<int> area_drift(BLACK_KNOB_BIG_SIZE_X, BLACK_KNOB_BIG_SIZE_Y);
  area_drift.setPosition(DRIFT_POS_X - BLACK_KNOB_BIG_OFFSET_X,
                         DRIFT_POS_Y - BLACK_KNOB_BIG_OFFSET_Y);
  m_drift.setBounds(area_drift);
}

void OscComponent::setOscType(int p_osc_type) {
  if (p_osc_type == 0) {
    return;
  }
  if (m_osc_type == p_osc_type) {
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
void OscComponent::setOscTypeBypass() { m_background = m_background_bypass; }

void OscComponent::setOscTypeAnalog() {
  m_background = ImageCache::getFromMemory(BinaryData::analog_backdrop_png,
                                           BinaryData::analog_backdrop_pngSize);
  showAnalogComponents();
}
void OscComponent::setOscTypeChiptune() {
  m_background = ImageCache::getFromMemory(
      BinaryData::chiptune_backdrop_png, BinaryData::chiptune_backdrop_pngSize);
  showChiptuneComponents();
}

void OscComponent::setOscTypeFM() {
  m_background = ImageCache::getFromMemory(BinaryData::fm_backdrop_png,
                                           BinaryData::fm_backdrop_pngSize);
  showFMComponents();
}

void OscComponent::setOscTypePM() {
  m_background = ImageCache::getFromMemory(BinaryData::pm_backdrop_png,
                                           BinaryData::pm_backdrop_pngSize);
  showPMComponents();
}

void OscComponent::setOscTypeChipdraw() {
  m_background = ImageCache::getFromMemory(
      BinaryData::chipdraw_backdrop_png, BinaryData::chipdraw_backdrop_pngSize);
  showChipdrawComponents();
}

void OscComponent::setOscTypeWavedraw() {
  m_background = ImageCache::getFromMemory(
      BinaryData::wavedraw_backdrop_png, BinaryData::wavedraw_backdrop_pngSize);
  showWavedrawComponents();
}

void OscComponent::setOscTypeSpecdraw() {
  m_background = ImageCache::getFromMemory(
      BinaryData::specdraw_backdrop_png, BinaryData::specdraw_backdrop_pngSize);
  showSpecdrawComponents();
}

void OscComponent::setOscTypeNoise() {
  m_background = ImageCache::getFromMemory(BinaryData::noise_backdrop_png,
                                           BinaryData::noise_backdrop_pngSize);
  showNoiseComponents();
}

void OscComponent::setOscTypeVector() {
  m_background = ImageCache::getFromMemory(BinaryData::vector_backdrop_png,
                                           BinaryData::vector_backdrop_pngSize);
  showVectorComponents();
}

void OscComponent::setOscTypeWavetable() {
  m_background =
      ImageCache::getFromMemory(BinaryData::wavetable_backdrop_png,
                                BinaryData::wavetable_backdrop_pngSize);
  showWavetableComponents();
}

void OscComponent::setOscTypeMulti() {
  m_background = ImageCache::getFromMemory(BinaryData::multi_backdrop_png,
                                           BinaryData::multi_backdrop_pngSize);
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
  m_wavedraw_convert_REMOVE.setVisible(false);
  m_chipdraw_convert_REMOVE.setVisible(false);
  m_specdraw_convert_REMOVE.setVisible(false);
  REMOVE_EDITOR.setVisible(false);
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
}
void OscComponent::showVolComponent() { m_vol.setVisible(true); }
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
  m_chipdraw_convert_REMOVE.setVisible(true);
  REMOVE_EDITOR.setVisible(true);
}

void OscComponent::showWavedrawComponents() {
  showVolComponent();
  showPitchComponents();
  m_wavedraw.setVisible(true);
  m_wavedraw_convert.setVisible(true);
  m_wavedraw_convert_REMOVE.setVisible(true);
  REMOVE_EDITOR.setVisible(true);
}

void OscComponent::showSpecdrawComponents() {
  showVolComponent();
  showPitchComponents();
  m_specdraw.setVisible(true);
  m_specdraw_convert.setVisible(true);
  m_specdraw_convert_REMOVE.setVisible(true);
  REMOVE_EDITOR.setVisible(true);
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
}

void OscComponent::showWavetableComponents() {
  showVolComponent();
  showPitchComponents();
  m_wavetable_waveselector.setColor(juce::Colour(10, 40, 50));
  m_wavetable_waveselector.setTopLeftPosition(WAVETABLE_WAVE_X,
                                              WAVETABLE_WAVE_Y);
  m_position.setVisible(true);
  m_wavetable_waveselector.setVisible(true);
}

void OscComponent::showMultiComponents() {
  showVolComponent();
  showPitchComponents();
  m_position_multi.setVisible(true);
  m_spread.setVisible(true);
  m_detune.setVisible(true);
  m_wavetable_waveselector.setColor(juce::Colour(10, 50, 40));

  m_wavetable_waveselector.setTopLeftPosition(MULTI_WAVE_X, MULTI_WAVE_Y);
  m_wavetable_waveselector.setVisible(true);
}

void OscComponent::showNoiseComponents() {
  showVolComponent();
  m_lp.setVisible(true);
  m_hp.setVisible(true);
}

void OscComponent::createWavedrawTables() {
  // DBG("CREATEWAVETABLES OSC\n\n");
  // DBG(m_osc_number);
  WavetableContainer::getInstance().createWavedrawTable(
      std::stoi(m_osc_number) - 1, m_wavedraw.getDrawnTable(), 44100.f);

  // write values to audiovaluetree
  float *table = m_wavedraw.getDrawnTable();
  auto node = m_value_tree.state;
  for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
    node.setProperty(
        String("[" + std::to_string(i) + "]osc" + m_osc_number + "_wavedraw"),
        table[i], nullptr);
  }
}

void OscComponent::createChipdrawTables() {
  WavetableContainer::getInstance().createChipdrawTable(
      std::stoi(m_osc_number) - 1, m_chipdraw.getDrawnTable(), 44100.f);

  // write values to audiovaluetree
  float *table = m_chipdraw.getDrawnTable();
  auto node = m_value_tree.state;
  for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
    node.setProperty(
        String("[" + std::to_string(i) + "]osc" + m_osc_number + "_chipdraw"),
        table[i], nullptr);
  }
  // for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
  //   m_value_tree
  //       .getParameter("osc" + m_osc_number + "_chipdraw[" + std::to_string(i)
  //       +
  //                     "]")
  //       ->setValueNotifyingHost(table[i] * 0.5 + 0.5);
  // }
}

void OscComponent::createSpecdrawTables() {
  WavetableContainer::getInstance().createSpecdrawTable(
      std::stoi(m_osc_number) - 1, m_specdraw.getDrawnTable(), 44100.f);
  // write values to audiovaluetree
  float *table = m_specdraw.getDrawnTable();
  auto node = m_value_tree.state;
  for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
    node.setProperty(
        String("[" + std::to_string(i) + "]osc" + m_osc_number + "_specdraw"),
        table[i], nullptr);
  }
  // for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
  //   m_value_tree
  //       .getParameter("osc" + m_osc_number + "_specdraw[" + std::to_string(i)
  //       +
  //                     "]")
  //       ->setValueNotifyingHost(table[i]);
  // }
}

void OscComponent::writeWavedrawTableToFile() {
  WavetableContainer::getInstance().writeWavedrawTable(
      m_wavedraw.getDrawnTable(), REMOVE_EDITOR.getText().toStdString());
}

void OscComponent::writeSpecdrawTableToFile() {
  WavetableContainer::getInstance().writeSpecdrawTable(
      m_specdraw.getDrawnTable(), REMOVE_EDITOR.getText().toStdString());
}

void OscComponent::writeChipdrawTableToFile() {
  WavetableContainer::getInstance().writeChipdrawTable(
      m_chipdraw.getDrawnTable(), REMOVE_EDITOR.getText().toStdString());
}

void OscComponent::forceValueTreeOntoComponents(ValueTree p_tree, int p_index,
                                                bool p_create_wavetables) {
  std::string index = std::to_string(p_index);

  // analog waveform
  switch ((int)m_value_tree.state[m_analog_wave_identifier]) {
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
  m_wavetable_waveselector.setValue(m_value_tree.state[m_wavetable_identifier]);

  // vector
  m_vec_a.setSelectedId(m_value_tree.state[m_vec_a_identifier]);
  m_vec_b.setSelectedId(m_value_tree.state[m_vec_b_identifier]);
  m_vec_c.setSelectedId(m_value_tree.state[m_vec_c_identifier]);
  m_vec_d.setSelectedId(m_value_tree.state[m_vec_d_identifier]);


  // fm
  m_carrier_waveselector.setValue(
      m_value_tree.state[m_carrier_wave_identifier]);
  m_modulator_waveselector.setValue(
      m_value_tree.state[m_modulator_wave_identifier]);

  m_carrier_ratio.setValue(GETAUDIO(m_carrier_ratio_identifier));
  //DBG((float GETAUDIO(m_carrier_ratio_identifier)));

  m_modulator_ratio.setValue(GETAUDIO(m_modulator_ratio_identifier));
  //DBG("1");

  m_fm_exp.setValue(
      m_value_tree.getParameterAsValue("osc" + m_osc_number + "_exp_fm")
          .getValue());

          //DBG("FM\n\n\n");
          //DBG((float)m_value_tree.getParameterAsValue("osc" + m_osc_number + "_exp_fm")
          //.getValue());

  // chiptune
  m_chiptune_waveselector.setValue(
      m_value_tree.state[String("osc" + m_osc_number + "_chipwave")]);
  auto node = m_value_tree.state;

  // wavedraw
  float wavedraw_values[WAVEDRAW_STEPS_X];
  for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
    wavedraw_values[i] = (float)node[String("[" + std::to_string(i) + "]osc" +
                                            m_osc_number + "_wavedraw")];
  }

  m_wavedraw.setDrawnTable(wavedraw_values);
  m_wavedraw_convert.setToggleState(true, dontSendNotification);

  // chipdraw
  for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
    wavedraw_values[i] = (float)node[String("[" + std::to_string(i) + "]osc" +
                                            m_osc_number + "_chipdraw")];
  }
  m_chipdraw.setDrawnTable(wavedraw_values);
  m_chipdraw_convert.setToggleState(true, dontSendNotification);

  // specdraw
  for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
    wavedraw_values[i] = (float)node[String("[" + std::to_string(i) + "]osc" +
                                            m_osc_number + "_specdraw")];
  }
  m_specdraw.setDrawnTable(wavedraw_values);
  m_specdraw_convert.setToggleState(true, dontSendNotification);

  if (p_create_wavetables) {
    createWavedrawTables();
    createChipdrawTables();
    createSpecdrawTables();
  }
}
