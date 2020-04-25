/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableSlider.h"
#include "Knob.h"
#include "LeftRightButton.h"
#include "ModMatrix.h"
#include "OdinButton.h"
#include "OdinTreeListener.h"
#include "Voice.h"
#include "audio/FX/Chorus.h"
#include "audio/FX/Delay.h"
#include "audio/FX/Flanger.h"
#include "audio/FX/Phaser.h"
#include "audio/FX/RingModulator.h"
#include "audio/Filters/CombFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Oscillators/WavetableContainer.h"

//todo remove
#include <chrono>

//#define ODIN_PROFILING

class OdinAudioProcessorEditor;

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

	WavetableContainer *getWavetableContainerPointer() {
		return &m_WT_container;
	}

	void startMidiLearn(const String &p_parameter_ID, OdinMidiLearnBase *p_GUI_control) {
		DBG("MIDI LEARN FOR PARAMETER " + p_parameter_ID + " WAS SIGNALED!");
		if (m_midi_learn_parameter_active) {
			m_midi_learn_control->stopMidiLearn();
		}
		m_midi_learn_parameter_ID     = p_parameter_ID;
		m_midi_learn_parameter_active = true;
		m_midi_learn_control          = p_GUI_control;
	}
	void midiForget(const String &p_parameter_ID, OdinMidiLearnBase *p_GUI_control) {
		for (std::multimap<int, RangedAudioParameter *>::iterator iter = m_midi_control_param_map.begin();
		     iter != m_midi_control_param_map.end();) {
			std::multimap<int, RangedAudioParameter *>::iterator erase_iter = iter++;
			if (erase_iter->second == m_value_tree.getParameter(p_parameter_ID)) {
				m_midi_control_param_map.erase(erase_iter);

				//remove the control from the list in valuetree
				if (m_value_tree.state.getChildWithName("midi_learn").hasProperty(p_parameter_ID)) {
					m_value_tree.state.getChildWithName("midi_learn").removeProperty(p_parameter_ID, nullptr);
				}

				DBG(m_value_tree.state.toXmlString());

#ifndef ODIN_DEBUG
				return;
#endif
			}
		}

#ifdef ODIN_DEBUG
		int counter = 1;
		DBG("=========");
		for (auto const &control : m_midi_control_param_map) {
			DBG(std::to_string(counter++) + ": " + control.second->paramID.toStdString());
		}
		DBG("=========");
#endif
	}

	void stopMidiLearn() {

		m_midi_learn_parameter_active = false;
		m_midi_learn_parameter_ID     = "";
		m_midi_learn_control          = nullptr;
	}

	std::function<void()> onSetStateInformation = []() {
		DBG("ATTENTION: onSetStateInformation() was not set yet, but "
		    "called!\n\n\n");
	};

	std::function<void(float)> updatePitchWheelGUI = [](float p_value) {
		DBG("ATTENTION: updatePitchWheelGUI() was not set yet, but "
		    "called!\n\n\n");
	};

	std::function<void(float)> updateModWheelGUI = [](float p_value) {
		DBG("ATTENTION: updateModWheelGUI() was not set yet, but "
		    "called!\n\n\n");
	};

	void midiNoteOff(int p_midi_note);
	void midiNoteOn(int p_midi_note, int p_midi_velocity);

	void setPolyLegato(bool p_is_poly) {
		bool legato_was_changed = m_voice_manager.setPolyLegato(p_is_poly);
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice[voice].setPolyLegato(p_is_poly);
		}

		if (legato_was_changed) {
			// reset engine here to get rid of trailing notes
			resetAudioEngine();
		}
	}

	// this should be called when patches are loaded and legato
	// enabled. it doesn't change values but clears all buffer
	// and makes it "untouched"
	void resetAudioEngine();
	void setFXButtonsPosition(int p_delay, int p_phaser, int p_flanger, int p_chorus);
	void attachNonParamListeners();
	void migratePatch(int p_patch_version_to_migrate_from);
	void readPatch(const ValueTree &newState);

private:
#ifdef ODIN_PROFILING
#define PROFILING_SAMPLES 44100
	int m_profiling_counter = 0;
#endif
	// OdinAudioProcessorEditor* m_editor = nullptr;
	void setFilter3EnvValue();
	void setBPM(float BPM);
	void setPitchWheelValue(int p_value);
	void setModWheelValue(int p_value);
	void checkEndGlobalEnvelope();
	void setModulationPointers();
	void addNonAudioParametersToTree();
	void handleMidiMessage(const MidiMessage &p_midi_message);

	bool m_midi_learn_parameter_active      = false;
	OdinMidiLearnBase *m_midi_learn_control = nullptr;
	String m_midi_learn_parameter_ID        = "";
	std::multimap<int, RangedAudioParameter *> m_midi_control_param_map;

	float m_osc_vol_smooth[3]             = {1.f, 1.f, 1.f}; // factor
	float m_fil_gain_smooth[3]            = {1.f, 1.f, 1.f}; // factor
	float m_osc_vol_control[3]            = {1.f, 1.f, 1.f}; // factor
	float m_fil_gain_control[3]           = {1.f, 1.f, 1.f}; // factor
	float m_fil_freq_control[3]           = {2000, 2000, 2000};
	float m_fil_freq_smooth[3]            = {2000, 2000, 2000};
	float m_pitch_bend_smooth             = 0.f;
	float m_pitch_bend_smooth_and_applied = 0.f;
	float m_modwheel_smooth               = 0.f;
	float m_x_smooth                      = 0.f;
	float m_y_smooth                      = 0.f;
	float m_master_smooth                 = 1.f;
	float m_master_control                = 1.f; // factor

	int m_last_midi_note = -1;

	void setSampleRate(float p_samplerate);
	void initializeModules();

	VoiceManager m_voice_manager;
	AudioProcessorValueTreeState m_value_tree;

	OdinTreeListener m_tree_listener_osc_pitch;
	OdinTreeListener m_tree_listener_osc_misc;
	OdinTreeListener m_tree_listener_osc_analog;
	OdinTreeListener m_tree_listener_osc_multi;
	OdinTreeListener m_tree_listener_osc_xy;
	OdinTreeListener m_tree_listener_osc_chip;
	OdinTreeListener m_tree_listener_osc_arp;
	OdinTreeListener m_tree_listener_osc_fm;
	OdinTreeListener m_tree_listener_osc_noise;
	OdinTreeListener m_tree_listener_fil_top;
	OdinTreeListener m_tree_listener_fil_center;
	OdinTreeListener m_tree_listener_fil_misc;
	OdinTreeListener m_tree_listener_fil_formant;
	OdinTreeListener m_tree_listener_amp;
	OdinTreeListener m_tree_listener_delay;
	OdinTreeListener m_tree_listener_chorus;
	OdinTreeListener m_tree_listener_phaser;
	OdinTreeListener m_tree_listener_flanger;
	OdinTreeListener m_tree_listener_adsr1;
	OdinTreeListener m_tree_listener_adsr2;
	OdinTreeListener m_tree_listener_adsr3;
	OdinTreeListener m_tree_listener_adsr4;
	OdinTreeListener m_tree_listener_lfo1;
	OdinTreeListener m_tree_listener_lfo2;
	OdinTreeListener m_tree_listener_lfo3;
	OdinTreeListener m_tree_listener_lfo4;
	//OdinTreeListener m_tree_listener_amount1;
	//OdinTreeListener m_tree_listener_amount2;
	//OdinTreeListener m_tree_listener_amount3;
	OdinTreeListener m_tree_listener_general_misc;

	ValueTree m_value_tree_draw;
	ValueTree m_value_tree_fx;
	ValueTree m_value_tree_lfo;
	ValueTree m_value_tree_misc;
	ValueTree m_value_tree_mod;
	ValueTree m_value_tree_osc;
	ValueTree m_value_tree_midi_learn;

	// these listens to non automatable vars
	OdinTreeListenerNonParam m_non_param_listener_fx;
	OdinTreeListenerNonParam m_non_param_listener_lfo;
	OdinTreeListenerNonParam m_non_param_listener_misc;
	OdinTreeListenerNonParam m_non_param_listener_mod;
	OdinTreeListenerNonParam m_non_param_listener_osc;

	void treeValueChangedNonParamFX(ValueTree &tree, const Identifier &identifier);
	void treeValueChangedNonParamLFO(ValueTree &tree, const Identifier &identifier);
	void treeValueChangedNonParamMisc(ValueTree &tree, const Identifier &identifier);
	void treeValueChangedNonParamMod(ValueTree &tree, const Identifier &identifier);
	void treeValueChangedNonParamOsc(ValueTree &tree, const Identifier &identifier);

	void treeValueChangedOscPitch(const String &p_ID, float p_new_value);
	void treeValueChangedOscAnalog(const String &p_ID, float p_new_value);
	void treeValueChangedOscMisc(const String &p_ID, float p_new_value);
	void treeValueChangedOscMulti(const String &p_ID, float p_new_value);
	void treeValueChangedOscXY(const String &p_ID, float p_new_value);
	void treeValueChangedOscChip(const String &p_ID, float p_new_value);
	void treeValueChangedOscArp(const String &p_ID, float p_new_value);
	void treeValueChangedOscFM(const String &p_ID, float p_new_value);
	void treeValueChangedOscNoise(const String &p_ID, float p_new_value);
	void treeValueChangedFilTop(const String &p_ID, float p_new_value);
	void treeValueChangedFilCenter(const String &p_ID, float p_new_value);
	void treeValueChangedFilMisc(const String &p_ID, float p_new_value);
	void treeValueChangedFilFormant(const String &p_ID, float p_new_value);
	void treeValueChangedAmp(const String &p_ID, float p_new_value);
	void treeValueChangedDelay(const String &p_ID, float p_new_value);
	void treeValueChangedChorus(const String &p_ID, float p_new_value);
	void treeValueChangedPhaser(const String &p_ID, float p_new_value);
	void treeValueChangedFlanger(const String &p_ID, float p_new_value);
	void treeValueChangedADSR1(const String &p_ID, float p_new_value);
	void treeValueChangedADSR2(const String &p_ID, float p_new_value);
	void treeValueChangedADSR3(const String &p_ID, float p_new_value);
	void treeValueChangedADSR4(const String &p_ID, float p_new_value);
	void treeValueChangedLFO1(const String &p_ID, float p_new_value);
	void treeValueChangedLFO2(const String &p_ID, float p_new_value);
	void treeValueChangedLFO3(const String &p_ID, float p_new_value);
	void treeValueChangedLFO4(const String &p_ID, float p_new_value);
	//void treeValueChangedAmount1(const String &p_ID, float p_new_value);
	//void treeValueChangedAmount2(const String &p_ID, float p_new_value);
	//void treeValueChangedAmount3(const String &p_ID, float p_new_value);
	void treeValueChangedGeneralMisc(const String &p_ID, float p_new_value);

	WavetableContainer m_WT_container;

	Voice m_voice[VOICES];

	LadderFilter m_ladder_filter[2];
	SEMFilter12 m_SEM_filter_12[2];
	Korg35Filter m_korg_filter[2];
	DiodeFilter m_diode_filter[2];
	FormantFilter m_formant_filter[2];
	CombFilter m_comb_filter[2];
	RingModulator m_ring_mod[2];
	Delay m_delay;   //is stereo delay
	Phaser m_phaser; // is stereo phaser
	Flanger m_flanger[2];
	Chorus m_chorus[2];

	LFO m_global_lfo;
	ADSREnvelope m_global_env;

	ModMatrix m_mod_matrix;
	ModSources m_mod_sources;
	ModDestinations m_mod_destinations;

	bool m_dist_on      = false;
	bool m_lfo1_sync    = false;
	bool m_lfo2_sync    = false;
	bool m_lfo3_sync    = false;
	bool m_lfo4_sync    = false;
	bool m_delay_sync   = false;
	bool m_chorus_sync  = false;
	bool m_phaser_sync  = false;
	bool m_flanger_sync = false;

	bool m_osc_wavetable_source_lfo[3] = {0}; //false <=> envelope

	bool m_render_LFO[4]  = {0};
	bool m_render_ADSR[2] = {0}; // 0 = mod, 1 = global

	// MOD SOURCES
	float m_adsr[VOICES][3]          = {0.f};
	float m_lfo[VOICES][3]           = {0.f};
	float m_global_env_mod_source    = 0;
	float m_global_lfo_mod_source    = 0;
	float m_filter_output[VOICES][2] = {0.f};
	float m_osc_output[VOICES][3]    = {0.f};
	float m_MIDI_channel_pressure    = 0.f;
	float m_constant                 = 1.f;
	float m_soft_pedal               = 0.f;
	float m_x                        = 0.f;
	float m_y                        = 0.f;

	// MOD DEST
	float *m_master_mod;
	int m_delay_position   = 0;
	int m_phaser_position  = 1;
	int m_chorus_position  = 2;
	int m_flanger_position = 3;

	OdinAudioProcessorEditor *m_editor_pointer;

	int m_osc_type[3] = {OSC_TYPE_ANALOG, 1, 1};
	int m_fil_type[3] = {FILTER_TYPE_LP24, 1, 1};

	bool m_is_standalone_plugin = true; // set true since is is only set after createEditor()

	float m_BPM = 120;

	bool m_force_values_onto_gui = false; // used for loading state and then remembering to force values once
	                                      // the editor was created
#include "AudioVarDeclarations.h"
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OdinAudioProcessor)
};
