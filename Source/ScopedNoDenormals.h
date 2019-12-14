#include "xmmintrin.h"

class ScopedNoDenormals
{
public:
    ScopedNoDenormals()
    {

        //There is also C99 way of doing this, but its not widely supported: fesetenv(...)

        oldMXCSR = _mm_getcsr(); /*read the old MXCSR setting */ \
        int newMXCSR = oldMXCSR | 0x8040; /* set DAZ and FZ bits */ \
        _mm_setcsr( newMXCSR); /*write the new MXCSR setting to the MXCSR */
    };
    
    ~ScopedNoDenormals()
    {
        _mm_setcsr( oldMXCSR );
    };
    
    int oldMXCSR;
    
};