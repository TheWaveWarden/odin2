#pragma once
//#include "../pluginconstants.h"
//#include "../synthfunctions.h"
#include <cmath>

#define OSC_FO_MOD_RANGE 2			//2 semitone default
#define OSC_HARD_SYNC_RATIO_RANGE 4	//4
#define OSC_PITCHBEND_MOD_RANGE 12	//12 semitone default
#define OSC_FO_MIN 20				//20Hz
#define OSC_FO_MAX 20480			//20.480kHz = 10 octaves up from 20Hz
#define OSC_FO_DEFAULT 440.0		//A5

class Oscillator
{
		
public:
	Oscillator(void);
	virtual ~Oscillator(void);	
	//
	// --- modulo functions for master/slave operation
	// --- increment the modulo counters
	inline void incModulo(){m_modulo += m_increment;}

	//todo potential bottleneck
	float pitchShiftMultiplier(float p_semitones){
		return pow(2.f, p_semitones / 12.f);
	}

	// --- check and wrap the modulo
	//     returns true if modulo wrapped
	inline bool checkWrapModulo()
	{	
		// --- for positive frequencies
		if(m_increment > 0 && m_modulo >= 1.0) 
		{
			m_modulo -= 1.0; 
			return true;
		}
		// --- for negative frequencies
		if(m_increment < 0 && m_modulo <= 0.0) 
		{
			m_modulo += 1.0; 
			return true;
		}
		return false;
	}
	// --- reset the modulo (required for master->slave operations)
	inline void resetModulo(double d = 0.0){m_modulo = d;}
	inline void setPitchBendMod(double dMod){m_mod_pitch_bend = dMod;}
	//virtual void startOscillator() {m_note_on = true;}
	//virtual void stopOscillator() {m_note_on = false;}
	virtual float doOscillate() = 0; 
	virtual void setSampleRate(float p_sample_rate){m_samplerate = p_sample_rate;}
	virtual void reset();

	// INLINE FUNCTIONS: these are inlined because they will be 
	//                   called every sample period
	//					 You may want to move them to the .cpp file and
	//                   enable the compiler Optimization setting for 
	//					 Inline Function Expansion: Any Suitable though
	//					 inlining here forces it.
	//
	// --- update the frequency, amp mod and PWM
	inline virtual void update()
	{

		// --- do the  complete frequency mod
		m_osc_freq_modded = m_osc_freq_base*pitchShiftMultiplier(m_mod_freq_exp + m_mod_exp_other +
													     m_mod_pitch_bend + 
													     m_octave*12.0 + 
													     m_semitones + 
													     m_cent/100.0);
		// --- apply linear FM (not used in book projects)
		m_osc_freq_modded += m_mod_freq_lin;

		// --- bound Fo (can go outside for FM/PM mod)
		//     +/- 20480 for FM/PM
		if(m_osc_freq_modded > OSC_FO_MAX)
			m_osc_freq_modded = OSC_FO_MAX;
		if(m_osc_freq_modded < -OSC_FO_MAX)
			m_osc_freq_modded = -OSC_FO_MAX;

		// --- calculate increment (a.k.a. phase a.m_k.a. phaseIncrement, etc...)
		m_increment = m_osc_freq_modded/m_samplerate;


	}

	inline virtual void setBaseFrequency(float p_freq){
		m_osc_freq_base = p_freq;
	}
public:

	
	//bool m_note_on; // --- oscillator run flag
	double m_osc_freq_base;	// oscillator frequency from MIDI note number
	double m_amplitude;	// 0->1 from GUI
	double m_modulo;	// modulo counter 0->1
	double m_increment;	// phase inc = fo/fs

	double m_mod_freq_exp;		 // modulation input -1 to +1 */
	double m_mod_freq_lin;		 // FM modulation input -1 to +1 (not actually used in Yamaha FM!) */
	
	int m_octave;	      // octave tweak
	int m_semitones;	  // semitones tweak
	int m_cent;			  // cents tweak

	bool m_reset = false;  //if this is true, osc will reset on noteon
	
	bool m_is_master_osc; // flag indicating we are a master oscillator
	
	Oscillator* m_buddy_oscillator;
	
protected:

	double m_samplerate;	     // fs
	double m_osc_freq_modded;    // current (actual) frequency of oscillator	

	double m_mod_pitch_bend;	 // modulation input -1 to +1 */
	double m_mod_amp;			 // output amplitude modulation for AM 0 to +1 (not dB)*/
	float  m_mod_exp_other = 0.f; // can be used for whatever (Chiptune Arp uses it)
};
