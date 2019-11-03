/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 26 Feb 2019 9:04:59pm
    Author:  frot

  ==============================================================================
*/

#include "ADSRComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
ADSRComponent::ADSRComponent(AudioProcessorValueTreeState &vts,
                             std::string p_adsr_number)
    : m_loop("loop_button", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_value_tree(vts), m_adsr_number(p_adsr_number) {

  m_attack_attach.reset(new OdinSliderAttachment(
      m_value_tree, ("env" + m_adsr_number + "_attack"), m_attack));
  m_decay_attach.reset(new OdinSliderAttachment(
      m_value_tree, "env" + m_adsr_number + "_decay", m_decay));
  m_sustain_attach.reset(new OdinSliderAttachment(
      m_value_tree, "env" + m_adsr_number + "_sustain", m_sustain));
  m_release_attach.reset(new OdinSliderAttachment(
      m_value_tree, "env" + m_adsr_number + "_release", m_release));

  juce::Image loop_1 = ImageCache::getFromMemory(
      BinaryData::buttonloop_1_png, BinaryData::buttonloop_1_pngSize);
  juce::Image loop_2 = ImageCache::getFromMemory(
      BinaryData::buttonloop_2_png, BinaryData::buttonloop_2_pngSize);
  juce::Image loop_3 = ImageCache::getFromMemory(
      BinaryData::buttonloop_3_png, BinaryData::buttonloop_3_pngSize);
  juce::Image loop_4 = ImageCache::getFromMemory(
      BinaryData::buttonloop_4_png, BinaryData::buttonloop_4_pngSize);

  juce::DrawableImage loop_draw1;
  juce::DrawableImage loop_draw2;
  juce::DrawableImage loop_draw3;
  juce::DrawableImage loop_draw4;

  loop_draw1.setImage(loop_1);
  loop_draw2.setImage(loop_2);
  loop_draw3.setImage(loop_3);
  loop_draw4.setImage(loop_4);

  m_loop.setImages(&loop_draw2, &loop_draw2, &loop_draw1, &loop_draw1,
                   &loop_draw4, &loop_draw4, &loop_draw3, &loop_draw3);
  m_loop.setClickingTogglesState(true);
  m_loop.setBounds(LOOP_POS_X, LOOP_POS_Y, loop_1.getWidth(),
                   loop_1.getHeight());
  addAndMakeVisible(m_loop);
  m_loop.setAlwaysOnTop(true);
  m_loop.setTriggeredOnMouseDown(true);
  m_loop.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());
  m_loop.setTooltip("Loops the envelopes attack\n and decay sections");

  m_attack.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
  addAndMakeVisible(m_attack);
  m_decay.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
  addAndMakeVisible(m_decay);
  m_sustain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
  addAndMakeVisible(m_sustain);
  m_release.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
  addAndMakeVisible(m_release);

  m_attack.setRange(A_LOW_LIMIT, A_HIGH_LIMIT);
  m_attack.setTooltip(
      "Attack\nDefines how long the envelope\ntakes to reach the top peak");
  m_attack.setTextValueSuffix(" s");

  m_decay.setTextValueSuffix(" s");
  m_decay.setTooltip("Decay\nDefines how long the\n envelope takes to fall "
                     "from the top\n peak to the sustain level");

  m_sustain.setNumDecimalPlacesToDisplay(3);
  m_sustain.setTooltip("Sustain\nDefines the height of the evelope\nafter the "
                       "decay section is finished");

  m_release.setTextValueSuffix(" s");
  m_release.setTooltip("Release\nDefines how long the envelope takes\n to fall "
                       "back to zero after\nthe key is released");

  m_loop_attach.reset(new OdinButtonAttachment(
      m_value_tree, "env" + m_adsr_number + "_loop", m_loop));

  m_attack.setNumDecimalPlacesToDisplay(3);
  m_decay.setNumDecimalPlacesToDisplay(3);
  m_sustain.setNumDecimalPlacesToDisplay(3);
  m_release.setNumDecimalPlacesToDisplay(3);

  SET_CTR_KEY(m_attack);
  SET_CTR_KEY(m_decay);
  SET_CTR_KEY(m_sustain);
  SET_CTR_KEY(m_release);

  m_attack.setBounds(SLIDER_POS_X, SLIDER_POS_Y, SLIDER_SIZE_X, SLIDER_SIZE_Y);
  m_decay.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 1 - 1, SLIDER_POS_Y,
                    SLIDER_SIZE_X, SLIDER_SIZE_Y);
  m_sustain.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 2, SLIDER_POS_Y,
                      SLIDER_SIZE_X, SLIDER_SIZE_Y);
  m_release.setBounds(SLIDER_POS_X + SLIDER_OFFSET * 3, SLIDER_POS_Y,
                      SLIDER_SIZE_X, SLIDER_SIZE_Y);
}

ADSRComponent::~ADSRComponent() {}

void ADSRComponent::paint(Graphics &g) {
}

