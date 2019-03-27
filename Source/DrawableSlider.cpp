/*
  ==============================================================================

    DrawableSlider.cpp
    Created: 26 Feb 2019 9:46:41pm
    Author:  frot

  ==============================================================================
*/

#include "DrawableSlider.h"
#include "GlobalIncludes.h"


//==============================================================================
DrawableSlider::DrawableSlider()
{
  setLookAndFeel(&m_feels);

  setPopupDisplayEnabled(true, false, nullptr);
  setNumDecimalPlacesToDisplay(3);

  m_handle = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/slider_handle.png"));
  //setRange(1,2);
}

DrawableSlider::~DrawableSlider() { setLookAndFeel(nullptr); }

void DrawableSlider::paint(Graphics &g)
{
  //g.setColour(Colours::grey);
  //g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.drawImageAt(m_handle, 0, (1.f - valueToProportionOfLength(getValue())) * (getHeight() - m_handle.getHeight()));

  //DBG(getValue());
}
