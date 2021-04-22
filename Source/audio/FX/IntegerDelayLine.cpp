#include "IntegerDelayLine.h"

IntegerDelayLine::IntegerDelayLine() {
}

IntegerDelayLine::~IntegerDelayLine() {
	if (m_length != -1) {
		deallocateDelayBuffer();
	}
}

void IntegerDelayLine::setDelayLength(int p_length) {
	DBG("Set Delay length: " << p_length);
	if (m_length != -1) {
		deallocateDelayBuffer();
	}

	// 1 sample is needed bc of output -> input delay, so make delay line 1 sample shorter
	m_length = p_length - 1;

	//read is always one ahead, to avoid saving value in separate var before overwriting
	m_buffer        = new float[m_length + 1];
	m_write_position = 0;

	for (int i = 0; i < m_length + 1; ++i) {
		m_buffer[i] = 0.f;
	}
}

void IntegerDelayLine::deallocateDelayBuffer() {
	delete[] m_buffer;
}

float IntegerDelayLine::doDelay(float p_input) {
  jassert(m_length != -1);

  //we always read one sample in front of write position. otherwise we must save sample in extra var
	m_buffer[m_write_position] = p_input;
	if (m_write_position < m_length) {
		return m_buffer[++m_write_position];
	} else {
    m_write_position = 0;
    return m_buffer[0];
	}
}