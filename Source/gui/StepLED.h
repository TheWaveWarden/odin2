/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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

#include <JuceHeader.h>

class StepLED : public Component {
public:
	StepLED() {
	}

	~StepLED() {
	}

	void paint(Graphics &g) override {
		if (m_LED_on) {
			g.drawImageAt(m_image_on, 0, 0);
		} else {
			g.drawImageAt(m_image_off, 0, 0);
		}
	}

	void resized() override {
	}

	void setGUIBig() {
		m_GUI_big   = true;
		m_image_on  = ImageCache::getFromMemory(BinaryData::Arp_LED_1_150_png, BinaryData::Arp_LED_1_150_pngSize);
		m_image_off = ImageCache::getFromMemory(BinaryData::Arp_LED_2_150_png, BinaryData::Arp_LED_2_150_pngSize);
		setSize(m_image_on.getHeight(), m_image_on.getWidth());
	}

	void setGUISmall() {
		m_GUI_big   = false;
		m_image_on  = ImageCache::getFromMemory(BinaryData::arp_LED_1_png, BinaryData::arp_LED_1_pngSize);
		m_image_off = ImageCache::getFromMemory(BinaryData::arp_LED_2_png, BinaryData::arp_LED_2_pngSize);
		setSize(m_image_on.getHeight(), m_image_on.getWidth());
	}

	void setLEDOn(bool p_on) {
		if (m_LED_on != p_on) {
			m_LED_on = p_on;
			//DBG("REPAINT");
			repaint();
		}
	}

private:
	bool m_GUI_big = true;
	bool m_LED_on  = false;

	juce::Image m_image_on;
	juce::Image m_image_off;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepLED)
};
