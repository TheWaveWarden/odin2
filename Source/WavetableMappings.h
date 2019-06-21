
#define MAP(inp, out) case inp :\
return out;\
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
    MAP(101,11)
    MAP(102,2)
    MAP(103,12)
    MAP(104,0)
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
  case 1:
    output = 2;
    break;
  case 2:
    output = 3;
    break;
  case 3:
    output = 4;
    break;
  case 4:
    output = 1;
    break;
  case 5:
    output = 99;
    break;
  case 6:
    output = 100;
    break;
  case 7:
    output = 94;
    break;
  case 8:
    output = 95;
    break;
  case 9:
    output = 96;
    break;
  case 10:
    output = 97;
    break;
  case 11:
    output = 98;
    break;
  case 12:
    output = 101;
    break;
  default:
    DBG("ERROR: tried to map unknown wavetable " + std::to_string(p_input) +
        " for chiptune osc");
    output = 0;
    break;
  }
  return 0;
}
