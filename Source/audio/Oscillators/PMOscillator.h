#pragma once

#include "Oscillator.h"
#include "PMCarrierOsc.h"
#include "WavetableOsc1D.h"


//#define EXP_FM_SEMITONES 24

class PMOscillator : public Oscillator {
public:
  PMOscillator();
  ~PMOscillator();

  void update() override;
  void reset() override;
  float doOscillate() override;
  void loadWavetables();

  void setSampleRate(float p_sr) override {
	  Oscillator::setSampleRate(p_sr);
	  m_carrier_osc.setSampleRate(p_sr);
	  m_modulator_osc.setSampleRate(p_sr);
  }

  inline void setPMAmount(float p_pm_amount) { m_PM_amount = p_pm_amount; }

  inline void setRatio(int p_carrier, int p_modulator) {
    m_carrier_ratio = p_carrier;
    m_modulator_ratio = p_modulator;
  }

  inline void setCarrierRatio(int p_carrier) { m_carrier_ratio = p_carrier; }

  inline void setModulatorRatio(int p_modulator) {
    m_modulator_ratio = p_modulator;
  }

  inline void selectWavetables(int p_wt_carrier, int p_wt_modulator) {
    m_carrier_osc.selectWavetable(p_wt_carrier);
    m_modulator_osc.selectWavetable(p_wt_modulator);
  }

  inline void selectCarrierWavetableByMapping(int p_wt_carrier) {
    m_carrier_osc.selectWavetableByMapping(p_wt_carrier);
  }

  inline void selectModulatorWavetableByMapping(int p_wt_modulator) {
    m_modulator_osc.selectWavetableByMapping(p_wt_modulator);
  }

  void setPMModPointer(float *p_pointer) { m_pm_mod = p_pointer; }

  void setCarrierRatioModPointer(float *p_pointer) {
    m_carrier_ratio_mod = p_pointer;
  }

  void setModulatorRatioModPointer(float *p_pointer) {
    m_modulator_ratio_mod = p_pointer;
  }

  PMCarrierOsc m_carrier_osc;
  WavetableOsc1D m_modulator_osc;

protected:
  float *m_pm_mod;
  float *m_carrier_ratio_mod;
  float *m_modulator_ratio_mod;

  float m_PM_amount = 0.f;
  int m_carrier_ratio = 1.f;
  int m_modulator_ratio = 1.f;

  // keep this to have a mod pointer for suboscs
  float m_zero_modulation_dummy = 0.f;
};
