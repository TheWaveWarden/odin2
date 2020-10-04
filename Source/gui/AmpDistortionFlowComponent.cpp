/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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

#include "AmpDistortionFlowComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

AmpDistortionFlowComponent::AmpDistortionFlowComponent(AudioProcessorValueTreeState &vts) :
    m_flow_left("flow_left", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_flow_right("flow_right", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_distortion("distortion", juce::DrawableButton::ButtonStyle::ImageRaw), m_value_tree(vts) {

	m_flow_left.setClickingTogglesState(true);
	addAndMakeVisible(m_flow_left);
	m_flow_left.setTooltip("Routes filter 2 into the amplifier");
	m_flow_left.setAlwaysOnTop(true);
	m_flow_left.setTriggeredOnMouseDown(true);
	m_flow_left.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_flow_right.setClickingTogglesState(true);
	addAndMakeVisible(m_flow_right);
	m_flow_right.setTooltip("Routes filter 1 into the amplifier.\n Note that "
	                        "filter 1 can also\nbe routed into filter 2");
	m_flow_right.setAlwaysOnTop(true);
	m_flow_right.setTriggeredOnMouseDown(true);
	m_flow_right.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	m_distortion.setClickingTogglesState(true);
	addAndMakeVisible(m_distortion);
	m_distortion.setTooltip("Enables the distortion section");
	m_distortion.setAlwaysOnTop(true);
	m_distortion.setTriggeredOnMouseDown(true);
	m_distortion.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_distortion.onClick = [&]() {
		setDistortionPanelActive(m_distortion.getToggleState());
		m_value_tree.state.getChildWithName("misc").setProperty(
		    "dist_on", m_distortion.getToggleState() ? 1.f : 0.f, nullptr);
		m_value_tree.state.getChildWithName("misc").sendPropertyChangeMessage("dist_on");
	};

	m_amp_gain.setSliderStyle(Slider::RotaryVerticalDrag);
	m_amp_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_amp_gain.setRange(AMP_GAIN_MIN, AMP_GAIN_MAX);
	m_amp_gain.setTextValueSuffix(" dB");
	m_amp_gain.setNumDecimalPlacesToDisplay(1);
	m_amp_gain.setKnobTooltip("The volume of\nthe amplifier");
	addAndMakeVisible(m_amp_gain);

	m_amp_pan.setSliderStyle(Slider::RotaryVerticalDrag);
	m_amp_pan.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_amp_pan.setRange(PAN_MIN, PAN_MAX);
	m_amp_pan.setNumDecimalPlacesToDisplay(3);
	m_amp_pan.setKnobTooltip("Pans the sound to the\nleft or the right");
	addAndMakeVisible(m_amp_pan);

	m_amp_velocity.setSliderStyle(Slider::RotaryVerticalDrag);
	m_amp_velocity.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_amp_velocity.setKnobTooltip("Modulates the amp-gain by MIDI-key velocity");
	addAndMakeVisible(m_amp_velocity);

	m_boost.setSliderStyle(Slider::RotaryVerticalDrag);
	m_boost.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_boost.setKnobTooltip("Boosts the signal to apply more damage");
	m_boost.setNumDecimalPlacesToDisplay(3);
	addAndMakeVisible(m_boost);

	m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry_wet.setKnobTooltip("The ratio of processed\nand unprocessed sound");
	addAndMakeVisible(m_dry_wet);

	m_distortion_algo.setInlay(1);
	m_distortion_algo.addItem("Clamp", 1);
	m_distortion_algo.addItem("Fold", 2);
	m_distortion_algo.addItem("Zero", 3);
	//m_distortion_algo.addItem("Sine", 4);
	//m_distortion_algo.addItem("Cube", 5);
	m_distortion_algo.setEditableText(false);
	m_distortion_algo.setSelectedId(1, dontSendNotification);
	m_distortion_algo.setColor(juce::STANDARD_DISPLAY_COLOR);
	m_distortion_algo.setTooltip("Selects the distortion algorithm to be used");
	m_distortion_algo.onChange = [&]() {
		m_value_tree.state.getChildWithName("misc").setProperty(
		    "dist_algo", m_distortion_algo.getSelectedId(), nullptr);
	};
	addAndMakeVisible(m_distortion_algo);

	m_amp_velocity_attach.reset(new OdinKnobAttachment(m_value_tree, "amp_velocity", m_amp_velocity));
	m_amp_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "amp_gain", m_amp_gain));
	m_amp_pan_attach.reset(new OdinKnobAttachment(m_value_tree, "amp_pan", m_amp_pan));
	m_dist_threshold_attach.reset(new OdinKnobAttachment(m_value_tree, "dist_boost", m_boost));
	m_dist_drywet_attach.reset(new OdinKnobAttachment(m_value_tree, "dist_drywet", m_dry_wet));

	m_fil1_to_amp_attach.reset(new OdinButtonAttachment(m_value_tree, "fil1_to_amp", m_flow_right));
	m_fil2_to_amp_attach.reset(new OdinButtonAttachment(m_value_tree, "fil2_to_amp", m_flow_left));

	// m_boost.textFromValueFunction = nullptr;
	m_boost.setNumDecimalPlacesToDisplay(3);
	m_amp_gain.setNumDecimalPlacesToDisplay(2);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);
	m_amp_velocity.setNumDecimalPlacesToDisplay(3);
	m_amp_pan.setNumDecimalPlacesToDisplay(3);

	SET_CTR_KEY(m_amp_gain);
	SET_CTR_KEY(m_amp_pan);
	SET_CTR_KEY(m_amp_velocity);
	SET_CTR_KEY(m_boost);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

AmpDistortionFlowComponent::~AmpDistortionFlowComponent() {
}

void AmpDistortionFlowComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	m_distortion_algo.setValue(m_value_tree.state.getChildWithName("misc")["dist_algo"]);
	setDistortionPanelActive((float)m_value_tree.state.getChildWithName("misc")["dist_on"] > 0.5f);
	m_distortion.setToggleState((float)m_value_tree.state.getChildWithName("misc")["dist_on"] > 0.5f, dontSendNotification);
}

void AmpDistortionFlowComponent::setDistortionPanelActive(bool p_active) {
	//DBG("SetDistortionPanelActive: " + std::to_string(p_active));
	m_distortion_algo.setColor(p_active ? DISTORTION_ON_COLOR : DARKGREY);
	m_distortion_on = p_active;
	repaint();
}


void AmpDistortionFlowComponent::setGUIBig(){
	m_GUI_big = true;

juce::Image flow_left_1 = ImageCache::getFromMemory(BinaryData::buttonleft_1_150_png, BinaryData::buttonleft_1_150_pngSize);
	juce::Image flow_left_2 = ImageCache::getFromMemory(BinaryData::buttonleft_2_150_png, BinaryData::buttonleft_2_150_pngSize);
	juce::Image flow_left_3 = ImageCache::getFromMemory(BinaryData::buttonleft_3_150_png, BinaryData::buttonleft_3_150_pngSize);
	juce::Image flow_left_4 = ImageCache::getFromMemory(BinaryData::buttonleft_4_150_png, BinaryData::buttonleft_4_150_pngSize);

	juce::DrawableImage flow_left_draw1;
	juce::DrawableImage flow_left_draw2;
	juce::DrawableImage flow_left_draw3;
	juce::DrawableImage flow_left_draw4;

	flow_left_draw1.setImage(flow_left_1);
	flow_left_draw2.setImage(flow_left_2);
	flow_left_draw3.setImage(flow_left_3);
	flow_left_draw4.setImage(flow_left_4);

	m_flow_left.setImages(&flow_left_draw2,
	                      &flow_left_draw2,
	                      &flow_left_draw1,
	                      &flow_left_draw1,
	                      &flow_left_draw4,
	                      &flow_left_draw4,
	                      &flow_left_draw3,
	                      &flow_left_draw3);
	m_flow_left.setBounds(OdinHelper::c150(FLOW_LEFT_POS_X)+3, OdinHelper::c150(FLOW_POS_Y), flow_left_1.getWidth(), flow_left_1.getHeight());

	juce::Image flow_right_1 =
	    ImageCache::getFromMemory(BinaryData::buttonright_1_150_png, BinaryData::buttonright_1_150_pngSize);
	juce::Image flow_right_2 =
	    ImageCache::getFromMemory(BinaryData::buttonright_2_150_png, BinaryData::buttonright_2_150_pngSize);
	juce::Image flow_right_3 =
	    ImageCache::getFromMemory(BinaryData::buttonright_3_150_png, BinaryData::buttonright_3_150_pngSize);
	juce::Image flow_right_4 =
	    ImageCache::getFromMemory(BinaryData::buttonright_4_150_png, BinaryData::buttonright_4_150_pngSize);

	juce::DrawableImage flow_right_draw1;
	juce::DrawableImage flow_right_draw2;
	juce::DrawableImage flow_right_draw3;
	juce::DrawableImage flow_right_draw4;

	flow_right_draw1.setImage(flow_right_1);
	flow_right_draw2.setImage(flow_right_2);
	flow_right_draw3.setImage(flow_right_3);
	flow_right_draw4.setImage(flow_right_4);

	m_flow_right.setImages(&flow_right_draw2,
	                       &flow_right_draw2,
	                       &flow_right_draw1,
	                       &flow_right_draw1,
	                       &flow_right_draw4,
	                       &flow_right_draw4,
	                       &flow_right_draw3,
	                       &flow_right_draw3);
	m_flow_right.setBounds(OdinHelper::c150(FLOW_RIGHT_POS_X), OdinHelper::c150(FLOW_POS_Y), flow_right_1.getWidth(), flow_right_1.getHeight());

	juce::Image distortion_1 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_1_150_png, BinaryData::buttondistortion_1_150_pngSize);
	juce::Image distortion_2 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_2_150_png, BinaryData::buttondistortion_2_150_pngSize);
	juce::Image distortion_3 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_3_150_png, BinaryData::buttondistortion_3_150_pngSize);
	juce::Image distortion_4 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_4_150_png, BinaryData::buttondistortion_4_150_pngSize);

	juce::DrawableImage distortion_draw1;
	juce::DrawableImage distortion_draw2;
	juce::DrawableImage distortion_draw3;
	juce::DrawableImage distortion_draw4;

	distortion_draw1.setImage(distortion_1);
	distortion_draw2.setImage(distortion_2);
	distortion_draw3.setImage(distortion_3);
	distortion_draw4.setImage(distortion_4);

	m_distortion.setImages(&distortion_draw2,
	                       &distortion_draw2,
	                       &distortion_draw1,
	                       &distortion_draw1,
	                       &distortion_draw4,
	                       &distortion_draw4,
	                       &distortion_draw3,
	                       &distortion_draw3);
	m_distortion.setBounds(OdinHelper::c150(DISTORTION_POS_X)-2, OdinHelper::c150(DISTORTION_POS_Y), distortion_1.getWidth(), distortion_1.getHeight());

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_150_png, BinaryData::metal_knob_big_150_pngSize);
	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_150_png, BinaryData::black_knob_mid_150_pngSize);
	juce::Image round_knob = ImageCache::getFromMemory(BinaryData::round_knob_150_png, BinaryData::round_knob_150_pngSize);

	m_amp_gain.setStrip(metal_knob_big, N_KNOB_FRAMES);

	m_amp_pan.setStrip(black_knob_mid, N_KNOB_FRAMES);

	m_amp_velocity.setStrip(black_knob_mid, N_KNOB_FRAMES);

	m_boost.setStrip(round_knob, N_KNOB_FRAMES);

	m_dry_wet.setStrip(round_knob, N_KNOB_FRAMES);

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_150_png, BinaryData::glaspanel_midbig_150_pngSize);
	m_distortion_algo.setImage(glas_panel);
	m_distortion_algo.setInlay(1);
	m_distortion_algo.setBounds(OdinHelper::c150(DIST_ALGO_POS_X), OdinHelper::c150(DIST_ALGO_POS_Y), glas_panel.getWidth(), glas_panel.getHeight());

	m_amp_gain.setBounds(OdinHelper::c150(AMP_GAIN_POS_X) - OdinHelper::c150(METAL_KNOB_BIG_OFFSET_X)-1,
	                     OdinHelper::c150(AMP_GAIN_POS_Y) - OdinHelper::c150(METAL_KNOB_BIG_OFFSET_Y)-1,
	                     OdinHelper::c150(METAL_KNOB_BIG_SIZE_X),
	                     OdinHelper::c150(METAL_KNOB_BIG_SIZE_Y));
	m_amp_pan.setBounds(OdinHelper::c150(AMP_PAN_POS_X) - OdinHelper::c150(BLACK_KNOB_MID_OFFSET_X),
	                    OdinHelper::c150(AMP_PAN_POS_Y) - OdinHelper::c150(BLACK_KNOB_MID_OFFSET_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_amp_velocity.setBounds(OdinHelper::c150(AMP_VEL_POS_X) - OdinHelper::c150(BLACK_KNOB_MID_OFFSET_X),
	                      OdinHelper::c150(AMP_VEL_POS_Y) - OdinHelper::c150(BLACK_KNOB_MID_OFFSET_Y)-1,
	                      OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                      OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_boost.setBounds(
	    OdinHelper::c150(BIAS_POS_X) - OdinHelper::c150(ROUND_KNOB_OFFSET_X)-1, OdinHelper::c150(BIAS_POS_Y) - OdinHelper::c150(ROUND_KNOB_OFFSET_Y)+1, OdinHelper::c150(ROUND_KNOB_SIZE_X)+2, OdinHelper::c150(ROUND_KNOB_SIZE_Y));
	m_dry_wet.setBounds(OdinHelper::c150(THRESHOLD_POS_X) - OdinHelper::c150(ROUND_KNOB_OFFSET_X)-1,
	                    OdinHelper::c150(THRESHOLD_POS_Y) - OdinHelper::c150(ROUND_KNOB_OFFSET_Y)+1,
	                    OdinHelper::c150(ROUND_KNOB_SIZE_X)+2,
	                    OdinHelper::c150(ROUND_KNOB_SIZE_Y));

	m_distortion_algo.setGUIBig();

	m_distortion_image = ImageCache::getFromMemory(BinaryData::distortion_on_150_png, BinaryData::distortion_on_150_pngSize);

	forceValueTreeOntoComponents(m_value_tree.state);
}
void AmpDistortionFlowComponent::setGUISmall(){
	m_GUI_big = false;
	
	juce::Image flow_left_1 = ImageCache::getFromMemory(BinaryData::buttonleft_1_png, BinaryData::buttonleft_1_pngSize);
	juce::Image flow_left_2 = ImageCache::getFromMemory(BinaryData::buttonleft_2_png, BinaryData::buttonleft_2_pngSize);
	juce::Image flow_left_3 = ImageCache::getFromMemory(BinaryData::buttonleft_3_png, BinaryData::buttonleft_3_pngSize);
	juce::Image flow_left_4 = ImageCache::getFromMemory(BinaryData::buttonleft_4_png, BinaryData::buttonleft_4_pngSize);

	juce::DrawableImage flow_left_draw1;
	juce::DrawableImage flow_left_draw2;
	juce::DrawableImage flow_left_draw3;
	juce::DrawableImage flow_left_draw4;

	flow_left_draw1.setImage(flow_left_1);
	flow_left_draw2.setImage(flow_left_2);
	flow_left_draw3.setImage(flow_left_3);
	flow_left_draw4.setImage(flow_left_4);

	m_flow_left.setImages(&flow_left_draw2,
	                      &flow_left_draw2,
	                      &flow_left_draw1,
	                      &flow_left_draw1,
	                      &flow_left_draw4,
	                      &flow_left_draw4,
	                      &flow_left_draw3,
	                      &flow_left_draw3);
	m_flow_left.setBounds(FLOW_LEFT_POS_X, FLOW_POS_Y, flow_left_1.getWidth(), flow_left_1.getHeight());

	juce::Image flow_right_1 =
	    ImageCache::getFromMemory(BinaryData::buttonright_1_png, BinaryData::buttonright_1_pngSize);
	juce::Image flow_right_2 =
	    ImageCache::getFromMemory(BinaryData::buttonright_2_png, BinaryData::buttonright_2_pngSize);
	juce::Image flow_right_3 =
	    ImageCache::getFromMemory(BinaryData::buttonright_3_png, BinaryData::buttonright_3_pngSize);
	juce::Image flow_right_4 =
	    ImageCache::getFromMemory(BinaryData::buttonright_4_png, BinaryData::buttonright_4_pngSize);

	juce::DrawableImage flow_right_draw1;
	juce::DrawableImage flow_right_draw2;
	juce::DrawableImage flow_right_draw3;
	juce::DrawableImage flow_right_draw4;

	flow_right_draw1.setImage(flow_right_1);
	flow_right_draw2.setImage(flow_right_2);
	flow_right_draw3.setImage(flow_right_3);
	flow_right_draw4.setImage(flow_right_4);

	m_flow_right.setImages(&flow_right_draw2,
	                       &flow_right_draw2,
	                       &flow_right_draw1,
	                       &flow_right_draw1,
	                       &flow_right_draw4,
	                       &flow_right_draw4,
	                       &flow_right_draw3,
	                       &flow_right_draw3);
	m_flow_right.setBounds(FLOW_RIGHT_POS_X, FLOW_POS_Y, flow_right_1.getWidth(), flow_right_1.getHeight());

	juce::Image distortion_1 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_1_png, BinaryData::buttondistortion_1_pngSize);
	juce::Image distortion_2 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_2_png, BinaryData::buttondistortion_2_pngSize);
	juce::Image distortion_3 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_3_png, BinaryData::buttondistortion_3_pngSize);
	juce::Image distortion_4 =
	    ImageCache::getFromMemory(BinaryData::buttondistortion_4_png, BinaryData::buttondistortion_4_pngSize);

	juce::DrawableImage distortion_draw1;
	juce::DrawableImage distortion_draw2;
	juce::DrawableImage distortion_draw3;
	juce::DrawableImage distortion_draw4;

	distortion_draw1.setImage(distortion_1);
	distortion_draw2.setImage(distortion_2);
	distortion_draw3.setImage(distortion_3);
	distortion_draw4.setImage(distortion_4);

	m_distortion.setImages(&distortion_draw2,
	                       &distortion_draw2,
	                       &distortion_draw1,
	                       &distortion_draw1,
	                       &distortion_draw4,
	                       &distortion_draw4,
	                       &distortion_draw3,
	                       &distortion_draw3);
	m_distortion.setBounds(DISTORTION_POS_X, DISTORTION_POS_Y, distortion_1.getWidth(), distortion_1.getHeight());

	juce::Image metal_knob_big =
	    ImageCache::getFromMemory(BinaryData::metal_knob_big_png, BinaryData::metal_knob_big_pngSize);
	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_png, BinaryData::black_knob_mid_pngSize);
	juce::Image round_knob = ImageCache::getFromMemory(BinaryData::round_knob_png, BinaryData::round_knob_pngSize);

	m_amp_gain.setStrip(metal_knob_big, N_KNOB_FRAMES);

	m_amp_pan.setStrip(black_knob_mid, N_KNOB_FRAMES);

	m_amp_velocity.setStrip(black_knob_mid, N_KNOB_FRAMES);

	m_boost.setStrip(round_knob, N_KNOB_FRAMES);

	m_dry_wet.setStrip(round_knob, N_KNOB_FRAMES);

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_png, BinaryData::glaspanel_midbig_pngSize);
	m_distortion_algo.setImage(glas_panel);
	m_distortion_algo.setInlay(1);
	m_distortion_algo.setBounds(DIST_ALGO_POS_X, DIST_ALGO_POS_Y, glas_panel.getWidth(), glas_panel.getHeight());

	m_amp_gain.setBounds(AMP_GAIN_POS_X - METAL_KNOB_BIG_OFFSET_X,
	                     AMP_GAIN_POS_Y - METAL_KNOB_BIG_OFFSET_Y,
	                     METAL_KNOB_BIG_SIZE_X,
	                     METAL_KNOB_BIG_SIZE_Y);
	m_amp_pan.setBounds(AMP_PAN_POS_X - BLACK_KNOB_MID_OFFSET_X,
	                    AMP_PAN_POS_Y - BLACK_KNOB_MID_OFFSET_Y,
	                    BLACK_KNOB_MID_SIZE_X,
	                    BLACK_KNOB_MID_SIZE_Y);
	m_amp_velocity.setBounds(AMP_VEL_POS_X - BLACK_KNOB_MID_OFFSET_X,
	                      AMP_VEL_POS_Y - BLACK_KNOB_MID_OFFSET_Y,
	                      BLACK_KNOB_MID_SIZE_X,
	                      BLACK_KNOB_MID_SIZE_Y);
	m_boost.setBounds(
	    BIAS_POS_X - ROUND_KNOB_OFFSET_X, BIAS_POS_Y - ROUND_KNOB_OFFSET_Y, ROUND_KNOB_SIZE_X, ROUND_KNOB_SIZE_Y);
	m_dry_wet.setBounds(THRESHOLD_POS_X - ROUND_KNOB_OFFSET_X,
	                    THRESHOLD_POS_Y - ROUND_KNOB_OFFSET_Y,
	                    ROUND_KNOB_SIZE_X,
	                    ROUND_KNOB_SIZE_Y);

	m_distortion_algo.setGUISmall();
	m_distortion_image = ImageCache::getFromMemory(BinaryData::distortion_on_png, BinaryData::distortion_on_pngSize);

	forceValueTreeOntoComponents(m_value_tree.state);
}