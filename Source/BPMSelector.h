/*
  ==============================================================================

    BPMSelector.h
    Created: 13 Jun 2019 9:47:14pm
    Author:  frot

  ==============================================================================
*/

#pragma once
#include "NumberSelector.h"

class BPMSelector : public NumberSelector {
public:
  BPMSelector(bool p_buttons_right) : NumberSelector(p_buttons_right) {
    juce::Image glas_panel = ImageCache::getFromMemory(
        BinaryData::glaspanel_midbig_png, BinaryData::glaspanel_midbig_pngSize);
    m_display.setImage(glas_panel);
    m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
    m_display.setInlay(1);
    m_display.setTextValueSuffix(" BPM");
    addAndMakeVisible(m_display);

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
    m_up.setBounds(glas_panel.getWidth(), 0, up_1.getWidth(), up_1.getHeight());
    addAndMakeVisible(m_up);
    m_up.setTriggeredOnMouseDown(false);
    m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());
    m_up.onClick = [&]() { increment(); };

    juce::Image down_1 = ImageCache::getFromMemory(
        BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
    juce::Image down_2 = ImageCache::getFromMemory(
        BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

    juce::DrawableImage down_draw1;
    juce::DrawableImage down_draw2;

    down_draw1.setImage(down_1);
    down_draw2.setImage(down_2);

    m_down.setImages(&down_draw2, &down_draw2, &down_draw1, &down_draw1,
                     &down_draw2, &down_draw2, &down_draw1, &down_draw1);
    m_down.setClickingTogglesState(true);
    m_down.setBounds(glas_panel.getWidth(), down_1.getHeight(),
                     down_1.getWidth(), down_1.getHeight());
    addAndMakeVisible(m_down);
    m_down.setTriggeredOnMouseDown(false);
    m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                     juce::Colour());
    m_down.onClick = [&]() { decrement(); };
    setSize(glas_panel.getWidth() + down_1.getWidth(),
            glas_panel.getHeight() + 1);

    if (p_buttons_right) {
      m_display.setTopLeftPosition(0, 0);
      m_up.setTopLeftPosition(glas_panel.getWidth() - 1, 1);
      m_down.setTopLeftPosition(glas_panel.getWidth() - 1,
                                down_1.getHeight() + 1);
    } else {
      m_display.setTopLeftPosition(down_1.getWidth(), 0);
      m_up.setTopLeftPosition(1, 1);
      m_down.setTopLeftPosition(1, down_1.getHeight() + 1);
    }

    m_display.onMouseDown = [&]() {
      // do nothing
    };

    m_display_width = glas_panel.getWidth();
    m_buttons_right = p_buttons_right;

    setValue(1);
  }
};