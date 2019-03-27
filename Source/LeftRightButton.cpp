/*
  ==============================================================================

    LeftRightButton.cpp
    Created: 27 Feb 2019 3:35:54pm
    Author:  frot

  ==============================================================================
*/

#include "LeftRightButton.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
LeftRightButton::LeftRightButton(const String &buttonName)
    : juce::Button(buttonName) {
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

LeftRightButton::~LeftRightButton() {
    //setClickingTogglesState(true);
}

void LeftRightButton::paintButton(Graphics &g,
                                  bool shouldDrawButtonAsHighlighted,
                                  bool shouldDrawButtonAsDown) {
  g.fillAll(Colours::transparentBlack);

  if (getToggleState()) {
    g.drawImageAt(m_image_left, 0, 0);
  } else {
    g.drawImageAt(m_image_right, 0, 0);
  }
}

void LeftRightButton::mouseDown(const MouseEvent &p_event) {
  juce::Point<int> mouse_position = getMouseXYRelative();
  bool left = mouse_position.getX() < getWidth() / 2.f ? true : false;
  if (left) {
    //Button::mouseDown(p_event);
    setToggleState(true, sendNotification);
  } else  {
    //Button::mouseDown(p_event);
    setToggleState(false, sendNotification);
  }
}

void LeftRightButton::resized() {
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
