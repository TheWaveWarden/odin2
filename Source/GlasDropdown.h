/*
  ==============================================================================

    GlasDropdown.h
    Created: 25 Feb 2019 11:24:02pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#define INLAY_DEFAULT 0
//==============================================================================
/*
 */
class GlasDropdown : public juce::ComboBox {
public:
  GlasDropdown();
  ~GlasDropdown();

  void paint(Graphics &) override;
  void resized() override;
  void setImage(juce::Image p_panel) { m_glaspanel = p_panel; }
  void setColor(juce::Colour p_color) { m_color = p_color; }
  void setGreyFirstElement(bool p_grey){m_grey_first_element = p_grey;}
  void setInlay(int p_inlay) { m_inlay = p_inlay; }
  int m_inlay = INLAY_DEFAULT;

  
  void reset(){
    setSelectedId(1, sendNotification);
    repaint();
  }

private:
  bool m_grey_first_element = false;

  juce::Image m_glaspanel;
  juce::Colour m_color;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlasDropdown)

  OdinMenuFeels m_menu_feels;
};
