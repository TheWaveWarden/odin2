/*
  ==============================================================================

    LFOComponent.h
    Created: 27 Feb 2019 5:33:38pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Knob.h"
#include "LFOSelectorComponent.h"
#include "GlobalIncludes.h"
#include "OdinButton.h"
#include "SyncTimeSelector.h"
#include "OdinControlAttachments.h"


#define LFO_FREQ_POS_X 74
#define LFO_FREQ_POS_Y 21

#define LFO_RESET_POS_X 63
#define LFO_RESET_POS_Y 1
#define SYNC_POS_X 2
#define SYNC_POS_Y 25

#define SELECTOR_POS_X 0
#define SELECTOR_POS_Y 1

#define OVERDRAW_POS_X 40
#define OVERDRAW_POS_Y 20
#define OVERDRAW_SIZE_X 70
#define OVERDRAW_SIZE_Y 40

#define SYNC_TIME_POS_X 38
#define SYNC_TIME_POS_Y 25

#define LFO_FREQ_MIN 0.05
#define LFO_FREQ_MAX 100
#define LFO_FREQ_DEFAULT 1
#define LFO_FREQ_MID 3

//==============================================================================
/*
 */
class LFOComponent : public Component {
public:
  LFOComponent(AudioProcessorValueTreeState& vts, std::string p_lfo_number, bool p_is_standalone);
  ~LFOComponent();

  void forceValueTreeOntoComponents(ValueTree p_tree);
  

  void paint(Graphics &) override;
  void setSync(bool p_sync){
    if(m_sync_active != p_sync){
      m_sync_active = p_sync;
      if(m_sync_active){
        m_freq.setVisible(false);
        m_sync_time.setVisible(true);
      } else {
        m_freq.setVisible(true);
        m_sync_time.setVisible(false);        
      }
      repaint();
    }
  }

  void setSyncOverdraw(juce::Image p_image){
    m_freq_overdraw = p_image;
  }

  bool setGUIBig();  
  bool setGUISmall();
private:

  bool m_GUI_big = false;

  void toggleSync(){
    setSync(!m_sync_active);
  }

  LFOSelectorComponent m_selector;
  SyncTimeSelector m_sync_time;

  Knob m_freq;

  OdinButton m_reset;
  OdinButton m_sync;

  juce::Image m_freq_overdraw;
  bool m_sync_active = false;

  std::string m_lfo_number;
  AudioProcessorValueTreeState& m_value_tree;

  std::unique_ptr<OdinKnobAttachment> m_freq_attach;

  //std::unique_ptr<OdinButtonAttachment> m_sync_attach;
  std::unique_ptr<OdinButtonAttachment> m_reset_attach;

  Identifier m_lfo_wave_identifier;
  Identifier m_lfo_synctime_numerator_identifier;
  Identifier m_lfo_synctime_denominator_identifier;

  bool m_is_standalone_plugin;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOComponent)
};
