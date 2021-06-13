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

#include "ModMatrix.h"

void ModMatrixRow::setSourcesAndDestinations(ModSources *p_source, ModDestinations *p_destination) {
	m_sources      = p_source;
	m_destinations = p_destination;
}

void ModMatrixRow::applyModulation() {

	if (m_active_1) {
		// poly destination -> #VOICES modulations
		if (m_destination_1_poly) {
			for (int voice = 0; voice < VOICES; ++voice) {
				if (m_scale) {
					if (m_scale_amount >= 0) {
						*(m_destination_1_value[voice]) += (*m_source_value[voice]) * m_mod_amount_1 *
						                                   fabs(m_mod_amount_1) *
						                                   (1 + (*m_scale_value[voice] - 1) * m_scale_amount);
					} else {
						*(m_destination_1_value[voice]) += (*m_source_value[voice]) * m_mod_amount_1 *
						                                   fabs(m_mod_amount_1) *
						                                   (1 + fabs(*m_scale_value[voice]) * m_scale_amount);
					}
				} else {
					// DBG("MOD SLOT ACTIVE");
					*(m_destination_1_value[voice]) += (*m_source_value[voice]) * m_mod_amount_1 * fabs(m_mod_amount_1);
				}
			}
		} else // mono destination
		{
			// use most recent voice for poly, for mono all #VOICES are identical so it
			// doesnt matter
			if (m_scale) {
				if (m_scale_amount >= 0) {
					*(m_destination_1_value[0]) += (*m_source_value[m_most_recent_voice]) * m_mod_amount_1 *
					                               fabs(m_mod_amount_1) *
					                               (1 + (*m_scale_value[m_most_recent_voice] - 1) * m_scale_amount);
				} else {
					*(m_destination_1_value[0]) += (*m_source_value[m_most_recent_voice]) * m_mod_amount_1 *
					                               fabs(m_mod_amount_1) *
					                               (1 + fabs(*m_scale_value[m_most_recent_voice]) * m_scale_amount);
				}
			} else {
				//DBG("dest before: " + std::to_string(*(m_destination_1_value[0])));
				//DBG("mod amount: " + std::to_string(m_mod_amount_1));
				//DBG("rec voice: " + std::to_string(m_most_recent_voice));
				//DBG("source: " + std::to_string((*m_source_value[m_most_recent_voice])));

				*(m_destination_1_value[0]) +=
				    (*m_source_value[m_most_recent_voice]) * m_mod_amount_1 * fabs(m_mod_amount_1);
				//DBG("dest after: " + std::to_string(*(m_destination_1_value[0])));
			}
		}
	}

	if (m_active_2) {
		// poly destination -> #VOICES modulations
		if (m_destination_2_poly) {
			for (int voice = 0; voice < VOICES; ++voice) {
				if (m_scale) {
					if (m_scale_amount >= 0) {
						*(m_destination_2_value[voice]) += (*m_source_value[voice]) * m_mod_amount_2 *
						                                   fabs(m_mod_amount_2) *
						                                   (1 + (*m_scale_value[voice] - 1) * m_scale_amount);
					} else {
						*(m_destination_2_value[voice]) += (*m_source_value[voice]) * m_mod_amount_2 *
						                                   fabs(m_mod_amount_2) *
						                                   (1 + fabs(*m_scale_value[voice]) * m_scale_amount);
					}
				} else {
					// DBG("MOD SLOT ACTIVE");
					*(m_destination_2_value[voice]) += (*m_source_value[voice]) * m_mod_amount_2 * fabs(m_mod_amount_2);
				}
			}
		} else // mono destination
		{
			// use most recent voice for poly, for mono all #VOICES are identical so it
			// doesnt matter
			if (m_scale) {
				if (m_scale_amount >= 0) {
					*(m_destination_2_value[0]) += (*m_source_value[m_most_recent_voice]) * m_mod_amount_2 *
					                               fabs(m_mod_amount_2) *
					                               (1 + (*m_scale_value[m_most_recent_voice] - 1) * m_scale_amount);
				} else {
					*(m_destination_2_value[0]) += (*m_source_value[m_most_recent_voice]) * m_mod_amount_2 *
					                               fabs(m_mod_amount_2) *
					                               (1 + fabs(*m_scale_value[m_most_recent_voice]) * m_scale_amount);
				}
			} else {
				*(m_destination_2_value[0]) +=
				    (*m_source_value[m_most_recent_voice]) * m_mod_amount_2 * fabs(m_mod_amount_2);
			}
		}
	}
}

void ModMatrixRow::setModSource(int p_source) {
	setModSource(p_source, m_source_value, m_source);
}

void ModMatrixRow::setModScale(int p_source) {
	setModSource(p_source, m_scale_value, m_scale);
}

void ModMatrixRow::setModSource(int p_source, float **p_source_pointers, int &p_source_store) {

	switch (p_source) {
	case 100:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].osc[0];
		}
		break;
	case 101:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].osc[1];
		}
		break;
	case 102:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].osc[2];
		}
		break;
	case 110:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].filter[0];
		}
		break;
	case 111:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].filter[1];
		}
		break;
	case 200:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].adsr[0];
		}
		break;
	case 201:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].adsr[1];
		}
		break;
	case 202:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].adsr[2];
		}
		break;
	case 203:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->global_adsr;
		}
		break;
	case 300:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].lfo[0];
		}
		break;
	case 301:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].lfo[1];
		}
		break;
	case 302:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].lfo[2];
		}
		break;
	case 303:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->global_lfo;
		}
		break;

	case 400:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->x;
		}
		break;
	case 401:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->y;
		}
		break;
	case 402:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->modwheel;
		}
		break;
	case 403:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->pitchwheel;
		}
		break;

	case 404:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].MIDI_key;
		}
		break;
	case 405:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].MIDI_velocity;
		}
		break;
	case 406:// ? this does not exists anymore... is here for crash avoidance when loading backward?
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].MIDI_aftertouch;
		}
		break;
	case 407:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->MIDI_breath;
		}
		break;
	case 409:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->MIDI_channel_pressure;
		}
		break;
	case 420:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].unison_position;
		}
		break;
	case 430:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].arp_mod_1;
		}
		break;
	case 435:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].arp_mod_2;
		}
		break;
	case 440:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->sustain_pedal;
		}
		break;
	case 450:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->soft_pedal;
		}
		break;
	case 999:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->voice[voice].random;
		}
		break;
	case 1000:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->constant;
		}
		break;
	case 0:
		//none
		break;
	default:
		//use const as default fallback (avoid segfaults down the line...):
		DBG("ERROR: Unknown modulation source was set: " + std::to_string(p_source));
		for (int voice = 0; voice < VOICES; ++voice) {
			p_source_pointers[voice] = m_sources->constant;
		}
		break;
	}

	p_source_store = p_source;

	checkRowActive();
}

void ModMatrixRow::setModDestination1(int p_destination) {
	setModDestination(p_destination, m_destination_1_value, m_destination_1_poly, m_destination_1);
}

void ModMatrixRow::setModDestination2(int p_destination) {
	setModDestination(p_destination, m_destination_2_value, m_destination_2_poly, m_destination_2);
	// DBG("val: " + std::to_string(p_destination));
}

void ModMatrixRow::setModDestination(int p_destination,
                                     float **p_destination_pointers,
                                     bool &p_dest_poly,
                                     int &p_destination_store) {

	switch (p_destination) {
	case 2:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].pitch_exponential);
		}
		p_dest_poly = true;
		break;

	case 102:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].pitch_exponential);
		}
		p_dest_poly = true;
		break;

	case 202:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].pitch_exponential);
		}
		p_dest_poly = true;
		break;

	case 3:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].pitch_linear);
		}
		p_dest_poly = true;
		break;

	case 103:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].pitch_linear);
		}
		p_dest_poly = true;
		break;

	case 203:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].pitch_linear);
		}
		p_dest_poly = true;
		break;

	case 4:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].vol);
		}
		p_dest_poly = true;
		break;

	case 104:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].vol);
		}
		p_dest_poly = true;
		break;

	case 204:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].vol);
		}
		p_dest_poly = true;
		break;

	case 10:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].pulse_width);
		}
		p_dest_poly = true;
		break;

	case 110:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].pulse_width);
		}
		p_dest_poly = true;
		break;

	case 210:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].pulse_width);
		}
		p_dest_poly = true;
		break;

	case 20:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].position);
		}
		p_dest_poly = true;
		break;
	case 120:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].position);
		}
		p_dest_poly = true;
		break;
	case 220:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].position);
		}
		p_dest_poly = true;
		break;

	case 21:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].detune);
		}
		p_dest_poly = true;
		break;
	case 121:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].detune);
		}
		p_dest_poly = true;
		break;
	case 221:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].detune);
		}
		p_dest_poly = true;
		break;

	case 22:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].spread);
		}
		p_dest_poly = true;
		break;
	case 122:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].spread);
		}
		p_dest_poly = true;
		break;
	case 222:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].spread);
		}
		p_dest_poly = true;
		break;

	case 30:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].x);
		}
		p_dest_poly = true;
		break;
	case 130:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].x);
		}
		p_dest_poly = true;
		break;
	case 230:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].x);
		}
		p_dest_poly = true;
		break;

	case 31:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].y);
		}
		p_dest_poly = true;
		break;
	case 131:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].y);
		}
		p_dest_poly = true;
		break;
	case 231:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].y);
		}
		p_dest_poly = true;
		break;

	case 40:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].arp_speed);
		}
		p_dest_poly = true;
		break;
	case 140:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].arp_speed);
		}
		p_dest_poly = true;
		break;
	case 240:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].arp_speed);
		}
		p_dest_poly = true;
		break;

	case 50:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].fm_amount);
		}
		p_dest_poly = true;
		break;
	case 150:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].fm_amount);
		}
		p_dest_poly = true;
		break;
	case 250:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].fm_amount);
		}
		p_dest_poly = true;
		break;

	case 51:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].carrier_ratio);
		}
		p_dest_poly = true;
		break;
	case 151:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].carrier_ratio);
		}
		p_dest_poly = true;
		break;
	case 251:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].carrier_ratio);
		}
		p_dest_poly = true;
		break;

	case 52:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].modulator_ratio);
		}
		p_dest_poly = true;
		break;
	case 152:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].modulator_ratio);
		}
		p_dest_poly = true;
		break;
	case 252:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].modulator_ratio);
		}
		p_dest_poly = true;
		break;

	case 60:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].lp_freq);
		}
		p_dest_poly = true;
		break;
	case 160:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].lp_freq);
		}
		p_dest_poly = true;
		break;
	case 260:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].lp_freq);
		}
		p_dest_poly = true;
		break;

	case 61:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[0].hp_freq);
		}
		p_dest_poly = true;
		break;
	case 161:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[1].hp_freq);
		}
		p_dest_poly = true;
		break;
	case 261:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].hp_freq);
		}
		p_dest_poly = true;
		break;

	case 301:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].freq);
		}
		p_dest_poly = true;
		break;
	case 401:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].freq);
		}
		p_dest_poly = true;
		break;
	case 501:
		p_destination_pointers[0] = &(m_destinations->filter3.freq);
		p_dest_poly               = false;
		break;

	case 302:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].res);
		}
		p_dest_poly = true;
		break;
	case 402:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].res);
		}
		p_dest_poly = true;
		break;
	case 502:
		p_destination_pointers[0] = &(m_destinations->filter3.res);
		p_dest_poly               = false;
		break;

	case 303:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].gain);
		}
		p_dest_poly = true;
		break;
	case 403:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].gain);
		}
		p_dest_poly = true;
		break;
	case 503:
		p_destination_pointers[0] = &(m_destinations->filter3.gain);
		p_dest_poly               = false;
		break;

	case 304:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].env_amount);
		}
		p_dest_poly = true;
		break;
	case 404:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].env_amount);
		}
		p_dest_poly = true;
		break;
	case 504:
		p_destination_pointers[0] = &(m_destinations->filter3.env_amount);
		p_dest_poly               = false;
		break;

	case 305:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].vel_amount);
		}
		p_dest_poly = true;
		break;
	case 405:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].vel_amount);
		}
		p_dest_poly = true;
		break;
	case 505:
		p_destination_pointers[0] = &(m_destinations->filter3.vel_amount);
		p_dest_poly               = false;
		break;

	case 306:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].kbd_amount);
		}
		p_dest_poly = true;
		break;
	case 406:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].kbd_amount);
		}
		p_dest_poly = true;
		break;
	case 506:
		p_destination_pointers[0] = &(m_destinations->filter3.kbd_amount);
		p_dest_poly               = false;
		break;

	case 307:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].saturation);
		}
		p_dest_poly = true;
		break;
	case 407:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].saturation);
		}
		p_dest_poly = true;
		break;
	case 507:
		p_destination_pointers[0] = &(m_destinations->filter3.saturation);
		p_dest_poly               = false;
		break;

	case 310:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].SEM_transition);
		}
		p_dest_poly = true;
		break;
	case 410:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].SEM_transition);
		}
		p_dest_poly = true;
		break;
	case 510:
		p_destination_pointers[0] = &(m_destinations->filter3.SEM_transition);
		p_dest_poly               = false;
		break;

	case 320:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].formant_transition);
		}
		p_dest_poly = true;
		break;
	case 420:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].formant_transition);
		}
		p_dest_poly = true;
		break;
	case 520:
		p_destination_pointers[0] = &(m_destinations->filter3.formant_transition);
		p_dest_poly               = false;
		break;

	case 330:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[0].ringmod_amount);
		}
		p_dest_poly = true;
		break;
	case 430:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].filter[1].ringmod_amount);
		}
		p_dest_poly = true;
		break;
	case 530:
		p_destination_pointers[0] = &(m_destinations->filter3.ringmod_amount);
		p_dest_poly               = false;
		break;

	case 601:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[0].attack);
		}
		p_dest_poly = true;
		break;
	case 611:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[1].attack);
		}
		p_dest_poly = true;
		break;
	case 621:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[2].attack);
		}
		p_dest_poly = true;
		break;
	case 631:
		p_destination_pointers[0] = &(m_destinations->global_adsr.attack);
		p_dest_poly               = false;
		break;

	case 602:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[0].decay);
		}
		p_dest_poly = true;
		break;
	case 612:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[1].decay);
		}
		p_dest_poly = true;
		break;
	case 622:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[2].decay);
		}
		p_dest_poly = true;
		break;
	case 632:
		p_destination_pointers[0] = &(m_destinations->global_adsr.decay);
		p_dest_poly               = false;
		break;

	case 603:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[0].sustain);
		}
		p_dest_poly = true;
		break;
	case 613:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[1].sustain);
		}
		p_dest_poly = true;
		break;
	case 623:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[2].sustain);
		}
		p_dest_poly = true;
		break;
	case 633:
		p_destination_pointers[0] = &(m_destinations->global_adsr.sustain);
		p_dest_poly               = false;
		break;

	case 604:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[0].release);
		}
		p_dest_poly = true;
		break;
	case 614:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[1].release);
		}
		p_dest_poly = true;
		break;
	case 624:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].adsr[2].release);
		}
		p_dest_poly = true;
		break;
	case 634:
		p_destination_pointers[0] = &(m_destinations->global_adsr.release);
		p_dest_poly               = false;
		break;

	case 651:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].lfo[0].freq);
		}
		p_dest_poly = true;
		break;
	case 661:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].lfo[1].freq);
		}
		p_dest_poly = true;
		break;
	case 671:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].lfo[2].freq);
		}
		p_dest_poly = true;
		break;
	case 681:
		p_destination_pointers[0] = &(m_destinations->global_lfo.freq);
		p_dest_poly               = false;
		break;

	case 701:
		p_destination_pointers[0] = &(m_destinations->delay.time);
		p_dest_poly               = false;
		break;
	case 702:
		p_destination_pointers[0] = &(m_destinations->delay.feedback);
		p_dest_poly               = false;
		break;
	case 703:
		p_destination_pointers[0] = &(m_destinations->delay.hp_freq);
		p_dest_poly               = false;
		break;
	case 704:
		p_destination_pointers[0] = &(m_destinations->delay.dry);
		p_dest_poly               = false;
		break;

	case 751:
		p_destination_pointers[0] = &(m_destinations->phaser.rate);
		p_dest_poly               = false;
		break;
	case 752:
		p_destination_pointers[0] = &(m_destinations->phaser.amount);
		p_dest_poly               = false;
		break;
	case 753:
		p_destination_pointers[0] = &(m_destinations->phaser.freq);
		p_dest_poly               = false;
		break;
	case 754:
		p_destination_pointers[0] = &(m_destinations->phaser.feedback);
		p_dest_poly               = false;
		break;
	case 755:
		p_destination_pointers[0] = &(m_destinations->phaser.drywet);
		p_dest_poly               = false;
		break;

	case 801:
		p_destination_pointers[0] = &(m_destinations->flanger.freq);
		p_dest_poly               = false;
		break;
	case 802:
		p_destination_pointers[0] = &(m_destinations->flanger.amount);
		p_dest_poly               = false;
		break;
	case 803:
		p_destination_pointers[0] = &(m_destinations->flanger.feedback);
		p_dest_poly               = false;
		break;
	case 804:
		p_destination_pointers[0] = &(m_destinations->flanger.drywet);
		p_dest_poly               = false;
		break;

	case 851:
		p_destination_pointers[0] = &(m_destinations->chorus.freq);
		p_dest_poly               = false;
		break;
	case 852:
		p_destination_pointers[0] = &(m_destinations->chorus.amount);
		p_dest_poly               = false;
		break;
	case 853:
		p_destination_pointers[0] = &(m_destinations->chorus.feedback);
		p_dest_poly               = false;
		break;
	case 854:
		p_destination_pointers[0] = &(m_destinations->chorus.drywet);
		p_dest_poly               = false;
		break;

	case 705:
		p_destination_pointers[0] = &(m_destinations->delay.wet);
		p_dest_poly               = false;
		break;

	case 900:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].amp.gain);
			p_dest_poly               = true;
		}
		break;
	case 901:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].amp.pan);
			p_dest_poly               = true;
		}
		break;
	case 902:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].amp.vel);
			p_dest_poly               = true;
		}
		break;

	case 950:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].distortion.boost);
			p_dest_poly               = true;
		}
		break;
	case 951:
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].distortion.drywet);
			p_dest_poly               = true;
		}
		break;
	case 970:
		p_destination_pointers[0] = &(m_destinations->arp.speed);
		p_dest_poly               = false;
		break;
	case 973:
		p_destination_pointers[0] = &(m_destinations->arp.gate);
		p_dest_poly               = false;
		break;
	case 980:
		p_destination_pointers[0] = &(m_destinations->xy.x);
		p_dest_poly               = false;
		break;
	case 982:
		p_destination_pointers[0] = &(m_destinations->xy.y);
		p_dest_poly               = false;
		break;
	case 999:
		p_destination_pointers[0] = &(m_destinations->misc.glide);
		p_dest_poly               = false;
		break;
	case 1000:
		p_destination_pointers[0] = &(m_destinations->misc.master);
		p_dest_poly               = false;
		break;
	case 0:
		//none
		break;
	default:
		DBG("ERROR: Unknown modulation dest was set: " + std::to_string(p_destination));
		// use osc3 spread as fallback (the most obscure I could find :D )
		for (int voice = 0; voice < VOICES; ++voice) {
			p_destination_pointers[voice] = &(m_destinations->voice[voice].osc[2].spread);
		}
		p_dest_poly = true;
		break;
		break;
	}
	p_destination_store = p_destination;

	// check if modulation still applies
	checkRowActive();
}

void ModMatrixRow::checkRowActive() {
	m_active_1 = (m_source && m_destination_1);
	m_active_2 = (m_source && m_destination_2);
	//DBG("src " + std::to_string(m_source) + " dest " + std::to_string(m_destination_1) +
	//    " act: " + std::to_string(m_active_1));
}

bool ModMatrixRow::usesLFO0() {
	return m_source == 300 || m_scale == 300;
}

bool ModMatrixRow::usesLFO1() {
	return m_source == 301 || m_scale == 301;
}

bool ModMatrixRow::usesLFO2() {
	return m_source == 302 || m_scale == 302;
}

bool ModMatrixRow::usesLFO3() {
	return m_source == 303 || m_scale == 303;
}

bool ModMatrixRow::usesADSR0() // mod env
{
	return m_source == 202 || m_scale == 202;
}

bool ModMatrixRow::usesADSR1() // global env
{
	return m_source == 203 || m_scale == 203;
}

//=========================================================
//=================== MODMATRIX ===========================
//=========================================================

void ModMatrix::applyModulation() {
	for (int row = 0; row < MODMATRIX_ROWS; ++row) {
		if (m_row[row]) {
			m_row[row].applyModulation();
		}
	}
}

void ModMatrix::setSourcesAndDestinations(ModSources *p_sources, ModDestinations *p_destinations) {
	for (int row = 0; row < MODMATRIX_ROWS; ++row) {
		m_row[row].setSourcesAndDestinations(p_sources, p_destinations);
	}
	m_sources      = p_sources;
	m_destinations = p_destinations;
	zeroAllDestinations();
	zeroAllSources();
}

// ModMatrix::ModMatrix(ModSources *p_sources, ModDestinations
// *p_destinations){
//  for (int row = 0; row < MODMATRIX_ROWS; ++row) {
//    m_row[row].setSourcesAndDestinations(p_sources, p_destinations);
//  }
//}

void ModMatrix::setModSource(int p_row, int p_source) {
	m_row[p_row].setModSource(p_source);
	checkWhichSourceToRender();
}

void ModMatrix::setModDestination1(int p_row, int p_destination) {
	m_row[p_row].setModDestination1(p_destination);
}
void ModMatrix::setModDestination2(int p_row, int p_destination) {
	m_row[p_row].setModDestination2(p_destination);
	// DBG("row: " + std::to_string(p_row));
}

void ModMatrix::setModScale(int p_row, int p_scale) {
	m_row[p_row].setModScale(p_scale);
	checkWhichSourceToRender();
}

void ModMatrix::setModAmount1(int p_row, float p_mod_amount) {
	m_row[p_row].setModAmount1(p_mod_amount);
}
void ModMatrix::setModAmount2(int p_row, float p_mod_amount) {
	m_row[p_row].setModAmount2(p_mod_amount);
	// DBG("row: " + std::to_string(p_row));
}

void ModMatrix::setScaleAmount(int p_row, float p_scale_amount) {
	m_row[p_row].setScaleAmount(p_scale_amount);
}

void ModMatrix::zeroAllSources() {
	memset(m_sources, 0, sizeof(ModSources));
}

void ModMatrix::zeroAllDestinations() {
	memset(m_destinations, 0, sizeof(ModDestinations));
}

void ModMatrix::checkWhichSourceToRender() {
	bool render_LFO[4]  = {0};
	bool render_ADSR[2] = {0};

	for (int row = 0; row < MODMATRIX_ROWS; ++row) {
		render_LFO[0] = m_row[row].usesLFO0() ? true : render_LFO[0];
		render_LFO[1] = m_row[row].usesLFO1() ? true : render_LFO[1];
		render_LFO[2] = m_row[row].usesLFO2() ? true : render_LFO[2];
		render_LFO[3] = m_row[row].usesLFO3() ? true : render_LFO[3];

		render_ADSR[0] = m_row[row].usesADSR0() ? true : render_ADSR[0];
		render_ADSR[1] = m_row[row].usesADSR1() ? true : render_ADSR[1];
	}

	setSourcesToRender(render_LFO[0], render_LFO[1], render_LFO[2], render_LFO[3], render_ADSR[0], render_ADSR[1]);
}