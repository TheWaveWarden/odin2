/*
  ==============================================================================

    LFOTableData.h
    Created: 30 May 2019 12:20:22am
    Author:  frot

  ==============================================================================
*/

#pragma once
#include "../../../OdinConstants.h"
const float (*getLFOtableData())[NUMBER_OF_LFOTABLES][1][WAVETABLE_LENGTH];
const float * getOneLFOTable(int p_wavetable);