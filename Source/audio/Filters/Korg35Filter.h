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

// The code for this filter is based on the diagrams and implementations in Will Pirkles book
// "Designing Software Synthesizer Plug-Ins in C++". Find out more on https://willpirkle.com

#pragma once
#include "OdinFilterBase.h"
#include "VAOnePoleFilter.h"

class Korg35Filter :
	public OdinFilterBase{
public:
	Korg35Filter();
	~Korg35Filter();

	void reset() override;
	void update() override;
	double doFilter(double xn) override;
	void setResControl(double res) override;

	inline void setFilterType(bool p_is_lowpass){
		m_is_lowpass = p_is_lowpass;//else HP
  		m_last_freq_modded = -1; // to signal recalculation of filter coeffs in update()
	}

	void setSampleRate(double p_sr) override;
	
protected:
    double m_last_freq_modded = -1;

	double m_k;
	double m_k_modded;
	double m_alpha;

	VAOnePoleFilter m_LPF1;
	VAOnePoleFilter m_LPF2;
	VAOnePoleFilter m_HPF1;
	VAOnePoleFilter m_HPF2;

	bool m_is_lowpass = true;//else HP
};

