#pragma once
#include "Oscillator.h"
#include "../OdinConstants.h"
#include "WavetableContainer.h"
#include "../Filters/DCBlockingFilter.h"
#include <fstream>//todo


#define SYNC_PORTION_OF_TABLE 16 //sync smoothing takes place 1/16th of a table


class WavetableOsc1D :
	public Oscillator
{
public:
    //this shall map wavetables properly.
	//Input: wavetable index from vlaueTree (propably from dropdown menu)
	//Ouptut: wavetable index as it was set in loadWavetables()
    std::function<void(int)> selectWavetableByMapping = [](int){
		DBG("\n\n\nERROR: setWavetableByMapping() not set!!!!\n\n\n");
	};

	WavetableOsc1D();
	virtual ~WavetableOsc1D();

	void setVoldB(float dB);
	virtual void selectWavetable(int p_wavetable_index);

	//overrides
	virtual void reset() override;
	virtual float doOscillate() override;
	virtual void update() override;

	//this function is used to support sync for all deriving classes.
	//It does the sync stuff and then calls doOscillate (from child classes)
	float doOscillateWithSync();

	//used to load wavetables can be overwritten by child classes
	virtual void loadWavetables();

	void loadWavedrawTables(int p_osc);
	void loadChipdrawTables(int p_osc);
	void loadSpecdrawTables(int p_osc);

	//these shouldn't be here but it seemed the least tedious solution...
	int wavetableMappingChiptune(int p_input);
	int wavetableMappingVector(int p_input);
	int wavetableMappingFM(int p_input);

	void setSyncOscillator(Oscillator* p_osc){
		m_sync_oscillator = p_osc;
	}

	void setSyncEnabled(bool p_sync){
		DBG("SETSYNCENABLED" + std::to_string((int)p_sync));
		m_sync_enabled = p_sync;
	}

	void setSampleRate(float p_sr) override {
		Oscillator::setSampleRate(p_sr);
		m_dc_blocking_filter.setSampleRate(p_sr);
	}

protected:
	virtual void initiateSync();

	virtual int getTableIndex();
	
	void setWavetablePointer(int p_wavetable_index, const float* p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]);
	void setWavetablePointerNONCONST(int p_wavetable_index, float* p_wavetable_pointers[SUBTABLES_PER_WAVETABLE]);
	
	inline float linearInterpolation(float p_low, float p_high, float p_distance){
		return (1.f - p_distance) * p_low + p_distance * p_high;
	}

	inline void checkWrapIndex(double &p_index){

		m_reset_flag = false;
		
		while(p_index < 0){
			p_index += WAVETABLE_LENGTH;
		}
		
		while(p_index >= WAVETABLE_LENGTH){
			p_index -= WAVETABLE_LENGTH;
			m_reset_flag = true;
		}
	}

	float doWavetable();

	//for sync
	DCBlockingFilter m_dc_blocking_filter;

	bool m_sync_enabled = true;

	float m_volume_factor = 1.f;

	double m_read_index = 0.f;
	double m_wavetable_inc = 0.f;

	Oscillator* m_sync_oscillator = nullptr;
	bool m_sync_in_progress = false;
	float m_value_before_sync = 0;
	//this points to the falling edge section which is used to transition on sync event
	//float** m_sync_table = nullptr;


	//tables
	const float* m_wavetable_pointers[NUMBER_OF_WAVETABLES + 9][SUBTABLES_PER_WAVETABLE];
	const float* m_current_table;
	int m_wavetable_index = 0;
	int m_sub_table_index = 0;
	
	int m_nr_of_wavetables = NUMBER_OF_WAVETABLES + 9;//can be overwritten in child classes 

	//std::ofstream debug_stream; //todo

};

