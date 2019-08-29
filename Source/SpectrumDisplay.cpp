/*
  ==============================================================================

    SpectrumDisplay.cpp
    Created: 28 Aug 2019 7:27:05pm
    Author:  frot

  ==============================================================================
*/

#include "SpectrumDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

//==============================================================================
SpectrumDisplay::SpectrumDisplay() {
  setSize(2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_X,
          2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_Y);
}

SpectrumDisplay::~SpectrumDisplay() {}

float fabs(float p_input) { return p_input > 0 ? p_input : -p_input; }

void SpectrumDisplay::paint(Graphics &g) {
#ifdef WTGEN
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

  float *spectrum_left =
      WavetableContainer::getInstance().m_fourier_coeffs[m_lower_wt][0];
  float scale_left =
      WavetableContainer::getInstance().m_fourier_coeffs[m_lower_wt][1][0];
  float *spectrum_right =
      WavetableContainer::getInstance().m_fourier_coeffs[m_higher_wt][0];
  float scale_right =
      WavetableContainer::getInstance().m_fourier_coeffs[m_higher_wt][1][0];

  float inverse_polation = 1.f - m_interpolation;

  float height = WTDISPLAY_SIZE_Y;

  for (int i = 1; i < WTDISPLAY_SIZE_X / SPECTRUM_LINE_WIDTH; ++i) {

    float draw_value = spectrum_left[i] * scale_left * inverse_polation +
                       spectrum_right[i] * scale_right * m_interpolation;

    draw_value = fabs(draw_value);
    draw_value = pow(draw_value, 0.5);

    if (draw_value > 1) {
      g.setColour(Colours::red);
    } else {
      g.setColour(Colours::white);
    }

    g.drawLine(WTDISPLAY_INLAY + (i)*SPECTRUM_LINE_WIDTH,
               WTDISPLAY_INLAY + height - draw_value * height,
               WTDISPLAY_INLAY + (i)*SPECTRUM_LINE_WIDTH,
               WTDISPLAY_INLAY + height, SPECTRUM_LINE_WIDTH-0.2);
  }
#endif
}
