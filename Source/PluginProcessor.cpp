/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <typeinfo>

#include "PluginEditor.h"
#include "PluginProcessor.h"

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
    : m_value_tree(*this, nullptr, Identifier("Odin"),
#include "AudioValueTree.h" //contains the definition of audiotree. WATCH CLOSELY: is IN m_value_tree constructor
                   ),
#include "ProcessorInitializerList.h" //contains the connection of Identifiers with their strings
{
#include "AudioParameterConnections.h" // constains the connection between raw float pointers and their ValueTree counter

  addNonAudioParametersToTree();

  m_is_standalone_plugin = (wrapperType == wrapperType_Standalone);
  DBG("standalone plugin? " + std::to_string((int)m_is_standalone_plugin));
  // DBG(typeid(wrapperType));
  // DBG(typeid(wrapperType_Standalone));

  // set up the tree listener
  m_tree_listener.onValueChange = [&](const String &p_ID, float p_new_value) {
    if (!treeValueChangedFirst(p_ID, p_new_value)) {
      if (!treeValueChangedSecond(p_ID, p_new_value)) // no change in first
      {
        if (!treeValueChangedThird(p_ID, p_new_value)) // no change in 1st + 2nd
        {
          treeValueChangedFourth(p_ID, p_new_value); // nochange in 1+2+3
        }
      }
    }
  };
  m_non_param_listener.onValueChange = [&](ValueTree &tree,
                                           const Identifier &identifier) {
    treeValueChangedNonParam(tree, identifier);
  };

  setSampleRate(44100.f);
  initializeModules();

  // create wavetables
  // WavetableContainer::getInstance().writeWavetablesToFiles();//use this to
  // write the tables to header files
  // WavetableContainer::getInstance().generateAudioValueCode();

  WavetableContainer::getInstance().loadWavetablesFromConstData();

  // WavetableContainer::getInstance().createWavetables(44100.f);
  // WavetableContainer::getInstance().loadWavetablesAfterFourierCreation();

  // create draw tables as well
  float draw_values[WAVEDRAW_STEPS_X];
  float spec_values[SPECDRAW_STEPS_X] = {0};
  float chip_values[CHIPDRAW_STEPS_X] = {0};
  spec_values[0] = 1.f;
  for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
    draw_values[i] = sin((float)i * 2 * M_PI / WAVEDRAW_STEPS_X) * 0.9;
  }
  for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
    if (i < CHIPDRAW_STEPS_X / 2) {
      chip_values[i] = 1;
    } else {
      chip_values[i] = -1;
    }
  }
  for (int osc = 0; osc < 3; ++osc) {
    WavetableContainer::getInstance().createWavedrawTable(osc, draw_values,
                                                          44100);
    WavetableContainer::getInstance().createChipdrawTable(osc, chip_values,
                                                          44100);
    WavetableContainer::getInstance().createSpecdrawTable(osc, spec_values,
                                                          44100);
  }

  // WavetableContainer::getInstance().createLFOtables(44100.);

  // WavetableContainer::getInstance().writeLFOtablesToFiles();//use this to

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
    m_global_lfo.loadWavetables();
  }
  for (int voice = 0; voice < VOICES; ++voice) {
    // DBG("pointer to voice " + std::to_string(voice) +
    //    " is: " + std::to_string((long)&m_voice[voice]));
    // DBG("pointer to bool " + std::to_string(voice) +
    //    " is: " + std::to_string((long)&(m_voice[voice].m_voice_active)));
    m_voice[voice].env[0].setEnvelopeEndPointers(
        &(m_voice[voice].m_voice_active), &(m_voice_manager.voice_busy[voice]));
  }

  // WavetableContainer::getInstance().createLFOCoefficientsFromLinSections(12,
  // spike, 1000, "Spike");
}

OdinAudioProcessor::~OdinAudioProcessor() {
  // WavetableContainer::getInstance().destroyWavetables();
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
  // m_voice[0].start(52, 100, 10);
  // m_amp.setMIDIVelocity(100);
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

  // get BPM info from host
  if (!m_is_standalone_plugin) {
    if (AudioPlayHead *playhead = getPlayHead()) {
      AudioPlayHead::CurrentPositionInfo current_position_info;
      playhead->getCurrentPosition(current_position_info);
      m_BPM = current_position_info.bpm;
      //todo this is expensive???
      SETVALUE("BPM", m_BPM);
    }
  }
  setBPM(m_BPM);

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

    m_pitch_bend_smooth = m_pitch_bend_smooth * PITCHBEND_SMOOTHIN_FACTOR +
                          (1.f - PITCHBEND_SMOOTHIN_FACTOR) * (*m_pitchbend);
    m_pitch_bend_smooth_and_applied =
        m_pitch_bend_smooth * (*m_pitchbend_amount);

    m_modwheel_smooth = m_modwheel_smooth * PITCHBEND_SMOOTHIN_FACTOR +
                        (1.f - PITCHBEND_SMOOTHIN_FACTOR) * (*m_modwheel);

    m_x_smooth = m_x_smooth * PAD_SMOOTHIN_FACTOR +
                 (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_x);
    m_y_smooth = m_y_smooth * PAD_SMOOTHIN_FACTOR +
                 (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_y);

    m_master_smooth = m_master_smooth * GAIN_SMOOTHIN_FACTOR +
                      (1 - GAIN_SMOOTHIN_FACTOR) * (m_master_control);

    //===== MIDI =====
    if (midi_message_remaining) {
      if (midi_message_sample <= sample) {
        // apply midi message
        if (midi_message.isNoteOn()) {
          midiNoteOn(midi_message.getNoteNumber(), midi_message.getVelocity());
        } else if (midi_message.isNoteOff()) {

          midiNoteOff(midi_message.getNoteNumber());
        } else if (midi_message.isPitchWheel()) {
          setPitchWheelValue(midi_message.getPitchWheelValue());
        } else if (midi_message.isSustainPedalOn()) {
          m_voice_manager.setSustainActive(true);
          DBG("Sustain pedal pressed");
        } else if (midi_message.isSustainPedalOff()) {
          DBG("Sustain pedal released");
          m_voice_manager.setSustainActive(false);
          for (int voice = 0; voice < VOICES; ++voice) {
            if (m_voice_manager.isOnKillList(voice)) {
              m_voice[voice].startRelease();
            }
          }
          m_voice_manager.clearKillList();
          checkEndGlobalEnvelope();
        } else if (midi_message.isAftertouch()) {
          // todo this is untested, are values set back to zero, or need to do
          // it manually?
          m_MIDI_aftertouch = (float)midi_message.getAfterTouchValue() / 127.f;
        } else {
          DBG("UNHANDELED MIDI MESSAGE: " + midi_message.getDescription());
        }

        if ((midi_message.isController() || midi_message.isPitchWheel()) /* &&
            !midi_message.isSustainPedalOn() &&
            !midi_message.isSustainPedalOff()*/
        ) {
          DBG("CONTROLLER");
          if (m_midi_learn_knob_active) {
            m_midi_control_list_knob.emplace(midi_message.getControllerNumber(),
                                             m_midi_learn_knob);
            m_midi_learn_knob->setMidiControlActive();
            m_midi_learn_knob_active = false;
            m_midi_learn_knob = nullptr;
            DBG("Added MIDI control on controller number " +
                std::to_string(midi_message.getControllerNumber()));
          }
          if (m_midi_learn_slider_active) {
            m_midi_control_list_slider.emplace(
                midi_message.getControllerNumber(), m_midi_learn_slider);
            m_midi_learn_slider->setMidiControlActive();
            m_midi_learn_slider_active = false;
            m_midi_learn_slider = nullptr;
            DBG("Added MIDI control on controller number " +
                std::to_string(midi_message.getControllerNumber()));
          }
          if (m_midi_learn_lrbutton_active) {
            m_midi_control_list_lrbutton.emplace(
                midi_message.getControllerNumber(), m_midi_learn_lrbutton);
            m_midi_learn_lrbutton->setMidiControlActive();
            m_midi_learn_lrbutton_active = false;
            m_midi_learn_lrbutton = nullptr;
            DBG("Added MIDI control on controller number " +
                std::to_string(midi_message.getControllerNumber()));
          }
          if (m_midi_learn_odinbutton_active) {
            m_midi_control_list_odinbutton.emplace(
                midi_message.getControllerNumber(), m_midi_learn_odinbutton);
            m_midi_learn_odinbutton->setMidiControlActive();
            m_midi_learn_odinbutton_active = false;
            m_midi_learn_odinbutton = nullptr;
            DBG("Added MIDI control on controller number " +
                std::to_string(midi_message.getControllerNumber()));
          }

          // do midi control
          for (auto const &control : m_midi_control_list_knob) {
            if (control.first == midi_message.getControllerNumber()) {
              const MessageManagerLock mmLock;
              control.second->setValue(
                  control.second->proportionOfLengthToValue(
                      (int)midi_message.getControllerValue() / 127.f));
            }
          }
          for (auto const &control : m_midi_control_list_slider) {
            if (control.first == midi_message.getControllerNumber()) {
              const MessageManagerLock mmLock;
              control.second->setValue(
                  control.second->proportionOfLengthToValue(
                      (int)midi_message.getControllerValue() / 127.f));
            }
          }
          for (auto const &control : m_midi_control_list_lrbutton) {
            if (control.first == midi_message.getControllerNumber()) {
              const MessageManagerLock mmLock;
              control.second->setToggleState(
                  (int)midi_message.getControllerValue() > 64,
                  sendNotificationAsync);
            }
          }
          for (auto const &control : m_midi_control_list_odinbutton) {
            if (control.first == midi_message.getControllerNumber()) {
              const MessageManagerLock mmLock;
              control.second->setToggleState(
                  (int)midi_message.getControllerValue() > 64,
                  sendNotificationAsync);
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

    // global lfo and envelope
    m_global_env_mod_source = m_global_env.doEnvelope();
    // DBG(m_global_env_mod_source);

    m_global_lfo.update();
    m_global_lfo_mod_source = m_global_lfo.doOscillate();

    // loop over all voices
    for (int voice = 0; voice < VOICES; ++voice) {
      if (m_voice[voice]) {

        for (int mod = 0; mod < 3; ++mod) {
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
          if (m_osc_type[osc] == OSC_TYPE_ANALOG) {
            m_voice[voice].analog_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].analog_osc[osc].doOscillate();
          }
          // wavetable osc
          else if (m_osc_type[osc] == OSC_TYPE_WAVETABLE) {
            // m_voice[voice].wavetable_osc[osc].setPosition(0);
            // m_voice[voice].wavetable_osc[osc].selectWavetable(0);
            m_voice[voice].wavetable_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].wavetable_osc[osc].doOscillate();
          }
          // multi osc
          else if (m_osc_type[osc] == OSC_TYPE_MULTI) {
            m_voice[voice].multi_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].multi_osc[osc].doOscillate();
          }
          // vector osc
          else if (m_osc_type[osc] == OSC_TYPE_VECTOR) {
            m_voice[voice].vector_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].vector_osc[osc].doOscillate();
          }
          // chiptune osc
          else if (m_osc_type[osc] == OSC_TYPE_CHIPTUNE) {
            m_voice[voice].chiptune_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].chiptune_osc[osc].doOscillate();
          }
          // fm osc
          else if (m_osc_type[osc] == OSC_TYPE_FM) {
            m_voice[voice].fm_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].fm_osc[osc].doOscillate();
          }
          // noise osc
          else if (m_osc_type[osc] == OSC_TYPE_NOISE) {
            m_osc_output[voice][osc] += m_voice[voice].noise_osc[osc].doNoise();
          }
          // wavedraw osc
          else if (m_osc_type[osc] == OSC_TYPE_WAVEDRAW) {
            m_voice[voice].wavedraw_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].wavedraw_osc[osc].doOscillate();
          }
          // chipdraw osc
          else if (m_osc_type[osc] == OSC_TYPE_CHIPDRAW) {
            m_voice[voice].chipdraw_osc[osc].update();
            m_osc_output[voice][osc] +=
                m_voice[voice].chipdraw_osc[osc].doOscillate();
          }
          // chipdraw osc
          else if (m_osc_type[osc] == OSC_TYPE_SPECDRAW) {
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

          if (m_fil_type[fil] == FILTER_TYPE_NONE) {
            m_filter_output[voice][fil] = filter_input[fil];
          } else if (m_fil_type[fil] == FILTER_TYPE_LP24 ||
                     m_fil_type[fil] == FILTER_TYPE_LP12 ||
                     m_fil_type[fil] == FILTER_TYPE_BP24 ||
                     m_fil_type[fil] == FILTER_TYPE_BP12 ||
                     m_fil_type[fil] == FILTER_TYPE_HP24 ||
                     m_fil_type[fil] == FILTER_TYPE_HP12) {
            m_voice[voice].ladder_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].ladder_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].ladder_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (m_fil_type[fil] == FILTER_TYPE_SEM12) {
            m_voice[voice].SEM_filter_12[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].SEM_filter_12[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].SEM_filter_12[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (m_fil_type[fil] == FILTER_TYPE_KORG_LP ||
                     m_fil_type[fil] == FILTER_TYPE_KORG_HP) {
            m_voice[voice].korg_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].korg_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].korg_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (m_fil_type[fil] == FILTER_TYPE_DIODE) {
            m_voice[voice].diode_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].diode_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].diode_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (m_fil_type[fil] == FILTER_TYPE_FORMANT) {
            m_voice[voice].formant_filter[fil].m_freq_base =
                m_fil_freq_smooth[fil];
            m_voice[voice].formant_filter[fil].update();
            m_filter_output[voice][fil] =
                m_voice[voice].formant_filter[fil].doFilter(filter_input[fil]) *
                m_fil_gain_smooth[fil];
          } else if (m_fil_type[fil] == FILTER_TYPE_COMB) {
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
      if (m_fil_type[2] == FILTER_TYPE_LP24 ||
          m_fil_type[2] == FILTER_TYPE_LP12 ||
          m_fil_type[2] == FILTER_TYPE_BP24 ||
          m_fil_type[2] == FILTER_TYPE_BP12 ||
          m_fil_type[2] == FILTER_TYPE_HP24 ||
          m_fil_type[2] == FILTER_TYPE_HP12) {
        m_ladder_filter[channel].m_freq_base = m_fil_freq_smooth[2];
        m_ladder_filter[channel].update();
        stereo_signal[channel] =
            m_ladder_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (m_fil_type[2] == FILTER_TYPE_SEM12) {
        m_SEM_filter_12[channel].m_freq_base = m_fil_freq_smooth[2];
        m_SEM_filter_12[channel].update();
        stereo_signal[channel] =
            m_SEM_filter_12[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (m_fil_type[2] == FILTER_TYPE_KORG_LP ||
                 m_fil_type[2] == FILTER_TYPE_KORG_HP) {
        m_korg_filter[channel].m_freq_base = m_fil_freq_smooth[2];
        m_korg_filter[channel].update();
        stereo_signal[channel] =
            m_korg_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (m_fil_type[2] == FILTER_TYPE_DIODE) {
        m_diode_filter[channel].m_freq_base = m_fil_freq_smooth[2];
        m_diode_filter[channel].update();
        stereo_signal[channel] =
            m_diode_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (m_fil_type[2] == FILTER_TYPE_FORMANT) {
        m_formant_filter[channel].m_freq_base = m_fil_freq_smooth[2];
        m_formant_filter[channel].update();
        stereo_signal[channel] =
            m_formant_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      } else if (m_fil_type[2] == FILTER_TYPE_COMB) {
        m_comb_filter[channel].setCombFreq(m_fil_freq_smooth[2]);
        stereo_signal[channel] =
            m_comb_filter[channel].doFilter(stereo_signal[channel]) *
            m_fil_gain_smooth[2];
      }

      //==== FX SECTION ====

      // ugly solution, yet here we go
      for (int fx_slot = 0; fx_slot < 4; ++fx_slot) {
        if (m_delay_position == fx_slot) {
          if (*m_delay_on) {
            stereo_signal[channel] =
                m_delay[channel].doDelay(stereo_signal[channel]);
          }
        } else if (m_phaser_position == fx_slot) {
          if (*m_phaser_on) {
            if (channel == 0) {
              stereo_signal[channel] =
                  m_phaser.doPhaserLeft(stereo_signal[channel]);
            } else {
              stereo_signal[channel] =
                  m_phaser.doPhaserRight(stereo_signal[channel]);
            }
          }
        } else if (m_flanger_position == fx_slot) {
          if (*m_flanger_on) {
            stereo_signal[channel] =
                m_flanger[channel].doFlanger(stereo_signal[channel]);
          }
        } else if (m_chorus_position == fx_slot) {
          if (*m_chorus_on) {
            stereo_signal[channel] =
                m_chorus[channel].doChorus(stereo_signal[channel]);
          }
        }
      }

      //===== OUTPUT ======

      float master_mod_factor = (*m_master_mod) > 0 ? 1.f + 4 * (*m_master_mod)
                                                    : (1.f + *m_master_mod);

      auto *channelData = buffer.getWritePointer(channel);
      channelData[sample] =
          stereo_signal[channel] * m_master_smooth * master_mod_factor;
      // DBG(m_master_smooth);

    } // stereo loop
  }   // sample loop
}

//==============================================================================
bool OdinAudioProcessor::hasEditor() const { return true; }

AudioProcessorEditor *OdinAudioProcessor::createEditor() {
  OdinAudioProcessorEditor* editor =
      new OdinAudioProcessorEditor(*this, m_value_tree, m_is_standalone_plugin);

  // typeid(wrapperType) == typeid(wrapperType_Standalone));
  if (m_force_values_onto_gui) {
    onSetStateInformation();
  }

  return editor;
}

//==============================================================================
void OdinAudioProcessor::getStateInformation(MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.

  // disable for standalone plugins
  if (wrapperType == wrapperType_Standalone) {
    return;
  }

  auto state = m_value_tree.copyState();
  std::unique_ptr<XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
  DBG("SET BINARY STATE!!");
}

void OdinAudioProcessor::setStateInformation(const void *data,
                                             int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.

  // disable for standalone plugins
  if (wrapperType == wrapperType_Standalone) {
    return;
  }

  std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
  if (xmlState.get() != nullptr) {
    if (xmlState->hasTagName(m_value_tree.state.getType()))
      m_value_tree.replaceState(ValueTree::fromXml(*xmlState));
    // force values on GUI
    m_force_values_onto_gui = true;
    DBG("LOADED BINARY STATE!!");

    // onSetStateInformation();
  }
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

  m_global_env.setSamplerate(p_samplerate);
  m_global_lfo.setSampleRate(p_samplerate);

  m_delay[0].setSampleRate(p_samplerate);
  m_delay[1].setSampleRate(p_samplerate);
  m_phaser.setSamplerate(p_samplerate);
  m_flanger[0].setSamplerate(p_samplerate);
  m_flanger[1].setSamplerate(p_samplerate);
  m_chorus[0].setSamplerate(p_samplerate);
  m_chorus[1].setSamplerate(p_samplerate);
}

void OdinAudioProcessor::initializeModules() {
  m_global_env.reset();
  m_global_env.setEnvelopeOff(); // so it doesn't start by itself

  m_mod_matrix.setSourcesAndDestinations(&m_mod_sources, &m_mod_destinations);

  m_phaser.setRadiusBase(1.25f);

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
    m_mod_sources.voice[voice].lfo[0] = &(m_lfo[voice][0]);
    m_mod_sources.voice[voice].lfo[1] = &(m_lfo[voice][1]);
    m_mod_sources.voice[voice].lfo[2] = &(m_lfo[voice][2]);
    m_mod_sources.voice[voice].MIDI_key = &(m_voice[voice].MIDI_key_mod_source);
    m_mod_sources.voice[voice].MIDI_velocity =
        &(m_voice[voice].MIDI_velocity_mod_source);
    m_mod_sources.voice[voice].random = &(m_voice[voice].random_modulation);
  }
  m_mod_sources.global_adsr = &m_global_env_mod_source;
  m_mod_sources.global_lfo = &m_global_lfo_mod_source;
  m_mod_sources.MIDI_aftertouch = &(m_MIDI_aftertouch);
  m_mod_sources.x = &m_x_smooth;
  m_mod_sources.y = &m_y_smooth;
  m_mod_sources.modwheel = &m_modwheel_smooth;
  m_mod_sources.pitchwheel = &m_pitch_bend_smooth;
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

      m_voice[voice].analog_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].wavetable_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].multi_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].vector_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].fm_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].chiptune_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].wavedraw_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].chipdraw_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));
      m_voice[voice].specdraw_osc[osc].setGlidePointer(
          &(m_mod_destinations.misc.glide));

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
      //   m_voice[voice].SEM_filter_24[fil].setFreqModPointer(
      //       &(m_mod_destinations.voice[voice].filter[fil].freq));

      m_voice[voice].ladder_filter[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].diode_filter[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].korg_filter[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      m_voice[voice].SEM_filter_12[fil].setResModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].res));
      // m_voice[voice].SEM_filter_24[fil].setResModPointer(
      //    &(m_mod_destinations.voice[voice].filter[fil].res));
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
      //   m_voice[voice].SEM_filter_24[fil].setVolModPointer(
      //       &(m_mod_destinations.voice[voice].filter[fil].gain));
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
      //   m_voice[voice].SEM_filter_24[fil].setEnvModPointer(
      //       &(m_mod_destinations.voice[voice].filter[fil].env_amount));
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
      //   m_voice[voice].SEM_filter_24[fil].setVelModPointer(
      //       &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
      m_voice[voice].comb_filter[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));
      m_voice[voice].formant_filter[fil].setVelModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].vel_amount));

      m_voice[voice].ladder_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].diode_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].korg_filter[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      m_voice[voice].SEM_filter_12[fil].setKbdModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
      //   m_voice[voice].SEM_filter_24[fil].setKbdModPointer(
      //       &(m_mod_destinations.voice[voice].filter[fil].kbd_amount));
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

      m_voice[voice].formant_filter[fil].setTransitionModPointer(
          &(m_mod_destinations.voice[voice].filter[fil].formant_transition));
    }

    for (int mod = 0; mod < 3; ++mod) {
      m_voice[voice].lfo[mod].setPitchModExpPointer(
          &(m_mod_destinations.voice[voice].lfo[mod].freq));

      m_voice[voice].env[mod].setAttackModPointer(
          &(m_mod_destinations.voice[voice].adsr[mod].attack));
      m_voice[voice].env[mod].setDecayModPointer(
          &(m_mod_destinations.voice[voice].adsr[mod].decay));
      m_voice[voice].env[mod].setSustainModPointer(
          &(m_mod_destinations.voice[voice].adsr[mod].sustain));
      m_voice[voice].env[mod].setReleaseModPointer(
          &(m_mod_destinations.voice[voice].adsr[mod].release));
    }
    m_global_lfo.setPitchModExpPointer(&(m_mod_destinations.global_lfo.freq));
    m_global_env.setAttackModPointer(&(m_mod_destinations.global_adsr.attack));
    m_global_env.setDecayModPointer(&(m_mod_destinations.global_adsr.decay));
    m_global_env.setSustainModPointer(
        &(m_mod_destinations.global_adsr.sustain));
    m_global_env.setReleaseModPointer(
        &(m_mod_destinations.global_adsr.release));
  }

  m_amp.setGainModPointer(&(m_mod_destinations.amp.gain));
  m_amp.setPanModPointer(&(m_mod_destinations.amp.pan));
  m_amp.setVelModPointer(&(m_mod_destinations.amp.vel));

  for (int stereo = 0; stereo < 2; ++stereo) {

    // todo ADD ALL DESTINATIONS FROM VOICE SECTIONS FOR FILTER3
    m_ladder_filter[stereo].setFreqModPointer(
        &(m_mod_destinations.filter3.freq));
    m_diode_filter[stereo].setFreqModPointer(
        &(m_mod_destinations.filter3.freq));
    m_korg_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
    m_comb_filter[stereo].setFreqModPointer(&(m_mod_destinations.filter3.freq));
    m_SEM_filter_12[stereo].setFreqModPointer(
        //     &(m_mod_destinations.filter3.freq));
        // m_SEM_filter_24[stereo].setFreqModPointer(
        &(m_mod_destinations.filter3.freq));

    m_ladder_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_diode_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_korg_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_SEM_filter_12[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    // m_SEM_filter_24[stereo].setResModPointer(&(m_mod_destinations.filter3.res));
    m_comb_filter[stereo].setResModPointer(&(m_mod_destinations.filter3.res));

    m_ladder_filter[stereo].setVolModPointer(
        &(m_mod_destinations.filter3.gain));
    m_diode_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
    m_korg_filter[stereo].setVolModPointer(&(m_mod_destinations.filter3.gain));
    m_SEM_filter_12[stereo].setVolModPointer(
        &(m_mod_destinations.filter3.gain));
    // m_SEM_filter_24[stereo].setVolModPointer(
    //     &(m_mod_destinations.filter3.gain));
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
    // m_SEM_filter_24[stereo].setEnvModPointer(
    //     &(m_mod_destinations.filter3.env_amount));
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
    // m_SEM_filter_24[stereo].setVelModPointer(
    //     &(m_mod_destinations.filter3.vel_amount));
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
    // m_SEM_filter_24[stereo].setKbdModPointer(
    //     &(m_mod_destinations.filter3.kbd_amount));
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

    m_formant_filter[stereo].setTransitionModPointer(
        &(m_mod_destinations.filter3.formant_transition));

    m_distortion[stereo].setThresholdModPointer(
        &(m_mod_destinations.distortion.threshold));
    m_distortion[stereo].setDryWetModPointer(
        &(m_mod_destinations.distortion.drywet));

    m_delay[stereo].setTimeModPointer(&(m_mod_destinations.delay.time));
    m_delay[stereo].setFeedbackModPointer(&(m_mod_destinations.delay.feedback));
    m_delay[stereo].setHPFreqModPointer(&(m_mod_destinations.delay.hp_freq));
    m_delay[stereo].setDryModPointer(&(m_mod_destinations.delay.dry));
    m_delay[stereo].setWetModPointer(&(m_mod_destinations.delay.wet));

    m_phaser.setRateModPointer(&(m_mod_destinations.phaser.rate));
    m_phaser.setAmountModPointer(&(m_mod_destinations.phaser.amount));
    m_phaser.setDryWetModPointer(&(m_mod_destinations.phaser.drywet));
    m_phaser.setFreqModPointer(&(m_mod_destinations.phaser.freq));
    m_phaser.setFeedbackModPointer(&(m_mod_destinations.phaser.feedback));

    m_flanger[stereo].setFreqModPointer(&(m_mod_destinations.flanger.freq));
    m_flanger[stereo].setAmountModPointer(&(m_mod_destinations.flanger.amount));
    m_flanger[stereo].setFeedbackModPointer(
        &(m_mod_destinations.flanger.feedback));
    m_flanger[stereo].setDryWetModPointer(&(m_mod_destinations.flanger.drywet));

    m_chorus[stereo].setFreqModPointer(&(m_mod_destinations.chorus.freq));
    m_chorus[stereo].setAmountModPointer(&(m_mod_destinations.chorus.amount));
    m_chorus[stereo].setFeedbackModPointer(
        &(m_mod_destinations.chorus.feedback));
    m_chorus[stereo].setDryWetModPointer(&(m_mod_destinations.chorus.drywet));

    m_master_mod = &(m_mod_destinations.misc.master);
  }
}

void OdinAudioProcessor::setPitchWheelValue(int p_value) {
  // todo this should update the GUI, lets see after MIDI learn
  *m_pitchbend = (float)(p_value - 8192) / 8192.f;
}

void OdinAudioProcessor::midiNoteOff(int p_midi_note) {
  DBG("NOTEOFF, key " + std::to_string(p_midi_note));

  if (!m_voice_manager.getSustainActive()) {
    for (int voice = 0; voice < VOICES; ++voice) {
      if (m_voice[voice].keyUp(p_midi_note)) {
        DBG("KeyUp on voice " + std::to_string(voice));
      }
    }
  } else {
    for (int voice = 0; voice < VOICES; ++voice) {
      if (m_voice[voice].usesThisMIDIKey(p_midi_note)) {
        m_voice_manager.addToKillList(voice, p_midi_note);
      }
    }
  }

  checkEndGlobalEnvelope();
}

void OdinAudioProcessor::checkEndGlobalEnvelope() {
  for (int voice = 0; voice < VOICES; ++voice) {
    if (m_voice[voice] && m_voice[voice].env[0].isBeforeRelease()) {
      // dont kill it
      return;
    }
  }
  // kill it
  m_global_env.startRelease();
  DBG("kill global env");
}

void OdinAudioProcessor::midiNoteOn(int p_midi_note, int p_midi_velocity) {

  m_global_env.restartEnvelope();
  if (*m_lfo4_reset) {
    m_global_lfo.voiceStart();
  }

  if (*m_phaser_reset) {
    m_phaser.resetLFO();
  }
  if (*m_flanger_reset) {
    m_flanger[0].resetLFO();
    m_flanger[1].resetLFO();
  }
  if (*m_chorus_reset) {
    m_chorus[0].resetLFO();
    m_chorus[1].resetLFO();
  }

  int voice_number = m_voice_manager.getVoice(p_midi_note);
  if (voice_number >= 0) { // else is on sustain
    if (m_last_midi_note == -1) {
      // first time glide - dont glide
      m_last_midi_note = p_midi_note;
    }
    m_voice[voice_number].start(p_midi_note, p_midi_velocity, m_last_midi_note);
    DBG("Started Voice Nr. " + std::to_string(voice_number) +
        " midikey: " + std::to_string(p_midi_note));
    m_amp.setMIDIVelocity(p_midi_velocity);
    m_last_midi_note = p_midi_note;
    m_mod_matrix.setMostRecentVoice(voice_number);
  }
}

void OdinAudioProcessor::resetAudioEngine() {
  for (int voice = 0; voice < VOICES; ++voice) {
    m_voice[voice].hardReset();
  }
  for (int stereo = 0; stereo < 2; ++stereo) {
    m_distortion[stereo].reset();

    m_ladder_filter[stereo].reset();
    m_SEM_filter_12[stereo].reset();
    m_korg_filter[stereo].reset();
    m_diode_filter[stereo].reset();
    m_formant_filter[stereo].reset();
    m_comb_filter[stereo].reset();

    m_delay[stereo].reset();
    m_flanger[stereo].reset();
    m_chorus[stereo].reset();
  }

  m_phaser.reset();
  m_global_lfo.reset();
  m_global_env.reset();

  m_voice_manager.reset();
}

void OdinAudioProcessor::setBPM(float p_BPM) {

  for (int voice = 0; voice < VOICES; ++voice) {
    m_voice[voice].setBPM(p_BPM, *m_lfo1_sync, *m_lfo2_sync, *m_lfo3_sync);
  }
  if (*m_delay_sync) {
    m_delay[0].setFreqBPM(p_BPM);
    m_delay[1].setFreqBPM(p_BPM);
  }
  if (*m_phaser_sync) {
    m_phaser.setFreqBPM(p_BPM);
  }
  if (*m_flanger_sync) {
    m_flanger[0].setFreqBPM(p_BPM);
    m_flanger[1].setFreqBPM(p_BPM);
  }
  if (*m_chorus_sync) {
    m_chorus[0].setFreqBPM(p_BPM);
    m_chorus[1].setFreqBPM(p_BPM);
  }
  if (*m_lfo4_sync) {
    m_global_lfo.setFreqBPM(p_BPM);
  }
}

void OdinAudioProcessor::addNonAudioParametersToTree() {

  auto node =
      m_value_tree.state; //.getOrCreateChildWithName("NO_PARAM", nullptr);
  for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
    float val = sin(2 * M_PI * i / (float)WAVEDRAW_STEPS_X) * 0.9;
    // do braces in the beginnning to speed up identification!?
    node.setProperty(String("[" + std::to_string(i) + "]osc1_wavedraw"), val,
                     nullptr);
    node.setProperty(String("[" + std::to_string(i) + "]osc2_wavedraw"), val,
                     nullptr);
    node.setProperty(String("[" + std::to_string(i) + "]osc3_wavedraw"), val,
                     nullptr);
  }
  for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
    float val = i < CHIPDRAW_STEPS_X / 2 ? 0.875f : -0.875f;
    // do braces in the beginnning to speed up identification!?
    node.setProperty(String("[" + std::to_string(i) + "]osc1_chipdraw"), val,
                     nullptr);
    node.setProperty(String("[" + std::to_string(i) + "]osc2_chipdraw"), val,
                     nullptr);
    node.setProperty(String("[" + std::to_string(i) + "]osc3_chipdraw"), val,
                     nullptr);
  }
  for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
    float val = i == 0 ? 1 : 0;
    // do braces in the beginnning to speed up identification!?
    node.setProperty(String("[" + std::to_string(i) + "]osc1_specdraw"), val,
                     nullptr);
    node.setProperty(String("[" + std::to_string(i) + "]osc2_specdraw"), val,
                     nullptr);
    node.setProperty(String("[" + std::to_string(i) + "]osc3_specdraw"), val,
                     nullptr);
  }

  node.setProperty("delay_synctime_numerator", 2, nullptr);
  node.setProperty("delay_synctime_denominator", 5, nullptr);
  node.setProperty("phaser_synctime_numerator", 2, nullptr);
  node.setProperty("phaser_synctime_denominator", 5, nullptr);
  node.setProperty("flanger_synctime_numerator", 2, nullptr);
  node.setProperty("flanger_synctime_denominator", 5, nullptr);
  node.setProperty("chorus_synctime_numerator", 2, nullptr);
  node.setProperty("chorus_synctime_denominator", 5, nullptr);

  node.setProperty("lfo1_synctime_numerator", 2, nullptr);
  node.setProperty("lfo1_synctime_denominator", 5, nullptr);
  node.setProperty("lfo2_synctime_numerator", 2, nullptr);
  node.setProperty("lfo2_synctime_denominator", 5, nullptr);
  node.setProperty("lfo3_synctime_numerator", 2, nullptr);
  node.setProperty("lfo3_synctime_denominator", 5, nullptr);
  node.setProperty("lfo4_synctime_numerator", 2, nullptr);
  node.setProperty("lfo4_synctime_denominator", 5, nullptr);

  node.setProperty("legato", 1, nullptr); // this is actually "poly" or
                                          // "!legato"

  node.setProperty("delay_selected", 1, nullptr);
  node.setProperty("phaser_selected", 0, nullptr);
  node.setProperty("flanger_selected", 0, nullptr);
  node.setProperty("chorus_selected", 0, nullptr);

  node.setProperty("source_[0]", 0, nullptr);
  node.setProperty("source_[1]", 0, nullptr);
  node.setProperty("source_[2]", 0, nullptr);
  node.setProperty("source_[3]", 0, nullptr);
  node.setProperty("source_[4]", 0, nullptr);
  node.setProperty("source_[5]", 0, nullptr);
  node.setProperty("source_[6]", 0, nullptr);
  node.setProperty("source_[7]", 0, nullptr);
  node.setProperty("source_[8]", 0, nullptr);
  node.setProperty("dest_1_[0]", 0, nullptr);
  node.setProperty("dest_1_[1]", 0, nullptr);
  node.setProperty("dest_1_[2]", 0, nullptr);
  node.setProperty("dest_1_[3]", 0, nullptr);
  node.setProperty("dest_1_[4]", 0, nullptr);
  node.setProperty("dest_1_[5]", 0, nullptr);
  node.setProperty("dest_1_[6]", 0, nullptr);
  node.setProperty("dest_1_[7]", 0, nullptr);
  node.setProperty("dest_1_[8]", 0, nullptr);
  node.setProperty("dest_2_[0]", 0, nullptr);
  node.setProperty("dest_2_[1]", 0, nullptr);
  node.setProperty("dest_2_[2]", 0, nullptr);
  node.setProperty("dest_2_[3]", 0, nullptr);
  node.setProperty("dest_2_[4]", 0, nullptr);
  node.setProperty("dest_2_[5]", 0, nullptr);
  node.setProperty("dest_2_[6]", 0, nullptr);
  node.setProperty("dest_2_[7]", 0, nullptr);
  node.setProperty("dest_2_[8]", 0, nullptr);
  node.setProperty("scale_[0]", 0, nullptr);
  node.setProperty("scale_[1]", 0, nullptr);
  node.setProperty("scale_[2]", 0, nullptr);
  node.setProperty("scale_[3]", 0, nullptr);
  node.setProperty("scale_[4]", 0, nullptr);
  node.setProperty("scale_[5]", 0, nullptr);
  node.setProperty("scale_[6]", 0, nullptr);
  node.setProperty("scale_[7]", 0, nullptr);
  node.setProperty("scale_[8]", 0, nullptr);

  node.setProperty("delay_position", 0, nullptr);
  node.setProperty("phaser_position", 1, nullptr);
  node.setProperty("chorus_position", 2, nullptr);
  node.setProperty("flanger_position", 3, nullptr);

  node.setProperty("dist_algo", 1.f, nullptr);
  DBG("dist algo:");
  DBG((int)node["dist_algo"]);

  node.setProperty("osc1_analog_wave", 0, nullptr);
  node.setProperty("osc2_analog_wave", 0, nullptr);
  node.setProperty("osc3_analog_wave", 0, nullptr);

  node.setProperty("osc1_type", OSC_TYPE_ANALOG, nullptr);
  node.setProperty("osc2_type", 1, nullptr);
  node.setProperty("osc3_type", 1, nullptr);
  node.setProperty("fil1_type", FILTER_TYPE_LP24, nullptr);
  node.setProperty("fil2_type", 1, nullptr);
  node.setProperty("fil3_type", 1, nullptr);

  node.setProperty("osc1_wavetable", 1, nullptr);
  node.setProperty("osc2_wavetable", 1, nullptr);
  node.setProperty("osc3_wavetable", 1, nullptr);
  node.setProperty("osc1_vec_a", 101, nullptr);
  node.setProperty("osc2_vec_a", 101, nullptr);
  node.setProperty("osc3_vec_a", 101, nullptr);
  node.setProperty("osc1_vec_b", 102, nullptr);
  node.setProperty("osc2_vec_b", 102, nullptr);
  node.setProperty("osc3_vec_b", 102, nullptr);
  node.setProperty("osc1_vec_c", 103, nullptr);
  node.setProperty("osc2_vec_c", 103, nullptr);
  node.setProperty("osc3_vec_c", 103, nullptr);
  node.setProperty("osc1_vec_d", 104, nullptr);
  node.setProperty("osc2_vec_d", 104, nullptr);
  node.setProperty("osc3_vec_d", 104, nullptr);
  node.setProperty("osc1_chipwave", 1, nullptr);
  node.setProperty("osc2_chipwave", 1, nullptr);
  node.setProperty("osc3_chipwave", 1, nullptr);
  node.setProperty("osc1_modulator_wave", 1, nullptr);
  node.setProperty("osc2_modulator_wave", 1, nullptr);
  node.setProperty("osc3_modulator_wave", 1, nullptr);
  node.setProperty("osc1_carrier_wave", 1, nullptr);
  node.setProperty("osc2_carrier_wave", 1, nullptr);
  node.setProperty("osc3_carrier_wave", 1, nullptr);
  node.setProperty("BPM", 120, nullptr);
}

void OdinAudioProcessor::setFXButtonsPosition(int p_delay, int p_phaser,
                                              int p_flanger, int p_chorus) {

  m_delay_position = p_delay;
  m_phaser_position = p_phaser;
  m_flanger_position = p_flanger;
  m_chorus_position = p_chorus;
}
