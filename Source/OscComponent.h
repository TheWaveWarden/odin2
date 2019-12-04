/*
  ==============================================================================

    OscComponent.h
    Created: 25 Feb 2019 12:31:26am
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChipdrawWindow.h"
#include "GlasDropdown.h"
#include "GlobalIncludes.h"
#include "IntegerKnob.h"
#include "Knob.h"
#include "LeftRightButton.h"
#include "NumberSelector.h"
#include "OdinButton.h"
#include "SpecdrawDisplay.h"
#include "Utilities.h"
#include "WavedrawDisplay.h"
#include "WaveformSelectorComponent.h"
#include "XYPadComponent.h"
#include "OdinControlAttachments.h"

//#include "PluginProcessor.h"

#define OCT_POS_X 139
#define SEMI_POS_X 169
#define FINE_POS_X 199
#define PITCH_POS_Y 20
#define VOL_POS_X 228
#define VOL_POS_Y 39
#define PW_POS_X 33
#define PW_POS_Y 86
#define DRIFT_POS_X 157
#define DRIFT_POS_Y 86
#define RESET_POS_X 214
#define RESET_POS_Y 70
#define OSC_SYNC_POS_X RESET_POS_X
#define OSC_SYNC_POS_Y 92
#define LED_POS_X (82 - 29)
#define LED_POS_Y (105 - 29)
#define STEP_1_POS_X 23
#define STEP_1_POS_Y 107
#define STEP_2_POS_X 67
#define STEP_2_POS_Y 107
#define STEP_3_POS_X 110
#define STEP_3_POS_Y 107
#define SPEED_POS_X 159
#define SPEED_POS_Y 93
#define ARP_POS_X 15
#define ARP_POS_Y 80
#define STEP_BUTTON_POS_X 102
#define STEP_BUTTON_POS_Y 88
#define NOISE_POS_X 145
#define NOISE_POS_Y 50
#define WAVE_CHIPTUNE_POS_X 20
#define WAVE_CHIPTUNE_POS_Y 50
#define WAVE_CARRIER_POS_X 8
#define WAVE_CARRIER_POS_Y 8
#define WAVE_MODULATOR_POS_X 130
#define WAVE_MODULATOR_POS_Y 118
#define RATIO_CARRIER_POS_X 8
#define RATIO_CARRIER_POS_Y 33
#define RATIO_MODULATOR_POS_X 193
#define RATIO_MODULATOR_POS_Y 93
#define FM_POS_X 98
#define FM_POS_Y 52
#define FM_EXP_POS_X 8
#define FM_EXP_POS_Y 118

#define CHIPDRAW_POSITION_X 5
#define CHIPDRAW_POSITION_Y 35

#define CONVERT_POS_X 215
#define CONVERT_POS_Y 114

#define HP_POS_X (54 - 8)
#define HP_POS_Y (72 + 6)
#define LP_POS_X (148 - 8)
#define LP_POS_Y (38 + 6)

#define XY_POS_X 7
#define XY_POS_Y 51
#define VEC_WAVE_X 120
#define VEC_WAVE_Y 52
#define VEC_WAVE_OFFSET 21


#define DETUNE_POS_X 100
#define DETUNE_POS_Y 50
#define SPREAD_POS_X 33
#define SPREAD_POS_Y 52
#define POSITION_MULTI_POS_X SPREAD_POS_X
#define POSITION_MULTI_POS_Y 90

#define WAVETABLE_WAVE_X 10
#define WAVETABLE_WAVE_Y 54
#define POS_MODULATION_X 40
#define POS_MODULATION_Y 101
#define POSITION_POS_X 122
#define POSITION_POS_Y 49
#define POS_ENV_POS_X 173
#define POS_ENV_POS_Y 98
#define MULTI_WAVE_X 80
#define MULTI_WAVE_Y 103

#define OCT_RANGE_MAX 3
#define SEMI_RANGE_MAX 7
#define FINE_RANGE_MAX 50
#define PW_DEFAULT 0.5
#define STEP_RANGE_MAX 12
#define STEP_2_DEFAULT 12
#define STEP_3_DEFAULT 7

#define SPEED_MAX 60
#define SPEED_MIN 1
#define SPEED_DEFAULT 20
#define SPEED_MID 15

#define VOL_MAX 12
#define VOL_MIN -20
#define VOL_DEFAULT 0

#define HP_MIN 80
#define HP_MAX 18000
#define HP_DEFAULT 80
#define HP_MID 2000

#define LP_MIN 80
#define LP_MAX 18000
#define LP_DEFAULT 18000
#define LP_MID 2000

#define X_POS_X 30
#define X_POS_Y 27
#define Y_POS_X 75
#define Y_POS_Y X_POS_Y

//#define FM_COLOR Colour(90, 40, 40)
#define FM_COLOR Colour(93, 41, 41)
#define PM_COLOR Colour(40, 40, 40)
//==============================================================================
/*
 */

class OscComponent : public Component {
public:
  OscComponent(OdinAudioProcessor &p_processor,
               AudioProcessorValueTreeState &vts, std::string p_osc_number);
  ~OscComponent();

  void paint(Graphics &) override;
  void setBackground(juce::Image p_background) { m_background = p_background; }
  void setBackgroundBypass(juce::Image p_background) {
    m_background_bypass = p_background;
  }
  int getOscType() { return m_osc_type; }
  void setOscType(int p_osc_type);
  void setOscTypeBypass();
  void setOscTypeAnalog();
  void setOscTypeChiptune();
  void setOscTypeFM();
  void setOscTypePM();
  void setOscTypeChipdraw();
  void setOscTypeWavedraw();
  void setOscTypeSpecdraw();
  void setOscTypeNoise();
  void setOscTypeVector();
  void setOscTypeWavetable();
  void setOscTypeMulti();

  void hideAllComponents();

  void showVolComponent();
  void showPitchComponents();
  void showAnalogComponents();
  void showChiptuneComponents();
  void showFMComponents();
  void showPMComponents();
  void showChipdrawComponents();
  void showWavedrawComponents();
  void showSpecdrawComponents();
  void showNoiseComponents();
  void showVectorComponents();
  void showWavetableComponents();
  void showMultiComponents();

#ifdef WTGEN
  void writeWavedrawTableToFile();
  void writeChipdrawTableToFile();
  void writeSpecdrawTableToFile();
#endif

  void createWavedrawTables();
  void createChipdrawTables();
  void createSpecdrawTables();

  void forceValueTreeOntoComponents(ValueTree p_tree, int p_index,
                                    bool p_create_wavetables);

  void resetVectorWaves();

protected:
  // TextEditor REMOVE_EDITOR;

  int m_osc_type = 0;

  juce::Image m_background;
  juce::Image m_background_bypass;

  IntegerKnob m_oct;
  IntegerKnob m_semi;
  Knob m_fine;
  Knob m_vol;
  Knob m_pw;
  Knob m_drift;
  IntegerKnob m_step_1;
  IntegerKnob m_step_2;
  IntegerKnob m_step_3;
  Knob m_fm;
  Knob m_speed;
  Knob m_lp;
  Knob m_hp;
  Knob m_position;
  Knob m_position_multi;
  Knob m_spread;
  Knob m_detune;
  Knob m_pos_mod;

  OdinButton m_arp;
  OdinButton m_step_button;
  OdinButton m_noise;
  OdinButton m_chipdraw_convert;
  OdinButton m_wavedraw_convert;

  // OdinButton m_wavedraw_convert_REMOVE;
  // OdinButton m_specdraw_convert_REMOVE;
  // OdinButton m_chipdraw_convert_REMOVE;

  OdinButton m_specdraw_convert;
  DrawableButton m_LED_saw;
  DrawableButton m_LED_pulse;
  DrawableButton m_LED_triangle;
  DrawableButton m_LED_sine;

  OdinButton m_reset;
  OdinButton m_sync;

  WaveformSelectorComponent m_chiptune_waveselector;
  WaveformSelectorComponent m_carrier_waveselector;
  WaveformSelectorComponent m_modulator_waveselector;
  WaveformSelectorComponent m_wavetable_waveselector;
  WaveformSelectorComponent m_modulation_source;

  NumberSelector m_carrier_ratio;
  NumberSelector m_modulator_ratio;

  LeftRightButton m_fm_exp;

  ChipdrawWindow m_chipdraw;
  WavedrawDisplay m_wavedraw;
  SpecdrawDisplay m_specdraw;

  XYPadComponent m_xy;
  Knob m_xy_x;
  Knob m_xy_y;
  GlasDropdown m_vec_a;
  GlasDropdown m_vec_b;
  GlasDropdown m_vec_c;
  GlasDropdown m_vec_d;

  std::string m_osc_number;
  AudioProcessorValueTreeState &m_value_tree;

  std::unique_ptr<OdinKnobAttachment> m_oct_attach;
  std::unique_ptr<OdinKnobAttachment> m_semi_attach;
  std::unique_ptr<OdinKnobAttachment> m_fine_attach;
  std::unique_ptr<OdinKnobAttachment> m_vol_attach;
  std::unique_ptr<OdinKnobAttachment> m_position_attach;
  std::unique_ptr<OdinKnobAttachment> m_detune_attach;
  std::unique_ptr<OdinKnobAttachment> m_multi_position_attach;
  std::unique_ptr<OdinKnobAttachment> m_spread_attach;
  std::unique_ptr<OdinKnobAttachment> m_pulsewidth_attach;
  std::unique_ptr<OdinKnobAttachment> m_drift_attach;
  std::unique_ptr<OdinKnobAttachment> m_arp_speed_attach;
  std::unique_ptr<OdinKnobAttachment> m_step_1_attach;
  std::unique_ptr<OdinKnobAttachment> m_step_2_attach;
  std::unique_ptr<OdinKnobAttachment> m_step_3_attach;
  std::unique_ptr<OdinKnobAttachment> m_fm_attach;
  std::unique_ptr<OdinKnobAttachment> m_lp_attach;
  std::unique_ptr<OdinKnobAttachment> m_hp_attach;
  std::unique_ptr<OdinKnobAttachment> m_x_attach;
  std::unique_ptr<OdinKnobAttachment> m_y_attach;
  std::unique_ptr<OdinKnobAttachment> m_pos_mod_attach;

  std::unique_ptr<OdinButtonAttachment> m_reset_attach;
  std::unique_ptr<OdinButtonAttachment> m_sync_attach;
  std::unique_ptr<OdinButtonAttachment> m_arp_on_attach;
  std::unique_ptr<OdinButtonAttachment> m_step_3_on_attach;
  std::unique_ptr<OdinButtonAttachment> m_chipnoise_attach;
  std::unique_ptr<OdinButtonAttachment> m_exp_fm_attach;

  // std::unique_ptr<ComboBoxAttachment> m_vec_a_attach;
  // std::unique_ptr<ComboBoxAttachment> m_vec_b_attach;
  // std::unique_ptr<ComboBoxAttachment> m_vec_c_attach;
  // std::unique_ptr<ComboBoxAttachment> m_vec_d_attach;

  Identifier m_wavetable_identifier;
  Identifier m_modulation_source_identifier;
  Identifier m_chipwave_identifier;
  Identifier m_modulator_wave_identifier;
  Identifier m_carrier_wave_identifier;
  Identifier m_modulator_ratio_identifier;
  Identifier m_carrier_ratio_identifier;
  Identifier m_analog_wave_identifier;
  Identifier m_vec_a_identifier;
  Identifier m_vec_b_identifier;
  Identifier m_vec_c_identifier;
  Identifier m_vec_d_identifier;
  Identifier m_pos_mod_identifier;

  OdinMenuFeels m_menu_feels;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscComponent)
};
