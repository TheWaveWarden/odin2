#include <typeinfo>

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "RetriggerAllListeners.h"
// these file contains implementation to avoid clutter in this file
#include "ValueChange.h"
#include "SetModulationPointers.h"

//==============================================================================
OdinAudioProcessor::OdinAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    :
    AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                       .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                       .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
                       ),
#endif
    : m_value_tree(*this, nullptr, Identifier("Odin"),
#include "AudioValueTree.h" //contains the definition of audiotree. WATCH CLOSELY: is IN m_value_tree constructor brackets
                   ),
#include "ProcessorInitializerList.h" //contains the connection of Identifiers with their strings
{
#include "AudioParameterConnections.h" // constains the connection between raw float pointers and their ValueTree counter
	addNonAudioParametersToTree();

#ifdef WTGEN
	m_voice[0].wavetable_osc[0].passVariablesToWTDisplay = [&](int p_lower, int p_higher, float p_interpol) {
		if (m_editor_pointer) {
			m_editor_pointer->m_wavetable_display.setVariables(p_lower, p_higher, p_interpol);
			m_editor_pointer->m_spectrum_display.setVariables(p_lower, p_higher, p_interpol);
		}
	};
#endif

	m_is_standalone_plugin = (wrapperType == wrapperType_Standalone);

	m_tree_listener_osc_pitch.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscPitch(p_ID, p_new_value);
	};
	m_tree_listener_osc_misc.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscMisc(p_ID, p_new_value);
	};
	m_tree_listener_osc_analog.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscAnalog(p_ID, p_new_value);
	};
	m_tree_listener_osc_multi.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscMulti(p_ID, p_new_value);
	};
	m_tree_listener_osc_xy.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscXY(p_ID, p_new_value);
	};
	m_tree_listener_osc_chip.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscChip(p_ID, p_new_value);
	};
	m_tree_listener_osc_arp.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscArp(p_ID, p_new_value);
	};
	m_tree_listener_osc_fm.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscFM(p_ID, p_new_value);
	};
	m_tree_listener_osc_noise.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedOscNoise(p_ID, p_new_value);
	};
	m_tree_listener_fil_top.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedFilTop(p_ID, p_new_value);
	};
	m_tree_listener_fil_center.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedFilCenter(p_ID, p_new_value);
	};
	m_tree_listener_fil_misc.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedFilMisc(p_ID, p_new_value);
	};
	m_tree_listener_fil_formant.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedFilFormant(p_ID, p_new_value);
	};
	m_tree_listener_amp.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedAmp(p_ID, p_new_value);
	};
	m_tree_listener_delay.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedDelay(p_ID, p_new_value);
	};
	m_tree_listener_chorus.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedChorus(p_ID, p_new_value);
	};
	m_tree_listener_phaser.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedPhaser(p_ID, p_new_value);
	};
	m_tree_listener_flanger.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedFlanger(p_ID, p_new_value);
	};
	m_tree_listener_adsr1.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedADSR1(p_ID, p_new_value);
	};
	m_tree_listener_adsr2.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedADSR2(p_ID, p_new_value);
	};
	m_tree_listener_adsr3.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedADSR3(p_ID, p_new_value);
	};
	m_tree_listener_adsr4.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedADSR4(p_ID, p_new_value);
	};
	m_tree_listener_lfo1.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedLFO1(p_ID, p_new_value);
	};
	m_tree_listener_lfo2.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedLFO2(p_ID, p_new_value);
	};
	m_tree_listener_lfo3.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedLFO3(p_ID, p_new_value);
	};
	m_tree_listener_lfo4.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedLFO4(p_ID, p_new_value);
	};
	m_tree_listener_amount1.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedAmount1(p_ID, p_new_value);
	};
	m_tree_listener_amount2.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedAmount2(p_ID, p_new_value);
	};
	m_tree_listener_amount3.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedAmount3(p_ID, p_new_value);
	};
	m_tree_listener_general_misc.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedGeneralMisc(p_ID, p_new_value);
	};
	m_non_param_listener_fx.onValueChange = [&](ValueTree &tree, const Identifier &identifier) {
		treeValueChangedNonParamFX(tree, identifier);
	};
	m_non_param_listener_lfo.onValueChange = [&](ValueTree &tree, const Identifier &identifier) {
		treeValueChangedNonParamLFO(tree, identifier);
	};
	m_non_param_listener_misc.onValueChange = [&](ValueTree &tree, const Identifier &identifier) {
		treeValueChangedNonParamMisc(tree, identifier);
	};
	m_non_param_listener_mod.onValueChange = [&](ValueTree &tree, const Identifier &identifier) {
		treeValueChangedNonParamMod(tree, identifier);
	};
	m_non_param_listener_osc.onValueChange = [&](ValueTree &tree, const Identifier &identifier) {
		treeValueChangedNonParamOsc(tree, identifier);
	};

	initializeModules();

	WavetableContainer::getInstance().loadWavetablesFromConstData();

	// create draw tables
	float draw_values[WAVEDRAW_STEPS_X];
	float spec_values[SPECDRAW_STEPS_X] = {0};
	float chip_values[CHIPDRAW_STEPS_X] = {0};
	spec_values[0]                      = 1.f;
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
		WavetableContainer::getInstance().createWavedrawTable(osc, draw_values, 44100);
		WavetableContainer::getInstance().createChipdrawTable(osc, chip_values, 44100);
		WavetableContainer::getInstance().createSpecdrawTable(osc, spec_values, 44100);
	}

	// load wavetables into oscs
	for (int i = 0; i < VOICES; ++i) {
		for (int osc = 0; osc < 3; ++osc) {
			m_voice[i].analog_osc[osc].loadWavetables();
			m_voice[i].wavetable_osc[osc].loadWavetables();
			m_voice[i].chiptune_osc[osc].loadWavetables();
			m_voice[i].vector_osc[osc].loadWavetables();
			m_voice[i].multi_osc[osc].loadWavetables();
			m_voice[i].fm_osc[osc].loadWavetables();
			m_voice[i].pm_osc[osc].loadWavetables();
			m_voice[i].wavedraw_osc[osc].loadWavedrawTables(osc);
			m_voice[i].chipdraw_osc[osc].loadChipdrawTables(osc);
			m_voice[i].specdraw_osc[osc].loadSpecdrawTables(osc);
			m_voice[i].lfo[osc].loadWavetables();
		}
		m_voice[i].ring_mod[0].loadWavetables();
		m_voice[i].ring_mod[1].loadWavetables();
		m_voice[i].ring_mod[0].selectWavetable(0);
		m_voice[i].ring_mod[1].selectWavetable(0);

		// set different initial values for filter env here....not best solution
		m_voice[i].env[1].setDecay(0.8f);
		m_voice[i].env[1].setSustain(0);
	}
	m_ring_mod[0].loadWavetables();
	m_ring_mod[1].loadWavetables();
	m_ring_mod[0].selectWavetable(0);
	m_ring_mod[1].selectWavetable(0);

	m_global_lfo.loadWavetables();

	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].env[0].setEnvelopeEndPointers(&(m_voice[voice].m_voice_active),
		                                             &(m_voice_manager.voice_busy[voice]));
	}

	// set mod sources to render from modmatrix
	m_mod_matrix.setSourcesToRender =
	    [&](bool p_LFO_0, bool p_LFO_1, bool p_LFO_2, bool p_LFO_3, bool p_ADSR_0, bool p_ADSR_1) {
		    m_render_LFO[0] = p_LFO_0;
		    m_render_LFO[1] = p_LFO_1;
		    m_render_LFO[2] = p_LFO_2;
		    m_render_LFO[3] = p_LFO_3;

		    m_render_ADSR[0] = p_ADSR_0;
		    m_render_ADSR[1] = p_ADSR_1;
	    };
	retriggerAllListeners();
}

OdinAudioProcessor::~OdinAudioProcessor() {
	// WavetableContainer::getInstance().destroyWavetables();
}

//==============================================================================
const String OdinAudioProcessor::getName() const {
	return JucePlugin_Name;
}

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

double OdinAudioProcessor::getTailLengthSeconds() const {
	return 0.0;
}

int OdinAudioProcessor::getNumPrograms() {
	return 1; // NB: some hosts don't cope very well if you tell them there are 0
	          // programs, so this should be at least 1, even if you're not really
	          // implementing programs.
}

int OdinAudioProcessor::getCurrentProgram() {
	return 0;
}

void OdinAudioProcessor::setCurrentProgram(int index) {
}

const String OdinAudioProcessor::getProgramName(int index) {
	return {};
}

void OdinAudioProcessor::changeProgramName(int index, const String &newName) {
}

//==============================================================================
void OdinAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	setSampleRate(sampleRate);
}

void OdinAudioProcessor::releaseResources() {
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OdinAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
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

void OdinAudioProcessor::processBlock(AudioBuffer<float> &buffer, MidiBuffer &midiMessages) {

	// get BPM info from host
	if (!m_is_standalone_plugin) {
		if (AudioPlayHead *playhead = getPlayHead()) {
			AudioPlayHead::CurrentPositionInfo current_position_info;
			playhead->getCurrentPosition(current_position_info);
			m_BPM = current_position_info.bpm;
			// ! bottleneck
			m_value_tree.state.getChildWithName("misc").setProperty("BPM", m_BPM, nullptr);
		}
	}
	setBPM(m_BPM);

	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels  = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	MidiMessage midi_message;
	int midi_message_sample;
	MidiBuffer::Iterator midi_iterator(midiMessages);
	bool midi_message_remaining = midi_iterator.getNextEvent(midi_message, midi_message_sample);

	// loop over samples
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

		//===== SMOOTH CONTROLS ======
		for (int i = 0; i < 3; ++i) {
			m_osc_vol_smooth[i] =
			    m_osc_vol_smooth[i] * GAIN_SMOOTHIN_FACTOR + (1.f - GAIN_SMOOTHIN_FACTOR) * m_osc_vol_control[i];
			m_fil_gain_smooth[i] =
			    m_fil_gain_smooth[i] * GAIN_SMOOTHIN_FACTOR + (1.f - GAIN_SMOOTHIN_FACTOR) * m_fil_gain_control[i];

			m_fil_freq_smooth[i] = m_fil_freq_smooth[i] * FILTER_FREQ_SMOOTHING_FACTOR +
			                       (1.f - FILTER_FREQ_SMOOTHING_FACTOR) * m_fil_freq_control[i];
		}

		m_pitch_bend_smooth =
		    m_pitch_bend_smooth * PITCHBEND_SMOOTHIN_FACTOR + (1.f - PITCHBEND_SMOOTHIN_FACTOR) * (*m_pitchbend);
		m_pitch_bend_smooth_and_applied = m_pitch_bend_smooth * (*m_pitchbend_amount);

		m_modwheel_smooth =
		    m_modwheel_smooth * PITCHBEND_SMOOTHIN_FACTOR + (1.f - PITCHBEND_SMOOTHIN_FACTOR) * (*m_modwheel);

		m_x_smooth = m_x_smooth * PAD_SMOOTHIN_FACTOR + (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_x);
		m_y_smooth = m_y_smooth * PAD_SMOOTHIN_FACTOR + (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_y);

		m_master_smooth = m_master_smooth * GAIN_SMOOTHIN_FACTOR + (1 - GAIN_SMOOTHIN_FACTOR) * (m_master_control);

		//===== MIDI =====
		while (midi_message_remaining && midi_message_sample <= sample) {
			// apply midi message
			if (midi_message.isNoteOn()) {
				midiNoteOn(midi_message.getNoteNumber(), midi_message.getVelocity());
			} else if (midi_message.isNoteOff()) {

				midiNoteOff(midi_message.getNoteNumber());
			} else if (midi_message.isPitchWheel()) {
				setPitchWheelValue(midi_message.getPitchWheelValue());
			} else if (midi_message.isController() && midi_message.getControllerNumber() == 1) { // modwheel
				setModWheelValue(midi_message.getControllerValue());
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
				// todo this is untested
				m_MIDI_aftertouch = (float)midi_message.getAfterTouchValue() / 127.f;
			} else if (midi_message.isSoftPedalOn()) {
				m_soft_pedal = 1.f;
			} else if (midi_message.isSoftPedalOff()) {
				m_soft_pedal = 0.f;
			}

			else {
				if (!midi_message.isMidiClock()) {
					DBG("UNHANDELED MIDI MESSAGE: " + midi_message.getDescription());
					if (midi_message.isController()) {
						DBG("\nController with number: " + std::to_string(midi_message.getControllerNumber()));
					}
				}
			}

			if ((midi_message.isController() /*|| midi_message.isPitchWheel()*/)) {
				// DBG("CONTROLLER");
				if (m_midi_learn_knob_active) {
					m_midi_control_list_knob.emplace(midi_message.getControllerNumber(), m_midi_learn_knob);
					m_midi_learn_knob->setMidiControlActive();
					m_midi_learn_knob_active = false;
					m_midi_learn_knob        = nullptr;
					DBG("Added MIDI control on controller number " +
					    std::to_string(midi_message.getControllerNumber()));
				}
				if (m_midi_learn_slider_active) {
					m_midi_control_list_slider.emplace(midi_message.getControllerNumber(), m_midi_learn_slider);
					m_midi_learn_slider->setMidiControlActive();
					m_midi_learn_slider_active = false;
					m_midi_learn_slider        = nullptr;
					DBG("Added MIDI control on controller number " +
					    std::to_string(midi_message.getControllerNumber()));
				}
				if (m_midi_learn_lrbutton_active) {
					m_midi_control_list_lrbutton.emplace(midi_message.getControllerNumber(), m_midi_learn_lrbutton);
					m_midi_learn_lrbutton->setMidiControlActive();
					m_midi_learn_lrbutton_active = false;
					m_midi_learn_lrbutton        = nullptr;
					DBG("Added MIDI control on controller number " +
					    std::to_string(midi_message.getControllerNumber()));
				}
				if (m_midi_learn_odinbutton_active) {
					m_midi_control_list_odinbutton.emplace(midi_message.getControllerNumber(), m_midi_learn_odinbutton);
					m_midi_learn_odinbutton->setMidiControlActive();
					m_midi_learn_odinbutton_active = false;
					m_midi_learn_odinbutton        = nullptr;
					DBG("Added MIDI control on controller number " +
					    std::to_string(midi_message.getControllerNumber()));
				}

				// do midi control
				for (auto const &control : m_midi_control_list_knob) {
					if (control.first == midi_message.getControllerNumber()) {
						const MessageManagerLock mmLock;
						control.second->setValue(
						    control.second->proportionOfLengthToValue((int)midi_message.getControllerValue() / 127.f));
					}
				}
				for (auto const &control : m_midi_control_list_slider) {
					if (control.first == midi_message.getControllerNumber()) {
						const MessageManagerLock mmLock;
						control.second->setValue(
						    control.second->proportionOfLengthToValue((int)midi_message.getControllerValue() / 127.f));
					}
				}
				for (auto const &control : m_midi_control_list_lrbutton) {
					if (control.first == midi_message.getControllerNumber()) {
						const MessageManagerLock mmLock;
						control.second->setToggleState((int)midi_message.getControllerValue() > 64,
						                               sendNotificationAsync);
					}
				}
				for (auto const &control : m_midi_control_list_odinbutton) {
					if (control.first == midi_message.getControllerNumber()) {
						const MessageManagerLock mmLock;
						control.second->setToggleState((int)midi_message.getControllerValue() > 64,
						                               sendNotificationAsync);
					}
				}
			}

			// get next midi message
			midi_message_remaining = midi_iterator.getNextEvent(midi_message, midi_message_sample);
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
		if (m_render_ADSR[1]) {
			m_global_env_mod_source = m_global_env.doEnvelope();
		}

		if (m_render_LFO[3]) {
			m_global_lfo.update();
			m_global_lfo_mod_source = m_global_lfo.doOscillate();
		}

		// loop over all voices
		for (int voice = 0; voice < VOICES; ++voice) {
			if (m_voice[voice]) {

				for (int mod = 0; mod < 3; ++mod) {
					//===== ADSR ======
					if (mod < 2 || m_render_ADSR[mod - 2]) {
						m_adsr[voice][mod] = m_voice[voice].env[mod].doEnvelope();
					}

					//====== LFO ======
					if (m_render_LFO[mod]) {
						// DBG("RENDER LFO " + std::to_string(mod));
						m_voice[voice].lfo[mod].update();
						m_lfo[voice][mod] = m_voice[voice].lfo[mod].doOscillate();
					}
				}

				//===== OSCS ======

				// output var for the individual oscs
				memset(m_osc_output, 0, sizeof(float) * VOICES * 3);

				for (int osc = 0; osc < 3; ++osc) {

					switch (m_osc_type[osc]) {
					case OSC_TYPE_ANALOG:
						m_voice[voice].analog_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].analog_osc[osc].doOscillateWithSync();
						break;
					case OSC_TYPE_WAVETABLE:
						m_voice[voice].wavetable_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].wavetable_osc[osc].doOscillateWithSync();
						break;
					case OSC_TYPE_MULTI:
						m_voice[voice].multi_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].multi_osc[osc].doOscillate();
						break;
					case OSC_TYPE_VECTOR:
						m_voice[voice].vector_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].vector_osc[osc].doOscillateWithSync();
						break;
					case OSC_TYPE_CHIPTUNE:
						m_voice[voice].chiptune_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].chiptune_osc[osc].doOscillateWithSync();
						break;
					case OSC_TYPE_FM:
						m_voice[voice].fm_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].fm_osc[osc].doOscillate();
						break;
					case OSC_TYPE_PM:
						m_voice[voice].pm_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].pm_osc[osc].doOscillate();
						break;
					case OSC_TYPE_NOISE:
						m_osc_output[voice][osc] += m_voice[voice].noise_osc[osc].doNoise();
						break;
					case OSC_TYPE_WAVEDRAW:
						m_voice[voice].wavedraw_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].wavedraw_osc[osc].doOscillateWithSync();
						break;
					case OSC_TYPE_CHIPDRAW:
						m_voice[voice].chipdraw_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].chipdraw_osc[osc].doOscillateWithSync();
						break;
					case OSC_TYPE_SPECDRAW:
						m_voice[voice].specdraw_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].specdraw_osc[osc].doOscillateWithSync();
						break;
					default:
						break;
					}
					// apply volume
					m_osc_output[voice][osc] *= m_osc_vol_smooth[osc];
				} // osc loop

				//===== FILTERS ======

				float filter_input[2] = {0};
				memset(m_filter_output, 0, sizeof(float) * VOICES * 2);
				m_voice[voice].setFilterEnvValue(m_adsr[voice][1]); // can be split up to individual filters
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
					switch (m_fil_type[fil]) {
					case FILTER_TYPE_NONE:
					default:
						m_filter_output[voice][fil] = filter_input[fil];
						break;
					case FILTER_TYPE_LP24:
					case FILTER_TYPE_LP12:
					case FILTER_TYPE_BP24:
					case FILTER_TYPE_BP12:
					case FILTER_TYPE_HP24:
					case FILTER_TYPE_HP12:
						m_voice[voice].ladder_filter[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].ladder_filter[fil].update();
						m_filter_output[voice][fil] =
						    m_voice[voice].ladder_filter[fil].doFilter(filter_input[fil]) * m_fil_gain_smooth[fil];
						break;
					case FILTER_TYPE_SEM12:
						m_voice[voice].SEM_filter_12[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].SEM_filter_12[fil].update();
						m_filter_output[voice][fil] =
						    m_voice[voice].SEM_filter_12[fil].doFilter(filter_input[fil]) * m_fil_gain_smooth[fil];
						break;
					case FILTER_TYPE_KORG_LP:
					case FILTER_TYPE_KORG_HP:
						m_voice[voice].korg_filter[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].korg_filter[fil].update();
						m_filter_output[voice][fil] =
						    m_voice[voice].korg_filter[fil].doFilter(filter_input[fil]) * m_fil_gain_smooth[fil];
						break;
					case FILTER_TYPE_DIODE:
						m_voice[voice].diode_filter[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].diode_filter[fil].update();
						m_filter_output[voice][fil] =
						    m_voice[voice].diode_filter[fil].doFilter(filter_input[fil]) * m_fil_gain_smooth[fil];
						break;
					case FILTER_TYPE_FORMANT:
						m_voice[voice].formant_filter[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].formant_filter[fil].update();
						m_filter_output[voice][fil] =
						    m_voice[voice].formant_filter[fil].doFilter(filter_input[fil]) * m_fil_gain_smooth[fil];
						break;
					case FILTER_TYPE_COMB:
						m_voice[voice].comb_filter[fil].setCombFreq(m_fil_freq_smooth[fil]);
						m_filter_output[voice][fil] =
						    m_voice[voice].comb_filter[fil].doFilter(filter_input[fil]) * m_fil_gain_smooth[fil];
						break;
					case FILTER_TYPE_RINGMOD:
						m_voice[voice].ring_mod[fil].setBaseFrequency(m_fil_freq_smooth[fil]);
						m_voice[voice].ring_mod[fil].setGlideTargetFrequency(m_fil_freq_smooth[fil]);
						m_voice[voice].ring_mod[fil].update();
						m_filter_output[voice][fil] =
						    m_voice[voice].ring_mod[fil].doRingModulator(filter_input[fil]) * m_fil_gain_smooth[fil];
						break;
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
		}     // voice loop

		//===== AMPLIFIER ======

		float stereo_signal[2];

		m_amp.doAmplifier(voices_output, stereo_signal[0], stereo_signal[1]);
		DBGAUDIO("ampout: " + std::to_string(stereo_signal[0]) + ", " + std::to_string(stereo_signal[1]))

		for (int channel = 0; channel < 2; ++channel) {

			//===== DISTORTION ======
			if (*m_dist_on) {
				stereo_signal[channel] = m_distortion[channel].doDistortion(stereo_signal[channel]);
				DBGAUDIO("distout[" + std::to_string(channel) + "]: " + std::to_string(stereo_signal[channel]))
			}

			//===== FILTER 3 ======

			setFilter3EnvValue();

			switch (m_fil_type[2]) {
			case FILTER_TYPE_LP24:
			case FILTER_TYPE_LP12:
			case FILTER_TYPE_BP24:
			case FILTER_TYPE_BP12:
			case FILTER_TYPE_HP24:
			case FILTER_TYPE_HP12:
				m_ladder_filter[channel].m_freq_base = m_fil_freq_smooth[2];
				m_ladder_filter[channel].update();
				stereo_signal[channel] =
				    m_ladder_filter[channel].doFilter(stereo_signal[channel]) * m_fil_gain_smooth[2];
				break;
			case FILTER_TYPE_SEM12:
				m_SEM_filter_12[channel].m_freq_base = m_fil_freq_smooth[2];
				m_SEM_filter_12[channel].update();
				stereo_signal[channel] =
				    m_SEM_filter_12[channel].doFilter(stereo_signal[channel]) * m_fil_gain_smooth[2];
				break;
			case FILTER_TYPE_KORG_LP:
			case FILTER_TYPE_KORG_HP:
				m_korg_filter[channel].m_freq_base = m_fil_freq_smooth[2];
				m_korg_filter[channel].update();
				stereo_signal[channel] = m_korg_filter[channel].doFilter(stereo_signal[channel]) * m_fil_gain_smooth[2];
				break;
			case FILTER_TYPE_DIODE:
				m_diode_filter[channel].m_freq_base = m_fil_freq_smooth[2];
				m_diode_filter[channel].update();
				stereo_signal[channel] =
				    m_diode_filter[channel].doFilter(stereo_signal[channel]) * m_fil_gain_smooth[2];
				break;
			case FILTER_TYPE_FORMANT:
				m_formant_filter[channel].m_freq_base = m_fil_freq_smooth[2];
				m_formant_filter[channel].update();
				stereo_signal[channel] =
				    m_formant_filter[channel].doFilter(stereo_signal[channel]) * m_fil_gain_smooth[2];
				break;
			case FILTER_TYPE_COMB:
				m_comb_filter[channel].setCombFreq(m_fil_freq_smooth[2]);
				stereo_signal[channel] = m_comb_filter[channel].doFilter(stereo_signal[channel]) * m_fil_gain_smooth[2];
				break;
			case FILTER_TYPE_RINGMOD:
				m_ring_mod[channel].setBaseFrequency(m_fil_freq_smooth[2]);
				m_ring_mod[channel].setGlideTargetFrequency(m_fil_freq_smooth[2]);

				m_ring_mod[channel].update();
				stereo_signal[channel] =
				    m_ring_mod[channel].doRingModulator(stereo_signal[channel]) * m_fil_gain_smooth[2];
				break;
			default:
				break;
			}
			DBGAUDIO("fil3out[" + std::to_string(channel) + "]: " + std::to_string(stereo_signal[channel]))

			//==== FX SECTION ====

			// ugly solution, yet here we go
			for (int fx_slot = 0; fx_slot < 4; ++fx_slot) {
				if (m_delay_position == fx_slot) {
					if (*m_delay_on) {
						if (channel == 0) {
							stereo_signal[channel] = m_delay.doDelayLeft(stereo_signal[channel]);
						} else {
							stereo_signal[channel] = m_delay.doDelayRight(stereo_signal[channel]);
						}
					}
				} else if (m_phaser_position == fx_slot) {
					if (*m_phaser_on) {
						if (channel == 0) {
							stereo_signal[channel] = m_phaser.doPhaserLeft(stereo_signal[channel]);
						} else {
							stereo_signal[channel] = m_phaser.doPhaserRight(stereo_signal[channel]);
						}
					}
				} else if (m_flanger_position == fx_slot) {
					if (*m_flanger_on) {
						stereo_signal[channel] = m_flanger[channel].doFlanger(stereo_signal[channel]);
					}
				} else if (m_chorus_position == fx_slot) {
					if (*m_chorus_on) {
						stereo_signal[channel] = m_chorus[channel].doChorus(stereo_signal[channel]);
					}
				}
			}
			DBGAUDIO("fxout[" + std::to_string(channel) + "]: " + std::to_string(stereo_signal[channel]))

			//===== OUTPUT ======

			float master_mod_factor = (*m_master_mod) > 0 ? 1.f + 4 * (*m_master_mod) : (1.f + *m_master_mod);

			auto *channelData   = buffer.getWritePointer(channel);
			channelData[sample] = stereo_signal[channel] * m_master_smooth * master_mod_factor;

			DBGAUDIO("masterout[" + std::to_string(channel) + "]: " + std::to_string(stereo_signal[channel]))

			// DBG(m_master_smooth);

		} // stereo loop
	}     // sample loop
}

//==============================================================================
bool OdinAudioProcessor::hasEditor() const {
	return true;
}

AudioProcessorEditor *OdinAudioProcessor::createEditor() {

	OdinAudioProcessorEditor *editor = new OdinAudioProcessorEditor(*this, m_value_tree, m_is_standalone_plugin);

	if (m_force_values_onto_gui) {
		onSetStateInformation();
	}

	m_editor_pointer = editor;

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

void OdinAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
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
	}

	retriggerAllListeners();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
	return new OdinAudioProcessor();
}

void OdinAudioProcessor::setSampleRate(float p_samplerate) {
	DBG("SampleRate was set to " + std::to_string((int)p_samplerate) + " Hz!");

	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].setSampleRate(p_samplerate);
	}
	for (int stereo = 0; stereo < 2; ++stereo) {
		m_flanger[stereo].setSampleRate(p_samplerate);
		m_chorus[stereo].setSampleRate(p_samplerate);

		m_ladder_filter[stereo].setSampleRate(p_samplerate);
		m_korg_filter[stereo].setSampleRate(p_samplerate);
		m_diode_filter[stereo].setSampleRate(p_samplerate);
		m_comb_filter[stereo].setSampleRate(p_samplerate);
		m_formant_filter[stereo].setSampleRate(p_samplerate);
		m_SEM_filter_12[stereo].setSampleRate(p_samplerate);
	}

	m_delay.setSampleRate(p_samplerate);
	m_phaser.setSampleRate(p_samplerate);
	m_global_env.setSampleRate(p_samplerate);
	m_global_lfo.setSampleRate(p_samplerate);
}

void OdinAudioProcessor::initializeModules() {
	m_global_env.reset();
	m_global_env.setEnvelopeOff(); // so it doesn't start by itself

	m_mod_matrix.setSourcesAndDestinations(&m_mod_sources, &m_mod_destinations);

	m_phaser.setRadiusBase(1.25f);

	// make "stereo" chorus & flanger
	m_chorus[1].setLFOResetPos(0.25);
	m_flanger[1].setLFOResetPos(0.5);

	setModulationPointers();

	// set analog osc as sync osc for start
	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].setOscSyncOscillator(&(m_voice[voice].analog_osc[0]));
	}
}


void OdinAudioProcessor::setPitchWheelValue(int p_value) {
	*m_pitchbend = (float)(p_value - 8192) / 8192.f;
	updatePitchWheelGUI(*m_pitchbend);
}

void OdinAudioProcessor::setModWheelValue(int p_value) {
	//todo is set on valuetree?
	*m_modwheel = (float)(p_value) / 128.f;
	updateModWheelGUI(*m_modwheel);
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
	//todo "else is on sustain".... u sure...? (recent changes to sustain)
	if (voice_number >= 0) { // else is on sustain
		if (m_last_midi_note == -1) {
			// first time glide - dont glide
			m_last_midi_note = p_midi_note;
		}
		m_voice[voice_number].start(p_midi_note, p_midi_velocity, m_last_midi_note);
		DBG("Started Voice Nr. " + std::to_string(voice_number) + " midikey: " + std::to_string(p_midi_note));
		m_amp.setMIDIVelocity(p_midi_velocity);
		m_last_midi_note = p_midi_note;
		m_mod_matrix.setMostRecentVoice(voice_number);
	}

	// set values to filters
	for (int stereo = 0; stereo < 2; ++stereo) {
		m_ladder_filter[stereo].m_MIDI_note = p_midi_note;
		m_diode_filter[stereo].m_MIDI_note  = p_midi_note;
		m_korg_filter[stereo].m_MIDI_note   = p_midi_note;
		m_SEM_filter_12[stereo].m_MIDI_note = p_midi_note;
		m_comb_filter[stereo].m_MIDI_note   = p_midi_note;
		m_ring_mod[stereo].m_MIDI_note      = p_midi_note;

		m_ladder_filter[stereo].m_MIDI_velocity = p_midi_velocity;
		m_diode_filter[stereo].m_MIDI_velocity  = p_midi_velocity;
		m_korg_filter[stereo].m_MIDI_velocity   = p_midi_velocity;
		m_SEM_filter_12[stereo].m_MIDI_velocity = p_midi_velocity;
		m_comb_filter[stereo].m_MIDI_velocity   = p_midi_velocity;
		m_ring_mod[stereo].m_MIDI_velocity      = p_midi_velocity;
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

		m_flanger[stereo].reset();
		m_chorus[stereo].reset();
	}

	m_delay.reset();
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
		m_delay.setFreqBPM(p_BPM);
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

	auto node = m_value_tree_draw;

	for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
		float val = sin(2 * M_PI * i / (float)WAVEDRAW_STEPS_X) * 0.9;
		node.setProperty(String("osc1_wavedraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc2_wavedraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc3_wavedraw_values_" + std::to_string(i)), val, nullptr);
	}
	for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
		float val = i < CHIPDRAW_STEPS_X / 2 ? 0.875f : -0.875f;
		node.setProperty(String("osc1_chipdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc2_chipdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc3_chipdraw_values_" + std::to_string(i)), val, nullptr);
	}
	for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
		float val = i == 0 ? 1 : 0;
		node.setProperty(String("osc1_specdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc2_specdraw_values_" + std::to_string(i)), val, nullptr);
		node.setProperty(String("osc3_specdraw_values_" + std::to_string(i)), val, nullptr);
		//		node.setProperty(String("osc3_specdraw_values_" + std::to_string(i)), 0.5f-i*0.005+cos(i*0.2f)*0.5f*(1.f-i*0.02), nullptr);
	}

	node = m_value_tree_fx;
	node.setProperty("delay_synctime_numerator", 2, nullptr);
	node.setProperty("delay_synctime_denominator", 5, nullptr);
	node.setProperty("phaser_synctime_numerator", 2, nullptr);
	node.setProperty("phaser_synctime_denominator", 5, nullptr);
	node.setProperty("flanger_synctime_numerator", 2, nullptr);
	node.setProperty("flanger_synctime_denominator", 5, nullptr);
	node.setProperty("chorus_synctime_numerator", 2, nullptr);
	node.setProperty("chorus_synctime_denominator", 5, nullptr);
	node.setProperty("delay_selected", 1, nullptr);
	node.setProperty("phaser_selected", 0, nullptr);
	node.setProperty("flanger_selected", 0, nullptr);
	node.setProperty("chorus_selected", 0, nullptr);
	node.setProperty("delay_position", 0, nullptr);
	node.setProperty("phaser_position", 1, nullptr);
	node.setProperty("chorus_position", 2, nullptr);
	node.setProperty("flanger_position", 3, nullptr);

	node = m_value_tree_lfo;
	node.setProperty("lfo1_synctime_numerator", 2, nullptr);
	node.setProperty("lfo1_synctime_denominator", 5, nullptr);
	node.setProperty("lfo2_synctime_numerator", 2, nullptr);
	node.setProperty("lfo2_synctime_denominator", 5, nullptr);
	node.setProperty("lfo3_synctime_numerator", 2, nullptr);
	node.setProperty("lfo3_synctime_denominator", 5, nullptr);
	node.setProperty("lfo4_synctime_numerator", 2, nullptr);
	node.setProperty("lfo4_synctime_denominator", 5, nullptr);
	node.setProperty("lfo_left_selected", 1, nullptr);
	node.setProperty("lfo_right_selected", 1, nullptr);

	node = m_value_tree_misc;
	node.setProperty("legato", 1, nullptr); // this is actually "poly" or !legato"
	node.setProperty("dist_algo", 1.f, nullptr);
	node.setProperty("BPM", 120, nullptr);
	node.setProperty("env_left_selected", 1, nullptr);
	node.setProperty("env_right_selected", 1, nullptr);
	node.setProperty("fil1_type", FILTER_TYPE_LP24, nullptr);
	node.setProperty("fil2_type", 1, nullptr);
	node.setProperty("fil3_type", 1, nullptr);

	node = m_value_tree_mod;
	node.setProperty("source_row_0", 0, nullptr);
	node.setProperty("source_row_1", 0, nullptr);
	node.setProperty("source_row_2", 0, nullptr);
	node.setProperty("source_row_3", 0, nullptr);
	node.setProperty("source_row_4", 0, nullptr);
	node.setProperty("source_row_5", 0, nullptr);
	node.setProperty("source_row_6", 0, nullptr);
	node.setProperty("source_row_7", 0, nullptr);
	node.setProperty("source_row_8", 0, nullptr);
	node.setProperty("dest_1_row_0", 0, nullptr);
	node.setProperty("dest_1_row_1", 0, nullptr);
	node.setProperty("dest_1_row_2", 0, nullptr);
	node.setProperty("dest_1_row_3", 0, nullptr);
	node.setProperty("dest_1_row_4", 0, nullptr);
	node.setProperty("dest_1_row_5", 0, nullptr);
	node.setProperty("dest_1_row_6", 0, nullptr);
	node.setProperty("dest_1_row_7", 0, nullptr);
	node.setProperty("dest_1_row_8", 0, nullptr);
	node.setProperty("dest_2_row_0", 0, nullptr);
	node.setProperty("dest_2_row_1", 0, nullptr);
	node.setProperty("dest_2_row_2", 0, nullptr);
	node.setProperty("dest_2_row_3", 0, nullptr);
	node.setProperty("dest_2_row_4", 0, nullptr);
	node.setProperty("dest_2_row_5", 0, nullptr);
	node.setProperty("dest_2_row_6", 0, nullptr);
	node.setProperty("dest_2_row_7", 0, nullptr);
	node.setProperty("dest_2_row_8", 0, nullptr);
	node.setProperty("scale_row_0", 0, nullptr);
	node.setProperty("scale_row_1", 0, nullptr);
	node.setProperty("scale_row_2", 0, nullptr);
	node.setProperty("scale_row_3", 0, nullptr);
	node.setProperty("scale_row_4", 0, nullptr);
	node.setProperty("scale_row_5", 0, nullptr);
	node.setProperty("scale_row_6", 0, nullptr);
	node.setProperty("scale_row_7", 0, nullptr);
	node.setProperty("scale_row_8", 0, nullptr);

	node = m_value_tree_osc;
	node.setProperty("osc1_analog_wave", 0, nullptr);
	node.setProperty("osc2_analog_wave", 0, nullptr);
	node.setProperty("osc3_analog_wave", 0, nullptr);
	node.setProperty("osc1_type", OSC_TYPE_ANALOG, nullptr);
	node.setProperty("osc2_type", 1, nullptr);
	node.setProperty("osc3_type", 1, nullptr);
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
}

void OdinAudioProcessor::setFXButtonsPosition(int p_delay, int p_phaser, int p_flanger, int p_chorus) {

	m_delay_position   = p_delay;
	m_phaser_position  = p_phaser;
	m_flanger_position = p_flanger;
	m_chorus_position  = p_chorus;
}

void OdinAudioProcessor::setFilter3EnvValue() {

	// get adsr value newest voice filterenv
	float env_value = m_adsr[m_voice_manager.getNewestVoiceIndex()][1];

	m_ladder_filter[0].m_env_value  = env_value;
	m_SEM_filter_12[0].m_env_value  = env_value;
	m_korg_filter[0].m_env_value    = env_value;
	m_diode_filter[0].m_env_value   = env_value;
	m_comb_filter[0].m_env_value    = env_value;
	m_formant_filter[0].m_env_value = env_value;
	m_ring_mod[0].m_env_value       = env_value;
	m_ladder_filter[1].m_env_value  = env_value;
	m_SEM_filter_12[1].m_env_value  = env_value;
	m_korg_filter[1].m_env_value    = env_value;
	m_diode_filter[1].m_env_value   = env_value;
	m_comb_filter[1].m_env_value    = env_value;
	m_formant_filter[1].m_env_value = env_value;
	m_ring_mod[1].m_env_value       = env_value;
}

void OdinAudioProcessor::attachNonParamListeners() {
	m_value_tree_fx   = m_value_tree.state.getChildWithName("fx");
	m_value_tree_osc  = m_value_tree.state.getChildWithName("osc");
	m_value_tree_mod  = m_value_tree.state.getChildWithName("mod");
	m_value_tree_misc = m_value_tree.state.getChildWithName("misc");
	m_value_tree_lfo  = m_value_tree.state.getChildWithName("lfo");
	m_value_tree_draw = m_value_tree.state.getChildWithName("draw");
}