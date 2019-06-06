/*
  ==============================================================================

    OdinTooltipWindow.h
    Created: 6 Jun 2019 5:24:16pm
    Author:  frederik_siepe

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class OdinTooltipWindow : public TooltipWindow
{

public:

  //forward constructor, no idea what it's doing...
  OdinTooltipWindow(std::nullptr_t p_ptr, int p_int) : TooltipWindow(p_ptr, p_int){}

  virtual String getTipFor(Component &p_component)
  {
    if(m_is_active){
      return TooltipWindow::getTipFor(p_component);
    }
    return "";
  }

  void activate(){
    m_is_active = true;
  }

private:
  //this is the reason for the entire subclass
  bool m_is_active = false;
};
