#include <JuceHeader.h>
#include "PatchBrowserScrollBar.h"

PatchBrowserScrollBar::PatchBrowserScrollBar()
{}

PatchBrowserScrollBar::~PatchBrowserScrollBar()
{
}

void PatchBrowserScrollBar::paint (juce::Graphics& g)
{
    g.fillAll (ODIN_BLUE);   // clear the background

    //g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("PatchBrowserScrollBar", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}


void PatchBrowserScrollBar::mouseDown(const MouseEvent& e){
  m_drag_reference_y = e.getScreenY() - getY();
}

void PatchBrowserScrollBar::mouseDrag(const MouseEvent& e){
  reportMouseDrag(m_drag_reference_y - e.getScreenY());
}
