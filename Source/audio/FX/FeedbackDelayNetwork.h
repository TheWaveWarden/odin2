#pragma once
#include "IntegerDelayLine.h"

//https://www.dsprelated.com/freebooks/pasp/FDN_Reverberation.html

#define DFN_SIZE 16

class FeedbackDelayNetwork {
public:
	FeedbackDelayNetwork();
	~FeedbackDelayNetwork();

  float doFDN(float p_input);

private:
	const int m_prime_numbers[32] = {2,  3,  5,  7,  11, 13, 17, 19, 23, 29,  31,  37,  41,  43,  47,  53,
	                               59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131};


  float m_delay_outputs[DFN_SIZE] = {0};

  //todo make const float
  float A[DFN_SIZE][DFN_SIZE] = {0};
  
	IntegerDelayLine m_delay[DFN_SIZE];
};
