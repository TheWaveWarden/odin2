#include "StepComponent.h"
#include <JuceHeader.h>

StepComponent::StepComponent(AudioProcessorValueTreeState &vts, int p_step_index) :
    m_value_tree(vts),
    m_step_on("step_" + std::to_string(p_step_index + 1) + "_on", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_step_index(p_step_index) {

	addAndMakeVisible(m_mod_1);
	m_mod_1.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mod_1.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mod_1.setKnobTooltip("TODO");
	m_mod_attach.reset(
	    new OdinKnobAttachment(m_value_tree, "step_" + std::to_string(m_step_index) + "_mod_1", m_mod_1));

	addAndMakeVisible(m_mod_2);
	m_mod_2.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mod_2.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mod_2.setKnobTooltip("TODO");
	m_transpose_attach.reset(
	    new OdinKnobAttachment(m_value_tree, "step_" + std::to_string(m_step_index) + "_mod_2", m_mod_2));

	addAndMakeVisible(m_step_on);
	m_step_on.setClickingTogglesState(true);
	m_step_on.setTooltip("TODO");
	m_step_on.setTriggeredOnMouseDown(true);
	m_step_on.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_step_on_attach.reset(
	    new OdinButtonAttachment(m_value_tree, "step_" + std::to_string(m_step_index) + "_on", m_step_on));

	addAndMakeVisible(m_led);
	
	m_mod_1.setNumDecimalPlacesToDisplay(3);
	m_mod_2.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_mod_2);
	SET_CTR_KEY(m_mod_1);
}

StepComponent::~StepComponent() {
}

void StepComponent::paint(Graphics &g) {
	if (m_GUI_big) {
		g.drawImageAt(m_knob_guide, KNOB_GUIDE_X_150, KNOB_GUIDE_1_Y_150);
		g.drawImageAt(m_knob_guide, KNOB_GUIDE_X_150, KNOB_GUIDE_2_Y_150);
	} else {
		g.drawImageAt(m_knob_guide, KNOB_GUIDE_X_100, KNOB_GUIDE_1_Y_100);
		g.drawImageAt(m_knob_guide, KNOB_GUIDE_X_100, KNOB_GUIDE_2_Y_100);
	}
}

void StepComponent::resized() {
}

void StepComponent::setGUIBig() {
	m_GUI_big = true;

	m_led.setGUIBig();
	m_led.setTopLeftPosition(STEP_LED_POS_X_150, STEP_LED_POS_Y_150);

	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_small_150_png, BinaryData::black_knob_small_150_pngSize);
	m_mod_1.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_mod_1.setBounds(KNOB_POS_X_150, MOD_POS_Y_150, BLACK_KNOB_SMALL_SIZE_X * 1.5, BLACK_KNOB_SMALL_SIZE_Y * 1.5);

	m_mod_2.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_mod_2.setBounds(KNOB_POS_X_150, TRANSPOSE_POS_Y_150, BLACK_KNOB_SMALL_SIZE_X * 1.5, BLACK_KNOB_SMALL_SIZE_Y * 1.5);

	juce::Image step_on_4 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_1_150_png, BinaryData::button_arp_step_1_150_pngSize);
	juce::Image step_on_2 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_2_150_png, BinaryData::button_arp_step_2_150_pngSize);
	juce::Image step_on_3 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_3_150_png, BinaryData::button_arp_step_3_150_pngSize);
	juce::Image step_on_1 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_4_150_png, BinaryData::button_arp_step_4_150_pngSize);

	juce::DrawableImage step_on_draw1;
	juce::DrawableImage step_on_draw2;
	juce::DrawableImage step_on_draw3;
	juce::DrawableImage step_on_draw4;

	step_on_draw1.setImage(step_on_1);
	step_on_draw2.setImage(step_on_2);
	step_on_draw3.setImage(step_on_3);
	step_on_draw4.setImage(step_on_4);

	m_step_on.setImages(&step_on_draw2,
	                    &step_on_draw2,
	                    &step_on_draw1,
	                    &step_on_draw1,
	                    &step_on_draw4,
	                    &step_on_draw4,
	                    &step_on_draw3,
	                    &step_on_draw3);
	m_step_on.setBounds(STEP_ON_X_150, STEP_ON_Y_150, step_on_1.getWidth(), step_on_1.getHeight());

	m_knob_guide =
	    ImageCache::getFromMemory(BinaryData::arp_knob_guide_150_png, BinaryData::arp_knob_guide_150_pngSize);
}

void StepComponent::setGUISmall() {
	m_GUI_big = false;

	m_led.setGUISmall();
	m_led.setTopLeftPosition(STEP_LED_POS_X_100, STEP_LED_POS_Y_100);

	juce::Image black_knob_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_small_png, BinaryData::black_knob_small_pngSize);
	m_mod_1.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_mod_1.setBounds(KNOB_POS_X_100, MOD_POS_Y_100, BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);

	m_mod_2.setStrip(black_knob_small, N_KNOB_FRAMES);
	m_mod_2.setBounds(KNOB_POS_X_100, TRANSPOSE_POS_Y_100, BLACK_KNOB_SMALL_SIZE_X, BLACK_KNOB_SMALL_SIZE_Y);

	juce::Image step_on_4 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_1_png, BinaryData::button_arp_step_1_pngSize);
	juce::Image step_on_2 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_2_png, BinaryData::button_arp_step_2_pngSize);
	juce::Image step_on_3 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_3_png, BinaryData::button_arp_step_3_pngSize);
	juce::Image step_on_1 =
	    ImageCache::getFromMemory(BinaryData::button_arp_step_4_png, BinaryData::button_arp_step_4_pngSize);

	juce::DrawableImage step_on_draw1;
	juce::DrawableImage step_on_draw2;
	juce::DrawableImage step_on_draw3;
	juce::DrawableImage step_on_draw4;

	step_on_draw1.setImage(step_on_1);
	step_on_draw2.setImage(step_on_2);
	step_on_draw3.setImage(step_on_3);
	step_on_draw4.setImage(step_on_4);

	m_step_on.setImages(&step_on_draw2,
	                    &step_on_draw2,
	                    &step_on_draw1,
	                    &step_on_draw1,
	                    &step_on_draw4,
	                    &step_on_draw4,
	                    &step_on_draw3,
	                    &step_on_draw3);
	m_step_on.setBounds(STEP_ON_X_100, STEP_ON_Y_100, step_on_1.getWidth(), step_on_1.getHeight());

	m_knob_guide =
	    ImageCache::getFromMemory(BinaryData::arp_knob_guide_png, BinaryData::arp_knob_guide_pngSize);
}

void StepComponent::setLEDActive(int p_step_index) {
	m_led.setLEDOn(p_step_index == m_step_index);
}

void StepComponent::setShowLED(int p_highest_led) {
	if ((p_highest_led > m_step_index) != m_show_led) {
		m_show_led = (p_highest_led > m_step_index);
		m_led.setVisible(m_show_led);
	}
}
