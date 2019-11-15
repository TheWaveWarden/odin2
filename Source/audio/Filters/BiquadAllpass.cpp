#include "BiquadAllpass.h"
#include <cmath>

BiquadAllpass::BiquadAllpass() {
	//stays the same for all configurations
	m_b2 = 1.;
}

BiquadAllpass::~BiquadAllpass() {
}

void BiquadAllpass::setFrequency(float p_frequency) {
	//Transfer function:
	//H(z) = (1/r² - 2/r*cos * z^-1 + z^-2)/(1 - 2/r*cos * z^-1 + 1/r² * z^-2)

	//convert freq to radial freq
	float freq_rad = p_frequency * m_one_over_samplerate * 2 * PI;

	m_b1 = -2.f / m_radius * cos(freq_rad);
	m_a1 = m_b1;
	m_b0 = 1.f / (m_radius * m_radius);
	m_a2 = m_b0;
}