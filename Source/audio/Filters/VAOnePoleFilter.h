#pragma once

#include "Filter.h"

class VAOnePoleFilter : public Filter
{
public:
	VAOnePoleFilter(void);
	~VAOnePoleFilter(void);
	double m_alpha;			// Feed Forward coeff
	double m_beta;
	double m_gamma;		// Pre-Gain
	double m_delta;		// FB_IN Coeff
	double m_epsilon;		// FB_OUT scalar
	double m_a_0;			// input gain
	double m_feedback;		// our own feedback coeff from S 
	void setFeedback(double fb){m_feedback = fb;}
	double getFeedbackOutput(){return m_beta*(m_z_1 + m_feedback*m_delta);}
	virtual void reset(){m_z_1 = 0; m_feedback = 0;}
	virtual void update();
	virtual double doFilter(double xn);
	inline void setLP(){m_is_lowpass = true;}
	inline void setHP(){m_is_lowpass = false;}
protected:
	bool m_is_lowpass = true; //else highpass
	double m_z_1;		// our z-1 storage location
};

