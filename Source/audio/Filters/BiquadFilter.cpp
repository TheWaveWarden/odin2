#include "BiquadFilter.h"

BiquadFilter::BiquadFilter() {
}

BiquadFilter::~BiquadFilter() {
}

void BiquadFilter::setFilterCoefficients(double p_a1, double p_a2, double p_b0, double p_b1, double p_b2) {
	m_a1 = p_a1;
	m_a2 = p_a2;
	m_b0 = p_b0;
	m_b1 = p_b1;
	m_b2 = p_b2;
}

double BiquadFilter::doFilter(double p_input) {

	//this uses direct form 2 from wikipedia page
	//https://en.wikipedia.org/wiki/Digital_biquad_filter
	double left_side = p_input - m_z2 * m_a2 - m_z1 * m_a1;
	double output    = m_b2 * m_z2 + m_b1 * m_z1 + m_b0 * left_side;

	//update buffers
	m_z2 = m_z1;
	m_z1 = left_side;

	return output;
}
