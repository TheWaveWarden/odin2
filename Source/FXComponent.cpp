/*
  ==============================================================================

    FXComponent.cpp
    Created: 27 Feb 2019 12:02:15am
    Author:  frot

  ==============================================================================
*/

#include "FXComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
FXComponent::FXComponent(AudioProcessorValueTreeState &vts,
                         std::string p_fx_name, bool p_is_standalone)
    : m_value_tree(vts), m_fx_name(p_fx_name),
      m_is_standalone_plugin(p_is_standalone),
      m_sync("sync", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_fx_synctime_denominator_identifier(p_fx_name + "_synctime_denominator"),
      m_fx_synctime_numerator_identifier(p_fx_name + "_synctime_numerator") {

  m_rate_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_rate", m_rate));
  m_amount_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_amount", m_amount));
  m_drywet_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_drywet", m_dry_wet));
  m_feedback_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_feedback", m_feedback));

  m_reset_attach.reset(
      new ButtonAttachment(m_value_tree, m_fx_name + "_reset", m_reset));

  juce::Image metal_knob_mid = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/knobs/metal2/metal_knob_mid.png"));
  m_amount.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_amount.setSliderStyle(Slider::RotaryVerticalDrag);
  m_amount.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_amount.setValue(FX_AMOUNT_DEFAULT);
  m_amount.setDoubleClickReturnValue(true, FX_AMOUNT_DEFAULT,
                                     ModifierKeys::ctrlModifier);
  m_amount.setKnobTooltip(
      "How much the internal\nLFO modulates the\nfrequency of this effect");
  addAndMakeVisible(m_amount);

  m_rate.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_rate.setSliderStyle(Slider::RotaryVerticalDrag);
  m_rate.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_rate.setRange(FX_FREQ_MIN, FX_FREQ_MAX);
  m_rate.setSkewFactorFromMidPoint(FX_FREQ_MID);
  m_rate.setTextValueSuffix(" Hz");
  m_rate.setNumDecimalPlacesToDisplay(2);
  m_rate.setValue(0.2f);
  m_rate.setDoubleClickReturnValue(true, FX_FREQ_DEFAULT,
                                   ModifierKeys::ctrlModifier);
  m_rate.setKnobTooltip("The frequency of the internal LFO");
  addAndMakeVisible(m_rate);

  m_feedback.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_feedback.setSliderStyle(Slider::RotaryVerticalDrag);
  m_feedback.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_feedback.setRange(-1, 1);
  if (m_fx_name == "chorus") {
    m_feedback.setValue(0);
    m_feedback.setDoubleClickReturnValue(true, 0, ModifierKeys::ctrlModifier);
  } else {
    m_feedback.setValue(0.6);
    m_feedback.setDoubleClickReturnValue(true, 0.6, ModifierKeys::ctrlModifier);
  }

  m_feedback.setKnobTooltip(
      "How much of the delay line is fed back onto itself");
  addAndMakeVisible(m_feedback);

  m_dry_wet.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
  m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_dry_wet.setValue(FX_DRYWET_DEFAULT);
  m_dry_wet.setDoubleClickReturnValue(true, FX_DRYWET_DEFAULT,
                                      ModifierKeys::ctrlModifier);

  m_dry_wet.setKnobTooltip("The mix of processed and\n unprocessed signals");
  addAndMakeVisible(m_dry_wet);

  juce::Image reset_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_1.png"));
  juce::Image reset_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_2.png"));
  juce::Image reset_3 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_3.png"));
  juce::Image reset_4 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_4.png"));

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
  m_reset.setBounds(FX_RESET_POS_X, FX_RESET_POS_Y, reset_1.getWidth(),
                    reset_1.getHeight());
  m_reset.setTriggeredOnMouseDown(true);
  m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());

  m_reset.setTooltip("Resets the internal\n LFO on every new note");
  addAndMakeVisible(m_reset);

  juce::Image sync_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonsync_1.png"));
  juce::Image sync_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonsync_2.png"));
  juce::Image sync_3 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonsync_3.png"));
  juce::Image sync_4 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonsync_4.png"));

  juce::DrawableImage sync_draw1;
  juce::DrawableImage sync_draw2;
  juce::DrawableImage sync_draw3;
  juce::DrawableImage sync_draw4;

  sync_draw1.setImage(sync_1);
  sync_draw2.setImage(sync_2);
  sync_draw3.setImage(sync_3);
  sync_draw4.setImage(sync_4);

  // TODO
  // if (!m_is_standalone_plugin) {
  m_sync_attach.reset(
      new ButtonAttachment(m_value_tree, m_fx_name + "_sync", m_sync));
  m_sync.setImages(&sync_draw2, &sync_draw2, &sync_draw1, &sync_draw1,
                   &sync_draw4, &sync_draw4, &sync_draw3, &sync_draw3);
  m_sync.setClickingTogglesState(true);
  m_sync.setBounds(FX_SYNC_POS_X, FX_SYNC_POS_Y, sync_1.getWidth(),
                   sync_1.getHeight());
  m_sync.setTriggeredOnMouseDown(true);
  m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());
  m_sync.setTooltip("Syncs the internal LFOs\nspeed to your track");
  addAndMakeVisible(m_sync);
  //}
  m_sync.onStateChange = [&]() {
    // if (!m_is_standalone_plugin) {
    setSyncEnabled(m_sync.getToggleState());
    //}
  };

  m_sync_time.OnValueChange = [&](int p_left, int p_right) {
    auto node = m_value_tree.state.getOrCreateChildWithName("NO_PARAM", nullptr);
    node.setProperty(m_fx_synctime_numerator_identifier, p_left, nullptr);
    node.setProperty(m_fx_synctime_denominator_identifier, p_right, nullptr);
  };

  m_sync_time.setTopLeftPosition(FX_SYNC_TIME_FX_POS_X, FX_SYNC_TIME_FX_POS_Y);
  m_sync_time.setTooltip("Set the delay time in sync to your track");
  addChildComponent(m_sync_time);

  m_amount.setBounds(FX_AMOUNT_POS_X, FX_AMOUNT_POS_Y,
                     metal_knob_mid.getWidth(), metal_knob_mid.getHeight());
  m_rate.setBounds(FX_FREQ_POS_X, FX_FREQ_POS_Y, metal_knob_mid.getWidth(),
                   metal_knob_mid.getHeight());
  m_feedback.setBounds(FX_FEEDBACK_POS_X, FX_FEEDBACK_POS_Y,
                       metal_knob_mid.getWidth(), metal_knob_mid.getHeight());
  m_dry_wet.setBounds(FX_DRY_WET_POS_X, FX_DRY_WET_POS_Y,
                      metal_knob_mid.getWidth(), metal_knob_mid.getHeight());

  m_rate.setNumDecimalPlacesToDisplay(3);
  m_amount.setNumDecimalPlacesToDisplay(3);
  m_feedback.setNumDecimalPlacesToDisplay(3);
  m_dry_wet.setNumDecimalPlacesToDisplay(3);
}

FXComponent::~FXComponent() {}

void FXComponent::paint(Graphics &g) {
  if (m_sync_enabled) {
    g.drawImageAt(m_background_sync, 0, 0);
  } else {
    g.drawImageAt(m_background_no_sync, 0, 0);
  }
}

void FXComponent::resized() {}

void FXComponent::setSyncEnabled(bool p_sync) {
  if (m_sync_enabled != p_sync) {
    m_sync_enabled = p_sync;
    if (m_sync_enabled) {
      m_rate.setVisible(false);
      m_sync_time.setVisible(true);
    } else {
      m_rate.setVisible(true);
      m_sync_time.setVisible(false);
    }
    repaint();
  }
}

void FXComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
  m_sync_time.setValueLeft(
      m_value_tree.getParameterAsValue(m_fx_synctime_numerator_identifier)
          .getValue());

  m_sync_time.setValueRight(
      m_value_tree.getParameterAsValue(m_fx_synctime_denominator_identifier)
          .getValue());
}
