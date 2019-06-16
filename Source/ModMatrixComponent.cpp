/*
  ==============================================================================

    ModMatrixComponent.cpp
    Created: 27 Feb 2019 1:27:55am
    Author:  frot

  ==============================================================================
*/

#include "ModMatrixComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
ModMatrixComponent::ModMatrixComponent(AudioProcessorValueTreeState &vts)
    : m_clear_button0("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button1("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button2("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button3("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button4("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button5("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button6("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button7("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button8("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_value_tree(vts), m_amount_1_identifier0("amount_1_[0]"),
      m_amount_2_identifier0("amount_2_[0]"),
      m_amount_3_identifier0("amount_3_[0]"),
      m_amount_1_identifier1("amount_1_[1]"),
      m_amount_2_identifier1("amount_2_[1]"),
      m_amount_3_identifier1("amount_3_[1]"),
      m_amount_1_identifier2("amount_1_[2]"),
      m_amount_2_identifier2("amount_2_[2]"),
      m_amount_3_identifier2("amount_3_[2]"),
      m_amount_1_identifier3("amount_1_[3]"),
      m_amount_2_identifier3("amount_2_[3]"),
      m_amount_3_identifier3("amount_3_[3]"),
      m_amount_1_identifier4("amount_1_[4]"),
      m_amount_2_identifier4("amount_2_[4]"),
      m_amount_3_identifier4("amount_3_[4]"),
      m_amount_1_identifier5("amount_1_[5]"),
      m_amount_2_identifier5("amount_2_[5]"),
      m_amount_3_identifier5("amount_3_[5]"),
      m_amount_1_identifier6("amount_1_[6]"),
      m_amount_2_identifier6("amount_2_[6]"),
      m_amount_3_identifier6("amount_3_[6]"),
      m_amount_1_identifier7("amount_1_[7]"),
      m_amount_2_identifier7("amount_2_[7]"),
      m_amount_3_identifier7("amount_3_[7]"),
      m_amount_1_identifier8("amount_1_[8]"),
      m_amount_2_identifier8("amount_2_[8]"),
      m_amount_3_identifier8("amount_3_[8]"),
      m_source_identifier0("source_[0]"), m_source_identifier1("source_[1]"),
      m_source_identifier2("source_[2]"), m_source_identifier3("source_[3]"),
      m_source_identifier4("source_[4]"), m_source_identifier5("source_[5]"),
      m_source_identifier6("source_[6]"), m_source_identifier7("source_[7]"),
      m_source_identifier8("source_[8]"), m_dest_1_identifier0("dest_1_[0]"),
      m_dest_1_identifier1("dest_1_[1]"), m_dest_1_identifier2("dest_1_[2]"),
      m_dest_1_identifier3("dest_1_[3]"), m_dest_1_identifier4("dest_1_[4]"),
      m_dest_1_identifier5("dest_1_[5]"), m_dest_1_identifier6("dest_1_[6]"),
      m_dest_1_identifier7("dest_1_[7]"), m_dest_1_identifier8("dest_1_[8]"),
      m_dest_2_identifier0("dest_2_[0]"), m_dest_2_identifier1("dest_2_[1]"),
      m_dest_2_identifier2("dest_2_[2]"), m_dest_2_identifier3("dest_2_[3]"),
      m_dest_2_identifier4("dest_2_[4]"), m_dest_2_identifier5("dest_2_[5]"),
      m_dest_2_identifier6("dest_2_[6]"), m_dest_2_identifier7("dest_2_[7]"),
      m_dest_2_identifier8("dest_2_[8]"), m_scale_identifier0("scale_[0]"),
      m_scale_identifier1("scale_[2]"), m_scale_identifier2("scale_[2]"),
      m_scale_identifier3("scale_[3]"), m_scale_identifier4("scale_[4]"),
      m_scale_identifier5("scale_[5]"), m_scale_identifier6("scale_[6]"),
      m_scale_identifier7("scale_[7]"), m_scale_identifier8("scale_[8]") {

  // create submenus to be inserted on demand
  for (int osc = 0; osc < 3; ++osc) {
    // analog oscs
    m_analog_osc_menu[osc].addItem(
        100 * osc + 1, "Osc" + std::to_string(osc + 1) + "Pitch Exp");
    m_analog_osc_menu[osc].addItem(
        100 * osc + 2, "Osc" + std::to_string(osc + 1) + " Pitch Lin");
    m_analog_osc_menu[osc].addItem(100 * osc + 3,
                                   "Osc" + std::to_string(osc + 1) + " Volume");
    m_analog_osc_menu[osc].addItem(
        100 * osc + 10, "Osc" + std::to_string(osc + 1) + " Pulse Width");

    // wavetable oscs
    m_wavetable_osc_menu[osc].addItem(
        100 * osc + 1, "Osc" + std::to_string(osc + 1) + " Pitch Exp");
    m_wavetable_osc_menu[osc].addItem(
        100 * osc + 2, "Osc" + std::to_string(osc + 1) + " Pitch Lin");
    m_wavetable_osc_menu[osc].addItem(
        100 * osc + 3, "Osc" + std::to_string(osc + 1) + " Volume");
    m_wavetable_osc_menu[osc].addItem(
        100 * osc + 20, "Osc" + std::to_string(osc + 1) + " Position");

    // multi oscs
    m_multi_osc_menu[osc].addItem(
        100 * osc + 1, "Osc" + std::to_string(osc + 1) + " Pitch Exp");
    m_multi_osc_menu[osc].addItem(
        100 * osc + 2, "Osc" + std::to_string(osc + 1) + " Pitch Lin");
    m_multi_osc_menu[osc].addItem(100 * osc + 3,
                                  "Osc" + std::to_string(osc + 1) + " Volume");
    m_multi_osc_menu[osc].addItem(100 * osc + 21,
                                  "Osc" + std::to_string(osc + 1) + " Detune");
    m_multi_osc_menu[osc].addItem(
        100 * osc + 20, "Osc" + std::to_string(osc + 1) + " Position");
    m_multi_osc_menu[osc].addItem(100 * osc + 22,
                                  "Osc" + std::to_string(osc + 1) + " Spread");

    // vector oscs
    m_vector_osc_menu[osc].addItem(
        100 * osc + 1, "Osc" + std::to_string(osc + 1) + " Pitch Exp");
    m_vector_osc_menu[osc].addItem(
        100 * osc + 2, "Osc" + std::to_string(osc + 1) + " Pitch Lin");
    m_vector_osc_menu[osc].addItem(100 * osc + 3,
                                   "Osc" + std::to_string(osc + 1) + " Volume");
    m_vector_osc_menu[osc].addItem(100 * osc + 30,
                                   "Osc" + std::to_string(osc + 1) + " X");
    m_vector_osc_menu[osc].addItem(100 * osc + 31,
                                   "Osc" + std::to_string(osc + 1) + " Y");

    // chiptune
    m_chiptune_osc_menu[osc].addItem(
        100 * osc + 1, "Osc" + std::to_string(osc + 1) + " Pitch Exp");
    m_chiptune_osc_menu[osc].addItem(
        100 * osc + 2, "Osc" + std::to_string(osc + 1) + " Pitch Lin");
    m_chiptune_osc_menu[osc].addItem(
        100 * osc + 3, "Osc" + std::to_string(osc + 1) + " Volume");
    m_chiptune_osc_menu[osc].addItem(
        100 * osc + 40, "Osc" + std::to_string(osc + 1) + " Arp Speed");

    // fm
    m_fm_osc_menu[osc].addItem(100 * osc + 1,
                               "Osc" + std::to_string(osc + 1) + " Pitch Exp");
    m_fm_osc_menu[osc].addItem(100 * osc + 2,
                               "Osc" + std::to_string(osc + 1) + " Pitch Lin");
    m_fm_osc_menu[osc].addItem(100 * osc + 3,
                               "Osc" + std::to_string(osc + 1) + " Volume");
    m_fm_osc_menu[osc].addItem(100 * osc + 50,
                               "Osc" + std::to_string(osc + 1) + " FM Amount");
    m_fm_osc_menu[osc].addItem(100 * osc + 51, "Osc" + std::to_string(osc + 1) +
                                                   " Carrier Ratio");
    m_fm_osc_menu[osc].addItem(100 * osc + 52, "Osc" + std::to_string(osc + 1) +
                                                   " Modulator Ratio");

    // noise
    m_noise_osc_menu[osc].addItem(
        100 * osc + 60, "Osc" + std::to_string(osc + 1) + " LP Frequency");
    m_noise_osc_menu[osc].addItem(
        100 * osc + 61, "Osc" + std::to_string(osc + 1) + " HP Frequency");

    // draw
    m_draw_osc_menu[osc].addItem(
        100 * osc + 1, "Osc" + std::to_string(osc + 1) + " Pitch Exp");
    m_draw_osc_menu[osc].addItem(
        100 * osc + 2, "Osc" + std::to_string(osc + 1) + " Pitch Lin");
    m_draw_osc_menu[osc].addItem(100 * osc + 3,
                                 "Osc" + std::to_string(osc + 1) + " Volume");
  }
  for (int fil = 0; fil < 3; ++fil) {

    // standard
    m_standard_fil_menu[fil].addItem(
        300 + 100 * fil + 1, "Filter" + std::to_string(fil + 1) + " Frequency");
    m_standard_fil_menu[fil].addItem(
        300 + 100 * fil + 2, "Filter" + std::to_string(fil + 1) + " Resonance");
    m_standard_fil_menu[fil].addItem(
        300 + 100 * fil + 3, "Filter" + std::to_string(fil + 1) + " Gain");
    m_standard_fil_menu[fil].addItem(300 + 100 * fil + 4,
                                     "Filter" + std::to_string(fil + 1) +
                                         " Env Amount");
    m_standard_fil_menu[fil].addItem(300 + 100 * fil + 5,
                                     "Filter" + std::to_string(fil + 1) +
                                         " Vel Amount");
    m_standard_fil_menu[fil].addItem(300 + 100 * fil + 6,
                                     "Filter" + std::to_string(fil + 1) +
                                         " Kbd Amount");
    m_standard_fil_menu[fil].addItem(300 + 100 * fil + 7,
                                     "Filter" + std::to_string(fil + 1) +
                                         " Saturation");

    // comb
    m_comb_fil_menu[fil].addItem(
        300 + 100 * fil + 1, "Filter" + std::to_string(fil + 1) + " Frequency");
    m_comb_fil_menu[fil].addItem(
        300 + 100 * fil + 2, "Filter" + std::to_string(fil + 1) + " Resonance");
    m_comb_fil_menu[fil].addItem(300 + 100 * fil + 3,
                                 "Filter" + std::to_string(fil + 1) + " Gain");
    m_comb_fil_menu[fil].addItem(300 + 100 * fil + 4,
                                 "Filter" + std::to_string(fil + 1) +
                                     " Env Amount");
    m_comb_fil_menu[fil].addItem(300 + 100 * fil + 5,
                                 "Filter" + std::to_string(fil + 1) +
                                     " Vel Amount");
    m_comb_fil_menu[fil].addItem(300 + 100 * fil + 6,
                                 "Filter" + std::to_string(fil + 1) +
                                     " Kbd Amount");

    // SEM
    m_SEM_fil_menu[fil].addItem(
        300 + 100 * fil + 1, "Filter" + std::to_string(fil + 1) + " Frequency");
    m_SEM_fil_menu[fil].addItem(
        300 + 100 * fil + 2, "Filter" + std::to_string(fil + 1) + " Resonance");
    m_SEM_fil_menu[fil].addItem(300 + 100 * fil + 3,
                                "Filter" + std::to_string(fil + 1) + " Gain");
    m_SEM_fil_menu[fil].addItem(300 + 100 * fil + 4,
                                "Filter" + std::to_string(fil + 1) +
                                    " Env Amount");
    m_SEM_fil_menu[fil].addItem(300 + 100 * fil + 5,
                                "Filter" + std::to_string(fil + 1) +
                                    " Vel Amount");
    m_SEM_fil_menu[fil].addItem(300 + 100 * fil + 6,
                                "Filter" + std::to_string(fil + 1) +
                                    " Kbd Amount");
    m_SEM_fil_menu[fil].addItem(300 + 100 * fil + 7,
                                "Filter" + std::to_string(fil + 1) +
                                    " Saturation");
    m_SEM_fil_menu[fil].addItem(300 + 100 * fil + 10,
                                "Filter" + std::to_string(fil + 1) +
                                    " SEM Transition");

    // formant
    m_formant_fil_menu[fil].addItem(
        300 + 100 * fil + 3, "Filter" + std::to_string(fil + 1) + " Gain");
    m_formant_fil_menu[fil].addItem(300 + 100 * fil + 4,
                                    "Filter" + std::to_string(fil + 1) +
                                        " Env Amount");
    m_formant_fil_menu[fil].addItem(300 + 100 * fil + 5,
                                    "Filter" + std::to_string(fil + 1) +
                                        " Vel Amount");
    m_formant_fil_menu[fil].addItem(300 + 100 * fil + 20,
                                    "Filter" + std::to_string(fil + 1) +
                                        " Formant Transition");
  }

  for (int mod = 0; mod < 4; ++mod) {

    std::string env_name;
    switch (mod) {
    case 0:
      env_name = "Amp Env";
      break;
    case 1:
      env_name = "Filter Env";
      break;
    case 2:
      env_name = "Mod Env";
      break;
    case 3:
      env_name = "Global Env";
      break;
    }

    m_adsr_menu[mod].addItem(600 + 10 * mod + 1, env_name + " Attack");
    m_adsr_menu[mod].addItem(600 + 10 * mod + 2, env_name + " Decay");
    m_adsr_menu[mod].addItem(600 + 10 * mod + 3, env_name + " Sustain");
    m_adsr_menu[mod].addItem(600 + 10 * mod + 4, env_name + " Release");

    if (mod < 3) {
      m_lfo_menu[mod].addItem(650 + 10 * mod + 1,
                              "LFO" + std::to_string(mod + 1) + " Freq");
    } else {
      m_lfo_menu[mod].addItem(650 + 10 * mod + 1, "Global LFO Freq");
    }
  }

  m_delay_menu.addItem(701, "Delay Time");
  m_delay_menu.addItem(702, "Delay Feedback");
  m_delay_menu.addItem(703, "Delay HP Freq");
  m_delay_menu.addItem(704, "Delay Dry");
  m_delay_menu.addItem(705, "Delay Wet");

  m_phaser_menu.addItem(751, "Phaser Rate");
  m_phaser_menu.addItem(752, "Phaser Amount");
  m_phaser_menu.addItem(753, "Phaser Freq");
  m_phaser_menu.addItem(754, "Phaser Feedback");
  m_phaser_menu.addItem(755, "Phaser DryWet");

  m_flanger_menu.addItem(801, "Flanger Freq");
  m_flanger_menu.addItem(802, "Flanger Amount");
  m_flanger_menu.addItem(803, "Flanger Feedback");
  m_flanger_menu.addItem(804, "Flanger DryWet");

  m_chorus_menu.addItem(851, "Chorus Freq");
  m_chorus_menu.addItem(852, "Chorus Amount");
  m_chorus_menu.addItem(853, "Chorus Feedback");
  m_chorus_menu.addItem(854, "Chorus DryWet");

  m_amp_menu.addItem(900, "Amp Gain");
  m_amp_menu.addItem(901, "Amp Pan");
  m_amp_menu.addItem(902, "Amp Velocity");

  m_distortion_menu.addItem(950, "Distortion Threshold");
  m_distortion_menu.addItem(951, "Distortion DryWet");

  // m_sources_menu.addItem(0, "Disabled");
  // m_sources_menu.addSeparator();
  m_sources_menu.addItem(100, "Oscillator 1");
  m_sources_menu.addItem(101, "Oscillator 2");
  m_sources_menu.addItem(102, "Oscillator 3");
  m_sources_menu.addItem(110, "Filter 1 Out");
  m_sources_menu.addItem(111, "Filter 2 Out");
  m_sources_menu.addSeparator();
  m_sources_menu.addItem(200, "Amp Envelope");
  m_sources_menu.addItem(201, "Filter Envelope");
  m_sources_menu.addItem(202, "Mod Envelope");
  m_sources_menu.addItem(203, "Global Envelope");
  m_sources_menu.addSeparator();
  m_sources_menu.addItem(300, "LFO 1");
  m_sources_menu.addItem(301, "LFO 2");
  m_sources_menu.addItem(302, "LFO 3");
  m_sources_menu.addItem(303, "Global LFO");
  m_sources_menu.addSeparator();
  m_sources_menu.addItem(400, "X");
  m_sources_menu.addItem(401, "Y");
  m_sources_menu.addSeparator();
  m_sources_menu.addItem(402, "ModWheel");
  m_sources_menu.addItem(403, "PitchBend");
  m_sources_menu.addItem(404, "MIDI note");
  m_sources_menu.addItem(405, "MIDI velocity");
  m_sources_menu.addItem(406, "MIDI aftertouch");
  m_sources_menu.addSeparator();
  m_sources_menu.addItem(999, "Random");
  m_sources_menu.addItem(1000, "Constant");
  // 1000 must be highest value

  juce::Image glas_left_down = ImageCache::getFromMemory(
      BinaryData::modpanelbig_left_down_png, BinaryData::modpanelbig_left_down_pngSize);
  juce::Image glas_left_mid = ImageCache::getFromMemory(
      BinaryData::modpanelbig_left_mid_png, BinaryData::modpanelbig_left_mid_pngSize);
  juce::Image glas_left_up = ImageCache::getFromMemory(
      BinaryData::modpanelbig_left_up_png, BinaryData::modpanelbig_left_up_pngSize);
  juce::Image glas_mid_down = ImageCache::getFromMemory(
      BinaryData::modpanelbig_mid_down_png, BinaryData::modpanelbig_mid_down_pngSize);
  juce::Image glas_mid_mid = ImageCache::getFromMemory(
      BinaryData::modpanelbig_mid_mid_png, BinaryData::modpanelbig_mid_mid_pngSize);
  juce::Image glas_mid_up = ImageCache::getFromMemory(
      BinaryData::modpanelbig_mid_up_png, BinaryData::modpanelbig_mid_up_pngSize);
  juce::Image glas_right_down = ImageCache::getFromMemory(
      BinaryData::modpanelbig_right_down_png, BinaryData::modpanelbig_right_down_pngSize);
  juce::Image glas_right_mid = ImageCache::getFromMemory(
      BinaryData::modpanelbig_right_mid_png, BinaryData::modpanelbig_right_mid_pngSize);
  juce::Image glas_right_up = ImageCache::getFromMemory(
      BinaryData::modpanelbig_right_up_png, BinaryData::modpanelbig_right_up_pngSize);
  juce::Image glas_small_down = ImageCache::getFromMemory(
      BinaryData::modpanelsmall_down_png, BinaryData::modpanelsmall_down_pngSize);
  juce::Image glas_small_mid = ImageCache::getFromMemory(
      BinaryData::modpanelsmall_mid_png, BinaryData::modpanelsmall_mid_pngSize);
  juce::Image glas_small_up = ImageCache::getFromMemory(
      BinaryData::modpanelsmall_up_png, BinaryData::modpanelsmall_up_pngSize);

  m_source[0].setImage(glas_left_up);
  m_amount_1[0].setImage(glas_small_up);
  m_dest_1[0].setImage(glas_mid_up);
  m_amount_2[0].setImage(glas_small_up);
  m_dest_2[0].setImage(glas_mid_up);
  m_amount_3[0].setImage(glas_small_up);
  m_scale[0].setImage(glas_right_up);
  for (int i = 1; i < N_ROWS - 1; ++i) {
    m_source[i].setImage(glas_left_mid);
    m_amount_1[i].setImage(glas_small_mid);
    m_dest_1[i].setImage(glas_mid_mid);
    m_amount_2[i].setImage(glas_small_mid);
    m_dest_2[i].setImage(glas_mid_mid);
    m_amount_3[i].setImage(glas_small_mid);
    m_scale[i].setImage(glas_right_mid);
  }
  m_source[N_ROWS - 1].setImage(glas_left_down);
  m_amount_1[N_ROWS - 1].setImage(glas_small_down);
  m_dest_1[N_ROWS - 1].setImage(glas_mid_down);
  m_amount_2[N_ROWS - 1].setImage(glas_small_down);
  m_dest_2[N_ROWS - 1].setImage(glas_mid_down);
  m_amount_3[N_ROWS - 1].setImage(glas_small_down);
  m_scale[N_ROWS - 1].setImage(glas_right_down);

  juce::Colour modmatrix_color = STANDARD_DISPLAY_COLOR;
  // juce::Colour modmatrix_color_bar(90, 150, 180);

  juce::Colour modmatrix_color_bar(60, 130, 160);
  juce::Colour modmatrix_color_bar_negative(230, 80, 110);

  m_color = modmatrix_color;

  for (int i = 0; i < N_ROWS; ++i) {
    // m_amount_1[i].setParameterId("amount_1_[" + std::to_string(i) + "]");
    // m_amount_2[i].setParameterId("amount_2_[" + std::to_string(i) + "]");
    // m_amount_3[i].setParameterId("amount_3_[" + std::to_string(i) + "]");
    // m_value_tree.addParameterListener("amount_1_[" + std::to_string(i) + "]",
    //                                   &(m_amount_1[i]));
    // m_value_tree.addParameterListener("amount_2_[" + std::to_string(i) + "]",
    //                                   &(m_amount_2[i]));
    // m_value_tree.addParameterListener("amount_3_[" + std::to_string(i) + "]",
    //                                   &(m_amount_3[i]));
    DBG("POINTER TO COMPONENT");
    DBG((long)&(m_amount_2[i]));

    m_source[i].setTooltip("Set the mod source");
    m_amount_1[i].setTooltip("Set the mod amount for the first destination");
    m_dest_1[i].setTooltip("Set the first mod destination");
    m_amount_2[i].setTooltip("Set the mod amount for the second destination");
    m_dest_2[i].setTooltip("Set the second mod destination");
    m_amount_3[i].setTooltip(
        "Set how much the scale signal scales the modulation");
    m_scale[i].setTooltip("Set the scale signal");

    m_source[i].setDefaultText("Source");
    m_dest_1[i].setDefaultText("Dest 1");
    m_dest_2[i].setDefaultText("Dest 2");
    m_scale[i].setDefaultText("Scale");

    m_source[i].clear();
    // createMenu(m_source[i].getRootMenu());
    setStandardMenu(m_source[i].getRootMenu());

    m_source[i].setEditableText(false);
    m_source[i].setSelectedId(1, dontSendNotification);
    m_source[i].setColor(modmatrix_color);
    m_source[i].setGreyFirstElement(true);
    addAndMakeVisible(m_source[i]);

    m_amount_1[i].setColor(modmatrix_color);
    m_amount_1[i].setColorBars(modmatrix_color_bar,
                               modmatrix_color_bar_negative);
    addAndMakeVisible(m_amount_1[i]);

    m_dest_1[i].rearrangeMenu = [&, i]() {
      m_dest_1[i].clear();
      createMenu(m_dest_1[i].getRootMenu());
    };
    m_dest_1[i].rearrangeMenu();
    m_dest_1[i].setEditableText(false);
    m_dest_1[i].setSelectedId(1, dontSendNotification);
    m_dest_1[i].setColor(modmatrix_color);
    m_dest_1[i].setGreyFirstElement(true);
    addAndMakeVisible(m_dest_1[i]);

    m_amount_2[i].setColor(modmatrix_color);
    m_amount_2[i].setColorBars(modmatrix_color_bar,
                               modmatrix_color_bar_negative);

    addAndMakeVisible(m_amount_2[i]);

    m_dest_2[i].rearrangeMenu = [&, i]() {
      m_dest_2[i].clear();
      createMenu(m_dest_2[i].getRootMenu());
    };
    m_dest_2[i].rearrangeMenu();
    m_dest_2[i].setEditableText(false);
    m_dest_2[i].setSelectedId(1, dontSendNotification);
    m_dest_2[i].setColor(modmatrix_color);
    m_dest_2[i].setGreyFirstElement(true);
    addAndMakeVisible(m_dest_2[i]);

    m_amount_3[i].setColor(modmatrix_color);
    m_amount_3[i].setColorBars(modmatrix_color_bar,
                               modmatrix_color_bar_negative);
    addAndMakeVisible(m_amount_3[i]);

    m_scale[i].clear();
    setStandardMenu(m_scale[i].getRootMenu());
    m_scale[i].setEditableText(false);
    m_scale[i].setSelectedId(1, dontSendNotification);
    m_scale[i].setColor(modmatrix_color);
    m_scale[i].setGreyFirstElement(true);
    addAndMakeVisible(m_scale[i]);
  }

  m_amount_1[0].onValueChange = [&](float p_value) {
    DBG("POINTER TO COMPONENT");
    DBG((long)&(m_amount_1[0]));
    m_value_tree.getParameter(m_amount_1_identifier0)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[0].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier0)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[0].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier0)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[1].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier1)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[1].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier1)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[1].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier1)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[2].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier2)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[2].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier2)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[2].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier2)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[3].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier3)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[3].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier3)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[3].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier3)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[4].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier4)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[4].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier4)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[4].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier4)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[5].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier5)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[5].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier5)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[5].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier5)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[6].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier6)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[6].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier6)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[6].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier6)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[7].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier7)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[7].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier7)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[7].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier7)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[8].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier8)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[8].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier8)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[8].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier8)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_source[0].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier0,
                                   m_source[0].getSelectedId(), nullptr);
  };
  m_dest_1[0].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier0,
                                   m_dest_1[0].getSelectedId(), nullptr);
  };
  m_dest_2[0].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier0,
                                   m_dest_2[0].getSelectedId(), nullptr);
  };
  m_scale[0].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier0,
                                   m_scale[0].getSelectedId(), nullptr);
  };

  m_source[1].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier1,
                                   m_source[1].getSelectedId(), nullptr);
  };
  m_dest_1[1].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier1,
                                   m_dest_1[1].getSelectedId(), nullptr);
  };
  m_dest_2[1].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier1,
                                   m_dest_2[1].getSelectedId(), nullptr);
  };
  m_scale[1].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier1,
                                   m_scale[1].getSelectedId(), nullptr);
  };

  m_source[2].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier2,
                                   m_source[2].getSelectedId(), nullptr);
  };
  m_dest_1[2].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier2,
                                   m_dest_1[2].getSelectedId(), nullptr);
  };
  m_dest_2[2].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier2,
                                   m_dest_2[2].getSelectedId(), nullptr);
  };
  m_scale[2].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier2,
                                   m_scale[2].getSelectedId(), nullptr);
  };

  m_source[3].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier3,
                                   m_source[3].getSelectedId(), nullptr);
  };
  m_dest_1[3].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier3,
                                   m_dest_1[3].getSelectedId(), nullptr);
  };
  m_dest_2[3].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier3,
                                   m_dest_2[3].getSelectedId(), nullptr);
  };
  m_scale[3].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier3,
                                   m_scale[3].getSelectedId(), nullptr);
  };

  m_source[4].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier4,
                                   m_source[4].getSelectedId(), nullptr);
  };
  m_dest_1[4].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier4,
                                   m_dest_1[4].getSelectedId(), nullptr);
  };
  m_dest_2[4].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier4,
                                   m_dest_2[4].getSelectedId(), nullptr);
  };
  m_scale[4].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier4,
                                   m_scale[4].getSelectedId(), nullptr);
  };

  m_source[5].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier5,
                                   m_source[5].getSelectedId(), nullptr);
  };
  m_dest_1[5].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier5,
                                   m_dest_1[5].getSelectedId(), nullptr);
  };
  m_dest_2[5].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier5,
                                   m_dest_2[5].getSelectedId(), nullptr);
  };
  m_scale[5].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier5,
                                   m_scale[5].getSelectedId(), nullptr);
  };

  m_source[6].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier6,
                                   m_source[6].getSelectedId(), nullptr);
  };
  m_dest_1[6].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier6,
                                   m_dest_1[6].getSelectedId(), nullptr);
  };
  m_dest_2[6].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier6,
                                   m_dest_2[6].getSelectedId(), nullptr);
  };
  m_scale[6].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier6,
                                   m_scale[6].getSelectedId(), nullptr);
  };

  m_source[7].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier7,
                                   m_source[7].getSelectedId(), nullptr);
  };
  m_dest_1[7].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier7,
                                   m_dest_1[7].getSelectedId(), nullptr);
  };
  m_dest_2[7].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier7,
                                   m_dest_2[7].getSelectedId(), nullptr);
  };
  m_scale[7].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier7,
                                   m_scale[7].getSelectedId(), nullptr);
  };

  m_source[8].onChange = [&]() {
    m_value_tree.state.setProperty(m_source_identifier8,
                                   m_source[8].getSelectedId(), nullptr);
  };
  m_dest_1[8].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_1_identifier8,
                                   m_dest_1[8].getSelectedId(), nullptr);
  };
  m_dest_2[8].onChange = [&]() {
    m_value_tree.state.setProperty(m_dest_2_identifier8,
                                   m_dest_2[8].getSelectedId(), nullptr);
  };
  m_scale[8].onChange = [&]() {
    m_value_tree.state.setProperty(m_scale_identifier8,
                                   m_scale[8].getSelectedId(), nullptr);
  };

  m_small_panel_width = glas_small_down.getWidth();
  m_big_panel_width = glas_mid_down.getWidth();
  m_panel_height = glas_left_down.getHeight();

  juce::Image clear_1 = ImageCache::getFromMemory(
      BinaryData::buttonclear_1_png, BinaryData::buttonclear_1_pngSize);
  juce::Image clear_2 = ImageCache::getFromMemory(
      BinaryData::buttonclear_2_png, BinaryData::buttonclear_2_pngSize);
  juce::Image clearlast_1 = ImageCache::getFromMemory(
      BinaryData::buttonclearlast_1_png, BinaryData::buttonclearlast_1_pngSize);
  juce::Image clearlast_2 = ImageCache::getFromMemory(
      BinaryData::buttonclearlast_2_png, BinaryData::buttonclearlast_2_pngSize);

  juce::DrawableImage clear_draw1;
  juce::DrawableImage clear_draw2;
  juce::DrawableImage clearlast_draw1;
  juce::DrawableImage clearlast_draw2;

  clear_draw1.setImage(clear_1);
  clear_draw2.setImage(clear_2);
  clearlast_draw1.setImage(clearlast_1);
  clearlast_draw2.setImage(clearlast_2);

  m_clear_button0.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button0.setClickingTogglesState(true);
  m_clear_button0.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            0 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button0);
  m_clear_button0.setTriggeredOnMouseDown(false);
  m_clear_button0.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button1.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button1.setClickingTogglesState(true);
  m_clear_button1.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            1 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button1);
  m_clear_button1.setTriggeredOnMouseDown(false);
  m_clear_button1.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button2.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button2.setClickingTogglesState(true);
  m_clear_button2.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            2 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button2);
  m_clear_button2.setTriggeredOnMouseDown(false);
  m_clear_button2.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button3.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button3.setClickingTogglesState(true);
  m_clear_button3.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            3 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button3);
  m_clear_button3.setTriggeredOnMouseDown(false);
  m_clear_button3.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button4.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button4.setClickingTogglesState(true);
  m_clear_button4.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            4 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button4);
  m_clear_button4.setTriggeredOnMouseDown(false);
  m_clear_button4.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button5.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button5.setClickingTogglesState(true);
  m_clear_button5.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            5 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button5);
  m_clear_button5.setTriggeredOnMouseDown(false);
  m_clear_button5.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button6.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button6.setClickingTogglesState(true);
  m_clear_button6.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            6 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button6);
  m_clear_button6.setTriggeredOnMouseDown(false);
  m_clear_button6.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button7.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button7.setClickingTogglesState(true);
  m_clear_button7.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            7 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button7);
  m_clear_button7.setTriggeredOnMouseDown(false);
  m_clear_button7.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button8.setImages(
      &clearlast_draw2, &clearlast_draw2, &clearlast_draw1, &clearlast_draw1,
      &clearlast_draw2, &clearlast_draw2, &clearlast_draw1, &clearlast_draw1);
  m_clear_button8.setClickingTogglesState(true);
  m_clear_button8.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            8 * m_panel_height, clearlast_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button8);
  m_clear_button8.setTriggeredOnMouseDown(false);
  m_clear_button8.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button0.onClick = [&]() { clearRow(0); };
  m_clear_button1.onClick = [&]() { clearRow(1); };
  m_clear_button2.onClick = [&]() { clearRow(2); };
  m_clear_button3.onClick = [&]() { clearRow(3); };
  m_clear_button4.onClick = [&]() { clearRow(4); };
  m_clear_button5.onClick = [&]() { clearRow(5); };
  m_clear_button6.onClick = [&]() { clearRow(6); };
  m_clear_button7.onClick = [&]() { clearRow(7); };
  m_clear_button8.onClick = [&]() { clearRow(8); };

  m_clear_button0.setTooltip("Clear the entire modmatrix row");
  m_clear_button1.setTooltip("Clear the entire modmatrix row");
  m_clear_button2.setTooltip("Clear the entire modmatrix row");
  m_clear_button3.setTooltip("Clear the entire modmatrix row");
  m_clear_button4.setTooltip("Clear the entire modmatrix row");
  m_clear_button5.setTooltip("Clear the entire modmatrix row");
  m_clear_button6.setTooltip("Clear the entire modmatrix row");
  m_clear_button7.setTooltip("Clear the entire modmatrix row");
  m_clear_button8.setTooltip("Clear the entire modmatrix row");

  for (int i = 0; i < N_ROWS; ++i) {
    m_amount_1[i].setParameterId("amount_1_[" + std::to_string(i) + "]");
    m_amount_2[i].setParameterId("amount_2_[" + std::to_string(i) + "]");
    m_amount_3[i].setParameterId("amount_3_[" + std::to_string(i) + "]");
    m_value_tree.addParameterListener("amount_1_[" + std::to_string(i) + "]",
                                      &(m_amount_1[i]));
    m_value_tree.addParameterListener("amount_2_[" + std::to_string(i) + "]",
                                      &(m_amount_2[i]));
    m_value_tree.addParameterListener("amount_3_[" + std::to_string(i) + "]",
                                      &(m_amount_3[i]));
  }

  forceValueTreeOntoComponents(m_value_tree.state);
}

ModMatrixComponent::~ModMatrixComponent() {
  for (int i = 0; i < N_ROWS; ++i) {
    m_value_tree.removeParameterListener("amount_1_[" + std::to_string(i) + "]",
                                         &(m_amount_1[i]));
    m_value_tree.removeParameterListener("amount_2_[" + std::to_string(i) + "]",
                                         &(m_amount_2[i]));
    m_value_tree.removeParameterListener("amount_3_[" + std::to_string(i) + "]",
                                         &(m_amount_3[i]));
  }
}

void ModMatrixComponent::paint(Graphics &g) {
  g.setColour(m_color);
  auto background = getLocalBounds();
  background.removeFromRight(19);
  g.fillRect(background);
}

void ModMatrixComponent::resized() {

  for (int i = 0; i < N_ROWS; ++i) {
    m_source[i].setBounds(0, i * m_panel_height, m_big_panel_width,
                          m_panel_height);
    m_amount_1[i].setBounds(m_big_panel_width, i * m_panel_height,
                            m_small_panel_width, m_panel_height);
    m_dest_1[i].setBounds(m_big_panel_width + m_small_panel_width,
                          i * m_panel_height, m_big_panel_width,
                          m_panel_height);
    m_amount_2[i].setBounds(2 * m_big_panel_width + m_small_panel_width,
                            i * m_panel_height, m_small_panel_width,
                            m_panel_height);
    m_dest_2[i].setBounds(2 * m_big_panel_width + 2 * m_small_panel_width,
                          i * m_panel_height, m_big_panel_width,
                          m_panel_height);
    m_amount_3[i].setBounds(3 * m_big_panel_width + 2 * m_small_panel_width,
                            i * m_panel_height, m_small_panel_width,
                            m_panel_height);
    m_scale[i].setBounds(3 * m_big_panel_width + 3 * m_small_panel_width,
                         i * m_panel_height, m_big_panel_width, m_panel_height);
  }
}

void ModMatrixComponent::clearRow(int p_row) {
  m_source[p_row].reset();
  m_amount_1[p_row].reset();
  m_dest_1[p_row].reset();
  m_amount_2[p_row].reset();
  m_dest_2[p_row].reset();
  m_amount_3[p_row].reset();
  m_scale[p_row].reset();
}

void ModMatrixComponent::setStandardMenu(PopupMenu *p_menu) {
  *p_menu = m_sources_menu;
}

void ModMatrixComponent::createMenu(PopupMenu *p_menu) {
  // read osc and filter types from parent
  int osc_type[3] = {0};
  int fil_type[3] = {0};
  getOscFilterTypes(osc_type[0], osc_type[1], osc_type[2], fil_type[0],
                    fil_type[1], fil_type[2]);

  // p_menu->addItem(0, "Disabled");
  // p_menu->addSeparator();

  for (int osc = 0; osc < 3; ++osc) {
    if (osc_type[osc] == OSC_TYPE_ANALOG) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1),
                         m_analog_osc_menu[osc], true);
    } else if (osc_type[osc] == OSC_TYPE_WAVETABLE) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1),
                         m_wavetable_osc_menu[osc], true);
    } else if (osc_type[osc] == OSC_TYPE_MULTI) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1),
                         m_multi_osc_menu[osc], true);
    } else if (osc_type[osc] == OSC_TYPE_VECTOR) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1),
                         m_vector_osc_menu[osc], true);
    } else if (osc_type[osc] == OSC_TYPE_FM) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1), m_fm_osc_menu[osc],
                         true);
    } else if (osc_type[osc] == OSC_TYPE_CHIPTUNE) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1),
                         m_chiptune_osc_menu[osc], true);
    } else if (osc_type[osc] == OSC_TYPE_NOISE) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1),
                         m_noise_osc_menu[osc], true);
    } else if (osc_type[osc] == OSC_TYPE_WAVEDRAW) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1), m_draw_osc_menu[osc],
                         true);
    } else if (osc_type[osc] == OSC_TYPE_CHIPDRAW) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1), m_draw_osc_menu[osc],
                         true);
    } else if (osc_type[osc] == OSC_TYPE_SPECDRAW) {
      p_menu->addSubMenu("Osc " + std::to_string(osc + 1), m_draw_osc_menu[osc],
                         true);
    } else {
      p_menu->addItem(998, "Osc " + std::to_string(osc + 1), false);
    }
  }

  p_menu->addSeparator();

  for (int fil = 0; fil < 3; ++fil) {
    if (fil_type[fil] == FILTER_TYPE_LP24 ||
        fil_type[fil] == FILTER_TYPE_LP12 ||
        fil_type[fil] == FILTER_TYPE_BP24 ||
        fil_type[fil] == FILTER_TYPE_BP12 ||
        fil_type[fil] == FILTER_TYPE_HP12 ||
        fil_type[fil] == FILTER_TYPE_HP24 ||
        fil_type[fil] == FILTER_TYPE_KORG_LP ||
        fil_type[fil] == FILTER_TYPE_KORG_HP ||
        fil_type[fil] == FILTER_TYPE_DIODE) {
      p_menu->addSubMenu("Filter " + std::to_string(fil + 1),
                         m_standard_fil_menu[fil], true);
    } else if (fil_type[fil] == FILTER_TYPE_FORMANT) {
      p_menu->addSubMenu("Filter " + std::to_string(fil + 1),
                         m_formant_fil_menu[fil], true);
    } else if (fil_type[fil] == FILTER_TYPE_SEM12) {
      p_menu->addSubMenu("Filter " + std::to_string(fil + 1),
                         m_SEM_fil_menu[fil], true);
    } else if (fil_type[fil] == FILTER_TYPE_COMB) {
      p_menu->addSubMenu("Filter " + std::to_string(fil + 1),
                         m_comb_fil_menu[fil]);
    } else {
      p_menu->addItem(998, "Filter " + std::to_string(fil + 1), false);
    }
  }

  p_menu->addSeparator();

  p_menu->addSubMenu("Amplifier", m_amp_menu, true);
  p_menu->addSubMenu("Distortion", m_distortion_menu, true);

  p_menu->addSeparator();

  p_menu->addSubMenu("Amp Env", m_adsr_menu[0], true);
  p_menu->addSubMenu("Filter Env", m_adsr_menu[1], true);
  p_menu->addSubMenu("Mod Env", m_adsr_menu[2], true);
  p_menu->addSubMenu("Global Env", m_adsr_menu[3], true);

  p_menu->addSeparator();

  p_menu->addSubMenu("LFO 1", m_lfo_menu[0], true);
  p_menu->addSubMenu("LFO 2", m_lfo_menu[1], true);
  p_menu->addSubMenu("LFO 3", m_lfo_menu[2], true);
  p_menu->addSubMenu("Global LFO", m_lfo_menu[3], true);

  p_menu->addSeparator();

  p_menu->addSubMenu("Delay", m_delay_menu, true);
  p_menu->addSubMenu("Phaser", m_phaser_menu, true);
  p_menu->addSubMenu("Flanger", m_flanger_menu, true);
  p_menu->addSubMenu("Chorus", m_chorus_menu, true);

  p_menu->addSeparator();

  p_menu->addItem(999, "Glide");
  p_menu->addItem(1000, "Master");
}

void ModMatrixComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
  for (int row = 0; row < N_ROWS; ++row) {
    m_source[row].setValue(
        m_value_tree.state[String("source_[" + std::to_string(row) + "]")]);

    m_dest_1[row].rearrangeMenu();
    m_dest_1[row].setValue(
        m_value_tree.state[String("dest_1_[" + std::to_string(row) + "]")]);

    m_dest_2[row].rearrangeMenu();
    m_dest_2[row].setValue(
        m_value_tree.state[String("dest_2_[" + std::to_string(row) + "]")]);

    m_scale[row].setValue(
        m_value_tree.state[String("scale_[" + std::to_string(row) + "]")]);

    m_amount_1[row].setValue(
        m_value_tree
            .getParameterAsValue("amount_1_[" + std::to_string(row) + "]")
            .getValue());
    m_amount_2[row].setValue(
        m_value_tree
            .getParameterAsValue("amount_2_[" + std::to_string(row) + "]")
            .getValue());
    m_amount_3[row].setValue(
        m_value_tree
            .getParameterAsValue("amount_3_[" + std::to_string(row) + "]")
            .getValue());
  }
}

