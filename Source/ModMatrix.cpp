/*
  ==============================================================================

    ModMatrix.cpp
    Created: 20 Apr 2019 5:04:47pm
    Author:  frot

  ==============================================================================
*/

#include "ModMatrix.h"

void ModMatrixRow::setSourcesAndDestinations(ModSources *p_source,
                                             ModDestinations *p_destination) {
  m_sources = p_source;
  m_destinations = p_destination;
}

void ModMatrixRow::applyModulation() {
  // DBG("henlo");
  // poly destination -> 12 modulations
  if (m_destination_poly) {
    for (int voice = 0; voice < VOICES; ++voice) {
      if (m_scale) {
        *(m_destination_value[voice]) +=
            (*m_source_value[voice]) * m_mod_amount *
            (1 + (*m_scale_value[voice] - 1) * m_scale_amount);
      } else {
        // DBG("MOD SLOT ACTIVE");
        *(m_destination_value[voice]) +=
            (*m_source_value[voice]) * m_mod_amount;
      }
    }
  }
}

void ModMatrixRow::setModSource(int p_source) {

  switch (p_source) {
  case 100:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].osc[0];
    }
    break;
  case 101:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].osc[1];
    }
    break;
  case 102:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].osc[2];
    }
    break;
  case 110:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].filter[0];
    }
    break;
  case 111:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].filter[1];
    }
    break;
  case 200:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].adsr[0];
    }
    break;
  case 201:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].adsr[1];
    }
    break;
  case 202:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].adsr[2];
    }
    break;
  case 203:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].adsr[3];
    }
    break;
  case 300:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].lfo[0];
    }
    break;
  case 301:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].lfo[1];
    }
    break;
  case 302:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].lfo[2];
    }
    break;
  case 303:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].lfo[3];
    }
    break;

  case 400:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->x;
    }
    break;
  case 401:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->y;
    }
    break;
  case 402:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->modwheel;
    }
    break;
  case 403:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->pitchwheel;
    }
    break;

  case 404:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].MIDI_key;
    }
    break;
  case 405:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->voice[voice].MIDI_velocity;
    }
    break;
  case 406:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->MIDI_aftertouch;
    }
    break;

  case 1000:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_source_value[voice] = m_sources->constant;
    }
    break;

  default:
    // todo
    break;
  }
  // DBG(p_source);

  // for(int voice = 0;  voice < VOICES; ++voice){
  //  m_source_value[voice] = &(m_sources->voice[voice].adsr[3]);
  //}

  m_source = p_source;
  m_active = (m_source && m_destination);

  DBG(m_source);
  DBG(m_destination);
}

void ModMatrixRow::setModDestination(int p_destination) {
  // DBG(p_destination);
  switch (p_destination) {
  case 1:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].pitch_exponential);
    }
    m_destination_poly = true;
    break;

  case 101:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].pitch_exponential);
    }
    m_destination_poly = true;
    break;

  case 201:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].pitch_exponential);
    }
    m_destination_poly = true;
    break;

  case 2:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].pitch_linear);
    }
    m_destination_poly = true;
    break;

  case 102:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].pitch_linear);
    }
    m_destination_poly = true;
    break;

  case 202:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].pitch_linear);
    }
    m_destination_poly = true;
    break;

  case 3:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[0].vol);
    }
    m_destination_poly = true;
    break;

  case 103:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[1].vol);
    }
    m_destination_poly = true;
    break;

  case 203:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[2].vol);
    }
    m_destination_poly = true;
    break;

  case 10:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].pulse_width);
    }
    m_destination_poly = true;
    break;

  case 110:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].pulse_width);
    }
    m_destination_poly = true;
    break;

  case 210:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].pulse_width);
    }
    m_destination_poly = true;
    break;

  case 20:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].position);
    }
    m_destination_poly = true;
    break;
  case 120:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].position);
    }
    m_destination_poly = true;
    break;
  case 220:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].position);
    }
    m_destination_poly = true;
    break;

  case 21:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].detune);
    }
    m_destination_poly = true;
    break;
  case 121:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].detune);
    }
    m_destination_poly = true;
    break;
  case 221:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].detune);
    }
    m_destination_poly = true;
    break;

  case 22:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].spread);
    }
    m_destination_poly = true;
    break;
  case 122:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].spread);
    }
    m_destination_poly = true;
    break;
  case 222:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].spread);
    }
    m_destination_poly = true;
    break;

  case 30:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[0].x);
    }
    m_destination_poly = true;
    break;
  case 130:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[1].x);
    }
    m_destination_poly = true;
    break;
  case 230:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[2].x);
    }
    m_destination_poly = true;
    break;

  case 31:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[0].y);
    }
    m_destination_poly = true;
    break;
  case 131:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[1].y);
    }
    m_destination_poly = true;
    break;
  case 231:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].osc[2].y);
    }
    m_destination_poly = true;
    break;

  case 40:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].arp_speed);
    }
    m_destination_poly = true;
    break;
  case 140:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].arp_speed);
    }
    m_destination_poly = true;
    break;
  case 240:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].arp_speed);
    }
    m_destination_poly = true;
    break;

  case 50:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].fm_amount);
    }
    m_destination_poly = true;
    break;
  case 150:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].fm_amount);
    }
    m_destination_poly = true;
    break;
  case 250:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].fm_amount);
    }
    m_destination_poly = true;
    break;

  case 51:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].carrier_ratio);
    }
    m_destination_poly = true;
    break;
  case 151:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].carrier_ratio);
    }
    m_destination_poly = true;
    break;
  case 251:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].carrier_ratio);
    }
    m_destination_poly = true;
    break;

  case 52:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].modulator_ratio);
    }
    m_destination_poly = true;
    break;
  case 152:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].modulator_ratio);
    }
    m_destination_poly = true;
    break;
  case 252:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].modulator_ratio);
    }
    m_destination_poly = true;
    break;

  case 60:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].lp_freq);
    }
    m_destination_poly = true;
    break;
  case 160:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].lp_freq);
    }
    m_destination_poly = true;
    break;
  case 260:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].lp_freq);
    }
    m_destination_poly = true;
    break;

  case 61:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[0].hp_freq);
    }
    m_destination_poly = true;
    break;
  case 161:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[1].hp_freq);
    }
    m_destination_poly = true;
    break;
  case 261:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].osc[2].hp_freq);
    }
    m_destination_poly = true;
    break;

  case 301:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].freq);
    }
    m_destination_poly = true;
    break;
  case 401:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].freq);
    }
    m_destination_poly = true;
    break;
  case 501:
    m_destination_value[0] = &(m_destinations->filter3.freq);
    m_destination_poly = false;
    break;

  case 302:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].res);
    }
    m_destination_poly = true;
    break;
  case 402:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].res);
    }
    m_destination_poly = true;
    break;
  case 502:
    m_destination_value[0] = &(m_destinations->filter3.res);
    m_destination_poly = false;
    break;

  case 303:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].gain);
    }
    m_destination_poly = true;
    break;
  case 403:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].gain);
    }
    m_destination_poly = true;
    break;
  case 503:
    m_destination_value[0] = &(m_destinations->filter3.gain);
    m_destination_poly = false;
    break;

  case 304:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].env_amount);
    }
    m_destination_poly = true;
    break;
  case 404:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].env_amount);
    }
    m_destination_poly = true;
    break;
  case 504:
    m_destination_value[0] = &(m_destinations->filter3.env_amount);
    m_destination_poly = false;
    break;

  case 305:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].vel_amount);
    }
    m_destination_poly = true;
    break;
  case 405:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].vel_amount);
    }
    m_destination_poly = true;
    break;
  case 505:
    m_destination_value[0] = &(m_destinations->filter3.vel_amount);
    m_destination_poly = false;
    break;

  case 306:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].kbd_amount);
    }
    m_destination_poly = true;
    break;
  case 406:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].kbd_amount);
    }
    m_destination_poly = true;
    break;
  case 506:
    m_destination_value[0] = &(m_destinations->filter3.kbd_amount);
    m_destination_poly = false;
    break;

  case 307:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].saturation);
    }
    m_destination_poly = true;
    break;
  case 407:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].saturation);
    }
    m_destination_poly = true;
    break;
  case 507:
    m_destination_value[0] = &(m_destinations->filter3.saturation);
    m_destination_poly = false;
    break;

  case 310:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[0].SEM_transition);
    }
    m_destination_poly = true;
    break;
  case 410:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].filter[1].SEM_transition);
    }
    m_destination_poly = true;
    break;
  case 510:
    m_destination_value[0] = &(m_destinations->filter3.SEM_transition);
    m_destination_poly = false;
    break;

  case 601:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[0].attack);
    }
    m_destination_poly = true;
    break;
  case 611:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[1].attack);
    }
    m_destination_poly = true;
    break;
  case 621:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[2].attack);
    }
    m_destination_poly = true;
    break;
  case 631:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[3].attack);
    }
    m_destination_poly = true;
    break;

  case 602:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[0].decay);
    }
    m_destination_poly = true;
    break;
  case 612:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[1].decay);
    }
    m_destination_poly = true;
    break;
  case 622:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[2].decay);
    }
    m_destination_poly = true;
    break;
  case 632:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[3].decay);
    }
    m_destination_poly = true;
    break;

  case 603:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[0].sustain);
    }
    m_destination_poly = true;
    break;
  case 613:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[1].sustain);
    }
    m_destination_poly = true;
    break;
  case 623:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[2].sustain);
    }
    m_destination_poly = true;
    break;
  case 633:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[3].sustain);
    }
    m_destination_poly = true;
    break;

  case 604:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[0].release);
    }
    m_destination_poly = true;
    break;
  case 614:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[1].release);
    }
    m_destination_poly = true;
    break;
  case 624:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[2].release);
    }
    m_destination_poly = true;
    break;
  case 634:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] =
          &(m_destinations->voice[voice].adsr[3].release);
    }
    m_destination_poly = true;
    break;

  case 651:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].lfo[0].freq);
    }
    m_destination_poly = true;
    break;
  case 661:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].lfo[1].freq);
    }
    m_destination_poly = true;
    break;
  case 671:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].lfo[2].freq);
    }
    m_destination_poly = true;
    break;
  case 681:
    for (int voice = 0; voice < VOICES; ++voice) {
      m_destination_value[voice] = &(m_destinations->voice[voice].lfo[3].freq);
    }
    m_destination_poly = true;
    break;
  }
  m_destination = p_destination;
  m_active = (m_source && m_destination);
  DBG(m_source);
  DBG(m_destination);
}

void ModMatrixRow::setModScale(int p_scale) { m_scale = p_scale; }

//=========================================================
//=================== MODMATRIX ===========================
//=========================================================

void ModMatrix::applyModulation() {
  for (int row = 0; row < MODMATRIX_ROWS; ++row) {
    if (m_row[row]) {
      m_row[row].applyModulation();
    }
  }
}

void ModMatrix::setSourcesAndDestinations(ModSources *p_sources,
                                          ModDestinations *p_destinations) {
  for (int row = 0; row < MODMATRIX_ROWS; ++row) {
    m_row[row].setSourcesAndDestinations(p_sources, p_destinations);
  }
  m_sources = p_sources;
  m_destinations = p_destinations;
  zeroAllDestinations();
  zeroAllSources();
}

// ModMatrix::ModMatrix(ModSources *p_sources, ModDestinations
// *p_destinations){
//  for (int row = 0; row < MODMATRIX_ROWS; ++row) {
//    m_row[row].setSourcesAndDestinations(p_sources, p_destinations);
//  }
//}

void ModMatrix::setModSource(int p_row, int p_source) {
  m_row[p_row].setModSource(p_source);
}

void ModMatrix::setModDestination(int p_row, int p_destination) {
  m_row[p_row].setModDestination(p_destination);
}

void ModMatrix::setModScale(int p_row, int p_scale) {
  m_row[p_row].setModScale(p_scale);
}

void ModMatrix::setModAmount(int p_row, float p_mod_amount) {
  m_row[p_row].setModAmount(p_mod_amount);
}

void ModMatrix::setScaleAmount(int p_row, float p_scale_amount) {
  m_row[p_row].setScaleAmount(p_scale_amount);
}

void ModMatrix::zeroAllSources() { memset(m_sources, 0, sizeof(ModSources)); }

void ModMatrix::zeroAllDestinations() {
  memset(m_destinations, 0, sizeof(ModDestinations));
}