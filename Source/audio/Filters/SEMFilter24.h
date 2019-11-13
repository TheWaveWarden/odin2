#pragma once
#include "Filter.h"
#include "VAOnePoleFilter.h"
#include "SEMFilter12.h"
#include "../../GlobalIncludes.h"

class SEMFilter24 : public Filter
{
  public:
	SEMFilter24(void);
	~SEMFilter24(void);
	void reset() override;
	void setResControl(double res) override;
	double doFilter(double xn) override;

	void setFreqModPointer(float* p_pointer) override {
		Filter::setFreqModPointer(p_pointer);
		m_SEM1.setFreqModPointer(p_pointer);
		m_SEM2.setFreqModPointer(p_pointer);
	}

	void setResModPointer(float* p_pointer) override {
		m_SEM1.setResModPointer(p_pointer);
		m_SEM2.setResModPointer(p_pointer);
		m_res_mod = p_pointer;
	}

	void setVolModPointer(float* p_pointer) override {
		m_SEM1.setVolModPointer(p_pointer);
		m_SEM2.setVolModPointer(p_pointer);
	}

	float m_transition;
	float m_resonance;

  public:
	inline virtual void update()
	{
		Filter::update();

		//PRE WARP FOR BZT
		double wd = 2 * M_PI * m_freq_modded;
		//double T = 1.0 / m_samplerate;
		double wa = (2 * m_samplerate) * tan(wd * m_one_over_samplerate * 0.5);
		double g = wa * m_one_over_samplerate * 0.5;

		float resonance_modded = m_resonance + (*m_res_mod) * 20.5 * 0.2;
    resonance_modded = resonance_modded > 21*0.2 ? 21*0.2 : resonance_modded;
    resonance_modded = resonance_modded < 0.1 ? 0.1 : resonance_modded;

		m_SEM1.m_resonance_modded = resonance_modded;
		m_SEM2.m_resonance_modded = resonance_modded;


		double r = 1.0 / (2.0 * resonance_modded);

    	m_SEM1.m_alpha_0 = 1.0 / (1.0 + 2.0 * r * g + g * g);
    	m_SEM1.m_alpha = g;
    	m_SEM1.m_rho = 2.0 * r + g;

    	m_SEM2.m_alpha_0 = 1.0 / (1.0 + 2.0 * r * g + g * g);
    	m_SEM2.m_alpha = g;
    	m_SEM2.m_rho = 2.0 * r + g;
	}

  protected:
	SEMFilter12 m_SEM1;
	SEMFilter12 m_SEM2;
};
