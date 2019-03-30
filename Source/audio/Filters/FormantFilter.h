#pragma once
#include "Filter.h"
#include "LadderFilter.h"


class FormantFilter :
	public Filter
{
public:
	FormantFilter();
	~FormantFilter();

	void update() override;
	void setResControl(double p_res) override;
	double doFilter(double p_input) override;
	void reset() override;

	void setFormantWidth(float p_width);

protected:

	float m_formant_width = 0.f;

	LadderFilter m_BPF1;
	LadderFilter m_BPF2;
};

