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

#include "WaveformSelectorComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"

WaveformSelectorComponent::WaveformSelectorComponent(bool p_buttons_right) :
    m_up("up", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_down("down", juce::DrawableButton::ButtonStyle::ImageRaw) {

	m_display.setInlay(1);
	addAndMakeVisible(m_display);

	m_up.setClickingTogglesState(true);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { decrement(); };

	m_down.setClickingTogglesState(true);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { increment(); };

	m_display.onMouseDown = [&]() { setValue(m_menu.show()); };

	m_buttons_right = p_buttons_right;

	m_menu.setLookAndFeel(&m_menu_feels);

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	setValue(1);
}

WaveformSelectorComponent::~WaveformSelectorComponent() {
	m_menu.setLookAndFeel(nullptr);
}

void WaveformSelectorComponent::paint(Graphics &g) {
	if (m_GUI_big) {
		//draw an outline
		g.setColour(juce::Colour(30, 30, 30));
		juce::Rectangle<int> rect = getLocalBounds();
		if (m_buttons_right) {
			rect.setX(110);
			rect.setY(1);
			rect.setHeight(rect.getHeight() - 2);
			rect.setWidth(53);
			g.drawRect(rect, 2);
		} else {
			rect.setX(1);
			rect.setY(1);
			rect.setHeight(rect.getHeight() - 2);
			rect.setWidth(53);
			g.drawRect(rect, 2);
		}
	}
}

void WaveformSelectorComponent::setGUIBig() {
	m_GUI_big = true;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_big_150_png, BinaryData::glaspanel_big_150_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(0, 0, glas_panel.getWidth() - 2, glas_panel.getHeight());
	m_display.setInlay(1);

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_150_png, BinaryData::buttonup_2_150_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_150_png, BinaryData::buttonup_1_150_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(glas_panel.getWidth() - 1, 1, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 =
	    ImageCache::getFromMemory(BinaryData::buttondown_2_150_png, BinaryData::buttondown_2_150_pngSize);
	juce::Image down_2 =
	    ImageCache::getFromMemory(BinaryData::buttondown_1_150_png, BinaryData::buttondown_1_150_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(glas_panel.getWidth() - 1, 1 + up_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight());

	if (m_buttons_right) {
		m_display.setTopLeftPosition(0, 0);
		m_up.setTopLeftPosition(glas_panel.getWidth() - 4, 1);
		m_down.setTopLeftPosition(glas_panel.getWidth() - 4, up_1.getHeight() + 1);
	} else {
		m_display.setTopLeftPosition(down_1.getWidth() - 2, 0);
		m_up.setTopLeftPosition(1, 1);
		m_down.setTopLeftPosition(1, up_1.getHeight() + 1);
	}

	m_display.setGUIBig();

	m_display_width = glas_panel.getWidth();
}
void WaveformSelectorComponent::setGUISmall() {
	m_GUI_big = false;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_big_png, BinaryData::glaspanel_big_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(glas_panel.getWidth() - 1, 1, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(glas_panel.getWidth() - 1, 1 + down_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight());

	if (m_buttons_right) {
		m_display.setTopLeftPosition(0, 0);
		m_up.setTopLeftPosition(glas_panel.getWidth() - 2, 1);
		m_down.setTopLeftPosition(glas_panel.getWidth() - 2, down_1.getHeight() + 1);
	} else {
		m_display.setTopLeftPosition(down_1.getWidth() - 1, 0);
		m_up.setTopLeftPosition(1, 1);
		m_down.setTopLeftPosition(1, down_1.getHeight() + 1);
	}

	m_display.setGUISmall();

	m_display_width = glas_panel.getWidth();
}