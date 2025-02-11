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

#include "LFOSelectorComponent.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "UIAssetManager.h"

LFOSelectorComponent::LFOSelectorComponent() : m_up("up", "", OdinButton::Type::up), m_down("down", "", OdinButton::Type::down) {

	addAndMakeVisible(m_display);

	m_up.setClickingTogglesState(false);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { decrement(); };

	m_down.setClickingTogglesState(false);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { increment(); };

	m_display.toParentMouseDown = [&](const MouseEvent e) {
		mouse_reference_value = -e.getScreenY();
		m_drag_initial_value  = m_value;
		Component::mouseDown(e);
	};

	m_display.toParentMouseDrag = [&](const MouseEvent e) {
		float mouse_moved = mouse_reference_value + e.getScreenY();

		float m_max = m_display.getNrOfWaves() - 1;
		float m_min = 0.f;

		int new_value = m_drag_initial_value + mouse_moved / 20.f;
		new_value     = new_value > m_max ? m_max : new_value;
		new_value     = new_value < m_min ? m_min : new_value;

		setValue(new_value);

		Component::mouseDrag(e);
	};

	m_display.toParentMouseUp = [&](const MouseEvent e) { Component::mouseUp(e); };
	m_display.onClick         = [&]() {
        m_popup.showMenuAsync(PopupMenu::Options(), [&](int ret) {
            if (ret) {
                // ret values are offset by one to allow popup return 0 == nothing
                setValue(ret - 1);
            }
            getTopLevelComponent()->repaint();
        });
	};

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	m_popup.setLookAndFeel(&m_popup_look_and_feel);
	m_popup.addItem(1, "si");
	m_popup.addItem(2, "sa");
	m_popup.addItem(3, "tr");
	m_popup.addItem(4, "sq50");
	m_popup.addItem(5, "sq25");
	m_popup.addItem(6, "sq12");
	m_popup.addItem(7, "pe");
	m_popup.addSeparator();
	m_popup.addItem(8, "sh");
	m_popup.addSeparator();
	m_popup.addItem(9, "p4");
	m_popup.addItem(10, "p6");
	m_popup.addItem(11, "p8");
	m_popup.addItem(12, "p12");
	m_popup.addSeparator();
	m_popup.addItem(13, "s3");
	m_popup.addItem(14, "s4");
	m_popup.addItem(15, "s6");
	m_popup.addItem(16, "s8");
	m_popup.addItem(17, "s12");
	m_popup.addSeparator();
	m_popup.addItem(18, "w1");
	m_popup.addItem(19, "w2");
	m_popup.addItem(20, "w3");
	m_popup.addSeparator();
	m_popup.addItem(21, "c1");
	m_popup.addItem(22, "c2");
	m_popup.addItem(23, "c3");
}

LFOSelectorComponent::~LFOSelectorComponent() {
	m_popup.setLookAndFeel(nullptr);
}

void LFOSelectorComponent::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::screen_up_down_13x4_R, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void LFOSelectorComponent::resized() {

	auto bounds        = getLocalBounds();
	auto button_bounds = bounds.removeFromRight(H);

	m_up.setBounds(button_bounds.removeFromTop(proportionOfHeight(0.5f)));
	m_down.setBounds(button_bounds);
	m_display.setBounds(bounds);
}