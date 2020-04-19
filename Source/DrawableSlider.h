/*
  ==============================================================================

    DrawableSlider.h
    Created: 26 Feb 2019 9:46:41pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include "InputField.h"
#include "OdinMidiLearnBase.h"

//==============================================================================
/*
 */

#include "../JuceLibraryCode/JuceHeader.h"

#define ADSR_LABEL_OFFSET_Y -8

class ADSRFeels : public juce::LookAndFeel_V4 {
public:
	void drawBubble(Graphics &g,
	                BubbleComponent &b,
	                const Point<float> &positionOfTip,
	                const Rectangle<float> &body) override {
		g.setColour(MENU_BACKGROUND_COLOR);
		g.fillRect(body); // pmai
		g.setColour(Colour(50, 50, 50));
		g.drawRect(body); // pmai
	}
	int getSliderPopupPlacement(Slider &slider) {
		return 2;
	}
	/*
    drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos, const
    Slider::SliderStyle, Slider &) {

                          }*/
};

class OdinAudioProcessor;

class DrawableSlider : public juce::Slider, public OdinMidiLearnBase {
public:
	DrawableSlider();
	~DrawableSlider();


	void setHandle(juce::Image p_handle){
		m_handle = p_handle;
	}
	
	String getTextFromValue(double value) override;

	void setTextValueSuffix(const String &suffix) {
		//setNumDecimalPlacesToDisplay(20);
		Slider::setTextValueSuffix(suffix);
	}

	void paint(Graphics &) override;

	void setImage(juce::Image p_handle) {
		m_handle = p_handle;
	}

	static void setOdinPointer(OdinAudioProcessor *p_pointer) {
		m_processor = p_pointer;
	}

	void stopMidiLearn() override {
		m_midi_learn = false;
		repaint();
	}

	void setMidiControlActive() override {
		m_midi_learn   = false;
		m_midi_control = true;
		repaint();
	}

	void mouseDown(const MouseEvent &event) override;

	void mouseDoubleClick(const MouseEvent &e) override {
		if (!e.mods.isRightButtonDown()) {
			if (auto editor = findParentComponentOfClass<juce::AudioProcessorEditor>()) {
				if (auto value_field = dynamic_cast<InputField *>(editor->findChildWithID("value_input"))) {

					value_field->setVisible(true);
					Point<int> point_in_parent(getX() + getWidth() / 2 - INPUT_LABEL_SIZE_X / 2, getBottom() + 10);
					Point<int> point_in_editor = editor->getLocalPoint(getParentComponent(), point_in_parent);

					value_field->setTopLeftPosition(point_in_editor);
					value_field->clear();
					value_field->grabKeyboardFocus();
					value_field->setAttachedSlider(this);
				}
			}
		}
		Component::mouseDoubleClick(e);
	}

private:
	static OdinAudioProcessor *m_processor;
	ADSRFeels m_feels;
	juce::Image m_handle;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableSlider)
};
