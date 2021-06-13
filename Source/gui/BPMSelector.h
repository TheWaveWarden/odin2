/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

#pragma once
#include "NumberSelector.h"

class BPMSelector : public NumberSelector {
public:
  BPMSelector(bool p_buttons_right) : NumberSelector(p_buttons_right) {
    m_display.setInlay(1);
    m_display.setTextValueSuffix(" BPM");
    addAndMakeVisible(m_display);

    m_up.setClickingTogglesState(true);
    addAndMakeVisible(m_up);
    m_up.setTriggeredOnMouseDown(false);
    m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());
    m_up.onClick = [&]() { increment(); };

    m_down.setClickingTogglesState(true);
    addAndMakeVisible(m_down);
    m_down.setTriggeredOnMouseDown(false);
    m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                     juce::Colour());
    m_down.onClick = [&]() { decrement(); };

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