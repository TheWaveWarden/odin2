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

#include "SpectrumDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

SpectrumDisplay::SpectrumDisplay() {
	setSize(2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_X, 2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_Y);
}

SpectrumDisplay::~SpectrumDisplay() {
}

float floatabs(float p_input) {
	return p_input > 0 ? p_input : -p_input;
}

void SpectrumDisplay::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

#ifdef WTGEN
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

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
		float *spectrum_left  = m_WT_container->m_fourier_coeffs[m_lower_wt][0];
		float scale_left      = m_WT_container->m_fourier_coeffs[m_lower_wt][1][0];
		float *spectrum_right = m_WT_container->m_fourier_coeffs[m_higher_wt][0];
		float scale_right     = m_WT_container->m_fourier_coeffs[m_higher_wt][1][0];

		float inverse_polation = 1.f - m_interpolation;

		float height = WTDISPLAY_SIZE_Y;

		for (int i = 1; i < WTDISPLAY_SIZE_X / SPECTRUM_LINE_WIDTH; ++i) {

			float draw_value =
			    spectrum_left[i] * scale_left * inverse_polation + spectrum_right[i] * scale_right * m_interpolation;

			draw_value = floatabs(draw_value);
			draw_value = pow(draw_value, 0.5);

			if (draw_value > 1) {
				g.setColour(Colours::red);
			} else {
				g.setColour(Colours::white);
			}

			g.drawLine(WTDISPLAY_INLAY + (i)*SPECTRUM_LINE_WIDTH,
			           WTDISPLAY_INLAY + height - draw_value * height,
			           WTDISPLAY_INLAY + (i)*SPECTRUM_LINE_WIDTH,
			           WTDISPLAY_INLAY + height,
			           SPECTRUM_LINE_WIDTH - 0.2);
		}

	} else {
		g.setColour(Colours::grey);
		g.drawArrow(Line<float>(17, getHeight() / 2, 7, getHeight() / 2), 0, 30, 15);
	}
#endif
}
