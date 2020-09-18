#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"
#include "OdinMidiLearnBase.h"

class OdinAudioProcessor;

class LeftRightButton : public juce::Button, public OdinMidiLearnBase {
public:
	LeftRightButton(const String &buttonName);
	~LeftRightButton();

	void paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
	void setImage(juce::Image p_image, int p_frame) {
		if (p_frame == 1) {
			m_image_left = p_image;
		} else {
			m_image_right = p_image;
		}
	}

	void mouseDown(const MouseEvent &) override;
	void mouseExit(const MouseEvent &) override {
	}
	static void setOdinPointer(OdinAudioProcessor *p_pointer) {
		m_processor = p_pointer;
	}

	void stopMidiLearn() override {
		m_midi_learn = false;
		repaint();
	}

	void setMidiControlActive() override {
		m_midi_learn   = false;
		m_midi_control = true;
		repaint();
	}

	void setValue(bool p_on) {
		setToggleState(p_on, sendNotification);
		repaint();
	}

private:
	static OdinAudioProcessor *m_processor;
	juce::Image m_image_left;
	juce::Image m_image_right;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftRightButton)
};
