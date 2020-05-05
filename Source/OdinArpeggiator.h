#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include <utility>

#define NUMBER_SEQUENCE_STEPS 16

class OdinArpeggiator {
    public:

    enum class ArpPattern{
        //dont change these (add inbetween, before, after) or you'll break patches
        Up = 10,
        Down = 20,
        UpAndDown = 30,
        DownAndUp = 40,
        Random = 50
    };

    static String ArpPatternToString(ArpPattern p_pattern);

	std::pair<int, int> getNoteOns(int &pio_step_active);
    std::vector<int> getNoteOffs();

	void setSampleRate(double p_samplerate);
	void reset();

	void midiNoteOn(int p_midi_note, int p_midi_velocity);
	void midiNoteOff(int p_midi_note);
	void allMidiNotesOff();

	void setSustainActive(bool p_sustain_active);

    void setSequenceStepActive(int p_step, bool p_active);
    void setBPM(double p_BPM);

private:
    void executeKillList();
    void generateSequence();
    void printSequence();
    std::pair<int, int> transpose(std::pair<int, int> note, int p_ocatve);

	bool m_sequence_steps_on[NUMBER_SEQUENCE_STEPS] = {
	    true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};

    std::vector<std::pair<int, int>> m_active_keys_and_velocities = {};
    std::vector<int> m_sustain_kill_list = {};

	double m_samplerate          = -1;
	double m_one_over_samplerate = -1;
    double m_BPM;
    bool m_sustain_active = false;
    bool m_start_pattern = false;

    //algorithm parameters
    int m_octaves = 2;
    int m_max_sequence_steps = NUMBER_SEQUENCE_STEPS;
    float m_arp_time = 0.2;
    float m_gate = 0.5;
    ArpPattern m_pattern = ArpPattern::UpAndDown;

    //temp parametes
    std::vector<std::pair<int, int>> m_arp_sequence;//note, velocity
    std::vector<std::pair<int, float>> m_playing_notes;//note, timer
    int m_current_arp_index = -1;
    int m_current_sequence_index = -1;
    double m_time_since_last_note = 0.;
};