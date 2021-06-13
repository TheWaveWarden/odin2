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

#include "Knob.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "StepLED.h"
#include <JuceHeader.h>

#define KNOB_POS_X_100 1
#define MOD_POS_Y_100 2
#define TRANSPOSE_POS_Y_100 34
#define STEP_ON_X_100 2
#define STEP_ON_Y_100 68
#define STEP_LED_POS_X_100 9
#define STEP_LED_POS_Y_100 95
#define KNOB_GUIDE_X_100 1
#define KNOB_GUIDE_1_Y_100 0
#define KNOB_GUIDE_2_Y_100 32

#define KNOB_POS_X_150 1
#define MOD_POS_Y_150 3
#define TRANSPOSE_POS_Y_150 51
#define STEP_ON_X_150 4
#define STEP_ON_Y_150 102
#define STEP_LED_POS_X_150 14
#define STEP_LED_POS_Y_150 139
#define KNOB_GUIDE_X_150 0
#define KNOB_GUIDE_1_Y_150 0
#define KNOB_GUIDE_2_Y_150 47

class StepComponent : public Component {
public:
	StepComponent(AudioProcessorValueTreeState &vts, int p_step_index);
	~StepComponent();

	void paint(Graphics &) override;
	void resized() override;

	void setGUIBig();
	void setGUISmall();

	void setLEDActive(int p_step_index);
	void setShowLED(int p_highest_led);
	void setModTranspose(bool p_is_mod);

private:

	bool m_GUI_big = true;

	Image m_knob_guide;

	AudioProcessorValueTreeState &m_value_tree;

	Knob m_mod_1;
	Knob m_mod_2;
	Knob m_transpose;

	OdinButton m_step_on;
	StepLED m_led;

	std::unique_ptr<OdinButtonAttachment> m_step_on_attach;
	std::unique_ptr<OdinKnobAttachment> m_mod_1_attach;
	std::unique_ptr<OdinKnobAttachment> m_mod_2_attach;
	std::unique_ptr<OdinKnobAttachment> m_transpose_attach;

	int m_step_index;
	bool m_show_led = true;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepComponent)
};
