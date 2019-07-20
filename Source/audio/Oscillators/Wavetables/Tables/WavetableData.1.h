#include "../../../OdinConstants.h"

const float (*getWavetableData())[NUMBER_OF_WAVETABLES][SUBTABLES_PER_WAVETABLE][WAVETABLE_LENGTH] ;
const float * getOneSubTable(int p_wavetable, int p_subtable);



//const float * getOneSubTable(int p_wavetable, int p_subtable){
//    return wavetable_data[p_wavetable][p_subtable];
//}
