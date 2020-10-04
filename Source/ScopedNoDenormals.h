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

#include "xmmintrin.h"

namespace denormals {
class ScopedNoDenormals {
public:
	ScopedNoDenormals() {

		//There is also C99 way of doing this, but its not widely supported: fesetenv(...)

		oldMXCSR     = _mm_getcsr();      /*read the old MXCSR setting */
		int newMXCSR = oldMXCSR | 0x8040; /* set DAZ and FZ bits */
		_mm_setcsr(newMXCSR);             /*write the new MXCSR setting to the MXCSR */
	};

	~ScopedNoDenormals() {
		_mm_setcsr(oldMXCSR);
	};

	int oldMXCSR;
};
} // namespace denormals