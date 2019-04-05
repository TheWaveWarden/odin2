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
      ) {
#include "AudioParameterConnections.h"

  // create wavetables
  WavetableContainer::getInstance().createWavetables(44100.f); // todo

  // load wavetables into oscs
  for (int i = 0; i < VOICES; ++i) {
    m_voice[i].analog_osc.loadWavetables();
    m_voice[i].chiptune_osc.loadWavetables();
    m_voice[i].vector_osc.loadWavetables();
    m_voice[i].multi_osc.loadWavetables();
    m_voice[i].fm_osc.loadWavetables();
  }
}

OdinAudioProcessor::~OdinAudioProcessor() {
  WavetableContainer::getInstance().destroyWavetables();
}

//==============================================================================
const String OdinAudioProcessor::getName() const { return JucePlugin_Name; }

bool OdinAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool OdinAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool OdinAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double OdinAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int OdinAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int OdinAudioProcessor::getCurrentProgram() { return 0; }

void OdinAudioProcessor::setCurrentProgram(int index) {}

const String OdinAudioProcessor::getProgramName(int index) { return {}; }

void OdinAudioProcessor::changeProgramName(int index, const String &newName) {}

//==============================================================================
void OdinAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  m_voice[0].start(52);
}

void OdinAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OdinAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
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
                                      MidiBuffer &midiMessages) {
  ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // DBG(*m_dest_1[0]);

  // todo remove later
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
    m_voice[0].analog_osc.update();
    float output = m_voice[0].analog_osc.doOscillate();
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
      auto *channelData = buffer.getWritePointer(channel);
      // ..do something to the data...
      //*channelData = output;
      channelData[sample] = output;
    }
  }
}






//==============================================================================
bool OdinAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor *OdinAudioProcessor::createEditor() {
  return new OdinAudioProcessorEditor(*this, m_parameters);
}

//==============================================================================
void OdinAudioProcessor::getStateInformation(MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void OdinAudioProcessor::setStateInformation(const void *data,
                                             int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new OdinAudioProcessor();
}
