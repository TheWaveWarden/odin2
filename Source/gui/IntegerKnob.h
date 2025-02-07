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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinKnob.h"

class IntegerKnob : public OdinKnob {
public:
  IntegerKnob(OdinKnob::Type type = OdinKnob::Type::unassigned);
  ~IntegerKnob();

  virtual double snapValue(double attemptedValue, DragMode dragMode){
    return round(attemptedValue);
  }

      private : JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntegerKnob)
};
