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
#include "DrawableSlider.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "TextLabel.h"

#define A_LOW_LIMIT 0.001
#define A_HIGH_LIMIT 10
#define A_DEFAULT 0.03
#define A_MID_VALUE 1

#define D_LOW_LIMIT A_LOW_LIMIT
#define D_HIGH_LIMIT A_HIGH_LIMIT
#define D_DEFAULT 1
#define D_MID_VALUE A_MID_VALUE

#define S_LOW_LIMIT 0
#define S_HIGH_LIMIT 1
#define S_DEFAULT 0.5
#define S_MID_VALUE 0.3

#define R_LOW_LIMIT A_LOW_LIMIT
#define R_HIGH_LIMIT A_HIGH_LIMIT
#define R_DEFAULT 0.03
#define R_MID_VALUE A_MID_VALUE
//==============================================================================
/*
 */
class ADSRComponent : public Component {
public:
	ADSRComponent(AudioProcessorValueTreeState &vts, const std::string &p_adsr_number);
	~ADSRComponent();

	void resized() override;

private:
    TextLabel m_attack_label;
    TextLabel m_decay_label;
    TextLabel m_sustain_label;
    TextLabel m_release_label;

	OdinButton m_loop;
	DrawableSlider m_attack;
	DrawableSlider m_decay;
	DrawableSlider m_sustain;
	DrawableSlider m_release;

	std::string m_adsr_number;
	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinSliderAttachment> m_attack_attach;
	std::unique_ptr<OdinSliderAttachment> m_decay_attach;
	std::unique_ptr<OdinSliderAttachment> m_sustain_attach;
	std::unique_ptr<OdinSliderAttachment> m_release_attach;

	std::unique_ptr<OdinButtonAttachment> m_loop_attach;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};
