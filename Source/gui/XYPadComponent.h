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
#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinKnob.h"

#define VECTOR_UP 1
#define VECTOR_DOWN 70
#define VECTOR_LEFT 4
#define VECTOR_RIGHT 87
class XYPadComponent : public Component, public SettableTooltipClient {
public:
	XYPadComponent(AudioProcessorValueTreeState &vts,
	               const std::string &p_param_prefix,
	               OdinKnob &p_x,
	               OdinKnob &p_y,
	               bool p_vector_pad = false);
	~XYPadComponent();

	void paint(Graphics &) override;
	void mouseDrag(const MouseEvent &event) override;
	void mouseDown(const MouseEvent &event) override;
	void mouseUp(const MouseEvent &event) override;
	void mouseInteraction();

	void setImage(juce::Image p_panel);
	void setInlay(int p_inlay);
	void setX(float p_x);
	void setY(float p_y);
	void setColor(juce::Colour p_color);
	void setLogoImage(juce::Image p_image);

private:
	AudioProcessorValueTreeState &m_value_tree;
	const std::string m_param_name_x;
	const std::string m_param_name_y;

	bool m_draw_logo = false;

	//this resolves the issue where the pad updates the knob, which updates the pad again
	bool m_lock_set_XY_while_drawing = false;

	float m_value_x = 0.f;
	float m_value_y = 0.f;

	int m_inlay;
	juce::Image m_panel;
	juce::Image m_logo;

	OdinKnob &m_knob_x;
	OdinKnob &m_knob_y;

	bool m_vector_pad;

	juce::Colour m_color;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYPadComponent)
};
