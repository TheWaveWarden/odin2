/*
  ==============================================================================

    ADSRComponent.h
    Created: 26 Feb 2019 9:04:59pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableSlider.h"
#include "OdinButton.h"

#define SLIDER_SIZE_X 23
#define SLIDER_SIZE_Y 79
#define SLIDER_POS_X 5
#define SLIDER_POS_Y 2
#define SLIDER_OFFSET 25
#define LOOP_POS_X 24
#define LOOP_POS_Y 82

#define A_LOW_LIMIT 0.001
#define A_HIGH_LIMIT 10
#define A_DEFAULT 0.03
#define A_MID_VALUE 1

#define D_LOW_LIMIT A_LOW_LIMIT
#define D_HIGH_LIMIT A_HIGH_LIMIT
#define D_DEFAULT 1
#define D_MID_VALUE A_MID_VALUE

#define S_LOW_LIMIT 0
#define S_HIGH_LIMIT 1
#define S_DEFAULT 0.5
#define S_MID_VALUE 0.3

#define R_LOW_LIMIT A_LOW_LIMIT
#define R_HIGH_LIMIT A_HIGH_LIMIT
#define R_DEFAULT 0.03
#define R_MID_VALUE A_MID_VALUE
//==============================================================================
/*
 */
class ADSRComponent : public Component {
public:
  ADSRComponent(AudioProcessorValueTreeState& vts, std::string p_adsr_number);
  ~ADSRComponent();

  void paint(Graphics &) override;

private:
  OdinButton m_loop;


  DrawableSlider m_attack;
  DrawableSlider m_decay;
  DrawableSlider m_sustain;
  DrawableSlider m_release;

  std::string m_adsr_number;
  AudioProcessorValueTreeState& m_value_tree;

  std::unique_ptr<SliderAttachment> m_attack_attach;
  std::unique_ptr<SliderAttachment> m_decay_attach;
  std::unique_ptr<SliderAttachment> m_sustain_attach;
  std::unique_ptr<SliderAttachment> m_release_attach;

  std::unique_ptr<ButtonAttachment> m_loop_attach;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};
