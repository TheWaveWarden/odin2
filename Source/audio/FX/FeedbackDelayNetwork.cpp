#include "FeedbackDelayNetwork.h"

float rand01() {
	return (float)rand() / (float)RAND_MAX;
}

int intPow(int x, unsigned int p) {
	if (p == 0)
		return 1;
	if (p == 1)
		return x;

	int tmp = intPow(x, p / 2);
	if (p % 2 == 0)
		return tmp * tmp;
	else
		return x * tmp * tmp;
}

FeedbackDelayNetwork::FeedbackDelayNetwork() {
	for (int i = 0; i < FDN_SIZE; ++i) {
		const float log_desired_length = log(200.f);
		const int prime_length         = (int)(log_desired_length / log(m_prime_numbers[i]) + 0.5f);

		m_delay[i].setDelayLength(intPow(m_prime_numbers[i], prime_length));
	}

	//set up matrix (Householder Matrix, N=16)
	for (int x_big = 0; x_big < 16; x_big += 4) {
		for (int y_big = 0; y_big < 16; y_big += 4) {
			float matrix_mult = (x_big == y_big) ? 1.f : -1.f;
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					if (x == y) {
						A[x_big + x][y_big + y] = FDN_GAIN_DEFAULT * matrix_mult;
					} else {
						A[x_big + x][y_big + y] = -FDN_GAIN_DEFAULT * matrix_mult;
					}
				}
			}
		}
	}
	for (int delay = 0; delay < FDN_SIZE; ++delay) {
		//m_input_gains[delay] = FDN_GAIN_DEFAULT * ((delay % 2) * 2 - 1);
		m_input_gains[delay]  = FDN_GAIN_DEFAULT;
		m_output_gains[delay] = FDN_GAIN_DEFAULT * rand01();
		m_allpass[delay].setRadius(1.3 + 0.1 * rand01());
		m_allpass[delay].setFrequency(2000.f + 1000.f * rand01());
		m_allpass[delay].setSampleRate(44100.f);
	}

	for (int y = 0; y < FDN_SIZE; ++y) {
		//DBG("input gains: " << m_input_gains[y]);
		DBG(A[0][y] << " " << A[1][y] << " " << A[2][y] << " " << A[3][y] << " " << A[4][y] << " " << A[5][y] << " "
		            << A[6][y] << " " << A[7][y] << " " << A[8][y] << " " << A[9][y] << " " << A[10][y] << " "
		            << A[11][y] << " " << A[12][y] << " " << A[13][y] << " " << A[14][y] << " " << A[15][y]);
	}

	ImpulseResponseCreator::saveImpulseResponse(
	    [&](float p_input) { return doFDN(p_input); }, 48000, "/home/frederik_siepe/odin2/IRs/FDN");
}

FeedbackDelayNetwork::~FeedbackDelayNetwork() {
}

float FeedbackDelayNetwork::doFDN(float p_input) {

	float delay_outputs_temp[FDN_SIZE];
	for (int delay = 0; delay < FDN_SIZE; ++delay) {
		delay_outputs_temp[delay] = m_allpass[delay].doFilter(m_delay[delay].doDelay(
		    m_input_gains[delay] * p_input +

		    //todo this is obviously not changeable with FDN_SIZE:
		    A[delay][0] * m_delay_outputs[0] + A[delay][1] * m_delay_outputs[1] + A[delay][2] * m_delay_outputs[2] +
		    A[delay][3] * m_delay_outputs[3] + A[delay][4] * m_delay_outputs[4] + A[delay][5] * m_delay_outputs[5] +
		    A[delay][6] * m_delay_outputs[6] + A[delay][7] * m_delay_outputs[7] + A[delay][8] * m_delay_outputs[8] +
		    A[delay][9] * m_delay_outputs[9] + A[delay][10] * m_delay_outputs[10] + A[delay][11] * m_delay_outputs[11] +
		    A[delay][12] * m_delay_outputs[12] + A[delay][13] * m_delay_outputs[13] +
		    A[delay][14] * m_delay_outputs[14] + A[delay][15] * m_delay_outputs[15]));
	}
	std::copy(std::begin(delay_outputs_temp), std::end(delay_outputs_temp), std::begin(m_delay_outputs));

	return (m_delay_outputs[0] * m_output_gains[1] + m_delay_outputs[1] * m_output_gains[1] +
	        m_delay_outputs[2] * m_output_gains[2] + m_delay_outputs[3] * m_output_gains[3] +
	        m_delay_outputs[4] * m_output_gains[4] + m_delay_outputs[5] * m_output_gains[5] +
	        m_delay_outputs[6] * m_output_gains[6] + m_delay_outputs[7] * m_output_gains[7] +
	        m_delay_outputs[8] * m_output_gains[8] + m_delay_outputs[9] * m_output_gains[9] +
	        m_delay_outputs[10] * m_output_gains[10] + m_delay_outputs[11] * m_output_gains[11] +
	        m_delay_outputs[12] * m_output_gains[12] + m_delay_outputs[13] * m_output_gains[13] +
	        m_delay_outputs[14] * m_output_gains[14] + m_delay_outputs[15] * m_output_gains[15]) *
	           m_dry_wet +
	       p_input * (1.f - m_dry_wet);
}

void FeedbackDelayNetwork::setDryWet(float p_dry_wet) {
	m_dry_wet = p_dry_wet;
}
