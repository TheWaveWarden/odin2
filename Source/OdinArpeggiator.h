#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include <utility>

#define NUMBER_SEQUENCE_STEPS 16

class OdinArpeggiator {
public:
	enum class ArpPattern {
		//dont change these (add inbetween, before, after) or you'll break patches
		Up          = 10,
		Down        = 20,
		UpAndDown   = 30,
		DownAndUp   = 40,
		Random      = 50,
		CrawlUp     = 60,
		CrawlDown   = 70,
		CrawlUpDown = 80,
		CrawlDownUp = 90,
	};

	static String ArpPatternToString(ArpPattern p_pattern);

	std::tuple<int, int, float, float> getNoteOns(int &pio_step_active);
	std::vector<int> getNoteOffs();

	void setSampleRate(double p_samplerate);
	void reset();

	void midiNoteOn(int p_midi_note, int p_midi_velocity);
	void midiNoteOff(int p_midi_note);
	void allMidiNotesOff();
	void endPlayingNotes();


	void setSustainActive(bool p_sustain_active);

	void setSequenceStepActive(int p_step, bool p_active);
	void setBPM(double p_BPM);
	void setOneShotEnabled(bool p_oneshot);

	void setSynctimeNumerator(float p_value);
	void setSynctimeDenominator(float p_value);

	void setOctaves(int p_new_value);
	void setDirection(int p_new_value);
	void setSteps(int p_new_value);
	void setGatePercent(int p_new_value);
	//void setStepTranspose(int p_step, int p_semi);
	void setStepMod1(int p_step, float p_mod);
	void setStepMod2(int p_step, float p_mod);

private:
	float m_synctime_numerator   = 2.f;
	float m_synctime_denominator = 16.f;
	float m_synctime_ratio       = 2.f / 16.f;

	void executeKillList();
	void generateSequence();
	void calcArpTime();

	void printSequence();
	std::pair<int, int> transposeOct(std::pair<int, int> note, int p_ocatve);
	std::tuple<int, int, float, float> transposeSemi(std::pair<int, int> p_note,
	                                                 int p_semitones,
	                                                 float p_mod_1,
	                                                 float p_mod_2);

	bool m_sequence_steps_on[NUMBER_SEQUENCE_STEPS] = {
	    true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true};
	//int m_transpose_steps[NUMBER_SEQUENCE_STEPS] = {0};
	float m_mod_1_steps[NUMBER_SEQUENCE_STEPS]   = {0};
	float m_mod_2_steps[NUMBER_SEQUENCE_STEPS]   = {0};

	std::vector<std::pair<int, int>> m_active_keys_and_velocities = {};
	std::vector<int> m_sustain_kill_list                          = {};

	double m_samplerate          = -1;
	double m_one_over_samplerate = -1;
	double m_BPM                 = 120.;
	bool m_sustain_active        = false;
	bool m_start_pattern         = false;

	//algorithm parameters
	int m_octaves            = 2;
	int m_max_sequence_steps = NUMBER_SEQUENCE_STEPS;
	float m_arp_time         = 0.2;
	float m_gate             = 0.5;
	ArpPattern m_pattern     = ArpPattern::Up;
	bool m_oneshot           = false;

	//temp parametes
	std::vector<std::pair<int, int>> m_arp_sequence;    //note, velocity
	std::vector<std::pair<int, float>> m_playing_notes; //note, timer
	int m_current_arp_index       = -1;
	int m_current_sequence_index  = -1;
	double m_time_since_last_note = 0.;
	bool m_oneshot_end_reached    = false;
};