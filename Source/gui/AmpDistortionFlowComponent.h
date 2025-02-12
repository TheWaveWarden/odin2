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
#include "GlassDropdown.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"
#include "OdinKnob.h"
#include "TextLabel.h"

#define THRESHOLD_DEFAULT 0.7
#define DRYWET_DIST_DEFAULT 1
#define AMP_GAIN_MIN -30
#define AMP_GAIN_MAX 12
#define AMP_GAIN_DEFAULT 0
#define PAN_MIN -1
#define PAN_MAX 1
#define PAN_DEFAULT 0

//==============================================================================
/*
 */
class AmpDistortionFlowComponent : public Component {
public:
	AmpDistortionFlowComponent(AudioProcessorValueTreeState &vts);
	~AmpDistortionFlowComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void paint(Graphics &g) override;
	void resized() override;

private:
	juce::Image m_distortion_image;

	void setDistortionPanelActive(bool p_active);

	bool m_distortion_on = false;

	TextLabel m_gain_label;
	TextLabel m_pan_label;
	TextLabel m_velocity_label;
	TextLabel m_boost_label;
	TextLabel m_drywet_label;
	TextLabel m_distortion_label;

	OdinKnob m_amp_gain;
	OdinKnob m_amp_pan;
	OdinKnob m_amp_velocity;
	OdinKnob m_boost;
	OdinKnob m_dry_wet;

	OdinButton m_flow_left;
	OdinButton m_flow_right;
	OdinButton m_distortion;

	GlassDropdown m_distortion_algo;

	AudioProcessorValueTreeState &m_value_tree;

	std::unique_ptr<OdinKnobAttachment> m_amp_pan_attach;
	std::unique_ptr<OdinKnobAttachment> m_amp_gain_attach;
	std::unique_ptr<OdinKnobAttachment> m_amp_velocity_attach;
	std::unique_ptr<OdinKnobAttachment> m_dist_threshold_attach;
	std::unique_ptr<OdinKnobAttachment> m_dist_drywet_attach;

	std::unique_ptr<OdinButtonAttachment> m_fil1_to_amp_attach;
	std::unique_ptr<OdinButtonAttachment> m_fil2_to_amp_attach;

	std::unique_ptr<ComboBoxAttachment> m_dist_algo_attach;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpDistortionFlowComponent)
};
