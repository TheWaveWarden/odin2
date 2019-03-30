#pragma once
class CubicShaper
{
  public:
	CubicShaper();
	~CubicShaper();

	//TODO include const filter

	inline void setAmount(float p_amount)
	{
		m_amount = -p_amount;
	}

	inline float doShaper(float p_input)
	{
		if (p_input > 0)
		{
			return p_input * p_input * p_input * m_amount -
				   p_input * p_input * m_amount * 2 +
				   p_input * (1.f + m_amount);
		}
		else
		{
			return p_input * p_input * p_input * m_amount +
				   p_input * p_input * m_amount * 2 +
				   p_input * (1.f + m_amount);
		}
	}

  protected:
	float m_amount = 0.f;
};
