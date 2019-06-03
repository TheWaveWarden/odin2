/*
  ==============================================================================

    LFOSelectorComponent.h
    Created: 27 Feb 2019 6:43:15pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFODisplayComponent.h"
#include "GlobalIncludes.h"

//==============================================================================
/*
 */
class LFOSelectorComponent : public Component {
public:
  LFOSelectorComponent();
  ~LFOSelectorComponent();

  void paint(Graphics &) override;
  void resized() override;

  void setValue(int p_value){
    if(p_value < m_display.getNrOfWaves() && p_value >= 0){
      m_value = p_value;
      m_display.setValue(p_value);
      OnValueChange(p_value);
    }
  }
  
  void setTooltip(const String p_text){
    m_display.setTooltip(p_text);
  }

  std::function<void(int)> OnValueChange = [](int){};
private:
  void increment(){
    if(m_value + 1 < m_display.getNrOfWaves()){
      m_display.setValue(++m_value);
      OnValueChange(m_value);
    }
  }
  
  void decrement(){
    if(m_value > 0){
      m_display.setValue(--m_value);
      OnValueChange(m_value);
    }
  }

  LFODisplayComponent m_display;

  DrawableButton m_up;
  DrawableButton m_down;

  int m_value = 0;

      JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOSelectorComponent)
};
