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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"

class OdinAudioProcessor;

class LeftRightButton : public juce::Button {
public:
    enum class Type {
        osc_fm_exp,
        filter_comb_polarity
    };

	LeftRightButton(const String &buttonName, Type p_type);
	~LeftRightButton();

	void paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

	void mouseDrag(const MouseEvent &) override;
	void mouseDown(const MouseEvent &) override;
	void mouseExit(const MouseEvent &) override {
	}
	static void setOdinPointer(OdinAudioProcessor *p_pointer) {
		m_processor = p_pointer;
	}

	void setValue(bool p_on) {
		setToggleState(p_on, sendNotification);
		repaint();
	}

private:
	static OdinAudioProcessor *m_processor;
	juce::Image m_image_left;
	juce::Image m_image_right;

    bool m_is_up_down = false;

    Type m_type;
    UIAssets::Indices m_asset;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftRightButton)
};
