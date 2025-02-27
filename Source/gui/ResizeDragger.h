#pragma once

#include "../GlobalIncludes.h"

class ResizeDragger : public juce::Component, public juce::SettableTooltipClient {

public:
	ResizeDragger();
	~ResizeDragger() override;

	void paint(juce::Graphics &) override;
	void resized() override;

	bool hitTest(int p_x, int p_y) override;

	void mouseEnter(const juce::MouseEvent &p_event) override;
	void mouseExit(const juce::MouseEvent &p_event) override;
	void mouseDrag(const juce::MouseEvent &p_event) override;
	void mouseDown(const juce::MouseEvent &p_event) override;

	std::function<void()> onIncrease;
	std::function<void()> onDecrease;

private:
	int m_drag_reference = 0;
	bool m_highlight     = false;

	juce::Colour m_colour_dragger;
	juce::Colour m_colour_highlight;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ResizeDragger)
};