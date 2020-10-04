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

#include <JuceHeader.h>
#include "PatchBrowserScrollBar.h"

PatchBrowserScrollBar::PatchBrowserScrollBar()
{}

PatchBrowserScrollBar::~PatchBrowserScrollBar()
{
}

void PatchBrowserScrollBar::paint (juce::Graphics& g)
{
#define ODIN_BLUE_DARKER Colour(0xff111e24)
    //g.setColour(ODIN_BLUE_DARKER);
    //g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 12);
    //g.setColour(ODIN_BLUE);
    //g.drawRoundedRectangle (0, 0, getWidth(), getHeight(), 12 , 3);


    g.fillAll (ODIN_BLUE_DARKER);   // clear the background
    g.setColour (ODIN_BLUE);
    g.drawRect (getLocalBounds(), 2);   // draw an outline around the component

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
