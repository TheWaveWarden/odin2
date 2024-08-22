/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
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

  void activate(bool p_active){
    m_is_active = p_active;
  }

private:
  //this is the reason for the entire subclass
  bool m_is_active = false;
};
