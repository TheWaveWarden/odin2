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

#include "XYSectionComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

XYSectionComponent::XYSectionComponent(AudioProcessorValueTreeState &vts,const std::string &p_section_name) :
    m_value_tree(vts), m_section_name(p_section_name), m_xy_pad(vts, "xy_", m_modx, m_mody) {

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


void XYSectionComponent::setGUIBig(){

	m_GUI_big = true;
	
	juce::Image glas_panel = ImageCache::getFromMemory(BinaryData::vectorpanel_150_png, BinaryData::vectorpanel_150_pngSize);
	m_xy_pad.setTopLeftPosition(0, OdinHelper::c150(XY_PAD_POSIITON_Y));
	m_xy_pad.setSize(glas_panel.getWidth(), glas_panel.getHeight());
	m_xy_pad.setInlay(1);
	m_xy_pad.setImage(glas_panel);


	juce::Image black_knob_very_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_very_small_150_png, BinaryData::black_knob_very_small_150_pngSize);

	m_modx.setStrip(black_knob_very_small, N_KNOB_FRAMES);

	m_mody.setStrip(black_knob_very_small, N_KNOB_FRAMES);

	m_modx.setBounds(OdinHelper::c150(MODX_POS_X) - OdinHelper::c150(BLACK_KNOB_VERY_SMALL_OFFSET_X)-1,
	                 OdinHelper::c150(MODX_POS_Y) - OdinHelper::c150(BLACK_KNOB_VERY_SMALL_OFFSET_Y),
	                 OdinHelper::c150(BLACK_KNOB_VERY_SMALL_SIZE_X),
	                 OdinHelper::c150(BLACK_KNOB_VERY_SMALL_SIZE_Y));
	m_mody.setBounds(OdinHelper::c150(MODY_POS_X) - OdinHelper::c150(BLACK_KNOB_VERY_SMALL_OFFSET_X)-1,
	                 OdinHelper::c150(MODY_POS_Y) - OdinHelper::c150(BLACK_KNOB_VERY_SMALL_OFFSET_Y),
	                 OdinHelper::c150(BLACK_KNOB_VERY_SMALL_SIZE_X),
	                 OdinHelper::c150(BLACK_KNOB_VERY_SMALL_SIZE_Y));

	juce::Image logo =
	    ImageCache::getFromMemory(BinaryData::WW_logo_xy_150_png, BinaryData::WW_logo_xy_150_pngSize);
	m_xy_pad.setLogoImage(logo);	

	m_xy_pad.setGUIBig();
}

void XYSectionComponent::setGUISmall(){

	m_GUI_big = false;

	juce::Image glas_panel = ImageCache::getFromMemory(BinaryData::vectorpanel_png, BinaryData::vectorpanel_pngSize);
	m_xy_pad.setTopLeftPosition(0, XY_PAD_POSIITON_Y);
	m_xy_pad.setSize(glas_panel.getWidth(), glas_panel.getHeight());
	m_xy_pad.setInlay(1);
	m_xy_pad.setImage(glas_panel);


	juce::Image black_knob_very_small =
	    ImageCache::getFromMemory(BinaryData::black_knob_very_small_png, BinaryData::black_knob_very_small_pngSize);

	m_modx.setStrip(black_knob_very_small, N_KNOB_FRAMES);

	m_mody.setStrip(black_knob_very_small, N_KNOB_FRAMES);

	m_modx.setBounds(MODX_POS_X - BLACK_KNOB_VERY_SMALL_OFFSET_X,
	                 MODX_POS_Y - BLACK_KNOB_VERY_SMALL_OFFSET_Y,
	                 BLACK_KNOB_VERY_SMALL_SIZE_X,
	                 BLACK_KNOB_VERY_SMALL_SIZE_Y);
	m_mody.setBounds(MODY_POS_X - BLACK_KNOB_VERY_SMALL_OFFSET_X,
	                 MODY_POS_Y - BLACK_KNOB_VERY_SMALL_OFFSET_Y,
	                 BLACK_KNOB_VERY_SMALL_SIZE_X,
	                 BLACK_KNOB_VERY_SMALL_SIZE_Y);

	juce::Image logo =
	    ImageCache::getFromMemory(BinaryData::WW_logo_xy_png, BinaryData::WW_logo_xy_pngSize);
	m_xy_pad.setLogoImage(logo);

	m_xy_pad.setGUISmall();
}