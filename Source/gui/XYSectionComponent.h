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
#include "OdinControlAttachments.h"
#include "OdinKnob.h"
#include "TextLabel.h"
#include "XYPadComponent.h"

class XYSectionComponent : public Component {
public:
	XYSectionComponent(AudioProcessorValueTreeState &vts, const std::string &p_section_name);
	~XYSectionComponent();

	void paint(Graphics &) override;
	void resized() override;

private:
	std::string m_section_name;
	AudioProcessorValueTreeState &m_value_tree;

	OdinKnob m_modx;
	OdinKnob m_mody;

	TextLabel m_label_x;
	TextLabel m_label_y;

	std::unique_ptr<OdinKnobAttachment> m_x_attach;
	std::unique_ptr<OdinKnobAttachment> m_y_attach;

	XYPadComponent m_xy_pad;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XYSectionComponent)
};
