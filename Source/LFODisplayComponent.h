/*
  ==============================================================================

    LFODisplayComponent.h
    Created: 27 Feb 2019 6:43:33pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include <vector>
//==============================================================================
/*
 */
class LFODisplayComponent : public Component, public SettableTooltipClient {
public:
  LFODisplayComponent();
  ~LFODisplayComponent();

  void paint(Graphics &) override;
  void setImage(juce::Image p_panel) { m_panel = p_panel; }
  void addWave(juce::Image p_wave);
  void setInlay(int p_inlay) { m_inlay = p_inlay; }
  int getNrOfWaves() { return m_waves.size(); }
  void setValue(int p_value) {
    m_value = p_value;
    repaint();
  }

  void mouseDown(const MouseEvent &event) override;
  void mouseDrag(const MouseEvent &event) override;
  void mouseUp(const MouseEvent &event) override;
  std::function<void(const MouseEvent&)> toParentMouseDown = [](const MouseEvent&){};
  std::function<void(const MouseEvent&)> toParentMouseDrag = [](const MouseEvent&){};
  std::function<void(const MouseEvent&)> toParentMouseUp = [](const MouseEvent&){};

  void clearWaves(){
    m_waves.clear();
  }

  void setInlayTop(int p_inlay){
    m_inlay_top = p_inlay;
  }

  void setInlayLeft(int p_inlay){
    m_inlay_left = p_inlay;
  }

private:
  juce::Image m_panel;
  std::vector<juce::Image> m_waves;
  int m_inlay = 0;
  int m_value = 0;

  int  m_inlay_top = 0;
  int  m_inlay_left = 0;



  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFODisplayComponent)
};
