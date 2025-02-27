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
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "OdinKnob.h"
#include "SyncTimeSelector.h"
#include "TextLabel.h"

#define HP_FREQ_MIN 80
#define HP_FREQ_MAX 18000
#define HP_FREQ_MID 1280
#define HP_FREQ_DEFAULT HP_FREQ_MIN
#define DRY_WET_MID 0.3
#define DRY_DEFAULT 1
#define WET_DEFAULT 0.8
#define FEEDBACK_MID 0.3
#define TIME_MAX 3
#define TIME_MIN 0.01
#define TIME_MID 1
#define TIME_DEFAULT 1

class DelayComponent : public Component {
public:
	DelayComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone);
	~DelayComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void paint(Graphics &) override;
	void resized() override;

	void setImage(juce::Image p_background) {
		m_background = p_background;
	}

	void setSync(bool p_sync_on);

private:
	bool m_is_standalone_plugin;
	bool m_sync_on = false;

	juce::Image m_background;

	TextLabel m_time_label;
	TextLabel m_feedback_label;
	TextLabel m_hp_label;
	TextLabel m_ducking_label;
	TextLabel m_dry_label;
	TextLabel m_wet_label;

	OdinKnob m_time;
	OdinKnob m_feedback;
	OdinKnob m_HP;
	OdinKnob m_ducking;
	OdinKnob m_dry;
	OdinKnob m_wet;

	SyncTimeSelector m_sync_time;

	OdinButton m_sync;
	OdinButton m_pingpong;

	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_delay_time_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_feedback_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_hp_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_ducking_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_dry_attach;
	std::unique_ptr<OdinKnobAttachment> m_delay_wet_attach;

	//std::unique_ptr<OdinButtonAttachment> m_sync_attach;
	std::unique_ptr<OdinButtonAttachment> m_pingpong_attach;

	Identifier m_delay_synctime_numerator_identifier;
	Identifier m_delay_synctime_denominator_identifier;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayComponent)
};
