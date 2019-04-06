/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OdinAudioProcessor::OdinAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
    : m_parameters(*this, nullptr, Identifier("Odin"),
#include "AudioValueTree.h"
      )
{
#include "AudioParameterConnections.h"

  //connect voice end to voice manager
  for (int i = 0; i < VOICES; ++i)
  {
    m_voice[i].onEnvelopeEnd = [&]() {
      m_voice_manager.freeVoice(i);
    };
  }

  // create wavetables
  WavetableContainer::getInstance().createWavetables(44100.f); // todo

  // load wavetables into oscs
  for (int i = 0; i < VOICES; ++i)
  {
    for (int osc = 0; osc < 3; ++osc)
    {
      m_voice[i].analog_osc[osc].loadWavetables();
      m_voice[i].chiptune_osc[osc].loadWavetables();
      m_voice[i].vector_osc[osc].loadWavetables();
      m_voice[i].multi_osc[osc].loadWavetables();
      m_voice[i].fm_osc[osc].loadWavetables();
    }
  }
}

OdinAudioProcessor::~OdinAudioProcessor()
{
  WavetableContainer::getInstance().destroyWavetables();
}

//==============================================================================
const String OdinAudioProcessor::getName() const { return JucePlugin_Name; }

bool OdinAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool OdinAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool OdinAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double OdinAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int OdinAudioProcessor::getNumPrograms()
{
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int OdinAudioProcessor::getCurrentProgram() { return 0; }

void OdinAudioProcessor::setCurrentProgram(int index) {}

const String OdinAudioProcessor::getProgramName(int index) { return {}; }

void OdinAudioProcessor::changeProgramName(int index, const String &newName) {}

//==============================================================================
void OdinAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  m_voice[0].start(52);
}

void OdinAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OdinAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
  ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void OdinAudioProcessor::processBlock(AudioBuffer<float> &buffer,
                                      MidiBuffer &midiMessages)
{
  ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  //loop over samples
  for (int sample = 0; sample < buffer.getNumSamples(); ++sample)  {

    //============================================================
    //======================== VOICES ============================
    //============================================================

    //we will write to this before the amplifier section
    float voices_output = 0;

    //loop over all voices
    for(int voice = 0; voice < VOICES; ++voice){
      if(m_voice[voice]){

        //===== OSCS ==========

        //output var for the individual oscs
        float osc_output[3];
        for(int osc = 0; osc < 3; ++osc){
          //analog osc
          if((int)m_osc_type[osc] == OSC_TYPE_ANALOG){
            m_voice[voice].analog_osc[osc].update();
            osc_output[osc] += m_voice[voice].analog_osc[osc].doOscillate();
          } 
          //wavetable osc
          else if((int)m_osc_type[osc] == OSC_TYPE_WAVETABLE) {
            m_voice[voice].wavetable_osc[osc].update();
            osc_output[osc] += m_voice[voice].multi_osc[osc].doOscillate();
          } 
          //multi osc
          else if((int)m_osc_type[osc] == OSC_TYPE_MULTI) {
            m_voice[voice].multi_osc[osc].update();
            osc_output[osc] += m_voice[voice].multi_osc[osc].doOscillate();
          } 
          //chiptune osc
          else if((int)m_osc_type[osc] == OSC_TYPE_CHIPTUNE) {
            m_voice[voice].chiptune_osc[osc].update();
            osc_output[osc] += m_voice[voice].chiptune_osc[osc].doOscillate();
          } 
        //just write all oscs to output for now...
        voices_output += osc_output[osc];
        }//osc loop
      }//voice active
    }//voice loop
    for (int channel = 0; channel < totalNumInputChannels; ++channel){
      auto *channelData = buffer.getWritePointer(channel);
      channelData[sample] = voices_output * 0.2f;
    }
  }
}

//==============================================================================
bool OdinAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor *OdinAudioProcessor::createEditor()
{
  return new OdinAudioProcessorEditor(*this, m_parameters);
}

//==============================================================================
void OdinAudioProcessor::getStateInformation(MemoryBlock &destData)
{
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void OdinAudioProcessor::setStateInformation(const void *data,
                                             int sizeInBytes)
{
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
  return new OdinAudioProcessor();
}
