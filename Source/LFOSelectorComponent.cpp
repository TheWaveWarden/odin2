/*
  ==============================================================================

    LFOSelectorComponent.cpp
    Created: 27 Feb 2019 6:43:15pm
    Author:  frot

  ==============================================================================
*/

#include "LFOSelectorComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
LFOSelectorComponent::LFOSelectorComponent()
    : m_up("up", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_down("down", juce::DrawableButton::ButtonStyle::ImageRaw) {

  juce::Image glas_panel = ImageCache::getFromMemory(
      BinaryData::glaspanel_mid_png, BinaryData::glaspanel_mid_pngSize);
  m_display.setImage(glas_panel);
  m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
  m_display.setInlay(1);
  addAndMakeVisible(m_display);

  juce::Image sine = ImageCache::getFromMemory(
      BinaryData::sine_png, BinaryData::sine_pngSize);
  m_display.addWave(sine);
  juce::Image saw = ImageCache::getFromMemory(
      BinaryData::saw_png, BinaryData::saw_pngSize);
  m_display.addWave(saw);
  juce::Image triangle = ImageCache::getFromMemory(
      BinaryData::triangle_png, BinaryData::triangle_pngSize);
  m_display.addWave(triangle);
  juce::Image square = ImageCache::getFromMemory(
      BinaryData::square50_png, BinaryData::square50_pngSize);
  m_display.addWave(square);
  juce::Image square25 = ImageCache::getFromMemory(
      BinaryData::square25_png, BinaryData::square25_pngSize);
  m_display.addWave(square25);
  juce::Image square12 = ImageCache::getFromMemory(
      BinaryData::square12_png, BinaryData::square12_pngSize);
  m_display.addWave(square12);
  juce::Image peak = ImageCache::getFromMemory(
      BinaryData::peak_png, BinaryData::peak_pngSize);
  m_display.addWave(peak);
  juce::Image SH =
      ImageCache::getFromMemory(BinaryData::SH_png, BinaryData::SH_pngSize);
  m_display.addWave(SH);
  juce::Image pyram4 = ImageCache::getFromMemory(
      BinaryData::pyram4_png, BinaryData::pyram4_pngSize);
  m_display.addWave(pyram4);
  juce::Image pyram6 = ImageCache::getFromMemory(
      BinaryData::pyram6_png, BinaryData::pyram6_pngSize);
  m_display.addWave(pyram6);
  juce::Image pyram8 = ImageCache::getFromMemory(
      BinaryData::pyram8_png, BinaryData::pyram8_pngSize);
  m_display.addWave(pyram8);
  juce::Image pyram12 = ImageCache::getFromMemory(
      BinaryData::pyram12_png, BinaryData::pyram12_pngSize);
  m_display.addWave(pyram12);
  juce::Image stair3 = ImageCache::getFromMemory(
      BinaryData::stair3_png, BinaryData::stair3_pngSize);
  m_display.addWave(stair3);
  juce::Image stair4 = ImageCache::getFromMemory(
      BinaryData::stair4_png, BinaryData::stair4_pngSize);
  m_display.addWave(stair4);
  juce::Image stair6 = ImageCache::getFromMemory(
      BinaryData::stair6_png, BinaryData::stair6_pngSize);
  m_display.addWave(stair6);
  juce::Image stair8 = ImageCache::getFromMemory(
      BinaryData::stair8_png, BinaryData::stair8_pngSize);
  m_display.addWave(stair8);
  juce::Image stair12 = ImageCache::getFromMemory(
      BinaryData::stair12_png, BinaryData::stair12_pngSize);
  m_display.addWave(stair12);
  juce::Image wavedraw1 = ImageCache::getFromMemory(
      BinaryData::wavedraw1_png, BinaryData::wavedraw1_pngSize);
  m_display.addWave(wavedraw1);
  juce::Image wavedraw2 = ImageCache::getFromMemory(
      BinaryData::wavedraw2_png, BinaryData::wavedraw2_pngSize);
  m_display.addWave(wavedraw2);
  juce::Image wavedraw3 = ImageCache::getFromMemory(
      BinaryData::wavedraw3_png, BinaryData::wavedraw3_pngSize);
  m_display.addWave(wavedraw3);

  juce::Image up_1 = ImageCache::getFromMemory(
      BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
  juce::Image up_2 = ImageCache::getFromMemory(
      BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

  juce::DrawableImage up_draw1;
  juce::DrawableImage up_draw2;

  up_draw1.setImage(up_1);
  up_draw2.setImage(up_2);

  m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2,
                 &up_draw2, &up_draw1, &up_draw1);
  m_up.setClickingTogglesState(true);
  m_up.setBounds(glas_panel.getWidth() - 2, 1, up_1.getWidth(),
                 up_1.getHeight());
  addAndMakeVisible(m_up);
  m_up.setTriggeredOnMouseDown(false);
  m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                 juce::Colour());
  m_up.onClick = [&]() { increment(); };

  juce::Image down_1 = ImageCache::getFromMemory(
      BinaryData::buttondown_2_png, BinaryData::));
  juce::Image down_2 = ImageCache::getFromMemory(
      BinaryData::buttondown_1_png, BinaryData::));

  juce::DrawableImage down_draw1;
  juce::DrawableImage down_draw2;

  down_draw1.setImage(down_1);
  down_draw2.setImage(down_2);

  m_down.setImages(&down_draw2, &down_draw2, &down_draw1, &down_draw1,
                   &down_draw2, &down_draw2, &down_draw1, &down_draw1);
  m_down.setClickingTogglesState(true);
  m_down.setBounds(glas_panel.getWidth() - 2, 1 + down_1.getHeight(),
                   down_1.getWidth(), down_1.getHeight());
  addAndMakeVisible(m_down);
  m_down.setTriggeredOnMouseDown(false);
  m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());
  m_down.onClick = [&]() { decrement(); };
  setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight());
}

LFOSelectorComponent::~LFOSelectorComponent() {}

void LFOSelectorComponent::paint(Graphics &g) {}

void LFOSelectorComponent::resized() {
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
