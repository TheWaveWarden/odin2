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
FXComponent::FXComponent() : 
      m_sync("sync", juce::DrawableButton::ButtonStyle::ImageRaw) , 
      m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw)  {
  juce::Image metal_knob_big = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png"));
  m_amount.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_amount.setSliderStyle(Slider::RotaryVerticalDrag);
  m_amount.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_amount.setValue(FX_AMOUNT_DEFAULT);
#ifdef ODIN_LINUX
  m_amount.setDoubleClickReturnValue(true, FX_AMOUNT_DEFAULT, ModifierKeys::ctrlModifier);
#endif
#ifdef ODIN_WIN
  m_amount.setDoubleClickReturnValue(true, FX_AMOUNT_DEFAULT);
#endif
  m_amount.setKnobTooltip("How much the internal\nLFO modulates the\nfrequency of this effect");
  addAndMakeVisible(m_amount);
  
  m_freq.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_freq.setSliderStyle(Slider::RotaryVerticalDrag);
  m_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_freq.setRange(FX_FREQ_MIN, FX_FREQ_MAX);
  m_freq.setSkewFactorFromMidPoint(FX_FREQ_MID);
  m_freq.setTextValueSuffix(" Hz");
  m_freq.setNumDecimalPlacesToDisplay(2);
  m_freq.setValue(FX_FREQ_DEFAULT);
#ifdef ODIN_LINUX
  m_freq.setDoubleClickReturnValue(true, FX_FREQ_DEFAULT, ModifierKeys::ctrlModifier);
#endif
#ifdef ODIN_WIN
  m_freq.setDoubleClickReturnValue(true, FX_FREQ_DEFAULT);
#endif
  m_freq.setKnobTooltip("The frequency of the internal LFO");
  addAndMakeVisible(m_freq);
  
  m_dry_wet.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
  m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_dry_wet.setValue(FX_DRYWET_DEFAULT);
#ifdef ODIN_LINUX
  m_dry_wet.setDoubleClickReturnValue(true, FX_DRYWET_DEFAULT, ModifierKeys::ctrlModifier);
#endif
#ifdef ODIN_WIN
  m_dry_wet.setDoubleClickReturnValue(true, FX_DRYWET_DEFAULT);
#endif

  m_dry_wet.setKnobTooltip("The mix of processed and\n unprocessed signals");
  addAndMakeVisible(m_dry_wet);


  juce::Image reset_1 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_1.png"));
  juce::Image reset_2 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_2.png"));
  juce::Image reset_3 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_3.png"));
  juce::Image reset_4 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonreset_lfo_4.png"));

  juce::DrawableImage reset_draw1;
  juce::DrawableImage reset_draw2;
  juce::DrawableImage reset_draw3;
  juce::DrawableImage reset_draw4;

  reset_draw1.setImage(reset_1);
  reset_draw2.setImage(reset_2);
  reset_draw3.setImage(reset_3);
  reset_draw4.setImage(reset_4);

  m_reset.setImages(&reset_draw2, &reset_draw2, &reset_draw1,
                    &reset_draw1, &reset_draw4, &reset_draw4,
                    &reset_draw3, &reset_draw3);
  m_reset.setClickingTogglesState(true);
  m_reset.setBounds(FX_RESET_POS_X, FX_RESET_POS_Y, reset_1.getWidth(),
                    reset_1.getHeight());
  m_reset.setTriggeredOnMouseDown(true);
  m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());
 
  m_reset.setTooltip("Resets the internal\n LFO on every new note");
  addAndMakeVisible(m_reset);


  juce::Image sync_1 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonsync_1.png"));
  juce::Image sync_2 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonsync_2.png"));
  juce::Image sync_3 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonsync_3.png"));
  juce::Image sync_4 = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/buttons/buttonsync_4.png"));

  juce::DrawableImage sync_draw1;
  juce::DrawableImage sync_draw2;
  juce::DrawableImage sync_draw3;
  juce::DrawableImage sync_draw4;

  sync_draw1.setImage(sync_1);
  sync_draw2.setImage(sync_2);
  sync_draw3.setImage(sync_3);
  sync_draw4.setImage(sync_4);

  m_sync.setImages(&sync_draw2, &sync_draw2, &sync_draw1,
                    &sync_draw1, &sync_draw4, &sync_draw4,
                    &sync_draw3, &sync_draw3);
  m_sync.setClickingTogglesState(true);
  m_sync.setBounds(FX_SYNC_POS_X, FX_SYNC_POS_Y, sync_1.getWidth(),
                    sync_1.getHeight());
  m_sync.setTriggeredOnMouseDown(true);
  m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());
  m_sync.onStateChange = [&](){
      //setSync(m_sync.getToggleState());
  };
  m_sync.setTooltip("Syncs the internal LFOs\nspeed to your track");
  addAndMakeVisible(m_sync);


}

FXComponent::~FXComponent() {}

void FXComponent::paint(Graphics &g) { g.drawImageAt(m_background, 0, 0); }

void FXComponent::resized() {
  m_amount.setBounds(AMOUNT_POS_X, AMOUNT_POS_Y, METAL_KNOB_BIG_SIZE_X, METAL_KNOB_BIG_SIZE_Y);
  m_freq.setBounds(FREQ_POS_X, FREQ_POS_Y, METAL_KNOB_BIG_SIZE_X, METAL_KNOB_BIG_SIZE_Y);
  m_dry_wet.setBounds(DRY_WET_POS_X, DRY_WET_POS_Y, METAL_KNOB_BIG_SIZE_X, METAL_KNOB_BIG_SIZE_Y);
    
}
