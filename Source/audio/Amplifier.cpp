#include "Amplifier.h"


void Amplifier::incWriteIndex() {
    if (++m_write_index >= WIDTH_DELAY_SAMPLES) {
      m_write_index = 0;
    }
}

void Amplifier::doAmplifier(float p_in, float &po_left_out, float &po_right_out) {

    // appply gain
    float gain_mod_factor =
        (*m_gain_mod) > 0 ? 1.f + 4 * (*m_gain_mod) : (1.f + *m_gain_mod);

    m_gain_smooth = m_gain_smooth * GAIN_SMOOTHIN_FACTOR +
                    (1 - GAIN_SMOOTHIN_FACTOR) * (m_gain);

    m_pan_smooth = m_pan_smooth * PAN_SMOOTHIN_FACTOR + (1 - PAN_SMOOTHIN_FACTOR) * m_pan;

    m_width_smooth = m_width_smooth * WIDTH_SMOOTHIN_FACTOR + (1 - WIDTH_SMOOTHIN_FACTOR) * m_width_seconds;

    p_in *= m_gain_smooth * gain_mod_factor;
    m_width_delay_buffer_left[m_write_index] = p_in;
    m_width_delay_buffer_right[m_write_index] = p_in;

    // apply velocity
    //float vel_modded = m_vel_amount + *(m_vel_mod);
    //vel_modded = vel_modded < 0 ? 0 : vel_modded;
    //vel_modded = vel_modded > 1 ? 1 : vel_modded;
    //p_in *= 1.f - vel_modded * 1.f + vel_modded * m_MIDI_vel;

    //do width
	  float read_index     = (float)m_write_index - m_width_smooth * m_samplerate;
	  int read_index_trunc = (int)read_index;
	  int read_index_next  = read_index_trunc + 1;
	  float frac           = read_index - (float)read_index_trunc;
	  while (read_index_trunc < 0) {
		  read_index_trunc += WIDTH_DELAY_SAMPLES;
	  }
	  while (read_index_next < 0) {
		  read_index_next += WIDTH_DELAY_SAMPLES;
	  }
	  float width_delayed;
    if(m_width_left){
	    float width_delayed = linearInterpolation(m_width_delay_buffer_left[read_index_trunc], m_width_delay_buffer_left[read_index_next], frac);
	    incWriteIndex();
      // do panning
      float pan_modded = m_pan_smooth + *m_pan_mod;
      pan_modded = pan_modded < -1 ? -1 : pan_modded;
      pan_modded = pan_modded > 1 ? 1 : pan_modded;
      if (pan_modded >= 0.f) {
        po_right_out = width_delayed;
        po_left_out = (1.f - pan_modded) * p_in;
      } else {
        po_right_out = (pan_modded + 1.f) * width_delayed;
        po_left_out = p_in;
      }
    } else {
      float width_delayed = linearInterpolation(m_width_delay_buffer_right[read_index_trunc], m_width_delay_buffer_right[read_index_next], frac);
      incWriteIndex();
      // do panning
      float pan_modded = m_pan_smooth + *m_pan_mod;
      pan_modded = pan_modded < -1 ? -1 : pan_modded;
      pan_modded = pan_modded > 1 ? 1 : pan_modded;
      if (pan_modded >= 0.f) {
        po_right_out = p_in;
        po_left_out = (1.f - pan_modded) * width_delayed;
      } else {
        po_right_out = (pan_modded + 1.f) * p_in;
        po_left_out = width_delayed;
      }
    }

  }