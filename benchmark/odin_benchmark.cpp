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

#include <iostream>
#include <unistd.h>
#include <random>
#include <ctime>
#include <cmath>

#include "benchmark/benchmark.h"

#include "../Source/audio/Oscillators/AnalogOscillator.h"
#include "../Source/audio/Oscillators/MultiOscillator.h"
#include "../Source/audio/Oscillators/NoiseOscillator.h"
#include "../Source/audio/Oscillators/FMOscillator.h"
#include "../../JUCE/modules/juce_dsp/maths/juce_FastMathApproximations.h"

void activateOscillator(Oscillator &p_osc, float *p_mod_dummy)
{
  p_osc.setSampleRate(44100);
  p_osc.setBaseFrequency(440);
  p_osc.setPitchBendPointer(p_mod_dummy);
  p_osc.setPitchModExpPointer(p_mod_dummy);
  p_osc.setPitchModLinPointer(p_mod_dummy);
  p_osc.setGlidePointer(p_mod_dummy);
  p_osc.setVolModPointer(p_mod_dummy);
}

static void VarModPure(benchmark::State &state)
{
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  for (auto _ : state)
  {
    mod_dummy = (float)std::rand() / RAND_MAX;
  }
}
BENCHMARK(VarModPure);

static void AnaOscConstMod(benchmark::State &state)
{
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);

  for (auto _ : state)
  {
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(AnaOscConstMod);

static void AnaOscVarMod(benchmark::State &state)
{
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);

  for (auto _ : state)
  {
    mod_dummy = (float)std::rand() / RAND_MAX;
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(AnaOscVarMod);

static void PWMOscConstMod(benchmark::State &state)
{
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);
  osc.selectWavetable(1);

  for (auto _ : state)
  {
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(PWMOscConstMod);

static void PWMOscVarMod(benchmark::State &state)
{
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  AnalogOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setPWMModPointer(&mod_dummy);
  osc.selectWavetable(1);

  for (auto _ : state)
  {
    mod_dummy = (float)std::rand() / RAND_MAX;
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(PWMOscVarMod);

static void MultiOscConstMod(benchmark::State &state)
{
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

  for (auto _ : state)
  {
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(MultiOscConstMod);

static void MultiOscUpdateOnly(benchmark::State &state)
{
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

  for (auto _ : state)
  {
    osc.update();
  }
}
BENCHMARK(MultiOscUpdateOnly);

static void MultiOscillateOnly(benchmark::State &state)
{
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
  osc.update();

  for (auto _ : state)
  {
    osc.doOscillate();
  }
}
BENCHMARK(MultiOscillateOnly);

static void MultiOscVarMod(benchmark::State &state)
{
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

  for (auto _ : state)
  {
    mod_dummy = (float)std::rand() / RAND_MAX;
    osc.update();
    osc.doOscillateWithSync();
  }
}
BENCHMARK(MultiOscVarMod);

static void NoiseOscConstMod(benchmark::State &state)
{
  float mod_dummy = 0.f;

  NoiseOscillator osc;
  osc.setHPModPointer(&mod_dummy);
  osc.setLPModPointer(&mod_dummy);
  osc.setVolModPointer(&mod_dummy);
  for (auto _ : state)
  {
    //mod_dummy = (float)std::rand()/RAND_MAX;
    osc.doNoise();
  }
}
BENCHMARK(NoiseOscConstMod);

static void NoiseOscVarMod(benchmark::State &state)
{
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  NoiseOscillator osc;
  osc.setHPModPointer(&mod_dummy);
  osc.setLPModPointer(&mod_dummy);
  osc.setVolModPointer(&mod_dummy);
  for (auto _ : state)
  {
    mod_dummy = (float)std::rand() / RAND_MAX;
    osc.doNoise();
  }
}
BENCHMARK(NoiseOscVarMod);

static void FMOscVarMod(benchmark::State &state)
{
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;
  std::srand(std::time(nullptr));

  FMOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setCarrierRatioModPointer(&mod_dummy);
  osc.setModulatorRatioModPointer(&mod_dummy);
  osc.setFMModPointer(&mod_dummy);

  for (auto _ : state)
  {
    mod_dummy = (float)std::rand() / RAND_MAX;
    osc.update();
    osc.doOscillate();
  }
}
BENCHMARK(FMOscVarMod);

static void FMOscConstMod(benchmark::State &state)
{
  WavetableContainer::getInstance().loadWavetablesFromConstData();
  float mod_dummy = 0.f;

  FMOscillator osc;
  activateOscillator(osc, &mod_dummy);
  osc.loadWavetables();
  osc.setCarrierRatioModPointer(&mod_dummy);
  osc.setModulatorRatioModPointer(&mod_dummy);
  osc.setFMModPointer(&mod_dummy);

  for (auto _ : state)
  {
    osc.update();
    osc.doOscillate();
  }
}
BENCHMARK(FMOscConstMod);

#define MATH_BENCHMARK_ITERATIONS 100
static void std_exp_times_100(benchmark::State &state)
{
  float x = -6;
  for (auto _ : state)
  {
    x = -6;
    for (int i = 0; i < MATH_BENCHMARK_ITERATIONS; ++i)
    {
      x += 0.1;
      benchmark::DoNotOptimize(std::exp(x));
    }
  }
}
BENCHMARK(std_exp_times_100);

static void juce_exp_times_100(benchmark::State &state)
{
  float x = -6;
  float result;
  for (auto _ : state)
  {
    x = -6;
    for (int i = 0; i < MATH_BENCHMARK_ITERATIONS; ++i)
    {
      x += 0.1;
      result = juce::dsp::FastMathApproximations::exp(x);
      benchmark::DoNotOptimize(result += 1);
      //std::exp(x);
    }
  }
}
BENCHMARK(juce_exp_times_100);

BENCHMARK_MAIN();
