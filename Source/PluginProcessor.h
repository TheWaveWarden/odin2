/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Voice.h"
#include "audio/Oscillators/WavetableContainer.h"
#include "OdinTreeListener.h"
#include "audio/Amplifier.h"

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
    VoiceManager m_voice_manager;
    AudioProcessorValueTreeState m_parameters;
    OdinTreeListener m_tree_listener;

    Voice m_voice[VOICES];
    Amplifier m_amp;

    void treeValueChanged(const String& p_ID, float p_new_value);

    int m_counter = 0;//todo remove
    #include "AudioVarDeclarations.h"
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OdinAudioProcessor)
};
