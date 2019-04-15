/*
  ==============================================================================

    WavedrawDisplay.cpp
    Created: 2 Mar 2019 1:38:37am
    Author:  frot

  ==============================================================================
*/

#include "WavedrawDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath> //todo remove

//==============================================================================
WavedrawDisplay::WavedrawDisplay() {
  m_glaspanel = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/drawpanel.png"));

  setSize(m_glaspanel.getWidth(), m_glaspanel.getHeight());

  for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
    m_draw_values[i] = sin((float)i * 2 * M_PI / WAVEDRAW_STEPS_X) * 0.9;
  }

  setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
  
}

WavedrawDisplay::~WavedrawDisplay() {}

void WavedrawDisplay::paint(Graphics &g) {

  g.setColour(m_color);
  juce::Point<int> top_left = getLocalBounds().getTopLeft();
  top_left.addXY(m_inlay + 1, m_inlay);
  juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
  bottom_right.addXY(-m_inlay - 1, -m_inlay);
  g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //

  g.setColour(m_draw_color);

  float width = (float)(getWidth() - DRAW_INLAY_LEFT - DRAW_INLAY_RIGHT) /
                (float)WAVEDRAW_STEPS_X;
  float height = (float)(getHeight() - DRAW_INLAY_UP - DRAW_INLAY_DOWN) / 2.f;
  float mid = (float)getHeight() / 2.f;

  for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
    // g.drawLine(DRAW_INLAY_LEFT + i * width, mid - m_draw_values[i] * height,
    // DRAW_INLAY_LEFT + (i + 1) * width, mid - m_draw_values[i] * height,
    // WAVEDRAW_THICCNESS);
    if (i != WAVEDRAW_STEPS_X - 1) {
      g.drawLine(DRAW_INLAY_LEFT + (i)*width, mid - m_draw_values[i] * height,
                 DRAW_INLAY_LEFT + (i + 1) * width,
                 mid - m_draw_values[i + 1] * height, WAVEDRAW_THICCNESS);
    } else {
      g.drawLine(DRAW_INLAY_LEFT + (i)*width, mid - m_draw_values[i] * height,
                 DRAW_INLAY_LEFT + (i + 1) * width,
                 mid - m_draw_values[0] * height, WAVEDRAW_THICCNESS);
    }
  }

  g.drawImageAt(m_glaspanel, 0, 0);
}

void WavedrawDisplay::resized() {}

void WavedrawDisplay::mouseDrag(const MouseEvent &event) { mouseInteraction(); }

void WavedrawDisplay::mouseDown(const MouseEvent &event) { mouseInteraction(); }

int max_int_wavedraw(int a, int b) { return a < b ? b : a; }//shouldnt be here
int min_int_wavedraw(int a, int b) { return a > b ? b : a; }//shouldnt be here

void WavedrawDisplay::mouseInteraction() {
  juce::Point<int> mouse_pos = getMouseXYRelative();
  float x = mouse_pos.getX();
  float y = mouse_pos.getY();

  // if (x > DRAW_INLAY_LEFT && x < getWidth() - DRAW_INLAY_RIGHT) {
  y = y < DRAW_INLAY_UP ? DRAW_INLAY_UP : y;
  y = y > getHeight() - DRAW_INLAY_DOWN ? getHeight() - DRAW_INLAY_DOWN : y;
  x = x > DRAW_INLAY_LEFT ? x : DRAW_INLAY_LEFT + 1;
  x = x < getWidth() - DRAW_INLAY_RIGHT ? x : getWidth() - DRAW_INLAY_RIGHT - 1;

  float float_x =
      (x - DRAW_INLAY_LEFT) / (getWidth() - DRAW_INLAY_LEFT - DRAW_INLAY_RIGHT);
  int step_x = (int)(floor(float_x * WAVEDRAW_STEPS_X));

  float float_y =
      (y - DRAW_INLAY_UP) / (getHeight() - DRAW_INLAY_UP - DRAW_INLAY_DOWN);
  float_y = 2 * (0.5 - float_y);
  // float_y = (round(float_y * WAVEDRAW_STEPS_Y))/WAVEDRAW_STEPS_Y;

  if (m_mouse_was_down) {
    for (int i = min_int_wavedraw(step_x, m_last_x_value); i <= max_int_wavedraw(step_x, m_last_x_value);
         ++i) {
      m_draw_values[i] = float_y;
    }
  } else {
    m_draw_values[step_x] = float_y;
  }
  m_last_x_value = step_x;
  //}
  onDraw();
  repaint();
  // if (!m_mouse_was_down) {
  m_mouse_was_down = true;
  //  DBG("MOUSE DOWN!");
  //}
}

void WavedrawDisplay::mouseUp(const MouseEvent &event) {
  // if(m_mouse_was_down){
  //  m_mouse_was_down = false;
  m_mouse_was_down = false;
  // DBG("UP!");
  //}
}

float* WavedrawDisplay::getDrawnTable(){
  return m_draw_values;
}
