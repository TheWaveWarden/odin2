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

#define N_KNOB_FRAMES 129

class OdinAudioProcessor;

class KnobFeels : public juce::LookAndFeel_V4 {
public:
	KnobFeels() {
	}

	void drawBubble(Graphics &g, BubbleComponent &b, const Point<float> &positionOfTip, const Rectangle<float> &body) override {
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
	enum class Type { unassigned, knob_4x4a, knob_4x4b, knob_5x5a, knob_5x5b, knob_6x6a, knob_6x6b, knob_8x8a, knob_8x8b, wheel };

	OdinKnob(Type type = Type::unassigned);

	~OdinKnob() {
		setLookAndFeel(nullptr);
	}
	void setTextValueSuffix(const String &suffix) {
		setNumDecimalPlacesToDisplay(3);
		Slider::setTextValueSuffix(suffix);
	}

	void setNumGuides(int p_num_guides) {
		m_num_guides = p_num_guides;
	}

	String getTextFromValue(double value) override;

	void paint(juce::Graphics &g) override;

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
	void drawGuides(juce::Graphics &g, bool isEnabled);

	int m_midi_learn_left_offset   = 0;
	int m_midi_learn_bottom_offset = 0;

	static OdinAudioProcessor *m_processor;
	bool m_is_vertical = true;
	int m_frames, m_width, m_height;

	KnobFeels m_knob_feels;
	Type m_type;
	int m_ui_asset_base;

	int m_inlay_x = 0;
	int m_inlay_y = 0;

	float m_center_pos_y = 0.5f;
	float m_guide_radius = 0.5f;
	int m_num_guides     = 17;
};

class DecibelKnob : public OdinKnob {
public:
	DecibelKnob(OdinKnob::Type type = OdinKnob::Type::unassigned) : OdinKnob(type) {
	}

private:
	String getTextFromValue(double value) override {
		if (value < -59.999) {
			return "-Inf dB";
		} else {
			return String((float)value, 2) + " dB";
		}
	}
};