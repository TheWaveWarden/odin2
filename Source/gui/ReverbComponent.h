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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlassDropdown.h"
#include "OdinKnob.h"
#include "OdinButton.h"
#include "TextLabel.h"
#include "OdinControlAttachments.h"

class ReverbComponent : public Component {
public:
	ReverbComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone);
	~ReverbComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void paint(Graphics &) override;
	void resized() override;

	void setImage(juce::Image p_background) {
		m_background = p_background;
	}

private:
	bool m_is_standalone_plugin;
	bool m_sync_on = false;

	juce::Image m_background;

    TextLabel m_delay_label;
    TextLabel m_EQ_gain_label;
    TextLabel m_EQ_freq_label;
    TextLabel m_dry_wet_label;
    TextLabel m_mid_hall_label;
    TextLabel m_hf_damp_label;

	OdinKnob m_delay; // zita: delay / size
	OdinKnob m_EQ_gain; // zita: eq
	OdinKnob m_EQ_freq; // zita: eq
	OdinKnob m_dry_wet;  // zita: mix
	OdinKnob m_mid_hall; // zita: mid hall
	OdinKnob m_hf_damp;  //zita: hf damp

	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_reverb_delay_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_EQ_gain_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_EQ_freq_attach;
	//std::unique_ptr<OdinKnobAttachment> m_reverb_ducking_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_drywet_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_mid_hall_attach;
	std::unique_ptr<OdinKnobAttachment> m_reverb_hf_damp_attach;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbComponent)
};
