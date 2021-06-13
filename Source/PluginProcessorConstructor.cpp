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

//this file is included from PluginProcessor.cpp to split the class implementation

#include "PluginProcessor.h"

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
	m_tree_listener_reverb.onValueChange = [&](const String &p_ID, float p_new_value) {
		treeValueChangedReverb(p_ID, p_new_value);
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
		m_voice[i].setTuningPtr(&m_tuning);

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
}