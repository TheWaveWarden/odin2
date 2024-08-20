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

#include "../GlobalIncludes.h"
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
	int getSliderPopupPlacement(Slider &slider) override {
		return 2;
	}
};

class OdinKnob : public juce::Slider, public OdinMidiLearnBase {
public:
	OdinKnob() {
		setLookAndFeel(&m_knob_feels);
		setRange(0, 1);

		setPopupDisplayEnabled(true, false, nullptr);
		setNumDecimalPlacesToDisplay(3);
		setVelocityModeParameters(1.0, 1, 0.0, true, ModifierKeys::shiftModifier);

		setTooltip("henlo");
	}

	~OdinKnob() {
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
		g.setColour(COL_LIGHT);
		const auto val                 = valueToProportionOfLength(getValue());
		static constexpr auto deadzone = 30.0f;
		static constexpr auto stroke   = 2.0f;
		const auto angle    = (deadzone + val * (360.0f - 2 * deadzone)) * juce::MathConstants<float>::twoPi / 360.0f;
		const auto sin      = std::sin(angle);
		const auto cos      = std::cos(angle);
		const auto center_x = getLocalBounds().getCentreX();
		const auto center_y = getLocalBounds().getCentreY();

		g.drawEllipse(getLocalBounds().toFloat().reduced(stroke / 2.0f), 1.0f);
		g.drawLine(center_x, center_y, center_x * (1.0f - sin), center_y * (1.0f + cos));
	}

	void mouseDown(const MouseEvent &event) override;

	void setKnobTooltip(const std::string &p_tooltip) {
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
	int m_frames, m_width, m_height;
	juce::Image m_filmstrip;

	KnobFeels m_knob_feels;

	//Label m_label;
};

class DecibelKnob : public OdinKnob {
	String getTextFromValue(double value) override {
		if (value < -59.999) {
			return "-Inf dB";
		} else {
			return String((float)value, 2) + " dB";
		}
	}
};