/*
  ==============================================================================

    LFOComponent.cpp
    Created: 27 Feb 2019 5:33:38pm
    Author:  frot

  ==============================================================================
*/

#include "LFOComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
LFOComponent::LFOComponent(AudioProcessorValueTreeState &vts,
                           std::string p_lfo_number)
    : m_value_tree(vts), m_lfo_number(p_lfo_number),
      m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_sync("sync", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_lfo_wave_identifier("lfo" + p_lfo_number + "_wave"),
      m_lfo_synctime_denominator_identifier("lfo" + p_lfo_number +
                                            "_synctime_denominator"),
      m_lfo_synctime_numerator_identifier("lfo" + p_lfo_number +
                                          "_synctime_numerator") {

  m_freq_attach.reset(new SliderAttachment(
      m_value_tree, "lfo" + m_lfo_number + "_freq", m_freq));

  m_sync_attach.reset(new ButtonAttachment(
      m_value_tree, "lfo" + m_lfo_number + "_sync", m_sync));
  m_reset_attach.reset(new ButtonAttachment(
      m_value_tree, "lfo" + m_lfo_number + "_reset", m_reset));

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
  m_reset.setBounds(LFO_RESET_POS_X, LFO_RESET_POS_Y, reset_1.getWidth(),
                    reset_1.getHeight());
  m_reset.setTooltip("Resets the LFO on\nevery new note");
  addAndMakeVisible(m_reset);
  m_reset.setTriggeredOnMouseDown(true);
  m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());

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

  m_sync.setImages(&sync_draw2, &sync_draw2, &sync_draw1, &sync_draw1,
                   &sync_draw4, &sync_draw4, &sync_draw3, &sync_draw3);
  m_sync.setClickingTogglesState(true);
  m_sync.setBounds(SYNC_POS_X, SYNC_POS_Y, sync_1.getWidth(),
                   sync_1.getHeight());
  m_sync.setTriggeredOnMouseDown(true);
  m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());
  m_sync.onClick = [&]() { setSync(m_sync.getToggleState()); };
  m_sync.setTooltip("Enables syncing the LFO\nto the speed of your track");
  addAndMakeVisible(m_sync);
  juce::Image black_knob_small = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png"));

  m_freq.setStrip(black_knob_small, N_KNOB_FRAMES);
  m_freq.setSliderStyle(Slider::RotaryVerticalDrag);
  m_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_freq.setRange(LFO_FREQ_MIN, LFO_FREQ_MAX);
  m_freq.setTextValueSuffix(" Hz");
  m_freq.setSkewFactorFromMidPoint(LFO_FREQ_MID);
  m_freq.setValue(LFO_FREQ_DEFAULT);
  m_freq.setNumDecimalPlacesToDisplay(2);
  m_freq.setDoubleClickReturnValue(true, LFO_FREQ_DEFAULT,
                                   ModifierKeys::ctrlModifier);
  m_freq.setKnobTooltip("The frequency of the LFO");
  addAndMakeVisible(m_freq);

  m_selector.OnValueChange = [&](int p_new_value) {
    m_value_tree.getParameter(m_lfo_wave_identifier)
        ->setValueNotifyingHost(((float)p_new_value) / 20.f);
  };
  m_selector.setTopLeftPosition(SELECTOR_POS_X, SELECTOR_POS_Y);
  m_selector.setTooltip("The waveform to be used for this LFO. WD1-3 are using "
                        "the wavedraw tables drawn in oscillators 1-3.");
  addAndMakeVisible(m_selector);

  m_sync_time.OnValueChange = [&](int p_left, int p_right) {
    m_value_tree.getParameter(m_lfo_synctime_numerator_identifier)
        ->setValueNotifyingHost(((float)p_left) / 20.f);
    m_value_tree.getParameter(m_lfo_synctime_denominator_identifier)
        ->setValueNotifyingHost(((float)p_right) / 20.f);
  };
  m_sync_time.setTopLeftPosition(SYNC_TIME_POS_X, SYNC_TIME_POS_Y);
  m_sync_time.setTooltip("Set the frequency in sync to your track.");
  addChildComponent(m_sync_time);

  setSync(false);
}

LFOComponent::~LFOComponent() {}

void LFOComponent::paint(Graphics &g) {
  if (m_sync_active) {
    // g.setColour(Colours::white);
    // g.fillRect(OVERDRAW_POS_X, OVERDRAW_POS_Y, OVERDRAW_SIZE_X,
    //           OVERDRAW_SIZE_Y);
    g.drawImageAt(m_freq_overdraw, OVERDRAW_POS_X, OVERDRAW_POS_Y);
  }
}

void LFOComponent::resized() {
  m_freq.setBounds(LFO_FREQ_POS_X, LFO_FREQ_POS_Y, BLACK_KNOB_SMALL_SIZE_Y,
                   BLACK_KNOB_SMALL_SIZE_Y);
}
