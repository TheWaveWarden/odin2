#pragma once
#include "../GlobalIncludes.h"

class GuiDataEditor : public juce::Component {
public:
	GuiDataEditor();
	~GuiDataEditor() override;

	void paint (juce::Graphics&) override;

	void mouseDown (const juce::MouseEvent& e) override;
	void mouseDrag (const juce::MouseEvent& e) override;
	void resized() override;

    void setParent(const juce::Rectangle<int>& p_parent);

    juce::Rectangle<int> getData() const;

private:
	juce::Colour m_colour_background = juce::Colours::black;
	juce::Colour m_colour_text       = juce::Colours::white;
	juce::Colour m_colour_border     = juce::Colours::grey;

	juce::Rectangle<int> m_data;
	juce::Rectangle<int> m_parent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuiDataEditor)
};