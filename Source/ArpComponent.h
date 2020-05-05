#pragma once

#include <JuceHeader.h>
#include "StepComponent.h"
#include "PluginProcessor.h"
#include "OdinArpeggiator.h"
#include "NumberSelectorWithText.h"
#include "SyncTimeSelector.h"
#include "OdinControlAttachments.h"
#include "OdinButton.h"

#define STEP_COMPONENT_WIDTH 41
#define STEP_COMPONENT_SPACING 7
#define STEP_COMPONENT_HEIGHT 156
#define STEP_COMPONENT_POS_X 69
#define STEP_COMPONENT_POS_Y 63

#define ON_OFF_POS_X 16
#define ON_OFF_POS_Y 13

#define ONE_SHOT_POS_X 2
#define ONE_SHOT_POS_Y 162

#define SYNC_TIME_ARP_POS_X 75
#define SYNC_TIME_ARP_POS_Y 20

#define OCTAVE_SELECTOR_X 210
#define OCTAVE_SELECTOR_Y 20

#define DIRECTION_SELECTOR_X 345
#define DIRECTION_SELECTOR_Y OCTAVE_SELECTOR_Y

#define STEP_SELECTOR_X 480
#define STEP_SELECTOR_Y OCTAVE_SELECTOR_Y

#define GATE_SELECTOR_X 615
#define GATE_SELECTOR_Y OCTAVE_SELECTOR_Y



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

private:
    bool m_GUI_big = false;

    void setNumberLEDsToShow(int p_number);

    OdinAudioProcessor &m_processor;
	AudioProcessorValueTreeState &m_value_tree;

    NumberSelectorWithText m_octave_selector;
    NumberSelectorWithText m_steps_selector;
    NumberSelectorWithText m_direction;
    NumberSelectorWithText m_gate;

    OdinButton m_on;
    OdinButton m_one_shot;

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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpComponent)
};
