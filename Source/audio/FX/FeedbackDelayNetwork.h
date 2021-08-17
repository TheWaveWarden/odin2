#pragma once
#include "../ImpulseResponseCreator.h"
#include "IntegerDelayLine.h"
#include "../Filters/BiquadAllpass.h"

//https://www.dsprelated.com/freebooks/pasp/FDN_Reverberation.html
//http://pub.dega-akustik.de/DAGA_2014/data/articles/000025.pdf

//! Note: this class is currently not used in the synthesizer
//! It has a rough, metallic sound and needs lots of more research and tweaking
//! to be usefull

#define FDN_SIZE 16
#define FDN_GAIN_DEFAULT 0.245

class FeedbackDelayNetwork {
public:
	FeedbackDelayNetwork();
	~FeedbackDelayNetwork();

	float doFDN(float p_input);
	void setDryWet(float p_dry_wet);

private:
	const int m_prime_numbers[32] = {2,  3,  5,  7,  11, 13, 17, 19, 23, 29,  31,  37,  41,  43,  47,  53,
	                                 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};

	float m_dry_wet                 = 0.5f;
	float m_delay_outputs[FDN_SIZE] = {0};

	//todo make const float
	float A[FDN_SIZE][FDN_SIZE]   = {0};
	float m_input_gains[FDN_SIZE] = {0};
	float m_output_gains[FDN_SIZE] = {0};

	IntegerDelayLine m_delay[FDN_SIZE];
	BiquadAllpass m_allpass[FDN_SIZE];
};
