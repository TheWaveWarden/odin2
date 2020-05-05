#pragma once

#include <JuceHeader.h>
#include "StepComponent.h"
#include "PluginProcessor.h"

#define STEP_COMPONENT_WIDTH 41
#define STEP_COMPONENT_SPACING 7
#define STEP_COMPONENT_HEIGHT 156
#define STEP_COMPONENT_POS_X 69
#define STEP_COMPONENT_POS_Y 63

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

    OdinAudioProcessor &m_processor;
	AudioProcessorValueTreeState &m_value_tree;

    //Knobs
    //Gate Length
    //Octave


    //Buttons
    //OneShot
    //On / Off

    //SyntimSelectorz & Dropdowns
    //Time
    //Direction
    //N° of steps


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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpComponent)
};
