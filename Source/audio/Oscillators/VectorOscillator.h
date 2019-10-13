#pragma once
#include "WavetableOsc1D.h"
class VectorOscillator : public WavetableOsc1D {
public:
  VectorOscillator();
  ~VectorOscillator();

  virtual void setBaseFrequency(float p_freq) override {
    WavetableOsc1D::setBaseFrequency(p_freq);
    // this means osc-restart so we will set pos_smooth to pos here (= hack)
    m_XY_pad_x_smooth = m_XY_pad_x;
    m_XY_pad_y_smooth = m_XY_pad_y;
  }

  virtual void reset() override {
    WavetableOsc1D::reset();

    m_XY_pad_x_smooth = m_XY_pad_x;
    m_XY_pad_y_smooth = m_XY_pad_y;
  }

  inline void setXYPad(float p_x, float p_y) {
    setX(p_x);
    setY(p_y);
  }

  inline void setX(float p_x) { m_XY_pad_x = p_x; }

  inline void setY(float p_y) { m_XY_pad_y = p_y; }

  void selectWavetable(int p_wavetable_index, int p_vector_point);
  void selectWavetableByMappingVector(int p_wavetable_index,
                                      int p_vector_point) {
    selectWavetable(wavetableMappingVector(p_wavetable_index), p_vector_point);
  }

  virtual float doOscillate() override;
  virtual void update() override;
  // virtual void loadWavetables() override;

  void setXModPointer(float *p_pointer) { m_mod_x = p_pointer; }
  void setYModPointer(float *p_pointer) { m_mod_y = p_pointer; }

protected:
  float *m_mod_x;
  float *m_mod_y;

  float doVectortable();

  float m_XY_pad_x = 0.f;
  float m_XY_pad_y = 0.f;

  float m_XY_pad_x_smooth = 0.f;
  float m_XY_pad_y_smooth = 0.f;

  const float *m_current_table_vec_0;
  const float *m_current_table_vec_1;
  const float *m_current_table_vec_2;
  const float *m_current_table_vec_3;

  int m_wavetable_index_0 = 0;
  int m_wavetable_index_1 = 0;
  int m_wavetable_index_2 = 0;
  int m_wavetable_index_3 = 0;
};
