/*
  ==============================================================================

    GlasDropdown.cpp
    Created: 25 Feb 2019 11:24:02pm
    Author:  frot

  ==============================================================================
*/

#include "GlasDropdown.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
GlasDropdown::GlasDropdown() {
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  setLookAndFeel(&m_menu_feels);
}

GlasDropdown::~GlasDropdown() { setLookAndFeel(nullptr); }

void GlasDropdown::paint(Graphics &g) {

  g.setColour(m_color);
  juce::Point<int> top_left = getLocalBounds().getTopLeft();
  top_left.addXY(m_inlay + 1, m_inlay);
  juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
  bottom_right.addXY(-m_inlay - 1, -m_inlay);
  g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); //

  g.setColour(Colours::white);

  if (getSelectedId() == 0 && m_grey_first_element) {
    g.setColour(Colours::lightgrey);
  }

  // g.setFont(bfont);Font current_font = g.getCurrentFont();
  Font current_font = g.getCurrentFont();
  current_font.setStyleFlags(1); // bold
  g.setFont(current_font);
  g.setFont(12.0f);

  if (getSelectedId() == 0) {
     g.drawText(m_default_text, getLocalBounds(), Justification::centred,
               true);
  } else {

    g.drawText(getText(), getLocalBounds(), Justification::centred,
               true);
  }
  g.drawImageAt(m_glaspanel, 0, 0);
}
