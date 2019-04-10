#include "FormantFilter.h"



FormantFilter::FormantFilter()
{
    m_BPF1.m_filter_type = LadderFilter::FILTERTYPE::LP2;//todo?
    m_BPF2.m_filter_type = LadderFilter::FILTERTYPE::BP2;

    m_BPF1.setResControl(0.8);
    m_BPF2.setResControl(0.8);

    //m_BPF1.enablePassBandCompensation();
    //m_BPF2.enablePassBandCompensation();

    reset();

    updateParabolas();
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

    m_BPF1.m_freq_base = m_a0 * m_transition * m_transition + m_b0 * m_transition + m_c0;
    m_BPF2.m_freq_base = m_a1 * m_transition * m_transition + m_b1 * m_transition + m_c1;
    //for(float transition = 0.f; transition <= 1.f; transition += 0.01){
    //  DBG(m_a0 * transition * transition + m_b0 * transition + m_c0);  
    //}
    //DBG("==================================================================");

    //m_BPF1.m_freq_base = m_formant_list[m_vowel_left][0] * (1.f - m_transition) + m_transition * m_formant_list[m_vowel_right][0];
    //m_BPF2.m_freq_base = m_formant_list[m_vowel_left][1] * (1.f - m_transition) + m_transition * m_formant_list[m_vowel_right][1];

//    m_BPF1.m_mod_frequency = m_formant_width;
//    m_BPF2.m_mod_frequency = -m_formant_width;

    m_BPF1.update();
    m_BPF2.update();
}

void FormantFilter::updateParabolas(){
    float f0 = m_formant_list[m_vowel_left][0];
    float f1 = m_formant_list[m_vowel_right][0];
    float f2 = f0 * pow(f1/f0, 0.5);

    m_a0 = 2*f1 - 4*f2 + 2*f0;
    m_b0 = 4*f2 - 3*f0 - f1;
    m_c0 = f0;

    f0 = m_formant_list[m_vowel_left][1];
    f1 = m_formant_list[m_vowel_right][1];
    f2 = f0 * pow(f1/f0, 0.5);

    m_a1 = 2*f1 - 4*f2 + 2*f0;
    m_b1 = 4*f2 - 3*f0 - f1;
    m_c1 = f0;
}

//void FormantFilter::setResControl(double p_res){
//    m_BPF1.setResControl(0.3 * p_res + 0.4);
//    m_BPF2.setResControl(0.3 * p_res + 0.4);
//}

void FormantFilter::setTransition(float p_trans){
    m_transition = p_trans;
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