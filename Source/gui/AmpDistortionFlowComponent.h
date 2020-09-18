#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlasDropdown.h"
#include "../GlobalIncludes.h"
#include "Knob.h"
#include "OdinButton.h"
#include "OdinControlAttachments.h"

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
		SET_INTERPOLATION_QUALITY(g)
		if (m_GUI_big) {
			if (m_distortion_on) {
				g.drawImageAt(
				    m_distortion_image, OdinHelper::c150(DISTORTION_OVERLAY_X), OdinHelper::c150(DISTORTION_OVERLAY_Y));
			}
		} else {
			if (m_distortion_on) {
				g.drawImageAt(m_distortion_image, DISTORTION_OVERLAY_X, DISTORTION_OVERLAY_Y);
			}
		}
	}

	void setGUIBig();
	void setGUISmall();

private:
	bool m_GUI_big = false;

	juce::Image m_distortion_image;

	void setDistortionPanelActive(bool p_active);

	bool m_distortion_on = false;

	Knob m_amp_gain;
	Knob m_amp_pan;
	Knob m_amp_velocity;
	Knob m_boost;
	Knob m_dry_wet;

	OdinButton m_flow_left;
	OdinButton m_flow_right;
	OdinButton m_distortion;

	GlasDropdown m_distortion_algo;

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
