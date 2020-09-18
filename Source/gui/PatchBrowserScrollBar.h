#pragma once

#include <JuceHeader.h>
#include "../GlobalIncludes.h"
#include <functional>

class PatchBrowserScrollBar  : public juce::Component
{
public:
    PatchBrowserScrollBar();
    ~PatchBrowserScrollBar() override;

    void paint (juce::Graphics&) override;
    void mouseDown(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;

    std::function<void(int)> reportMouseDrag;

    int m_drag_reference_y;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchBrowserScrollBar)
};
