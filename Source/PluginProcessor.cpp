/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// this file contains implementation
#include "ValueChange.h"

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
#include "AudioValueTree.h" //contains the definition of audiotree. WATCH CLOSELY: is IN m_parameters constructor
                   ),
#include "ProcessorInitializerList.h" //contains the connection of Identifiers with their strings
{
#include "AudioParameterConnections.h" // constains the connection between raw float pointers and their ValueTree counter

  // set up the tree listener
  m_tree_listener.onValueChange = [&](const String &p_ID, float p_new_value) {
    treeValueChanged(p_ID, p_new_value);
  };

  // connect voice end to voice manager
  for (int i = 0; i < VOICES; ++i) {
    m_voice[i].onEnvelopeEnd = [&]() { m_voice_manager.freeVoice(i); };
  }

  setSampleRate(44100.f);
  initializeModules();

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
      m_voice[i].wavedraw_osc[osc].loadWavedrawTables(osc);
      m_voice[i].chipdraw_osc[osc].loadChipdrawTables(osc);
      m_voice[i].specdraw_osc[osc].loadSpecdrawTables(osc);
      m_voice[i].lfo[osc].loadWavetables();
    }
    for (int mod = 0; mod < 4; ++mod) {
      m_voice[i].lfo[mod].loadWavetables();
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
  m_voice[0].start(52, 100);
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

  MidiMessage midi_message;
  int midi_message_sample;
  MidiBuffer::Iterator midi_iterator(midiMessages);
  bool midi_message_remaining =
      midi_iterator.getNextEvent(midi_message, midi_message_sample);

  // loop over samples
  for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

    //===== SMOOTH CONTROLS ======
    for (int i = 0; i < 3; ++i) {
      m_osc_vol_smooth[i] = m_osc_vol_smooth[i] * GAIN_SMOOTHIN_FACTOR +
                            (1.f - GAIN_SMOOTHIN_FACTOR) * m_osc_vol_control[i];
      m_fil_gain_smooth[i] =
          m_fil_gain_smooth[i] * GAIN_SMOOTHIN_FACTOR +
          (1.f - GAIN_SMOOTHIN_FACTOR) * m_fil_gain_control[i];

      m_fil_freq_smooth[i] =
          m_fil_freq_smooth[i] * FILTER_FREQ_SMOOTHING_FACTOR +
          (1.f - FILTER_FREQ_SMOOTHING_FACTOR) * m_fil_freq_control[i];
    }

    m_pitch_bend_smooth_and_applied =
        m_pitch_bend_smooth_and_applied * PITCHBEND_SMOOTHIN_FACTOR +
        (1.f - PITCHBEND_SMOOTHIN_FACTOR) * (*m_pitchbend) *
            (*m_pitchbend_amount);

    m_modwheel_smooth = m_modwheel_smooth * PITCHBEND_SMOOTHIN_FACTOR +
                        (1.f - PITCHBEND_SMOOTHIN_FACTOR) * (*m_modwheel);

    m_x_smooth = m_x_smooth * PAD_SMOOTHIN_FACTOR + (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_x);
    m_y_smooth = m_y_smooth * PAD_SMOOTHIN_FACTOR + (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_y);

    //===== MIDI =====
    if (midi_message_remaining) {
      if (midi_message_sample <= sample) {
        // apply midi message
        if (midi_message.isNoteOn()) {
          m_voice[m_voice_manager.getVoice()].start(
              midi_message.getNoteNumber(), midi_message.getVelocity());
        } else if (midi_message.isNoteOff()) {
          for (int voice = 0; voice < VOICES; ++voice) {
            if (m_voice[voice].keyUp(midi_message.getNoteNumber())) {
              // todo HACK it should only be keyupped here
              m_voice_manager.freeVoice(voice);
            }
          }
        }

        // get next midi message
        midi_message_remaining =
            midi_iterator.getNextEvent(midi_message, midi_message_sample);
      }
    }

    //============================================================
    //======================= MODMATRIX ==========================
    //============================================================

    m_mod_matrix.zeroAllDestinations();
    m_mod_matrix.applyModulation();

    //============================================================
    //======================== VOICES ============================
    //============================================================

    // we will write to this before the amplifier section
    float voices_output = 0;

    // loop over all voices
    for (int voice = 0; voice < VOICES; ++voice) {
      if (m_voice[voice]) {

        for (int mod = 0; mod < 4; ++mod) {
          //===== ADSR ======
          m_adsr[voice][mod] = m_voice[voice].env[mod].doEnvelope();

          //====== LFO ======
          m_voice[voice].lfo[mod].update();
          m_lfo[voice][mod] = m_voice[voice].lfo[mod].doOscillate();
        }

        //===== OSCS ======

        // output var for the individual oscs
        memset(m_osc_output, 0, sizeof(float) * VOICES * 3);

        for (int osc = 0; osc < 3; ++osc) {
          // analog osc
          if (*m_osc_type[osc] == OSC_TYPE_ANALOG) {
            m_voice[voice].analog_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].analog_osc[osc].doOscillate();
          }
          // wavetable osc
          else if (*m_osc_type[osc] == OSC_TYPE_WAVETABLE) {
            // m_voice[voice].wavetable_osc[osc].setPosition(0);
            // m_voice[voice].wavetable_osc[osc].selectWavetable(0);
            m_voice[voice].wavetable_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].wavetable_osc[osc].doOscillate();
          }
          // multi osc
          else if (*m_osc_type[osc] == OSC_TYPE_MULTI) {
            m_voice[voice].multi_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].multi_osc[osc].doOscillate();
          }
          // vector osc
          else if (*m_osc_type[osc] == OSC_TYPE_VECTOR) {
            m_voice[voice].vector_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].vector_osc[osc].doOscillate();
          }
          // chiptune osc
          else if (*m_osc_type[osc] == OSC_TYPE_CHIPTUNE) {
            m_voice[voice].chiptune_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].chiptune_osc[osc].doOscillate();
          }
          // fm osc
          else if (*m_osc_type[osc] == OSC_TYPE_FM) {
            m_voice[voice].fm_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].fm_osc[osc].doOscillate();
          }
          // noise osc
          else if (*m_osc_type[osc] == OSC_TYPE_NOISE) {
            m_osc_output[voice][osc] += m_voice[voice].noise_osc[osc].doNoise();
          }
          // wavedraw osc
          else if (*m_osc_type[osc] == OSC_TYPE_WAVEDRAW) {
            m_voice[voice].wavedraw_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].wavedraw_osc[osc].doOscillate();
          }
          // chipdraw osc
          else if (*m_osc_type[osc] == OSC_TYPE_CHIPDRAW) {
            m_voice[voice].chipdraw_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].chipdraw_osc[osc].doOscillate();
          }
          // chipdraw osc
          else if (*m_osc_type[osc] == OSC_TYPE_SPECDRAW) {
            m_voice[voice].specdraw_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].specdraw_osc[osc].doOscillate();
          }
          // apply volume
          m_osc_output[voice][osc] *= m_osc_vol_smooth[osc];
        } // osc loop

        //===== FILTERS ======

        float filter_input[2] = {0};
        memset(m_filter_output, 0, sizeof(float) * VOICES * 2);
        m_voice[voice].setFilterEnvValue(
            m_adsr[voice][1]); // can be split up to individual filters
        for (int fil = 0; fil < 2; ++fil) {
          // get filter inputs, fil1->fil2 is done at the end of fil1 calc
          if (*m_fil_osc1[fil]) {
            filter_input[fil] += m_osc_output[voice][0];
          }
          if (*m_fil_osc2[fil]) {
            filter_input[fil] += m_osc_output[voice][1];
          }
          if (*m_fil_osc3[fil]) {
            filter_input[fil] += m_osc_output[voice][2];
          }

          if (*m_fil_type[fil] == FILTER_TYPE_NONE) {
            m_filter_output[voice][fil] = filter_input[fil];
          } else if (*m_fil_type[fil] == FILTER_TYPE_LP24 ||
                     *m_fil_type[fil] == FILTER_TYPE_LP12 ||
                     *m_fil_type[fil] == FILTER_TYPE_BP24 ||
                     *m_fil_type[fil] == FILTER_TYPE_BP12 ||
                     *m_fil_type[fil] == FILTER_TYPE_HP24 ||
                     *m_fil_type[fil] == FILTER_TYPE_HP12) {
            m_voice[voice].ladder_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].ladder_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].ladder_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (*m_fil_type[fil] == FILTER_TYPE_SEM24) {
            m_voice[voice].SEM_filter_24[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].SEM_filter_24[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].SEM_filter_24[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];

          } else if (*m_fil_type[fil] == FILTER_TYPE_SEM12) {
            m_voice[voice].SEM_filter_12[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            // todo need params set to wörk...
            // m_voice[voice].SEM_filter_12[fil].setResControl(0);
            // m_voice[voice].SEM_filter_12[fil].m_freq_base = 2000.f;
            // m_voice[voice].SEM_filter_12[fil].m_overdrive = 0.f;
            // m_voice[voice].SEM_filter_12[fil].m_transition = -1.f;
            m_voice[voice].SEM_filter_12[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].SEM_filter_12[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (*m_fil_type[fil] == FILTER_TYPE_KORG) {
            m_voice[voice].korg_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].korg_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].korg_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (*m_fil_type[fil] == FILTER_TYPE_DIODE) {
            m_voice[voice].diode_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].diode_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].diode_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (*m_fil_type[fil] == FILTER_TYPE_FORMANT) {
            m_voice[voice].formant_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].formant_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].formant_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (*m_fil_type[fil] == FILTER_TYPE_COMB) {
            m_voice[voice].comb_filter[fil].setCombFreq(m_fil_freq_smooth[fil]);
            m_filter_output[voice][fil] =
                m_voice[voice].comb_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          }

          // add first filter to second filter input
          if (fil == 0 && *m_fil2_fil1) {
            filter_input[1] += m_filter_output[voice][0];
          }
        } // filter loop

        if (*m_fil1_to_amp) {
          voices_output += m_filter_output[voice][0] * m_adsr[voice][0];
        }
        if (*m_fil2_to_amp) {
          voices_output += m_filter_output[voice][1] * m_adsr[voice][0];
        }

      } // voice active
    }   // voice loop

    //===== AMPLIFIER ======

    float stereo_signal[2];

    m_amp.doAmplifier(voices_output, stereo_signal[0], stereo_signal[1]);

    for (int channel = 0; channel < 2; ++channel) {

      //===== DISTORTION ======
      if (*m_dist_on) {
        stereo_signal[channel] =
            m_distortion[channel].doDistortion(stereo_signal[channel]);
      }

      //===== FILTER 3 ======
      if (*m_fil_type[2] == FILTER_TYPE_LP24 ||
          *m_fil_type[2] == FILTER_TYPE_LP12 ||
          *m_fil_type[2] == FILTER_TYPE_BP24 ||
          *m_fil_type[2] == FILTER_TYPE_BP12 ||
          *m_fil_type[2] == FILTER_TYPE_HP24 ||
          *m_fil_type[2] == FILTER_TYPE_HP12) {
        m_ladder_filter[channel].m_freq_base =
            m_fil_freq_smooth[2];
        m_ladder_filter[channel].update();
        stereo_signal[channel] =
            m_ladder_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (*m_fil_type[2] == FILTER_TYPE_SEM24) {
        m_SEM_filter_24[channel].m_freq_base =
            m_fil_freq_smooth[2];
        m_SEM_filter_24[channel].update();
        stereo_signal[channel] =
            m_SEM_filter_24[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (*m_fil_type[2] == FILTER_TYPE_SEM12) {
        m_SEM_filter_12[channel].m_freq_base =
            m_fil_freq_smooth[2];
        m_SEM_filter_12[channel].update();
        stereo_signal[channel] =
            m_SEM_filter_12[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (*m_fil_type[2] == FILTER_TYPE_KORG) {
        m_korg_filter[channel].m_freq_base =
            m_fil_freq_smooth[2];
        m_korg_filter[channel].update();
        stereo_signal[channel] =
            m_korg_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (*m_fil_type[2] == FILTER_TYPE_DIODE) {
        m_diode_filter[channel].m_freq_base =
            m_fil_freq_smooth[2];
        m_diode_filter[channel].update();
        stereo_signal[channel] =
            m_diode_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (*m_fil_type[2] == FILTER_TYPE_FORMANT) {
        m_formant_filter[channel].m_freq_base =
            m_fil_freq_smooth[2];
        m_formant_filter[channel].update();
        stereo_signal[channel] =
            m_formant_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (*m_fil_type[2] == FILTER_TYPE_COMB) {
        m_comb_filter[channel].setCombFreq(m_fil_freq_smooth[2]);
        stereo_signal[channel] =
            m_comb_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      }

      //==== FX SECTION ====

      // ugly solution, yet here we go
      for (int fx_slot = 0; fx_slot < 4; ++fx_slot) {
        if ((int)*m_delay_position == fx_slot) {
          if (*m_delay_on) {
            stereo_signal[channel] =
                m_delay[channel].doDelay(stereo_signal[channel]);
          }
        } else if ((int)*m_phaser_position == fx_slot) {
          if (*m_phaser_on) {
            stereo_signal[channel] =
                m_phaser[channel].doPhaser(stereo_signal[channel]);
          }
        } else if ((int)*m_flanger_position == fx_slot) {
          if (*m_flanger_on) {
            stereo_signal[channel] =
                m_flanger[channel].doFlanger(stereo_signal[channel]);
          }
        } else if ((int)*m_chorus_position == fx_slot) {
          if (*m_chorus_on) {
            stereo_signal[channel] =
                m_chorus[channel].doChorus(stereo_signal[channel]);
          }
        }
      }

      //===== OUTPUT ======

      auto *channelData = buffer.getWritePointer(channel);
      channelData[sample] = stereo_signal[channel];

    } // stereo loop
  }   // sample loop
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

void OdinAudioProcessor::setSampleRate(float p_samplerate) {
  // todo set ALL samplerates here and check where the host sets them
  for (int voice = 0; voice < VOICES; ++voice) {
    m_voice[voice].setSampleRate(p_samplerate);
  }

  m_delay[0].setSampleRate(p_samplerate);
  m_delay[1].setSampleRate(p_samplerate);
  m_phaser[0].setSamplerate(p_samplerate);
  m_phaser[1].setSamplerate(p_samplerate);
  m_flanger[0].setSamplerate(p_samplerate);
  m_flanger[1].setSamplerate(p_samplerate);
  m_chorus[0].setSamplerate(p_samplerate);
  m_chorus[1].setSamplerate(p_samplerate);
}

void OdinAudioProcessor::initializeModules() {
  m_mod_matrix.setSourcesAndDestinations(&m_mod_sources, &m_mod_destinations);

  m_phaser[0].setBaseFreq(18000.f);
  m_phaser[1].setBaseFreq(18000.f);
  m_phaser[0].setRadiusBase(1.25f);
  m_phaser[1].setRadiusBase(1.25f);

  setModulationPointers();
}

void OdinAudioProcessor::setModulationPointers() {
  //========================================
  //==============  SOURCES  ===============
  //========================================

  for (int voice = 0; voice < VOICES; ++voice) {
    m_mod_sources.voice[voice].osc[0] = &(m_osc_output[voice][0]);
    m_mod_sources.voice[voice].osc[1] = &(m_osc_output[voice][1]);
    m_mod_sources.voice[voice].osc[2] = &(m_osc_output[voice][2]);
    m_mod_sources.voice[voice].filter[0] = &(m_filter_output[voice][0]);
    m_mod_sources.voice[voice].filter[1] = &(m_filter_output[voice][1]);
    m_mod_sources.voice[voice].adsr[0] = &(m_adsr[voice][0]);
    m_mod_sources.voice[voice].adsr[1] = &(m_adsr[voice][1]);
    m_mod_sources.voice[voice].adsr[2] = &(m_adsr[voice][2]);
    m_mod_sources.voice[voice].adsr[3] = &(m_adsr[voice][3]);
    m_mod_sources.voice[voice].lfo[0] = &(m_lfo[voice][0]);
    m_mod_sources.voice[voice].lfo[1] = &(m_lfo[voice][1]);
    m_mod_sources.voice[voice].lfo[2] = &(m_lfo[voice][2]);
    m_mod_sources.voice[voice].lfo[3] = &(m_lfo[voice][3]);
    m_mod_sources.voice[voice].MIDI_key = &(m_voice[voice].MIDI_key_mod_source);
    m_mod_sources.voice[voice].MIDI_velocity =
        &(m_voice[voice].MIDI_velocity_mod_source);
  }
  m_mod_sources.MIDI_aftertouch = &(m_MIDI_aftertouch);
  m_mod_sources.x = &m_x_smooth;
  m_mod_sources.y = &m_y_smooth;
  m_mod_sources.modwheel = &m_modwheel_smooth;
  m_mod_sources.pitchwheel = &m_pitch_bend_smooth_and_applied;
  m_mod_sources.constant = &(m_constant);

  //========================================
  //============= DESTINATIONS =============
  //========================================
  for (int voice = 0; voice < VOICES; ++voice) {
    for (int osc = 0; osc < 3; ++osc) {
      m_voice[voice].analog_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].wavetable_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].multi_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].vector_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].fm_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].chiptune_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].wavedraw_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].chipdraw_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));
      m_voice[voice].specdraw_osc[osc].setPitchBendPointer(
          &(m_pitch_bend_smooth_and_applied));

      m_voice[voice].analog_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].wavetable_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].multi_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].vector_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].fm_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].chiptune_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].wavedraw_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].chipdraw_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));
      m_voice[voice].specdraw_osc[osc].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_exponential));

      m_voice[voice].analog_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].wavetable_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].multi_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].vector_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].fm_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].chiptune_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].wavedraw_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].chipdraw_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));
      m_voice[voice].specdraw_osc[osc].setPitchModLinPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pitch_linear));

      m_voice[voice].analog_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].wavetable_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].multi_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].vector_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].fm_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].chiptune_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].wavedraw_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].chipdraw_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].specdraw_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));
      m_voice[voice].noise_osc[osc].setVolModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].vol));

      m_voice[voice].analog_osc[osc].setPWMModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].pulse_width));

      m_voice[voice].wavetable_osc[osc].setPosModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].position));
      m_voice[voice].multi_osc[osc].setPosModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].position));

      m_voice[voice].multi_osc[osc].setDetuneModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].detune));

      m_voice[voice].multi_osc[osc].setSpreadModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].spread));

      m_voice[voice].vector_osc[osc].setXModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].x));
      m_voice[voice].vector_osc[osc].setYModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].y));

      m_voice[voice].chiptune_osc[osc].setArpSpeedModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].arp_speed));

      m_voice[voice].fm_osc[osc].setFMModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].fm_amount));

      m_voice[voice].fm_osc[osc].setCarrierRatioModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].carrier_ratio));
      m_voice[voice].fm_osc[osc].setModulatorRatioModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].modulator_ratio));

      m_voice[voice].noise_osc[osc].setHPModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].hp_freq));
      m_voice[voice].noise_osc[osc].setLPModPointer(
          &(m_mod_destinations.voice[voice].osc[osc].lp_freq));
    }
    for (int fil = 0; fil < 2; ++fil) {
      m_voice[voice].ladder_filter[fil].setFreqModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].freq));
      m_voice[voice].diode_filter[fil].setFreqModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].freq));
      m_voice[voice].korg_filter[fil].setFreqModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].freq));
      m_voice[voice].comb_filter[fil].setFreqModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].freq));
      m_voice[voice].SEM_filter_12[fil].setFreqModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].freq));
      m_voice[voice].SEM_filter_24[fil].setFreqModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].freq));

      m_voice[voice].ladder_filter[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].diode_filter[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].korg_filter[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].SEM_filter_12[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].SEM_filter_24[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].comb_filter[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));

      m_voice[voice].ladder_filter[fil].setVolModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].gain));
      m_voice[voice].diode_filter[fil].setVolModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].gain));
      m_voice[voice].korg_filter[fil].setVolModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].gain));
      m_voice[voice].SEM_filter_12[fil].setVolModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].gain));
      m_voice[voice].SEM_filter_24[fil].setVolModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].gain));
      m_voice[voice].comb_filter[fil].setVolModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].gain));
      m_voice[voice].formant_filter[fil].setVolModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].gain));

      m_voice[voice].ladder_filter[fil].setEnvModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].env_amount));
      m_voice[voice].diode_filter[fil].setEnvModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].env_amount));
      m_voice[voice].korg_filter[fil].setEnvModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].env_amount));
      m_voice[voice].SEM_filter_12[fil].setEnvModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].env_amount));
      m_voice[voice].SEM_filter_24[fil].setEnvModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].env_amount));
      m_voice[voice].comb_filter[fil].setEnvModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].env_amount));
      m_voice[voice].formant_filter[fil].setEnvModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].env_amount));

      m_voice[voice].ladder_filter[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
      m_voice[voice].diode_filter[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
      m_voice[voice].korg_filter[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
      m_voice[voice].SEM_filter_12[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
      m_voice[voice].SEM_filter_24[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
      m_voice[voice].comb_filter[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));

      m_voice[voice].ladder_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].diode_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].korg_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].SEM_filter_12[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].SEM_filter_24[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].comb_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].formant_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));

      m_voice[voice].ladder_filter[fil].setSaturationModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].saturation));
      m_voice[voice].diode_filter[fil].setSaturationModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].saturation));
      m_voice[voice].korg_filter[fil].setSaturationModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].saturation));

      m_voice[voice].SEM_filter_12[fil].setTransitionModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].SEM_transition));
    }

    for (int mod = 0; mod < 4; ++mod) {
      m_voice[voice].lfo[mod].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].lfo[mod].freq));
    }
  }

  for (int stereo = 0; stereo < 2; ++stereo) {

    // todo ADD ALL DESTINATIONS FROM VOICE SECTIONS FOR FILTER3
    m_ladder_filter[stereo].setFreqModPointer(
        &(m_mod_destinations.filter3.freq));
    m_diode_filter[stereo].setFreqModPointer(
        &(m_mod_destinations.filter3.freq));
    m_korg_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
    m_comb_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
    m_SEM_filter_12[stereo].setFreqModPointer(
        &(m_mod_destinations.filter3.freq));
    m_SEM_filter_24[stereo].setFreqModPointer(
        &(m_mod_destinations.filter3.freq));

    m_ladder_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_diode_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_korg_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_SEM_filter_12[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_SEM_filter_24[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_comb_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));

    m_ladder_filter[stereo].setVolModPointer(
        &(m_mod_destinations.filter3.gain));
    m_diode_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
    m_korg_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
    m_SEM_filter_12[stereo].setVolModPointer(
        &(m_mod_destinations.filter3.gain));
    m_SEM_filter_24[stereo].setVolModPointer(
        &(m_mod_destinations.filter3.gain));
    m_comb_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
    m_formant_filter[stereo].setVolModPointer(
        &(m_mod_destinations.filter3.gain));

    m_ladder_filter[stereo].setEnvModPointer(
        &(m_mod_destinations.filter3.env_amount));
    m_diode_filter[stereo].setEnvModPointer(
        &(m_mod_destinations.filter3.env_amount));
    m_korg_filter[stereo].setEnvModPointer(
        &(m_mod_destinations.filter3.env_amount));
    m_SEM_filter_12[stereo].setEnvModPointer(
        &(m_mod_destinations.filter3.env_amount));
    m_SEM_filter_24[stereo].setEnvModPointer(
        &(m_mod_destinations.filter3.env_amount));
    m_comb_filter[stereo].setEnvModPointer(
        &(m_mod_destinations.filter3.env_amount));
    m_formant_filter[stereo].setEnvModPointer(
        &(m_mod_destinations.filter3.env_amount));

    m_ladder_filter[stereo].setVelModPointer(
        &(m_mod_destinations.filter3.vel_amount));
    m_diode_filter[stereo].setVelModPointer(
        &(m_mod_destinations.filter3.vel_amount));
    m_korg_filter[stereo].setVelModPointer(
        &(m_mod_destinations.filter3.vel_amount));
    m_SEM_filter_12[stereo].setVelModPointer(
        &(m_mod_destinations.filter3.vel_amount));
    m_SEM_filter_24[stereo].setVelModPointer(
        &(m_mod_destinations.filter3.vel_amount));
    m_comb_filter[stereo].setVelModPointer(
        &(m_mod_destinations.filter3.vel_amount));

    m_ladder_filter[stereo].setKbdModPointer(
        &(m_mod_destinations.filter3.kbd_amount));
    m_diode_filter[stereo].setKbdModPointer(
        &(m_mod_destinations.filter3.kbd_amount));
    m_korg_filter[stereo].setKbdModPointer(
        &(m_mod_destinations.filter3.kbd_amount));
    m_SEM_filter_12[stereo].setKbdModPointer(
        &(m_mod_destinations.filter3.kbd_amount));
    m_SEM_filter_24[stereo].setKbdModPointer(
        &(m_mod_destinations.filter3.kbd_amount));
    m_comb_filter[stereo].setKbdModPointer(
        &(m_mod_destinations.filter3.kbd_amount));
    m_formant_filter[stereo].setKbdModPointer(
        &(m_mod_destinations.filter3.kbd_amount));

    m_ladder_filter[stereo].setSaturationModPointer(
        &(m_mod_destinations.filter3.saturation));
    m_diode_filter[stereo].setSaturationModPointer(
        &(m_mod_destinations.filter3.saturation));
    m_korg_filter[stereo].setSaturationModPointer(
        &(m_mod_destinations.filter3.saturation));

    m_SEM_filter_12[stereo].setTransitionModPointer(
        &(m_mod_destinations.filter3.SEM_transition));

    m_delay[stereo].setHPFreqModPointer(&(m_mod_destinations.delay.hp_freq));
  }

    

}