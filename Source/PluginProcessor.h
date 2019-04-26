/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinTreeListener.h"
#include "Voice.h"
#include "audio/Amplifier.h"
#include "audio/FX/Chorus.h"
#include "audio/FX/Delay.h"
#include "audio/FX/Flanger.h"
#include "audio/FX/OversamplingDistortion.h"
#include "audio/FX/Phaser.h"
#include "audio/Filters/CombFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Filters/SEMFilter24.h"
#include "audio/Oscillators/WavetableContainer.h"
#include "ModMatrix.h" 

//==============================================================================
/**
 */
class OdinAudioProcessor : public AudioProcessor {
public:
  //==============================================================================
  OdinAudioProcessor();
  ~OdinAudioProcessor();

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(AudioBuffer<float> &, MidiBuffer &) override;

  //==============================================================================
  AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const String getProgramName(int index) override;
  void changeProgramName(int index, const String &newName) override;

  //==============================================================================
  void getStateInformation(MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

private:
  float m_osc_vol_smooth[3] = {1.f, 1.f, 1.f};   // factor
  float m_fil_gain_smooth[3] = {1.f, 1.f, 1.f};  // factor
  float m_osc_vol_control[3] = {1.f, 1.f, 1.f};  // factor
  float m_fil_gain_control[3] = {1.f, 1.f, 1.f}; // factor

  void setSampleRate(float p_samplerate);
  void initializeModules();

  VoiceManager m_voice_manager;
  AudioProcessorValueTreeState m_parameters;
  OdinTreeListener m_tree_listener;

  Voice m_voice[VOICES];
  Amplifier m_amp;
  OversamplingDistortion m_distortion[2];

  LadderFilter m_ladder_filter[2];
  SEMFilter24 m_SEM_filter_24[2];
  SEMFilter12 m_SEM_filter_12[2];
  Korg35Filter m_korg_filter[2];
  DiodeFilter m_diode_filter[2];
  FormantFilter m_formant_filter[2];
  CombFilter m_comb_filter[2];
  Delay m_delay[2];
  Phaser m_phaser[2];
  Flanger m_flanger[2];
  Chorus m_chorus[2];

  ModMatrix m_mod_matrix;
  ModSources m_mod_sources;
  ModDestinations m_mod_destinations;

  void setModulationPointers();
  void treeValueChanged(const String &p_ID, float p_new_value);

  //MOD SOURCES
  float m_adsr[VOICES][4] = {0.f};
  float m_lfo[VOICES][4] = {0.f};
  float m_filter_output[VOICES][2] = {0.f};
  float m_osc_output[VOICES][3] = {0.f};
  float m_MIDI_key[VOICES] = {0.f};
  float m_MIDI_velocity[VOICES] = {0.f};
  float m_MIDI_aftertouch = 0.f;
  float m_constant = 1.f;
  //float m_modwheel = 0.f;
  //float m_pitchwheel = 0.f;
  float m_x = 0.f;
  float m_y = 0.f;


  int m_counter = 0; // todo remove
#include "AudioVarDeclarations.h"
  //==============================================================================
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OdinAudioProcessor)
};
