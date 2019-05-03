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
OscComponent::OscComponent(AudioProcessorValueTreeState &vts,
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
      m_analog_wave_identifier("osc" + p_osc_number + "_analog_wave") {

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

  m_vec_a_attach.reset(new ComboBoxAttachment(
      m_value_tree, "osc" + m_osc_number + "_vec_a", m_vec_a));
  m_vec_b_attach.reset(new ComboBoxAttachment(
      m_value_tree, "osc" + m_osc_number + "_vec_b", m_vec_b));
  m_vec_c_attach.reset(new ComboBoxAttachment(
      m_value_tree, "osc" + m_osc_number + "_vec_c", m_vec_c));
  m_vec_d_attach.reset(new ComboBoxAttachment(
      m_value_tree, "osc" + m_osc_number + "_vec_d", m_vec_d));

  m_vol.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
      N_KNOB_FRAMES);
  m_vol.setSliderStyle(Slider::RotaryVerticalDrag);
  m_vol.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_vol.setRange(VOL_MIN, VOL_MAX);
  m_vol.setTextValueSuffix(" dB");
  m_vol.setValue(0);
  m_vol.setNumDecimalPlacesToDisplay(1);
  m_vol.setKnobTooltip("The volume of the oscillator");
  addChildComponent(m_vol);

  Rectangle<int> area_vol(BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);
  area_vol.setPosition(VOL_POS_X - BLACK_KNOB_SMALL_OFFSET_X,
                       VOL_POS_Y - BLACK_KNOB_SMALL_OFFSET_Y);
  m_vol.setBounds(area_vol);

  juce::Image reset_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_osc_1.png"));
  juce::Image reset_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_osc_2.png"));
  juce::Image reset_3 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_osc_3.png"));
  juce::Image reset_4 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_osc_4.png"));

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
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
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
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
      N_KNOB_FRAMES);
  m_semi.setSliderStyle(Slider::RotaryVerticalDrag);
  m_semi.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_semi.setRange(-SEMI_RANGE_MAX, SEMI_RANGE_MAX);
  m_semi.setNumDecimalPlacesToDisplay(0);
  m_semi.setKnobTooltip("The pitch of\nthe oscillator in semitones");
  addChildComponent(m_semi);

  m_fine.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
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

  juce::Image LED_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/LED_1.png"));
  juce::Image LED_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/LED_2.png"));
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
  m_LED_saw.setToggleState(true, dontSendNotification);
  m_LED_saw.setTriggeredOnMouseDown(false);
  m_LED_saw.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                      juce::Colour());
  m_LED_saw.setTooltip("Select the saw wave");
  m_LED_saw.onStateChange = [&]() {
    if (m_LED_saw.getToggleState()) {
      m_value_tree.getParameter(m_analog_wave_identifier)
          ->setValueNotifyingHost(0);
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
      m_value_tree.getParameter(m_analog_wave_identifier)
          ->setValueNotifyingHost(2.f / 4.f);
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
      m_value_tree.getParameter(m_analog_wave_identifier)
          ->setValueNotifyingHost(3.f / 4.f);
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
      m_value_tree.getParameter(m_analog_wave_identifier)
          ->setValueNotifyingHost(1);
    }
  };
  addChildComponent(m_LED_sine);

  m_pw.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
      N_KNOB_FRAMES);
  m_pw.setSliderStyle(Slider::RotaryVerticalDrag);
  m_pw.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_pw.setValue(PW_DEFAULT);
  m_pw.setDoubleClickReturnValue(true, PW_DEFAULT, ModifierKeys::ctrlModifier);
  m_pw.setKnobTooltip("The pulse width if\nthe pulse wave is selected");
  addChildComponent(m_pw);

  m_drift.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black4/black_knob_big.png")),
      N_KNOB_FRAMES);
  m_drift.setSliderStyle(Slider::RotaryVerticalDrag);
  m_drift.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_drift.setKnobTooltip("Slightly detunes the oscillator\nover time, like an "
                         "analog\noscillator would");
  addChildComponent(m_drift);

  juce::Image arp_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonarpeggiator_1.png"));
  juce::Image arp_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonarpeggiator_2.png"));
  juce::Image arp_3 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonarpeggiator_3.png"));
  juce::Image arp_4 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonarpeggiator_4.png"));

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

  juce::Image noise_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonnoise_1.png"));
  juce::Image noise_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonnoise_2.png"));
  juce::Image noise_3 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonnoise_3.png"));
  juce::Image noise_4 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonnoise_4.png"));

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

  juce::Image step_button_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonstep_1.png"));
  juce::Image step_button_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonstep_2.png"));
  juce::Image step_button_3 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonstep_3.png"));
  juce::Image step_button_4 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonstep_4.png"));

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
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
      256);
  m_step_1.setBounds(STEP_1_POS_X, STEP_1_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                     BLACK_KNOB_SMALL_SIZE_Y);
  m_step_1.setSliderStyle(Slider::RotaryVerticalDrag);
  m_step_1.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_step_1.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
  m_step_1.setNumDecimalPlacesToDisplay(0);
  m_step_1.setKnobTooltip(
      "The pitch of the\nfirst step of the\narpeggiator in semitones");
  addChildComponent(m_step_1);

  m_step_2.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
      256);
  m_step_2.setBounds(STEP_2_POS_X, STEP_2_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                     BLACK_KNOB_SMALL_SIZE_Y);
  m_step_2.setSliderStyle(Slider::RotaryVerticalDrag);
  m_step_2.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_step_2.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
  m_step_2.setValue(STEP_2_DEFAULT);
  m_step_2.setNumDecimalPlacesToDisplay(0);
  m_step_2.setKnobTooltip(
      "The pitch of the\nsecond step of the\narpeggiator in semitones");
  m_step_2.setDoubleClickReturnValue(true, STEP_2_DEFAULT,
                                     ModifierKeys::ctrlModifier);
  addChildComponent(m_step_2);

  m_step_3.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
      256);
  m_step_3.setBounds(STEP_3_POS_X, STEP_3_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                     BLACK_KNOB_SMALL_SIZE_Y);
  m_step_3.setSliderStyle(Slider::RotaryVerticalDrag);
  m_step_3.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_step_3.setRange(-STEP_RANGE_MAX, STEP_RANGE_MAX);
  m_step_3.setValue(STEP_3_DEFAULT);
  m_step_3.setNumDecimalPlacesToDisplay(0);
  m_step_3.setKnobTooltip(
      "The pitch of the\nthird step of the\narpeggiator in semitones");
  m_step_3.setDoubleClickReturnValue(true, STEP_2_DEFAULT,
                                     ModifierKeys::ctrlModifier);
  addChildComponent(m_step_3);

  m_fm.setStrip(ImageCache::getFromFile(juce::File(
                    GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png")),
                256);
  m_fm.setBounds(FM_POS_X, FM_POS_Y, METAL_KNOB_BIG_SIZE_X,
                 METAL_KNOB_BIG_SIZE_Y);
  m_fm.setSliderStyle(Slider::RotaryVerticalDrag);
  m_fm.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_fm.setKnobTooltip(
      "How much the modulator\nmodulates the pitch of\n the carrier wave");
  addChildComponent(m_fm);

  m_lp.setStrip(ImageCache::getFromFile(juce::File(
                    GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png")),
                256);
  m_lp.setBounds(LP_POS_X, LP_POS_Y, METAL_KNOB_BIG_SIZE_X,
                 METAL_KNOB_BIG_SIZE_Y);
  m_lp.setSliderStyle(Slider::RotaryVerticalDrag);
  m_lp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_lp.setRange(LP_MIN, LP_MAX);
  m_lp.setSkewFactorFromMidPoint(LP_MID);
  m_lp.setValue(LP_DEFAULT);
  m_lp.setDoubleClickReturnValue(true, LP_DEFAULT, ModifierKeys::ctrlModifier);
  m_lp.setTextValueSuffix(" Hz");
  m_lp.setNumDecimalPlacesToDisplay(0);
  m_lp.setKnobTooltip(
      "The frequency of\nthe lowpass filter which\nis applied to the noise");
  addChildComponent(m_lp);

  m_hp.setStrip(ImageCache::getFromFile(juce::File(
                    GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png")),
                256);
  m_hp.setBounds(HP_POS_X, HP_POS_Y, METAL_KNOB_BIG_SIZE_X,
                 METAL_KNOB_BIG_SIZE_Y);
  m_hp.setSliderStyle(Slider::RotaryVerticalDrag);
  m_hp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_hp.setRange(HP_MIN, HP_MAX);
  m_hp.setSkewFactorFromMidPoint(HP_MID);
  m_hp.setTextValueSuffix(" Hz");
  m_hp.setValue(HP_DEFAULT);
  m_hp.setDoubleClickReturnValue(true, HP_DEFAULT, ModifierKeys::ctrlModifier);
  m_hp.setNumDecimalPlacesToDisplay(0);
  m_hp.setKnobTooltip(
      "The frequency of\nthe highspass filter which\nis applied to the noise");
  addChildComponent(m_hp);

  m_position.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png")),
      256);
  m_position.setBounds(POSITION_POS_X, POSITION_POS_Y, METAL_KNOB_BIG_SIZE_X,
                       METAL_KNOB_BIG_SIZE_Y);
  m_position.setSliderStyle(Slider::RotaryVerticalDrag);
  m_position.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_position.setKnobTooltip("The position in the\nwavetable used. There "
                            "are\nfour tables through which\nyou can sweep");
  addChildComponent(m_position);

  m_detune.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png")),
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
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/metal1/metal_knob_small.png")),
      256);
  m_spread.setBounds(SPREAD_POS_X, SPREAD_POS_Y, METAL_KNOB_SMALL_SIZE_X,
                     METAL_KNOB_SMALL_SIZE_Y);
  m_spread.setSliderStyle(Slider::RotaryVerticalDrag);
  m_spread.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_spread.setKnobTooltip(
      "Spreads the oscillators to\n different positions in the\n wavetable");
  addChildComponent(m_spread);

  m_position_multi.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/metal1/metal_knob_small.png")),
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
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png")),
      256);
  m_speed.setBounds(SPEED_POS_X, SPEED_POS_Y, BLACK_KNOB_SMALL_SIZE_X,
                    BLACK_KNOB_SMALL_SIZE_Y);
  m_speed.setSliderStyle(Slider::RotaryVerticalDrag);
  m_speed.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_speed.setTextValueSuffix(" Hz");
  m_speed.setRange(SPEED_MIN, SPEED_MAX);
  m_speed.setSkewFactorFromMidPoint(SPEED_MID);
  m_speed.setValue(SPEED_DEFAULT);
  m_speed.setDoubleClickReturnValue(true, SPEED_DEFAULT,
                                    ModifierKeys::ctrlModifier);
  m_speed.setNumDecimalPlacesToDisplay(1);
  m_speed.setKnobTooltip("Speed of the arpeggiator");
  addChildComponent(m_speed);

  juce::Image chipdraw_convert_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonapply_1.png"));
  juce::Image chipdraw_convert_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonapply_2.png"));
  juce::Image chipdraw_convert_3 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonapply_3.png"));
  juce::Image chipdraw_convert_4 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonapply_4.png"));

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
    m_value_tree.getParameter(m_chipwave_identifier)
        ->setValueNotifyingHost(((float)p_new_value) / 5.f);
  };
  m_chiptune_waveselector.setTopLeftPosition(WAVE_CHIPTUNE_POS_X,
                                             WAVE_CHIPTUNE_POS_Y);
  m_chiptune_waveselector.addWave(1, "Saw");
  m_chiptune_waveselector.setDecrementValue(1, 1);
  m_chiptune_waveselector.addWave(2, "Pulse");
  m_chiptune_waveselector.addWave(3, "WOW");
  m_chiptune_waveselector.addWave(4, "henlo");
  m_chiptune_waveselector.setIncrementValue(4, 55);
  m_chiptune_waveselector.addWaveToSubmenu(55, "CHIPTUNE", 0);
  m_chiptune_waveselector.setDecrementValue(55, 4);
  m_chiptune_waveselector.addWaveToSubmenu(56, "lelz", 0);
  m_chiptune_waveselector.setIncrementValue(56, 56);
  m_chiptune_waveselector.applySubmenu(0, "realcrazyWaves");

  m_chiptune_waveselector.setValue(1);
  m_chiptune_waveselector.setColor(chip_color);
  m_chiptune_waveselector.setTooltip("Selects the wave for the oscillator");
  addChildComponent(m_chiptune_waveselector);

  juce::Colour fm_color(90, 40, 40);

  m_carrier_waveselector.OnValueChange = [&](int p_new_value) {
    m_value_tree.getParameter(m_carrier_wave_identifier)
        ->setValueNotifyingHost(((float)p_new_value - 0.5f) / 1000.f);
  };
  m_carrier_waveselector.setTopLeftPosition(WAVE_CARRIER_POS_X,
                                            WAVE_CARRIER_POS_Y);
  m_carrier_waveselector.addWave(1, "Saw");
  m_carrier_waveselector.addWave(2, "Pulse");
  m_carrier_waveselector.addWave(3, "WOW");
  m_carrier_waveselector.addWave(4, "henlo");
  m_carrier_waveselector.addWave(5, "CARRIER");
  m_carrier_waveselector.setValue(1);
  m_carrier_waveselector.setColor(fm_color);
  m_carrier_waveselector.setTooltip("Selects the wave for the carrier osc");
  addChildComponent(m_carrier_waveselector);

  m_wavetable_waveselector.OnValueChange = [&](int p_new_value) {
    m_value_tree.getParameter(m_wavetable_identifier)
        ->setValueNotifyingHost(((float)p_new_value - 0.5f) / 1000.f);
  };
  m_wavetable_waveselector.setTopLeftPosition(WAVE_CARRIER_POS_X,
                                              WAVE_CARRIER_POS_Y);
  m_wavetable_waveselector.addWave(1, "Classic Analog");
  m_wavetable_waveselector.addWave(2, "Additive 1");
  m_wavetable_waveselector.addWave(3, "Additive 2");
  m_wavetable_waveselector.addWave(4, "Additive 3");
  m_wavetable_waveselector.addWave(5, "Additive 4");
  m_wavetable_waveselector.addWave(6, "Harmonics1");
  m_wavetable_waveselector.addWave(7, "Harmonics2");
  m_wavetable_waveselector.addWave(8, "Harmonics3");
  m_wavetable_waveselector.addWave(9, "Harmonics4");
  m_wavetable_waveselector.addWave(10, "Organ");
  m_wavetable_waveselector.addWave(11, "BrokenSine");
  m_wavetable_waveselector.addWave(12, "Skyline");
  m_wavetable_waveselector.addWave(13, "Soft");
  m_wavetable_waveselector.addWave(14, "MultiSaw");
  m_wavetable_waveselector.addWave(15, "Wave15");
  m_wavetable_waveselector.addWave(16, "Wave16");
  m_wavetable_waveselector.addWave(17, "Wave17");
  m_wavetable_waveselector.addWave(18, "Wave18");
  m_wavetable_waveselector.addWave(19, "Wave19");
  m_wavetable_waveselector.addWave(20, "Wave20");
  m_wavetable_waveselector.addWave(21, "Wave21");
  m_wavetable_waveselector.addWave(22, "Wave22");
  m_wavetable_waveselector.addWave(23, "Wave23");
  m_wavetable_waveselector.addWave(1000, "Last");
  m_wavetable_waveselector.setValue(1);
  m_wavetable_waveselector.setTooltip("Selects the wave for the oscillator");
  addChildComponent(m_wavetable_waveselector);

  m_modulator_waveselector.OnValueChange = [&](int p_new_value) {
    m_value_tree.getParameter(m_modulator_wave_identifier)
        ->setValueNotifyingHost(((float)p_new_value - 0.5f) / 1000.f);
  };
  m_modulator_waveselector.setTopLeftPosition(WAVE_MODULATOR_POS_X,
                                              WAVE_MODULATOR_POS_Y);
  m_modulator_waveselector.addWave(1, "Saw");
  m_modulator_waveselector.addWave(2, "Pulse");
  m_modulator_waveselector.addWave(3, "WOW");
  m_modulator_waveselector.addWave(4, "henlo");
  m_modulator_waveselector.addWave(5, "MODULATOR");
  m_modulator_waveselector.setColor(fm_color);
  m_modulator_waveselector.setValue(1);
  m_modulator_waveselector.setTooltip("Selects the wave for the modulator osc");
  addChildComponent(m_modulator_waveselector);

  m_carrier_ratio.OnValueChange = [&](int p_new_value) {
    m_value_tree.getParameter(m_carrier_ratio_identifier)
        ->setValueNotifyingHost(((float)p_new_value - 0.5f) / 12.f);
  };
  m_carrier_ratio.setTopLeftPosition(RATIO_CARRIER_POS_X, RATIO_CARRIER_POS_Y);
  m_carrier_ratio.setRange(1, 12);
  m_carrier_ratio.setValue(1);
  m_carrier_ratio.setColor(fm_color);
  m_carrier_ratio.setTooltip(
      "The pitch ratio of the carrier to base frequency");
  addChildComponent(m_carrier_ratio);

  m_modulator_ratio.OnValueChange = [&](int p_new_value) {
    m_value_tree.getParameter(m_modulator_ratio_identifier)
        ->setValueNotifyingHost(((float)p_new_value - 0.5f) / 12.f);
  };
  m_modulator_ratio.setTopLeftPosition(RATIO_MODULATOR_POS_X,
                                       RATIO_MODULATOR_POS_Y);
  m_modulator_ratio.setRange(1, 12);
  m_modulator_ratio.setValue(1);
  m_modulator_ratio.setColor(fm_color);
  m_modulator_ratio.setTooltip(
      "The pitch ratio of the modulator to base frequency");
  addChildComponent(m_modulator_ratio);

  juce::Image fm_exp_left = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonexplin_1.png"));
  juce::Image fm_exp_right = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonexplin_3.png"));
  m_fm_exp.setImage(fm_exp_left, 1);
  m_fm_exp.setImage(fm_exp_right, 2);
  m_fm_exp.setBounds(FM_EXP_POS_X, FM_EXP_POS_Y, fm_exp_left.getWidth(),
                     fm_exp_left.getHeight());
  m_fm_exp.setToggleState(true, dontSendNotification);
  m_fm_exp.onStateChange = [&]() {
    // setLfo13(m_fm_exp_button.getToggleState());
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

  juce::Image glas_panel = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/vectorpanel.png"));
  m_xy.setTopLeftPosition(XY_POS_X, XY_POS_Y);
  m_xy.setSize(glas_panel.getWidth(), glas_panel.getHeight());
  m_xy.setImage(glas_panel);
  m_xy.setInlay(1);
  m_xy.setColor(vector_color);
  m_xy.setTooltip(
      "Transition seamlessly through four waveforms. Each corner contains a "
      "waveform, which can be selected by the dropdowns on the right.");
  addAndMakeVisible(m_xy);

  juce::Image glas_panel_vecwave = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/glaspanel_big.png"));
  m_vec_a.setImage(glas_panel_vecwave);
  m_vec_a.setInlay(1);
  m_vec_a.addItem("Saw", 1);
  m_vec_a.addItem("Square", 2);
  m_vec_a.addItem("Triangle", 3);
  m_vec_a.addItem("Sine", 4);
  m_vec_a.setEditableText(false);
  m_vec_a.setSelectedId(1, dontSendNotification);
  m_vec_a.setBounds(VEC_WAVE_X, VEC_WAVE_Y, glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_a.setColor(vector_color);
  m_vec_a.setTooltip("Select the waveform to the bottom left of the XY pad");
  addChildComponent(m_vec_a);

  m_vec_b.setImage(glas_panel_vecwave);
  m_vec_b.setInlay(1);
  m_vec_b.addItem("Saw", 1);
  m_vec_b.addItem("Square", 2);
  m_vec_b.addItem("Triangle", 3);
  m_vec_b.addItem("Sine", 4);
  m_vec_b.setEditableText(false);
  m_vec_b.setSelectedId(2, dontSendNotification);
  m_vec_b.setBounds(VEC_WAVE_X, VEC_WAVE_Y + 1 * VEC_WAVE_OFFSET,
                    glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_b.setColor(vector_color);
  m_vec_b.setTooltip("Select the waveform to the top left of the XY pad");
  addChildComponent(m_vec_b);

  m_vec_c.setImage(glas_panel_vecwave);
  m_vec_c.setInlay(1);
  m_vec_c.addItem("Saw", 1);
  m_vec_c.addItem("Square", 2);
  m_vec_c.addItem("Triangle", 3);
  m_vec_c.addItem("Sine", 4);
  m_vec_c.setEditableText(false);
  m_vec_c.setSelectedId(3, dontSendNotification);
  m_vec_c.setBounds(VEC_WAVE_X, VEC_WAVE_Y + 2 * VEC_WAVE_OFFSET,
                    glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_c.setColor(vector_color);
  m_vec_c.setTooltip("Select the waveform to the top right of the XY pad");
  addChildComponent(m_vec_c);

  m_vec_d.setImage(glas_panel_vecwave);
  m_vec_d.setInlay(1);
  m_vec_d.addItem("Saw", 1);
  m_vec_d.addItem("Square", 2);
  m_vec_d.addItem("Triangle", 3);
  m_vec_d.addItem("Sine", 4);
  m_vec_d.setEditableText(false);
  m_vec_d.setSelectedId(4, dontSendNotification);
  m_vec_d.setBounds(VEC_WAVE_X, VEC_WAVE_Y + 3 * VEC_WAVE_OFFSET,
                    glas_panel_vecwave.getWidth(),
                    glas_panel_vecwave.getHeight());
  m_vec_d.setColor(vector_color);
  m_vec_d.setTooltip("Select the waveform to the bottom right of the XY pad");
  addChildComponent(m_vec_d);

  m_xy_x.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black1/black_knob_very_small.png")),
      256);
  m_xy_x.setBounds(X_POS_X, X_POS_Y, BLACK_KNOB_VERY_SMALL_SIZE_X,
                    BLACK_KNOB_VERY_SMALL_SIZE_Y);
  m_xy_x.setSliderStyle(Slider::RotaryVerticalDrag);
  m_xy_x.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_xy_x.setDoubleClickReturnValue(true, 0,
                                    ModifierKeys::ctrlModifier);
  m_xy_x.setNumDecimalPlacesToDisplay(3);
  m_xy_x.setKnobTooltip("X part of the XY pad");
  m_xy_x.onValueChange = [&] { m_xy.setX(m_xy_x.getValue()); };
  
  addChildComponent(m_xy_x);


  m_xy_y.setStrip(
      ImageCache::getFromFile(juce::File(
          GRAPHICS_PATH + "cropped/knobs/black1/black_knob_very_small.png")),
      256);
  m_xy_y.setBounds(Y_POS_X, Y_POS_Y, BLACK_KNOB_VERY_SMALL_SIZE_X,
                    BLACK_KNOB_VERY_SMALL_SIZE_Y);
  m_xy_y.setSliderStyle(Slider::RotaryVerticalDrag);
  m_xy_y.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_xy_y.setDoubleClickReturnValue(true, 0,
                                    ModifierKeys::ctrlModifier);
  m_xy_y.setNumDecimalPlacesToDisplay(3);
  m_xy_y.setKnobTooltip("X part of the XY pad");
  m_xy_y.onValueChange = [&] { m_xy.setY(m_xy_y.getValue()); };
  addChildComponent(m_xy_y);

  // todo
  REMOVE_EDITOR.setBounds(0, 0, 100, 25);
  addChildComponent(REMOVE_EDITOR);

  setSize(247, 145);
}

OscComponent::~OscComponent() {}

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
  case 2:
    setOscTypeAnalog();
    break;
  case 3:
    setOscTypeWavetable();
    break;
  case 4:
    setOscTypeMulti();
    break;
  case 5:
    setOscTypeVector();
    break;
  case 6:
    setOscTypeChiptune();
    break;
  case 7:
    setOscTypeFM();
    break;
  case 8:
    setOscTypeNoise();
    break;
  case 9:
    setOscTypeWavedraw();
    break;
  case 10:
    setOscTypeChipdraw();
    break;
  case 11:
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
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/analog_backdrop.png"));
  showAnalogComponents();
}
void OscComponent::setOscTypeChiptune() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/chiptune_backdrop.png"));
  showChiptuneComponents();
}

void OscComponent::setOscTypeFM() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/fm_backdrop.png"));
  showFMComponents();
}

void OscComponent::setOscTypeChipdraw() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/chipdraw_backdrop.png"));
  showChipdrawComponents();
}

void OscComponent::setOscTypeWavedraw() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/wavedraw_backdrop.png"));
  showWavedrawComponents();
}

void OscComponent::setOscTypeSpecdraw() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/specdraw_backdrop.png"));
  showSpecdrawComponents();
}

void OscComponent::setOscTypeNoise() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/noise_backdrop.png"));
  showNoiseComponents();
}

void OscComponent::setOscTypeVector() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/vector_backdrop.png"));
  showVectorComponents();
}

void OscComponent::setOscTypeWavetable() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/wavetable_backdrop.png"));
  showWavetableComponents();
}

void OscComponent::setOscTypeMulti() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/multi_backdrop.png"));
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
  // todo sampling freqencies
  WavetableContainer::getInstance().createWavedrawTable(
      std::stoi(m_osc_number) - 1, m_wavedraw.getDrawnTable(), 44100.f);
}

void OscComponent::createChipdrawTables() {
  WavetableContainer::getInstance().createChipdrawTable(
      std::stoi(m_osc_number) - 1, m_chipdraw.getDrawnTable(), 44100.f);
}

void OscComponent::createSpecdrawTables() {
  WavetableContainer::getInstance().createSpecdrawTable(
      std::stoi(m_osc_number) - 1, m_specdraw.getDrawnTable(), 44100.f);
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
