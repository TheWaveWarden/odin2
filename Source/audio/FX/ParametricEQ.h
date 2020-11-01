/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

// The implementation for this EQ module is taken from zita-rev1 with some adjustments. See
// https://github.com/royvegard/zita-rev1 for the original code.

#define DBG_VAR(var) DBG(#var << ": " << var)

#ifndef __PAREQ_H
#define __PAREQ_H

#include <math.h>
#include <stdint.h>
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../GlobalIncludes.h"

#define REV_EQ_DEFAULT_FREQ 1000
#define REV_EQ_DEFAULT_GAIN_DB 0         //these two are linked
#define REV_EQ_DEFUALT_GAIN_MULTIPLIER 1 //these two are linked
#define EQ_MAXCH 2

class ParametricEQ {
public:
	ParametricEQ(void);
	~ParametricEQ(void);

	void setfsamp(float fsamp);
	void setparam(float f, float g) {
		_f0 = f;
		_g0 = powf(10.0f, 0.05f * g);
		_touch0++;
	}
	void reset(void);
	void prepare();
	void process(float data[]) {
		if (_state != BYPASS)
			process1(data);
		//else
		//DBG("BYPASS");
	}
	void dump(std::string name);

private:
	enum { BYPASS, STATIC, SMOOTH };

	void calcpar1(int nsamp, float g, float f);
	void process1(float data[]);

	volatile int16_t _touch0;
	volatile int16_t _touch1;
	bool _bypass;
	int _state;
	float _fsamp;

	float _g0, _g1;
	float _f0, _f1;
	float _c1, _dc1;
	float _c2, _dc2;
	float _gg, _dgg;

	float _z1[EQ_MAXCH];
	float _z2[EQ_MAXCH];
};

#endif
