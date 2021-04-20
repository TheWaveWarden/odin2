#pragma once
#include <JuceHeader.h>


class IntegerDelayLine {
public:
	IntegerDelayLine();
	~IntegerDelayLine();

	void setDelayLength(int p_length);
	void deallocateDelayBuffer();
	float doDelay(float p_input);

private:
	float *m_buffer;
	int m_length = -1;
  size_t m_write_position = 0;
};
