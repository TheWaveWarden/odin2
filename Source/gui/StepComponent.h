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

#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "OdinKnob.h"
#include "StepLED.h"
#include <JuceHeader.h>


class StepComponent : public Component {
public:
	StepComponent(AudioProcessorValueTreeState &vts, int p_step_index);
	~StepComponent();

	void resized() override;

	void setLEDActive(int p_step_index);
	void setShowLED(int p_highest_led);
	void setModTranspose(bool p_is_mod);

private:
	Image m_knob_guide;

	AudioProcessorValueTreeState &m_value_tree;

	OdinKnob m_mod_1;
	OdinKnob m_mod_2;
	OdinKnob m_transpose;

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
