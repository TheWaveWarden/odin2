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

#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinButton.h"
#include "UIAssetManager.h"

class DragButton : public juce::Component, public juce::SettableTooltipClient {
public:
	DragButton(const String &p_text, juce::ComponentBoundsConstrainer *constrainer) : m_constrainer(constrainer), m_text(p_text) {
	}

	void paint(juce::Graphics &g) override {

		auto asset = int(UIAssets::Indices::bttn_12x4_FX_off);
		if (m_pressed)
			asset += 3;
		else if (m_highlight)
			asset += 1;

		g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices(asset), ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);

		g.setColour(m_pressed ? COL_TEXT_BLUE_DARK : COL_TEXT_BLUE);
		g.setFont(Helpers::getAldrichFont(H * 0.53f));
		g.drawText(m_text, getLocalBounds().removeFromBottom(proportionOfHeight(0.94f)), Justification::centred);
	}

	void setIsPressed(bool p_pressed) {
		m_pressed = p_pressed;
		repaint();
	}

	bool isInRangeDrag(float p_low, int p_check, float p_high) {
		return ((p_check >= p_low) && (p_check <= p_high));
	}
	void mouseDrag(const MouseEvent &event) override {
		m_dragger.dragComponent(this, event, m_constrainer);
		const auto width = getWidth();

		if (isInRangeDrag(0 * width, getX(), 0.5f * width)) {
			lambdaMouseDrag(0);
			return;
		}

		if (isInRangeDrag(0.5f * width, getX(), 1.5f * width)) {
			lambdaMouseDrag(1);
			return;
		}

		if (isInRangeDrag(1.5f * width, getX(), 2.5f * width)) {
			lambdaMouseDrag(2);
			return;
		}

		if (isInRangeDrag(2.5f * width, getX(), 3.5f * width)) {
			lambdaMouseDrag(3);
			return;
		}

		if (isInRangeDrag(3.5f * width, getX(), 4.f * width)) {
			lambdaMouseDrag(4);
			return;
		}
	}

	void mouseDown(const MouseEvent &event) override {
		setAlwaysOnTop(true);
		m_dragger.startDraggingComponent(this, event);
		lambdaMouseDown();
		m_pressed = true;
	}

	void mouseUp(const MouseEvent &event) override {
		setAlwaysOnTop(false);
		lambdaMouseUp();
	}

	void mouseMove(const MouseEvent &event) override {
		if (!m_highlight) {
			m_highlight = true;
			repaint();
		}
	}

	void mouseExit(const MouseEvent &event) override {
		if (m_highlight) {
			m_highlight = false;
			repaint();
		}
	}

	// std::function lambdaMouseDown;
	std::function<void(int)> lambdaMouseDrag;
	std::function<void()> lambdaMouseUp;
	std::function<void()> lambdaMouseDown;

private:
	bool m_pressed   = false;
	bool m_highlight = false;

	juce::String m_text;
	juce::ComponentDragger m_dragger;
	juce::ComponentBoundsConstrainer *m_constrainer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragButton)
};
