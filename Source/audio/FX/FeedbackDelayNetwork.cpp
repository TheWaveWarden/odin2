#include "FeedbackDelayNetwork.h"

FeedbackDelayNetwork::FeedbackDelayNetwork() {
	for (int i = 0; i < DFN_SIZE; ++i) {
		m_delay[i].setDelayLength(m_prime_numbers[i]);
	}

	//set up matrix
	for (int x_big = 0; x_big < 16; x_big += 4) {
		for (int y_big = 0; y_big < 16; y_big += 4) {
			for (int x = 0; x < 4; ++x) {
				for (int y = 0; y < 4; ++y) {
					if (x == y) {
						A[x_big + x][y_big + y] = 1;
					} else {
						A[x_big + x][y_big + y] = -1;
					}
				}
			}
		}
	}

	for (int y = 0; y < DFN_SIZE; ++y) {
		DBG(A[0][y] << " " << A[1][y] << " " << A[2][y] << " " << A[3][y] << " " << A[4][y] << " " << A[5][y] << " "
		            << A[6][y] << " " << A[7][y] << " " << A[8][y] << " " << A[9][y] << " " << A[10][y] << " "
		            << A[11][y] << " " << A[12][y] << " " << A[13][y] << " " << A[14][y] << " " << A[15][y]);
	}
}

FeedbackDelayNetwork::~FeedbackDelayNetwork() {
}

float FeedbackDelayNetwork::doFDN(float p_input) {

	float delay_outputs_temp[DFN_SIZE];
	for (int delay = 0; delay < DFN_SIZE; ++delay) {
		delay_outputs_temp[delay] = m_delay[delay].doDelay(
		    //todo this is obviously not changeable with DFN_SIZE:
		    A[delay][0] * m_delay_outputs[0] + A[delay][1] * m_delay_outputs[1] + A[delay][2] * m_delay_outputs[2] +
		    A[delay][3] * m_delay_outputs[3] + A[delay][4] * m_delay_outputs[4] + A[delay][5] * m_delay_outputs[5] +
		    A[delay][6] * m_delay_outputs[6] + A[delay][7] * m_delay_outputs[7] + A[delay][8] * m_delay_outputs[8] +
		    A[delay][9] * m_delay_outputs[9] + A[delay][10] * m_delay_outputs[10] + A[delay][11] * m_delay_outputs[11] +
		    A[delay][12] * m_delay_outputs[12] + A[delay][13] * m_delay_outputs[13] +
		    A[delay][14] * m_delay_outputs[14] + A[delay][15] * m_delay_outputs[15]);
	}
	std::copy(std::begin(delay_outputs_temp), std::end(delay_outputs_temp), std::begin(m_delay_outputs));

	return m_delay_outputs[0] + m_delay_outputs[1] + m_delay_outputs[2] + m_delay_outputs[3] + m_delay_outputs[4] +
	       m_delay_outputs[5] + m_delay_outputs[6] + m_delay_outputs[7] + m_delay_outputs[8] + m_delay_outputs[9] +
	       m_delay_outputs[10] + m_delay_outputs[11] + m_delay_outputs[12] + m_delay_outputs[13] + m_delay_outputs[14] +
	       m_delay_outputs[15];
}
