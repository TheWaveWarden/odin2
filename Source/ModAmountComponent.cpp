/*
  ==============================================================================

    ModAmountComponent.cpp
    Created: 3 Mar 2019 2:02:02am
    Author:  frot

  ==============================================================================
*/

#include "ModAmountComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream

//==============================================================================
ModAmountComponent::ModAmountComponent() {
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  // setDoubleClickReturnValue(true, 0, ModifierKeys::ctrlModifier);
  
}

ModAmountComponent::~ModAmountComponent() {}

void ModAmountComponent::paint(Graphics &g) {
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
  g.fillRect(juce::Rectangle<int>(top_left, bottom_right)); // pmai

  if (m_value > 0) {
    g.setColour(m_color_bar);
    bottom_right.addXY(-(getWidth() - m_inlay * 2) * (1.f - m_value), -m_inlay);
    g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
  } else {
    g.setColour(m_color_bar);
    top_left.addXY((getWidth() - m_inlay * 2) * (1 +m_value), m_inlay);
    g.fillRect(juce::Rectangle<int>(top_left, bottom_right));
  }

  g.setFont(12.0f);
  std::stringstream stream;
  stream << std::fixed << std::setprecision(0) << m_value * 100;
  std::string value_string = stream.str();
  g.setColour(Colours::white);
  //if(value_string == "0"){
  //  g.setColour(juce::Colours::grey);
  //}
  g.drawText(value_string, getLocalBounds(), Justification::centred,
             true); 

  g.drawImageAt(m_glas_panel, 0, 0);
}

void ModAmountComponent::resized() {
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void ModAmountComponent::mouseDrag(const MouseEvent &event) {
  m_value = m_drag_start_value +
            (m_drag_start_y - getMouseXYRelative().getY()) * DRAG_SCALAR;
  m_value = m_value > 1 ? 1 : m_value;
  m_value = m_value < -1 ? -1 : m_value;
  onValueChange(m_value);
  repaint();
}
