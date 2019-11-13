#pragma once
#include "WavetableOsc1D.h"
class WavetableOsc2D : public WavetableOsc1D {
public:
  WavetableOsc2D();
  ~WavetableOsc2D();

  virtual void setBaseFrequency(float p_freq) override {
    WavetableOsc1D::setBaseFrequency(p_freq);
    //this means osc-restart so we will set pos_smooth to pos here (= hack)
    m_position_2D_smooth = m_position_2D;
  }

  virtual float doOscillate() override;
  virtual void update() override;

  virtual void reset() override {
    WavetableOsc1D::reset();
    m_position_2D_smooth = m_position_2D;
  }

  inline void setPosition(float p_position) {
    m_position_2D = p_position;
#ifdef WTGEN
    passVariablesToWTDisplayInternal();
#endif
  }

  virtual void loadWavetables() override;

  void setPosModPointer(float *p_pointer) { m_pos_mod = p_pointer; }
  void setPosEnvAmount(float p_pos_env){
    m_pos_env = p_pos_env;
  }
  inline void setEnvValue(float p_env){
    m_env_value = p_env;
  }


#ifdef WTGEN
  std::function<void(int, int, float)> passVariablesToWTDisplay = [](int, int,
                                                                     float) {};
  void passVariablesToWTDisplayInternal() {
    float interpol;
    int p_left_table;
    int p_right_table;

    if (m_position_2D < 0.333333333333f) {
      p_left_table = 0;
      p_right_table = 1;
      interpol = m_position_2D * 3;
    } else if (m_position_2D < 0.666666666f) {
      p_left_table = 1;
      p_right_table = 2;
      interpol = (m_position_2D - 0.333333333f) * 3;
    } else {
      p_left_table = 2;
      p_right_table = 3;
      interpol = (m_position_2D - 0.6666666f) * 3;
    }

    // do remapping
    int lower = WavetableContainer::getInstance().getWavetableIndexFromName(
        getWavetableName(m_wavetable_index, p_left_table));
    int higher = WavetableContainer::getInstance().getWavetableIndexFromName(
        getWavetableName(m_wavetable_index, p_right_table));

    // pass to lambda
    passVariablesToWTDisplay(lower, higher, interpol);
  }

  void selectWavetable(int p_wavetable_index) override {
    WavetableOsc1D::selectWavetable(p_wavetable_index);
    passVariablesToWTDisplayInternal();
  }

#endif
protected:
  std::string getWavetableName(int p_wt_2D, int sub_table_2D);

  inline void getTableIndicesAndInterpolation(int &p_left_table,
                                              int &p_right_table,
                                              float &p_interpolation_value,
                                              float p_position_2D) {
    if (p_position_2D < 0.333333333333f) {
      p_left_table = 0;
      p_right_table = 1;
      p_interpolation_value = p_position_2D * 3;
      return;
    } else if (p_position_2D < 0.666666666f) {
      p_left_table = 1;
      p_right_table = 2;
      p_interpolation_value = (p_position_2D - 0.333333333f) * 3;
      return;
    }
    p_left_table = 2;
    p_right_table = 3;
    p_interpolation_value = (p_position_2D - 0.6666666f) * 3;
    return;
  }

  float *m_pos_mod;

  float m_position_2D = 0.f;
  float m_position_2D_smooth = 0.f;
  //how much modenv is applied to position
  float m_pos_env = 0.f;
  float m_env_value;
  void setWavetablePointer(
      int p_wavetable_index, int p_2D_sub_table,
      const float *p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]);
  float doWavetable2D();



  const float
      *m_wavetable_pointers_2D[NUMBER_OF_WAVETABLES_2D][SUBTABLES_PER_WAVETABLE]
                              [TABLES_PER_2D_WT];
  const float **m_current_table_2D;
};
