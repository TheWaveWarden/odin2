/*
  ==============================================================================

    XYPadComponent.h
    Created: 27 Feb 2019 8:45:18pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Knob.h"

#define HANDLE_DIAMETER 8
#define HANDLE_INLAY 4

#define VECTOR_UP 1
#define VECTOR_DOWN 70
#define VECTOR_LEFT 4
#define VECTOR_RIGHT 87
//==============================================================================
/*
 */
class XYPadComponent : public Component, public SettableTooltipClient {
public:
  XYPadComponent(Knob &p_x, Knob &p_y, bool p_vector_pad = false);
  ~XYPadComponent();

  void paint(Graphics &) override;
  void resized() override;
  void mouseDrag(const MouseEvent &event) override;
  void mouseDown(const MouseEvent &event) override;
  void mouseInteraction();

  void setImage(juce::Image p_panel) { m_panel = p_panel; }
  void setInlay(int p_inlay) { m_inlay = p_inlay; }
  void setX(float p_x) {
    m_value_x = p_x;
    repaint();
  }
  void setY(float p_y) {
    m_value_y = p_y;
    repaint();
  }

  void setColor(juce::Colour p_color){
    m_color = p_color;
  }

private:
  float m_value_x = 0.f;
  float m_value_y = 0.f;

  int m_inlay;
  juce::Image m_panel;

  Knob &m_knob_x;
  Knob &m_knob_y;

  bool m_vector_pad;

  juce::Colour m_color = STANDARD_DISPLAY_COLOR;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYPadComponent)
};
