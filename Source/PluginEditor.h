/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

#include "ADSRComponent.h"
#include "AmpDistortionFlowComponent.h"
#include "DelayComponent.h"
#include "DragButton.h"
#include "FXButtonsSection.h"
#include "FXComponent.h"
#include "FilterComponent.h"
#include "GlobalIncludes.h"
#include "Knob.h"
#include "LFOComponent.h"
#include "LeftRightButton.h"
#include "ModMatrixComponent.h"
#include "NumberSelector.h"
#include "OscComponent.h"
#include "PitchWheel.h"
#include "SaveLoadComponent.h"
#include "XYSectionComponent.h"

#define GLIDE_POS_X 117
#define GLIDE_POS_Y 522
#define MASTER_POS_X GLIDE_POS_X
#define MASTER_POS_Y 568

#define OSC1_POS_X 29
#define OSC2_POS_X 277
#define OSC3_POS_X 525
#define OSC_POS_Y 29 // todo
#define OSC_SIZE_X 247
#define OSC_SIZE_Y 145

#define FILTER_SIZE_X 247
#define FILTER_SIZE_Y 134
#define FIL1_POS_X 29
#define FIL1_POS_Y 177
#define FIL2_POS_X 525
#define FIL2_POS_Y FIL1_POS_Y
#define FIL3_POS_X OSC2_POS_X
#define FIL3_POS_Y 317

#define DROPDOWN_OSC1_POS_X 245
#define DROPDOWN_OSC2_POS_X 493
#define DROPDOWN_OSC3_POS_X 741
#define DROPDOWN_POS_Y 36

#define DROPDOWN_FILTER1_POS_X 245
#define DROPDOWN_FILTER1_POS_Y 182
#define DROPDOWN_FILTER2_POS_X 741
#define DROPDOWN_FILTER2_POS_Y DROPDOWN_FILTER1_POS_Y
#define DROPDOWN_FILTER3_POS_X DROPDOWN_OSC2_POS_X
#define DROPDOWN_FILTER3_POS_Y 323

#define BUTTON_1_LEFT_POS_X 33
#define BUTTON_1_LEFT_POS_Y 239
#define BUTTON_2_RIGHT_POS_X 742
#define BUTTON_2_RIGHT_POS_Y BUTTON_1_LEFT_POS_Y

#define MIDSECTION_SIZE_X 252
#define MIDSECTION_SIZE_Y 134
#define MIDSECTION_POS_X 275
#define MIDSECTION_POS_Y 175

#define ADSR_SIZE_X 108
#define ADSR_SIZE_Y 96
#define ADSR_LEFT_POS_X 33
#define ADSR_LEFT_POS_Y 330
#define ADSR_RIGHT_POS_X 162
#define ADSR_RIGHT_POS_Y 330

#define FX_BUTTON_X 525
#define FX_BUTTON_Y 310
#define FX_BUTTON_OFFSET 61
#define FX_ON_BUTTON_X 526
#define FX_ON_BUTTON_Y 330

#define LEFT_FX_POS_X 526
#define LEFT_FX_POS_Y 343
#define RIGHT_FX_POS_X 746
#define RIGHT_FX_POS_Y LEFT_FX_POS_Y

#define FX_AREA_POS_X 527
#define FX_AREA_POS_Y 343
#define FX_AREA_SIZE_X 243
#define FX_AREA_SIZE_Y 107

#define MATRIX_SIZE_X 500
#define MATRIX_SIZE_Y 144
#define MATRIX_POS_X 274
#define MATRIX_POS_Y 452

#define SAVE_LOAD_POS_X 27
#define SAVE_LOAD_POS_Y 2
#define SAVE_LOAD_SIZE_X 288
#define SAVE_LOAD_SIZE_Y 25

#define ENV13_POS_X 30
#define ENV13_POS_Y 313
#define ENV24_POS_X 158
#define ENV24_POS_Y ENV13_POS_Y

#define LFO13_POS_X 30
#define LFO13_POS_Y 428
#define LFO24_POS_X 156
#define LFO24_POS_Y LFO13_POS_Y

#define LFO_SIZE_X 116
#define LFO_SIZE_Y 51
#define LFO_RIGHT_POS_X 157
#define LFO_RIGHT_POS_Y 448
#define LFO_LEFT_POS_X 30
#define LFO_LEFT_POS_Y LFO_RIGHT_POS_Y

#define XY_COMPONENT_POS_X 143
#define XY_COMPONENT_POS_Y 502
#define XY_COMPONENT_SIZE_X 123
#define XY_COMPONENT_SIZE_Y 93

#define QUESTION_POS_X 745
#define QUESTION_POS_Y 5

#define PITCHWHEEL_X 42
#define MODWHEEL_X 72
#define WHEEL_Y 497

#define PITCH_AMOUNT_X 35
#define PITCH_AMOUNT_Y 578

#define MASTER_MAX 12
#define MASTER_MIN -20
#define MASTER_DEFAULT 0

#define FONT_SIZE_Y 16
#define FONT_SIZE_X 10
#define FONT_SPACE_Y 4
#define TOOLTIP_INLAY 10


class TooltipFeels : public LookAndFeel_V4 {
public:
  TooltipFeels() {
    setColour(TooltipWindow::ColourIds::textColourId, MENU_FONT_COLOR);
    setColour(TooltipWindow::ColourIds::outlineColourId, MENU_FONT_COLOR);
    setColour(TooltipWindow::ColourIds::backgroundColourId,
              MENU_BACKGROUND_COLOR);
  }
};



//==============================================================================
/**
*/
class OdinAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    OdinAudioProcessorEditor (OdinAudioProcessor&);
    ~OdinAudioProcessorEditor();

    //==============================================================================
   void paint(Graphics &) override;
  void resized() override;
  void arrangeFXOnButtons(std::map<std::string, int> p_map);
  void setActiveFXPanel(std::string p_name);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OdinAudioProcessor& processor;
    
    void setTooltipEnabled(bool p_enabled);
  //==============================================================================
  // Your private member variables go here...
  OdinMenuFeels m_menu_feels;

  Knob m_glide;
  Knob m_master;

  PitchWheel m_pitchwheel;
  Knob m_modwheel;
  NumberSelector m_pitch_amount;

  OscComponent m_osc1;
  OscComponent m_osc2;
  OscComponent m_osc3;

  FilterComponent m_fil1_component;
  FilterComponent m_fil2_component;
  FilterComponent m_fil3_component;

  AmpDistortionFlowComponent m_midsection;

  ADSRComponent m_adsr_1;
  ADSRComponent m_adsr_2;
  ADSRComponent m_adsr_3;
  ADSRComponent m_adsr_4;

  LFOComponent m_lfo_1;
  LFOComponent m_lfo_2;
  LFOComponent m_lfo_3;
  LFOComponent m_lfo_4;

  FXComponent m_flanger;
  FXComponent m_phaser;
  FXComponent m_chorus;
  DelayComponent m_delay;

  ModMatrixComponent m_mod_matrix;

  SaveLoadComponent m_save_load;

  XYSectionComponent m_xy_section;

  juce::PopupMenu m_osc_dropdown_menu;
  juce::DrawableButton m_osc1_dropdown;
  juce::DrawableButton m_osc2_dropdown;
  juce::DrawableButton m_osc3_dropdown;

  juce::PopupMenu m_filter_dropdown_menu;
  juce::DrawableButton m_filter1_dropdown;
  juce::DrawableButton m_filter2_dropdown;
  juce::DrawableButton m_filter3_dropdown;

  juce::DrawableButton m_filleft_button1;
  juce::DrawableButton m_filleft_button2;
  juce::DrawableButton m_filleft_button3;

  juce::DrawableButton m_filright_button1;
  juce::DrawableButton m_filright_button2;
  juce::DrawableButton m_filright_button3;
  juce::DrawableButton m_filright_buttonf1;

  FXButtonsSection m_fx_buttons_section;

  juce::DrawableButton m_flanger_on_button;
  juce::DrawableButton m_phaser_on_button;
  juce::DrawableButton m_chorus_on_button;
  juce::DrawableButton m_delay_on_button;

  juce::DrawableButton m_question_button;

  juce::ComponentDragger m_dragger;

  LeftRightButton m_env_13_button;
  LeftRightButton m_env_24_button;

  LeftRightButton m_lfo_13_button;
  LeftRightButton m_lfo_24_button;

  TooltipWindow m_tooltip;
  TooltipFeels m_tooltip_feels;
  void setOsc1Plate(int p_osc_type);
  void setOsc2Plate(int p_osc_type);
  void setOsc3Plate(int p_osc_type);
  void setFilter1Plate(int p_osc_type);
  void setFilter2Plate(int p_osc_type);
  void setFilter3Plate(int p_osc_type);
  void setEnv13(bool p_env13);
  void setEnv24(bool p_env24);
  void setLfo13(bool p_lfo13);
  void setLfo24(bool p_lfo24);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OdinAudioProcessorEditor)
};
