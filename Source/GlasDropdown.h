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
#include <functional>
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
  void setDefaultText(std::string p_text){m_default_text = p_text;}
  int m_inlay = INLAY_DEFAULT;

  void mouseDown(const MouseEvent &event) {
    rearrangeMenu();
    ComboBox::mouseDown(event);
  }


  void setValue(int p_value){
    setSelectedId(p_value, sendNotification);
    repaint();
  }
  
  void reset(){
    setSelectedId(0, sendNotification);
    repaint();
  }

  std::function<void()> rearrangeMenu = [](){};
private:

  std::string m_default_text = "undefined";

  bool m_grey_first_element = false;

  juce::Image m_glaspanel;
  juce::Colour m_color;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlasDropdown)

  OdinMenuFeels m_menu_feels;
};
