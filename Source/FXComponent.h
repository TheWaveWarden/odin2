/*
  ==============================================================================

    FXComponent.h
    Created: 27 Feb 2019 12:02:15am
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "OdinButton.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include "Knob.h"
#include "SyncTimeSelector.h"

#define SPACING 56


#define FX_FREQ_POS_X 20
#define FX_FREQ_POS_Y 15
#define FX_AMOUNT_POS_X FX_FREQ_POS_X + 1 * SPACING
#define FX_AMOUNT_POS_Y 52
#define FX_FEEDBACK_POS_X FX_FREQ_POS_X + 2 * SPACING
#define FX_FEEDBACK_POS_Y FX_FREQ_POS_Y
#define FX_DRY_WET_POS_X FX_FREQ_POS_X + 3 * SPACING
#define FX_DRY_WET_POS_Y FX_AMOUNT_POS_Y
#define FX_SYNC_POS_X 24
#define FX_SYNC_POS_Y 70
#define FX_RESET_POS_X 129
#define FX_RESET_POS_Y FX_SYNC_POS_Y
#define FX_SYNC_TIME_FX_POS_X 5
#define FX_SYNC_TIME_FX_POS_Y 20

#define FX_FREQ_MIN 0.05
#define FX_FREQ_MAX 20
#define FX_FREQ_MID 0.8
#define FX_FREQ_DEFAULT 1
#define FX_AMOUNT_DEFAULT 0.3

#define FX_DRYWET_DEFAULT 1

//==============================================================================
/*
 */
class FXComponent : public Component {
public:
  FXComponent(AudioProcessorValueTreeState &vts, std::string p_fx_name, bool p_is_standalone);
  ~FXComponent();

  void forceValueTreeOntoComponents(ValueTree p_tree);
  

  void paint(Graphics &) override;
  void resized() override;
  
  void setSyncTimeColor(juce::Colour p_color){
    m_sync_time.setColor(p_color);
  }

  void setImage(juce::Image p_background, bool p_sync) {
    if (p_sync) {
      m_background_sync = p_background;
    } else {
      m_background_no_sync = p_background;
    }
  }

  void setSyncEnabled(bool p_sync);

private:
  bool m_is_standalone_plugin;

  Knob m_rate;
  Knob m_amount;
  Knob m_feedback;
  Knob m_dry_wet;

  OdinButton m_sync;
  OdinButton m_reset;

  juce::Image m_background_sync;
  juce::Image m_background_no_sync;

  SyncTimeSelector m_sync_time;

  bool m_sync_enabled = false;

  std::string m_fx_name;
  AudioProcessorValueTreeState &m_value_tree;

  std::unique_ptr<SliderAttachment> m_rate_attach;
  std::unique_ptr<SliderAttachment> m_amount_attach;
  std::unique_ptr<SliderAttachment> m_drywet_attach;
  std::unique_ptr<SliderAttachment> m_feedback_attach;

  std::unique_ptr<ButtonAttachment> m_sync_attach;
  std::unique_ptr<ButtonAttachment> m_reset_attach;

  Identifier m_fx_synctime_numerator_identifier;
  Identifier m_fx_synctime_denominator_identifier;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXComponent)
};
