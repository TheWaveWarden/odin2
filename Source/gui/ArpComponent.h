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

#include <JuceHeader.h>
#include "StepComponent.h"
#include "../PluginProcessor.h"
#include "OdinArpeggiator.h"
#include "NumberSelectorWithText.h"
#include "SyncTimeSelector.h"
#include "OdinControlAttachments.h"
#include "OdinButton.h"

#define STEP_COMPONENT_WIDTH_100 27
#define STEP_COMPONENT_SPACING_100 5
#define STEP_COMPONENT_HEIGHT_100 111
#define STEP_COMPONENT_POS_X_100 46
#define STEP_COMPONENT_POS_Y_100 33
#define ON_OFF_POS_X_100 7
#define ON_OFF_POS_Y_100 6
#define ONE_SHOT_POS_X_100 1
#define ONE_SHOT_POS_Y_100 104
#define MOD_TRANSPOSE_POS_X_100 ONE_SHOT_POS_X_100
#define MOD_TRANSPOSE_POS_Y_100 73
#define OCTAVE_SELECTOR_X_100 140
#define OCTAVE_SELECTOR_Y_100 8
#define SYNC_TIME_ARP_POS_X_100 50
#define SYNC_TIME_ARP_POS_Y_100 OCTAVE_SELECTOR_Y_100
#define DIRECTION_SELECTOR_X_100 230
#define DIRECTION_SELECTOR_Y_100 OCTAVE_SELECTOR_Y_100
#define STEP_SELECTOR_X_100 320
#define STEP_SELECTOR_Y_100 OCTAVE_SELECTOR_Y_100
#define GATE_SELECTOR_X_100 410
#define GATE_SELECTOR_Y_100 OCTAVE_SELECTOR_Y_100



#define STEP_COMPONENT_WIDTH_150 41
#define STEP_COMPONENT_SPACING_150 7
#define STEP_COMPONENT_HEIGHT_150 166
#define STEP_COMPONENT_POS_X_150 69
#define STEP_COMPONENT_POS_Y_150 53

#define ON_OFF_POS_X_150 11
#define ON_OFF_POS_Y_150 8

#define ONE_SHOT_POS_X_150 6
#define ONE_SHOT_POS_Y_150 159

#define MOD_TRANSPOSE_POS_X_150 ONE_SHOT_POS_X_150
#define MOD_TRANSPOSE_POS_Y_150 110

#define OCTAVE_SELECTOR_X_150 210
#define OCTAVE_SELECTOR_Y_150 12

#define SYNC_TIME_ARP_POS_X_150 75
#define SYNC_TIME_ARP_POS_Y_150 OCTAVE_SELECTOR_Y_150

#define DIRECTION_SELECTOR_X_150 345
#define DIRECTION_SELECTOR_Y_150 OCTAVE_SELECTOR_Y_150

#define STEP_SELECTOR_X_150 480
#define STEP_SELECTOR_Y_150 OCTAVE_SELECTOR_Y_150

#define GATE_SELECTOR_X_150 615
#define GATE_SELECTOR_Y_150 OCTAVE_SELECTOR_Y_150

#define ARP_TIMER_MILLISECS 10



#define NUMBER_OF_STEPS 16

class ArpComponent    : public Component, public Timer
{
public:
    ArpComponent(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts);
    ~ArpComponent();

    void paint (Graphics&) override;
    void resized() override;

    void setGUIBig();
    void setGUISmall();

    //a timer to retrieve the data from audiothread
    void timerCallback() override;
    void setLEDActive(int p_LED);
    void setNoLEDActive();
    void setVisibleAndStartTimer(bool p_set_visible);

	void forceValueTreeOntoComponents(ValueTree p_tree);

private:
    void setNumberLEDsToShow(int p_number);
    void setModTranspose(bool p_is_mod);
    
    bool m_GUI_big = true;
    //Image m_background;


    OdinAudioProcessor &m_processor;
	AudioProcessorValueTreeState &m_value_tree;

    NumberSelectorWithText m_octave_selector;
    NumberSelectorWithText m_steps_selector;
    NumberSelectorWithText m_direction;
    NumberSelectorWithText m_gate;

    OdinButton m_on;
    OdinButton m_one_shot;
    OdinButton m_mod_transpose;

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpComponent)
};