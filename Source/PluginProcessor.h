/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include "OdinTreeListener.h"
#include "audio/FX/Chorus.h"
#include "audio/FX/Delay.h"
#include "audio/FX/Flanger.h"
#include "audio/FX/Phaser.h"
#include "audio/FX/ZitaReverb.h"
#include "audio/FX/RingModulator.h"
#include "audio/Filters/CombFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Oscillators/WavetableContainer.h"
#include "audio/Voice.h"
#include "gui/DrawableSlider.h"
#include "gui/OdinKnob.h"
#include "gui/LeftRightButton.h"
#include "gui/ModMatrix.h"
#include "gui/OdinArpeggiator.h"
#include "gui/OdinButton.h"

#include <list>

class OdinEditor;

class OdinAudioProcessor : public AudioProcessor {
public:

	OdinAudioProcessor();
	~OdinAudioProcessor();

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

	void processBlock(AudioBuffer<float> &, MidiBuffer &) override;
	AudioProcessorEditor *createEditor() override;
	bool hasEditor() const override;
	void onEditorDestruction();
	const String getName() const override;
	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const String getProgramName(int index) override;
	void changeProgramName(int index, const String &newName) override;
	void getStateInformation(MemoryBlock &destData) override;
	void setStateInformation(const void *data, int sizeInBytes) override;
	WavetableContainer *getWavetableContainerPointer();
	void midiNoteOff(int p_midi_note);
	void midiNoteOn(int p_midi_note,
	                int p_midi_velocity,
	                float p_arp_mod_1       = 0.f,
	                float p_arp_mod_2       = 0.f,
	                bool p_add_to_mono_list = true);
	void handleMidiNoteOff(int p_midi_note);
	void handleMidiNoteOn(int p_midi_note, int p_midi_velocity);
	void allNotesOff();
	void setMonoPolyLegato(PlayModes p_mode);
	// this should be called when patches are loaded or playmode has changed
	// .it doesn't change values but clears all buffers
	// and makes it "untouched"
	void resetAudioEngine();
	void setFXButtonsPosition(int p_delay, int p_phaser, int p_flanger, int p_chorus, int p_reverb);
	void attachNonParamListeners();
	void migratePatch(ValueTree &p_patch);
	void readPatch(const ValueTree &newState);

	std::function<void()> onSetStateInformation    = []() {};
	std::function<void(float)> updatePitchWheelGUI = [](float p_value) {};
	std::function<void(float)> updateModWheelGUI   = [](float p_value) {};

	// this is used to retrigger a held down note if the note after it was released. It stores note and velocity
	std::list<std::pair<int, int>> m_playmode_mono_note_list;
	Atomic<int> m_step_led_active = -1;
	Tunings::Tuning m_tuning;

private:
	void setSampleRate(float p_samplerate);
	void setFilter3EnvValue();
	void setBPM(float BPM);
	void setPitchWheelValue(int p_value);
	void setModWheelValue(int p_value);
	void checkEndGlobalEnvelope();
	void setModulationPointers();
	void addNonAudioParametersToTree();
	void handleMidiMessage(const MidiMessage &p_midi_message);
	void createDrawTablesFromValueTree();
	bool checkLoadParameter(const String &p_name);
	void writeDefaultWavedrawValuesToTree(int p_osc);
	void writeDefaultChipdrawValuesToTree(int p_osc);
	void writeDefaultSpecdrawValuesToTree(int p_osc);
	void initializeModules();

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
	void treeValueChangedReverb(const String &p_ID, float p_new_value);
	void treeValueChangedChorus(const String &p_ID, float p_new_value);
	void treeValueChangedPhaser(const String &p_ID, float p_new_value);
	void treeValueChangedArp(const String &p_ID, float p_new_value);
	void treeValueChangedFlanger(const String &p_ID, float p_new_value);
	void treeValueChangedADSR1(const String &p_ID, float p_new_value);
	void treeValueChangedADSR2(const String &p_ID, float p_new_value);
	void treeValueChangedADSR3(const String &p_ID, float p_new_value);
	void treeValueChangedADSR4(const String &p_ID, float p_new_value);
	void treeValueChangedLFO1(const String &p_ID, float p_new_value);
	void treeValueChangedLFO2(const String &p_ID, float p_new_value);
	void treeValueChangedLFO3(const String &p_ID, float p_new_value);
	void treeValueChangedLFO4(const String &p_ID, float p_new_value);
	void treeValueChangedGeneralMisc(const String &p_ID, float p_new_value);

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
	OdinTreeListener m_tree_listener_reverb;
	OdinTreeListener m_tree_listener_chorus;
	OdinTreeListener m_tree_listener_phaser;
	OdinTreeListener m_tree_listener_arp;
	OdinTreeListener m_tree_listener_flanger;
	OdinTreeListener m_tree_listener_adsr1;
	OdinTreeListener m_tree_listener_adsr2;
	OdinTreeListener m_tree_listener_adsr3;
	OdinTreeListener m_tree_listener_adsr4;
	OdinTreeListener m_tree_listener_lfo1;
	OdinTreeListener m_tree_listener_lfo2;
	OdinTreeListener m_tree_listener_lfo3;
	OdinTreeListener m_tree_listener_lfo4;
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

	WavetableContainer m_WT_container;
	OdinArpeggiator m_arpeggiator;
	Voice m_voice[VOICES];

	LadderFilter m_ladder_filter[2];
	SEMFilter12 m_SEM_filter_12[2];
	Korg35Filter m_korg_filter[2];
	DiodeFilter m_diode_filter[2];
	FormantFilter m_formant_filter[2];
	CombFilter m_comb_filter[2];
	RingModulator m_ring_mod[2];
	Delay m_delay;          //is stereo delay
	Phaser m_phaser;        // is stereo phaser
	ZitaReverb m_reverb_zita;
	Flanger m_flanger[2];
	Chorus m_chorus[2];

	LFO m_global_lfo;
	ADSREnvelope m_global_env;

	ModMatrix m_mod_matrix;
	ModSources m_mod_sources;
	ModDestinations m_mod_destinations;

	String m_midi_learn_parameter_ID        = "";
	std::multimap<int, RangedAudioParameter *> m_midi_control_param_map;
	bool m_midi_learn_parameter_active = false;

	float m_osc_vol_smooth[3]             = {1.f, 1.f, 1.f}; // factor
	float m_fil_gain_smooth[3]            = {1.f, 1.f, 1.f}; // factor
	float m_osc_vol_control[3]            = {1.f, 1.f, 1.f}; // factor
	float m_fil_gain_control[3]           = {1.f, 1.f, 1.f}; // factor
	float m_fil_freq_control[3]           = {20000, 2000, 2000};
	float m_fil_freq_smooth[3]            = {20000, 2000, 2000};
	float m_pitch_bend_smooth             = 0.f;
	float m_pitch_bend_smooth_and_applied = 0.f;
	float m_modwheel_smooth               = 0.f;
	float m_x_smooth                      = 0.f;
	float m_y_smooth                      = 0.f;
	float m_x_modded                      = 0.f;
	float m_y_modded                      = 0.f;
	float m_master_smooth                 = 1.f;
	float m_master_control                = 1.f; // factor

	int m_last_midi_note = -1;

	PlayModes m_mono_poly_legato = PlayModes::Poly;

	float m_samplerate = 44800.f;

	bool m_dist_on        = false;
	bool m_lfo1_sync      = false;
	bool m_lfo2_sync      = false;
	bool m_lfo3_sync      = false;
	bool m_lfo4_sync      = false;
	bool m_delay_sync     = false;
	bool m_chorus_sync    = false;
	bool m_phaser_sync    = false;
	bool m_flanger_sync   = false;
	bool m_arpeggiator_on = false;

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
	float m_pitchbend_amount         = 12.f;
	float m_midi_breath              = 0.f;

	// MOD DEST
	float *m_master_mod;
	float *m_osc_vol_mod[VOICES][3];
	float *m_fil_12_vol_mod[VOICES][2];
	float *m_fil_3_vol_mod;
	float *m_x_mod;
	float *m_y_mod;

	int m_delay_position   = 0;
	int m_phaser_position  = 1;
	int m_chorus_position  = 2;
	int m_flanger_position = 3;
	int m_reverb_position  = 4;

	OdinEditor *m_editor_pointer = nullptr;

	int m_osc_type[3] = {OSC_TYPE_ANALOG, 1, 1};
	int m_fil_type[3] = {FILTER_TYPE_LP24, 1, 1};

	bool m_is_standalone_plugin = true; // set true since is is only set after createEditor()

	float m_BPM = 120;

	bool m_force_values_onto_gui = false; // used for loading state and then remembering to force values once
	                                      // the editor was created

	//the panning distributions for each unison count
	std::map<int, std::vector<float>> m_unison_pan_positions = {
	    {1, {0.f}},
	    {2, {-1.f, 1.f}},
	    {3, {-1.f, 0.f, 1.f}},
	    {4, {-1.f, -0.33333f, 0.3333333f, 1.f}},
	    {6, {-1.f, -0.6f, -0.2f, 0.2f, 0.6f, 1.f}},
	    {12,
	     {-1.f, -0.8181f, -0.6363f, -0.45454f, -0.2787f, -0.0909f, 0.0909f, 0.2787f, 0.45454f, 0.6363f, 0.8181f, 1.f}}};

	// map to "shuffle" the unison pan positions to get the detune positions
	std::map<int, std::vector<int>> m_unison_detune_positions = {{1, {0}},
	                                                             {2, {0, 1}},
	                                                             {3, {0, 1, 2}},
	                                                             //shuffling starts here:
	                                                             {4, {2, 0, 3, 1}},
	                                                             {6, {1, 4, 2, 0, 5, 3}},
	                                                             {12, {3, 9, 6, 10, 7, 4, 0, 11, 5, 1, 8, 2}}};
	std::map<int, float> m_unison_gain_factors                = {
        {1, 1.f}, {2, 0.86f}, {3, 0.75f}, {4, 0.65f}, {6, 0.53f}, {12, 0.45f}};
#include "AudioVarDeclarations.h"
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OdinAudioProcessor)
};
