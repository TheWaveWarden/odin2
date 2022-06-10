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

#include "include/Tunings.h"
#include "../GlobalIncludes.h"
#include "Amplifier.h"
#include "FX/OversamplingDistortion.h"
#include "FX/RingModulator.h"
#include "Filters/CombFilter.h"
#include "Filters/DiodeFilter.h"
#include "Filters/FormantFilter.h"
#include "Filters/Korg35Filter.h"
#include "Filters/LadderFilter.h"
#include "Filters/SEMFilter12.h"
#include "Oscillators/AnalogOscillator.h"
#include "Oscillators/ChiptuneOscillator.h"
#include "Oscillators/FMOscillator.h"
#include "Oscillators/LFO.h"
#include "Oscillators/MultiOscillator.h"
#include "Oscillators/NoiseOscillator.h"
#include "Oscillators/PMOscillator.h"
#include "Oscillators/VectorOscillator.h"
#include "Oscillators/WavetableOsc2D.h"
#include "ADSR.h"

#include <cstdlib>

// one voice of the polyphonic voices, i.e. everything up to the amplifier
struct Voice {

	// implicit conversion to bool
	operator bool() const {
		return m_voice_active;
	}

	Voice() {
		std::srand((unsigned int)std::time(nullptr));
		generateNewRandomValue();

		for (int osc = 0; osc < 3; ++osc) {
			chiptune_osc[osc].selectWavetableByMapping = [&, osc](int p_input) {
				chiptune_osc[osc].selectWavetable(chiptune_osc[osc].wavetableMappingChiptune(p_input));
			};
			fm_osc[osc].m_carrier_osc.selectWavetableByMapping = [&, osc](int p_input) {
				fm_osc[osc].m_carrier_osc.selectWavetable(fm_osc[osc].m_carrier_osc.wavetableMappingFM(p_input));
			};
			fm_osc[osc].m_modulator_osc.selectWavetableByMapping = [&, osc](int p_input) {
				fm_osc[osc].m_modulator_osc.selectWavetable(fm_osc[osc].m_modulator_osc.wavetableMappingFM(p_input));
			};

			pm_osc[osc].m_carrier_osc.selectWavetableByMapping = [&, osc](int p_input) {
				pm_osc[osc].m_carrier_osc.selectWavetable(pm_osc[osc].m_carrier_osc.wavetableMappingFM(p_input));
			};
			pm_osc[osc].m_modulator_osc.selectWavetableByMapping = [&, osc](int p_input) {
				pm_osc[osc].m_modulator_osc.selectWavetable(pm_osc[osc].m_modulator_osc.wavetableMappingFM(p_input));
			};

			analog_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			wavedraw_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			chipdraw_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			specdraw_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			wavetable_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			multi_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			vector_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			chiptune_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			fm_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
			pm_osc[osc].setUnisonDetuneFactorPointer(&unison_detune_factor);
		}

		amp.setUnisonPanPositionPointer(&unison_pan_position);
		amp.setUnisonGainReductionPointer(&unison_gain_reduction);
	}

	void setAftertouch(int p_note_number, float p_value) {
		if (p_note_number == m_MIDI_key) {
			MIDI_aftertouch_mod_source = p_value;
		}
	}

	void setOscSyncEnabled(bool p_enabled, int p_osc) {
		analog_osc[p_osc].setSyncEnabled(p_enabled);
		wavetable_osc[p_osc].setSyncEnabled(p_enabled);
		wavedraw_osc[p_osc].setSyncEnabled(p_enabled);
		specdraw_osc[p_osc].setSyncEnabled(p_enabled);
		chipdraw_osc[p_osc].setSyncEnabled(p_enabled);
		chiptune_osc[p_osc].setSyncEnabled(p_enabled);
		vector_osc[p_osc].setSyncEnabled(p_enabled);
	}

	void setOscSyncOscillator(Oscillator *p_osc) {
		analog_osc[1].setSyncOscillator(p_osc);
		wavetable_osc[1].setSyncOscillator(p_osc);
		wavedraw_osc[1].setSyncOscillator(p_osc);
		specdraw_osc[1].setSyncOscillator(p_osc);
		chipdraw_osc[1].setSyncOscillator(p_osc);
		chiptune_osc[1].setSyncOscillator(p_osc);
		vector_osc[1].setSyncOscillator(p_osc);

		analog_osc[2].setSyncOscillator(p_osc);
		wavetable_osc[2].setSyncOscillator(p_osc);
		wavedraw_osc[2].setSyncOscillator(p_osc);
		specdraw_osc[2].setSyncOscillator(p_osc);
		chipdraw_osc[2].setSyncOscillator(p_osc);
		chiptune_osc[2].setSyncOscillator(p_osc);
		vector_osc[2].setSyncOscillator(p_osc);
	}

	void setMonoPolyLegato(PlayModes p_mode) {
		m_mono_poly_legato = p_mode;
	}

	void generateNewRandomValue() {
		random_modulation = 2.f * ((float)rand() / (float)RAND_MAX) - 1.f;
	}

	float MIDINoteToFreq(int p_MIDI_note) {
		return m_tuning_ptr->frequencyForMidiNote(p_MIDI_note);
		//return 27.5f * pow(2.f, (float)(p_MIDI_note - 21) / 12.f);
	}

	void setBPM(float p_BPM, bool p_LFO1_reset, bool p_LFO2_reset, bool p_LFO3_reset) {
		if (p_LFO1_reset) {
			lfo[0].setFreqBPM(p_BPM);
		}
		if (p_LFO2_reset) {
			lfo[1].setFreqBPM(p_BPM);
		}
		if (p_LFO3_reset) {
			lfo[2].setFreqBPM(p_BPM);
		}
	}

	void hardReset() {
		MIDI_aftertouch_mod_source = 0.f;
		for (int fil = 0; fil < 2; ++fil) {
			ladder_filter[fil].reset();
			diode_filter[fil].reset();
			formant_filter[fil].reset();
			korg_filter[fil].reset();
			SEM_filter_12[fil].reset();
			comb_filter[fil].reset();
		}
		for (int osc = 0; osc < 3; ++osc) {
			// use start voice, oscs will reset if reset is active
			analog_osc[osc].reset();
			wavetable_osc[osc].reset();
			wavedraw_osc[osc].reset();
			chipdraw_osc[osc].reset();
			specdraw_osc[osc].reset();
			multi_osc[osc].reset();
			vector_osc[osc].reset();
			chiptune_osc[osc].reset();
			fm_osc[osc].reset();
			pm_osc[osc].reset();
			wavedraw_osc[osc].reset();
			specdraw_osc[osc].reset();
			chipdraw_osc[osc].reset();
			lfo[osc].reset();
		}
		distortion[0].reset();
		distortion[1].reset();

		m_voice_active = false;
	}

	void setUnisonPanAmount(float p_amount) {
		amp.setUnisonPanAmount(p_amount);
	}

	void start(int p_MIDI_key,
	           int p_MIDI_velocity,
	           int p_last_MIDI_key,
	           float p_unison_pan,
	           float p_unison_detune,
	           float p_unison_gain_reduction,
	           bool p_unison_active, float p_arp_mod_1, float p_arp_mod_2) {
		reset(p_unison_active);
		setOscBaseFreq(MIDINoteToFreq(p_MIDI_key), MIDINoteToFreq(p_last_MIDI_key));
		setFilterMIDIValues(p_MIDI_key, p_MIDI_velocity);
		m_voice_active           = true;
		m_MIDI_key               = p_MIDI_key;
		MIDI_key_mod_source      = (float)p_MIDI_key / 127.f;
		MIDI_velocity_mod_source = (float)p_MIDI_velocity / 127.f;

		switch(m_mono_poly_legato){
			case PlayModes::Legato:
			for (int mod = 0; mod < 3; ++mod) {
				env[mod].restartEnvelopeLegato();
			}
			break;
			case PlayModes::Retrig:
			for (int mod = 0; mod < 3; ++mod) {
				env[mod].restartEnvelopeRetrig();
			}
			break;
			case PlayModes::Poly:
			for (int mod = 0; mod < 3; ++mod) {
				env[mod].reset();
			}
			break;
		}

		generateNewRandomValue();
		MIDI_aftertouch_mod_source = 0.f;
		unison_pan_position        = p_unison_pan;
		unison_detune_position     = p_unison_detune;
		unison_gain_reduction      = p_unison_gain_reduction;
		//DBG("unison gain:" + std::to_string(p_unison_gain_reduction));
		m_is_in_release            = false;
		m_arp_mod_1 = p_arp_mod_1;
		m_arp_mod_2 = p_arp_mod_2;
		//DBG(m_arp_mod_1);
		//DBG(m_arp_mod_2);
		calcUnisonDetuneFactor();
		//DBG("Started voice");
	}

	// starts release on envelopes if this is the key that was pressed
	// returns true if the voice was actually stopped
	bool keyUp(int p_MIDI_key) {
		if (m_MIDI_key == p_MIDI_key) {
			//DBG("Stopping envelopes on key " + std::to_string(m_MIDI_key));
			startRelease();
			return true;
		}
		return false;
	}

	void forceKeyUp(){
		startRelease();
	}

	void startRelease() {
		//DBG("Stopping envelopes on key " + std::to_string(m_MIDI_key) +
		//    " after sustian was released");
		env[0].startRelease();
		env[1].startRelease();
		env[2].startRelease();
		m_is_in_release = true;
	}

	bool isInRelease() {
		return m_is_in_release;
	}

	//this is needed so the voicemanager doesn't recognize this voice as 
	//"in release" when it was added to the unison queue as "free" before the "in release" check
	void setReleaseInactiveBeforeStart(){
		m_is_in_release = false;
	}

	bool usesThisMIDIKey(int p_MIDI_key) {
		if (m_MIDI_key == p_MIDI_key) {
			return true;
		}
		return false;
	}

	void setOctave(int p_octave, int p_osc) {
		float pitch_multiplier = Oscillator::pitchShiftMultiplier(p_octave * 12);

		analog_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		wavedraw_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		chipdraw_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		specdraw_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		wavetable_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		multi_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		vector_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		chiptune_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		fm_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
		pm_osc[p_osc].setOctaveMultiplier(pitch_multiplier);
	}

	void setSemitones(int p_semi, int p_osc) {
		float pitch_multiplier = Oscillator::pitchShiftMultiplier(p_semi);

		analog_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		wavedraw_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		chipdraw_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		specdraw_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		wavetable_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		multi_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		vector_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		chiptune_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		fm_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
		pm_osc[p_osc].setSemitoneMultiplier(pitch_multiplier);
	}

	void setFinetune(float p_fine, int p_osc) {
		float pitch_multiplier = Oscillator::pitchShiftMultiplier(p_fine * 0.01f);

		analog_osc[p_osc].setCentMultiplier(pitch_multiplier);
		wavedraw_osc[p_osc].setCentMultiplier(pitch_multiplier);
		chipdraw_osc[p_osc].setCentMultiplier(pitch_multiplier);
		specdraw_osc[p_osc].setCentMultiplier(pitch_multiplier);
		wavetable_osc[p_osc].setCentMultiplier(pitch_multiplier);
		multi_osc[p_osc].setCentMultiplier(pitch_multiplier);
		vector_osc[p_osc].setCentMultiplier(pitch_multiplier);
		chiptune_osc[p_osc].setCentMultiplier(pitch_multiplier);
		fm_osc[p_osc].setCentMultiplier(pitch_multiplier);
		pm_osc[p_osc].setCentMultiplier(pitch_multiplier);
	}

	void setGlide(float p_glide) {
		for (int osc = 0; osc < 3; ++osc) {
			analog_osc[osc].setGlide(p_glide);
			wavedraw_osc[osc].setGlide(p_glide);
			chipdraw_osc[osc].setGlide(p_glide);
			specdraw_osc[osc].setGlide(p_glide);
			wavetable_osc[osc].setGlide(p_glide);
			multi_osc[osc].setGlide(p_glide);
			vector_osc[osc].setGlide(p_glide);
			chiptune_osc[osc].setGlide(p_glide);
			fm_osc[osc].setGlide(p_glide);
			pm_osc[osc].setGlide(p_glide);
		}
	}

	void killGlide(int p_osc) {
		analog_osc[p_osc].killGlide();
		wavedraw_osc[p_osc].killGlide();
		chipdraw_osc[p_osc].killGlide();
		specdraw_osc[p_osc].killGlide();
		wavetable_osc[p_osc].killGlide();
		multi_osc[p_osc].killGlide();
		vector_osc[p_osc].killGlide();
		chiptune_osc[p_osc].killGlide();
		fm_osc[p_osc].killGlide();
		pm_osc[p_osc].killGlide();
	}

	void setReset(bool p_reset, int p_osc) {
		analog_osc[p_osc].setResetActive(p_reset);
		wavedraw_osc[p_osc].setResetActive(p_reset);
		chipdraw_osc[p_osc].setResetActive(p_reset);
		specdraw_osc[p_osc].setResetActive(p_reset);
		wavetable_osc[p_osc].setResetActive(p_reset);
		multi_osc[p_osc].setResetActive(p_reset);
		vector_osc[p_osc].setResetActive(p_reset);
		chiptune_osc[p_osc].setResetActive(p_reset);
		fm_osc[p_osc].setResetActive(p_reset);
		pm_osc[p_osc].setResetActive(p_reset);
	}

	void setOscBaseFreq(float p_freq, float p_last_freq) {
		for (int osc = 0; osc < 3; ++osc) {
			analog_osc[osc].setBaseFrequency(p_last_freq);
			analog_osc[osc].setGlideTargetFrequency(p_freq);
			wavedraw_osc[osc].setBaseFrequency(p_last_freq);
			wavedraw_osc[osc].setGlideTargetFrequency(p_freq);
			chipdraw_osc[osc].setBaseFrequency(p_last_freq);
			chipdraw_osc[osc].setGlideTargetFrequency(p_freq);
			specdraw_osc[osc].setBaseFrequency(p_last_freq);
			specdraw_osc[osc].setGlideTargetFrequency(p_freq);
			wavetable_osc[osc].setBaseFrequency(p_last_freq);
			wavetable_osc[osc].setGlideTargetFrequency(p_freq);
			multi_osc[osc].setBaseFrequency(p_last_freq);
			multi_osc[osc].setGlideTargetFrequency(p_freq);
			vector_osc[osc].setBaseFrequency(p_last_freq);
			vector_osc[osc].setGlideTargetFrequency(p_freq);
			chiptune_osc[osc].setBaseFrequency(p_last_freq);
			chiptune_osc[osc].setGlideTargetFrequency(p_freq);
			fm_osc[osc].setBaseFrequency(p_last_freq);
			pm_osc[osc].setBaseFrequency(p_last_freq);
			fm_osc[osc].setGlideTargetFrequency(p_freq);
			pm_osc[osc].setGlideTargetFrequency(p_freq);
		}
	}

	void reset(bool p_unison_active) {
		resetLegato(p_unison_active);
		if (m_mono_poly_legato == PlayModes::Poly) {
			for (int fil = 0; fil < 2; ++fil) {
				ladder_filter[fil].reset();
				diode_filter[fil].reset();
				formant_filter[fil].reset();
				korg_filter[fil].reset();
				SEM_filter_12[fil].reset();
				comb_filter[fil].reset();
			}
		}
	}

	void resetLegato(bool unison_active) {

		//we reset the phase in legato and the voice is not carried over from an old note
		bool envelope_off = env[0].isEnvelopeOff();// i.e. voice isn't playing anymore
		bool reset_phase = unison_active && envelope_off;

		for (int osc = 0; osc < 3; ++osc) {
			// use start voice, oscs will reset if reset is active
			analog_osc[osc].voiceStart(reset_phase);
			wavetable_osc[osc].voiceStart(reset_phase);
			wavedraw_osc[osc].voiceStart(reset_phase);
			chipdraw_osc[osc].voiceStart(reset_phase);
			specdraw_osc[osc].voiceStart(reset_phase);
			//special case:
			multi_osc[osc].voiceStart(envelope_off);
			vector_osc[osc].voiceStart(reset_phase);
			chiptune_osc[osc].voiceStart(reset_phase);
			fm_osc[osc].voiceStart(reset_phase);
			pm_osc[osc].voiceStart(reset_phase);
			wavedraw_osc[osc].voiceStart(reset_phase);
			specdraw_osc[osc].voiceStart(reset_phase);
			chipdraw_osc[osc].voiceStart(reset_phase);
			lfo[osc].voiceStart(false);
		}
	}

	void setFilterMIDIValues(int p_MIDI_note, int p_MIDI_vel) {
		// shift note here so the lowest note possible is 21
		p_MIDI_note -= 21;
		p_MIDI_note = p_MIDI_note < 0 ? 0 : p_MIDI_note;

		for (int fil = 0; fil < 2; ++fil) {
			ladder_filter[fil].m_MIDI_note = p_MIDI_note;
			diode_filter[fil].m_MIDI_note  = p_MIDI_note;
			korg_filter[fil].m_MIDI_note   = p_MIDI_note;
			SEM_filter_12[fil].m_MIDI_note = p_MIDI_note;
			comb_filter[fil].m_MIDI_note   = p_MIDI_note;
			ring_mod[fil].m_MIDI_note      = p_MIDI_note;

			ladder_filter[fil].m_MIDI_velocity = p_MIDI_vel;
			diode_filter[fil].m_MIDI_velocity  = p_MIDI_vel;
			korg_filter[fil].m_MIDI_velocity   = p_MIDI_vel;
			SEM_filter_12[fil].m_MIDI_velocity = p_MIDI_vel;
			comb_filter[fil].m_MIDI_velocity   = p_MIDI_vel;
			ring_mod[fil].m_MIDI_velocity      = p_MIDI_vel;
		}
	}

	void setKbd(float p_kbd, int p_fil) {
		ladder_filter[p_fil].m_kbd_mod_amount = p_kbd;
		SEM_filter_12[p_fil].m_kbd_mod_amount = p_kbd;
		// SEM_filter_24[p_fil].m_kbd_mod_amount = p_kbd;
		korg_filter[p_fil].m_kbd_mod_amount  = p_kbd;
		diode_filter[p_fil].m_kbd_mod_amount = p_kbd;
		comb_filter[p_fil].m_kbd_mod_amount  = p_kbd;
		ring_mod[p_fil].m_kbd_mod_amount     = p_kbd;
	}

	void setVelModAmount(float p_vel, int p_fil) {
		ladder_filter[p_fil].m_vel_mod_amount = p_vel;
		SEM_filter_12[p_fil].m_vel_mod_amount = p_vel;
		// SEM_filter_24[p_fil].m_vel_mod_amount = p_vel;
		korg_filter[p_fil].m_vel_mod_amount    = p_vel;
		diode_filter[p_fil].m_vel_mod_amount   = p_vel;
		comb_filter[p_fil].m_vel_mod_amount    = p_vel;
		formant_filter[p_fil].m_vel_mod_amount = p_vel;
		ring_mod[p_fil].m_vel_mod_amount       = p_vel;
	}

	void setSaturation(float p_sat, int p_fil) {
		ladder_filter[p_fil].m_overdrive = p_sat;
		SEM_filter_12[p_fil].m_overdrive = p_sat;
		// SEM_filter_24[p_fil].m_overdrive = p_sat;
		korg_filter[p_fil].m_overdrive  = p_sat;
		diode_filter[p_fil].m_overdrive = p_sat;
	}

	void setEnvModAmount(float p_env, int p_fil) {
		// set as quadratic
		ladder_filter[p_fil].m_env_mod_amount = p_env;
		SEM_filter_12[p_fil].m_env_mod_amount = p_env;
		// SEM_filter_24[p_fil].m_env_mod_amount = p_env;
		korg_filter[p_fil].m_env_mod_amount    = p_env;
		diode_filter[p_fil].m_env_mod_amount   = p_env;
		comb_filter[p_fil].m_env_mod_amount    = p_env;
		formant_filter[p_fil].m_env_mod_amount = p_env;
		ring_mod[p_fil].m_env_mod_amount       = p_env;
	}

	void setFilterEnvValue(float p_env) {
		ladder_filter[0].m_env_value = p_env;
		SEM_filter_12[0].m_env_value = p_env;
		// SEM_filter_24[0].m_env_value = p_env;
		korg_filter[0].m_env_value    = p_env;
		diode_filter[0].m_env_value   = p_env;
		comb_filter[0].m_env_value    = p_env;
		formant_filter[0].m_env_value = p_env;
		ring_mod[0].m_env_value       = p_env;

		ladder_filter[1].m_env_value = p_env;
		SEM_filter_12[1].m_env_value = p_env;
		// SEM_filter_24[1].m_env_value = p_env;
		korg_filter[1].m_env_value    = p_env;
		diode_filter[1].m_env_value   = p_env;
		comb_filter[1].m_env_value    = p_env;
		formant_filter[1].m_env_value = p_env;
		ring_mod[1].m_env_value       = p_env;
	}

	void setFilterFreq(float p_freq, int p_fil) {
		// note: not used right now since control is smoothed
		ladder_filter[p_fil].m_freq_base = p_freq;
		SEM_filter_12[p_fil].m_freq_base = p_freq;
		// SEM_filter_24[p_fil].m_freq_base = p_freq;
		korg_filter[p_fil].m_freq_base  = p_freq;
		diode_filter[p_fil].m_freq_base = p_freq;
		comb_filter[p_fil].setCombFreq(p_freq);
	}

	void setFilterRes(float p_res, int p_fil) {
		ladder_filter[p_fil].setResControl(p_res);
		SEM_filter_12[p_fil].setResControl(p_res);
		korg_filter[p_fil].setResControl(p_res);
		diode_filter[p_fil].setResControl(p_res);
		comb_filter[p_fil].setResonance(p_res);
	}

	void setSampleRate(float p_samplerate) {

		//DBG("setsamplerate VOICE\n\n");
		env[0].setSampleRate(p_samplerate);
		env[1].setSampleRate(p_samplerate);
		env[2].setSampleRate(p_samplerate);

		amp.setSampleRate(p_samplerate);

		for (int fil = 0; fil < 2; ++fil) {
			ladder_filter[fil].setSampleRate(p_samplerate);
			SEM_filter_12[fil].setSampleRate(p_samplerate);
			korg_filter[fil].setSampleRate(p_samplerate);
			diode_filter[fil].setSampleRate(p_samplerate);
			comb_filter[fil].setSampleRate(p_samplerate);
			ring_mod[fil].setSampleRate(p_samplerate);
			formant_filter[fil].setSampleRate(p_samplerate);
		}

		for (int osc = 0; osc < 3; ++osc) {
			analog_osc[osc].setSampleRate(p_samplerate);
			wavetable_osc[osc].setSampleRate(p_samplerate);
			wavedraw_osc[osc].setSampleRate(p_samplerate);
			chipdraw_osc[osc].setSampleRate(p_samplerate);
			specdraw_osc[osc].setSampleRate(p_samplerate);
			multi_osc[osc].setSampleRate(p_samplerate);
			vector_osc[osc].setSampleRate(p_samplerate);
			chiptune_osc[osc].setSampleRate(p_samplerate);
			fm_osc[osc].setSampleRate(p_samplerate);
			pm_osc[osc].setSampleRate(p_samplerate);
			noise_osc[osc].setSampleRate(p_samplerate);
			wavedraw_osc[osc].setSampleRate(p_samplerate);
			specdraw_osc[osc].setSampleRate(p_samplerate);
			chipdraw_osc[osc].setSampleRate(p_samplerate);
			lfo[osc].setSampleRate(p_samplerate);
		}
	}

	void setUnisonDetuneAmount(float p_amount) {
		unison_detune_amount = p_amount;
		//only recalc if this voice is active right now
		if (m_voice_active) {
			calcUnisonDetuneFactor();
		}
	}

	// this needs to be calculated when unison_detune_position has changed (voice start)
	// or when the uniston detune amount has changed and the voice is active
	void calcUnisonDetuneFactor() {
		unison_detune_factor = pow(2.f, unison_detune_position * unison_detune_amount / 12.f);
	}

	void setTuningPtr(Tunings::Tuning* p_tuning_ptr){
		m_tuning_ptr = p_tuning_ptr;
	}

	// oscs
	AnalogOscillator analog_osc[3];
	WavetableOsc2D wavetable_osc[3];
	MultiOscillator multi_osc[3];
	VectorOscillator vector_osc[3];
	ChiptuneOscillator chiptune_osc[3];
	FMOscillator fm_osc[3];
	PMOscillator pm_osc[3];
	NoiseOscillator noise_osc[3];
	WavetableOsc1D wavedraw_osc[3];
	WavetableOsc1D chipdraw_osc[3];
	WavetableOsc1D specdraw_osc[3];

	// filter
	LadderFilter ladder_filter[2];
	SEMFilter12 SEM_filter_12[2];
	Korg35Filter korg_filter[2];
	DiodeFilter diode_filter[2];
	FormantFilter formant_filter[2];
	CombFilter comb_filter[2];
	RingModulator ring_mod[2];
	// ADSRs
	ADSREnvelope env[3];
	LFO lfo[3];

	//Amp & Distortion
	Amplifier amp;
	OversamplingDistortion distortion[2];

	//the following two are set static on voice-start and are used to calculate actual pan and detune dynamically
	//see OdinAudioProcessor::m_unison_pan_positions and m_unison_detune_positions for distributions
	float unison_pan_position    = 0; // [-1,1]
	float unison_detune_position = 0; //[-1,1]

	Tunings::Tuning* m_tuning_ptr;
	float unison_detune_factor = 1.f; //calculated from unison_detune_position

	float unison_detune_amount  = 0.08f;
	float unison_gain_reduction = 1.f;
	float m_arp_mod_1 = 0.f;
	float m_arp_mod_2 = 0.f;

	PlayModes m_mono_poly_legato = PlayModes::Poly;
	// modulation values
	float MIDI_key_mod_source        = 0.f;
	float MIDI_velocity_mod_source   = 0.f;
	float MIDI_aftertouch_mod_source = 0.f;
	float random_modulation;

	// called when the envelope ends to signal voice end to voice manager
	// std::function<void()> onEnvelopeEnd = []() {};
	bool m_is_in_release;
	bool m_voice_active = false;
	int m_MIDI_key      = 0;
};


class VoiceManager {
public:
	VoiceManager() {

		//fill voice history in ascending order
		for(int voice = 0; voice < VOICES; ++voice) {
			m_voice_history.push_back(voice);
		}
	}

	//voice return priority:
	// 0. legato (fixed)
	// 1. same key in sustain
	// 2. free voices
	// 3. oldest voices in release
	// 4. oldest voices
	std::vector<int> getVoices(int p_note, int p_unison) {

		std::vector<int> ret;

		// in Legato and Mono mode every unison voice is the same
		if (m_mono_poly_legato != PlayModes::Poly) {
			for (int voice = 0; voice < p_unison; ++voice) {
				ret.push_back(voice);
			}
			return ret;
		}

		// return note if it is in sustain (processor kills it)
		if (m_sustain_active) {
			for (int voice = 0; voice < VOICES; ++voice) {
				//check if THIS note is on kill list
				if (m_kill_list[voice] && m_kill_list_note[voice] == p_note) {
					// note is on sustain... remove it from kill list, reset voice and use that one
					m_kill_list[voice] = false;
					DBG("Note " + std::to_string(p_note) + " stole voice " + std::to_string(voice) +
					    " out of sustain!");
					ret.push_back(voice);
					if (ret.size() >= p_unison) {
						return ret;
					}
				}
			}
		}

		// look for free voices
		for (int i = 0; i < VOICES; ++i) {
			if (!voice_busy[i]) {
				voice_busy[i] = true;
				//so it doesn't get detected as "in release" in the next block:
				m_actual_voice_pointers[i]->setReleaseInactiveBeforeStart();
				removeFromKillList(i);
				updateVoiceHistory(i);
				ret.push_back(i);
				if (ret.size() >= p_unison) {
					return ret;
				}
			}
		}

		// look for voices in release
		for (int i = VOICES - 1; i >= 0; --i) {
			if (m_actual_voice_pointers[m_voice_history[i]]->isInRelease()){
				removeFromKillList(m_voice_history[i]);
				ret.push_back(m_voice_history[i]);
				updateVoiceHistory(m_voice_history[i]);
				DBG("Stole voice " + std::to_string(m_voice_history[i]) + " out of release");
				if (ret.size() >= p_unison) {
					return ret;
				}
				// shift, since voice history was updated
				++i;
			}
		}

		// we still need voices, so we steal them
		int voices_to_steal = p_unison - ret.size();
		for (int steal = 0; steal < voices_to_steal; ++steal) {
			//we update history at the end of loop, so [VOICES - 1] is different every time
			DBG("Voice manager STOLE voice " + std::to_string(m_voice_history[VOICES - 1]));
			removeFromKillList(m_voice_history[VOICES - 1]);
			ret.push_back(m_voice_history[VOICES - 1]);
			updateVoiceHistory(m_voice_history[VOICES - 1]);
		}
		return ret;
	}

	int getNewestVoiceIndex() {
		return m_voice_history[0];
	}

	// marks a voice as free again
	void freeVoice(int p_voice) {
		voice_busy[p_voice] = false;
		DBG("Voice manager freed voice " + std::to_string(p_voice));
	}

	void setSustainActive(bool p_active) {
		m_sustain_active       = p_active;
		m_sustain_active_float = p_active ? 1.f : 0.f;
	}

	bool getSustainActive() {
		return m_sustain_active;
	}

	// adds to killlist to be killed after sustain pedal gets lifted
	void addToKillList(int p_voice, int p_note) {
		m_kill_list_note[p_voice] = p_note;
		m_kill_list[p_voice]      = true;
	}

	void removeFromKillList(int p_voice) {
		m_kill_list[p_voice] = false;
	}

	bool isOnKillList(int p_voice) {
		return m_kill_list[p_voice];
	}

	void clearKillList() {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_kill_list[voice] = false;
		}
	}

	void updateVoiceHistory(int p_next_voice) {
		int index = 0;
		while (p_next_voice != m_voice_history[index]) {
			++index;
		}

		// p_next_voice was at position index, move all above one down:
		for (int voice = index; voice > 0; --voice) {
			m_voice_history[voice] = m_voice_history[voice - 1];
		}

		// now set new as newest value:
		m_voice_history[0] = p_next_voice;

		// DBG(std::to_string(m_voice_history[0]) + " " +
		//     std::to_string(m_voice_history[1]) + " " +
		//     std::to_string(m_voice_history[2]) + " " +
		//     std::to_string(m_voice_history[3]) + " " +
		//     std::to_string(m_voice_history[4]) + " " +
		//     std::to_string(m_voice_history[5]) + " " +
		//     std::to_string(m_voice_history[6]) + " " +
		//     std::to_string(m_voice_history[7]) + " " +
		//     std::to_string(m_voice_history[8]) + " " +
		//     std::to_string(m_voice_history[9]) + " " +
		//     std::to_string(m_voice_history[10]) + " " +
		//     std::to_string(m_voice_history[11]));
	}

	bool setMonoPolyLegato(PlayModes p_mode) {
		if (m_mono_poly_legato != p_mode) {
			m_mono_poly_legato = p_mode;
			return true; // value was changed
		}
		return false;
	}

	void reset() {
		for (int voice = 0; voice < VOICES; ++voice) {
			m_voice_history[voice] = voice;
			voice_busy[voice]      = false;
			m_kill_list[voice]     = false;
		}
		m_sustain_active       = false;
		m_sustain_active_float = 0.f;
	}

	bool voice_busy[VOICES] = {0}; // is voice busy

	Voice *m_actual_voice_pointers[VOICES];

	float m_sustain_active_float = 0; //for modulation, "copy" of the bool
protected:

	bool m_sustain_active = false;

	PlayModes m_mono_poly_legato = PlayModes::Poly;
	// used to determine oldest voice for stealing
	std::vector<int> m_voice_history;

	bool m_kill_list[VOICES] = {0};
	int m_kill_list_note[VOICES];
};