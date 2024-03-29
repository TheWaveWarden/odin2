/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
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

// The implementation for this ZitaReverb module is taken from zita-rev1 with some adjustments. See
// https://github.com/royvegard/zita-rev1 for the original code.

#include "ZitaReverb.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// -----------------------------------------------------------------------

Diff1::Diff1(void) : _size(0), _line(0) {
}

Diff1::~Diff1(void) {
	fini();
}

void Diff1::reset() {
	memset(_line, 0, _size * sizeof(float));
	_i = 0;
}

void Diff1::init(int size, float c) {
	//deallocate if needed
	if (_size != 0)
		delete[] _line;

	_size = size;
	_line = new float[size];
	if (_line) {
		memset(_line, 0, size * sizeof(float));
	}
	_i = 0;
	_c = c;
}

void Diff1::fini(void) {
	//deallocate if needed
	if (_size != 0)
		delete[] _line;

	_size = 0;
	_line = 0;
}

void Filt1::reset() {
	_slo = 0;
	_shi = 0;
}

// -----------------------------------------------------------------------

RevDelay::RevDelay(void) : _size(0), _line(0) {
}

RevDelay::~RevDelay(void) {
	fini();
}

void RevDelay::init(int size) {
	//deallocate if needed
	if (_size != 0)
		delete[] _line;

	_size = size;
	_line = new float[size];
	memset(_line, 0, size * sizeof(float));
	_i = 0;
}

void RevDelay::reset() {
	if (_size != 0) {
		memset(_line, 0, _size * sizeof(float));
	}
	_i = 0;
}

void RevDelay::fini(void) {
	delete[] _line;
	_size = 0;
	_line = 0;
}

// -----------------------------------------------------------------------

Vdelay::Vdelay(void) : _size(0), _line(0) {
}

Vdelay::~Vdelay(void) {
	fini();
}

void Vdelay::init(int size) {
	//deallocate if needed
	if (_size != 0)
		delete[] _line;

	_size = size;
	_line = new float[size];
	memset(_line, 0, size * sizeof(float));
	_ir = 0;
	_iw = 0;
}

void Vdelay::fini(void) {
	//deallocate if needed
	if (_size != 0)
		delete[] _line;

	_size = 0;
	_line = 0;
}

void Vdelay::reset() {
	memset(_line, 0, _size * sizeof(float));
}

void Vdelay::set_delay(int del) {
	_ir = _iw - del;
	if (_ir < 0)
		_ir += _size;
}

// -----------------------------------------------------------------------

void Filt1::set_params(float del, float tmf, float tlo, float wlo, float thi, float chi) {
	float g, t;

	_gmf = powf(0.001f, del / tmf);
	_glo = powf(0.001f, del / tlo) / _gmf - 1.0f;
	_wlo = wlo;
	g    = powf(0.001f, del / thi) / _gmf;
	t    = (1 - g * g) / (2 * g * g * chi);
	_whi = (sqrtf(1 + 4 * t) - 1) / (2 * t);
}

// -----------------------------------------------------------------------

ZitaReverb::ZitaReverb(void) {
	_tdiff1 = {20346e-6f, 24421e-6f, 31604e-6f, 27333e-6f, 22904e-6f, 29291e-6f, 13458e-6f, 19123e-6f};
	_tdelay = {153129e-6f, 210389e-6f, 127837e-6f, 256891e-6f, 174713e-6f, 192303e-6f, 125000e-6f, 219991e-6f};

	//todo find good eq values
	_pareq[0].setQ(REVERB_EQ_Q_DEFAULT);
	_pareq[1].setQ(REVERB_EQ_Q_DEFAULT);

	_pareq[0].setFreq(1000.f);
	_pareq[1].setFreq(1000.f);

	//set default vals
	_ipdel = 0.04f;
	_xover = 200.0f;
	_rtlow = 3.0f;
	_rtmid = 2.0f;
	_fdamp = 3e3f;
	_opmix = 0.25f;
	_rgxyz = 0.0f;
}

ZitaReverb::~ZitaReverb(void) {
	fini();
}

void ZitaReverb::setSampleRate(float fsamp) {
	int i, k1, k2;

	_fsamp = fsamp;
	// _cntA1 = 1;
	// _cntA2 = 0;
	// _cntB1 = 1;
	// _cntB2 = 0;
	// _cntC1 = 1;
	// _cntC2 = 0;
	_Adirty = _Bdirty = _Cdirty = true;

	_g0 = _d0 = 0;
	_g1 = _d1 = 0;

	//TODO MEMORY LEAK!!!!
	_vdelay0.init((int)(0.1f * _fsamp));
	_vdelay1.init((int)(0.1f * _fsamp));
	for (i = 0; i < 8; i++) {
		k1 = (int)(floorf(_tdiff1[i] * _fsamp + 0.5f));
		k2 = (int)(floorf(_tdelay[i] * _fsamp + 0.5f));
		_diff1[i].init(k1, (i & 1) ? -0.6f : 0.6f);
		_delay[i].init(k2 - k1);
	}

	_pareq[0].setSampleRate(fsamp);
	_pareq[1].setSampleRate(fsamp);

	//_pareq2.setfsamp(fsamp);
}

void ZitaReverb::fini(void) {
	for (int i = 0; i < 8; i++)
		_delay[i].fini();
}

void ZitaReverb::prepare() {
	//int a, b, c,
	int i, k;
	float t0, t1, wlo, chi;

	// a   = _cntA1;
	// b   = _cntB1;
	// c   = _cntC1;
	_d0 = _d1 = 0;

	if (_Adirty) {
		k = (int)(floorf((_ipdel - 0.020f) * _fsamp + 0.5f));
		_vdelay0.set_delay(k);
		_vdelay1.set_delay(k);
		//_cntA2 = _cntA1;
		_Adirty = false;
	}

	if (_Bdirty) {
		wlo = 6.2832f * _xover / _fsamp;
		if (_fdamp > 0.49f * _fsamp)
			chi = 2;
		else
			chi = 1 - cosf(6.2832f * _fdamp / _fsamp);
		for (i = 0; i < 8; i++) {
			_filt1[i].set_params(_tdelay[i], _rtmid, _rtlow, wlo, 0.5f * _rtmid, chi);
		}
		//_cntB2 = _cntB1;
		_Bdirty = false;
	}

	if (_Cdirty) {
		// t0     = (1 - _opmix) * (1 + _opmix);
		// t1     = 0.7f * _opmix * (2 - _opmix) / sqrtf(_rtmid);
		// _d0    = t0 - _g0;
		// _d1    = t1 - _g1;
		// _cntC2 = _cntC1;
		_g0     = (1.f - _opmix) * (1.f - _opmix);
		_g1     = 1.f - _g0;
		_Cdirty = false;
	}

	//_pareq1.prepare();
	//_pareq2.prepare(nfram);

	// DBG_VAR(_g0);
	// DBG_VAR(_g1);
	// DBG_VAR(_d0);
	// DBG_VAR(_d1);
	// DBG_VAR(t0);
	// DBG_VAR(t1);

	// _d0 = 0;
	// _d1 = 0;
	// _g0 = 0.75;
	// _g1 = 0.25;
	// set_delay(40);
	// set_xover(2);
	// set_rtlow(3);
	// set_rtmid(2);
	// set_fdamp(6);
	// set_opmix(0.5);
	// //set_rgxyz();
	// set_eq1(200, 0);
}

void ZitaReverb::process(float input[2]) {

	int i, n;
	float *p0, *p1;
	float *q0, *q1, *q2, *q3;
	float t, g, x0, x1, x2, x3, x4, x5, x6, x7;
	float out[2];

	//g = sqrtf(0.125f);
	g = 0.35355f;

	// p0 = inp[0];
	// p1 = inp[1];
	// q0 = out[0];
	// q1 = out[1];
	// q2 = out[2];
	// q3 = out[3];

	//loop over samples -> sample based from here
	//p0[i] p1[i] is left / right input
	//AMBISONIC: four channel -> _ambis switches can be ignored
	//4 channel output??
	//q0[i]
	_vdelay0.write(input[0]);
	_vdelay1.write(input[1]);

	t  = 0.3f * _vdelay0.read();
	x0 = _diff1[0].process(_delay[0].read() + t);
	x1 = _diff1[1].process(_delay[1].read() + t);
	x2 = _diff1[2].process(_delay[2].read() - t);
	x3 = _diff1[3].process(_delay[3].read() - t);
	t  = 0.3f * _vdelay1.read();
	x4 = _diff1[4].process(_delay[4].read() + t);
	x5 = _diff1[5].process(_delay[5].read() + t);
	x6 = _diff1[6].process(_delay[6].read() - t);
	x7 = _diff1[7].process(_delay[7].read() - t);

	t = x0 - x1;
	x0 += x1;
	x1 = t;
	t  = x2 - x3;
	x2 += x3;
	x3 = t;
	t  = x4 - x5;
	x4 += x5;
	x5 = t;
	t  = x6 - x7;
	x6 += x7;
	x7 = t;
	t  = x0 - x2;
	x0 += x2;
	x2 = t;
	t  = x1 - x3;
	x1 += x3;
	x3 = t;
	t  = x4 - x6;
	x4 += x6;
	x6 = t;
	t  = x5 - x7;
	x5 += x7;
	x7 = t;
	t  = x0 - x4;
	x0 += x4;
	x4 = t;
	t  = x1 - x5;
	x1 += x5;
	x5 = t;
	t  = x2 - x6;
	x2 += x6;
	x6 = t;
	t  = x3 - x7;
	x3 += x7;
	x7 = t;

	//_g1 += _d1;
	out[0] = _g1 * (x1 + x2);
	out[1] = _g1 * (x1 - x2);

	_delay[0].write(_filt1[0].process(g * x0));
	_delay[1].write(_filt1[1].process(g * x1));
	_delay[2].write(_filt1[2].process(g * x2));
	_delay[3].write(_filt1[3].process(g * x3));
	_delay[4].write(_filt1[4].process(g * x4));
	_delay[5].write(_filt1[5].process(g * x5));
	_delay[6].write(_filt1[6].process(g * x6));
	_delay[7].write(_filt1[7].process(g * x7));

	out[0] = _pareq[0].doFilter(out[0]);
	out[1] = _pareq[1].doFilter(out[1]);

	//_pareq2.process(out);
	//for (i = 0; i < nfram; i++) {
	//_g0 += _d0;
	out[0] += _g0 * input[0];
	out[1] += _g0 * input[1];
	//}

	// DBG("==");
	// DBG_VAR(_g0);
	// DBG_VAR(_g1);
	// DBG_VAR(_d0);
	// DBG_VAR(_d1);

	input[0] = out[0];
	input[1] = out[1];
}

void ZitaReverb::set_delay(float v) {
	_ipdel = v;
	//_cntA1++;
	_Adirty = true;
	prepare();
}

void ZitaReverb::set_xover(float v) {
	_xover = v;
	//_cntB1++;
	_Bdirty = true;
	prepare();
}

void ZitaReverb::set_rtlow(float v) {
	_rtlow = v;
	//_cntB1++;
	_Cdirty = true;
	prepare();
}

void ZitaReverb::set_rtmid(float v) {
	_rtmid = v;
	//_cntB1++;
	//_cntC1++;
	_Bdirty = true;
	_Cdirty = true;
	prepare();
}

void ZitaReverb::set_fdamp(float v) {
	_fdamp = v;
	//_cntB1++;
	_Bdirty = true;
	prepare();
}

void ZitaReverb::set_opmix(float v) {
	_opmix = v;
	//_cntC1++;
	_Cdirty = true;
	prepare();
}

void ZitaReverb::set_rgxyz(float v) {
	_rgxyz = v;
	//_cntC1++;
	_Cdirty = true;
	prepare();
}

void ZitaReverb::set_eq1(float f, float g) {
	//_eq_gain = g;
	//_eq_freq = f;
	_pareq[0].setFreq(f);
	_pareq[1].setFreq(f);
	_pareq[0].setQ(g);
	_pareq[1].setQ(g);
	prepare();
}

void ZitaReverb::set_eq1_gain(float g) {
	//_eq_gain = g;
	_pareq[0].setGain(g);
	_pareq[1].setGain(g);
	prepare();
}

void ZitaReverb::set_eq1_freq(float f) {
	//_eq_freq = f;
	_pareq[0].setFreq(f);
	_pareq[1].setFreq(f);
	prepare();
}

void ZitaReverb::set_ducking(float d) {
}

// void ZitaReverb::set_eq2(float f, float g) {
// 	_pareq2.setparam(f, g);
// }

// -----------------------------------------------------------------------
void Diff1::dump(std::string name) {
	DBG("===");
	DBG("Dumping Diff1 " << name);
	DBG_VAR(_i);
	DBG_VAR(_c);
	DBG_VAR(_size);
}
void RevDelay::dump(std::string name) {
	DBG("===");
	DBG("Dumping Delay " << name);
	//DBG_VAR(_i);
	DBG_VAR(_size);
}
void Vdelay::dump(std::string name) {
	DBG("===");
	DBG("Dumping VDelay " << name);
	DBG_VAR(_ir);
	DBG_VAR(_iw);
	DBG_VAR(_size);
}
void Filt1::dump(std::string name) {
	DBG("===");
	DBG("Dumping Fil1 " << name);
	DBG_VAR(_gmf);
	DBG_VAR(_glo);
	DBG_VAR(_wlo);
	DBG_VAR(_whi);
	DBG_VAR(_slo);
	DBG_VAR(_shi);
}
void ZitaReverb::dump(std::string name) {
	//DBG("===================================");
	//DBG("Dumping Reverb " << name);
	// DBG_VAR(_fsamp);
	// //DBG_VAR(_cntA1);
	// //DBG_VAR(_cntB1);
	// //DBG_VAR(_cntC1);
	// //DBG_VAR(_cntA2);
	// //DBG_VAR(_cntB2);
	// //DBG_VAR(_cntC2);
	// DBG_VAR(_ipdel);
	// DBG_VAR(_xover);
	// DBG_VAR(_rtlow);
	// DBG_VAR(_rtmid);
	// DBG_VAR(_fdamp);
	// DBG_VAR(_opmix);
	// DBG_VAR(_rgxyz);
	// DBG_VAR(_g0);
	// DBG_VAR(_d0);
	// DBG_VAR(_g1);
	// DBG_VAR(_d1);

	// _vdelay0.dump("_vdelay0");
	// _vdelay1.dump("_vdelay1");
	// _diff1[0].dump("_diff1[0]");
	// _diff1[1].dump("_diff1[1]");
	// _diff1[2].dump("_diff1[2]");
	// _diff1[3].dump("_diff1[3]");
	// _diff1[4].dump("_diff1[4]");
	// _diff1[5].dump("_diff1[5]");
	// _diff1[6].dump("_diff1[6]");
	// _diff1[7].dump("_diff1[7]");

	// _filt1[0].dump("_filt1[0]");
	// _filt1[1].dump("_filt1[1]");
	// _filt1[2].dump("_filt1[2]");
	// _filt1[3].dump("_filt1[3]");
	// _filt1[4].dump("_filt1[4]");
	// _filt1[5].dump("_filt1[5]");
	// _filt1[6].dump("_filt1[6]");
	// _filt1[7].dump("_filt1[7]");

	// _delay[0].dump("_delay[0]");
	// _delay[1].dump("_delay[1]");
	// _delay[2].dump("_delay[2]");
	// _delay[3].dump("_delay[3]");
	// _delay[4].dump("_delay[4]");
	// _delay[5].dump("_delay[5]");
	// _delay[6].dump("_delay[6]");
	// _delay[7].dump("_delay[7]");

	//_pareq1.dump("EQ");

	//DBG("===================================");
}

void ZitaReverb::reset() {
	DBG("Zita reset()");

	_vdelay0.reset();
	_vdelay1.reset();

	for (int i = 0; i < 8; ++i) {
		_diff1[i].reset();
		_filt1[i].reset();
		_delay[i].reset();
	}
}