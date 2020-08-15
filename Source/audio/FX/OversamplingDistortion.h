#pragma once

#define THRESHOLD_MIN 0.05f
#define DISTORTION_OUTPUT_SCALAR 1.0

class OversamplingDistortion {
public:
  OversamplingDistortion();
  ~OversamplingDistortion();

  enum DistortionAlgorithm { Clamp = 1, Fold = 2, Zero = 3, Sine = 4, Cube = 5};

  double doDistortion(double p_input);

  inline void setThreshold(float p_threshold) {
    p_threshold = 1.f - p_threshold;
    m_threshold = p_threshold * p_threshold * p_threshold; 
  }

  // inline void setBias(float p_bias){
  //	m_bias = p_bias;
  //}

  // inline void setOverdrive(bool p_overdrive){
  //	m_is_overdrive = p_overdrive;
  //}

  void setAlgorithm(int p_algo) { m_algorithm = (DistortionAlgorithm)p_algo; }

  inline void setDryWet(float p_drywet) { m_drywet = p_drywet; }

  void setThresholdModPointer(float *p_pointer) { m_threshold_mod = p_pointer; }

  void setDryWetModPointer(float *p_pointer) { m_drywet_mod = p_pointer; }

  void reset() {
    for (int i = 0; i < 10; ++i) {
      xv[i] = yv[i] = 0;
    }
    m_threshold_smooth = m_threshold;
  }

protected:
  float *m_threshold_mod;
  float *m_drywet_mod;

  // bool m_is_overdrive = true; // else fuzz
  DistortionAlgorithm m_algorithm = Clamp;

  double m_last_input = 0.f;
  float m_bias = 0.f;
  float m_threshold = 0.343f;        //(1-0.3)^3
  float m_threshold_smooth = 0.343f; //(1-0.3)^3
  float m_drywet = 1.f;

  // IIR Filter buffers for downsampling
  // coefficients (see implementation) are taken from
  // http://www-users.cs.york.ac.uk/~fisher/cgi-bin/mkfscript
  double xv[10] = {0.};
  double yv[10] = {0.};
};
