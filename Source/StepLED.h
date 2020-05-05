#pragma once

#include <JuceHeader.h>

class StepLED : public Component {
public:
	StepLED() {
	}

	~StepLED() {
	}

	void paint(Graphics &g) override {
		if (m_GUI_big) {
			if (m_LED_on) {
				g.drawImageAt(m_image_on, 0, 0);
			} else {
				g.drawImageAt(m_image_off, 0, 0);
			}
		} else {
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
		m_GUI_big = false;
	}

	void setLEDOn(bool p_on) {
		if (m_LED_on != p_on) {
			m_LED_on = p_on;
			//DBG("REPAINT");
			repaint();
		}
	}

private:
	bool m_GUI_big = false;
	bool m_LED_on  = false;

	juce::Image m_image_on;
	juce::Image m_image_off;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepLED)
};
