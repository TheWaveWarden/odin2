/*
  ==============================================================================

    WavetableDisplay.cpp
    Created: 28 Aug 2019 7:27:05pm
    Author:  frot

  ==============================================================================
*/

#include "WavetableDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
WavetableDisplay::WavetableDisplay() {
  setSize(2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_X,
          2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_Y);
}

WavetableDisplay::~WavetableDisplay() {}

void WavetableDisplay::paint(Graphics &g) {

  g.fillAll(getLookAndFeel().findColour(
      ResizableWindow::backgroundColourId)); // clear the background

  g.setColour(Colours::grey);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component
  g.setColour(Colours::white);
  /*
      g.setFont (14.0f);
      g.drawText ("LOWER WT: " + std::to_string(m_lower_wt) + "\nHIGHER WT:" +
      std::to_string(m_higher_wt) + "\nINTERPOLATION: " +
      std::to_string(m_interpolation), getLocalBounds(), Justification::centred,
      true);   // draw some placeholder text

  */

  if (m_position_left) {

    const float *left_table =
        WavetableContainer::getInstance().getWavetablePointers(m_lower_wt)[0];
    const float *right_table =
        WavetableContainer::getInstance().getWavetablePointers(m_higher_wt)[0];

    float inverse_polation = 1.f - m_interpolation;

    float width = 1; // step size 1
    float height = WTDISPLAY_SIZE_Y / 2;
    float mid = (float)WTDISPLAY_SIZE_Y / 2.f;

    for (int i = 0; i < WTDISPLAY_SIZE_X; ++i) {

      float draw_value =
          left_table[i] * inverse_polation + right_table[i] * m_interpolation;

      if (i != WTDISPLAY_SIZE_X) {
        float draw_value_next = left_table[i + 1] * inverse_polation +
                                right_table[i + 1] * m_interpolation;
        g.drawLine(WTDISPLAY_INLAY + (i)*width,
                   WTDISPLAY_INLAY + mid - draw_value * height,
                   WTDISPLAY_INLAY + (i + 1) * width,
                   WTDISPLAY_INLAY + mid - draw_value_next * height,
                   WAVEDISPLAY_THICCNESS);
      } else {
        float draw_value_next =
            left_table[0] * inverse_polation + right_table[0] * m_interpolation;
        g.drawLine(WTDISPLAY_INLAY + (i)*width,
                   WTDISPLAY_INLAY + mid - draw_value * height,
                   WTDISPLAY_INLAY + (i + 1) * width,
                   WTDISPLAY_INLAY + mid - draw_value_next * height,
                   WAVEDISPLAY_THICCNESS);
      }
    }
  } else {
    g.setColour(Colours::grey);
    g.drawArrow(Line<float>(17, getHeight() / 2, 7, getHeight() / 2), 0, 30,
                15);
  }
}
