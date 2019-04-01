/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Voice.h"

#define VOICES 12

//==============================================================================
/**
*/
class OdinAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    OdinAudioProcessor();
    ~OdinAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
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
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    Voice m_voice[VOICES];
    AudioProcessorValueTreeState m_parameters;

    float* m_phaser_on_parameter;
    //osc1
    float* m_osc1_oct;
    float* m_osc1_semi;
    float* m_osc1_fine;
    float* m_osc1_vol;
    float* m_osc1_position;
    float* m_osc1_detune;
    float* m_osc1_multi_position;
    float* m_osc1_spread;
    float* m_osc1_pulsewidth;
    float* m_osc1_drift;
    float* m_osc1_arp_speed;
    float* m_osc1_step_1;
    float* m_osc1_step_2;
    float* m_osc1_step_3;
    float* m_osc1_fm;
    float* m_osc1_lp;
    float* m_osc1_hp;
    float* m_osc1_reset;
    float* m_osc1_arp_on;
    float* m_osc1_step_3_on;
    float* m_osc1_chipnoise;
    float* m_osc1_exp_fm;
    //osc2
    float* m_osc2_oct;
    float* m_osc2_semi;
    float* m_osc2_fine;
    float* m_osc2_vol;
    float* m_osc2_position;
    float* m_osc2_detune;
    float* m_osc2_multi_position;
    float* m_osc2_spread;
    float* m_osc2_pulsewidth;
    float* m_osc2_drift;
    float* m_osc2_arp_speed;
    float* m_osc2_step_1;
    float* m_osc2_step_2;
    float* m_osc2_step_3;
    float* m_osc2_fm;
    float* m_osc2_lp;
    float* m_osc2_hp;
    float* m_osc2_reset;
    float* m_osc2_arp_on;
    float* m_osc2_step_3_on;
    float* m_osc2_chipnoise;
    float* m_osc2_exp_fm;
    //osc3
    float* m_osc3_oct;
    float* m_osc3_semi;
    float* m_osc3_fine;
    float* m_osc3_vol;
    float* m_osc3_position;
    float* m_osc3_detune;
    float* m_osc3_multi_position;
    float* m_osc3_spread;
    float* m_osc3_pulsewidth;
    float* m_osc3_drift;
    float* m_osc3_arp_speed;
    float* m_osc3_step_1;
    float* m_osc3_step_2;
    float* m_osc3_step_3;
    float* m_osc3_fm;
    float* m_osc3_lp;
    float* m_osc3_hp;
    float* m_osc3_reset;
    float* m_osc3_arp_on;
    float* m_osc3_step_3_on;
    float* m_osc3_chipnoise;
    float* m_osc3_exp_fm;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OdinAudioProcessor)
};
