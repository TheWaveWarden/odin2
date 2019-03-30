#include "ChiptuneArpeggiator.h"



int ChiptuneArpeggiator::doArpeggiator(){
    if(!m_arp_active){
        return 0;
    }

    //inc & wrap position inside step
    m_position_in_step += m_inc;
    while(m_position_in_step > STEP_LENGTH){
        m_position_in_step -= STEP_LENGTH;
        ++m_current_step;
        //inc & wrap step
        if(m_current_step > 2 || (!m_step_three_active && m_current_step > 1)){
            m_current_step = 0;
        }
    }

    return m_step_semitones[m_current_step];
}
