#pragma once

#include <JuceHeader.h>
#include "Knob.h"
#include "StepLED.h"
#include "OdinButton.h"

#define KNOB_POS_X 1
#define MOD_POS_Y 1
#define TRANSPOSE_POS_Y 43
#define STEP_ON_X 3
#define STEP_ON_Y 90
#define STEP_LED_POS_X 9
#define STEP_LED_POS_Y 110


class StepComponent    : public Component
{
public:
    StepComponent();
    ~StepComponent();

    void paint (Graphics&) override;
    void resized() override;

    void setGUIBig();
    void setGUISmall();
private:

	  Knob m_mod;
	  Knob m_transpose;
    OdinButton m_step_on;
    StepLED m_led;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepComponent)
};
