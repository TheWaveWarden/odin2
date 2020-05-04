/*
  ==============================================================================

    StepComponent.h
    Created: 4 May 2020 1:23:41am
    Author:  frederik_siepe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Knob.h"

#define KNOB_POS_X 1
#define MOD_POS_Y 1
#define TRANSPOSE_POS_Y 43

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepComponent)
};
