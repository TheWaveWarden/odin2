#include <iostream>
#include <unistd.h>
#include <random>
#include <ctime>

#include "benchmark/benchmark.h"

#include "../Source/audio/Oscillators/AnalogOscillator.h"
#include "../Source/audio/Oscillators/MultiOscillator.h"
#include "../Source/audio/Oscillators/NoiseOscillator.h"

#define SIZE_MAP 12
#define CONST_FREQ_ARRAY                                                       \
  { 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440, 440 }
#define VAR_FREQ_ARRAY                                                         \
  { 27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080, 600, 170}

void activateOscillator(Oscillator &p_osc, float *p_mod_dummy) {
  p_osc.setSampleRate(44100);
  p_osc.setBaseFrequency(440);
  p_osc.setPitchBendPointer(p_mod_dummy);
  p_osc.setPitchModExpPointer(p_mod_dummy);
  p_osc.setPitchModLinPointer(p_mod_dummy);
  p_osc.setGlidePointer(p_mod_dummy);
  p_osc.setVolModPointer(p_mod_dummy);
}

static void VarModPure(benchmark::State &state) {
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  for (auto _ : state) {
    mod_dummy = (float)std::rand()/RAND_MAX;
  }
}
BENCHMARK(VarModPure);



static void AnaOscConstMod(benchmark::State &state) {
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;
  int index = 0;
  float freq_map[SIZE_MAP] = CONST_FREQ_ARRAY;

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);

  for (auto _ : state) {
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(AnaOscConstMod);



static void AnaOscVarMod(benchmark::State &state) {
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);

  for (auto _ : state) {
    mod_dummy = (float)std::rand()/RAND_MAX;
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(AnaOscVarMod);





static void PWMOscConstMod(benchmark::State &state) {
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);
  osc.selectWavetable(1);

  for (auto _ : state) {
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(PWMOscConstMod);

static void PWMOscVarMod(benchmark::State &state) {
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);
  osc.selectWavetable(1);

  for (auto _ : state) {
    mod_dummy = (float)std::rand()/RAND_MAX;
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(PWMOscVarMod);


static void MultiOscConstMod(benchmark::State &state) {
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;

  MultiOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setDetuneModPointer(&mod_dummy);
  osc.setPosModPointer(&mod_dummy);
  osc.setSpreadModPointer(&mod_dummy);
  osc.setDetune(1.f);
  osc.setWavetableMultiSpread(1.f);

  for (auto _ : state) {
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(MultiOscConstMod);

static void MultiOscVarMod(benchmark::State &state) {
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  MultiOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setDetuneModPointer(&mod_dummy);
  osc.setPosModPointer(&mod_dummy);
  osc.setSpreadModPointer(&mod_dummy);
  osc.setDetune(1.f);
  osc.setWavetableMultiSpread(1.f);

  for (auto _ : state) {
    mod_dummy = (float)std::rand()/RAND_MAX;
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(MultiOscVarMod);

static void NoiseOscConstMod(benchmark::State &state) {
  float mod_dummy = 0.f;

  NoiseOscillator osc;
  osc.setHPModPointer(&mod_dummy);
  osc.setLPModPointer(&mod_dummy);
  osc.setVolModPointer(&mod_dummy);
  for (auto _ : state) {
    //mod_dummy = (float)std::rand()/RAND_MAX;
    osc.doNoise();
  }
}
BENCHMARK(NoiseOscConstMod);


static void NoiseOscVarMod(benchmark::State &state) {
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  NoiseOscillator osc;
  osc.setHPModPointer(&mod_dummy);
  osc.setLPModPointer(&mod_dummy);
  osc.setVolModPointer(&mod_dummy);
  for (auto _ : state) {
    mod_dummy = (float)std::rand()/RAND_MAX;
    osc.doNoise();
  }
}
BENCHMARK(NoiseOscVarMod);





BENCHMARK_MAIN();
