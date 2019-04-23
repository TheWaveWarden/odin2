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

// ModMatrix::ModMatrix(ModSources *p_sources, ModDestinations *p_destinations){
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