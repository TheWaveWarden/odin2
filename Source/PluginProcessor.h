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
#include "audio/Amplifier.h"
#include "audio/FX/Chorus.h"
#include "audio/FX/Delay.h"
#include "audio/FX/Flanger.h"
#include "audio/FX/OversamplingDistortion.h"
#include "audio/FX/Phaser.h"
#include "audio/FX/RingModulator.h"
#include "audio/Filters/CombFilter.h"
#include "audio/Filters/DiodeFilter.h"
#include "audio/Filters/FormantFilter.h"
#include "audio/Filters/Korg35Filter.h"
#include "audio/Filters/LadderFilter.h"
#include "audio/Filters/SEMFilter12.h"
#include "audio/Filters/SEMFilter24.h"
#include "audio/Oscillators/WavetableContainer.h"

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

	// void startMidiLearn(Knob *p_knob) {
	// 	DBG("MIDI LEARN WAS SIGNALED");
	// 	if (m_midi_learn_slider) {
	// 		m_midi_learn_slider->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_knob) {
	// 		m_midi_learn_knob->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_lrbutton) {
	// 		m_midi_learn_lrbutton->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_odinbutton) {
	// 		m_midi_learn_odinbutton->stopMidiLearn();
	// 	}
	// 	m_midi_learn_knob              = p_knob;
	// 	m_midi_learn_lrbutton_active   = false;
	// 	m_midi_learn_slider_active     = false;
	// 	m_midi_learn_odinbutton_active = false;
	// 	m_midi_learn_knob_active       = true;
	// }

	void startMidiLearn(const String &p_parameter_ID, OdinMidiLearnBase *p_GUI_control) {
		DBG("MIDI LEARN FOR PARAMETER " + p_parameter_ID + " WAS SIGNALED!");
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
				return;
			}
		}

#ifdef ODIN_DEBUG
		int counter = 1;
		DBG("=========");
		for (auto const &control : m_midi_control_param_map) {
			DBG(std::to_string(counter) + ": " + control.second->getName(100).toStdString());
		}
		DBG("=========");
#endif
	}

	// void midiForget(Knob *p_knob) {
	// 	for (std::multimap<int, Knob *>::iterator iter = m_midi_control_list_knob.begin();
	// 	     iter != m_midi_control_list_knob.end();) {
	// 		std::multimap<int, Knob *>::iterator erase_iter = iter++;
	// 		if (erase_iter->second == p_knob) {
	// 			m_midi_control_list_knob.erase(erase_iter);
	// 			return;
	// 		}
	// 	}
	// }

	// void startMidiLearn(DrawableSlider *p_slider) {
	// 	DBG("MIDI LEARN WAS SIGNALED");
	// 	if (m_midi_learn_slider) {
	// 		m_midi_learn_slider->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_knob) {
	// 		m_midi_learn_knob->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_lrbutton) {
	// 		m_midi_learn_lrbutton->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_odinbutton) {
	// 		m_midi_learn_odinbutton->stopMidiLearn();
	// 	}
	// 	m_midi_learn_slider            = p_slider;
	// 	m_midi_learn_knob_active       = false;
	// 	m_midi_learn_lrbutton_active   = false;
	// 	m_midi_learn_odinbutton_active = false;
	// 	m_midi_learn_slider_active     = true;
	// }

	// void midiForget(DrawableSlider *p_slider) {
	// 	for (std::multimap<int, DrawableSlider *>::iterator iter = m_midi_control_list_slider.begin();
	// 	     iter != m_midi_control_list_slider.end();) {
	// 		std::multimap<int, DrawableSlider *>::iterator erase_iter = iter++;
	// 		if (erase_iter->second == p_slider) {
	// 			m_midi_control_list_slider.erase(erase_iter);
	// 			return;
	// 		}
	// 	}
	// }

	// void startMidiLearn(LeftRightButton *p_button) {
	// 	DBG("MIDI LEARN WAS SIGNALED");
	// 	if (m_midi_learn_slider) {
	// 		m_midi_learn_slider->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_knob) {
	// 		m_midi_learn_knob->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_lrbutton) {
	// 		m_midi_learn_lrbutton->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_odinbutton) {
	// 		m_midi_learn_odinbutton->stopMidiLearn();
	// 	}
	// 	m_midi_learn_lrbutton          = p_button;
	// 	m_midi_learn_knob_active       = false;
	// 	m_midi_learn_slider_active     = false;
	// 	m_midi_learn_odinbutton_active = false;
	// 	m_midi_learn_lrbutton_active   = true;
	// }

	// void midiForget(LeftRightButton *p_button) {
	// 	for (std::multimap<int, LeftRightButton *>::iterator iter = m_midi_control_list_lrbutton.begin();
	// 	     iter != m_midi_control_list_lrbutton.end();) {
	// 		std::multimap<int, LeftRightButton *>::iterator erase_iter = iter++;
	// 		if (erase_iter->second == p_button) {
	// 			m_midi_control_list_lrbutton.erase(erase_iter);
	// 			return;
	// 		}
	// 	}
	// }

	// void startMidiLearn(OdinButton *p_button) {
	// 	DBG("MIDI LEARN WAS SIGNALED");
	// 	if (m_midi_learn_slider) {
	// 		m_midi_learn_slider->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_knob) {
	// 		m_midi_learn_knob->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_lrbutton) {
	// 		m_midi_learn_lrbutton->stopMidiLearn();
	// 	}
	// 	if (m_midi_learn_odinbutton) {
	// 		m_midi_learn_odinbutton->stopMidiLearn();
	// 	}
	// 	m_midi_learn_odinbutton        = p_button;
	// 	m_midi_learn_knob_active       = false;
	// 	m_midi_learn_slider_active     = false;
	// 	m_midi_learn_lrbutton_active   = false;
	// 	m_midi_learn_odinbutton_active = true;
	// }

	// void midiForget(OdinButton *p_button) {
	// 	for (std::multimap<int, OdinButton *>::iterator iter = m_midi_control_list_odinbutton.begin();
	// 	     iter != m_midi_control_list_odinbutton.end();) {
	// 		std::multimap<int, OdinButton *>::iterator erase_iter = iter++;
	// 		if (erase_iter->second == p_button) {
	// 			m_midi_control_list_odinbutton.erase(erase_iter);
	// 			return;
	// 		}
	// 	}
	// }

	void stopMidiLearn() {
		// m_midi_learn_knob_active       = false;
		// m_midi_learn_knob              = nullptr;
		// m_midi_learn_slider_active     = false;
		// m_midi_learn_slider            = nullptr;
		// m_midi_learn_lrbutton_active   = false;
		// m_midi_learn_lrbutton          = nullptr;
		// m_midi_learn_odinbutton_active = false;
		// m_midi_learn_odinbutton        = nullptr;
		m_midi_learn_parameter_active = false;
		m_midi_learn_parameter_ID = "";
		m_midi_learn_control = nullptr;
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
		m_voice[0].setPolyLegato(p_is_poly);

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
	void retriggerAllListeners();

private:
	// OdinAudioProcessorEditor* m_editor = nullptr;
	void setFilter3EnvValue();

	void setBPM(float BPM);

	bool m_midi_learn_knob_active = false;
	Knob *m_midi_learn_knob       = nullptr;
	std::multimap<int, Knob *> m_midi_control_list_knob;

	bool m_midi_learn_slider_active     = false;
	DrawableSlider *m_midi_learn_slider = nullptr;
	std::multimap<int, DrawableSlider *> m_midi_control_list_slider;

	bool m_midi_learn_lrbutton_active      = false;
	LeftRightButton *m_midi_learn_lrbutton = nullptr;
	std::multimap<int, LeftRightButton *> m_midi_control_list_lrbutton;

	bool m_midi_learn_odinbutton_active = false;
	OdinButton *m_midi_learn_odinbutton = nullptr;
	std::multimap<int, OdinButton *> m_midi_control_list_odinbutton;

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
	OdinTreeListener m_tree_listener_amount1;
	OdinTreeListener m_tree_listener_amount2;
	OdinTreeListener m_tree_listener_amount3;
	OdinTreeListener m_tree_listener_general_misc;

	ValueTree m_value_tree_draw;
	ValueTree m_value_tree_fx;
	ValueTree m_value_tree_lfo;
	ValueTree m_value_tree_misc;
	ValueTree m_value_tree_mod;
	ValueTree m_value_tree_osc;

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
	void treeValueChangedAmount1(const String &p_ID, float p_new_value);
	void treeValueChangedAmount2(const String &p_ID, float p_new_value);
	void treeValueChangedAmount3(const String &p_ID, float p_new_value);
	void treeValueChangedGeneralMisc(const String &p_ID, float p_new_value);

	Voice m_voice[VOICES];
	Amplifier m_amp;
	OversamplingDistortion m_distortion[2];

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

	void setPitchWheelValue(int p_value);
	void setModWheelValue(int p_value);
	void checkEndGlobalEnvelope();

	void setModulationPointers();

	void addNonAudioParametersToTree();

	bool m_render_LFO[4]  = {0};
	bool m_render_ADSR[2] = {0}; // 0 = mod, 1 = global

	// MOD SOURCES
	float m_adsr[VOICES][3]          = {0.f};
	float m_lfo[VOICES][3]           = {0.f};
	float m_global_env_mod_source    = 0;
	float m_global_lfo_mod_source    = 0;
	float m_filter_output[VOICES][2] = {0.f};
	float m_osc_output[VOICES][3]    = {0.f};
	float m_MIDI_aftertouch          = 0.f;
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
