#include "SEMFilter12.h"

SEMFilter12::SEMFilter12()
{
    m_alpha_0 = 1.0;
    m_alpha = 1.0;
    m_rho = 1.0;
    m_aux_control = 0.5;

    reset();

    m_transition = -1.;
}

SEMFilter12::~SEMFilter12()
{
}

void SEMFilter12::reset()
{
    m_z_1 = 0.0;
    m_z_2 = 0.0;
}

void SEMFilter12::update()
{
    Filter::update();

    double wd = 2 * M_PI * m_freq_modded;
    double T = 1.0 / m_samplerate;
    double wa = (2 / T) * tan(wd * T / 2);
    double g = wa * T / 2;

    double r = 1.0 / (2.0 * m_resonance);

    m_alpha_0 = 1.0 / (1.0 + 2.0 * r * g + g * g);
    m_alpha = g;
    m_rho = 2.0 * r + g;
}

double SEMFilter12::doFilter(double xn)
{

    double hpf = m_alpha_0 * (xn - m_rho * m_z_1 - m_z_2);
    double bpf = m_alpha * hpf + m_z_1;

    

    double lpf = m_alpha * bpf + m_z_2;
    double r = 1.0 / (2.0 * m_resonance);
    double bsf = xn - 2.0 * r * bpf;

    m_z_1 = m_alpha * hpf + bpf;
    m_z_2 = m_alpha * bpf + lpf;

    if (m_transition < 0)
    {
        xn = (1 + m_transition) * bsf - m_transition * lpf;
    }
    else
    {
        xn = m_transition * hpf + (1 - m_transition) * bsf;
    }

    if (m_overdrive < 1.){
	    //interpolate here so we have possibility of pure linear Processing
	    xn = xn * (1. - m_overdrive) + m_overdrive * fasttanh(xn);
	} else {
	    xn = fasttanh(m_overdrive * xn);
	}

    return xn;
}

void SEMFilter12::setResControl(double p_res)
{
    m_resonance = 24.5 * p_res * p_res + 0.5;
}
