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
#include "LFOSelectorComponent.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "OdinKnob.h"
#include "TextLabel.h"
#include "SyncTimeSelector.h"

#define LFO_FREQ_MIN 0.05
#define LFO_FREQ_MAX 100
#define LFO_FREQ_DEFAULT 1
#define LFO_FREQ_MID 3

class LFOComponent : public Component {
public:
	LFOComponent(AudioProcessorValueTreeState &vts, const std::string &p_lfo_number, bool p_is_standalone);
	~LFOComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void resized() override;
	void setSync(bool p_sync) {
		if (m_sync_active != p_sync) {
			m_sync_active = p_sync;
			if (m_sync_active) {
				m_freq.setVisible(false);
				m_sync_time.setVisible(true);
			} else {
				m_freq.setVisible(true);
				m_sync_time.setVisible(false);
			}
			repaint();
		}
	}

	void setSyncOverdraw(juce::Image p_image) {
		m_freq_overdraw = p_image;
	}

private:
	Image m_knob_guide;

	void toggleSync() {
		setSync(!m_sync_active);
	}

	LFOSelectorComponent m_selector;
	SyncTimeSelector m_sync_time;

	OdinKnob m_freq;

	OdinButton m_reset;
	OdinButton m_sync;

	juce::Image m_freq_overdraw;
	bool m_sync_active = false;

	std::string m_lfo_number;
	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_freq_attach;

	std::unique_ptr<OdinButtonAttachment> m_reset_attach;

	Identifier m_lfo_wave_identifier;
	Identifier m_lfo_synctime_numerator_identifier;
	Identifier m_lfo_synctime_denominator_identifier;

	bool m_is_standalone_plugin;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOComponent)
};
