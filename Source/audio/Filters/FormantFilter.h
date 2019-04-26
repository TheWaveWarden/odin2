#pragma once
#include "Filter.h"
#include "LadderFilter.h"


class FormantFilter :
	public Filter
{
public:
	FormantFilter();
	~FormantFilter();

	void update() override;
	//void setResControl(double p_res) override;
	double doFilter(double p_input) override;
	void reset() override;
	void setTransition(float p_trans);
	void setVowelLeft(int p_vowel){
		m_vowel_left = p_vowel;
		updateParabolas();
	}
	void setVowelRight(int p_vowel){
		m_vowel_right = p_vowel;
		updateParabolas();
	}

	void setVolModPointer(float* p_pointer) { m_vol_mod = p_pointer; }


protected:

	float* m_vol_mod;
	float m_zero_dummy;

	void updateParabolas();
	

	float m_transition = 0.f;
	int m_vowel_left = 0;
	int m_vowel_right = 1;

	float m_formant_list [8][2] =
	{
		{1000.f,1400.f},  // A
		{500.f, 2300.f }, // E
		{320.f, 3200.f},  // I
		{500.f, 1000.f},  // O
		{320.f, 800.f},   // U
		{700.f, 1800.f},  // Ä
		{500.f, 1500.f},  // Ö
		{320.f, 1650.f},  // Ü
	};

	//for parabola interpolation
	float m_a0;
	float m_b0;
	float m_c0;
	float m_a1;
	float m_b1;
	float m_c1;


	LadderFilter m_BPF1;
	LadderFilter m_BPF2;
};

