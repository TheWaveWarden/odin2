/*
  ==============================================================================

    PhaserComponent.cpp
    Created: 27 Feb 2019 12:02:15am
    Author:  frot

  ==============================================================================
*/

#include "PhaserComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
PhaserComponent::PhaserComponent(AudioProcessorValueTreeState &vts,
                                 std::string p_fx_name, bool p_is_standalone)
    : m_value_tree(vts), m_fx_name(p_fx_name),
      m_is_standalone_plugin(p_is_standalone),
      m_sync("sync", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_fx_synctime_denominator_identifier(p_fx_name + "_synctime_denominator"),
      m_fx_synctime_numerator_identifier(p_fx_name + "_synctime_numerator") {

  m_rate_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_rate", m_rate));
      m_freq_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_freq", m_freq));
      m_width_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_width", m_width));
  m_mod_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_mod", m_mod));
  m_drywet_attach.reset(
      new SliderAttachment(m_value_tree, m_fx_name + "_drywet", m_dry_wet));

  m_reset_attach.reset(
      new ButtonAttachment(m_value_tree, m_fx_name + "_reset", m_reset));

  juce::Image metal_knob_mid = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/knobs/metal2/metal_knob_mid.png"));
  m_mod.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_mod.setSliderStyle(Slider::RotaryVerticalDrag);
  m_mod.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_mod.setValue(FX_AMOUNT_DEFAULT);
  m_mod.setDoubleClickReturnValue(true, FX_AMOUNT_DEFAULT,
                                  ModifierKeys::ctrlModifier);
  m_mod.setKnobTooltip(
      "How much the internal\nLFO modulates the\nfrequency of this effect");
  addAndMakeVisible(m_mod);

  m_rate.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_rate.setSliderStyle(Slider::RotaryVerticalDrag);
  m_rate.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_rate.setRange(FX_FREQ_MIN, FX_FREQ_MAX);
  m_rate.setSkewFactorFromMidPoint(FX_FREQ_MID);
  m_rate.setTextValueSuffix(" Hz");
  m_rate.setNumDecimalPlacesToDisplay(2);
  m_rate.setValue(FX_FREQ_DEFAULT);
  m_rate.setDoubleClickReturnValue(true, FX_FREQ_DEFAULT,
                                   ModifierKeys::ctrlModifier);
  m_rate.setKnobTooltip("The frequency of the internal LFO");
  addAndMakeVisible(m_rate);

  m_freq.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_freq.setSliderStyle(Slider::RotaryVerticalDrag);
  m_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_freq.setRange(80, 18000);
  m_freq.setSkewFactorFromMidPoint(2000);
  m_freq.setTextValueSuffix(" Hz");
  m_freq.setNumDecimalPlacesToDisplay(2);
  m_freq.setValue(FX_FREQ_DEFAULT);
  m_freq.setDoubleClickReturnValue(true, FX_FREQ_DEFAULT,
                                   ModifierKeys::ctrlModifier);
  m_freq.setKnobTooltip("The base frequency of the internal allpass filters");
  addAndMakeVisible(m_freq);

  m_width.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_width.setSliderStyle(Slider::RotaryVerticalDrag);
  m_width.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_width.setRange(0, 1);
  m_width.setNumDecimalPlacesToDisplay(2);
  m_width.setValue(0.5);
  m_width.setDoubleClickReturnValue(true, 0.5, ModifierKeys::ctrlModifier);
  m_width.setKnobTooltip("The spreading of the internal allpass filters");
  addAndMakeVisible(m_width);

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
  m_reset.setBounds(PHASER_RESET_POS_X, PHASER_RESET_POS_Y, reset_1.getWidth(),
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

  // if (!m_is_standalone_plugin) {
  m_sync_attach.reset(
      new ButtonAttachment(m_value_tree, m_fx_name + "_sync", m_sync));
  m_sync.setImages(&sync_draw2, &sync_draw2, &sync_draw1, &sync_draw1,
                   &sync_draw4, &sync_draw4, &sync_draw3, &sync_draw3);
  m_sync.setClickingTogglesState(true);
  m_sync.setBounds(PHASER_SYNC_POS_X, PHASER_SYNC_POS_Y, sync_1.getWidth(),
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
    m_value_tree.getParameter(m_fx_synctime_numerator_identifier)
        ->setValueNotifyingHost(((float)p_left) / 8.f);
    m_value_tree.getParameter(m_fx_synctime_denominator_identifier)
        ->setValueNotifyingHost(((float)p_right) / 7.f);
  };
  m_sync_time.setTopLeftPosition(PHASER_SYNC_TIME_POS_X,
                                 PHASER_SYNC_TIME_POS_Y);
  m_sync_time.setTooltip("Set the delay time in sync to your track");
  addChildComponent(m_sync_time);

  m_rate.setNumDecimalPlacesToDisplay(3);
  m_mod.setNumDecimalPlacesToDisplay(3);
  m_dry_wet.setNumDecimalPlacesToDisplay(3);

  m_mod.setBounds(PHASER_MOD_POS_X, PHASER_MOD_POS_Y, metal_knob_mid.getWidth(),
                  metal_knob_mid.getWidth());
  m_rate.setBounds(PHASER_RATE_POS_X, PHASER_RATE_POS_Y,
                   metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
  m_dry_wet.setBounds(PHASER_DRY_WET_POS_X, PHASER_DRY_WET_POS_Y,
                      metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
  m_freq.setBounds(PHASER_FREQ_POS_X, PHASER_FREQ_POS_Y,
                   metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
  m_width.setBounds(PHASER_WIDTH_POS_X, PHASER_WIDTH_POS_Y,
                   metal_knob_mid.getWidth(), metal_knob_mid.getWidth());
}

PhaserComponent::~PhaserComponent() {}

void PhaserComponent::paint(Graphics &g) {
  if (m_sync_enabled) {
    g.drawImageAt(m_background_sync, 0, 0);
  } else {
    g.drawImageAt(m_background_no_sync, 0, 0);
  }
}

void PhaserComponent::resized() {}

void PhaserComponent::setSyncEnabled(bool p_sync) {
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

void PhaserComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
  m_sync_time.setValueLeft(
      m_value_tree.getParameterAsValue(m_fx_synctime_numerator_identifier)
          .getValue());

  m_sync_time.setValueRight(
      m_value_tree.getParameterAsValue(m_fx_synctime_denominator_identifier)
          .getValue());
}
