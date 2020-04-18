/*
  ==============================================================================

    Knob.h
    Created: 23 Feb 2019 2:34:46pm
    Author:  frot

  ==============================================================================
*/
#pragma once

#include "GlobalIncludes.h"
#include "InputField.h"
#include "OdinMidiLearnBase.h"

#define N_KNOB_FRAMES 256

#define BLACK_KNOB_VERY_SMALL_SIZE_X 21
#define BLACK_KNOB_VERY_SMALL_SIZE_Y 24
#define BLACK_KNOB_VERY_SMALL_OFFSET_X 11
#define BLACK_KNOB_VERY_SMALL_OFFSET_Y 11

#define BLACK_KNOB_SMALL_SIZE_X 27
#define BLACK_KNOB_SMALL_SIZE_Y 30
#define BLACK_KNOB_SMALL_OFFSET_X 14
#define BLACK_KNOB_SMALL_OFFSET_Y 13

#define BLACK_KNOB_MID_SIZE_X 29
#define BLACK_KNOB_MID_SIZE_Y 32
#define BLACK_KNOB_MID_OFFSET_X 15
#define BLACK_KNOB_MID_OFFSET_Y 14

#define BLACK_KNOB_BIG_SIZE_X 32
#define BLACK_KNOB_BIG_SIZE_Y 35
#define BLACK_KNOB_BIG_OFFSET_X 17
#define BLACK_KNOB_BIG_OFFSET_Y 15

#define METAL_KNOB_SMALL_SIZE_X 33
#define METAL_KNOB_SMALL_SIZE_Y 35
#define METAL_KNOB_SMALL_OFFSET_X 18
#define METAL_KNOB_SMALL_OFFSET_Y 16

#define METAL_KNOB_MID_SIZE_X 39
#define METAL_KNOB_MID_SIZE_Y 42

#define METAL_KNOB_BIG_SIZE_X 45
#define METAL_KNOB_BIG_SIZE_Y 48
#define METAL_KNOB_BIG_OFFSET_X 25
#define METAL_KNOB_BIG_OFFSET_Y 20

#define WHEEL_SIZE_X 17

#define ROUND_KNOB_SIZE_X 36
#define ROUND_KNOB_SIZE_Y 39
#define ROUND_KNOB_OFFSET_X 19
#define ROUND_KNOB_OFFSET_Y 17

#define LABEL_OFFSET_Y -8
#define LABEL_SIZE_X 90
#define LABEL_SIZE_Y 40

#define BLACK_KNOB_VERY_SMALL_LEFT_OFFSET 1
#define BLACK_KNOB_SMALL_LEFT_OFFSET 2
#define BLACK_KNOB_MID_LEFT_OFFSET 1
#define BLACK_KNOB_BIG_LEFT_OFFSET 2
#define METAL_KNOB_SMALL_LEFT_OFFSET 2
#define METAL_KNOB_MID_LEFT_OFFSET 2
#define METAL_KNOB_BIG_LEFT_OFFSET 4
#define ROUND_KNOB_LEFT_OFFSET 2
#define WHEEL_LEFT_OFFSET 3

#define BLACK_KNOB_VERY_SMALL_BOTTOM_OFFSET 1
#define BLACK_KNOB_SMALL_BOTTOM_OFFSET 3
#define BLACK_KNOB_MID_BOTTOM_OFFSET 3
#define BLACK_KNOB_BIG_BOTTOM_OFFSET 4
#define METAL_KNOB_SMALL_BOTTOM_OFFSET 2
#define METAL_KNOB_MID_BOTTOM_OFFSET 5
#define METAL_KNOB_BIG_BOTTOM_OFFSET 6
#define ROUND_KNOB_BOTTOM_OFFSET 4

class OdinAudioProcessor;

class KnobFeels : public juce::LookAndFeel_V4 {
public:
	KnobFeels() {
	}

	void drawBubble(Graphics &g,
	                BubbleComponent &b,
	                const Point<float> &positionOfTip,
	                const Rectangle<float> &body) override {
		g.setColour(MENU_BACKGROUND_COLOR);
		g.fillRect(body); // pmai
		g.setColour(Colour(50, 50, 50));
		g.drawRect(body); // pmai
		g.setFont(30.f);
	}
	int getSliderPopupPlacement(Slider &slider) {
		return 2;
	}
};

class Knob : public juce::Slider, public OdinMidiLearnBase {
public:
	Knob() {
		setLookAndFeel(&m_knob_feels);
		setRange(0, 1);

		setPopupDisplayEnabled(true, false, nullptr);
		setNumDecimalPlacesToDisplay(3);
		setVelocityModeParameters(1.0, 1, 0.0, true, ModifierKeys::shiftModifier);

		setTooltip("henlo");
	}

	~Knob() {
		setLookAndFeel(nullptr);
	}
	void setTextValueSuffix(const String &suffix) {
		setNumDecimalPlacesToDisplay(3);
		Slider::setTextValueSuffix(suffix);
	}

	String getTextFromValue(double value) override;

	void setStrip(juce::Image p_strip, size_t p_frames, bool p_is_vertical = true) {
		m_is_vertical = p_is_vertical;
		m_filmstrip   = p_strip;
		//DBG(p_strip.getHeight());
		//DBG(p_frames);
		//DBG("--");
		if (m_is_vertical) {
			m_width  = p_strip.getWidth();
			m_height = p_strip.getHeight() / p_frames;
		} else {
			m_width  = p_strip.getWidth() / p_frames;
			m_height = p_strip.getHeight();
		}
		m_frames = p_frames;

		// m_drag_label.setVisible(false);
	}

	void paint(juce::Graphics &g) override {
		SET_INTERPOLATION_QUALITY(g)

		std::size_t image_number = static_cast<std::size_t>(
		    // 0.5 + (getValue() - getMinimum()) / (getMaximum() - getMinimum()) *
		    valueToProportionOfLength(getValue()) * (m_frames - 1));
		if (m_is_vertical) {
			g.drawImage(m_filmstrip, 0, 0, m_width, m_height, 0, image_number * m_height, m_width, m_height);
		} else {
			g.drawImage(m_filmstrip, 0, 0, m_width, m_height, image_number * m_width, 0, m_width, m_height);
		}

		if (m_midi_learn) {
			g.setColour(Colours::red);
			g.drawRoundedRectangle(getLocalBounds().getX() + m_midi_learn_left_offset,
			                       getLocalBounds().getY(),
			                       getLocalBounds().getWidth() - m_midi_learn_left_offset,
			                       getLocalBounds().getHeight() - m_midi_learn_bottom_offset,
			                       5,
			                       2); // draw an outline around the component
		} else if (m_midi_control) {
			g.setColour(Colours::green);
			g.drawRoundedRectangle(getLocalBounds().getX() + m_midi_learn_left_offset,
			                       getLocalBounds().getY(),
			                       getLocalBounds().getWidth() - m_midi_learn_left_offset,
			                       getLocalBounds().getHeight() - m_midi_learn_bottom_offset,
			                       5,
			                       2); // draw an outline around the component
		}
	}

	// this does not override, but hide (hides other variants as well)
	void setBounds(int x, int y, int width, int height) {
		switch (width) {
		case ROUND_KNOB_SIZE_X:
			m_midi_learn_left_offset   = ROUND_KNOB_LEFT_OFFSET;
			m_midi_learn_bottom_offset = ROUND_KNOB_BOTTOM_OFFSET;
			break;
		case METAL_KNOB_SMALL_SIZE_X:
			m_midi_learn_left_offset   = METAL_KNOB_SMALL_LEFT_OFFSET;
			m_midi_learn_bottom_offset = METAL_KNOB_SMALL_BOTTOM_OFFSET;
			break;
		case METAL_KNOB_MID_SIZE_X:
			m_midi_learn_left_offset   = METAL_KNOB_MID_LEFT_OFFSET;
			m_midi_learn_bottom_offset = METAL_KNOB_MID_BOTTOM_OFFSET;
			break;
		case METAL_KNOB_BIG_SIZE_X:
			m_midi_learn_left_offset   = METAL_KNOB_BIG_LEFT_OFFSET;
			m_midi_learn_bottom_offset = METAL_KNOB_BIG_BOTTOM_OFFSET;
			break;
		case BLACK_KNOB_VERY_SMALL_SIZE_X:
			m_midi_learn_left_offset   = BLACK_KNOB_VERY_SMALL_LEFT_OFFSET;
			m_midi_learn_bottom_offset = BLACK_KNOB_VERY_SMALL_BOTTOM_OFFSET;
			break;
		case BLACK_KNOB_SMALL_SIZE_X:
			m_midi_learn_left_offset   = BLACK_KNOB_SMALL_LEFT_OFFSET;
			m_midi_learn_bottom_offset = BLACK_KNOB_SMALL_BOTTOM_OFFSET;
			break;
		case BLACK_KNOB_MID_SIZE_X:
			m_midi_learn_left_offset   = BLACK_KNOB_MID_LEFT_OFFSET;
			m_midi_learn_bottom_offset = BLACK_KNOB_MID_BOTTOM_OFFSET;
			break;
		case BLACK_KNOB_BIG_SIZE_X:
			m_midi_learn_left_offset   = BLACK_KNOB_BIG_LEFT_OFFSET;
			m_midi_learn_bottom_offset = BLACK_KNOB_BIG_BOTTOM_OFFSET;
			break;
		case WHEEL_SIZE_X:
			m_midi_learn_left_offset = WHEEL_LEFT_OFFSET;
			break;
		default:
			DBG("Unknown knob width " + std::to_string(width) + "px in Knob::setBounds()");
			break;
		}
		Slider::setBounds(x, y, width, height);
	}

	void mouseDown(const MouseEvent &event) override;

	void setKnobTooltip(const std::string p_tooltip) {
		setTooltip(p_tooltip);
	}

	static void setOdinPointer(OdinAudioProcessor *p_pointer) {
		m_processor = p_pointer;
	}

	void stopMidiLearn() override {
		m_midi_learn = false;
		//todo replace lock by async MM call
		//const MessageManagerLock mmLock;
		repaint();
	}

	void setMidiControlActive() override {
		m_midi_learn   = false;
		m_midi_control = true;
		//todo replace lock by async MM call
		//const MessageManagerLock mmLock;
		repaint();
	}

	void mouseDoubleClick(const MouseEvent &e) override {
		if (!e.mods.isRightButtonDown()) {
			if (auto editor = findParentComponentOfClass<juce::AudioProcessorEditor>()) {
				if (auto value_field = dynamic_cast<InputField *>(editor->findChildWithID("value_input"))) {

					value_field->setVisible(true);
					Point<int> point_in_parent(getX() + getWidth() / 2 - INPUT_LABEL_SIZE_X / 2, getBottom() + 10);
					Point<int> point_in_editor = editor->getLocalPoint(getParentComponent(), point_in_parent);
					if (point_in_editor.getY() > 580) {
						point_in_editor -= Point<int>(0, 30);
					}

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
	int m_midi_learn_left_offset   = 0;
	int m_midi_learn_bottom_offset = 0;

	static OdinAudioProcessor *m_processor;
	bool m_is_vertical = true;
	std::size_t m_frames, m_width, m_height;
	juce::Image m_filmstrip;

	KnobFeels m_knob_feels;

	//Label m_label;
};