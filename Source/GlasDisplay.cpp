/*
  ==============================================================================

    GlasDisplay.cpp
    Created: 27 Feb 2019 11:56:53am
    Author:  frot

  ==============================================================================
*/

#include "GlasDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
GlasDisplay::GlasDisplay() {
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  setLookAndFeel(&m_menu_feels);
}

GlasDisplay::~GlasDisplay() {
  setLookAndFeel(nullptr);  
}

void GlasDisplay::paint(Graphics &g) {
  SET_INTERPOLATION_QUALITY(g)

  g.setColour(m_color);
  juce::Point<int> top_left = getLocalBounds().getTopLeft();
  top_left.addXY(m_inlay, m_inlay);
  juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
  bottom_right.addXY(-m_inlay, -m_inlay);
  g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //pmai
  g.setColour(Colours::white);
  Font current_font = g.getCurrentFont();
  current_font.setStyleFlags(1);//bold
  g.setFont(current_font);
  g.setFont(12.0f);
  g.drawText(m_text, getLocalBounds(), Justification::centred,
             true); // draw some placeholder text

  g.drawImageAt(m_glas_panel, 0, 0);


}

void 	GlasDisplay::mouseDown (const MouseEvent &event){
  onMouseDown();
  toParentMouseDown(event);
  Component::mouseDown(event);
}

void 	GlasDisplay::mouseDrag (const MouseEvent &event){
  toParentMouseDrag(event);
  Component::mouseDrag(event);
}

void 	GlasDisplay::mouseUp (const MouseEvent &event){
  toParentMouseUp(event);
  Component::mouseUp(event);
}

