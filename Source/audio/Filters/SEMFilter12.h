#pragma once
#include "Filter.h"
#include "../../GlobalIncludes.h"
class SEMFilter12 :
	public Filter
{
public:
	SEMFilter12();
	~SEMFilter12();
	void reset() override;
	void update() override;
	double doFilter(double xn) override;
	void setResControl(double res) override;

	double m_transition;
	double m_resonance;
	double m_resonance_modded;
	double m_alpha;
	double m_alpha_0;
	double m_rho;

	void setTransitionModPointer(float* p_pointer){
		m_transition_mod = p_pointer;
	}

protected:
	float* m_transition_mod = &m_mod_dummy_zero;

	double m_z_1;
	double m_z_2;
};

