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

#ifndef __REVERB_H
#define __REVERB_H

#define REVERB_EQ_Q_DEFAULT 0.9f

//#include "ParametricEQ.h"
#include "../Filters/BiquadEQ.h"
#include <string>
#include "../JuceLibraryCode/JuceHeader.h"

class Diff1 {
private:
	friend class ZitaReverb;

    void dump(std::string name);

	Diff1(void);
	~Diff1(void);

	void init(int size, float c);
	void fini(void);
	void reset();

	float process(float x) {
		float z = _line[_i];
		x -= _c * z;
		_line[_i] = x;
		if (++_i == _size)
			_i = 0;
		return z + _c * x;
	}

	int _i;
	float _c;
	int _size = 0;
	float *_line;
};

// -----------------------------------------------------------------------

class Filt1 {
private:
	friend class ZitaReverb;

    void dump(std::string name);

	Filt1(void) : _slo(0), _shi(0) {
	}
	~Filt1(void) {
	}

	void reset();

	void set_params(float del, float tmf, float tlo, float wlo, float thi, float chi);

	float process(float x) {
		_slo += _wlo * (x - _slo) + 1e-10f;
		x += _glo * _slo;
		_shi += _whi * (x - _shi);
		return _gmf * _shi;
	}
	float _gmf;
	float _glo;
	float _wlo;
	float _whi;
	float _slo;
	float _shi;
};

// -----------------------------------------------------------------------

class RevDelay {
private:
	friend class ZitaReverb;
    void dump(std::string name);

	RevDelay(void);
	~RevDelay(void);

	void init(int size);
	void fini(void);
	void reset();

	float read(void) {
		return _line[_i];
	}

	void write(float x) {
		_line[_i++] = x;
		if (_i == _size)
			_i = 0;
	}

	int _i;
	int _size = 0;
	float *_line;
};

// -----------------------------------------------------------------------


/**
 * A simple, non-fractional delay line without feedback
 */
class Vdelay {
private:
	friend class ZitaReverb;
    void dump(std::string name);

	Vdelay(void);
	~Vdelay(void);

	void init(int size);
	void fini(void);
	void reset();
	void set_delay(int del);

	float read(void) {
		float x = _line[_ir++];
		if (_ir == _size)
			_ir = 0;
		return x;
	}

	void write(float x) {
		_line[_iw++] = x;
		if (_iw == _size)
			_iw = 0;
	}

	int _ir; // read index
	int _iw; // write index
	int _size = 0; //delay line size
	float *_line;
};

// -----------------------------------------------------------------------

class ZitaReverb {
public:
	ZitaReverb(void);
	~ZitaReverb(void);
    void dump(std::string name);

	void setSampleRate(float fsamp);
	void fini(void);

	void prepare();
	float* process(float input[2]);
	void reset();

	void set_delay(float v);  //v in seconds
	void set_xover(float v);  // set low freq, v in Hz  
	void set_rtlow(float v);  //v in seconds (RT60: time for signal to attenuate 60dB)
	void set_rtmid(float v);  //v in seconds (RT60: time for signal to attenuate 60dB)
	void set_fdamp(float v);  //v in Hz
	void set_opmix(float v);  //drywet [0,1]
	void set_rgxyz(float v);
	void set_eq1(float f, float g);
	void set_eq1_gain(float g);
	void set_eq1_freq(float f);
	void set_ducking(float d); //unused?

private:
	float _fsamp;

	Vdelay _vdelay0;
	Vdelay _vdelay1;
	Diff1 _diff1[8];
	Filt1 _filt1[8];
	RevDelay _delay[8];

	//basically a "set-dirty" mechanism ?
	// volatile int _cntA1;
	// volatile int _cntB1;
	// volatile int _cntC1;
	// int _cntA2;
	// int _cntB2;
	// int _cntC2;
	bool _Adirty;
	bool _Bdirty;
	bool _Cdirty;

	float _ipdel;
	float _xover;
	float _rtlow;
	float _rtmid;
	float _fdamp;
	float _opmix;
	float _rgxyz;

	float _g0, _d0;
	float _g1, _d1;

	//float _eq_gain = REV_EQ_DEFAULT_GAIN_DB;
	//float _eq_freq = REV_EQ_DEFAULT_FREQ;

	BiquadEQ _pareq[2];
	//ParametricEQ _pareq2;

	float _tdiff1[8];
	float _tdelay[8];
};

// -----------------------------------------------------------------------

#endif
