#include "OdinArpeggiator.h"
#include <algorithm>
#include <random>

#define NO_NOTE std::make_tuple(-1, 0, 0.f)

bool sortKeysDownToUp(std::pair<int, int> a, std::pair<int, int> b) {
	return a.first < b.first;
}

bool sortKeysUpToDown(std::pair<int, int> a, std::pair<int, int> b) {
	return a.first > b.first;
}

String OdinArpeggiator::ArpPatternToString(ArpPattern p_pattern) {
	switch (p_pattern) {
	case ArpPattern::Up:
		return "Up";
	case ArpPattern::Down:
		return "Down";
	case ArpPattern::UpAndDown:
		return "Up Down";
	case ArpPattern::DownAndUp:
		return "Down Up";
	case ArpPattern::Random:
		return "Random";
	case ArpPattern::CrawlUp:
		return "CrawlUp";
	case ArpPattern::CrawlDown:
		return "CrawlDown";
	case ArpPattern::CrawlUpDown:
		return "CrawlUpDo";
	case ArpPattern::CrawlDownUp:
		return "CrawlDoUp";
	default:
		return "Unknown";
	}
}

std::tuple<int, int, float> OdinArpeggiator::getNoteOns(int &pio_step_active) {
	jassert(m_samplerate > 0);

	if (m_oneshot_end_reached) {
		pio_step_active = -1;
		return NO_NOTE;
	}

	//pattern is empty, nothing to do
	if (m_arp_sequence.size() == 0) {
		pio_step_active = -1;
		return NO_NOTE;
	}

	//increment time
	m_time_since_last_note += m_one_over_samplerate;

	//start new pattern?
	if (m_start_pattern) {
		m_start_pattern          = false;
		m_current_arp_index      = 0;
		m_current_sequence_index = 0;
		pio_step_active          = 0;
		m_time_since_last_note   = 0.f;
		if (m_sequence_steps_on[m_current_sequence_index]) {
			m_playing_notes.push_back(std::make_pair(
			    m_arp_sequence[m_current_arp_index].first + m_transpose_steps[m_current_sequence_index], 0.f));
			return transposeSemi(m_arp_sequence[m_current_arp_index],
			                     m_transpose_steps[m_current_sequence_index],
			                     m_mod_steps[m_current_sequence_index]);
		} else {
			return NO_NOTE;
		}
	}

	//start new note?
	if (m_time_since_last_note > m_arp_time) {
		++m_current_arp_index;
		if (m_current_arp_index >= m_arp_sequence.size()) {
			//updown skips a beat when wrapping
			if ((m_pattern == ArpPattern::UpAndDown || m_pattern == ArpPattern::UpAndDown) &&
			    m_arp_sequence.size() > 1) {
				m_current_arp_index = 1;
			} else {
				m_current_arp_index = 0;
			}
		}
		++m_current_sequence_index;
		if (m_current_sequence_index >= m_max_sequence_steps) {
			m_current_sequence_index = 0;
			if (m_oneshot) {
				m_current_sequence_index = 0;
				pio_step_active          = -1;
				m_oneshot_end_reached    = true;
				return NO_NOTE;
			}
		}
		pio_step_active        = m_current_sequence_index;
		m_time_since_last_note = 0.f;

		if (m_sequence_steps_on[m_current_sequence_index]) {
			m_playing_notes.push_back(std::make_pair(
			    m_arp_sequence[m_current_arp_index].first + m_transpose_steps[m_current_sequence_index], 0.f));
			return transposeSemi(m_arp_sequence[m_current_arp_index],
			                     m_transpose_steps[m_current_sequence_index],
			                     m_mod_steps[m_current_sequence_index]);
		}
	}

	//well then do nothing I guess
	pio_step_active = m_current_sequence_index;
	return NO_NOTE;
}

std::vector<int> OdinArpeggiator::getNoteOffs() {
	std::vector<int> ret;
	for (int note = 0; note < m_playing_notes.size(); ++note) {
		m_playing_notes[note].second += m_one_over_samplerate;
		if (m_playing_notes[note].second > m_arp_time * m_gate) {
			ret.push_back(m_playing_notes[note].first);
			m_playing_notes.erase(m_playing_notes.begin() + note);
			//go one back since we deleted one
			--note;
		}
	}

	return ret;
}

void OdinArpeggiator::setSampleRate(double p_samplerate) {
	m_samplerate          = p_samplerate;
	m_one_over_samplerate = 1. / m_samplerate;
}

void OdinArpeggiator::reset() {
	allMidiNotesOff();
	m_current_arp_index = -1;
}

void OdinArpeggiator::midiNoteOn(int p_midi_note, int p_midi_velocity) {
	m_active_keys_and_velocities.push_back(std::make_pair(p_midi_note, p_midi_velocity));
	if (m_active_keys_and_velocities.size() == 1) {
		m_start_pattern = true;
	}
	generateSequence();
	m_oneshot_end_reached = false;
}

void OdinArpeggiator::midiNoteOff(int p_midi_note) {
	for (int key = 0; key < m_active_keys_and_velocities.size(); ++key) {
		if (m_active_keys_and_velocities[key].first == p_midi_note) {
			if (m_sustain_active) {
				m_sustain_kill_list.push_back(p_midi_note);
				generateSequence();
			} else {
				m_active_keys_and_velocities.erase(m_active_keys_and_velocities.begin() + key);
				//in case we have the same note two times in a row (they are sorted):
				--key;
			}
		}
	}
	generateSequence();
}

void OdinArpeggiator::allMidiNotesOff() {
	m_active_keys_and_velocities.clear();
	m_sustain_kill_list.clear();
	m_arp_sequence.clear();
	m_current_arp_index = -1;
}

void OdinArpeggiator::setSustainActive(bool p_sustain_active) {
	m_sustain_active = p_sustain_active;
	if (!m_sustain_active) {
		executeKillList();
	}
}

void OdinArpeggiator::executeKillList() {
	// we are now in sustain off, so midinoteoff kills notes for sure
	for (auto key : m_sustain_kill_list) {
		midiNoteOff(key);
	}
	generateSequence();
}

void OdinArpeggiator::setSequenceStepActive(int p_step, bool p_active) {
	m_sequence_steps_on[p_step] = p_active;
}

std::pair<int, int> OdinArpeggiator::transposeOct(std::pair<int, int> p_note, int p_octave) {
	return std::make_pair(p_note.first + 12 * p_octave, p_note.second);
}

std::tuple<int, int, float> OdinArpeggiator::transposeSemi(std::pair<int, int> p_note, int p_semitones, float p_mod) {
	return std::make_tuple(p_note.first + p_semitones, p_note.second, p_mod);
}

void OdinArpeggiator::generateSequence() { //sort active keys

	m_arp_sequence.clear();
	switch (m_pattern) {
	case ArpPattern::Up:
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysDownToUp);
		for (int octave = 0; octave < m_octaves; ++octave) {
			for (auto note : m_active_keys_and_velocities) {
				m_arp_sequence.push_back(transposeOct(note, octave));
			}
		}
		break;
	case ArpPattern::Down:
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysUpToDown);
		for (int octave = m_octaves - 1; octave >= 0; --octave) {
			for (auto note : m_active_keys_and_velocities) {
				m_arp_sequence.push_back(transposeOct(note, octave));
			}
		}
		break;
	case ArpPattern::UpAndDown:
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysDownToUp);
		for (int octave = 0; octave < m_octaves; ++octave) {
			for (auto note : m_active_keys_and_velocities) {
				m_arp_sequence.push_back(transposeOct(note, octave));
			}
		}
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysUpToDown);
		{
			bool first_note = true;
			for (int octave = m_octaves - 1; octave >= 0; --octave) {
				for (auto note : m_active_keys_and_velocities) {
					//omit first note when going down again
					if (first_note) {
						first_note = false;
					} else {
						m_arp_sequence.push_back(transposeOct(note, octave));
					}
				}
			}
		}
		break;
	case ArpPattern::Random:
		//do up pattern and shuffle it
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysDownToUp);
		for (int octave = 0; octave < m_octaves; ++octave) {
			for (auto note : m_active_keys_and_velocities) {
				m_arp_sequence.push_back(transposeOct(note, octave));
			}
		}
		{
			auto RNG = std::default_random_engine{};
			std::shuffle(std::begin(m_arp_sequence), std::end(m_arp_sequence), RNG);
		}
		break;
	default:
		break;
	}

	printSequence();
}

void OdinArpeggiator::printSequence() {
	DBG("Current sequence:");
	for (auto note : m_arp_sequence) {
		DBG("Key: " + String(note.first) + ", Vel: " + String(note.second));
	}
	DBG("----");
}

void OdinArpeggiator::setBPM(double p_BPM) {
	m_BPM = p_BPM;
	calcArpTime();
}

void OdinArpeggiator::setOneShotEnabled(bool p_oneshot) {
	m_oneshot = p_oneshot;
}

void OdinArpeggiator::setSynctimeNumerator(float p_value) {
	DBG("numerator");
	m_synctime_numerator = p_value;
	m_synctime_ratio     = p_value / m_synctime_denominator;
	calcArpTime();
}

void OdinArpeggiator::setSynctimeDenominator(float p_value) {
	m_synctime_denominator = p_value;
	m_synctime_ratio       = m_synctime_numerator / p_value;
	calcArpTime();
}

void OdinArpeggiator::setOctaves(int p_new_value) {
	m_octaves = p_new_value;
}

void OdinArpeggiator::setDirection(int p_new_value) {
	m_pattern = (ArpPattern)p_new_value;
}

void OdinArpeggiator::setSteps(int p_new_value) {
	m_max_sequence_steps = p_new_value;
}

void OdinArpeggiator::setGatePercent(int p_new_value) {
	m_gate = (float)p_new_value / 100.f;
}

void OdinArpeggiator::calcArpTime() {
	m_arp_time = m_synctime_ratio * 240.f / m_BPM;
}

void OdinArpeggiator::setStepTranspose(int p_step, int p_semi) {
	m_transpose_steps[p_step] = p_semi;
}

void OdinArpeggiator::setStepMod(int p_step, float p_mod) {
	m_mod_steps[p_step] = p_mod;
}