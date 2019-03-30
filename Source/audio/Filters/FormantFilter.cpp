#include "FormantFilter.h"



FormantFilter::FormantFilter()
{
    m_BPF1.m_filter_type = LadderFilter::FILTERTYPE::BP4;
    m_BPF2.m_filter_type = LadderFilter::FILTERTYPE::BP4;

    reset();
}


FormantFilter::~FormantFilter()
{
}

void FormantFilter::reset(){
    m_BPF1.reset();
    m_BPF2.reset();
}

void FormantFilter::update(){
    Filter::update();

    m_BPF1.m_freq_base = m_freq_modded;
    m_BPF2.m_freq_base = m_freq_modded;

    m_BPF1.m_mod_frequency = m_formant_width;
    m_BPF2.m_mod_frequency = -m_formant_width;

    m_BPF1.update();
    m_BPF2.update();
}

void FormantFilter::setResControl(double p_res){
    m_BPF1.setResControl(0.3 * p_res + 0.4);
    m_BPF2.setResControl(0.3 * p_res + 0.4);
}

void FormantFilter::setFormantWidth(float p_formant_width){
    m_formant_width = p_formant_width;
}


double FormantFilter::doFilter(double p_input){
    double out = m_BPF1.doFilter(m_BPF2.doFilter(p_input));

    if (m_overdrive < 1.) {
			//interpolate here so we have possibility of pure linear Processing
			out = out * (1. - m_overdrive) + m_overdrive * fasttanh(out);
	}
	else {
		out = fasttanh(m_overdrive*out);
	}

    return out;
}