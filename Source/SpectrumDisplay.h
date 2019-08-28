/*
  ==============================================================================

    SpectrumDisplay.h
    Created: 28 Aug 2019 7:27:05pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "audio/Oscillators/WavetableContainer.h"


#define WTDISPLAY_SIZE_X 512
#define WTDISPLAY_SIZE_Y 200
#define WTDISPLAY_INLAY 6
#define WAVEDISPLAY_THICCNESS 3
#define SPECTRUM_LINE_WIDTH 3



//==============================================================================
/*
*/
class SpectrumDisplay    : public Component
{
public:
    SpectrumDisplay();
    ~SpectrumDisplay();

    void paint (Graphics&) override;
    
    void setVariables(int p_lower, int p_higher, float p_interpol){
      m_lower_wt = p_lower;
      m_higher_wt = p_higher;
      m_interpolation = p_interpol;
      repaint();
    }
    
private:
    int m_lower_wt = 5; //saw
    int m_higher_wt = 2; //ChipSquare50
    float m_interpolation = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectrumDisplay)
};
