/*
  ==============================================================================

    SpecdrawDisplay.cpp
    Created: 2 Mar 2019 11:00:22am
    Author:  frot

  ==============================================================================
*/

#include "SpecdrawDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
SpecdrawDisplay::SpecdrawDisplay() {
  m_glaspanel = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/drawpanel.png"));

  setSize(m_glaspanel.getWidth(), m_glaspanel.getHeight());

  setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
}

SpecdrawDisplay::~SpecdrawDisplay() {}

void SpecdrawDisplay::paint(Graphics &g) {

  g.setColour(m_color);
  juce::Point<int> top_left = getLocalBounds().getTopLeft();
  top_left.addXY(m_inlay + 1, m_inlay);
  juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
  bottom_right.addXY(-m_inlay - 1, -m_inlay);
  g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //

  g.setColour(m_draw_color);

  float height = (float)(getHeight() - DRAW_INLAY_UP_SPEC - DRAW_INLAY_DOWN_SPEC);

  for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
    g.drawLine(DRAW_INLAY_LEFT_SPEC + i,
               getHeight() - DRAW_INLAY_DOWN_SPEC - m_draw_values[i] * height,
               DRAW_INLAY_LEFT_SPEC + i, getHeight() - DRAW_INLAY_DOWN_SPEC);
  }

  g.drawImageAt(m_glaspanel, 0, 0);
}

void SpecdrawDisplay::resized() {}

void SpecdrawDisplay::mouseDrag(const MouseEvent &event) { mouseInteraction(); }

void SpecdrawDisplay::mouseDown(const MouseEvent &event) { mouseInteraction(); }

int max_int(int a, int b) { return a < b ? b : a; }
int min_int(int a, int b) { return a > b ? b : a; }

void SpecdrawDisplay::mouseInteraction() {
  juce::Point<int> mouse_pos = getMouseXYRelative();
  int x = mouse_pos.getX();
  float y = mouse_pos.getY();

  // if (x > DRAW_INLAY_LEFT_SPEC && x < getWidth() - DRAW_INLAY_RIGHT_SPEC) {
  y = y < DRAW_INLAY_UP_SPEC ? DRAW_INLAY_UP_SPEC : y;
  y = y > getHeight() - DRAW_INLAY_DOWN_SPEC ? getHeight() - DRAW_INLAY_DOWN_SPEC : y;
  x = x > DRAW_INLAY_LEFT_SPEC ? x : DRAW_INLAY_LEFT_SPEC + 1;
  x = x < getWidth() - DRAW_INLAY_RIGHT_SPEC ? x : getWidth() - DRAW_INLAY_RIGHT_SPEC - 1;
  x -= DRAW_INLAY_LEFT_SPEC + 1;

  float float_y = (getHeight() - DRAW_INLAY_DOWN_SPEC - y) /
                  (getHeight() - DRAW_INLAY_UP_SPEC - DRAW_INLAY_DOWN_SPEC);
  // float_y = (round(float_y * SPECDRAW_STEPS_Y))/SPECDRAW_STEPS_Y;
  if (m_mouse_was_down) {
    /*float left_y_value;
    float right_y_value;
    if(min_int(x, m_last_x_value) == x){
      left_y_value = float_y;
      right_y_value = m_last_y_value;
    }
    if(max_int(x, m_last_x_value) == x){
      right_y_value = float_y;
      left_y_value = m_last_y_value;
    }*/
    for (int i = min_int(x, m_last_x_value); i <= max_int(x, m_last_x_value);
         ++i) {
      m_draw_values[i] = float_y;//left_y_value  + (right_y_value - left_y_value)*(float)(i - min_int(x, m_last_x_value))/(max_int(x, m_last_x_value)- min_int(x, m_last_x_value));
    }
  } else {
    m_draw_values[x] = float_y;
  }
  m_last_x_value = x;
  m_last_y_value = float_y;
  //}
  onDraw();
  repaint();
  // if (!m_mouse_was_down) {
  m_mouse_was_down = true;
  //  DBG("MOUSE DOWN!");
  //}
}

void SpecdrawDisplay::mouseUp(const MouseEvent &event) {
  // if(m_mouse_was_down){
  //  m_mouse_was_down = false;
  m_mouse_was_down = false;
  //DBG("UP!");
  //}
}
