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

#include "ParametricEQ.h"
#include <math.h>
#include <string.h>

ParametricEQ::ParametricEQ(void) : _touch0(0), _touch1(0), _state(BYPASS), _g0(1), _g1(1), _f0(1e3f), _f1(1e3f) {
	setfsamp(0.0f);
}

ParametricEQ::~ParametricEQ(void) {
}

void ParametricEQ::setfsamp(float fsamp) {
	_fsamp = fsamp;
	reset();
}

void ParametricEQ::reset(void) {
	memset(_z1, 0, sizeof(float) * MAXCH);
	memset(_z2, 0, sizeof(float) * MAXCH);
}

void ParametricEQ::prepare() {
	bool upd = false;
	float g, f;

	if (_touch1 != _touch0) {
		g = _g0;
		f = _f0;
		if (g != _g1) {
			upd = true;
			if (g > 2 * _g1)
				_g1 *= 2;
			else if (_g1 > 2 * g)
				_g1 /= 2;
			else
				_g1 = g;
		}
		if (f != _f1) {
			upd = true;
			if (f > 2 * _f1)
				_f1 *= 2;
			else if (_f1 > 2 * f)
				_f1 /= 2;
			else
				_f1 = f;
		}
		if (upd) {
			if ((_state == BYPASS) && (_g1 == 1)) {
				calcpar1(0, _g1, _f1);
			} else {
				_state = SMOOTH;
				calcpar1(1, _g1, _f1);
			}
		} else {
			_touch1 = _touch0;
			if (fabs(_g1 - 1) < 0.001f) {
				_state = BYPASS;
				reset();
			} else {
				_state = STATIC;
			}
		}
	}
}

void ParametricEQ::calcpar1(int nsamp, float g, float f) {
	float b, c1, c2, gg;

	f *= float(M_PI) / _fsamp;
	b  = 2 * f / sqrtf(g);
	gg = 0.5f * (g - 1);
	c1 = -cosf(2 * f);
	c2 = (1 - b) / (1 + b);
	if (nsamp) {
		_dc1 = (c1 - _c1) / nsamp + 1e-30f;
		_dc2 = (c2 - _c2) / nsamp + 1e-30f;
		_dgg = (gg - _gg) / nsamp + 1e-30f;
	} else {
		_c1 = c1;
		_c2 = c2;
		_gg = gg;
	}
}

void ParametricEQ::process1(float data[]) {
	int i, j;
	float c1, c2, gg;
	float x, y, z1, z2;
	float *p;

	c1 = _c1;
	c2 = _c2;
	gg = _gg;
	if (_state == SMOOTH) {
		for (i = 0; i < 2 /*nchan*/; i++) {
			p  = &(data[i]);
			z1 = _z1[i];
			z2 = _z2[i];
			c1 = _c1;
			c2 = _c2;
			gg = _gg;
			//for (j = 0; j < nsamp; j++) {
			c1 += _dc1;
			c2 += _dc2;
			gg += _dgg;
			x    = *p;
			y    = x - c2 * z2;
			*p++ = x - gg * (z2 + c2 * y - x);
			y -= c1 * z1;
			z2 = z1 + c1 * y;
			z1 = y + 1e-20f;
			//}
			_z1[i] = z1;
			_z2[i] = z2;
		}
		_c1 = c1;
		_c2 = c2;
		_gg = gg;
	} else {
		for (i = 0; i < 2 /*nchan*/; i++) {
			p  = &(data[i]);
			z1 = _z1[i];
			z2 = _z2[i];
			//for (j = 0; j < nsamp; j++) {
			x    = *p;
			y    = x - c2 * z2;
			*p++ = x - gg * (z2 + c2 * y - x);
			y -= c1 * z1;
			z2 = z1 + c1 * y;
			z1 = y + 1e-20f;
			//}
			_z1[i] = z1;
			_z2[i] = z2;
		}
	}
}
