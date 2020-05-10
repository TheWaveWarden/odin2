#pragma once

#include "Knob.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "StepLED.h"
#include <JuceHeader.h>

#define KNOB_POS_X 1
#define MOD_POS_Y 3
#define TRANSPOSE_POS_Y 51
#define STEP_ON_X 3
#define STEP_ON_Y 102
#define STEP_LED_POS_X 13
#define STEP_LED_POS_Y 142
#define KNOB_GUIDE_X 0
#define KNOB_GUIDE_1_Y 0
#define KNOB_GUIDE_2_Y 47

class StepComponent : public Component {
public:
	StepComponent(AudioProcessorValueTreeState &vts, int p_step_index);
	~StepComponent();

	void paint(Graphics &) override;
	void resized() override;

	void setGUIBig();
	void setGUISmall();

	void setLEDActive(int p_step_index);
	void setShowLED(int p_highest_led);

private:
	bool m_GUI_big = false;

	Image m_knob_guide;

	AudioProcessorValueTreeState &m_value_tree;

	Knob m_mod_1;
	Knob m_mod_2;
	OdinButton m_step_on;
	StepLED m_led;
	
	std::unique_ptr<OdinButtonAttachment> m_step_on_attach;
	std::unique_ptr<OdinKnobAttachment> m_mod_attach;
	std::unique_ptr<OdinKnobAttachment> m_transpose_attach;

    int m_step_index;
	bool m_show_led = true;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepComponent)
};
