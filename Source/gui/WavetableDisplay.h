#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../audio/Oscillators/WavetableContainer.h"


#define WTDISPLAY_SIZE_X 512
#define WTDISPLAY_SIZE_Y 200
#define WTDISPLAY_INLAY 6
#define WAVEDISPLAY_THICCNESS 3

class WavetableDisplay    : public Component
{
public:
    WavetableDisplay();
    ~WavetableDisplay();

    void paint (Graphics&) override;
    
    void setVariables(int p_lower, int p_higher, float p_interpol){
      m_lower_wt = p_lower;
      m_higher_wt = p_higher;
      m_interpolation = p_interpol;
      repaint();
    }

    void togglePosition(){
      if(m_position_left){
        m_position_left = false;
        setTopLeftPosition(getX() + m_position_offset, getY());
      } else {
        m_position_left = true;
        setTopLeftPosition(getX() - m_position_offset, getY());
      }
    }

    void mouseUp(const MouseEvent& e) override{
      togglePosition();
      Component::mouseUp(e);
    }
    
private:
    int m_lower_wt = 5; //saw
    int m_higher_wt = 2; //ChipSquare50
    float m_interpolation = 0;

        //for moving out of the way:
    bool m_position_left = true;
    float m_position_offset = 505;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableDisplay)
};
