/*
  ==============================================================================

    DelayComponent.cpp
    Created: 27 Feb 2019 12:02:33am
    Author:  frot

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayComponent.h"

//==============================================================================
DelayComponent::DelayComponent() : 
      m_sync("sync", juce::DrawableButton::ButtonStyle::ImageRaw) {

                  
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
  m_sync.setBounds(DELAY_SYNC_POS_X, DELAY_SYNC_POS_Y, sync_1.getWidth(),
                    sync_1.getHeight());
  m_sync.setTriggeredOnMouseDown(true);
  m_sync.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());
  m_sync.onStateChange = [&](){
      setSync(m_sync.getToggleState());
  };
  m_sync.setTooltip("Syncs the delay time to your track");
  addAndMakeVisible(m_sync);
    
  juce::Image metal_knob_big = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png"));
      juce::Image black_knob_mid = ImageCache::getFromFile(juce::File(
      GRAPHICS_PATH + "cropped/knobs/black3/black_knob_mid.png"));


  m_time.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_time.setSliderStyle(Slider::RotaryVerticalDrag);
  m_time.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_time.setRange(TIME_MIN,TIME_MAX);
  m_time.setSkewFactorFromMidPoint(TIME_MID);
  m_time.setValue(TIME_DEFAULT);
  m_time.setTextValueSuffix(" s");
  m_time.setNumDecimalPlacesToDisplay(2);
  m_time.setDoubleClickReturnValue(true, TIME_DEFAULT, ModifierKeys::ctrlModifier);
  m_time.setKnobTooltip("The time for the \nsignal to echo back");
  addAndMakeVisible(m_time);

  
  m_feedback.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_feedback.setSliderStyle(Slider::RotaryVerticalDrag);
  m_feedback.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_feedback.setSkewFactorFromMidPoint(FEEDBACK_MID);
  m_feedback.setKnobTooltip("How much to echo back");
  addAndMakeVisible(m_feedback);

  
  m_HP.setStrip(black_knob_mid, N_KNOB_FRAMES);
  m_HP.setSliderStyle(Slider::RotaryVerticalDrag);
  m_HP.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_HP.setRange(HP_FREQ_MIN, HP_FREQ_MAX);
  m_HP.setSkewFactorFromMidPoint(HP_FREQ_MID);
  m_HP.setDoubleClickReturnValue(true, HP_FREQ_DEFAULT, ModifierKeys::ctrlModifier);
  m_HP.setValue(HP_FREQ_DEFAULT);
  m_HP.setTextValueSuffix (" Hz");
  m_HP.setNumDecimalPlacesToDisplay(1);
  m_HP.setKnobTooltip("The cutoff frequency\n for a highpass filter\n which is applied\nto the signals");
  addAndMakeVisible(m_HP);

  
  m_ducking.setStrip(black_knob_mid, N_KNOB_FRAMES);
  m_ducking.setSliderStyle(Slider::RotaryVerticalDrag);
  m_ducking.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_ducking.setKnobTooltip("Lowers the wet volume\nwhen a signal is at the\ninput of the delay\nUse this to declutter\nyour sounds");
  addAndMakeVisible(m_ducking);

  
  m_dry.setStrip(black_knob_mid, N_KNOB_FRAMES);
  m_dry.setSliderStyle(Slider::RotaryVerticalDrag);
  m_dry.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_dry.setSkewFactorFromMidPoint(DRY_WET_MID);
  m_dry.setValue(DRY_DEFAULT);
  m_dry.setDoubleClickReturnValue(true, DRY_DEFAULT, ModifierKeys::ctrlModifier);
  m_dry.setKnobTooltip("Volume of the input signal");
  addAndMakeVisible(m_dry);

  m_wet.setStrip(black_knob_mid, N_KNOB_FRAMES);
  m_wet.setSliderStyle(Slider::RotaryVerticalDrag);
  m_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_wet.setSkewFactorFromMidPoint(DRY_WET_MID);
  m_wet.setValue(WET_DEFAULT);
  m_wet.setDoubleClickReturnValue(true, WET_DEFAULT, ModifierKeys::ctrlModifier);
  m_wet.setKnobTooltip("Volume of the delayed signal only");
  addAndMakeVisible(m_wet);

  m_sync_time.setTopLeftPosition(SYNC_TIME_DELAY_POS_X, SYNC_TIME_DELAY_POS_Y);
  m_sync_time.setTooltip("Set the delay time in sync to your track");
  addChildComponent(m_sync_time);

}

DelayComponent::~DelayComponent()
{
}

void DelayComponent::paint (Graphics& g)
{
    g.drawImageAt(m_background, 0,0);
}

void DelayComponent::resized()
{
    m_time.setBounds(TIME_POS_X, TIME_POS_Y, METAL_KNOB_BIG_SIZE_X, METAL_KNOB_BIG_SIZE_Y);
    m_feedback.setBounds(FEEDBACK_POS_X, FEEDBACK_POS_Y, METAL_KNOB_BIG_SIZE_X, METAL_KNOB_BIG_SIZE_Y);
    m_HP.setBounds(DELAY_HP_POS_X, DELAY_HP_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
    m_ducking.setBounds(DUCKING_POS_X, DUCKING_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
    m_dry.setBounds(DRY_POS_X, DRY_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
    m_wet.setBounds(WET_POS_X, WET_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);

}
