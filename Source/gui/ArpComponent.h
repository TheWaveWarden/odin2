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

#include "../PluginProcessor.h"
#include "NumberSelectorWithText.h"
#include "OdinArpeggiator.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "StepComponent.h"
#include "SyncTimeSelector.h"
#include "TextLabel.h"
#include <JuceHeader.h>


#define ARP_TIMER_MILLISECS 10

#define NUMBER_OF_STEPS 16

class ArpComponent : public Component, public Timer {
public:
	ArpComponent(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts);
	~ArpComponent();

	void paint(Graphics &) override;
	void resized() override;

	//a timer to retrieve the data from audiothread
	void timerCallback() override;
	void setLEDActive(int p_LED);
	void setNoLEDActive();
	void setVisibleAndStartTimer(bool p_set_visible);

	void forceValueTreeOntoComponents(ValueTree p_tree);

private:
	void setNumberLEDsToShow(int p_number);
	void setModTranspose(bool p_is_mod);

    void setComponentsEnabled();

	OdinAudioProcessor &m_processor;
	AudioProcessorValueTreeState &m_value_tree;

	NumberSelectorWithText m_octave_selector;
	NumberSelectorWithText m_steps_selector;
	NumberSelectorWithText m_direction;
	NumberSelectorWithText m_gate;

	TextLabel m_mod1_label;

	OdinButton m_on;
	OdinButton m_one_shot;
	DoubleTextButton m_mod_transpose;

	SyncTimeSelector m_sync_time;

	StepComponent m_step_0;
	StepComponent m_step_1;
	StepComponent m_step_2;
	StepComponent m_step_3;
	StepComponent m_step_4;
	StepComponent m_step_5;
	StepComponent m_step_6;
	StepComponent m_step_7;
	StepComponent m_step_8;
	StepComponent m_step_9;
	StepComponent m_step_10;
	StepComponent m_step_11;
	StepComponent m_step_12;
	StepComponent m_step_13;
	StepComponent m_step_14;
	StepComponent m_step_15;

	std::unique_ptr<OdinButtonAttachment> m_on_attach;
	std::unique_ptr<OdinButtonAttachment> m_one_shot_attach;
	std::unique_ptr<OdinButtonAttachment> m_mod_transpose_attach;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArpComponent)
};