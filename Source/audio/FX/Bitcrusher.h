#pragma once

#include <cstdint>
#include <cmath>

class Bitcrusher
{
public:
	Bitcrusher();
	virtual ~Bitcrusher();

	inline void reset(){
		m_current_value_left = 0.f;
		m_current_value_right = 0.f;
	}

	inline void set_bits(uint8_t p_bits){
		if(p_bits == m_bits){
			return;
		}
		m_bits = p_bits;
		m_discretization_scalar = powf(2.f, (float)(m_bits - 1));
	}

	inline void set_edge(float p_edge){
		m_edge = p_edge;
	}

	inline void set_dry_wet(float p_dry_wet){
		m_dry_wet = p_dry_wet;
	}

	inline float do_bitcrush_left(float p_input){
		float target_value = floor(p_input * m_discretization_scalar) / m_discretization_scalar;
		m_current_value_left += (target_value - m_current_value_left) * m_edge;
		return m_current_value_left * (m_dry_wet) + p_input *(1.f - m_dry_wet);
	}

	inline float do_bitcrush_right(float p_input){
		float target_value = floor(p_input * m_discretization_scalar) / m_discretization_scalar;
		m_current_value_right += (target_value - m_current_value_right) * m_edge;
		return m_current_value_right * (m_dry_wet) + p_input *(1.f - m_dry_wet);
	}

protected:
	float m_current_value_left = 0.f;
	float m_current_value_right = 0.f;
	float m_discretization_scalar = 8.f;// 2^(4-1)
	float m_edge = 0.8f;
	float m_dry_wet = 1.f;
	uint8_t m_bits = 4;
};

