
#define MAP(inp, out)                                                          \
  case inp:                                                                    \
    return out;                                                                \
    break;

int WavetableOsc1D::wavetableMappingFM(int p_input) {
  DBG("fm");
  if (p_input == 1) { // extra sine for FM/PM
    return 0;
  }
  // apart from the extra sine, vec/fm mappings are identical
  return wavetableMappingVector(p_input);
}

int WavetableOsc1D::wavetableMappingVector(int p_input) {

  switch (p_input) {
    MAP(101, 11)
    MAP(102, 2)
    MAP(103, 12)
    MAP(104, 0)
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
    MAP(1, 2)
    MAP(2, 3)
    MAP(3, 4)
    MAP(4, 1)
    MAP(5, 99)
    MAP(6, 100)
    MAP(7, 94)
    MAP(8, 95)
    MAP(9, 96)
    MAP(10, 97)
    MAP(11, 98)
    MAP(12, 101)
  default:
    DBG("ERROR: tried to map unknown wavetable " + std::to_string(p_input) +
        " for chiptune osc");
    output = 0;
    break;
  }
  return 0;
}
