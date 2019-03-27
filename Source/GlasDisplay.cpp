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
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */
  g.setColour(m_color);
  juce::Point<int> top_left = getLocalBounds().getTopLeft();
  top_left.addXY(m_inlay, m_inlay);
  juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
  bottom_right.addXY(-m_inlay, -m_inlay);
  g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //pmai
  g.setColour(Colours::white);
  g.setFont(12.0f);
  g.drawText(m_text, getLocalBounds(), Justification::centred,
             true); // draw some placeholder text

  g.drawImageAt(m_glas_panel, 0, 0);


}

void GlasDisplay::resized() {
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void 	GlasDisplay::mouseDown (const MouseEvent &event){
  onMouseDown();
}

