


#define MAP(inp, out)                                                          \
  case inp:                                                                    \
    return out;                                                                \
    break;

int WavetableOsc1D::wavetableMappingFM(int p_input) {
  if (p_input == 1) { // extra sine for FM/PM
    return 0;
  }
  // apart from the extra sine, vec/fm mappings are identical
  return wavetableMappingVector(p_input);
}

//FOR FUTURE ME:

//there are multiple mapping systems:

//1. the one in WavetableOsc2D.cpp:
//needs special mapping because of 2D layout, is based on finding strings

//2. this one
//needed because dropdowns need to be monotonely increasing
//takes value from dropdown and gives according WT_NR (from fouriercoeff header)

// Yes it's pretty complicated
// No I'm not gonny change it

//Thanks for listening to my TedTalk.



int WavetableOsc1D::wavetableMappingVector(int p_input) {

  switch (p_input) {
    MAP(101, 5)//analog
    MAP(102, 2)
    MAP(103, 6)
    MAP(104, 0)
    MAP(121,140)//nasal
    MAP(122,141)
    MAP(123,142)
    MAP(124,143)
    MAP(131,152)//reed
    MAP(132,153)
    MAP(133,154)
    MAP(134,155)
    MAP(141,88)//glass
    MAP(142,89)
    MAP(143,90)
    MAP(144,91)
    MAP(151,136)//fm
    MAP(152,137)
    MAP(153,138)
    MAP(154,139)
    MAP(161,44)//brokensin
    MAP(162,45)
    MAP(163,46)
    MAP(164,47)
    MAP(171,48)//harsh
    MAP(172,49)
    MAP(173,50)
    MAP(174,51)
    MAP(181,148)//mean
    MAP(182,149)
    MAP(183,150)
    MAP(184,151)
    MAP(191,56)//rectangular
    MAP(192,57)
    MAP(193,58)
    MAP(194,59)
    MAP(201,144)//bitreduced
    MAP(202,145)
    MAP(203,146)
    MAP(204,147)
    MAP(211,60)//strings
    MAP(212,61)
    MAP(213,62)
    MAP(214,63)
    MAP(221,65)//piano
    MAP(222,66)
    MAP(223,67)
    MAP(224,64)
    MAP(231,40)//organ
    MAP(232,41)
    MAP(233,42)
    MAP(234,43)
    MAP(241,68)//oboe
    MAP(242,69)
    MAP(243,70)
    MAP(244,71)
    MAP(251,76)//trumpet
    MAP(252,77)
    MAP(253,78)
    MAP(254,79)
    MAP(261,72)//mixed A
    MAP(262,73)
    MAP(263,74)
    MAP(264,75)
    MAP(271,108)//mixed B
    MAP(272,109)
    MAP(273,110)
    MAP(274,111)
    MAP(281,112)//mixed C
    MAP(282,113)
    MAP(283,114)
    MAP(284,115)
    MAP(291,116)//mixed D
    MAP(292,117)
    MAP(293,118)
    MAP(294,119)
    MAP(301,120)//voice A
    MAP(302,121)
    MAP(303,122)
    MAP(304,123)
    MAP(311,124)//voice B
    MAP(312,125)
    MAP(313,126)
    MAP(314,127)
    MAP(321,128)//voice C
    MAP(322,129)
    MAP(323,130)
    MAP(324,131)
    MAP(331,132)//voice D
    MAP(332,133)
    MAP(333,134)
    MAP(334,135)
    MAP(341,8)//additive A
    MAP(342,9)
    MAP(343,10)
    MAP(344,11)
    MAP(351,12)//add B
    MAP(352,13)
    MAP(353,14)
    MAP(354,15)
    MAP(361,17)//add C
    MAP(362,18)
    MAP(363,19)
    MAP(364,16)
    MAP(371,20)//add D
    MAP(372,21)
    MAP(373,22)
    MAP(374,23)
    MAP(381,32)//OT A
    MAP(382,33)
    MAP(383,34)
    MAP(384,35)
    MAP(391,36)//OT B
    MAP(392,37)
    MAP(393,38)
    MAP(394,39)
    MAP(401,28)//harm A
    MAP(402,29)
    MAP(403,30)
    MAP(404,31)
    MAP(411,24)//harm B
    MAP(412,25)
    MAP(413,26)
    MAP(414,27)
    MAP(421,92)//mutsaw A
    MAP(422,93)
    MAP(423,94)
    MAP(424,95)
    MAP(431,96)//mutsaw B
    MAP(432,97)
    MAP(433,98)
    MAP(434,99)
    MAP(441,100)//mutsquare A
    MAP(442,101)
    MAP(443,102)
    MAP(444,103)
    MAP(451,104)//mutsquare B
    MAP(452,105)
    MAP(453,106)
    MAP(454,107)
    //chiptune
    MAP(501, 2)//squares
    MAP(502, 3)
    MAP(503, 4)
    MAP(504, 1)//triangle
    MAP(505, 85)//chipsaw
    MAP(506, 86)//chipsine
    /*MAP(507, 94)
    MAP(508, 95)
    MAP(509, 96)
    MAP(510, 97)
    MAP(511, 98)
    MAP(512, 101)
    MAP(513, 156)
    MAP(514, 157)
    MAP(515, 158)
    MAP(516, 159)*/
    //wavedraw
    MAP(601, NUMBER_OF_WAVETABLES)
    MAP(602, NUMBER_OF_WAVETABLES+1)
    MAP(603, NUMBER_OF_WAVETABLES+2)
    //chipdraw
    MAP(701, NUMBER_OF_WAVETABLES+3)
    MAP(702, NUMBER_OF_WAVETABLES+4)
    MAP(703, NUMBER_OF_WAVETABLES+5)
    //spec
    MAP(801, NUMBER_OF_WAVETABLES+6)
    MAP(802, NUMBER_OF_WAVETABLES+7)
    MAP(803, NUMBER_OF_WAVETABLES+8)
  default:
    DBG("ERROR: tried to map unknown wavetable " + std::to_string(p_input) +
        " for vec/fm/pm osc");
    return 0;
    break;
  }
  return 0;
}

int WavetableOsc1D::wavetableMappingChiptune(int p_input) {
  int output = 0;
  switch (p_input) {
    //chiptune
    MAP(1, 2)//squares
    MAP(2, 3)
    MAP(3, 4)
    MAP(4, 1)//triangle
    MAP(5, 85)//saw
    MAP(6, 86)//sine
    /*MAP(7, 94)
    MAP(8, 95)
    MAP(9, 96)
    MAP(10, 97)
    MAP(11, 98)
    MAP(12, 101)
    MAP(13, 156)
    MAP(14, 157)
    MAP(15, 158)
    MAP(16, 159)
    //wavtables
     MAP(101, 5)
    MAP(102, 2)
    MAP(103, 6)
    MAP(104, 0)
    MAP(121,44)
    MAP(122,45)
    MAP(123,46)
    MAP(124,47)
    MAP(131,48)
    MAP(132,49)
    MAP(133,50)
    MAP(134,51)
    MAP(141,148)
    MAP(142,149)
    MAP(143,150)
    MAP(144,151)
    MAP(151,56)
    MAP(152,57)
    MAP(153,58)
    MAP(154,59)
    MAP(161,140)
    MAP(162,141)
    MAP(163,142)
    MAP(164,143)
    MAP(171,152)
    MAP(172,153)
    MAP(173,154)
    MAP(174,155)
    MAP(181,136)
    MAP(182,137)
    MAP(183,138)
    MAP(184,139)
    MAP(191,88)
    MAP(192,89)
    MAP(193,90)
    MAP(194,91)
    MAP(201,144)
    MAP(202,145)
    MAP(203,146)
    MAP(204,147)
    MAP(211,60)
    MAP(212,61)
    MAP(213,62)
    MAP(214,63)
    MAP(221,64)
    MAP(222,65)
    MAP(223,66)
    MAP(224,67)
    MAP(231,40)
    MAP(232,41)
    MAP(233,42)
    MAP(234,43)
    MAP(241,68)
    MAP(242,69)
    MAP(243,70)
    MAP(244,71)
    MAP(251,72)
    MAP(252,73)
    MAP(253,74)
    MAP(254,75)
    MAP(261,76)
    MAP(262,77)
    MAP(263,78)
    MAP(264,79)
    MAP(271,120)
    MAP(272,121)
    MAP(273,122)
    MAP(274,123)
    MAP(281,124)
    MAP(282,125)
    MAP(283,126)
    MAP(284,127)
    MAP(291,128)
    MAP(292,129)
    MAP(293,130)
    MAP(294,131)
    MAP(301,132)
    MAP(302,133)
    MAP(303,134)
    MAP(304,135)
    MAP(311,8)
    MAP(312,9)
    MAP(313,10)
    MAP(314,11)
    MAP(321,12)
    MAP(322,13)
    MAP(323,14)
    MAP(324,15)
    MAP(331,16)
    MAP(332,17)
    MAP(333,18)
    MAP(334,19)
    MAP(341,20)
    MAP(342,21)
    MAP(343,22)
    MAP(344,23)
    MAP(351,24)
    MAP(352,25)
    MAP(353,26)
    MAP(354,27)
    MAP(361,28)
    MAP(362,29)
    MAP(363,30)
    MAP(364,31)
    MAP(371,32)
    MAP(372,33)
    MAP(373,34)
    MAP(374,35)
    MAP(381,36)
    MAP(382,37)
    MAP(383,38)
    MAP(384,39)
    MAP(391,92)
    MAP(392,93)
    MAP(393,94)
    MAP(394,95)
    MAP(401,96)
    MAP(402,97)
    MAP(403,98)
    MAP(404,99)
    MAP(411,100)
    MAP(412,101)
    MAP(413,102)
    MAP(414,103)
    MAP(421,104)
    MAP(422,105)
    MAP(423,106)
    MAP(424,107)
    MAP(431,108)
    MAP(432,109)
    MAP(433,110)
    MAP(434,111)
    MAP(441,112)
    MAP(442,113)
    MAP(443,114)
    MAP(444,115)
    MAP(451,116)
    MAP(452,117)
    MAP(453,118)
    MAP(454,119)
    //wavedraw
    MAP(601, NUMBER_OF_WAVETABLES)
    MAP(602, NUMBER_OF_WAVETABLES+1)
    MAP(603, NUMBER_OF_WAVETABLES+2)*/
    //chipdraw
    MAP(701, NUMBER_OF_WAVETABLES+3)
    MAP(702, NUMBER_OF_WAVETABLES+4)
    MAP(703, NUMBER_OF_WAVETABLES+5)
    //spec
    /*MAP(801, NUMBER_OF_WAVETABLES+6)
    MAP(802, NUMBER_OF_WAVETABLES+7)
    MAP(803, NUMBER_OF_WAVETABLES+8)*/
  default:
    DBG("ERROR: tried to map unknown wavetable " + std::to_string(p_input) +
        " for chiptune osc");
    output = 0;
    break;
  }
  return 0;
}
