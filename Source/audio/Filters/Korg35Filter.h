#pragma once
#include "Filter.h"
#include "VAOnePoleFilter.h"

class Korg35Filter :
	public Filter{
public:
	Korg35Filter(void);
	~Korg35Filter(void);

	void reset() override;
	void update() override;
	double doFilter(double xn) override;
	void setResControl(double res) override;

	inline void setFilterType(bool p_is_lowpass){
		m_is_lowpass = p_is_lowpass;//else HP
	}
	
private:
	double m_k;
	double m_alpha;

	VAOnePoleFilter m_LPF1;
	VAOnePoleFilter m_LPF2;
	VAOnePoleFilter m_HPF1;
	VAOnePoleFilter m_HPF2;

	bool m_is_lowpass = true;//else HP
};

