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

#if __aarch64__
#else
#include "xmmintrin.h"
#endif

namespace denormals {
class ScopedNoDenormals {
public:
	ScopedNoDenormals() {

		//There is also C99 way of doing this, but its not widely supported: fesetenv(...)
#if __aarch64__
      uint64_t FPCR = 0;
      asm volatile("MRS %0, FPCR " : "=r"(FPCR));

      uint64_t FPCR_new = FPCR | (1ULL << 24);
      asm volatile("MSR FPCR, %0 " : : "r"(FPCR_new));

      priorS = FPCR;
#else
		oldMXCSR     = _mm_getcsr();      /*read the old MXCSR setting */
		int newMXCSR = oldMXCSR | 0x8040; /* set DAZ and FZ bits */
		_mm_setcsr(newMXCSR);             /*write the new MXCSR setting to the MXCSR */
#endif
	};

	~ScopedNoDenormals() {
#if __aarch64__
      asm volatile("MSR FPCR, %0 " : : "r"(priorS));
#else
		_mm_setcsr(oldMXCSR);
#endif
	};

#if __aarch64__
  uint64_t priorS;
#else
	int oldMXCSR;
#endif
};
} // namespace denormals