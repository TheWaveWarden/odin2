#include "OdinArpeggiator.h"
#include <algorithm>
#include <random>

#define NO_NOTE std::make_pair(-1, 0)

bool sortKeysDownToUp(std::pair<int, int> a, std::pair<int, int> b) {
	return a.first < b.first;
}

bool sortKeysUpToDown(std::pair<int, int> a, std::pair<int, int> b) {
	return a.first > b.first;
}

std::pair<int, int> OdinArpeggiator::getNoteOns(int &pio_step_active) {
	jassert(m_samplerate > 0);

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
			m_playing_notes.push_back(std::make_pair(m_arp_sequence[m_current_arp_index].first, 0.f));
			return m_arp_sequence[m_current_arp_index];
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
		}
		pio_step_active        = m_current_sequence_index;
		m_time_since_last_note = 0.f;

		if (m_sequence_steps_on[m_current_sequence_index]) {
			m_playing_notes.push_back(std::make_pair(m_arp_sequence[m_current_arp_index].first, 0.f));
			return m_arp_sequence[m_current_arp_index];
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

std::pair<int, int> OdinArpeggiator::transpose(std::pair<int, int> note, int p_octave) {
	return std::make_pair(note.first + 12 * p_octave, note.second);
}

void OdinArpeggiator::generateSequence() { //sort active keys

	m_arp_sequence.clear();
	switch (m_pattern) {
	case ArpPattern::Up:
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysDownToUp);
		for (int octave = 0; octave < m_octaves; ++octave) {
			for (auto note : m_active_keys_and_velocities) {
				m_arp_sequence.push_back(transpose(note, octave));
			}
		}
		break;
	case ArpPattern::Down:
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysUpToDown);
		for (int octave = m_octaves - 1; octave >= 0; --octave) {
			for (auto note : m_active_keys_and_velocities) {
				m_arp_sequence.push_back(transpose(note, octave));
			}
		}
		break;
	case ArpPattern::UpAndDown:
		std::sort(m_active_keys_and_velocities.begin(), m_active_keys_and_velocities.end(), sortKeysDownToUp);
		for (int octave = 0; octave < m_octaves; ++octave) {
			for (auto note : m_active_keys_and_velocities) {
				m_arp_sequence.push_back(transpose(note, octave));
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
						m_arp_sequence.push_back(transpose(note, octave));
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
				m_arp_sequence.push_back(transpose(note, octave));
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
}