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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "Knob.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"


#define REVERB_POS_X_SPACING 45
#define REVERB_POS_X_FIRST 18
#define REVERB_POS_Y_TOP 10
#define REVERB_POS_Y_BOTTOM 60


#define REVERB_DELAY_POS_X (REVERB_POS_X_FIRST + 0 * REVERB_POS_X_SPACING)
#define REVERB_DELAY_POS_Y REVERB_POS_Y_BOTTOM

#define REVERB_LOW_FREQ_POS_X (REVERB_POS_X_FIRST + 1 * REVERB_POS_X_SPACING)
#define REVERB_LOW_FREQ_POS_Y REVERB_POS_Y_TOP
#define REVERB_LOW_TIME_POS_X (REVERB_POS_X_FIRST + 1 * REVERB_POS_X_SPACING)
#define REVERB_LOW_TIME_POS_Y REVERB_POS_Y_BOTTOM

#define REVERB_MID_TIME_POS_X (REVERB_POS_X_FIRST + 2 * REVERB_POS_X_SPACING)
#define REVERB_MID_TIME_POS_Y REVERB_POS_Y_TOP
#define REVERB_HF_DAMPING_POS_X (REVERB_POS_X_FIRST + 2 * REVERB_POS_X_SPACING)
#define REVERB_HF_DAMPING_POS_Y REVERB_POS_Y_BOTTOM

#define REVERB_EQ_GAIN_POS_X (REVERB_POS_X_FIRST + 3 * REVERB_POS_X_SPACING)
#define REVERB_EQ_GAIN_POS_Y REVERB_POS_Y_TOP
#define REVERB_DRY_POS_X (REVERB_POS_X_FIRST + 3 * REVERB_POS_X_SPACING)
#define REVERB_DRY_POS_Y REVERB_POS_Y_BOTTOM

#define REVERB_DUCKING_POS_X (REVERB_POS_X_FIRST + 4 * REVERB_POS_X_SPACING)
#define REVERB_DUCKING_POS_Y REVERB_POS_Y_TOP
#define REVERB_WET_POS_X (REVERB_POS_X_FIRST + 4 * REVERB_POS_X_SPACING)
#define REVERB_WET_POS_Y REVERB_POS_Y_BOTTOM


class ReverbComponent : public Component {
public:
	ReverbComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone);
	~ReverbComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void paint(Graphics &) override;

	void setImage(juce::Image p_background) {
		m_background = p_background;
	}

	void setGUIBig();
	void setGUISmall();

private:
	bool m_GUI_big = true;

	bool m_is_standalone_plugin;
	bool m_sync_on = false;

	juce::Image m_background;

	Knob m_delay;
	Knob m_roomsize;
	Knob m_diffusion;
	Knob m_decay;
	Knob m_buildup;
	Knob m_EQ_gain;
	Knob m_EQ_freq;
	Knob m_ducking;
	Knob m_dry_wet;

	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_reverb_delay_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_roomsize_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_diffusion_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_decay_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_buildup_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_EQ_gain_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_EQ_freq_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_ducking_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_drywet_attach;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbComponent)
};
