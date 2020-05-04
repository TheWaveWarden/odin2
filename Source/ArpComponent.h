#pragma once

#include <JuceHeader.h>
#include "StepComponent.h"

#define STEP_COMPONENT_WIDTH 42
#define STEP_COMPONENT_HEIGHT 135
#define STEP_COMPONENT_POS_X 69
#define STEP_COMPONENT_POS_Y 78

#define NUMBER_OF_STEPS 16

class ArpComponent    : public Component
{
public:
    ArpComponent();
    ~ArpComponent();

    void paint (Graphics&) override;
    void resized() override;
    void setGUIBig();
    void setGUISmall();

private:
    bool m_GUI_big = false;

    StepComponent m_steps[NUMBER_OF_STEPS];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArpComponent)
};
