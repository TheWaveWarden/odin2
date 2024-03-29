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

#include "WavetableDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

WavetableDisplay::WavetableDisplay() {
	setSize(2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_X, 2 * WTDISPLAY_INLAY + WTDISPLAY_SIZE_Y);
}

WavetableDisplay::~WavetableDisplay() {
}

void WavetableDisplay::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

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

		const float *left_table  = m_WT_container->getWavetablePointers(m_lower_wt)[0];
		const float *right_table = m_WT_container->getWavetablePointers(m_higher_wt)[0];

		// const float *left_table =
		//     m_WT_container->getWavetablePointers(2)[0];
		// const float *right_table =
		//     m_WT_container->getWavetablePointers(2)[SUBTABLES_PER_WAVETABLE-20];

		float inverse_polation = 1.f - m_interpolation;

		float width  = 1; // step size 1
		float height = WTDISPLAY_SIZE_Y / 2;
		float mid    = (float)WTDISPLAY_SIZE_Y / 2.f;

		for (int i = 0; i < WTDISPLAY_SIZE_X; ++i) {

			g.setColour(Colours::white);
			// if(i >= 256 - 16 && i <= 256 + 16){
			//   g.setColour(Colours::red);
			// }

			float draw_value = left_table[i] * inverse_polation + right_table[i] * m_interpolation;

			if (i != WTDISPLAY_SIZE_X) {
				float draw_value_next = left_table[i + 1] * inverse_polation + right_table[i + 1] * m_interpolation;
				g.drawLine(WTDISPLAY_INLAY + (i)*width,
				           WTDISPLAY_INLAY + mid - draw_value * height,
				           WTDISPLAY_INLAY + (i + 1) * width,
				           WTDISPLAY_INLAY + mid - draw_value_next * height,
				           WAVEDISPLAY_THICCNESS);
			} else {
				float draw_value_next = left_table[0] * inverse_polation + right_table[0] * m_interpolation;
				g.drawLine(WTDISPLAY_INLAY + (i)*width,
				           WTDISPLAY_INLAY + mid - draw_value * height,
				           WTDISPLAY_INLAY + (i + 1) * width,
				           WTDISPLAY_INLAY + mid - draw_value_next * height,
				           WAVEDISPLAY_THICCNESS);
			}
		}
	} else {
		g.setColour(Colours::grey);
		g.drawArrow(Line<float>(17, getHeight() / 2, 7, getHeight() / 2), 0, 30, 15);
	}
}
