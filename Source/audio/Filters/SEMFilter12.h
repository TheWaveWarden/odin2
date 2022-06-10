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

#pragma once
#include "OdinFilterBase.h"
#include "../../GlobalIncludes.h"
class SEMFilter12 :
	public OdinFilterBase
{
public:
	SEMFilter12();
	~SEMFilter12();
	void reset() override;
	void update() override;
	double doFilter(double xn) override;
	void setResControl(double res) override;

	double m_transition;
	double m_resonance;
	double m_resonance_modded;
	double m_alpha;
	double m_alpha_0;
	double m_rho;

	void setTransitionModPointer(float* p_pointer){
		m_transition_mod = p_pointer;
	}
	void setSampleRate(double p_sr) override;

protected:
	double m_last_freq_modded = -1;

	float* m_transition_mod = &m_mod_dummy_zero;

	double m_z_1;
	double m_z_2;
};

