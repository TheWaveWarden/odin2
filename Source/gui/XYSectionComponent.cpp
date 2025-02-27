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

#include "XYSectionComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"

XYSectionComponent::XYSectionComponent(AudioProcessorValueTreeState &vts, const std::string &p_section_name) :
    m_value_tree(vts),
    m_section_name(p_section_name),
    m_xy_pad(vts, "xy_", m_modx, m_mody),
    m_label_x("X"),
    m_label_y("Y"),
    m_modx(OdinKnob::Type::knob_6x6a),
    m_mody(OdinKnob::Type::knob_6x6a) {

	addAndMakeVisible(m_label_x);
	addAndMakeVisible(m_label_y);

	m_xy_pad.setInlay(1);
	m_xy_pad.setTooltip("An XY pad to be used as a modulation source in the modmatrix.");
	addAndMakeVisible(m_xy_pad);

	m_modx.setSliderStyle(Slider::RotaryVerticalDrag);
	m_modx.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_modx.setRange(0, 1);
	m_modx.onValueChange = [&] { m_xy_pad.setX(m_modx.getValue()); };
	m_modx.setTooltip("The X coordinate\nof the XY pad");
	addAndMakeVisible(m_modx);

	m_mody.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mody.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mody.setRange(0, 1);
	m_mody.onValueChange = [&] { m_xy_pad.setY(m_mody.getValue()); };
	m_mody.setTooltip("The Y coordinate\nof the XY pad");
	addAndMakeVisible(m_mody);

	m_x_attach.reset(new OdinKnobAttachment(m_value_tree, "xy_x", m_modx));
	m_y_attach.reset(new OdinKnobAttachment(m_value_tree, "xy_y", m_mody));

	SET_CTR_KEY(m_modx);
	SET_CTR_KEY(m_mody);

	m_modx.setNumDecimalPlacesToDisplay(3);
	m_mody.setNumDecimalPlacesToDisplay(3);
}

XYSectionComponent::~XYSectionComponent() {
}

void XYSectionComponent::paint(Graphics &g) {
}

void XYSectionComponent::resized() {
	GET_LOCAL_AREA(m_modx, "Modx");
	GET_LOCAL_AREA(m_mody, "Mody");
	GET_LOCAL_AREA(m_xy_pad, "XyPad");

	GET_LOCAL_AREA(m_label_x, "LabelX");
	GET_LOCAL_AREA(m_label_y, "LabelY");
}