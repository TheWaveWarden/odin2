#pragma once
#include "Filter.h"
#include "VAOnePoleFilter.h"
#include "SEMFilter12.h"

class SEMFilter24 : public Filter
{
  public:
	SEMFilter24(void);
	~SEMFilter24(void);
	void reset() override;
	void setResControl(double res) override;
	double doFilter(double xn) override;

	float m_transition;
	float m_resonance;

  public:
	inline virtual void update()
	{
		Filter::update();

		//PRE WARP FOR BZT
		double wd = 2 * M_PI * m_freq_modded;
		double T = 1.0 / m_samplerate;
		double wa = (2 / T) * tan(wd * T / 2);
		double g = wa * T / 2;

		double r = 1.0 / (2.0 * m_resonance);

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
