
//====================
//======= LFOS =======
//====================

#include "Wavetables/Coefficients/LFOPyramid4.h"
#include "Wavetables/Coefficients/LFOPyramid6.h"
#include "Wavetables/Coefficients/LFOPyramid8.h"
#include "Wavetables/Coefficients/LFOPyramid12.h"
#include "Wavetables/Coefficients/LFOStair3.h"
#include "Wavetables/Coefficients/LFOStair4.h"
#include "Wavetables/Coefficients/LFOStair6.h"
#include "Wavetables/Coefficients/LFOStair8.h"
#include "Wavetables/Coefficients/LFOStair12.h"
#include "Wavetables/Coefficients/LFOSquare50.h"
#include "Wavetables/Coefficients/LFOSquare25.h"
#include "Wavetables/Coefficients/LFOSquare12.h"
#include "Wavetables/Coefficients/LFOSpike.h"




#include "Wavetables/Coefficients/Sine.h"                //0
#include "Wavetables/Coefficients/ChiptuneTriangle.h"    //1
#include "Wavetables/Coefficients/ChiptuneSquare50.h"    //2
#include "Wavetables/Coefficients/ChiptuneSquare25.h"    //3
#include "Wavetables/Coefficients/ChiptuneSquare12_5.h"  //4
#include "Wavetables/Coefficients/ChiptuneCustom1.h"     //5
#include "Wavetables/Coefficients/ChiptuneCustom2.h"     //6
#include "Wavetables/Coefficients/ChiptuneCustom3.h"     //7
#include "Wavetables/Coefficients/ChiptuneCustom4.h"     //8
#include "Wavetables/Coefficients/ChiptuneCustom5.h"     //9
#include "Wavetables/Coefficients/ChiptuneCustom6.h"     //10
#include "Wavetables/Coefficients/Saw.h"                 //11
#include "Wavetables/Coefficients/Triangle.h"            //12
#include "Wavetables/Coefficients/FatSaw.h"                //13

#include "Wavetables/Coefficients/Additive1.h"//14
#include "Wavetables/Coefficients/Additive2.h"//15
#include "Wavetables/Coefficients/Additive3.h"//16
#include "Wavetables/Coefficients/Additive4.h"//17
#include "Wavetables/Coefficients/Additive5.h"//18
#include "Wavetables/Coefficients/Additive6.h"//19
#include "Wavetables/Coefficients/Additive7.h"//20
#include "Wavetables/Coefficients/Additive8.h"//21
#include "Wavetables/Coefficients/Additive9.h"//22
#include "Wavetables/Coefficients/Additive10.h"//23
#include "Wavetables/Coefficients/Additive11.h"//24
#include "Wavetables/Coefficients/Additive12.h"//25
#include "Wavetables/Coefficients/Additive13.h"//26
#include "Wavetables/Coefficients/Additive14.h"//27
#include "Wavetables/Coefficients/Additive15.h"//28
#include "Wavetables/Coefficients/Additive16.h"//29

#include "Wavetables/Coefficients/Harmonics1.h"//30
#include "Wavetables/Coefficients/Harmonics2.h"//31
#include "Wavetables/Coefficients/Harmonics3.h"//32
#include "Wavetables/Coefficients/Harmonics4.h"//33
#include "Wavetables/Coefficients/Harmonics5.h"//34
#include "Wavetables/Coefficients/Harmonics6.h"//35
#include "Wavetables/Coefficients/Harmonics7.h"//36
#include "Wavetables/Coefficients/Harmonics8.h"//37 this is overtone 1
#include "Wavetables/Coefficients/Harmonics9.h"//38
#include "Wavetables/Coefficients/Harmonics10.h"//39
#include "Wavetables/Coefficients/Harmonics11.h"//40
#include "Wavetables/Coefficients/Harmonics12.h"//41
#include "Wavetables/Coefficients/Harmonics13.h"//42
#include "Wavetables/Coefficients/Harmonics14.h"//43
#include "Wavetables/Coefficients/Harmonics15.h"//44
#include "Wavetables/Coefficients/Harmonics16.h"//45 this is overtone 8

#include "Wavetables/Coefficients/Organ1.h"//46
#include "Wavetables/Coefficients/Organ2.h"//47
#include "Wavetables/Coefficients/Organ3.h"//48
#include "Wavetables/Coefficients/Organ4.h"//49

#include "Wavetables/Coefficients/BrokenSine1.h"//50
#include "Wavetables/Coefficients/BrokenSine2.h"//51
#include "Wavetables/Coefficients/BrokenSine3.h"//52
#include "Wavetables/Coefficients/BrokenSine4.h"//53

#include "Wavetables/Coefficients/Skyline1.h"//54
#include "Wavetables/Coefficients/Skyline2.h"//55
#include "Wavetables/Coefficients/Skyline3.h"//56
#include "Wavetables/Coefficients/Skyline4.h"//57

#include "Wavetables/Coefficients/Soft1.h"//58
#include "Wavetables/Coefficients/Soft2.h"//59
#include "Wavetables/Coefficients/Soft3.h"//60
#include "Wavetables/Coefficients/Soft4.h"//61

#include "Wavetables/Coefficients/MultiSaw1.h"//62
#include "Wavetables/Coefficients/MultiSaw2.h"//63
#include "Wavetables/Coefficients/MultiSaw3.h"//64
#include "Wavetables/Coefficients/MultiSaw4.h"//65

#include "Wavetables/Coefficients/Rectangular1.h"//66
#include "Wavetables/Coefficients/Rectangular2.h"//67
#include "Wavetables/Coefficients/Rectangular3.h"//68
#include "Wavetables/Coefficients/Rectangular4.h"//69

#include "Wavetables/Coefficients/Violin1.h"//70
#include "Wavetables/Coefficients/Violin2.h"//71
#include "Wavetables/Coefficients/Cello1.h" //72
#include "Wavetables/Coefficients/Cello2.h" //73

#include "Wavetables/Coefficients/Piano1.h" //74
#include "Wavetables/Coefficients/Piano2.h" //75
#include "Wavetables/Coefficients/Piano3.h" //76
#include "Wavetables/Coefficients/Piano4.h" //77

#include "Wavetables/Coefficients/Oboe1.h" //78
#include "Wavetables/Coefficients/Oboe2.h" //79
#include "Wavetables/Coefficients/Oboe3.h" //80
#include "Wavetables/Coefficients/Oboe4.h" //81


#include "Wavetables/Coefficients/Flute1.h" //82
#include "Wavetables/Coefficients/Flute2.h" //83
#include "Wavetables/Coefficients/Flute3.h" //84
#include "Wavetables/Coefficients/Flute4.h" //85


#include "Wavetables/Coefficients/Trumpet1.h" //86
#include "Wavetables/Coefficients/Trumpet2.h" //87
#include "Wavetables/Coefficients/Trumpet3.h" //88
#include "Wavetables/Coefficients/Trumpet4.h" //89


#include "Wavetables/Coefficients/MusicBox1.h" //90
#include "Wavetables/Coefficients/MusicBox2.h" //91
#include "Wavetables/Coefficients/MusicBox3.h" //92
#include "Wavetables/Coefficients/MusicBox4.h" //93

#include "Wavetables/Coefficients/CTDiverging.h"//94
#include "Wavetables/Coefficients/CTHigh.h"//95
#include "Wavetables/Coefficients/CTHigh2.h"//96
#include "Wavetables/Coefficients/CTHigh3.h"//97
#include "Wavetables/Coefficients/CTRich.h"//98
#include "Wavetables/Coefficients/CTSaw.h"//99
#include "Wavetables/Coefficients/CTSine.h"//100
#include "Wavetables/Coefficients/CTSoftTune.h"//101


#include "Wavetables/Coefficients/Sparse1.h" //102
#include "Wavetables/Coefficients/Sparse2.h" //103
#include "Wavetables/Coefficients/Sparse3.h" //104
#include "Wavetables/Coefficients/Sparse4.h" //105

m_highest_loaded_table = 105;
#include "Wavetables/Coefficients/SawMutated1.h" //106
#include "Wavetables/Coefficients/SawMutated2.h" //107
#include "Wavetables/Coefficients/SawMutated3.h" //108



#include "Wavetables/Coefficients/FatSawMutated1.h" //109
#include "Wavetables/Coefficients/FatSawMutated2.h" //110
#include "Wavetables/Coefficients/FatSawMutated3.h" //111
#include "Wavetables/Coefficients/FatSawMutated4.h" //112
#include "Wavetables/Coefficients/FatSawMutated5.h" //113
#include "Wavetables/Coefficients/FatSawMutated6.h" //114
#include "Wavetables/Coefficients/FatSawMutated7.h" //115
#include "Wavetables/Coefficients/FatSawMutated8.h" //116

#include "Wavetables/Coefficients/ChiptuneSquare50Mutated1.h" //117
#include "Wavetables/Coefficients/ChiptuneSquare50Mutated2.h" //118
#include "Wavetables/Coefficients/ChiptuneSquare50Mutated3.h" //119
#include "Wavetables/Coefficients/ChiptuneSquare50Mutated4.h" //120
#include "Wavetables/Coefficients/ChiptuneSquare50Mutated5.h" //121
#include "Wavetables/Coefficients/ChiptuneSquare50Mutated6.h" //122
#include "Wavetables/Coefficients/ChiptuneSquare50Mutated7.h" //123
#include "Wavetables/Coefficients/ChiptuneSquare50Mutated8.h" //124

#include "Wavetables/Coefficients/SawMutated13.h" //125
#include "Wavetables/Coefficients/SawMutated14.h" //126
#include "Wavetables/Coefficients/SawMutated15.h" //127
#include "Wavetables/Coefficients/SawMutated16.h" //128
#include "Wavetables/Coefficients/SawMutated17.h" //129
#include "Wavetables/Coefficients/SawMutated18.h" //130
#include "Wavetables/Coefficients/SawMutated19.h" //131
#include "Wavetables/Coefficients/SawMutated20.h" //132

#include "Wavetables/Coefficients/Perlin1.h" //133
#include "Wavetables/Coefficients/Perlin2.h" //134
#include "Wavetables/Coefficients/Perlin3.h" //135
#include "Wavetables/Coefficients/Perlin4.h" //136

#include "Wavetables/Coefficients/Perlin5.h" //137
#include "Wavetables/Coefficients/Perlin6.h" //138
#include "Wavetables/Coefficients/Perlin7.h" //139
#include "Wavetables/Coefficients/Perlin8.h" //140
#include "Wavetables/Coefficients/Perlin9.h" //141
#include "Wavetables/Coefficients/Perlin10.h" //142
#include "Wavetables/Coefficients/Perlin11.h" //143
#include "Wavetables/Coefficients/Perlin12.h" //144


#include "Wavetables/Coefficients/AKWF_hvoice_0002.h" //145
#include "Wavetables/Coefficients/AKWF_hvoice_0010.h" //146
#include "Wavetables/Coefficients/AKWF_hvoice_0014.h" //147
#include "Wavetables/Coefficients/AKWF_hvoice_0019.h" //148
#include "Wavetables/Coefficients/AKWF_hvoice_0020.h" //149
#include "Wavetables/Coefficients/AKWF_hvoice_0021.h" //150
#include "Wavetables/Coefficients/AKWF_hvoice_0029.h" //151
#include "Wavetables/Coefficients/AKWF_hvoice_0032.h" //152
#include "Wavetables/Coefficients/AKWF_hvoice_0037.h" //153
#include "Wavetables/Coefficients/AKWF_hvoice_0041.h" //154
#include "Wavetables/Coefficients/AKWF_hvoice_0047.h" //155
#include "Wavetables/Coefficients/AKWF_hvoice_0049.h" //156
#include "Wavetables/Coefficients/AKWF_hvoice_0056.h" //157
#include "Wavetables/Coefficients/AKWF_hvoice_0064.h" //158
#include "Wavetables/Coefficients/AKWF_hvoice_0071.h" //159
#include "Wavetables/Coefficients/AKWF_hvoice_0093.h" //160
#include "Wavetables/Coefficients/AKWF_fmsynth_0011.h" //161
#include "Wavetables/Coefficients/AKWF_fmsynth_0032.h" //162
#include "Wavetables/Coefficients/AKWF_fmsynth_0034.h" //163
#include "Wavetables/Coefficients/AKWF_fmsynth_0081.h" //164
#include "Wavetables/Coefficients/AKWF_birds_0010.h" //165
#include "Wavetables/Coefficients/AKWF_birds_0011.h" //166
#include "Wavetables/Coefficients/AKWF_birds_0014.h" //167
#include "Wavetables/Coefficients/AKWF_birds_0004.h" //168
#include "Wavetables/Coefficients/AKWF_bitreduced_0002.h" //169
#include "Wavetables/Coefficients/AKWF_bitreduced_0003.h" //170
#include "Wavetables/Coefficients/AKWF_bitreduced_0006.h" //171
#include "Wavetables/Coefficients/AKWF_bitreduced_0011.h" //172
#include "Wavetables/Coefficients/AKWF_raw_0027.h" //173
#include "Wavetables/Coefficients/AKWF_raw_0025.h" //174
#include "Wavetables/Coefficients/AKWF_raw_0029.h" //175
#include "Wavetables/Coefficients/AKWF_raw_0034.h" //176
