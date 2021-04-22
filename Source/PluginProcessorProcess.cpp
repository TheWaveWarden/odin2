/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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
#include "ScopedNoDenormals.h"
#include <math.h> //todo remove isnan

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
	//auto totalNumInputChannels  = getTotalNumInputChannels();
	//auto totalNumOutputChannels = getTotalNumOutputChannels();

	MidiMessage midi_message;
	int midi_message_sample          = -1;
	MidiBufferIterator midi_iterator = midiMessages.begin();
	bool midi_message_remaining      = !midiMessages.isEmpty();
	if (midi_message_remaining) {
		midi_message        = (*midi_iterator).getMessage();
		midi_message_sample = (*midi_iterator).samplePosition;
	}

	// loop over samples
	for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {

		// do Arpeggiator
		if (m_arpeggiator_on) {
			int step_active;
			auto note = m_arpeggiator.getNoteOns(step_active);
			m_step_led_active.set(step_active);
			if (std::get<0>(note) != -1) {
				midiNoteOn(std::get<0>(note), std::get<1>(note), std::get<2>(note), std::get<3>(note));
			}
			auto off_notes = m_arpeggiator.getNoteOffs();
			for (auto note_to_kill : off_notes) {
				midiNoteOff(note_to_kill);
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

		if (*m_x_mod) {
			m_x_modded = m_x_smooth + *m_x_mod;
			m_x_modded = m_x_modded > 1.f ? 1.f : m_x_modded;
			m_x_modded = m_x_modded < 0.f ? 0.f : m_x_modded;
		} else {
			m_x_modded = m_x_smooth;
		}
		if (*m_y_mod) {
			m_y_modded = m_y_smooth + *m_y_mod;
			m_y_modded = m_y_modded > 1.f ? 1.f : m_y_modded;
			m_y_modded = m_y_modded < 0.f ? 0.f : m_y_modded;
		} else {
			m_y_modded = m_y_smooth;
		}

		m_master_smooth = m_master_smooth * GAIN_SMOOTHIN_FACTOR + (1 - GAIN_SMOOTHIN_FACTOR) * (m_master_control);

		//============================================================
		//========================== MIDI ============================
		//============================================================
		while (midi_message_remaining && midi_message_sample <= sample) {
			handleMidiMessage(midi_message);
			// get next midi message
			midi_iterator++;
			midi_message_remaining = (midi_iterator != midiMessages.end());
			if (midi_message_remaining) {
				midi_message        = (*midi_iterator).getMessage();
				midi_message_sample = (*midi_iterator).samplePosition;
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

				//apply amp envelope
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
		} // stereo loop

		//==== FX SECTION ====

		// ugly solution, yet here we go:
		// check for each fx if its position is slot and then render it
		for (int fx_slot = 0; fx_slot < 5; ++fx_slot) {
			if (m_delay_position == fx_slot) {
				if (*m_delay_on) {
					stereo_signal[0] = m_delay.doDelayLeft(stereo_signal[0]);
					stereo_signal[1] = m_delay.doDelayRight(stereo_signal[1]);
				}
			} else if (m_phaser_position == fx_slot) {
				if (*m_phaser_on) {
					stereo_signal[0] = m_phaser.doPhaserLeft(stereo_signal[0]);
					stereo_signal[1] = m_phaser.doPhaserRight(stereo_signal[1]);
				}
			} else if (m_flanger_position == fx_slot) {
				if (*m_flanger_on) {
					stereo_signal[0] = m_flanger[0].doFlanger(stereo_signal[0]);
					stereo_signal[1] = m_flanger[1].doFlanger(stereo_signal[1]);
				}
			} else if (m_chorus_position == fx_slot) {
				if (*m_chorus_on) {
					stereo_signal[0] = m_chorus[0].doChorus(stereo_signal[0]);
					stereo_signal[1] = m_chorus[1].doChorus(stereo_signal[1]);
				}
			} else if (m_reverb_position == fx_slot) {
				if (*m_reverb_on) {
					stereo_signal[0] = m_reverb_fdn[0].doFDN(stereo_signal[0]);
					stereo_signal[1] = m_reverb_fdn[1].doFDN(stereo_signal[1]);
					//if (m_reverb_module_used == ReverbModule::Surge) {
					//	m_reverb_surge.process(stereo_signal[0], stereo_signal[1]);
					//} else {
					//	auto rev_out = m_reverb_zita.process(stereo_signal);
					//	stereo_signal[0] = rev_out[0];
					//	stereo_signal[1] = rev_out[1];
					//}
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

		auto *channelData   = buffer.getWritePointer(0);
		channelData[sample] = stereo_signal[0] * master_vol_modded;
		channelData         = buffer.getWritePointer(1);
		channelData[sample] = stereo_signal[1] * master_vol_modded;

	} // sample loop
}
