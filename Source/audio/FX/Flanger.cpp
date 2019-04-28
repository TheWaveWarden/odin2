#include "Flanger.h"

Flanger::Flanger() {}

Flanger::~Flanger() {}

float Flanger::doFlanger(float p_input) {

  incrementLFO();
  float LFO = doLFO();

  float amount_modded = m_LFO_amount + *m_amount_mod;
  amount_modded = amount_modded < 0 ? 0 : amount_modded;

  // todo amount mod still blows up... limit?
  m_delay_time_control =
      m_base_time + LFO * amount_modded * FLANGER_LFO_MAX_RANGE;

  float drywet_modded = m_dry_wet + *m_drywet_mod;
  drywet_modded = drywet_modded < 0 ? 0 : drywet_modded;
  drywet_modded = drywet_modded > 1 ? 1 : drywet_modded;

  return drywet_modded * doFilter(p_input) + (1.f - drywet_modded) * p_input;
}
