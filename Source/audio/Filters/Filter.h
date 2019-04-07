#pragma once

//#include "../pluginconstants.h"
//#include "../synthfunctions.h"
#include <cmath>

// 46.881879936465680 semitones = semitonesBetweenFrequencies(80, 18000.0)/2.0
#define FILTER_FC_MOD_RANGE 46.881879936465680
#define FILTER_FC_MIN 80		// 80Hz
#define FILTER_FC_MAX 18000		// 18kHz
#define FILTER_FC_DEFAULT 10000 // 10kHz
#define FILTER_Q_DEFAULT 0.707  // Butterworth

// Filter Abastract Base Class for all filters
class Filter
{
  public:
	Filter(void);
	~Filter(void);

	//todo potential bottleneck
	float pitchShiftMultiplier(float p_semitones){
		return pow(2.f, p_semitones / 12.f);
	}
	//todo
	float fasttanh(float p_input){
		return p_input;
	}


	double m_freq_base;
	double m_res_base;
	int m_MIDI_note = 0;
	
	float m_kbd_mod_amount = 0;
	float m_vel_mod_amount = 0;
    float m_env_mod_amount = 0;

	// --- for an aux filter specific like SEM BSF
	//     control or paasband gain comp (Moog)
	double m_aux_control;
	// --- for NLP - Non Linear Procssing
	bool m_NLP;
	double m_overdrive;

  public:
	inline void setFcMod(double d) { m_mod_frequency = d; }
	virtual double doFilter(double xn) = 0;
	inline virtual void setSampleRate(double d) { m_samplerate = d; }
	virtual void reset();
	virtual void setResControl(double p_res){}
	inline virtual void update()
	{

		//setResControl(m_res_base);

		m_freq_modded = m_freq_base * pitchShiftMultiplier(m_mod_frequency);

		if (m_freq_modded > FILTER_FC_MAX)
			m_freq_modded = FILTER_FC_MAX;
		if (m_freq_modded < FILTER_FC_MIN)
			m_freq_modded = FILTER_FC_MIN;
	}
	
	double m_mod_frequency = 0;
	
  protected:
	double m_samplerate;
	double m_freq_modded;
	double m_res_modded;

};
