#pragma once
#include "Filter.h"
#include "VAOnePoleFilter.h"
class DiodeFilter :
	public 	Filter
{
public:
	DiodeFilter(void);
	~DiodeFilter(void);
	void reset() override;
	void update() override;
	double doFilter(double xn) override;
	void setResControl(double res) override;


private:
	double m_k;
	double m_gamma;
	double m_sg1;
	double m_sg2;
	double m_sg3;
	double m_sg4;

	VAOnePoleFilter m_LPF1;
	VAOnePoleFilter m_LPF2;
	VAOnePoleFilter m_LPF3;
	VAOnePoleFilter m_LPF4;

};

