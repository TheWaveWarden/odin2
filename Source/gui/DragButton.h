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

#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinButton.h"

class DragButton : public juce::Component, public juce::SettableTooltipClient {
public:
	DragButton(const String &p_text, juce::ComponentBoundsConstrainer *constrainer) :
	    m_constrainer(constrainer), m_text(p_text) {
	}

	void paint(juce::Graphics &g) override {
		g.setColour(COL_LIGHT);
		g.drawRect(getLocalBounds(), 1);

		if (m_pressed) {
			g.setColour(COL_LIGHT.withAlpha(0.5f));
			g.fillRect(getLocalBounds());
		}

		g.setColour(COL_LIGHT);
		g.setFont(H * 0.6f);
		g.drawText(m_text, getLocalBounds(), Justification::centred);
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

	// std::function lambdaMouseDown;
	std::function<void(int)> lambdaMouseDrag;
	std::function<void()> lambdaMouseUp;
	std::function<void()> lambdaMouseDown;

	void setGUIBig() {
		m_GUI_big = true;
	}
	void setGUISmall() {
		m_GUI_big = false;
	}

private:
	bool m_GUI_big = true;
	bool m_pressed = false;

	juce::String m_text;
	juce::ComponentDragger m_dragger;
	juce::ComponentBoundsConstrainer *m_constrainer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragButton)
};
