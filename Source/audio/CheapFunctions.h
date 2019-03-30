#pragma once
#include <cmath>



#define MAX_TABLE_ADSR 10000

class CheapFunctions
{
public:
	static CheapFunctions& getInstance(){
		static CheapFunctions instance;
		return instance;
	}
	CheapFunctions(CheapFunctions const &) = delete;
	//void operator=(CheapFunctions const&)  = delete;

	virtual ~CheapFunctions();

	void generateADSRPow();

	inline double linearInterpolation(double lower, double higher, double frac){
		return lower + (higher - lower) * frac;
	}

	inline double cheapADSRPow(double p_decay_times_sr){
		if(p_decay_times_sr < 0.001 * 44100 || p_decay_times_sr > 10000*44100){
			return pow(0.001, 1./ p_decay_times_sr);
		}
		
		double array_index = p_decay_times_sr * 1000. / 44100. - 1;
		int array_index_trunc = (int) array_index;
		int array_index_next = array_index_trunc + 1;
		double frac = array_index - (double)array_index_trunc;

		array_index_trunc = array_index_trunc > MAX_TABLE_ADSR - 1 ? MAX_TABLE_ADSR - 1 : array_index_trunc;
		array_index_next = array_index_next > MAX_TABLE_ADSR - 1 ? MAX_TABLE_ADSR -1 : array_index_next;
		array_index_trunc = array_index_trunc < 0 ? 0 : array_index_trunc; 

		return linearInterpolation(m_adsr_pow_table[array_index_trunc], m_adsr_pow_table[array_index_next], frac);
	}

private:

	CheapFunctions();

	//include tables
	#include "LookupTables/adsr_pow.h"


};

