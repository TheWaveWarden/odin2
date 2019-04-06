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

  // connect voice end to voice manager
  for (int i = 0; i < VOICES; ++i) {
    m_voice[i].onEnvelopeEnd = [&]() { m_voice_manager.freeVoice(i); };
  }

  // create wavetables
  WavetableContainer::getInstance().createWavetables(44100.f); // todo

  // load wavetables into oscs
  for (int i = 0; i < VOICES; ++i) {
    for (int osc = 0; osc < 3; ++osc) {
      m_voice[i].analog_osc[osc].loadWavetables();
      m_voice[i].wavetable_osc[osc].loadWavetables();
      m_voice[i].chiptune_osc[osc].loadWavetables();
      m_voice[i].vector_osc[osc].loadWavetables();
      m_voice[i].multi_osc[osc].loadWavetables();
      m_voice[i].fm_osc[osc].loadWavetables();
    }
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

  // DBG(totalNumInputChannels);
  // DBG(totalNumOutputChannels);
  // DBG("-----");

  // loop over samples
  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

    //============================================================
    //======================== VOICES ============================
    //============================================================

    // we will write to this before the amplifier section
    float voices_output = 0;

    // loop over all voices
    for (int voice = 0; voice < VOICES; ++voice) {
      if (m_voice[voice]) {

        //===== OSCS ======

        // output var for the individual oscs
        float osc_output[3] = {0};
        for (int osc = 0; osc < 3; ++osc) {
          // analog osc
          if (*m_osc_type[osc] == OSC_TYPE_ANALOG) {
            m_voice[voice].analog_osc[osc].update();
            osc_output[osc] += m_voice[voice].analog_osc[osc].doOscillate();
          }
          // wavetable osc
          else if (*m_osc_type[osc] == OSC_TYPE_WAVETABLE) {
            // m_voice[voice].wavetable_osc[osc].setPosition(0);
            // m_voice[voice].wavetable_osc[osc].selectWavetable(0);
            m_voice[voice].wavetable_osc[osc].update();
            osc_output[osc] += m_voice[voice].wavetable_osc[osc].doOscillate();
          }
          // multi osc
          else if (*m_osc_type[osc] == OSC_TYPE_MULTI) {
            m_voice[voice].multi_osc[osc].update();
            osc_output[osc] += m_voice[voice].multi_osc[osc].doOscillate();
          }
          // vector osc
          else if (*m_osc_type[osc] == OSC_TYPE_VECTOR) {
            m_voice[voice].vector_osc[osc].update();
            osc_output[osc] += m_voice[voice].vector_osc[osc].doOscillate();
          }
          // chiptune osc
          else if (*m_osc_type[osc] == OSC_TYPE_CHIPTUNE) {
            m_voice[voice].chiptune_osc[osc].update();
            osc_output[osc] += m_voice[voice].chiptune_osc[osc].doOscillate();
          }
          // fm osc
          else if (*m_osc_type[osc] == OSC_TYPE_FM) {
            m_voice[voice].fm_osc[osc].update();
            osc_output[osc] += m_voice[voice].fm_osc[osc].doOscillate();
          }
          // noise osc
          else if (*m_osc_type[osc] == OSC_TYPE_NOISE) {
            m_voice[voice].noise_osc[osc].setFilterFreqs(18000, 80);
            osc_output[osc] += m_voice[voice].noise_osc[osc].doNoise();
          }
        } // osc loop

        //===== FILTERS ======
        DBG(*m_fil_type[0]);
        float filter_input[2] = {0};
        float filter_output[2] = {0};
        for (int fil = 0; fil < 2; ++fil) {
          // get filter inputs, fil1->fil2 is done at the end of fil1 calc
          if (*m_fil_osc1[fil]) {
            filter_input[fil] += osc_output[0];
          }
          if (*m_fil_osc2[fil]) {
            filter_input[fil] += osc_output[1];
          }
          if (*m_fil_osc3[fil]) {
            filter_input[fil] += osc_output[2];
          }

          if (*m_fil_type[fil] == FILTER_TYPE_NONE) {
            filter_output[fil] = filter_input[fil];
          } else if (*m_fil_type[fil] == FILTER_TYPE_LP24 ||
                     *m_fil_type[fil] == FILTER_TYPE_LP12 ||
                     *m_fil_type[fil] == FILTER_TYPE_BP24 ||
                     *m_fil_type[fil] == FILTER_TYPE_BP12 ||
                     *m_fil_type[fil] == FILTER_TYPE_HP24 ||
                     *m_fil_type[fil] == FILTER_TYPE_HP12) {
            m_voice[voice].ladder_filter[fil].update();
            filter_output[fil] =
                m_voice[voice].ladder_filter[fil].doFilter(filter_input[fil]);
          } else if (*m_fil_type[fil] == FILTER_TYPE_SEM24) {
            m_voice[voice].SEM_filter_24[fil].update();
            filter_output[fil] =
                m_voice[voice].SEM_filter_24[fil].doFilter(filter_input[fil]);

          } else if (*m_fil_type[fil] == FILTER_TYPE_SEM12) {
            //todo need params set to wörk...
            m_voice[voice].SEM_filter_12[fil].update();
            filter_output[fil] =
                m_voice[voice].SEM_filter_12[fil].doFilter(filter_input[fil]);
          } else if (*m_fil_type[fil] == FILTER_TYPE_KORG) {
            m_voice[voice].korg_filter[fil].update();
            filter_output[fil] =
                m_voice[voice].korg_filter[fil].doFilter(filter_input[fil]);
          } else if (*m_fil_type[fil] == FILTER_TYPE_DIODE) {
            m_voice[voice].diode_filter[fil].update();
            filter_output[fil] =
                m_voice[voice].diode_filter[fil].doFilter(filter_input[fil]);
          } else if (*m_fil_type[fil] == FILTER_TYPE_FORMANT) {
            m_voice[voice].formant_filter[fil].update();
            filter_output[fil] =
                m_voice[voice].formant_filter[fil].doFilter(filter_input[fil]);
          } else if (*m_fil_type[fil] == FILTER_TYPE_COMB) {
            filter_output[fil] =
                m_voice[voice].comb_filter[fil].doFilter(filter_input[fil]);
          }

          // add first filter to second
          if (fil == 0 && *m_fil2_fil1) {
            filter_input[1] += filter_output[0];
          }
        } // filter loop

        if (*m_fil1_to_amp) {
          voices_output += filter_output[0];
        }
        if (*m_fil2_to_amp) {
          voices_output += filter_output[1];
        }
      } // voice active
    }   // voice loop
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
      auto *channelData = buffer.getWritePointer(channel);
      channelData[sample] = voices_output * 0.2f;
    } // channel
  }   // sample
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
