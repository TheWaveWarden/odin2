#pragma once
#include "../Filters/DCBlockingFilter.h"
#include "../OdinConstants.h"
#include "Oscillator.h"
#include "WavetableContainer.h"
#include <functional>

#ifdef BENCHMARK
#define DBG(text) 
#endif


#define SYNC_PORTION_OF_TABLE 16 // sync smoothing takes place 1/16th of a table

class WavetableOsc1D : public Oscillator {
public:
  // this shall map wavetables properly.
  // Input: wavetable index from valueTree (propably from dropdown menu)
  // Ouptut: wavetable index as it was set in loadWavetables()
  std::function<void(int)> selectWavetableByMapping = [](int) {
    DBG("\n\n\nERROR: setWavetableByMapping() not set!!!!\n\n\n");
  };

  virtual void randomizePhase() override {
    float white_noise = (float)rand();
	  m_read_index = white_noise / (float)RAND_MAX * WAVETABLE_LENGTH;
    DBG("WT_RANDO PHAASE");
  }

  WavetableOsc1D();
  virtual ~WavetableOsc1D();

  void setVoldB(float dB);
  virtual void selectWavetable(int p_wavetable_index);

  // overrides
  virtual void reset() override;
  virtual float doOscillate() override;
  virtual void update() override;

  // this function is used to support sync for all deriving classes.
  // It does the sync stuff and then calls doOscillate (from child classes)
  float doOscillateWithSync();

  // used to load wavetables can be overwritten by child classes
  virtual void loadWavetables();

  void loadWavedrawTables(int p_osc);
  void loadChipdrawTables(int p_osc);
  void loadSpecdrawTables(int p_osc);

  // these shouldn't be here but it seemed the least tedious solution...
  int wavetableMappingChiptune(int p_input);
  int wavetableMappingVector(int p_input);
  int wavetableMappingFM(int p_input);

  void setSyncOscillator(Oscillator *p_osc) { m_sync_oscillator = p_osc; }

  void setSyncEnabled(bool p_sync) { m_sync_enabled = p_sync; }

  void setSampleRate(float p_sr) override {
    Oscillator::setSampleRate(p_sr);
    m_dc_blocking_filter.setSampleRate(p_sr);
  }

  void setWavetableContainer(WavetableContainer* p_WT_container){
    m_WT_container = p_WT_container;
  }

protected:
  virtual void initiateSync();

  virtual int getTableIndex();

  void setWavetablePointer(
      int p_wavetable_index,
      const float *p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]);
  void setWavetablePointerNONCONST(
      int p_wavetable_index,
      float *p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]);

  inline float linearInterpolation(float p_low, float p_high,
                                   float p_distance) {
    //return (1.f - p_distance) * p_low + p_distance * p_high;
    //p_low - p_distance * p_low + p_distance * p_high;
    return p_distance * (p_high - p_low) + p_low;
  }

  inline void checkWrapIndex(double &p_index) {

    m_reset_flag = false;

    while (p_index < 0) {
      p_index += WAVETABLE_LENGTH;
    }

    while (p_index >= WAVETABLE_LENGTH) {
      p_index -= WAVETABLE_LENGTH;
      m_reset_flag = true;
      m_reset_position = p_index;
    }
  }

  float doWavetable();

  // for sync
  DCBlockingFilter m_dc_blocking_filter;

  bool m_sync_enabled = false;

  float m_volume_factor = 1.f;

  double m_read_index = 0.f;
  double m_wavetable_inc = 0.f;

  Oscillator *m_sync_oscillator = nullptr;
  float m_sync_anti_aliasing_inc_factor = 1.f;

  // downsampling filter (see distortion for details)
  double xv[10] = {0.};
  double yv[10] = {0.};

  // tables
  const float
      *m_wavetable_pointers[NUMBER_OF_WAVETABLES + 9] //+ 9 for drawtables
                           [SUBTABLES_PER_WAVETABLE];
  const float *m_current_table;
  int m_wavetable_index = 0;
  int m_sub_table_index = 0;

  int m_nr_of_wavetables =      // + 9 for draw pointers
      NUMBER_OF_WAVETABLES + 9; // can be overwritten in child classes

  //for optimization
  int m_last_table_index;
  float m_one_over_last_mid_freq = 0.f;
  //for each minor third interval, this array holds 1 / f, so we can efficiently calculate if we are still in the minor third interval
  const float m_table_one_over_mid_freq_values[SUBTABLES_PER_WAVETABLE] = {0.0396548, 0.0333456, 0.0280402, 0.0235789, 0.0198274, 0.0166728, 0.0140201, 0.0117895, 0.00991372, 0.00833641, 0.00701006, 0.00589473, 0.00495686, 0.00416821, 0.00350503, 0.00294737, 0.00247843, 0.0020841, 0.00175252, 0.00147369, 0.00123922, 0.00104205, 0.000876259, 0.000736843, 0.000619609, 0.000521027, 0.00043813, 0.000368422, 0.000309804, 0.000260514, 0.000219065, 0.000184211, 0.000154902};

  WavetableContainer *m_WT_container;

#define ROOT_OF_MINOR_THIRD 1.09050f
#define ONE_OVER_ROOT_OF_MINOR_THIRD 0.917004f
};
