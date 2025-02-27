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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>

class LFODisplayComponent : public Component, public SettableTooltipClient {
public:
	LFODisplayComponent();
	~LFODisplayComponent();

	void paint(Graphics &) override;

	int getNrOfWaves() {
		return (int)m_waves.size();
	}

	void setValue(int p_value) {
		m_value = p_value;
		repaint();
	}

	void resized() override;

	void mouseEnter(const MouseEvent &event) override;
	void mouseExit(const MouseEvent &event) override;
	void mouseDown(const MouseEvent &event) override;
	void mouseDrag(const MouseEvent &event) override;
	void mouseUp(const MouseEvent &event) override;

	std::function<void(const MouseEvent &)> toParentMouseDown = [](const MouseEvent &) {};
	std::function<void(const MouseEvent &)> toParentMouseDrag = [](const MouseEvent &) {};
	std::function<void(const MouseEvent &)> toParentMouseUp   = [](const MouseEvent &) {};
	std::function<void()> onClick;

	void clearWaves() {
		m_waves.clear();
		m_waves_highlight.clear();
	}

	static void drawSineImage(juce::Graphics &g, juce::Colour p_col);
	static void drawSawImage(juce::Graphics &g, juce::Colour p_col);
	static void drawTriangleImage(juce::Graphics &g, juce::Colour p_col);
	static void drawSquareImage(juce::Graphics &g, juce::Colour p_col, float p_width);
	static void drawPointyImage(juce::Graphics &g, juce::Colour p_col);
	static void drawSHImage(juce::Graphics &g, juce::Colour p_col);
	static void drawPyramidImage(juce::Graphics &g, juce::Colour p_col, int p_steps);
	static void drawStepImage(juce::Graphics &g, juce::Colour p_col, int p_steps);
	static void drawTextImage(juce::Graphics &g, juce::Colour p_col, juce::String p_text);

private:
	int m_value = 0;

	std::vector<juce::Image> m_waves;
	std::vector<juce::Image> m_waves_highlight;

	bool m_highlight = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFODisplayComponent)
};
