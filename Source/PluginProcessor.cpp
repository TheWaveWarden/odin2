#include <typeinfo>

#include "PluginEditor.h"
#include "PluginProcessor.h"
// these file contains implementation to avoid clutter in this file
#include "AddNonAudioParametersToValueTree.h"
#include "MigratePatch.h"
#include "ReadPatch.h"
#include "ScopedNoDenormals.h"
#include "SetModulationPointers.h"
#include "ValueChange.h"

OdinAudioProcessor::OdinAudioProcessor() :
    AudioProcessor(BusesProperties().withOutput("Output", AudioChannelSet::stereo(), true)),
    m_value_tree(*this, nullptr, Identifier("Odin"),
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
	m_tree_listener_arp.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedArp(p_ID, p_new_value);
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

	//distribute WTContainer to audio core
	for (int i = 0; i < VOICES; ++i) {
		for (int osc = 0; osc < 3; ++osc) {
			m_voice[i].analog_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].wavetable_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].chiptune_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].vector_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].multi_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].fm_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].pm_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].wavedraw_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].chipdraw_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].specdraw_osc[osc].setWavetableContainer(&m_WT_container);
			m_voice[i].lfo[osc].setWavetableContainer(&m_WT_container);
		}
		m_voice[i].ring_mod[0].setWavetableContainer(&m_WT_container);
		m_voice[i].ring_mod[1].setWavetableContainer(&m_WT_container);
	}
	m_ring_mod[0].setWavetableContainer(&m_WT_container);
	m_ring_mod[1].setWavetableContainer(&m_WT_container);

	m_global_lfo.setWavetableContainer(&m_WT_container);

	initializeModules();
	m_WT_container.loadWavetablesFromConstData();

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
		m_WT_container.createWavedrawTable(osc, draw_values, 44100);
		m_WT_container.createChipdrawTable(osc, chip_values, 44100);
		m_WT_container.createSpecdrawTable(osc, spec_values, 44100);
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

		// set different initial values for filter and mod env here....not best solution
		m_voice[i].env[1].setDecay(0.8f);
		m_voice[i].env[1].setSustain(0);
		m_voice[i].env[2].setDecay(0.8f);
		m_voice[i].env[2].setSustain(0);
	}
	m_ring_mod[0].loadWavetables();
	m_ring_mod[1].loadWavetables();
	m_ring_mod[0].selectWavetable(0);
	m_ring_mod[1].selectWavetable(0);

	m_global_lfo.loadWavetables();

	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].env[0].setEnvelopeEndPointers(&(m_voice[voice].m_voice_active),
		                                             &(m_voice_manager.voice_busy[voice]));
		m_voice_manager.m_actual_voice_pointers[voice] = &(m_voice[voice]);
	}

	// set mod sources to render from modmatrix
	m_mod_matrix.setSourcesToRender =
	    [&](bool p_LFO_0, bool p_LFO_1, bool p_LFO_2, bool p_LFO_3, bool p_ADSR_0, bool p_ADSR_1) {
		    //wavetable oscs might need LFO1
		    m_render_LFO[0] = p_LFO_0 || (m_osc_type[0] == OSC_TYPE_WAVETABLE && m_osc_wavetable_source_lfo[0]) ||
		                      (m_osc_type[1] == OSC_TYPE_WAVETABLE && m_osc_wavetable_source_lfo[1]) ||
		                      (m_osc_type[2] == OSC_TYPE_WAVETABLE && m_osc_wavetable_source_lfo[2]);
		    m_render_LFO[1] = p_LFO_1;
		    m_render_LFO[2] = p_LFO_2;
		    m_render_LFO[3] = p_LFO_3;

		    //wavetable oscs might need ModEnv
		    m_render_ADSR[0] = p_ADSR_0 || (m_osc_type[0] == OSC_TYPE_WAVETABLE && !m_osc_wavetable_source_lfo[0]) ||
		                       (m_osc_type[1] == OSC_TYPE_WAVETABLE && !m_osc_wavetable_source_lfo[1]) ||
		                       (m_osc_type[2] == OSC_TYPE_WAVETABLE && !m_osc_wavetable_source_lfo[2]);
		    m_render_ADSR[1] = p_ADSR_1;

		    //DBG("RENDERING MODSOURCES:");
		    //DBG("LFO: " + std::to_string((int)m_render_LFO[0]) + " " + std::to_string((int)m_render_LFO[1]) + " " +
		    //    std::to_string((int)m_render_LFO[2]) + " " + std::to_string((int)m_render_LFO[3]) +
		    //    " ADSR: " + std::to_string((int)m_render_ADSR[0]) + " " + std::to_string((int)m_render_ADSR[1]));
	    };

	m_master_control = Decibels::decibelsToGain(-7.f);
	m_master_smooth  = m_master_control;
}

OdinAudioProcessor::~OdinAudioProcessor() {
	// m_WT_container->destroyWavetables();
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

	//avoid denormals
	denormals::ScopedNoDenormals snd;

	// get BPM info from host
	if (AudioPlayHead *playhead = getPlayHead()) {
		AudioPlayHead::CurrentPositionInfo current_position_info;
		playhead->getCurrentPosition(current_position_info);
		if (m_BPM != current_position_info.bpm) {
			m_value_tree.state.getChildWithName("misc").setProperty("BPM", m_BPM, nullptr);
		}
		m_BPM = current_position_info.bpm;
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

		// do Arpeggiator
		if(m_arpeggiator_on){
			int step_active;
			auto note = m_arpeggiator.getNoteOns(step_active);
			m_step_led_active.set(step_active);
			if(note.first != -1){
				midiNoteOn(note.first, note.second);
			}
			auto off_notes = m_arpeggiator.getNoteOffs();
			for(auto note : off_notes){
				midiNoteOff(note);
			}
		}

		//============================================================
		//======================= SMOOTHING ==========================
		//============================================================
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

		m_pitch_bend_smooth_and_applied = m_pitch_bend_smooth * m_pitchbend_amount;

		m_modwheel_smooth =
		    m_modwheel_smooth * PITCHBEND_SMOOTHIN_FACTOR + (1.f - PITCHBEND_SMOOTHIN_FACTOR) * (*m_modwheel);

		m_x_smooth = m_x_smooth * PAD_SMOOTHIN_FACTOR + (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_x);
		m_y_smooth = m_y_smooth * PAD_SMOOTHIN_FACTOR + (1.f - PAD_SMOOTHIN_FACTOR) * (*m_xy_y);

		m_master_smooth = m_master_smooth * GAIN_SMOOTHIN_FACTOR + (1 - GAIN_SMOOTHIN_FACTOR) * (m_master_control);

		//============================================================
		//========================== MIDI ============================
		//============================================================
		while (midi_message_remaining && midi_message_sample <= sample) {
			handleMidiMessage(midi_message);
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

		// global lfo and envelope
		if (m_render_ADSR[1]) {
			m_global_env_mod_source = m_global_env.doEnvelope();
		}

		if (m_render_LFO[3]) {
			m_global_lfo.update();
			m_global_lfo_mod_source = m_global_lfo.doOscillate();
		}

		float stereo_signal[2] = {0};

		// output var for the individual oscs and filters - these are modulation sources as well
		memset(m_osc_output, 0, sizeof(float) * VOICES * 3);
		memset(m_filter_output, 0, sizeof(float) * VOICES * 2);

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

				for (int osc = 0; osc < 3; ++osc) {

					switch (m_osc_type[osc]) {
					case OSC_TYPE_ANALOG:
						m_voice[voice].analog_osc[osc].update();
						m_osc_output[voice][osc] += m_voice[voice].analog_osc[osc].doOscillateWithSync();
						break;
					case OSC_TYPE_WAVETABLE:
						m_voice[voice].wavetable_osc[osc].update();
						//set modulation envelope/lfo
						m_voice[voice].wavetable_osc[osc].setPosModValue(
						    m_osc_wavetable_source_lfo[osc] ? m_lfo[voice][0] : m_adsr[voice][2]);
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

					// apply volume & modulation
					float osc_vol_modded = m_osc_vol_smooth[osc];
					if (*m_osc_vol_mod[voice][osc]) {
						if (*m_osc_vol_mod[voice][osc] < 0.f) {
							//negative modulation just modulates down to -inf dB
							osc_vol_modded = m_osc_vol_smooth[osc] * (1.f + *m_osc_vol_mod[voice][osc]);
							osc_vol_modded = osc_vol_modded < 0 ? 0 : osc_vol_modded;
						} else {
							if (m_osc_vol_smooth[osc] > MINUS_12_dB_GAIN) {
								// volume level above -12dB, modulate to plus 12 dB
								osc_vol_modded *= pow(PLUS_12_dB_GAIN, *m_osc_vol_mod[voice][osc]);
								osc_vol_modded = osc_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : osc_vol_modded;
							} else {
								// if volume level is below -12dB then just modulate up to 0dB
								osc_vol_modded += (1.f - osc_vol_modded) * *m_osc_vol_mod[voice][osc];
								osc_vol_modded = osc_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : osc_vol_modded;
							}
						}
					}
					m_osc_output[voice][osc] *= osc_vol_modded;
				} // osc loop

				//===== FILTERS ======

				float filter_input[2] = {0};
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
						m_filter_output[voice][fil] = m_voice[voice].ladder_filter[fil].doFilter(filter_input[fil]);
						break;
					case FILTER_TYPE_SEM12:
						m_voice[voice].SEM_filter_12[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].SEM_filter_12[fil].update();
						m_filter_output[voice][fil] = m_voice[voice].SEM_filter_12[fil].doFilter(filter_input[fil]);
						break;
					case FILTER_TYPE_KORG_LP:
					case FILTER_TYPE_KORG_HP:
						m_voice[voice].korg_filter[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].korg_filter[fil].update();
						m_filter_output[voice][fil] = m_voice[voice].korg_filter[fil].doFilter(filter_input[fil]);
						break;
					case FILTER_TYPE_DIODE:
						m_voice[voice].diode_filter[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].diode_filter[fil].update();
						m_filter_output[voice][fil] = m_voice[voice].diode_filter[fil].doFilter(filter_input[fil]);
						break;
					case FILTER_TYPE_FORMANT:
						m_voice[voice].formant_filter[fil].m_freq_base = m_fil_freq_smooth[fil];
						m_voice[voice].formant_filter[fil].update();
						m_filter_output[voice][fil] = m_voice[voice].formant_filter[fil].doFilter(filter_input[fil]);
						break;
					case FILTER_TYPE_COMB:
						m_voice[voice].comb_filter[fil].setCombFreq(m_fil_freq_smooth[fil]);
						m_filter_output[voice][fil] = m_voice[voice].comb_filter[fil].doFilter(filter_input[fil]);
						break;
					case FILTER_TYPE_RINGMOD:
						m_voice[voice].ring_mod[fil].setBaseFrequency(m_fil_freq_smooth[fil]);
						m_voice[voice].ring_mod[fil].setGlideTargetFrequency(m_fil_freq_smooth[fil]);
						m_voice[voice].ring_mod[fil].update();
						m_filter_output[voice][fil] = m_voice[voice].ring_mod[fil].doRingModulator(filter_input[fil]);
						break;
					}

					//apply gain & -modulation
					if (m_fil_type[fil] != FILTER_TYPE_NONE) {
						// apply volume & modulation
						float fil_vol_modded = m_fil_gain_smooth[fil];
						if (*m_fil_12_vol_mod[voice][fil]) {
							if (*m_fil_12_vol_mod[voice][fil] < 0.f) {
								//negative modulation just modulates down to -inf dB
								fil_vol_modded = m_fil_gain_smooth[fil] * (1.f + *m_fil_12_vol_mod[voice][fil]);
								fil_vol_modded = fil_vol_modded < 0 ? 0 : fil_vol_modded;
							} else {
								if (m_fil_gain_smooth[fil] > MINUS_12_dB_GAIN) {
									// volume level above -12dB, modulate to plus 12 dB
									fil_vol_modded *= pow(PLUS_12_dB_GAIN, *m_fil_12_vol_mod[voice][fil]);
									fil_vol_modded =
									    fil_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : fil_vol_modded;
								} else {
									// if volume level is below -12dB then just modulate up to 0dB
									fil_vol_modded += (1.f - fil_vol_modded) * *m_fil_12_vol_mod[voice][fil];
									fil_vol_modded =
									    fil_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : fil_vol_modded;
								}
							}
						}
						m_filter_output[voice][fil] *= fil_vol_modded;
					}

					// add first filter to second filter input
					if (fil == 0 && *m_fil2_fil1) {
						filter_input[1] += m_filter_output[voice][0];
					}
				} // filter loop

				float voices_output = 0;

				if (*m_fil1_to_amp) {
					voices_output += m_filter_output[voice][0];
				}
				if (*m_fil2_to_amp) {
					voices_output += m_filter_output[voice][1];
				}

				//SIGNAL IS POLY STEREO FROM HERE ON
				float stereo_signal_voice[2];

				//===== AMPLIFIER ======
				m_voice[voice].amp.doAmplifier(voices_output, stereo_signal_voice[0], stereo_signal_voice[1]);

				//===== DISTORTION ======
				if (m_dist_on) {
					stereo_signal_voice[0] = m_voice[voice].distortion[0].doDistortion(stereo_signal_voice[0]);
					stereo_signal_voice[1] = m_voice[voice].distortion[1].doDistortion(stereo_signal_voice[1]);
				}

				stereo_signal[0] += stereo_signal_voice[0] * m_adsr[voice][0];
				stereo_signal[1] += stereo_signal_voice[1] * m_adsr[voice][0];
			} // voice active
		}     // voice loop

		for (int channel = 0; channel < 2; ++channel) {

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
				stereo_signal[channel] = m_ladder_filter[channel].doFilter(stereo_signal[channel]);
				break;
			case FILTER_TYPE_SEM12:
				m_SEM_filter_12[channel].m_freq_base = m_fil_freq_smooth[2];
				m_SEM_filter_12[channel].update();
				stereo_signal[channel] = m_SEM_filter_12[channel].doFilter(stereo_signal[channel]);
				break;
			case FILTER_TYPE_KORG_LP:
			case FILTER_TYPE_KORG_HP:
				m_korg_filter[channel].m_freq_base = m_fil_freq_smooth[2];
				m_korg_filter[channel].update();
				stereo_signal[channel] = m_korg_filter[channel].doFilter(stereo_signal[channel]);
				break;
			case FILTER_TYPE_DIODE:
				m_diode_filter[channel].m_freq_base = m_fil_freq_smooth[2];
				m_diode_filter[channel].update();
				stereo_signal[channel] = m_diode_filter[channel].doFilter(stereo_signal[channel]);
				break;
			case FILTER_TYPE_FORMANT:
				m_formant_filter[channel].m_freq_base = m_fil_freq_smooth[2];
				m_formant_filter[channel].update();
				stereo_signal[channel] = m_formant_filter[channel].doFilter(stereo_signal[channel]);
				break;
			case FILTER_TYPE_COMB:
				m_comb_filter[channel].setCombFreq(m_fil_freq_smooth[2]);
				stereo_signal[channel] = m_comb_filter[channel].doFilter(stereo_signal[channel]);
				break;
			case FILTER_TYPE_RINGMOD:
				m_ring_mod[channel].setBaseFrequency(m_fil_freq_smooth[2]);
				m_ring_mod[channel].setGlideTargetFrequency(m_fil_freq_smooth[2]);

				m_ring_mod[channel].update();
				stereo_signal[channel] = m_ring_mod[channel].doRingModulator(stereo_signal[channel]);
				break;
			default:
				break;
			}
			//apply gain & -modulation
			if (m_fil_type[2] != FILTER_TYPE_NONE) {
				// apply volume & modulation
				float fil_vol_modded = m_fil_gain_smooth[2];
				if (*m_fil_3_vol_mod) {
					if (*m_fil_3_vol_mod < 0.f) {
						//negative modulation just modulates down to -inf dB
						fil_vol_modded = m_fil_gain_smooth[2] * (1.f + *m_fil_3_vol_mod);
						fil_vol_modded = fil_vol_modded < 0 ? 0 : fil_vol_modded;
					} else {
						if (m_fil_gain_smooth[2] > MINUS_12_dB_GAIN) {
							// volume level above -12dB, modulate to plus 12 dB
							fil_vol_modded *= pow(PLUS_12_dB_GAIN, *m_fil_3_vol_mod);
							fil_vol_modded = fil_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : fil_vol_modded;
						} else {
							// if volume level is below -12dB then just modulate up to 0dB
							fil_vol_modded += (1.f - fil_vol_modded) * *m_fil_3_vol_mod;
							fil_vol_modded = fil_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : fil_vol_modded;
						}
					}
				}
				stereo_signal[channel] *= fil_vol_modded;
			}

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

			//===== OUTPUT ======

			// apply volume & modulation
			float master_vol_modded = m_master_smooth;
			if (*m_master_mod) {
				if (*m_master_mod < 0.f) {
					//negative modulation just modulates down to -inf dB
					master_vol_modded = m_master_smooth * (1.f + *m_master_mod);
					master_vol_modded = master_vol_modded < 0 ? 0 : master_vol_modded;
				} else {
					if (m_master_smooth > MINUS_12_dB_GAIN) {
						// volume level above -12dB, modulate to plus 12 dB
						master_vol_modded *= pow(PLUS_12_dB_GAIN, *m_master_mod);
						master_vol_modded = master_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : master_vol_modded;
					} else {
						// if volume level is below -12dB then just modulate up to 0dB
						master_vol_modded += (1.f - master_vol_modded) * *m_master_mod;
						master_vol_modded = master_vol_modded > PLUS_12_dB_GAIN ? PLUS_12_dB_GAIN : master_vol_modded;
					}
				}
			}

			auto *channelData   = buffer.getWritePointer(channel);
			channelData[sample] = stereo_signal[channel] * master_vol_modded;

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
		//onSetStateInformation();
	}

	m_editor_pointer = editor;

	return editor;
}

//==============================================================================
void OdinAudioProcessor::getStateInformation(MemoryBlock &destData) {
	// this is called when DAW saves a file

	// disable for standalone plugins
	//if (wrapperType == wrapperType_Standalone) {
	//	return;
	//}

	auto state = m_value_tree.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

//this is called when DAW restores a file
void OdinAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {

	// disable for standalone plugins
	if (wrapperType == wrapperType_Standalone) {
		return;
	}

	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr) {
		if (xmlState->hasTagName(m_value_tree.state.getType())) {

			//avoid reading from newer patch versions
			int patch_migration_version_read =
			    xmlState->getChildByName("misc")->getIntAttribute("patch_migration_version");
			if (patch_migration_version_read > ODIN_PATCH_MIGRATION_VERSION) {
				AlertWindow::showMessageBox(
				    AlertWindow::AlertIconType::WarningIcon,
				    "You are trying to load a project which was saved with a newer Version of Odin2. Please go to "
				    "www.thewavewarden.com and download the newest version to properly use this project!",
				    "Thanks, I will!");
				return;
			}

			//load data
			readPatch(ValueTree::fromXml(*xmlState));

			//set legato here, so it is active before opening the GUI
			setPolyLegato((float)m_value_tree.state.getChildWithName("misc")["legato"] > 0.5);

			//set the correct version since an old one was maybe set from patch
			m_value_tree.state.getChildWithName("misc").setProperty("version_minor", ODIN_MINOR_VERSION, nullptr);
			m_value_tree.state.getChildWithName("misc").setProperty("version_patch", ODIN_PATCH_VERSION, nullptr);
			m_value_tree.state.getChildWithName("misc").setProperty(
			    "patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);

			m_force_values_onto_gui = true;

			//create midi learn map from valuetree
			for (int i = 0; i < m_value_tree_midi_learn.getNumProperties(); ++i) {
				m_midi_control_param_map.emplace(
				    (int)m_value_tree_midi_learn[m_value_tree_midi_learn.getPropertyName(i)],
				    m_value_tree.getParameter(m_value_tree_midi_learn.getPropertyName(i)));
			}

			createDrawTablesFromValueTree();

			if (m_editor_pointer) {
				m_editor_pointer->forceValueTreeOntoComponents(false);
			}
		}
	}
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
		m_ring_mod[stereo].setSampleRate(p_samplerate);
	}

	m_delay.setSampleRate(p_samplerate);
	m_phaser.setSampleRate(p_samplerate);
	m_global_env.setSampleRate(p_samplerate);
	m_global_lfo.setSampleRate(p_samplerate);
	m_arpeggiator.setSampleRate(p_samplerate);
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
		//todo these are probably set wrongly on plugin load....
		m_voice[voice].setOscSyncOscillator(&(m_voice[voice].analog_osc[0]));
	}
}

void OdinAudioProcessor::setPitchWheelValue(int p_value) {
	*m_pitchbend = (float)(p_value - 8192) / 8192.f;
	updatePitchWheelGUI(*m_pitchbend);
}

void OdinAudioProcessor::setModWheelValue(int p_value) {
	*m_modwheel = (float)(p_value) / 128.f;
	updateModWheelGUI(*m_modwheel);
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
	//DBG("kill global env");
}

void OdinAudioProcessor::handleMidiNoteOn(int p_midi_note, int p_midi_velocity){
	if(m_arpeggiator_on){
		m_arpeggiator.midiNoteOn(p_midi_note, p_midi_velocity);
	} else {
		midiNoteOn(p_midi_note, p_midi_velocity);
	}
}

void OdinAudioProcessor::handleMidiNoteOff(int p_midi_note){
	if(m_arpeggiator_on){
		m_arpeggiator.midiNoteOff(p_midi_note);
	} else {
		midiNoteOff(p_midi_note);
	}
}


void OdinAudioProcessor::midiNoteOn(int p_midi_note, int p_midi_velocity) {

	m_global_env.restartEnvelope();
	if (*m_lfo4_reset) {
		m_global_lfo.voiceStart(false);
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

	int unison_counter = 0;
	int unison_voices  = m_value_tree.state.getChildWithName("misc")["unison_voices"];
	auto voice_numbers = m_voice_manager.getVoices(p_midi_note, unison_voices);
	if (m_last_midi_note == -1) {
		// first time glide - dont glide
		m_last_midi_note = p_midi_note;
	}
	for (int new_voice : voice_numbers) {
		m_voice[new_voice].start(
		    p_midi_note,
		    p_midi_velocity,
		    m_last_midi_note,
		    m_unison_pan_positions[unison_voices][unison_counter],
			//we "shuffle" pan positions around to get detune positions:
		    m_unison_pan_positions[unison_voices][m_unison_detune_positions[unison_voices][unison_counter]],
		    m_unison_gain_factors[unison_voices],
		    unison_voices > 1);
		DBG("NoteOn,  key " + std::to_string(p_midi_note) + ", voice " + std::to_string(new_voice));
		//if (m_voice_manager.legatoEnabled()) {
		//m_voice[new_voice].amp.setMIDIVelocityLegato(p_midi_velocity);
		//} else {
		m_voice[new_voice].amp.setMIDIVelocity(p_midi_velocity);
		//}
		m_mod_matrix.setMostRecentVoice(new_voice);
		++unison_counter;
	}
	m_last_midi_note = p_midi_note;

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

void OdinAudioProcessor::midiNoteOff(int p_midi_note) {
	DBG("NoteOff, key " + std::to_string(p_midi_note));

	if (!m_voice_manager.getSustainActive()) {
		for (int voice = 0; voice < VOICES; ++voice) {
			if (m_voice[voice].keyUp(p_midi_note)) {
				//DBG("KeyUp on voice " + std::to_string(voice));
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

void OdinAudioProcessor::resetAudioEngine() {

	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].hardReset();
	}
	for (int stereo = 0; stereo < 2; ++stereo) {
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
		m_voice[voice].setBPM(p_BPM, m_lfo1_sync, m_lfo2_sync, m_lfo3_sync);
	}
	if (m_delay_sync) {
		m_delay.setFreqBPM(p_BPM);
	}
	if (m_phaser_sync) {
		m_phaser.setFreqBPM(p_BPM);
	}
	if (m_flanger_sync) {
		m_flanger[0].setFreqBPM(p_BPM);
		m_flanger[1].setFreqBPM(p_BPM);
	}
	if (m_chorus_sync) {
		m_chorus[0].setFreqBPM(p_BPM);
		m_chorus[1].setFreqBPM(p_BPM);
	}
	if (m_lfo4_sync) {
		m_global_lfo.setFreqBPM(p_BPM);
	}
	m_arpeggiator.setBPM(p_BPM);
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
	m_value_tree_fx         = m_value_tree.state.getChildWithName("fx");
	m_value_tree_osc        = m_value_tree.state.getChildWithName("osc");
	m_value_tree_mod        = m_value_tree.state.getChildWithName("mod");
	m_value_tree_misc       = m_value_tree.state.getChildWithName("misc");
	m_value_tree_lfo        = m_value_tree.state.getChildWithName("lfo");
	m_value_tree_draw       = m_value_tree.state.getChildWithName("draw");
	m_value_tree_midi_learn = m_value_tree.state.getChildWithName("midi_learn");
}

void OdinAudioProcessor::handleMidiMessage(const MidiMessage &p_midi_message) {
	//DBG(p_midi_message.getDescription());
	// apply midi message
	if (p_midi_message.isNoteOn()) {
		midiNoteOn(p_midi_message.getNoteNumber(), p_midi_message.getVelocity());
	} else if (p_midi_message.isNoteOff()) {
		midiNoteOff(p_midi_message.getNoteNumber());
	} else if (p_midi_message.isPitchWheel()) {
		setPitchWheelValue(p_midi_message.getPitchWheelValue());
	} else if (p_midi_message.isController() && p_midi_message.getControllerNumber() == 1) { // modwheel
		setModWheelValue(p_midi_message.getControllerValue());
	} else if (p_midi_message.isSustainPedalOn()) {
		m_voice_manager.setSustainActive(true);
		DBG("Sustain pedal pressed");
	} else if (p_midi_message.isSustainPedalOff()) {
		DBG("Sustain pedal released");
		m_voice_manager.setSustainActive(false);
		for (int voice = 0; voice < VOICES; ++voice) {
			if (m_voice_manager.isOnKillList(voice)) {
				m_voice[voice].startRelease();
			}
		}
		m_voice_manager.clearKillList();
		checkEndGlobalEnvelope();
	} else if (p_midi_message.isAftertouch()) {
		// todo this is untested
		DBG("receiving afterouch, note: " + std::to_string(p_midi_message.getNoteNumber()) +
		    ", value: " + std::to_string((float)p_midi_message.getAfterTouchValue() / 127.f));
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setAftertouch(p_midi_message.getNoteNumber(),
			                             (float)p_midi_message.getAfterTouchValue() / 127.f);
		}
	} else if (p_midi_message.isChannelPressure()) {
		m_MIDI_channel_pressure = (float)p_midi_message.getChannelPressureValue() / 127.f;
	} else if (p_midi_message.isSoftPedalOn()) {
		m_soft_pedal = 1.f;
	} else if (p_midi_message.isSoftPedalOff()) {
		m_soft_pedal = 0.f;
	}

	else {
		if (!p_midi_message.isMidiClock()) {
			//DBG("UNHANDELED MIDI MESSAGE: " + p_midi_message.getDescription());
			if (p_midi_message.isController()) {
				//DBG("Controller with number: " + std::to_string(p_midi_message.getControllerNumber()));
			}
		}
	}

	if ((p_midi_message.isController() && !(p_midi_message.isAllNotesOff()))) {

		// midi learn
		if (m_midi_learn_parameter_active) {
			m_midi_control_param_map.emplace(p_midi_message.getControllerNumber(),
			                                 m_value_tree.getParameter(m_midi_learn_parameter_ID));
			m_midi_learn_control->setMidiControlActive();
			m_midi_learn_parameter_active = false;
			m_midi_learn_control          = nullptr;
			//add control to value tree
			m_value_tree.state.getChildWithName("midi_learn")
			    .setProperty(m_midi_learn_parameter_ID, p_midi_message.getControllerNumber(), nullptr);

			DBG(m_value_tree.state.toXmlString());

			DBG("Added MIDI control for parameter " +
			    m_midi_control_param_map.find(p_midi_message.getControllerNumber())->second->paramID +
			    " on controller number " + std::to_string(p_midi_message.getControllerNumber()));
			m_midi_learn_parameter_ID = "";
#ifdef ODIN_DEBUG
			int counter = 1;
			DBG("=========");
			for (auto const &control : m_midi_control_param_map) {
				DBG(std::to_string(counter++) + ": " + control.second->paramID.toStdString());
			}
			DBG("=========");
#endif
		}

		// do midi control
		for (auto const &control : m_midi_control_param_map) {
			if (control.first == p_midi_message.getControllerNumber()) {
				//todo replace lock by async MM call
				//const MessageManagerLock mmLock;
				control.second->setValueNotifyingHost(/*control.second->convertFrom0to1(*/
				                                      (float)p_midi_message.getControllerValue() / 127.f); //));
			}
		}
	}
}