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

#include "LFOSelectorComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"

LFOSelectorComponent::LFOSelectorComponent() :
    m_up("up", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_down("down", juce::DrawableButton::ButtonStyle::ImageRaw) {

	m_display.setInlay(1);
	addAndMakeVisible(m_display);

	m_up.setClickingTogglesState(true);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { increment(); };

	m_down.setClickingTogglesState(true);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { decrement(); };

	m_display.toParentMouseDown = [&](const MouseEvent e) {
		mouse_reference_value = e.getScreenY();
		m_drag_initial_value  = m_value;
		Component::mouseDown(e);
	};

	m_display.toParentMouseDrag = [&](const MouseEvent e) {
		float mouse_moved = mouse_reference_value - e.getScreenY();

		float m_max = m_display.getNrOfWaves() - 1;
		float m_min = 0.f;

		int new_value = m_drag_initial_value + mouse_moved / 20.f;
		new_value     = new_value > m_max ? m_max : new_value;
		new_value     = new_value < m_min ? m_min : new_value;

		setValue(new_value);

		Component::mouseDrag(e);
	};

	m_display.toParentMouseUp = [&](const MouseEvent e) {};

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
}

LFOSelectorComponent::~LFOSelectorComponent() {
}

void LFOSelectorComponent::paint(Graphics &g) {
}

void LFOSelectorComponent::setGUIBig() {
	m_GUI_big = true;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_mid_150_png, BinaryData::glaspanel_mid_150_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);

	m_display.clearWaves();

	juce::Image sine = ImageCache::getFromMemory(BinaryData::sine_150_png, BinaryData::sine_150_pngSize);
	m_display.addWave(sine);
	juce::Image saw = ImageCache::getFromMemory(BinaryData::saw_150_png, BinaryData::saw_150_pngSize);
	m_display.addWave(saw);
	juce::Image triangle = ImageCache::getFromMemory(BinaryData::triangle_150_png, BinaryData::triangle_150_pngSize);
	m_display.addWave(triangle);
	juce::Image square = ImageCache::getFromMemory(BinaryData::square50_150_png, BinaryData::square50_150_pngSize);
	m_display.addWave(square);
	juce::Image square25 = ImageCache::getFromMemory(BinaryData::square25_150_png, BinaryData::square25_150_pngSize);
	m_display.addWave(square25);
	juce::Image square12 = ImageCache::getFromMemory(BinaryData::square12_150_png, BinaryData::square12_150_pngSize);
	m_display.addWave(square12);
	juce::Image peak = ImageCache::getFromMemory(BinaryData::peak_150_png, BinaryData::peak_150_pngSize);
	m_display.addWave(peak);
	juce::Image SH = ImageCache::getFromMemory(BinaryData::SH_150_png, BinaryData::SH_150_pngSize);
	m_display.addWave(SH);
	juce::Image pyram4 = ImageCache::getFromMemory(BinaryData::pyram4_150_png, BinaryData::pyram4_150_pngSize);
	m_display.addWave(pyram4);
	juce::Image pyram6 = ImageCache::getFromMemory(BinaryData::pyram6_150_png, BinaryData::pyram6_150_pngSize);
	m_display.addWave(pyram6);
	juce::Image pyram8 = ImageCache::getFromMemory(BinaryData::pyram8_150_png, BinaryData::pyram8_150_pngSize);
	m_display.addWave(pyram8);
	juce::Image pyram12 = ImageCache::getFromMemory(BinaryData::pyram12_150_png, BinaryData::pyram12_150_pngSize);
	m_display.addWave(pyram12);
	juce::Image stair3 = ImageCache::getFromMemory(BinaryData::stair3_150_png, BinaryData::stair3_150_pngSize);
	m_display.addWave(stair3);
	juce::Image stair4 = ImageCache::getFromMemory(BinaryData::stair4_150_png, BinaryData::stair4_150_pngSize);
	m_display.addWave(stair4);
	juce::Image stair6 = ImageCache::getFromMemory(BinaryData::stair6_150_png, BinaryData::stair6_150_pngSize);
	m_display.addWave(stair6);
	juce::Image stair8 = ImageCache::getFromMemory(BinaryData::stair8_150_png, BinaryData::stair8_150_pngSize);
	m_display.addWave(stair8);
	juce::Image stair12 = ImageCache::getFromMemory(BinaryData::stair12_150_png, BinaryData::stair12_150_pngSize);
	m_display.addWave(stair12);
	juce::Image wavedraw1 = ImageCache::getFromMemory(BinaryData::wavedraw1_150_png, BinaryData::wavedraw1_150_pngSize);
	m_display.addWave(wavedraw1);
	juce::Image wavedraw2 = ImageCache::getFromMemory(BinaryData::wavedraw2_150_png, BinaryData::wavedraw2_150_pngSize);
	m_display.addWave(wavedraw2);
	juce::Image wavedraw3 = ImageCache::getFromMemory(BinaryData::wavedraw3_150_png, BinaryData::wavedraw3_150_pngSize);
	m_display.addWave(wavedraw3);

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_150_png, BinaryData::buttonup_2_150_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_150_png, BinaryData::buttonup_1_150_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(glas_panel.getWidth() - 2, 2, up_1.getWidth(), up_1.getHeight());

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
	m_down.setBounds(glas_panel.getWidth() - 2, 2 + up_1.getHeight(), down_1.getWidth(), down_1.getHeight());

	m_display.setInlayTop(2);
	m_display.setInlayLeft(2);

	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight());
}
void LFOSelectorComponent::setGUISmall() {
	m_GUI_big = false;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_mid_png, BinaryData::glaspanel_mid_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);

	m_display.clearWaves();

	juce::Image sine = ImageCache::getFromMemory(BinaryData::sine_png, BinaryData::sine_pngSize);
	m_display.addWave(sine);
	juce::Image saw = ImageCache::getFromMemory(BinaryData::saw_png, BinaryData::saw_pngSize);
	m_display.addWave(saw);
	juce::Image triangle = ImageCache::getFromMemory(BinaryData::triangle_png, BinaryData::triangle_pngSize);
	m_display.addWave(triangle);
	juce::Image square = ImageCache::getFromMemory(BinaryData::square50_png, BinaryData::square50_pngSize);
	m_display.addWave(square);
	juce::Image square25 = ImageCache::getFromMemory(BinaryData::square25_png, BinaryData::square25_pngSize);
	m_display.addWave(square25);
	juce::Image square12 = ImageCache::getFromMemory(BinaryData::square12_png, BinaryData::square12_pngSize);
	m_display.addWave(square12);
	juce::Image peak = ImageCache::getFromMemory(BinaryData::peak_png, BinaryData::peak_pngSize);
	m_display.addWave(peak);
	juce::Image SH = ImageCache::getFromMemory(BinaryData::SH_png, BinaryData::SH_pngSize);
	m_display.addWave(SH);
	juce::Image pyram4 = ImageCache::getFromMemory(BinaryData::pyram4_png, BinaryData::pyram4_pngSize);
	m_display.addWave(pyram4);
	juce::Image pyram6 = ImageCache::getFromMemory(BinaryData::pyram6_png, BinaryData::pyram6_pngSize);
	m_display.addWave(pyram6);
	juce::Image pyram8 = ImageCache::getFromMemory(BinaryData::pyram8_png, BinaryData::pyram8_pngSize);
	m_display.addWave(pyram8);
	juce::Image pyram12 = ImageCache::getFromMemory(BinaryData::pyram12_png, BinaryData::pyram12_pngSize);
	m_display.addWave(pyram12);
	juce::Image stair3 = ImageCache::getFromMemory(BinaryData::stair3_png, BinaryData::stair3_pngSize);
	m_display.addWave(stair3);
	juce::Image stair4 = ImageCache::getFromMemory(BinaryData::stair4_png, BinaryData::stair4_pngSize);
	m_display.addWave(stair4);
	juce::Image stair6 = ImageCache::getFromMemory(BinaryData::stair6_png, BinaryData::stair6_pngSize);
	m_display.addWave(stair6);
	juce::Image stair8 = ImageCache::getFromMemory(BinaryData::stair8_png, BinaryData::stair8_pngSize);
	m_display.addWave(stair8);
	juce::Image stair12 = ImageCache::getFromMemory(BinaryData::stair12_png, BinaryData::stair12_pngSize);
	m_display.addWave(stair12);
	juce::Image wavedraw1 = ImageCache::getFromMemory(BinaryData::wavedraw1_png, BinaryData::wavedraw1_pngSize);
	m_display.addWave(wavedraw1);
	juce::Image wavedraw2 = ImageCache::getFromMemory(BinaryData::wavedraw2_png, BinaryData::wavedraw2_pngSize);
	m_display.addWave(wavedraw2);
	juce::Image wavedraw3 = ImageCache::getFromMemory(BinaryData::wavedraw3_png, BinaryData::wavedraw3_pngSize);
	m_display.addWave(wavedraw3);

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(glas_panel.getWidth() - 2, 1, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(glas_panel.getWidth() - 2, 1 + down_1.getHeight(), down_1.getWidth(), down_1.getHeight());

	m_display.setInlayTop(0);
	m_display.setInlayLeft(0);

	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight());
}