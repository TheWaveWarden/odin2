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

#include "BiquadFilter.h"
#include "../OdinConstants.h"

class BiquadAllpass : public BiquadFilter
{
public:
	BiquadAllpass();
	virtual ~BiquadAllpass();

	inline void setRadius(float p_radius){
		m_radius = p_radius;
	}

	void setFrequency(float p_frequency);


protected:

	float m_radius;
};

