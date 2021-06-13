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

#include "ReverbComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

ReverbComponent::ReverbComponent(AudioProcessorValueTreeState &vts, bool p_is_standalone) :
    m_value_tree(vts), m_is_standalone_plugin(p_is_standalone) {

	m_reverb_delay_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_delay", m_delay));
	m_reverb_roomsize_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_roomsize", m_roomsize));
	m_reverb_diffusion_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_diffusion", m_diffusion));
	m_reverb_decay_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_decaytime", m_decay));
	m_reverb_buildup_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_buildup", m_buildup));
	m_reverb_EQ_gain_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqgain", m_EQ_gain));
	m_reverb_EQ_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_eqfreq", m_EQ_freq));
	m_reverb_ducking_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_ducking", m_ducking));
	m_reverb_drywet_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_drywet", m_dry_wet));
	m_reverb_type_attach.reset(new ComboBoxAttachment(m_value_tree, "rev_type", m_module));

	m_reverb_low_freq_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_low_freq", m_low_freq));
	m_reverb_low_hall_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_low_hall", m_low_hall));
	m_reverb_mid_hall_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_mid_hall", m_mid_hall));
	m_reverb_hf_damp_attach.reset(new OdinKnobAttachment(m_value_tree, "rev_hf_damp", m_hf_damp));

	m_delay.setSliderStyle(Slider::RotaryVerticalDrag);
	m_delay.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_delay.setTextValueSuffix(" ms");
	m_delay.setKnobTooltip("m_delay");
	addAndMakeVisible(m_delay);

	m_low_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_low_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_low_freq.setTextValueSuffix(" Hz");
	m_low_freq.setKnobTooltip("TODO LOW_FREQ");
	m_low_freq.setNumDecimalPlacesToDisplay(0);
	addAndMakeVisible(m_low_freq);

	m_low_hall.setSliderStyle(Slider::RotaryVerticalDrag);
	m_low_hall.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_low_hall.setTextValueSuffix(" s");
	m_low_hall.setKnobTooltip("TODO LOW HALL");
	m_low_hall.setNumDecimalPlacesToDisplay(2);
	addAndMakeVisible(m_low_hall);

	m_mid_hall.setSliderStyle(Slider::RotaryVerticalDrag);
	m_mid_hall.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_mid_hall.setTextValueSuffix(" s");
	m_mid_hall.setKnobTooltip("TODO MID HALL");
	m_mid_hall.setNumDecimalPlacesToDisplay(2);
	addAndMakeVisible(m_mid_hall);

	m_hf_damp.setSliderStyle(Slider::RotaryVerticalDrag);
	m_hf_damp.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_hf_damp.setTextValueSuffix(" Hz");
	m_hf_damp.setKnobTooltip("TODO HF DAMP");
	m_hf_damp.setNumDecimalPlacesToDisplay(0);
	addAndMakeVisible(m_hf_damp);

	m_roomsize.setSliderStyle(Slider::RotaryVerticalDrag);
	m_roomsize.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	//m_roomsize.setTextValueSuffix("");
	m_roomsize.setKnobTooltip("m_roomsize");
	addAndMakeVisible(m_roomsize);

	m_diffusion.setSliderStyle(Slider::RotaryVerticalDrag);
	m_diffusion.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	//m_diffusion.setTextValueSuffix("");
	m_diffusion.setKnobTooltip("m_diffusion");
	addAndMakeVisible(m_diffusion);

	m_decay.setSliderStyle(Slider::RotaryVerticalDrag);
	m_decay.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_decay.setTextValueSuffix(" s");
	m_decay.setKnobTooltip("m_decay");
	addAndMakeVisible(m_decay);

	m_buildup.setSliderStyle(Slider::RotaryVerticalDrag);
	m_buildup.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	//m_buildup.setTextValueSuffix("");
	m_buildup.setKnobTooltip("m_buildup");
	addAndMakeVisible(m_buildup);

	m_EQ_gain.setSliderStyle(Slider::RotaryVerticalDrag);
	m_EQ_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_EQ_gain.setTextValueSuffix(" dB");
	m_EQ_gain.setNumDecimalPlacesToDisplay(1);
	m_EQ_gain.setKnobTooltip("EQ GAIN");
	addAndMakeVisible(m_EQ_gain);

	m_EQ_freq.setSliderStyle(Slider::RotaryVerticalDrag);
	m_EQ_freq.setTextValueSuffix(" Hz");
	m_EQ_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_EQ_freq.setKnobTooltip("EQ FREQ");
	addAndMakeVisible(m_EQ_freq);

	m_ducking.setSliderStyle(Slider::RotaryVerticalDrag);
	m_ducking.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_ducking.setKnobTooltip("DUCKING");
	addAndMakeVisible(m_ducking);

	m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
	m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	m_dry_wet.setKnobTooltip("DRYWET");
	addAndMakeVisible(m_dry_wet);

	m_delay.setNumDecimalPlacesToDisplay(3);
	m_roomsize.setNumDecimalPlacesToDisplay(3);
	m_diffusion.setNumDecimalPlacesToDisplay(3);
	m_decay.setNumDecimalPlacesToDisplay(3);
	m_buildup.setNumDecimalPlacesToDisplay(3);
	m_ducking.setNumDecimalPlacesToDisplay(3);
	m_EQ_freq.setNumDecimalPlacesToDisplay(3);
	m_dry_wet.setNumDecimalPlacesToDisplay(3);

	m_module.setInlay(1);
	m_module.addItem("Zita", 1);
	m_module.addItem("Surge", 2);
	m_module.setEditableText(false);
	m_module.showTriangle();
	m_module.setSelectedId(1, dontSendNotification);
	m_module.setColor(juce::STANDARD_DISPLAY_COLOR);
	m_module.setTooltip("Selects whether to use the reverb algorithm from Surge or Zita-rev1");

	m_module.onChange = [&]() {
		//m_value_tree.state.getChildWithName("fx").setProperty("reverb_module", m_module.getSelectedId(), nullptr);
		setReverbType((ReverbType)m_module.getSelectedId());
	};
	//addAndMakeVisible(m_module);

	SET_CTR_KEY(m_EQ_gain);
	SET_CTR_KEY(m_EQ_freq);
	SET_CTR_KEY(m_ducking);
	SET_CTR_KEY(m_dry_wet);

	forceValueTreeOntoComponents(m_value_tree.state);
}

ReverbComponent::~ReverbComponent() {
}

void ReverbComponent::paint(Graphics &g) {

	//DEBUG_COMPONENT_OUTLINE
	SET_INTERPOLATION_QUALITY(g)

	g.drawImageAt(m_background, 0, 0);

	if (m_GUI_big) {
		g.setColour(Colours::white);
		// Rectangle<int> bounds;

		// bounds.setBounds(OdinHelper::c150(REVERB_DELAY_POS_X),
		//                  OdinHelper::c150(REVERB_DELAY_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("Pre-Delay", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_ROOMSIZE_POS_X),
		//                  OdinHelper::c150(REVERB_ROOMSIZE_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("Roomsize", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_DIFFUSION_POS_X),
		//                  OdinHelper::c150(REVERB_DIFFUSION_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("Diffusion", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_DECAY_POS_X),
		//                  OdinHelper::c150(REVERB_DECAY_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("Decay", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_BUILDUP_POS_X),
		//                  OdinHelper::c150(REVERB_BUILDUP_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("Buildup", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_EQ_GAIN_POS_X),
		//                  OdinHelper::c150(REVERB_EQ_GAIN_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("EQ Gain", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_WIDTH_POS_X),
		//                  OdinHelper::c150(REVERB_WIDTH_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("Width", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_DRY_POS_X),
		//                  OdinHelper::c150(REVERB_DRY_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("EQ Freq", bounds, Justification::centredLeft, true);

		// bounds.setBounds(OdinHelper::c150(REVERB_WET_POS_X),
		//                  OdinHelper::c150(REVERB_WET_POS_Y) + OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y) + 4,
		//                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X) * 2.f,
		//                  10);
		// g.drawText("DryWet", bounds, Justification::centredLeft, true);
	}
}

void ReverbComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	//todo make dynamic
	setReverbType(ReverbType::Zita);
}

void ReverbComponent::setGUIBig() {
	m_GUI_big = true;

	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_150_png, BinaryData::black_knob_mid_150_pngSize);
	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_150_png, BinaryData::metal_knob_mid_150_pngSize);
	juce::Image black_knob_big =
	    ImageCache::getFromMemory(BinaryData::black_knob_big_150_png, BinaryData::black_knob_big_150_pngSize);

	m_delay.setStrip(metal_knob_mid, N_KNOB_FRAMES);
	m_roomsize.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_diffusion.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_decay.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_buildup.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_EQ_gain.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_ducking.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_EQ_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_dry_wet.setStrip(metal_knob_mid, N_KNOB_FRAMES);

	m_low_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_low_hall.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_mid_hall.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_hf_damp.setStrip(black_knob_mid, N_KNOB_FRAMES);

	m_low_freq.setBounds(OdinHelper::c150(REVERB_LOW_FREQ_POS_X),
	                     OdinHelper::c150(REVERB_LOW_FREQ_POS_Y),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_low_hall.setBounds(OdinHelper::c150(REVERB_LOW_HALL_POS_X),
	                     OdinHelper::c150(REVERB_LOW_HALL_POS_Y),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_mid_hall.setBounds(OdinHelper::c150(REVERB_MID_HALL_POS_X),
	                     OdinHelper::c150(REVERB_MID_HALL_POS_Y),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_hf_damp.setBounds(OdinHelper::c150(REVERB_HF_DAMP_POS_X),
	                    OdinHelper::c150(REVERB_HF_DAMP_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));

	m_delay.setBounds(OdinHelper::c150(REVERB_DELAY_POS_X),
	                  OdinHelper::c150(REVERB_DELAY_POS_Y),
	                  OdinHelper::c150(METAL_KNOB_MID_SIZE_X),
	                  OdinHelper::c150(METAL_KNOB_MID_SIZE_Y));
	m_roomsize.setBounds(OdinHelper::c150(REVERB_ROOMSIZE_POS_X),
	                     OdinHelper::c150(REVERB_ROOMSIZE_POS_Y),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                     OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_diffusion.setBounds(OdinHelper::c150(REVERB_DIFFUSION_POS_X),
	                      OdinHelper::c150(REVERB_DIFFUSION_POS_Y),
	                      OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                      OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_decay.setBounds(OdinHelper::c150(REVERB_DECAY_POS_X),
	                  OdinHelper::c150(REVERB_DECAY_POS_Y),
	                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                  OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_buildup.setBounds(OdinHelper::c150(REVERB_BUILDUP_POS_X),
	                    OdinHelper::c150(REVERB_BUILDUP_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));

	m_EQ_gain.setBounds(OdinHelper::c150(REVERB_EQ_GAIN_POS_X),
	                    OdinHelper::c150(REVERB_EQ_GAIN_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_ducking.setBounds(OdinHelper::c150(REVERB_WIDTH_POS_X),
	                    OdinHelper::c150(REVERB_WIDTH_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_EQ_freq.setBounds(OdinHelper::c150(REVERB_DRY_POS_X),
	                    OdinHelper::c150(REVERB_DRY_POS_Y),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(BLACK_KNOB_MID_SIZE_Y));
	m_dry_wet.setBounds(OdinHelper::c150(REVERB_WET_POS_X),
	                    OdinHelper::c150(REVERB_WET_POS_Y),
	                    OdinHelper::c150(METAL_KNOB_MID_SIZE_X),
	                    OdinHelper::c150(METAL_KNOB_MID_SIZE_Y));

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_150_png, BinaryData::glaspanel_midbig_150_pngSize);
	m_module.setImage(glas_panel);
	m_module.setInlay(1);
	m_module.setBounds(
	    OdinHelper::c150(MODULE_POS_X), OdinHelper::c150(MODULE_POS_Y), glas_panel.getWidth(), glas_panel.getHeight());
	m_module.setGUIBig();

	forceValueTreeOntoComponents(m_value_tree.state);
}

void ReverbComponent::setGUISmall() {
	m_GUI_big = false;

	juce::Image metal_knob_mid =
	    ImageCache::getFromMemory(BinaryData::metal_knob_mid_150_png, BinaryData::metal_knob_mid_150_pngSize);
	juce::Image black_knob_mid =
	    ImageCache::getFromMemory(BinaryData::black_knob_mid_png, BinaryData::black_knob_mid_pngSize);
	m_EQ_gain.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_ducking.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_EQ_freq.setStrip(black_knob_mid, N_KNOB_FRAMES);
	m_dry_wet.setStrip(black_knob_mid, N_KNOB_FRAMES);

	m_EQ_gain.setBounds(REVERB_EQ_GAIN_POS_X, REVERB_EQ_GAIN_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
	m_ducking.setBounds(REVERB_WIDTH_POS_X, REVERB_WIDTH_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
	m_EQ_freq.setBounds(REVERB_DRY_POS_X, REVERB_DRY_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
	m_dry_wet.setBounds(REVERB_WET_POS_X, REVERB_WET_POS_Y, BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_png, BinaryData::glaspanel_midbig_pngSize);
	m_module.setImage(glas_panel);
	m_module.setInlay(1);
	m_module.setBounds(MODULE_POS_X, MODULE_POS_Y, glas_panel.getWidth(), glas_panel.getHeight());
	m_module.setGUISmall();

	forceValueTreeOntoComponents(m_value_tree.state);
}

void ReverbComponent::setReverbType(ReverbType p_type) {
	m_reverb_type = p_type;

	//hide everything
	//m_delay.setVisible(false);
	m_roomsize.setVisible(false);
	m_diffusion.setVisible(false);
	m_decay.setVisible(false);
	m_buildup.setVisible(false);
	m_ducking.setVisible(false);

	//show needed controls
	if (m_reverb_type == ReverbType::Zita) {
	} else {
		m_delay.setVisible(true);
		m_roomsize.setVisible(true);
		m_diffusion.setVisible(true);
		m_decay.setVisible(true);
		m_buildup.setVisible(true);
		m_ducking.setVisible(true);
	}

	//set background images
	juce::Image reverb_image;
	if (m_GUI_big) {
		if (m_reverb_type == ReverbType::Zita) {
			reverb_image =
			    ImageCache::getFromMemory(BinaryData::reverb_zita_150_png, BinaryData::reverb_zita_150_pngSize);
		} else {
			reverb_image =
			    ImageCache::getFromMemory(BinaryData::reverb_surge_150_png, BinaryData::reverb_surge_150_pngSize);
		}
		setImage(reverb_image);
	} else {
		//todo
		if (m_reverb_type == ReverbType::Zita) {
			reverb_image =
			    ImageCache::getFromMemory(BinaryData::reverb_zita_150_png, BinaryData::reverb_zita_150_pngSize);
		} else {
			reverb_image =
			    ImageCache::getFromMemory(BinaryData::reverb_surge_150_png, BinaryData::reverb_surge_150_pngSize);
		}
		setImage(reverb_image);
	}

	repaint();
}
