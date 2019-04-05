#pragma once

#include "CheapFunctions.h"
#include <fstream> //todo  delay

#define MIN_DECAY_RELEASE_VAL 0.001
#include <cmath>

class ADSREnvelope
{
  public:
	ADSREnvelope();
	~ADSREnvelope();

	float doEnvelope();
	void startRelease();
	int getCurrentSection();

	void testADSRPow();
	void profileCheapPow();

	void setFinishFlag(bool* p_finish_flag){
		m_envelope_finish_flag = p_finish_flag;
	}

	inline void reset()
	{
		m_current_section = -1;
		m_current_value = 0.f;
		m_attack_start_value = 0.f;
	}

	inline void setLoop(bool p_loop)
	{
		m_loop = p_loop;
	}

	inline void setSamplerate(float p_samplerate)
	{
		m_samplerate = p_samplerate;
	}

	inline void setAttack(float p_attack)
	{
		m_attack = p_attack;
	}

	inline void setDecay(float p_decay)
	{
		m_decay = p_decay;
	}

	inline void setSustain(float p_sustain)
	{
		m_sustain = p_sustain;
	}

	inline void setRelease(float p_release)
	{
		m_release = p_release;
	}

	/*inline */double calcDecayFactor(double p_decay)
	{
		return pow(MIN_DECAY_RELEASE_VAL, 1./m_samplerate/p_decay);
	}

	/*inline */double calcDecayFactorCheap(double p_decay)
	{
		return CheapFunctions::getInstance().cheapADSRPow(m_samplerate * p_decay);
	}

  protected:
	//CheapFunctions& m_cheap_functions;

	int m_current_section = -1; //before start:-1, A0, D1, S2, R3
	bool m_loop = false;

	double m_attack = 0.3f;
	double m_attack_start_value = 0.f;
	double m_decay = 1.f;
	double m_decay_factor = 0.9998;
	double m_sustain = 0.5f;
	double m_release = 1.f;
	double m_release_factor = 0.9998;
	double m_release_start_value = 1.;

	double m_current_value = 0.f;
	double m_samplerate;

	bool* m_envelope_finish_flag = nullptr;
};
