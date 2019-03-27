/*
  ==============================================================================

    GlasDisplay.h
    Created: 27 Feb 2019 11:56:53am
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#define INLAY_DEF 1

//==============================================================================
/*
 */
class GlasDisplay : public SettableTooltipClient, public Component {
public:
  GlasDisplay();
  ~GlasDisplay();

  void paint(Graphics &) override;
  void resized() override;
  void setImage(juce::Image p_panel){
    m_glas_panel = p_panel;
  } 
  void setText(std::string p_text){
    m_text = p_text;
    repaint();
  }
  void setInlay(int p_inlay){m_inlay = p_inlay;}

  void setColor(juce::Colour p_color){
    m_color = p_color;
  }

  void 	mouseDown (const MouseEvent &event) override;
  std::function<void()> onMouseDown = [](){};//overwriteable with lambda
private:
  juce::Colour m_color = juce::Colours::black;
  int m_inlay = INLAY_DEF;
  juce::Image m_glas_panel;
  std::string m_text = "text";
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlasDisplay)

  OdinMenuFeels m_menu_feels;
};
