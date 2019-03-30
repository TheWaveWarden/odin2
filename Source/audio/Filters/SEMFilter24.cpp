#include "SEMFilter24.h"



SEMFilter24::SEMFilter24(void)
{
	reset();
	m_transition = -1.;
}


SEMFilter24::~SEMFilter24(void){}

void SEMFilter24::reset() {
	m_SEM1.reset();
	m_SEM2.reset();
}

void SEMFilter24::setResControl(double p_res) {
	m_resonance = 24.5 * p_res*p_res + 0.5;
	m_resonance *= 0.2;
	m_SEM1.m_resonance = m_resonance;
	m_SEM2.m_resonance = m_resonance;
}

double SEMFilter24::doFilter(double xn) {

    m_SEM1.m_transition = m_transition;
    m_SEM2.m_transition = m_transition;
    m_SEM1.m_overdrive = m_overdrive;
    m_SEM2.m_overdrive = m_overdrive;

    return m_SEM1.doFilter(m_SEM2.doFilter(xn));
}

