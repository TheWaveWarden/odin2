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

#define AMP_GAIN_POS_X (400 - 275)
#define AMP_GAIN_POS_Y (222 - 175)
#define AMP_PAN_POS_X (465 - 275)
#define AMP_PAN_POS_Y (212 - 175)
#define AMP_VEL_POS_X (335 - 275)
#define AMP_VEL_POS_Y (212 - 175)
#define BIAS_POS_X (330 - 275)
#define BIAS_POS_Y (286 - 175)
#define THRESHOLD_POS_X (470 - 275)
#define THRESHOLD_POS_Y (286 - 175)
#define DISTORTION_POS_X 93
#define DISTORTION_POS_Y 87

#define FLOW_RIGHT_POS_X 0
#define FLOW_LEFT_POS_X 226
#define FLOW_POS_Y 2

#define DIST_ALGO_POS_X 92
#define DIST_ALGO_POS_Y 114

#define THRESHOLD_DEFAULT 0.7
#define DRYWET_DIST_DEFAULT 1
#define AMP_GAIN_MIN -30
#define AMP_GAIN_MAX 12
#define AMP_GAIN_DEFAULT 0
#define PAN_MIN -1
#define PAN_MAX 1
#define PAN_DEFAULT 0

#define DISTORTION_OVERLAY_X 22
#define DISTORTION_OVERLAY_Y 81

//==============================================================================
/*
 */
class AmpDistortionFlowComponent : public Component {
public:
	AmpDistortionFlowComponent(AudioProcessorValueTreeState &vts);
	~AmpDistortionFlowComponent();

	void forceValueTreeOntoComponents(ValueTree p_tree);

	void paint(Graphics &g) override {
		g.setColour(COL_LIGHT);
		g.drawRect(getLocalBounds(), 1);
	}
	void resized() override;

	void setGUIBig();
	void setGUISmall();

private:
	bool m_GUI_big = true;

	juce::Image m_distortion_image;

	void setDistortionPanelActive(bool p_active);

	bool m_distortion_on = false;

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

	//std::unique_ptr<OdinButtonAttachment> m_dist_on_attach;
	std::unique_ptr<OdinButtonAttachment> m_fil1_to_amp_attach;
	std::unique_ptr<OdinButtonAttachment> m_fil2_to_amp_attach;

	std::unique_ptr<ComboBoxAttachment> m_dist_algo_attach;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpDistortionFlowComponent)
};
