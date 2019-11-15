/*
  ==============================================================================

    NumberSelector.cpp
    Created: 1 Mar 2019 9:29:33pm
    Author:  frot

  ==============================================================================
*/

#include "NumberSelector.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
NumberSelector::NumberSelector(bool p_buttons_right) :
    m_up("up", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_down("down", juce::DrawableButton::ButtonStyle::ImageRaw) {

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_small_png, BinaryData::glaspanel_small_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);
	m_display.setText(std::to_string(m_value));

	m_display.toParentMouseDown = [&](const MouseEvent e) {
		mouse_reference_value = e.getScreenY();
		m_drag_initial_value  = m_value;
		// Component::mouseDown(e);
	};

	m_display.toParentMouseDrag = [&](const MouseEvent e) {
		float mouse_moved = mouse_reference_value - e.getScreenY();

		int new_value = m_drag_initial_value + mouse_moved / m_mouse_drag_divisor;
		new_value     = new_value > m_max ? m_max : new_value;
		new_value     = new_value < m_min ? m_min : new_value;

		setValue(new_value);

		// Component::mouseDrag(e);
	};

	m_display.toParentMouseUp = [&](const MouseEvent e) {};

	addAndMakeVisible(m_display);

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setClickingTogglesState(true);
	m_up.setBounds(glas_panel.getWidth(), 0, up_1.getWidth(), up_1.getHeight());
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { increment(); };

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setClickingTogglesState(true);
	m_down.setBounds(glas_panel.getWidth(), down_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { decrement(); };
	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight() + 1);

	if (p_buttons_right) {
		m_display.setTopLeftPosition(0, 0);
		m_up.setTopLeftPosition(glas_panel.getWidth() - 1, 1);
		m_down.setTopLeftPosition(glas_panel.getWidth() - 1, down_1.getHeight() + 1);
	} else {
		m_display.setTopLeftPosition(down_1.getWidth(), 0);
		m_up.setTopLeftPosition(1, 1);
		m_down.setTopLeftPosition(1, down_1.getHeight() + 1);
	}

	m_display.onMouseDown = [&]() {
		// do nothing
	};

	m_display_width = glas_panel.getWidth();
	m_buttons_right = p_buttons_right;

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	setValue(1);
}

NumberSelector::~NumberSelector() {
}

void NumberSelector::paint(Graphics &g) {
}
