#pragma once

#define DRIFT_LENGTH_SECONDS 5

class DriftGenerator
{
public:
	DriftGenerator();
	~DriftGenerator();


	float doDrift();
	void initialize(float p_samplerate);
	void setSampleRate(float p_samplerate);
protected:

	float getRand();
	void calcNewCoeffs();

	int m_counter = 0;
	int m_drift_length; // multiples of samplerate
	float m_last_value;
	float m_next_value;
	//float m_a = 0.f;
	//float m_b = 0.f;
	//float m_c = 0.f;

	float m_samplerate;
	float m_one_over_samplerate;
};

