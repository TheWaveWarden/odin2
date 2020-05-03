#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include <utility>

#define NUMBER_SEQUENCE_STEPS 16

class OdinArpeggiator {
    public:

    enum class ArpPattern{
        Up,
        Down,
        UpAndDown,
        DownAndUp,
        Random
    };

	std::pair<int, int> getNoteOns();
    std::vector<int> getNoteOffs();

	void setSampleRate(double p_samplerate);
	void reset();

	void midiNoteOn(int p_midi_note, int p_midi_velocity);
	void midiNoteOff(int p_midi_note);
	void allMidiNotesOff();

	void setSustainActive(bool p_sustain_active);

    void setSequenceStepAvtive(int p_step, bool p_active);
    void setBPM(double p_BPM);

private:
    void executeKillList();
    void generateSequence();
    void printSequence();
    std::pair<int, int> transpose(std::pair<int, int> note, int p_ocatve);

	bool m_sequence_steps_on[NUMBER_SEQUENCE_STEPS] = {
	    true, true, false, true, true, false, true, true, false, true, true, false, true, false, true, false};

    std::vector<std::pair<int, int>> m_active_keys_and_velocities = {};
    std::vector<int> m_sustain_kill_list = {};

    double m_time_since_last_note = 0.;
	double m_samplerate          = -1;
	double m_one_over_samplerate = -1;
    double m_BPM;
    bool m_sustain_active = false;
    bool m_start_pattern = false;


    //algorithm parameters
    int m_octaves = 1;
    int m_max_sequence_steps = NUMBER_SEQUENCE_STEPS;
    float m_arp_time = 0.3;
    float m_gate = 0.5;
    ArpPattern m_pattern = ArpPattern::UpAndDown;

    //temp parametes
    std::vector<std::pair<int, int>> m_arp_sequence;//note, velocity
    std::vector<std::pair<int, float>> m_playing_notes;//note, timer
    int m_current_arp_index = -1;
    int m_current_sequence_index = -1;
};