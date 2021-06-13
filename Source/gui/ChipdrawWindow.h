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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#define INLAY_DEFAULT_CHIPDRAW 2

#define CHIPDRAW_STEPS_Y 8
#define DRAW_INLAY_LEFT 3
#define DRAW_INLAY_RIGHT 3
#define DRAW_INLAY_UP 5
#define DRAW_INLAY_DOWN 3
#define CHIPDRAW_THICCNESS 2

class ChipdrawWindow : public Component, public SettableTooltipClient
{
public:
  ChipdrawWindow();
  ~ChipdrawWindow();

  void paint(Graphics &) override;
  void setInlay(int p_inlay) { m_inlay = p_inlay; }
  void setColor(juce::Colour p_color) { m_color = p_color; }
  void setDrawColor(juce::Colour p_draw_color) { m_draw_color = p_draw_color; }

  void mouseDrag(const MouseEvent &event) override;
  void mouseDown(const MouseEvent &event) override;
  void mouseUp(const MouseEvent &event) override;

  void mouseInteraction();

  std::function<void()> onDraw= []() {};

  float* getDrawnTable();

  void setDrawnTable(float p_table[CHIPDRAW_STEPS_X]){
    for(int i = 0; i < CHIPDRAW_STEPS_X; ++i){
      //DBG(p_table[i]);
      m_draw_values[i] = p_table[i];
    }
    repaint();
  }
  
  void setGUIBig();
  void setGUISmall();
private:
  bool m_GUI_big = true;

  bool m_mouse_was_down = false;
  int m_last_x_value;
  float m_last_y_value;

  int m_inlay = INLAY_DEFAULT_CHIPDRAW;
  juce::Image m_glaspanel;
  juce::Colour m_color = juce::Colours::black;
  juce::Colour m_draw_color = juce::Colours::black;

  juce::Colour m_fill_color = juce::Colour(0x47d1bc84);

#ifndef WTGEN
  float m_draw_values[CHIPDRAW_STEPS_X] = {0.875f, 0.875f, 0.875f, 0.875f, 0.875f, 0.875f, 0.875f, 0.875f, -0.875f, -0.875f, -0.875f, -0.875f, -0.875f, -0.875f, -0.875f, -0.875f};
#endif
#ifdef WTGEN
  float m_draw_values[CHIPDRAW_STEPS_X] = {0};
#endif

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChipdrawWindow)
};
