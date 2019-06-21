/*
	RackAFX(TM)
	Applications Programming Interface
	Derived Class Object Implementation
*/


#include <vector>
#include "Odin.h"

#include <stdexcept>


/* constructor()
	You can initialize variables here.
	You can also allocate memory here as long is it does not
	require the plugin to be fully instantiated. If so, allocate in init()

*/
COdin::COdin()
{
	// Added by RackAFX - DO NOT REMOVE
	//
	// initUI() for GUI controls: this must be called before initializing/using any GUI variables
	initUI();
	// END initUI()

	// built in initialization
	m_PlugInName = "Odin";

	// Default to Stereo Operation:
	// Change this if you want to support more/less channels
	m_uMaxInputChannels = 2;
	m_uMaxOutputChannels = 2;

	// use of MIDI controllers to adjust sliders/knobs
	m_bEnableMIDIControl = true;		// by default this is enabled

	// DO NOT CHANGE let RackAFX change it for you; use Edit Project to alter
	m_bUseCustomVSTGUI = false;

	// output only - SYNTH - plugin DO NOT CHANGE let RackAFX change it for you; use Edit Project to alter
	m_bOutputOnlyPlugIn = true;

	// change to true if you want all MIDI messages
	m_bWantAllMIDIMessages = false;

	// un-comment this for VST/AU Buffer-style processing
	// m_bWantVSTBuffers = true;

	// Finish initializations here
	m_pMidiEventList = NULL; // --- for sample accurate MIDI in VST, AU (AAX has its own method)

}


/* destructor()
	Destroy dynamically allocated variables
*/
COdin::~COdin(void)
{
	WavetableContainer::getInstance().destroyWavetables();
}

/*
initialize()
	Called by the client after creation; the parent window handle is now valid
	so you can use the Plug-In -> Host functions here
	See the website www.willpirkle.com for more details
*/
bool __stdcall COdin::initialize()
{
	//create wavetables in wavetablecontainer singleton
	WavetableContainer::getInstance().createWavetables((float)m_nSampleRate);
	
	//create some chipdraw arrays for testing and create the tables
	float  chipdraw[CHIPDRAW_LENGTH];
	for(int i = 0; i < CHIPDRAW_LENGTH; ++i){
		chipdraw[i] = (float)i;
	}
	WavetableContainer::getInstance().createChipdrawTable(0, chipdraw, (float)m_nSampleRate);

	float flip = 1;
	for(int i = 0; i < CHIPDRAW_LENGTH; ++i){
		chipdraw[i] = flip;
		flip = -flip;
	}
	WavetableContainer::getInstance().createChipdrawTable(1, chipdraw, (float)m_nSampleRate);

	for(int i = 0; i < CHIPDRAW_LENGTH; ++i){
		chipdraw[i] = (float)i * flip;
		flip = -flip;
	}
	WavetableContainer::getInstance().createChipdrawTable(2, chipdraw, (float)m_nSampleRate);

	float wavedraw[WAVEDRAW_LENGTH];
	for(int i = 0; i < WAVEDRAW_LENGTH; ++i){
		if(i < WAVEDRAW_LENGTH / 3.f){
			wavedraw[i] = 1.f;
		} else if (i < WAVEDRAW_LENGTH / 3.f * 2.f){
			wavedraw[i] = -1.f;
		} else {
			wavedraw[i] = 0.f;
		}
	}	
	WavetableContainer::getInstance().createWavedrawTable(0, wavedraw, (float)m_nSampleRate);

	float specdraw[SIN_AND_COS][SPECDRAW_LENGTH] = {0};
	for(int i = 1; i < SPECDRAW_LENGTH; ++i){
		specdraw[0][i] = 1;
	}
	WavetableContainer::getInstance().createSpecdrawTable(0, specdraw, (float)m_nSampleRate);




	//get wavetables from container
	m_chiptune_osc.loadWavetables();
	m_analog_osc.loadWavetables();
	m_2D_osc.loadWavetables();
	m_vec_osc.loadWavetables();
	m_multi_osc.loadWavetables();
	m_FM_osc.loadWavetables();
	m_wave1D_osc.loadWavetables();

	//WavetableContainer::getInstance().writeScaleFactorsToFile();
	m_adsr.setSampleRate(44100.f);
	m_adsr.reset();

	m_delay.setSampleRate(44100.f);
	m_comb_filter.setSampleRate(44100.f);
	m_allpass_filter.setSampleRate(44100.f);
	m_phaser.setSampleRate(44100.f);
	m_flanger.setSampleRate(44100.f);
	m_chorus.setSampleRate(44100.f);
	//CheapFunctions::getInstance().generateADSRPow();
	//m_adsr.testADSRPow();
	//m_adsr.profileCheapPow();

	// m_adsr.setLoop(true);
	// std::ofstream adsr;
	// adsr.open("E:\\odin\\DEBUG.txt");
	// adsr << "grid,value\n";
	// for (size_t i = 0; i < 150000; ++i){
	// 	adsr << i;
	// 	adsr << ",";
	// 	adsr << m_adsr.doEnvelope();
	// 	adsr << "\n";
	// 	if(i == 145000){
	// 		m_adsr.startRelease();
	// 	}
	// }

	// adsr.close();


	return true;
}

/*
processRackAFXMessage()
	Called for a variety of reasons, but we override here to pick up a MIDIEventList interface
	for sample accurate MIDI in VST/AU NOTE: this is only for use in processVSTBuffer()
	If you use processAudioFrame( ) you do not need to bother with this as you already
	have sample accurate MIDI in v6.8.0.5 and above
	See the website www.willpirkle.com for more details
*/
void __stdcall COdin::processRackAFXMessage(UINT uMessage, PROCESS_INFO& processInfo)
{
	// --- always call base class first
	CPlugIn::processRackAFXMessage(uMessage, processInfo);

	// --- for MIDI Event list handling (AU, VST, AAX with processVSTBuffer())
	if(uMessage == midiEventList)
	{
		m_pMidiEventList = processInfo.pIMidiEventList;
	}
}

/* prepareForPlay()
	Called by the client after Play() is initiated but before audio streams

	You can perform buffer flushes and per-run intializations.
	You can check the following variables and use them if needed:

	m_nNumWAVEChannels;
	m_nSampleRate;
	m_nBitDepth;

	NOTE: the above values are only valid during prepareForPlay() and
		  processAudioFrame() because the user might change to another wave file,
		  or use the sound card, oscillators, or impulse response mechanisms

    NOTE: if you alloctae memory in this function, destroy it in the destructor above
*/
bool __stdcall COdin::prepareForPlay()
{
	// Add your code here:



	// --- let base class do its thing
	return CPlugIn::prepareForPlay();
}

/* processAudioFrame

// ALL VALUES IN AND OUT ON THE RANGE OF -1.0 TO + 1.0

LEFT INPUT = pInputBuffer[0];
RIGHT INPUT = pInputBuffer[1]

LEFT OUTPUT = pInputBuffer[0]
RIGHT OUTPUT = pOutputBuffer[1]

HOST INFORMATION is available in m_HostProcessInfo:

// --- for RackAFX and all derivative projects:
m_HostProcessInfo.uAbsoluteSampleBufferIndex = sample index of top of current audio buffer
m_HostProcessInfo.dAbsoluteSampleBufferTime = time (sec) of sample in top of current audio buffer
m_HostProcessInfo.dBPM = Host Tempo setting in BPM
m_HostProcessInfo.fTimeSigNumerator = Host Time Signature Numerator (if supported by host)
m_HostProcessInfo.uTimeSigDenomintor = Host Time Signature Denominator (if supported by host)

// --- see the definition of HOST_INFO in the pluginconstants.h file for variables that are
//     unique to AU, AAX and VST for use in your ported projects!
*/
bool __stdcall COdin::processAudioFrame(float* pInputBuffer, float* pOutputBuffer, UINT uNumInputChannels, UINT uNumOutputChannels)
{
	// --- for VST3 plugins only
	doVSTSampleAccurateParamUpdates();

	// --- smooth parameters (if enabled) DO NOT REMOVE
	smoothParameterValues();

	m_bitcrusher.set_bits(m_bitcrush_bits);
	m_bitcrusher.set_edge(m_bitcrush_edge);
	m_bitcrusher.set_dry_wet(m_bitcrush_dry_wet);

	m_wave1D_osc.selectWavetable(m_wavetableosc_wavetable);
	m_wave1D_osc.update();
	//pOutputBuffer[0] = m_wave1D_osc.doOscillate();

	if(m_use_chiptune_noise == 1){
		m_chiptune_osc.setOperationModeNoise();
	} else {
		m_chiptune_osc.setOPerationModeWave();
	}
	m_chiptune_osc.selectWavetable(m_wavetableosc_wavetable);
	m_chiptune_osc.setArpStepThreeOn(m_arp_step3_active == 1);
	m_chiptune_osc.setArpSemitone(0, m_arp1_semitones);
	m_chiptune_osc.setArpSemitone(1, m_arp2_semitones);
	m_chiptune_osc.setArpSemitone(2, m_arp3_semitones);
	m_chiptune_osc.setArpSpeed(m_arp_speed);
	m_chiptune_osc.update();
	pOutputBuffer[0] = m_chiptune_osc.doOscillate();

	m_wave1D_osc.selectWavetable(m_wavetableosc_wavetable);
	m_wave1D_osc.update();
	//pOutputBuffer[0] = m_wave1D_osc.doOscillate();

	
	m_analog_osc.selectWavetable(m_wavetableosc_wavetable);
	m_analog_osc.setPWMDuty(m_pwm_duty);
	m_analog_osc.setDrift(m_analog_drift);

	m_analog_osc.update();

	//pOutputBuffer[0] = m_analog_osc.doOscillate();
	
	m_2D_osc.setPosition(m_2D_position);
	m_2D_osc.selectWavetable(m_wavetableosc_wavetable);
	m_2D_osc.update();

	//pOutputBuffer[0] = m_2D_osc.doOscillate();
	
	m_vec_osc.setXYPad(m_xy_pad_x, m_xy_pad_y);
	m_vec_osc.selectWavetable(0,0);
	m_vec_osc.selectWavetable(1,1);
	m_vec_osc.selectWavetable(2,2);
	m_vec_osc.selectWavetable(3,3);
	m_vec_osc.update();
	//pOutputBuffer[0] = m_vec_osc.doOscillate();
	
	m_multi_osc.setPosition(m_2D_position);
	m_multi_osc.setDetune(m_multiosc_detune);
	m_multi_osc.setWavetableMultiSpread(m_multiosc_wt_spread);
	m_multi_osc.selectWavetable(m_wavetableosc_wavetable);
	m_multi_osc.selectWavetable(m_wavetableosc_wavetable);
	m_multi_osc.selectWavetable(m_wavetableosc_wavetable);
	m_multi_osc.selectWavetable(m_wavetableosc_wavetable);
	m_multi_osc.update();
	//pOutputBuffer[0] = m_multi_osc.doOscillate();
	
	m_noise_osc.setFilterFreqs(m_noise_lp, m_noise_hp);
	//pOutputBuffer[0] = m_noise_osc.doNoise();
	
	m_FM_osc.setFMAmount(m_FM_amount);
	m_FM_osc.selectWavetables(m_FM_carrier_wave, m_FM_modulator_wave);
	m_FM_osc.setFMExponential(m_FM_exponential != 0);
	m_FM_osc.setRatio(m_carrier_ratio, m_modulator_ratio);
	
	m_FM_osc.update();
	//pOutputBuffer[0] = m_FM_osc.doOscillate();

	m_ladder_filter.setFilterType(m_filter_type);
	m_ladder_filter.setResControl(m_filter_res);
	m_ladder_filter.m_freq_base = m_filter_freq;
	m_ladder_filter.m_overdrive = m_filter_saturation;
	m_ladder_filter.update();
	// pOutputBuffer[0] = m_ladder_filter.doFilter(pOutputBuffer[0]);

	m_diode_filter.setResControl(m_filter_res);
	m_diode_filter.m_freq_base = m_filter_freq;
	m_diode_filter.m_overdrive = m_filter_saturation;
	m_diode_filter.update();
	//pOutputBuffer[0] = m_diode_filter.doFilter(pOutputBuffer[0]);

	m_korg_filter.setResControl(m_filter_res);
	m_korg_filter.setFilterType(m_filter_type == 0);
	m_korg_filter.m_freq_base = m_filter_freq;
	m_korg_filter.m_overdrive = m_filter_saturation;
	m_korg_filter.update();
	//pOutputBuffer[0] = m_korg_filter.doFilter(pOutputBuffer[0]);

	m_SEM12_filter.setResControl(m_filter_res);
	m_SEM12_filter.m_freq_base = m_filter_freq;
	m_SEM12_filter.m_overdrive = m_filter_saturation;
	m_SEM12_filter.m_transition = m_filter_transition;
	m_SEM12_filter.update();
	//pOutputBuffer[0] = m_SEM12_filter.doFilter(pOutputBuffer[0]);

	m_SEM24_filter.setResControl(m_filter_res);
	m_SEM24_filter.m_freq_base = m_filter_freq;
	m_SEM24_filter.m_overdrive = m_filter_saturation;
	m_SEM24_filter.m_transition = m_filter_transition;
	m_SEM24_filter.update();
	//pOutputBuffer[0] = m_SEM24_filter.doFilter(pOutputBuffer[0]);

	m_formant_filter.setResControl(m_filter_res);
	m_formant_filter.m_freq_base = m_filter_freq;
	m_formant_filter.m_overdrive = m_filter_saturation;
	m_formant_filter.setFormantWidth(m_formant_width);
	m_formant_filter.update();
	//pOutputBuffer[0] = m_formant_filter.doFilter(pOutputBuffer[0]);

	m_comb_filter.setCombFreq(m_filter_freq);
	m_comb_filter.setPositive(m_comb_positive != 0);
	m_comb_filter.setResonance(m_filter_res);
	//pOutputBuffer[0] = m_comb_filter.doFilter(pOutputBuffer[0]);

	m_allpass_filter.setRadius(m_allpass_radius);
	m_allpass_filter.setFrequency(m_filter_freq);
	if(m_use_allpass == 1){
		//pOutputBuffer[0] = (m_allpass_filter.doFilter(pOutputBuffer[0]) + pOutputBuffer[0]) * 0.5f;
	}


	m_adsr.setAttack(m_attack);
	m_adsr.setDecay(m_decay);
	m_adsr.setSustain(m_sustain);
	m_adsr.setRelease(m_release);
	m_adsr.setLoop(m_env_loop != 0);
	pOutputBuffer[0] *= m_adsr.doEnvelope();

	m_phaser.setAmount(m_phaser_mod);
	m_phaser.setBaseFreq(m_filter_freq);
	m_phaser.setRadiusBase(m_allpass_radius);
	m_phaser.setLFOAmplitude(m_phaser_lfo_amplitude);
	m_phaser.setLFOFreq(m_phaser_lfo_freq);
	//pOutputBuffer[0] = m_phaser.doPhaser(pOutputBuffer[0]);

	m_flanger.setResonance(m_filter_res);
	m_flanger.setLFOAmount(m_phaser_lfo_amplitude);
	m_flanger.setLFOFreq(m_phaser_lfo_freq);
	m_flanger.setDryWet(m_flanger_dry_wet);
	m_flanger.setPositive(m_comb_positive == 1);
	//pOutputBuffer[0] = m_flanger.doFlanger(pOutputBuffer[0]);

	m_chorus.setAmount(m_phaser_lfo_amplitude);
	m_chorus.setDryWet(m_flanger_dry_wet);
	m_chorus.setLFOFreq(m_phaser_lfo_freq);
	pOutputBuffer[0] = m_chorus.doChorus(pOutputBuffer[0]);

	m_cubic_shaper.setAmount(m_shaper_amount);
	//pOutputBuffer[0] = m_cubic_shaper.doShaper(pOutputBuffer[0]);

	m_quartic_shaper.setAmount(m_shaper_amount);
	//pOutputBuffer[0] = m_quartic_shaper.doShaper(pOutputBuffer[0]);

	m_distortion.setBias(m_distortion_bias);
	m_distortion.setThreshold(m_distortion_threshold);
	m_distortion.setOverdrive(m_distortion_type != 0);
	if(m_use_distortion != 0){
		pOutputBuffer[0] = m_distortion.doDistortion(pOutputBuffer[0]);
	}

	m_delay.setDelayTime(m_delay_time);
	m_delay.setFeedback(m_delay_feedback);
	m_delay.setDry(m_delay_dry);
	m_delay.setWet(m_delay_wet);
	m_delay.setHPFreq(m_delay_filter_freq);
	m_delay.setDucking(m_delay_ducking);
	//pOutputBuffer[0] = m_delay.doDelay(pOutputBuffer[0]);


	//pOutputBuffer[0] = pInputBuffer[0];// m_chiptune_osc.doOscillate();
	pOutputBuffer[1] = pOutputBuffer[0];
	
	
	
	//pOutputBuffer[0] = m_bitcrusher.do_bitcrush_left(pInputBuffer[0]);
	//pOutputBuffer[0] = pInputBuffer[0];
	// Mono-In, Stereo-Out (AUX Effect)
	//if(uNumInputChannels == 1 && uNumOutputChannels == 2)
	//	pOutputBuffer[1] = pOutputBuffer[0];

	// Stereo-In, Stereo-Out (INSERT Effect)
	//if(uNumInputChannels == 2 && uNumOutputChannels == 2)
	//	pOutputBuffer[1] = m_bitcrusher.do_bitcrush_right(pInputBuffer[1]);


	return true;
}


/* ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! ----------------------------------- //
   	**--0x2983--**

UIList Index	Variable Name					Control Index		
-------------------------------------------------------------------
0				m_wavetableosc_wavetable          0
1				m_wtf_dial                        1
2				m_use_chiptune_noise              2
3				m_delay_ducking                   4
4				m_delay_filter_freq               5
5				m_delay_dry                       6
6				m_delay_wet                       7
7				m_delay_feedback                  8
8				m_delay_time                      9
9				m_arp1_semitones                  10
10				m_arp2_semitones                  11
11				m_arp3_semitones                  12
12				m_arp_speed                       13
13				m_arp_step3_active                22
14				m_analog_drift                    30
15				m_pwm_duty                        31
16				m_distortion_type                 36
17				m_bitcrush_dry_wet                37
18				m_bitcrush_bits                   38
19				m_bitcrush_edge                   39
20				m_2D_position                     100
21				m_multiosc_detune                 101
22				m_multiosc_wt_spread              102
23				m_distortion_bias                 106
24				m_distortion_threshold            107
25				m_use_distortion                  108
26				m_shaper_amount                   109
27				m_xy_pad_x                        110
28				m_xy_pad_y                        111
29				m_flanger_dry_wet                 114
30				m_env_loop                        115
31				m_attack                          116
32				m_decay                           117
33				m_sustain                         118
34				m_release                         119
35				m_noise_hp                        120
36				m_noise_lp                        121
37				m_phaser_lfo_freq                 122
38				m_phaser_lfo_amplitude            123
39				m_phaser_mod                   124
40				m_use_allpass                     125
41				m_comb_positive                   126
42				m_allpass_radius                  127
43				m_formant_width                   128
44				m_filter_transition               129
45				m_FM_amount                       130
46				m_FM_carrier_wave                 131
47				m_FM_modulator_wave               132
48				m_FM_exponential                  133
49				m_carrier_ratio                   134
50				m_modulator_ratio                 135
51				m_filter_type                     136
52				m_filter_saturation               137
53				m_filter_freq                     138
54				m_filter_res                      139

	Assignable Buttons               Index
-----------------------------------------------
	B1                                50
	B2                                51
	B3                                52

-----------------------------------------------
	Joystick List Boxes (Classic)    Index
-----------------------------------------------
	Drop List A                       60
	Drop List B                       61
	Drop List C                       62
	Drop List D                       63

-----------------------------------------------

	**--0xFFDD--**
// ------------------------------------------------------------------------------- */
// Add your UI Handler code here ------------------------------------------------- //
//
bool __stdcall COdin::userInterfaceChange(int nControlIndex)
{
	// decode the control index, or delete the switch and use brute force calls
	switch(nControlIndex)
	{
		case 0:
		{
			break;
		}

		default:
			break;
	}

	return true;
}

// --- message for updating GUI from plugin; see the comment block above userInterfaceChange( ) for
//     the index values to use when sending outbound parameter changes
//     see www.willpirkle.com for information on using this function to update the GUI from your plugin
//     The bLoadingPreset flag will be set if this is being called as a result of a preset load; you may
//     want to ignore GUI updates for presets depending on how your update works!
//     NOTE: this function will be called even if no audio is flowing (unlike userInterfaceChange( ) which
//           will only get called while in the audio processing loop (see Thread Safety document on website)
bool __stdcall COdin::checkUpdateGUI(int nControlIndex, float fValue, CLinkedList<GUI_PARAMETER>& guiParameters, bool bLoadingPreset)
{
	// decode the control index
	switch(nControlIndex)
	{
		case 0:
		{
			// return true; // if update needed
			// break;		// if no update needed
		}

		default:
			break;
	}
	return false;
}

// --- process aux inputs
//     This function will be called once for each Aux Input bus, currently:
//
//     Aux Input 1: Sidechain
//     May add more input busses in the future
//
//     see www.willpirkle.com for info on using the Aux input bus
bool __stdcall COdin::processAuxInputBus(audioProcessData* pAudioProcessData)
{
	/* --- pick up pointers to the Aux Input busses for sidechain or other Aux processing

	     	Ordinarily, you just copy the buffer pointers to member variables and then
	    	use the pointers in your process( ) function.

	     	However, you also have the option of pre-processing the Aux inputs in-place
	     	in these buffers, though no idea when you might need that...

	Example:
	if(pAudioProcessData->uInputBus == 1)
	{
		// --- save varius pointers, in practice you only save the
		//     pointer you need for your particular process( ) function
		//     Note these are member variables you need to declare on your own in the .h file!
		//
		m_pSidechainFrameBuffer = pAudioProcessData->pFrameInputBuffer; // <-- for processAudioFrame( )
		m_pSidechainRAFXBuffer = pAudioProcessData->pRAFXInputBuffer;	// <-- for processRackAFXAudioBuffer
		m_ppSidechainVSTBuffer = pAudioProcessData->ppVSTInputBuffer;	// <-- for processVSTAudioBuffer

		// --- sidechain input activation/channels
		m_bSidechainEnabled = pAudioProcessData->bInputEnabled;
		m_uSidechainChannelCount = pAudioProcessData->uNumInputChannels;
	}

	*/

	return true;
}

/* joystickControlChange

	Indicates the user moved the joystick point; the variables are the relative mixes
	of each axis; the values will add up to 1.0

			B
			|
		A -	x -	C
			|
			D

	The point in the very center (x) would be:
	fControlA = 0.25
	fControlB = 0.25
	fControlC = 0.25
	fControlD = 0.25

	AC Mix = projection on X Axis (0 -> 1)
	BD Mix = projection on Y Axis (0 -> 1)
*/
bool __stdcall COdin::joystickControlChange(float fControlA, float fControlB, float fControlC, float fControlD, float fACMix, float fBDMix)
{
	// add your code here

	return true;
}



/* processAudioBuffer

	// ALL VALUES IN AND OUT ON THE RANGE OF -1.0 TO + 1.0

	The I/O buffers are interleaved depending on the number of channels. If uNumChannels = 2, then the
	buffer is L/R/L/R/L/R etc...

	if uNumChannels = 6 then the buffer is L/R/C/Sub/BL/BR etc...

	It is up to you to decode and de-interleave the data.

	To use this function set m_bWantBuffers = true in your constructor.

	******************************
	********* IMPORTANT! *********
	******************************
	If you are going to ultimately use <Make VST> or <Make AU> to port your project and you want to process
	buffers instead of frames, you need to override processVSTAudioBuffer() below instead;

	processRackAFXAudioBuffer() is NOT supported in <Make VST>, <Make AU>, and <Make AAX>

	HOST INFORMATION is available in m_HostProcessInfo:

	m_HostProcessInfo.uAbsoluteSampleBufferIndex = sample index of top of current audio buffer
	m_HostProcessInfo.dAbsoluteSampleBufferTime = time (sec) of sample in top of current audio buffer
	m_HostProcessInfo.dBPM = Host Tempo setting in BPM
	m_HostProcessInfo.fTimeSigNumerator = Host Time Signature Numerator (if supported by host)
	m_HostProcessInfo.uTimeSigDenomintor = Host Time Signature Denominator (if supported by host)
*/
bool __stdcall COdin::processRackAFXAudioBuffer(float* pInputBuffer, float* pOutputBuffer,
													   UINT uNumInputChannels, UINT uNumOutputChannels,
													   UINT uBufferSize)
{

	for(UINT i=0; i<uBufferSize; i++)
	{
		// smooth parameters (if enabled) DO NOT REMOVE
		smoothParameterValues(); // done on a per-sample-interval basis

		// pass through code
		pOutputBuffer[i] = pInputBuffer[i];
	}


	return true;
}



/* processVSTAudioBuffer

	// ALL VALUES IN AND OUT ON THE RANGE OF -1.0 TO + 1.0

	NOTE: You do not have to implement this function if you don't want to; the processAudioFrame()
	will still work; however this using function will be more CPU efficient for your plug-in, and will
	override processAudioFrame().

	To use this function set m_bWantVSTBuffers = true in your constructor.

	The VST input and output buffers are pointers-to-pointers. The pp buffers are the same depth as uNumChannels, so
	if uNumChannels = 2, then ppInputs would contain two pointers,

		inBuffer[0] = a pointer to the LEFT buffer of data
		inBuffer[1] = a pointer to the RIGHT buffer of data

	Similarly, outBuffer would have 2 pointers, one for left and one for right.

	For 5.1 audio you would get 6 pointers in each buffer.

*/
bool __stdcall COdin::processVSTAudioBuffer(float** inBuffer, float** outBuffer, UINT uNumChannels, int inFramesToProcess)
{
	// PASS Through example
	// MONO First
	float* pInputL  = inBuffer[0];
	float* pOutputL = outBuffer[0];
	float* pInputR  = NULL;
	float* pOutputR = NULL;

	// if STEREO,
	if(inBuffer[1])
		pInputR = inBuffer[1];

	if(outBuffer[1])
		pOutputR = outBuffer[1];

	// Process audio by de-referencing ptrs
	// this is siple pass through code
	unsigned int uSample = 0;
	while (--inFramesToProcess >= 0)
	{
		// --- fire midi events (AU, VST2, AAX buffer processing only; not needed if you use processAudioFrame())
		if (m_pMidiEventList)
			m_pMidiEventList->fireMidiEvent(uSample++);

		// --- sample accurate automation for VST3 only
		doVSTSampleAccurateParamUpdates();

		// --- smooth parameters (if enabled) DO NOT REMOVE
		smoothParameterValues(); // done on a per-sample-interval basis

		// --- Left channel processing
		*pOutputL = *pInputL;

		// --- If there is a right channel
		if(pInputR && pOutputR)
			*pOutputR = *pInputR;
		else if(pOutputR) // 1->2 mapping
			*pOutputR = *pOutputL;

		// --- advance pointers
		pInputL++;
		pOutputL++;
		if(pInputR) pInputR++;
		if(pOutputR) pOutputR++;
	}
	// --- all OK
	return true;
}



float midiToFrequency(int p_midi_note){
	float ret = 440.f * pow(2,((float)(p_midi_note-69))/12.f);
	return ret;
}

bool __stdcall COdin::midiNoteOn(UINT uChannel, UINT uMIDINote, UINT uVelocity)
{	

	m_wave1D_osc.startOscillator();
	m_wave1D_osc.setBaseFrequency(midiToFrequency(uMIDINote));

	m_chiptune_osc.startOscillator();
	m_chiptune_osc.setBaseFrequency(midiToFrequency(uMIDINote));

	m_analog_osc.startOscillator();
	m_analog_osc.setBaseFrequency(midiToFrequency(uMIDINote));
	
	m_2D_osc.startOscillator();
	m_2D_osc.setBaseFrequency(midiToFrequency(uMIDINote));
	
	m_vec_osc.startOscillator();
	m_vec_osc.setBaseFrequency(midiToFrequency(uMIDINote));

	m_multi_osc.startOscillator();
	m_multi_osc.setBaseFrequency(midiToFrequency(uMIDINote));

	m_FM_osc.startOscillator();
	m_FM_osc.setBaseFrequency(midiToFrequency(uMIDINote));

	m_noise_osc.startOscillator();

	m_adsr.reset();
	return true;
}

bool __stdcall COdin::midiNoteOff(UINT uChannel, UINT uMIDINote, UINT uVelocity, bool bAllNotesOff)
{
	//m_chiptune_osc.stopOscillator();
	m_analog_osc.stopOscillator();
	m_2D_osc.stopOscillator();
	m_vec_osc.stopOscillator();
	m_multi_osc.stopOscillator();
	m_noise_osc.stopOscillator();
	m_FM_osc.stopOscillator();
	m_wave1D_osc.stopOscillator();
	m_adsr.startRelease();
	return true;
}

// uModValue = 0->127
bool __stdcall COdin::midiModWheel(UINT uChannel, UINT uModValue)
{
	return true;
}

// nActualPitchBendValue 		= -8192 -> +8191, 0 at center
// fNormalizedPitchBendValue 	= -1.0  -> +1.0,  0 at center
bool __stdcall COdin::midiPitchBend(UINT uChannel, int nActualPitchBendValue, float fNormalizedPitchBendValue)
{
	return true;
}

// MIDI Clock
// http://home.roadrunner.com/~jgglatt/tech/midispec/clock.htm
/* There are 24 MIDI Clocks in every quarter note. (12 MIDI Clocks in an eighth note, 6 MIDI Clocks in a 16th, etc).
   Therefore, when a slave device counts down the receipt of 24 MIDI Clock messages, it knows that one quarter note
   has passed. When the slave counts off another 24 MIDI Clock messages, it knows that another quarter note has passed.
   Etc. Of course, the rate that the master sends these messages is based upon the master's tempo.

   For example, for a tempo of 120 BPM (ie, there are 120 quarter notes in every minute), the master sends a MIDI clock
   every 20833 microseconds. (ie, There are 1,000,000 microseconds in a second. Therefore, there are 60,000,000
   microseconds in a minute. At a tempo of 120 BPM, there are 120 quarter notes per minute. There are 24 MIDI clocks
   in each quarter note. Therefore, there should be 24 * 120 MIDI Clocks per minute.
   So, each MIDI Clock is sent at a rate of 60,000,000/(24 * 120) microseconds).
*/
bool __stdcall COdin::midiClock()
{

	return true;
}

// any midi message other than note on, note off, pitchbend, mod wheel or clock
bool __stdcall COdin::midiMessage(unsigned char cChannel, unsigned char cStatus, unsigned char
						   				  cData1, unsigned char cData2)
{
	return true;
}

/* doVSTSampleAccurateParamUpdates
	Short handler for VST3 sample accurate automation added in v6.8.0.5
	There is nothing for you to modify here.
*/
void COdin::doVSTSampleAccurateParamUpdates()
{
	// --- for sample accurate parameter automation in VST3 plugins; ignore otherwise
	if (!m_ppControlTable) return; /// should NEVER happen
	for (int i = 0; i < m_uControlListCount; i++)
	{
		if (m_ppControlTable[i] && m_ppControlTable[i]->pvAddlData)
		{
			double dValue = 0;
			if (((IParamUpdateQueue *)m_ppControlTable[i]->pvAddlData)->getNextValue(dValue))
			{
				setNormalizedParameter(m_ppControlTable[i], dValue, true);
			}
		}
	}
}

// --- showGUI()
//     This is the main interface function for all GUIs, including custom GUIs
//     It is also where you deal with Custom Controls (see Advanced GUI API)
void* __stdcall COdin::showGUI(void* pInfo)
{
	// --- ALWAYS try base class first in case of future updates
	void* result = CPlugIn::showGUI(pInfo);
	if(result)
		return result;

	/* Uncomment if using advanced GUI API: see www.willpirkle.com for details and sample code
	// --- uncloak the info struct
	VSTGUI_VIEW_INFO* info = (VSTGUI_VIEW_INFO*)pInfo;
	if(!info) return NULL;

	switch(info->message)
	{
		case GUI_DID_OPEN:
		{
			return NULL;
		}
		case GUI_WILL_CLOSE:
		{
			return NULL;
		}
		case GUI_CUSTOMVIEW:
		{
			// --- create custom view, return a CView* cloaked as void* or NULL if not supported
			return NULL;
		}

		case GUI_HAS_USER_CUSTOM:
		{
			// --- set this variable to true if you have a custom GUI
			info->bHasUserCustomView = false;
			return NULL;
		}

		// --- create your custom VSTGUI4 object using the CVSTGUIController (supplied),
		//     a subclass of the CVSTGUIController that you supply, a VSTGUI4 object
		//     that is derived at least from: VSTGUIEditorInterface, CControlListener, CBaseObject
		//     see VSTGUIController.h for an example
		//
		//     open() sets the new size of the window in info->size
		//     return a pointer to the newly created object
		case GUI_USER_CUSTOM_OPEN:
		{
			return NULL;
		}
		// --- call the close() function and delete the controller object
		case GUI_USER_CUSTOM_CLOSE:
		{
			return NULL;
		}
		// --- handle paint-specific timer stuff
		case GUI_TIMER_PING:
		{

			return NULL;
		}
	} */

	return NULL;
}

// --- DO NOT EDIT OR DELETE THIS FUNCTION ----------------------------------------------- //
bool __stdcall COdin::initUI()
{
	// ADDED BY RACKAFX -- DO NOT EDIT THIS CODE!!! -------------------------------------- //
	if(m_UIControlList.count() > 0)
		return true;

// **--0xDEA7--**

	std::vector<CUICtrl*> uiCtrls;

	int nIndexer = 0;
	m_wavetableosc_wavetable = 14;
	CUICtrl* ui0 = new CUICtrl;
	ui0->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui0->uControlId = 0;
	ui0->bLogSlider = false;
	ui0->bExpSlider = false;
	ui0->fUserDisplayDataLoLimit = 0.000000;
	ui0->fUserDisplayDataHiLimit = 14.000000;
	ui0->uUserDataType = intData;
	ui0->fInitUserIntValue = 14.000000;
	ui0->fInitUserFloatValue = 0;
	ui0->fInitUserDoubleValue = 0;
	ui0->fInitUserUINTValue = 0;
	ui0->m_pUserCookedIntData = &m_wavetableosc_wavetable;
	ui0->m_pUserCookedFloatData = NULL;
	ui0->m_pUserCookedDoubleData = NULL;
	ui0->m_pUserCookedUINTData = NULL;
	ui0->cControlUnits = "";
	ui0->cVariableName = "m_wavetableosc_wavetable";
	ui0->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui0->dPresetData[0] = 14.000000;ui0->dPresetData[1] = 0.000000;ui0->dPresetData[2] = 0.000000;ui0->dPresetData[3] = 0.000000;ui0->dPresetData[4] = 0.000000;ui0->dPresetData[5] = 0.000000;ui0->dPresetData[6] = 0.000000;ui0->dPresetData[7] = 0.000000;ui0->dPresetData[8] = 0.000000;ui0->dPresetData[9] = 0.000000;ui0->dPresetData[10] = 0.000000;ui0->dPresetData[11] = 0.000000;ui0->dPresetData[12] = 0.000000;ui0->dPresetData[13] = 0.000000;ui0->dPresetData[14] = 0.000000;ui0->dPresetData[15] = 0.000000;
	ui0->cControlName = "WTOsc WT";
	ui0->bOwnerControl = false;
	ui0->bMIDIControl = false;
	ui0->uMIDIControlCommand = 176;
	ui0->uMIDIControlName = 3;
	ui0->uMIDIControlChannel = 0;
	ui0->nGUIRow = nIndexer++;
	ui0->nGUIColumn = -1;
	ui0->bEnableParamSmoothing = false;
	ui0->fSmoothingTimeInMs = 100.00;
	ui0->uControlTheme[0] = 0; ui0->uControlTheme[1] = 0; ui0->uControlTheme[2] = 0; ui0->uControlTheme[3] = 0; ui0->uControlTheme[4] = 0; ui0->uControlTheme[5] = 0; ui0->uControlTheme[6] = 0; ui0->uControlTheme[7] = 0; ui0->uControlTheme[8] = 4; ui0->uControlTheme[9] = 0; ui0->uControlTheme[10] = 0; ui0->uControlTheme[11] = 0; ui0->uControlTheme[12] = 0; ui0->uControlTheme[13] = 0; ui0->uControlTheme[14] = 0; ui0->uControlTheme[15] = 0; ui0->uControlTheme[16] = 2; ui0->uControlTheme[17] = 0; ui0->uControlTheme[18] = 0; ui0->uControlTheme[19] = 0; ui0->uControlTheme[20] = 0; ui0->uControlTheme[21] = 0; ui0->uControlTheme[22] = 0; ui0->uControlTheme[23] = 0; ui0->uControlTheme[24] = 0; ui0->uControlTheme[25] = 0; ui0->uControlTheme[26] = 0; ui0->uControlTheme[27] = 0; ui0->uControlTheme[28] = 0; ui0->uControlTheme[29] = 0; ui0->uControlTheme[30] = 0; ui0->uControlTheme[31] = 0; 
	ui0->uFluxCapControl[0] = 0; ui0->uFluxCapControl[1] = 0; ui0->uFluxCapControl[2] = 0; ui0->uFluxCapControl[3] = 0; ui0->uFluxCapControl[4] = 0; ui0->uFluxCapControl[5] = 0; ui0->uFluxCapControl[6] = 0; ui0->uFluxCapControl[7] = 0; ui0->uFluxCapControl[8] = 0; ui0->uFluxCapControl[9] = 0; ui0->uFluxCapControl[10] = 0; ui0->uFluxCapControl[11] = 0; ui0->uFluxCapControl[12] = 0; ui0->uFluxCapControl[13] = 0; ui0->uFluxCapControl[14] = 0; ui0->uFluxCapControl[15] = 0; ui0->uFluxCapControl[16] = 0; ui0->uFluxCapControl[17] = 0; ui0->uFluxCapControl[18] = 0; ui0->uFluxCapControl[19] = 0; ui0->uFluxCapControl[20] = 0; ui0->uFluxCapControl[21] = 0; ui0->uFluxCapControl[22] = 0; ui0->uFluxCapControl[23] = 0; ui0->uFluxCapControl[24] = 0; ui0->uFluxCapControl[25] = 0; ui0->uFluxCapControl[26] = 0; ui0->uFluxCapControl[27] = 0; ui0->uFluxCapControl[28] = 0; ui0->uFluxCapControl[29] = 0; ui0->uFluxCapControl[30] = 0; ui0->uFluxCapControl[31] = 0; ui0->uFluxCapControl[32] = 0; ui0->uFluxCapControl[33] = 0; ui0->uFluxCapControl[34] = 0; ui0->uFluxCapControl[35] = 0; ui0->uFluxCapControl[36] = 0; ui0->uFluxCapControl[37] = 0; ui0->uFluxCapControl[38] = 0; ui0->uFluxCapControl[39] = 0; ui0->uFluxCapControl[40] = 0; ui0->uFluxCapControl[41] = 0; ui0->uFluxCapControl[42] = 0; ui0->uFluxCapControl[43] = 0; ui0->uFluxCapControl[44] = 0; ui0->uFluxCapControl[45] = 0; ui0->uFluxCapControl[46] = 0; ui0->uFluxCapControl[47] = 0; ui0->uFluxCapControl[48] = 0; ui0->uFluxCapControl[49] = 0; ui0->uFluxCapControl[50] = 0; ui0->uFluxCapControl[51] = 0; ui0->uFluxCapControl[52] = 0; ui0->uFluxCapControl[53] = 0; ui0->uFluxCapControl[54] = 0; ui0->uFluxCapControl[55] = 0; ui0->uFluxCapControl[56] = 0; ui0->uFluxCapControl[57] = 0; ui0->uFluxCapControl[58] = 0; ui0->uFluxCapControl[59] = 0; ui0->uFluxCapControl[60] = 0; ui0->uFluxCapControl[61] = 0; ui0->uFluxCapControl[62] = 0; ui0->uFluxCapControl[63] = 0; 
	ui0->fFluxCapData[0] = 0.000000; ui0->fFluxCapData[1] = 0.000000; ui0->fFluxCapData[2] = 0.000000; ui0->fFluxCapData[3] = 0.000000; ui0->fFluxCapData[4] = 0.000000; ui0->fFluxCapData[5] = 0.000000; ui0->fFluxCapData[6] = 0.000000; ui0->fFluxCapData[7] = 0.000000; ui0->fFluxCapData[8] = 0.000000; ui0->fFluxCapData[9] = 0.000000; ui0->fFluxCapData[10] = 0.000000; ui0->fFluxCapData[11] = 0.000000; ui0->fFluxCapData[12] = 0.000000; ui0->fFluxCapData[13] = 0.000000; ui0->fFluxCapData[14] = 0.000000; ui0->fFluxCapData[15] = 0.000000; ui0->fFluxCapData[16] = 0.000000; ui0->fFluxCapData[17] = 0.000000; ui0->fFluxCapData[18] = 0.000000; ui0->fFluxCapData[19] = 0.000000; ui0->fFluxCapData[20] = 0.000000; ui0->fFluxCapData[21] = 0.000000; ui0->fFluxCapData[22] = 0.000000; ui0->fFluxCapData[23] = 0.000000; ui0->fFluxCapData[24] = 0.000000; ui0->fFluxCapData[25] = 0.000000; ui0->fFluxCapData[26] = 0.000000; ui0->fFluxCapData[27] = 0.000000; ui0->fFluxCapData[28] = 0.000000; ui0->fFluxCapData[29] = 0.000000; ui0->fFluxCapData[30] = 0.000000; ui0->fFluxCapData[31] = 0.000000; ui0->fFluxCapData[32] = 0.000000; ui0->fFluxCapData[33] = 0.000000; ui0->fFluxCapData[34] = 0.000000; ui0->fFluxCapData[35] = 0.000000; ui0->fFluxCapData[36] = 0.000000; ui0->fFluxCapData[37] = 0.000000; ui0->fFluxCapData[38] = 0.000000; ui0->fFluxCapData[39] = 0.000000; ui0->fFluxCapData[40] = 0.000000; ui0->fFluxCapData[41] = 0.000000; ui0->fFluxCapData[42] = 0.000000; ui0->fFluxCapData[43] = 0.000000; ui0->fFluxCapData[44] = 0.000000; ui0->fFluxCapData[45] = 0.000000; ui0->fFluxCapData[46] = 0.000000; ui0->fFluxCapData[47] = 0.000000; ui0->fFluxCapData[48] = 0.000000; ui0->fFluxCapData[49] = 0.000000; ui0->fFluxCapData[50] = 0.000000; ui0->fFluxCapData[51] = 0.000000; ui0->fFluxCapData[52] = 0.000000; ui0->fFluxCapData[53] = 0.000000; ui0->fFluxCapData[54] = 0.000000; ui0->fFluxCapData[55] = 0.000000; ui0->fFluxCapData[56] = 0.000000; ui0->fFluxCapData[57] = 0.000000; ui0->fFluxCapData[58] = 0.000000; ui0->fFluxCapData[59] = 0.000000; ui0->fFluxCapData[60] = 0.000000; ui0->fFluxCapData[61] = 0.000000; ui0->fFluxCapData[62] = 0.000000; ui0->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui0);


	m_wtf_dial = 0;
	CUICtrl* ui1 = new CUICtrl;
	ui1->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui1->uControlId = 1;
	ui1->bLogSlider = false;
	ui1->bExpSlider = false;
	ui1->fUserDisplayDataLoLimit = 0.000000;
	ui1->fUserDisplayDataHiLimit = 14.000000;
	ui1->uUserDataType = intData;
	ui1->fInitUserIntValue = 0.000000;
	ui1->fInitUserFloatValue = 0;
	ui1->fInitUserDoubleValue = 0;
	ui1->fInitUserUINTValue = 0;
	ui1->m_pUserCookedIntData = &m_wtf_dial;
	ui1->m_pUserCookedFloatData = NULL;
	ui1->m_pUserCookedDoubleData = NULL;
	ui1->m_pUserCookedUINTData = NULL;
	ui1->cControlUnits = "Units";
	ui1->cVariableName = "m_wtf_dial";
	ui1->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui1->dPresetData[0] = 0.000000;ui1->dPresetData[1] = 0.000000;ui1->dPresetData[2] = 0.000000;ui1->dPresetData[3] = 0.000000;ui1->dPresetData[4] = 0.000000;ui1->dPresetData[5] = 0.000000;ui1->dPresetData[6] = 0.000000;ui1->dPresetData[7] = 0.000000;ui1->dPresetData[8] = 0.000000;ui1->dPresetData[9] = 0.000000;ui1->dPresetData[10] = 0.000000;ui1->dPresetData[11] = 0.000000;ui1->dPresetData[12] = 0.000000;ui1->dPresetData[13] = 0.000000;ui1->dPresetData[14] = 0.000000;ui1->dPresetData[15] = 0.000000;
	ui1->cControlName = "WTFDIAL";
	ui1->bOwnerControl = false;
	ui1->bMIDIControl = false;
	ui1->uMIDIControlCommand = 176;
	ui1->uMIDIControlName = 3;
	ui1->uMIDIControlChannel = 0;
	ui1->nGUIRow = nIndexer++;
	ui1->nGUIColumn = -1;
	ui1->bEnableParamSmoothing = false;
	ui1->fSmoothingTimeInMs = 100.00;
	ui1->uControlTheme[0] = 0; ui1->uControlTheme[1] = 0; ui1->uControlTheme[2] = 0; ui1->uControlTheme[3] = 0; ui1->uControlTheme[4] = 0; ui1->uControlTheme[5] = 0; ui1->uControlTheme[6] = 0; ui1->uControlTheme[7] = 0; ui1->uControlTheme[8] = 0; ui1->uControlTheme[9] = 0; ui1->uControlTheme[10] = 0; ui1->uControlTheme[11] = 0; ui1->uControlTheme[12] = 0; ui1->uControlTheme[13] = 0; ui1->uControlTheme[14] = 0; ui1->uControlTheme[15] = 0; ui1->uControlTheme[16] = 2; ui1->uControlTheme[17] = 0; ui1->uControlTheme[18] = 0; ui1->uControlTheme[19] = 0; ui1->uControlTheme[20] = 0; ui1->uControlTheme[21] = 0; ui1->uControlTheme[22] = 0; ui1->uControlTheme[23] = 0; ui1->uControlTheme[24] = 0; ui1->uControlTheme[25] = 0; ui1->uControlTheme[26] = 0; ui1->uControlTheme[27] = 0; ui1->uControlTheme[28] = 0; ui1->uControlTheme[29] = 0; ui1->uControlTheme[30] = 0; ui1->uControlTheme[31] = 0; 
	ui1->uFluxCapControl[0] = 0; ui1->uFluxCapControl[1] = 0; ui1->uFluxCapControl[2] = 0; ui1->uFluxCapControl[3] = 0; ui1->uFluxCapControl[4] = 0; ui1->uFluxCapControl[5] = 0; ui1->uFluxCapControl[6] = 0; ui1->uFluxCapControl[7] = 0; ui1->uFluxCapControl[8] = 0; ui1->uFluxCapControl[9] = 0; ui1->uFluxCapControl[10] = 0; ui1->uFluxCapControl[11] = 0; ui1->uFluxCapControl[12] = 0; ui1->uFluxCapControl[13] = 0; ui1->uFluxCapControl[14] = 0; ui1->uFluxCapControl[15] = 0; ui1->uFluxCapControl[16] = 0; ui1->uFluxCapControl[17] = 0; ui1->uFluxCapControl[18] = 0; ui1->uFluxCapControl[19] = 0; ui1->uFluxCapControl[20] = 0; ui1->uFluxCapControl[21] = 0; ui1->uFluxCapControl[22] = 0; ui1->uFluxCapControl[23] = 0; ui1->uFluxCapControl[24] = 0; ui1->uFluxCapControl[25] = 0; ui1->uFluxCapControl[26] = 0; ui1->uFluxCapControl[27] = 0; ui1->uFluxCapControl[28] = 0; ui1->uFluxCapControl[29] = 0; ui1->uFluxCapControl[30] = 0; ui1->uFluxCapControl[31] = 0; ui1->uFluxCapControl[32] = 0; ui1->uFluxCapControl[33] = 0; ui1->uFluxCapControl[34] = 0; ui1->uFluxCapControl[35] = 0; ui1->uFluxCapControl[36] = 0; ui1->uFluxCapControl[37] = 0; ui1->uFluxCapControl[38] = 0; ui1->uFluxCapControl[39] = 0; ui1->uFluxCapControl[40] = 0; ui1->uFluxCapControl[41] = 0; ui1->uFluxCapControl[42] = 0; ui1->uFluxCapControl[43] = 0; ui1->uFluxCapControl[44] = 0; ui1->uFluxCapControl[45] = 0; ui1->uFluxCapControl[46] = 0; ui1->uFluxCapControl[47] = 0; ui1->uFluxCapControl[48] = 0; ui1->uFluxCapControl[49] = 0; ui1->uFluxCapControl[50] = 0; ui1->uFluxCapControl[51] = 0; ui1->uFluxCapControl[52] = 0; ui1->uFluxCapControl[53] = 0; ui1->uFluxCapControl[54] = 0; ui1->uFluxCapControl[55] = 0; ui1->uFluxCapControl[56] = 0; ui1->uFluxCapControl[57] = 0; ui1->uFluxCapControl[58] = 0; ui1->uFluxCapControl[59] = 0; ui1->uFluxCapControl[60] = 0; ui1->uFluxCapControl[61] = 0; ui1->uFluxCapControl[62] = 0; ui1->uFluxCapControl[63] = 0; 
	ui1->fFluxCapData[0] = 0.000000; ui1->fFluxCapData[1] = 0.000000; ui1->fFluxCapData[2] = 0.000000; ui1->fFluxCapData[3] = 0.000000; ui1->fFluxCapData[4] = 0.000000; ui1->fFluxCapData[5] = 0.000000; ui1->fFluxCapData[6] = 0.000000; ui1->fFluxCapData[7] = 0.000000; ui1->fFluxCapData[8] = 0.000000; ui1->fFluxCapData[9] = 0.000000; ui1->fFluxCapData[10] = 0.000000; ui1->fFluxCapData[11] = 0.000000; ui1->fFluxCapData[12] = 0.000000; ui1->fFluxCapData[13] = 0.000000; ui1->fFluxCapData[14] = 0.000000; ui1->fFluxCapData[15] = 0.000000; ui1->fFluxCapData[16] = 0.000000; ui1->fFluxCapData[17] = 0.000000; ui1->fFluxCapData[18] = 0.000000; ui1->fFluxCapData[19] = 0.000000; ui1->fFluxCapData[20] = 0.000000; ui1->fFluxCapData[21] = 0.000000; ui1->fFluxCapData[22] = 0.000000; ui1->fFluxCapData[23] = 0.000000; ui1->fFluxCapData[24] = 0.000000; ui1->fFluxCapData[25] = 0.000000; ui1->fFluxCapData[26] = 0.000000; ui1->fFluxCapData[27] = 0.000000; ui1->fFluxCapData[28] = 0.000000; ui1->fFluxCapData[29] = 0.000000; ui1->fFluxCapData[30] = 0.000000; ui1->fFluxCapData[31] = 0.000000; ui1->fFluxCapData[32] = 0.000000; ui1->fFluxCapData[33] = 0.000000; ui1->fFluxCapData[34] = 0.000000; ui1->fFluxCapData[35] = 0.000000; ui1->fFluxCapData[36] = 0.000000; ui1->fFluxCapData[37] = 0.000000; ui1->fFluxCapData[38] = 0.000000; ui1->fFluxCapData[39] = 0.000000; ui1->fFluxCapData[40] = 0.000000; ui1->fFluxCapData[41] = 0.000000; ui1->fFluxCapData[42] = 0.000000; ui1->fFluxCapData[43] = 0.000000; ui1->fFluxCapData[44] = 0.000000; ui1->fFluxCapData[45] = 0.000000; ui1->fFluxCapData[46] = 0.000000; ui1->fFluxCapData[47] = 0.000000; ui1->fFluxCapData[48] = 0.000000; ui1->fFluxCapData[49] = 0.000000; ui1->fFluxCapData[50] = 0.000000; ui1->fFluxCapData[51] = 0.000000; ui1->fFluxCapData[52] = 0.000000; ui1->fFluxCapData[53] = 0.000000; ui1->fFluxCapData[54] = 0.000000; ui1->fFluxCapData[55] = 0.000000; ui1->fFluxCapData[56] = 0.000000; ui1->fFluxCapData[57] = 0.000000; ui1->fFluxCapData[58] = 0.000000; ui1->fFluxCapData[59] = 0.000000; ui1->fFluxCapData[60] = 0.000000; ui1->fFluxCapData[61] = 0.000000; ui1->fFluxCapData[62] = 0.000000; ui1->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui1);


	m_use_chiptune_noise = 0;
	CUICtrl* ui2 = new CUICtrl;
	ui2->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui2->uControlId = 2;
	ui2->bLogSlider = false;
	ui2->bExpSlider = false;
	ui2->fUserDisplayDataLoLimit = 0.000000;
	ui2->fUserDisplayDataHiLimit = 1.000000;
	ui2->uUserDataType = intData;
	ui2->fInitUserIntValue = 0.000000;
	ui2->fInitUserFloatValue = 0;
	ui2->fInitUserDoubleValue = 0;
	ui2->fInitUserUINTValue = 0;
	ui2->m_pUserCookedIntData = &m_use_chiptune_noise;
	ui2->m_pUserCookedFloatData = NULL;
	ui2->m_pUserCookedDoubleData = NULL;
	ui2->m_pUserCookedUINTData = NULL;
	ui2->cControlUnits = "";
	ui2->cVariableName = "m_use_chiptune_noise";
	ui2->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui2->dPresetData[0] = 0.000000;ui2->dPresetData[1] = 0.000000;ui2->dPresetData[2] = 0.000000;ui2->dPresetData[3] = 0.000000;ui2->dPresetData[4] = 0.000000;ui2->dPresetData[5] = 0.000000;ui2->dPresetData[6] = 0.000000;ui2->dPresetData[7] = 0.000000;ui2->dPresetData[8] = 0.000000;ui2->dPresetData[9] = 0.000000;ui2->dPresetData[10] = 0.000000;ui2->dPresetData[11] = 0.000000;ui2->dPresetData[12] = 0.000000;ui2->dPresetData[13] = 0.000000;ui2->dPresetData[14] = 0.000000;ui2->dPresetData[15] = 0.000000;
	ui2->cControlName = "Use Chiptune Noisej";
	ui2->bOwnerControl = false;
	ui2->bMIDIControl = false;
	ui2->uMIDIControlCommand = 176;
	ui2->uMIDIControlName = 3;
	ui2->uMIDIControlChannel = 0;
	ui2->nGUIRow = nIndexer++;
	ui2->nGUIColumn = -1;
	ui2->bEnableParamSmoothing = false;
	ui2->fSmoothingTimeInMs = 100.00;
	ui2->uControlTheme[0] = 0; ui2->uControlTheme[1] = 0; ui2->uControlTheme[2] = 0; ui2->uControlTheme[3] = 0; ui2->uControlTheme[4] = 0; ui2->uControlTheme[5] = 0; ui2->uControlTheme[6] = 0; ui2->uControlTheme[7] = 0; ui2->uControlTheme[8] = 0; ui2->uControlTheme[9] = 0; ui2->uControlTheme[10] = 0; ui2->uControlTheme[11] = 0; ui2->uControlTheme[12] = 0; ui2->uControlTheme[13] = 0; ui2->uControlTheme[14] = 0; ui2->uControlTheme[15] = 0; ui2->uControlTheme[16] = 2; ui2->uControlTheme[17] = 0; ui2->uControlTheme[18] = 0; ui2->uControlTheme[19] = 0; ui2->uControlTheme[20] = 0; ui2->uControlTheme[21] = 0; ui2->uControlTheme[22] = 0; ui2->uControlTheme[23] = 0; ui2->uControlTheme[24] = 0; ui2->uControlTheme[25] = 0; ui2->uControlTheme[26] = 0; ui2->uControlTheme[27] = 0; ui2->uControlTheme[28] = 0; ui2->uControlTheme[29] = 0; ui2->uControlTheme[30] = 0; ui2->uControlTheme[31] = 0; 
	ui2->uFluxCapControl[0] = 0; ui2->uFluxCapControl[1] = 0; ui2->uFluxCapControl[2] = 0; ui2->uFluxCapControl[3] = 0; ui2->uFluxCapControl[4] = 0; ui2->uFluxCapControl[5] = 0; ui2->uFluxCapControl[6] = 0; ui2->uFluxCapControl[7] = 0; ui2->uFluxCapControl[8] = 0; ui2->uFluxCapControl[9] = 0; ui2->uFluxCapControl[10] = 0; ui2->uFluxCapControl[11] = 0; ui2->uFluxCapControl[12] = 0; ui2->uFluxCapControl[13] = 0; ui2->uFluxCapControl[14] = 0; ui2->uFluxCapControl[15] = 0; ui2->uFluxCapControl[16] = 0; ui2->uFluxCapControl[17] = 0; ui2->uFluxCapControl[18] = 0; ui2->uFluxCapControl[19] = 0; ui2->uFluxCapControl[20] = 0; ui2->uFluxCapControl[21] = 0; ui2->uFluxCapControl[22] = 0; ui2->uFluxCapControl[23] = 0; ui2->uFluxCapControl[24] = 0; ui2->uFluxCapControl[25] = 0; ui2->uFluxCapControl[26] = 0; ui2->uFluxCapControl[27] = 0; ui2->uFluxCapControl[28] = 0; ui2->uFluxCapControl[29] = 0; ui2->uFluxCapControl[30] = 0; ui2->uFluxCapControl[31] = 0; ui2->uFluxCapControl[32] = 0; ui2->uFluxCapControl[33] = 0; ui2->uFluxCapControl[34] = 0; ui2->uFluxCapControl[35] = 0; ui2->uFluxCapControl[36] = 0; ui2->uFluxCapControl[37] = 0; ui2->uFluxCapControl[38] = 0; ui2->uFluxCapControl[39] = 0; ui2->uFluxCapControl[40] = 0; ui2->uFluxCapControl[41] = 0; ui2->uFluxCapControl[42] = 0; ui2->uFluxCapControl[43] = 0; ui2->uFluxCapControl[44] = 0; ui2->uFluxCapControl[45] = 0; ui2->uFluxCapControl[46] = 0; ui2->uFluxCapControl[47] = 0; ui2->uFluxCapControl[48] = 0; ui2->uFluxCapControl[49] = 0; ui2->uFluxCapControl[50] = 0; ui2->uFluxCapControl[51] = 0; ui2->uFluxCapControl[52] = 0; ui2->uFluxCapControl[53] = 0; ui2->uFluxCapControl[54] = 0; ui2->uFluxCapControl[55] = 0; ui2->uFluxCapControl[56] = 0; ui2->uFluxCapControl[57] = 0; ui2->uFluxCapControl[58] = 0; ui2->uFluxCapControl[59] = 0; ui2->uFluxCapControl[60] = 0; ui2->uFluxCapControl[61] = 0; ui2->uFluxCapControl[62] = 0; ui2->uFluxCapControl[63] = 0; 
	ui2->fFluxCapData[0] = 0.000000; ui2->fFluxCapData[1] = 0.000000; ui2->fFluxCapData[2] = 0.000000; ui2->fFluxCapData[3] = 0.000000; ui2->fFluxCapData[4] = 0.000000; ui2->fFluxCapData[5] = 0.000000; ui2->fFluxCapData[6] = 0.000000; ui2->fFluxCapData[7] = 0.000000; ui2->fFluxCapData[8] = 0.000000; ui2->fFluxCapData[9] = 0.000000; ui2->fFluxCapData[10] = 0.000000; ui2->fFluxCapData[11] = 0.000000; ui2->fFluxCapData[12] = 0.000000; ui2->fFluxCapData[13] = 0.000000; ui2->fFluxCapData[14] = 0.000000; ui2->fFluxCapData[15] = 0.000000; ui2->fFluxCapData[16] = 0.000000; ui2->fFluxCapData[17] = 0.000000; ui2->fFluxCapData[18] = 0.000000; ui2->fFluxCapData[19] = 0.000000; ui2->fFluxCapData[20] = 0.000000; ui2->fFluxCapData[21] = 0.000000; ui2->fFluxCapData[22] = 0.000000; ui2->fFluxCapData[23] = 0.000000; ui2->fFluxCapData[24] = 0.000000; ui2->fFluxCapData[25] = 0.000000; ui2->fFluxCapData[26] = 0.000000; ui2->fFluxCapData[27] = 0.000000; ui2->fFluxCapData[28] = 0.000000; ui2->fFluxCapData[29] = 0.000000; ui2->fFluxCapData[30] = 0.000000; ui2->fFluxCapData[31] = 0.000000; ui2->fFluxCapData[32] = 0.000000; ui2->fFluxCapData[33] = 0.000000; ui2->fFluxCapData[34] = 0.000000; ui2->fFluxCapData[35] = 0.000000; ui2->fFluxCapData[36] = 0.000000; ui2->fFluxCapData[37] = 0.000000; ui2->fFluxCapData[38] = 0.000000; ui2->fFluxCapData[39] = 0.000000; ui2->fFluxCapData[40] = 0.000000; ui2->fFluxCapData[41] = 0.000000; ui2->fFluxCapData[42] = 0.000000; ui2->fFluxCapData[43] = 0.000000; ui2->fFluxCapData[44] = 0.000000; ui2->fFluxCapData[45] = 0.000000; ui2->fFluxCapData[46] = 0.000000; ui2->fFluxCapData[47] = 0.000000; ui2->fFluxCapData[48] = 0.000000; ui2->fFluxCapData[49] = 0.000000; ui2->fFluxCapData[50] = 0.000000; ui2->fFluxCapData[51] = 0.000000; ui2->fFluxCapData[52] = 0.000000; ui2->fFluxCapData[53] = 0.000000; ui2->fFluxCapData[54] = 0.000000; ui2->fFluxCapData[55] = 0.000000; ui2->fFluxCapData[56] = 0.000000; ui2->fFluxCapData[57] = 0.000000; ui2->fFluxCapData[58] = 0.000000; ui2->fFluxCapData[59] = 0.000000; ui2->fFluxCapData[60] = 0.000000; ui2->fFluxCapData[61] = 0.000000; ui2->fFluxCapData[62] = 0.000000; ui2->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui2);


	m_delay_ducking = 0.000000;
	CUICtrl* ui3 = new CUICtrl;
	ui3->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui3->uControlId = 4;
	ui3->bLogSlider = false;
	ui3->bExpSlider = false;
	ui3->fUserDisplayDataLoLimit = 0.000000;
	ui3->fUserDisplayDataHiLimit = 1.000000;
	ui3->uUserDataType = floatData;
	ui3->fInitUserIntValue = 0;
	ui3->fInitUserFloatValue = 0.000000;
	ui3->fInitUserDoubleValue = 0;
	ui3->fInitUserUINTValue = 0;
	ui3->m_pUserCookedIntData = NULL;
	ui3->m_pUserCookedFloatData = &m_delay_ducking;
	ui3->m_pUserCookedDoubleData = NULL;
	ui3->m_pUserCookedUINTData = NULL;
	ui3->cControlUnits = "";
	ui3->cVariableName = "m_delay_ducking";
	ui3->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui3->dPresetData[0] = 0.000000;ui3->dPresetData[1] = 0.000000;ui3->dPresetData[2] = 0.000000;ui3->dPresetData[3] = 0.000000;ui3->dPresetData[4] = 0.000000;ui3->dPresetData[5] = 0.000000;ui3->dPresetData[6] = 0.000000;ui3->dPresetData[7] = 0.000000;ui3->dPresetData[8] = 0.000000;ui3->dPresetData[9] = 0.000000;ui3->dPresetData[10] = 0.000000;ui3->dPresetData[11] = 0.000000;ui3->dPresetData[12] = 0.000000;ui3->dPresetData[13] = 0.000000;ui3->dPresetData[14] = 0.000000;ui3->dPresetData[15] = 0.000000;
	ui3->cControlName = "Delay Ducking";
	ui3->bOwnerControl = false;
	ui3->bMIDIControl = false;
	ui3->uMIDIControlCommand = 176;
	ui3->uMIDIControlName = 3;
	ui3->uMIDIControlChannel = 0;
	ui3->nGUIRow = nIndexer++;
	ui3->nGUIColumn = -1;
	ui3->bEnableParamSmoothing = false;
	ui3->fSmoothingTimeInMs = 100.00;
	ui3->uControlTheme[0] = 0; ui3->uControlTheme[1] = 0; ui3->uControlTheme[2] = 0; ui3->uControlTheme[3] = 0; ui3->uControlTheme[4] = 0; ui3->uControlTheme[5] = 0; ui3->uControlTheme[6] = 0; ui3->uControlTheme[7] = 0; ui3->uControlTheme[8] = 0; ui3->uControlTheme[9] = 0; ui3->uControlTheme[10] = 0; ui3->uControlTheme[11] = 0; ui3->uControlTheme[12] = 0; ui3->uControlTheme[13] = 0; ui3->uControlTheme[14] = 0; ui3->uControlTheme[15] = 0; ui3->uControlTheme[16] = 2; ui3->uControlTheme[17] = 0; ui3->uControlTheme[18] = 0; ui3->uControlTheme[19] = 0; ui3->uControlTheme[20] = 0; ui3->uControlTheme[21] = 0; ui3->uControlTheme[22] = 0; ui3->uControlTheme[23] = 0; ui3->uControlTheme[24] = 0; ui3->uControlTheme[25] = 0; ui3->uControlTheme[26] = 0; ui3->uControlTheme[27] = 0; ui3->uControlTheme[28] = 0; ui3->uControlTheme[29] = 0; ui3->uControlTheme[30] = 0; ui3->uControlTheme[31] = 0; 
	ui3->uFluxCapControl[0] = 0; ui3->uFluxCapControl[1] = 0; ui3->uFluxCapControl[2] = 0; ui3->uFluxCapControl[3] = 0; ui3->uFluxCapControl[4] = 0; ui3->uFluxCapControl[5] = 0; ui3->uFluxCapControl[6] = 0; ui3->uFluxCapControl[7] = 0; ui3->uFluxCapControl[8] = 0; ui3->uFluxCapControl[9] = 0; ui3->uFluxCapControl[10] = 0; ui3->uFluxCapControl[11] = 0; ui3->uFluxCapControl[12] = 0; ui3->uFluxCapControl[13] = 0; ui3->uFluxCapControl[14] = 0; ui3->uFluxCapControl[15] = 0; ui3->uFluxCapControl[16] = 0; ui3->uFluxCapControl[17] = 0; ui3->uFluxCapControl[18] = 0; ui3->uFluxCapControl[19] = 0; ui3->uFluxCapControl[20] = 0; ui3->uFluxCapControl[21] = 0; ui3->uFluxCapControl[22] = 0; ui3->uFluxCapControl[23] = 0; ui3->uFluxCapControl[24] = 0; ui3->uFluxCapControl[25] = 0; ui3->uFluxCapControl[26] = 0; ui3->uFluxCapControl[27] = 0; ui3->uFluxCapControl[28] = 0; ui3->uFluxCapControl[29] = 0; ui3->uFluxCapControl[30] = 0; ui3->uFluxCapControl[31] = 0; ui3->uFluxCapControl[32] = 0; ui3->uFluxCapControl[33] = 0; ui3->uFluxCapControl[34] = 0; ui3->uFluxCapControl[35] = 0; ui3->uFluxCapControl[36] = 0; ui3->uFluxCapControl[37] = 0; ui3->uFluxCapControl[38] = 0; ui3->uFluxCapControl[39] = 0; ui3->uFluxCapControl[40] = 0; ui3->uFluxCapControl[41] = 0; ui3->uFluxCapControl[42] = 0; ui3->uFluxCapControl[43] = 0; ui3->uFluxCapControl[44] = 0; ui3->uFluxCapControl[45] = 0; ui3->uFluxCapControl[46] = 0; ui3->uFluxCapControl[47] = 0; ui3->uFluxCapControl[48] = 0; ui3->uFluxCapControl[49] = 0; ui3->uFluxCapControl[50] = 0; ui3->uFluxCapControl[51] = 0; ui3->uFluxCapControl[52] = 0; ui3->uFluxCapControl[53] = 0; ui3->uFluxCapControl[54] = 0; ui3->uFluxCapControl[55] = 0; ui3->uFluxCapControl[56] = 0; ui3->uFluxCapControl[57] = 0; ui3->uFluxCapControl[58] = 0; ui3->uFluxCapControl[59] = 0; ui3->uFluxCapControl[60] = 0; ui3->uFluxCapControl[61] = 0; ui3->uFluxCapControl[62] = 0; ui3->uFluxCapControl[63] = 0; 
	ui3->fFluxCapData[0] = 0.000000; ui3->fFluxCapData[1] = 0.000000; ui3->fFluxCapData[2] = 0.000000; ui3->fFluxCapData[3] = 0.000000; ui3->fFluxCapData[4] = 0.000000; ui3->fFluxCapData[5] = 0.000000; ui3->fFluxCapData[6] = 0.000000; ui3->fFluxCapData[7] = 0.000000; ui3->fFluxCapData[8] = 0.000000; ui3->fFluxCapData[9] = 0.000000; ui3->fFluxCapData[10] = 0.000000; ui3->fFluxCapData[11] = 0.000000; ui3->fFluxCapData[12] = 0.000000; ui3->fFluxCapData[13] = 0.000000; ui3->fFluxCapData[14] = 0.000000; ui3->fFluxCapData[15] = 0.000000; ui3->fFluxCapData[16] = 0.000000; ui3->fFluxCapData[17] = 0.000000; ui3->fFluxCapData[18] = 0.000000; ui3->fFluxCapData[19] = 0.000000; ui3->fFluxCapData[20] = 0.000000; ui3->fFluxCapData[21] = 0.000000; ui3->fFluxCapData[22] = 0.000000; ui3->fFluxCapData[23] = 0.000000; ui3->fFluxCapData[24] = 0.000000; ui3->fFluxCapData[25] = 0.000000; ui3->fFluxCapData[26] = 0.000000; ui3->fFluxCapData[27] = 0.000000; ui3->fFluxCapData[28] = 0.000000; ui3->fFluxCapData[29] = 0.000000; ui3->fFluxCapData[30] = 0.000000; ui3->fFluxCapData[31] = 0.000000; ui3->fFluxCapData[32] = 0.000000; ui3->fFluxCapData[33] = 0.000000; ui3->fFluxCapData[34] = 0.000000; ui3->fFluxCapData[35] = 0.000000; ui3->fFluxCapData[36] = 0.000000; ui3->fFluxCapData[37] = 0.000000; ui3->fFluxCapData[38] = 0.000000; ui3->fFluxCapData[39] = 0.000000; ui3->fFluxCapData[40] = 0.000000; ui3->fFluxCapData[41] = 0.000000; ui3->fFluxCapData[42] = 0.000000; ui3->fFluxCapData[43] = 0.000000; ui3->fFluxCapData[44] = 0.000000; ui3->fFluxCapData[45] = 0.000000; ui3->fFluxCapData[46] = 0.000000; ui3->fFluxCapData[47] = 0.000000; ui3->fFluxCapData[48] = 0.000000; ui3->fFluxCapData[49] = 0.000000; ui3->fFluxCapData[50] = 0.000000; ui3->fFluxCapData[51] = 0.000000; ui3->fFluxCapData[52] = 0.000000; ui3->fFluxCapData[53] = 0.000000; ui3->fFluxCapData[54] = 0.000000; ui3->fFluxCapData[55] = 0.000000; ui3->fFluxCapData[56] = 0.000000; ui3->fFluxCapData[57] = 0.000000; ui3->fFluxCapData[58] = 0.000000; ui3->fFluxCapData[59] = 0.000000; ui3->fFluxCapData[60] = 0.000000; ui3->fFluxCapData[61] = 0.000000; ui3->fFluxCapData[62] = 0.000000; ui3->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui3);


	m_delay_filter_freq = 40.000000;
	CUICtrl* ui4 = new CUICtrl;
	ui4->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui4->uControlId = 5;
	ui4->bLogSlider = false;
	ui4->bExpSlider = true;
	ui4->fUserDisplayDataLoLimit = 40.000000;
	ui4->fUserDisplayDataHiLimit = 18000.000000;
	ui4->uUserDataType = floatData;
	ui4->fInitUserIntValue = 0;
	ui4->fInitUserFloatValue = 40.000000;
	ui4->fInitUserDoubleValue = 0;
	ui4->fInitUserUINTValue = 0;
	ui4->m_pUserCookedIntData = NULL;
	ui4->m_pUserCookedFloatData = &m_delay_filter_freq;
	ui4->m_pUserCookedDoubleData = NULL;
	ui4->m_pUserCookedUINTData = NULL;
	ui4->cControlUnits = "";
	ui4->cVariableName = "m_delay_filter_freq";
	ui4->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui4->dPresetData[0] = 40.000000;ui4->dPresetData[1] = 0.000000;ui4->dPresetData[2] = 0.000000;ui4->dPresetData[3] = 0.000000;ui4->dPresetData[4] = 0.000000;ui4->dPresetData[5] = 0.000000;ui4->dPresetData[6] = 0.000000;ui4->dPresetData[7] = 0.000000;ui4->dPresetData[8] = 0.000000;ui4->dPresetData[9] = 0.000000;ui4->dPresetData[10] = 0.000000;ui4->dPresetData[11] = 0.000000;ui4->dPresetData[12] = 0.000000;ui4->dPresetData[13] = 0.000000;ui4->dPresetData[14] = 0.000000;ui4->dPresetData[15] = 0.000000;
	ui4->cControlName = "Delay Highpass Freq";
	ui4->bOwnerControl = false;
	ui4->bMIDIControl = false;
	ui4->uMIDIControlCommand = 176;
	ui4->uMIDIControlName = 3;
	ui4->uMIDIControlChannel = 0;
	ui4->nGUIRow = nIndexer++;
	ui4->nGUIColumn = -1;
	ui4->bEnableParamSmoothing = false;
	ui4->fSmoothingTimeInMs = 100.00;
	ui4->uControlTheme[0] = 0; ui4->uControlTheme[1] = 0; ui4->uControlTheme[2] = 0; ui4->uControlTheme[3] = 0; ui4->uControlTheme[4] = 0; ui4->uControlTheme[5] = 0; ui4->uControlTheme[6] = 0; ui4->uControlTheme[7] = 0; ui4->uControlTheme[8] = 0; ui4->uControlTheme[9] = 0; ui4->uControlTheme[10] = 0; ui4->uControlTheme[11] = 0; ui4->uControlTheme[12] = 0; ui4->uControlTheme[13] = 0; ui4->uControlTheme[14] = 0; ui4->uControlTheme[15] = 0; ui4->uControlTheme[16] = 2; ui4->uControlTheme[17] = 0; ui4->uControlTheme[18] = 0; ui4->uControlTheme[19] = 0; ui4->uControlTheme[20] = 0; ui4->uControlTheme[21] = 0; ui4->uControlTheme[22] = 0; ui4->uControlTheme[23] = 0; ui4->uControlTheme[24] = 0; ui4->uControlTheme[25] = 0; ui4->uControlTheme[26] = 0; ui4->uControlTheme[27] = 0; ui4->uControlTheme[28] = 0; ui4->uControlTheme[29] = 0; ui4->uControlTheme[30] = 0; ui4->uControlTheme[31] = 0; 
	ui4->uFluxCapControl[0] = 0; ui4->uFluxCapControl[1] = 0; ui4->uFluxCapControl[2] = 0; ui4->uFluxCapControl[3] = 0; ui4->uFluxCapControl[4] = 0; ui4->uFluxCapControl[5] = 0; ui4->uFluxCapControl[6] = 0; ui4->uFluxCapControl[7] = 0; ui4->uFluxCapControl[8] = 0; ui4->uFluxCapControl[9] = 0; ui4->uFluxCapControl[10] = 0; ui4->uFluxCapControl[11] = 0; ui4->uFluxCapControl[12] = 0; ui4->uFluxCapControl[13] = 0; ui4->uFluxCapControl[14] = 0; ui4->uFluxCapControl[15] = 0; ui4->uFluxCapControl[16] = 0; ui4->uFluxCapControl[17] = 0; ui4->uFluxCapControl[18] = 0; ui4->uFluxCapControl[19] = 0; ui4->uFluxCapControl[20] = 0; ui4->uFluxCapControl[21] = 0; ui4->uFluxCapControl[22] = 0; ui4->uFluxCapControl[23] = 0; ui4->uFluxCapControl[24] = 0; ui4->uFluxCapControl[25] = 0; ui4->uFluxCapControl[26] = 0; ui4->uFluxCapControl[27] = 0; ui4->uFluxCapControl[28] = 0; ui4->uFluxCapControl[29] = 0; ui4->uFluxCapControl[30] = 0; ui4->uFluxCapControl[31] = 0; ui4->uFluxCapControl[32] = 0; ui4->uFluxCapControl[33] = 0; ui4->uFluxCapControl[34] = 0; ui4->uFluxCapControl[35] = 0; ui4->uFluxCapControl[36] = 0; ui4->uFluxCapControl[37] = 0; ui4->uFluxCapControl[38] = 0; ui4->uFluxCapControl[39] = 0; ui4->uFluxCapControl[40] = 0; ui4->uFluxCapControl[41] = 0; ui4->uFluxCapControl[42] = 0; ui4->uFluxCapControl[43] = 0; ui4->uFluxCapControl[44] = 0; ui4->uFluxCapControl[45] = 0; ui4->uFluxCapControl[46] = 0; ui4->uFluxCapControl[47] = 0; ui4->uFluxCapControl[48] = 0; ui4->uFluxCapControl[49] = 0; ui4->uFluxCapControl[50] = 0; ui4->uFluxCapControl[51] = 0; ui4->uFluxCapControl[52] = 0; ui4->uFluxCapControl[53] = 0; ui4->uFluxCapControl[54] = 0; ui4->uFluxCapControl[55] = 0; ui4->uFluxCapControl[56] = 0; ui4->uFluxCapControl[57] = 0; ui4->uFluxCapControl[58] = 0; ui4->uFluxCapControl[59] = 0; ui4->uFluxCapControl[60] = 0; ui4->uFluxCapControl[61] = 0; ui4->uFluxCapControl[62] = 0; ui4->uFluxCapControl[63] = 0; 
	ui4->fFluxCapData[0] = 0.000000; ui4->fFluxCapData[1] = 0.000000; ui4->fFluxCapData[2] = 0.000000; ui4->fFluxCapData[3] = 0.000000; ui4->fFluxCapData[4] = 0.000000; ui4->fFluxCapData[5] = 0.000000; ui4->fFluxCapData[6] = 0.000000; ui4->fFluxCapData[7] = 0.000000; ui4->fFluxCapData[8] = 0.000000; ui4->fFluxCapData[9] = 0.000000; ui4->fFluxCapData[10] = 0.000000; ui4->fFluxCapData[11] = 0.000000; ui4->fFluxCapData[12] = 0.000000; ui4->fFluxCapData[13] = 0.000000; ui4->fFluxCapData[14] = 0.000000; ui4->fFluxCapData[15] = 0.000000; ui4->fFluxCapData[16] = 0.000000; ui4->fFluxCapData[17] = 0.000000; ui4->fFluxCapData[18] = 0.000000; ui4->fFluxCapData[19] = 0.000000; ui4->fFluxCapData[20] = 0.000000; ui4->fFluxCapData[21] = 0.000000; ui4->fFluxCapData[22] = 0.000000; ui4->fFluxCapData[23] = 0.000000; ui4->fFluxCapData[24] = 0.000000; ui4->fFluxCapData[25] = 0.000000; ui4->fFluxCapData[26] = 0.000000; ui4->fFluxCapData[27] = 0.000000; ui4->fFluxCapData[28] = 0.000000; ui4->fFluxCapData[29] = 0.000000; ui4->fFluxCapData[30] = 0.000000; ui4->fFluxCapData[31] = 0.000000; ui4->fFluxCapData[32] = 0.000000; ui4->fFluxCapData[33] = 0.000000; ui4->fFluxCapData[34] = 0.000000; ui4->fFluxCapData[35] = 0.000000; ui4->fFluxCapData[36] = 0.000000; ui4->fFluxCapData[37] = 0.000000; ui4->fFluxCapData[38] = 0.000000; ui4->fFluxCapData[39] = 0.000000; ui4->fFluxCapData[40] = 0.000000; ui4->fFluxCapData[41] = 0.000000; ui4->fFluxCapData[42] = 0.000000; ui4->fFluxCapData[43] = 0.000000; ui4->fFluxCapData[44] = 0.000000; ui4->fFluxCapData[45] = 0.000000; ui4->fFluxCapData[46] = 0.000000; ui4->fFluxCapData[47] = 0.000000; ui4->fFluxCapData[48] = 0.000000; ui4->fFluxCapData[49] = 0.000000; ui4->fFluxCapData[50] = 0.000000; ui4->fFluxCapData[51] = 0.000000; ui4->fFluxCapData[52] = 0.000000; ui4->fFluxCapData[53] = 0.000000; ui4->fFluxCapData[54] = 0.000000; ui4->fFluxCapData[55] = 0.000000; ui4->fFluxCapData[56] = 0.000000; ui4->fFluxCapData[57] = 0.000000; ui4->fFluxCapData[58] = 0.000000; ui4->fFluxCapData[59] = 0.000000; ui4->fFluxCapData[60] = 0.000000; ui4->fFluxCapData[61] = 0.000000; ui4->fFluxCapData[62] = 0.000000; ui4->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui4);


	m_delay_dry = 1.000000;
	CUICtrl* ui5 = new CUICtrl;
	ui5->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui5->uControlId = 6;
	ui5->bLogSlider = false;
	ui5->bExpSlider = false;
	ui5->fUserDisplayDataLoLimit = 0.000000;
	ui5->fUserDisplayDataHiLimit = 1.000000;
	ui5->uUserDataType = floatData;
	ui5->fInitUserIntValue = 0;
	ui5->fInitUserFloatValue = 1.000000;
	ui5->fInitUserDoubleValue = 0;
	ui5->fInitUserUINTValue = 0;
	ui5->m_pUserCookedIntData = NULL;
	ui5->m_pUserCookedFloatData = &m_delay_dry;
	ui5->m_pUserCookedDoubleData = NULL;
	ui5->m_pUserCookedUINTData = NULL;
	ui5->cControlUnits = "";
	ui5->cVariableName = "m_delay_dry";
	ui5->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui5->dPresetData[0] = 1.000000;ui5->dPresetData[1] = 0.000000;ui5->dPresetData[2] = 0.000000;ui5->dPresetData[3] = 0.000000;ui5->dPresetData[4] = 0.000000;ui5->dPresetData[5] = 0.000000;ui5->dPresetData[6] = 0.000000;ui5->dPresetData[7] = 0.000000;ui5->dPresetData[8] = 0.000000;ui5->dPresetData[9] = 0.000000;ui5->dPresetData[10] = 0.000000;ui5->dPresetData[11] = 0.000000;ui5->dPresetData[12] = 0.000000;ui5->dPresetData[13] = 0.000000;ui5->dPresetData[14] = 0.000000;ui5->dPresetData[15] = 0.000000;
	ui5->cControlName = "Delay Dry";
	ui5->bOwnerControl = false;
	ui5->bMIDIControl = false;
	ui5->uMIDIControlCommand = 176;
	ui5->uMIDIControlName = 3;
	ui5->uMIDIControlChannel = 0;
	ui5->nGUIRow = nIndexer++;
	ui5->nGUIColumn = -1;
	ui5->bEnableParamSmoothing = false;
	ui5->fSmoothingTimeInMs = 100.00;
	ui5->uControlTheme[0] = 0; ui5->uControlTheme[1] = 0; ui5->uControlTheme[2] = 0; ui5->uControlTheme[3] = 0; ui5->uControlTheme[4] = 0; ui5->uControlTheme[5] = 0; ui5->uControlTheme[6] = 0; ui5->uControlTheme[7] = 0; ui5->uControlTheme[8] = 0; ui5->uControlTheme[9] = 0; ui5->uControlTheme[10] = 0; ui5->uControlTheme[11] = 0; ui5->uControlTheme[12] = 0; ui5->uControlTheme[13] = 0; ui5->uControlTheme[14] = 0; ui5->uControlTheme[15] = 0; ui5->uControlTheme[16] = 2; ui5->uControlTheme[17] = 0; ui5->uControlTheme[18] = 0; ui5->uControlTheme[19] = 0; ui5->uControlTheme[20] = 0; ui5->uControlTheme[21] = 0; ui5->uControlTheme[22] = 0; ui5->uControlTheme[23] = 0; ui5->uControlTheme[24] = 0; ui5->uControlTheme[25] = 0; ui5->uControlTheme[26] = 0; ui5->uControlTheme[27] = 0; ui5->uControlTheme[28] = 0; ui5->uControlTheme[29] = 0; ui5->uControlTheme[30] = 0; ui5->uControlTheme[31] = 0; 
	ui5->uFluxCapControl[0] = 0; ui5->uFluxCapControl[1] = 0; ui5->uFluxCapControl[2] = 0; ui5->uFluxCapControl[3] = 0; ui5->uFluxCapControl[4] = 0; ui5->uFluxCapControl[5] = 0; ui5->uFluxCapControl[6] = 0; ui5->uFluxCapControl[7] = 0; ui5->uFluxCapControl[8] = 0; ui5->uFluxCapControl[9] = 0; ui5->uFluxCapControl[10] = 0; ui5->uFluxCapControl[11] = 0; ui5->uFluxCapControl[12] = 0; ui5->uFluxCapControl[13] = 0; ui5->uFluxCapControl[14] = 0; ui5->uFluxCapControl[15] = 0; ui5->uFluxCapControl[16] = 0; ui5->uFluxCapControl[17] = 0; ui5->uFluxCapControl[18] = 0; ui5->uFluxCapControl[19] = 0; ui5->uFluxCapControl[20] = 0; ui5->uFluxCapControl[21] = 0; ui5->uFluxCapControl[22] = 0; ui5->uFluxCapControl[23] = 0; ui5->uFluxCapControl[24] = 0; ui5->uFluxCapControl[25] = 0; ui5->uFluxCapControl[26] = 0; ui5->uFluxCapControl[27] = 0; ui5->uFluxCapControl[28] = 0; ui5->uFluxCapControl[29] = 0; ui5->uFluxCapControl[30] = 0; ui5->uFluxCapControl[31] = 0; ui5->uFluxCapControl[32] = 0; ui5->uFluxCapControl[33] = 0; ui5->uFluxCapControl[34] = 0; ui5->uFluxCapControl[35] = 0; ui5->uFluxCapControl[36] = 0; ui5->uFluxCapControl[37] = 0; ui5->uFluxCapControl[38] = 0; ui5->uFluxCapControl[39] = 0; ui5->uFluxCapControl[40] = 0; ui5->uFluxCapControl[41] = 0; ui5->uFluxCapControl[42] = 0; ui5->uFluxCapControl[43] = 0; ui5->uFluxCapControl[44] = 0; ui5->uFluxCapControl[45] = 0; ui5->uFluxCapControl[46] = 0; ui5->uFluxCapControl[47] = 0; ui5->uFluxCapControl[48] = 0; ui5->uFluxCapControl[49] = 0; ui5->uFluxCapControl[50] = 0; ui5->uFluxCapControl[51] = 0; ui5->uFluxCapControl[52] = 0; ui5->uFluxCapControl[53] = 0; ui5->uFluxCapControl[54] = 0; ui5->uFluxCapControl[55] = 0; ui5->uFluxCapControl[56] = 0; ui5->uFluxCapControl[57] = 0; ui5->uFluxCapControl[58] = 0; ui5->uFluxCapControl[59] = 0; ui5->uFluxCapControl[60] = 0; ui5->uFluxCapControl[61] = 0; ui5->uFluxCapControl[62] = 0; ui5->uFluxCapControl[63] = 0; 
	ui5->fFluxCapData[0] = 0.000000; ui5->fFluxCapData[1] = 0.000000; ui5->fFluxCapData[2] = 0.000000; ui5->fFluxCapData[3] = 0.000000; ui5->fFluxCapData[4] = 0.000000; ui5->fFluxCapData[5] = 0.000000; ui5->fFluxCapData[6] = 0.000000; ui5->fFluxCapData[7] = 0.000000; ui5->fFluxCapData[8] = 0.000000; ui5->fFluxCapData[9] = 0.000000; ui5->fFluxCapData[10] = 0.000000; ui5->fFluxCapData[11] = 0.000000; ui5->fFluxCapData[12] = 0.000000; ui5->fFluxCapData[13] = 0.000000; ui5->fFluxCapData[14] = 0.000000; ui5->fFluxCapData[15] = 0.000000; ui5->fFluxCapData[16] = 0.000000; ui5->fFluxCapData[17] = 0.000000; ui5->fFluxCapData[18] = 0.000000; ui5->fFluxCapData[19] = 0.000000; ui5->fFluxCapData[20] = 0.000000; ui5->fFluxCapData[21] = 0.000000; ui5->fFluxCapData[22] = 0.000000; ui5->fFluxCapData[23] = 0.000000; ui5->fFluxCapData[24] = 0.000000; ui5->fFluxCapData[25] = 0.000000; ui5->fFluxCapData[26] = 0.000000; ui5->fFluxCapData[27] = 0.000000; ui5->fFluxCapData[28] = 0.000000; ui5->fFluxCapData[29] = 0.000000; ui5->fFluxCapData[30] = 0.000000; ui5->fFluxCapData[31] = 0.000000; ui5->fFluxCapData[32] = 0.000000; ui5->fFluxCapData[33] = 0.000000; ui5->fFluxCapData[34] = 0.000000; ui5->fFluxCapData[35] = 0.000000; ui5->fFluxCapData[36] = 0.000000; ui5->fFluxCapData[37] = 0.000000; ui5->fFluxCapData[38] = 0.000000; ui5->fFluxCapData[39] = 0.000000; ui5->fFluxCapData[40] = 0.000000; ui5->fFluxCapData[41] = 0.000000; ui5->fFluxCapData[42] = 0.000000; ui5->fFluxCapData[43] = 0.000000; ui5->fFluxCapData[44] = 0.000000; ui5->fFluxCapData[45] = 0.000000; ui5->fFluxCapData[46] = 0.000000; ui5->fFluxCapData[47] = 0.000000; ui5->fFluxCapData[48] = 0.000000; ui5->fFluxCapData[49] = 0.000000; ui5->fFluxCapData[50] = 0.000000; ui5->fFluxCapData[51] = 0.000000; ui5->fFluxCapData[52] = 0.000000; ui5->fFluxCapData[53] = 0.000000; ui5->fFluxCapData[54] = 0.000000; ui5->fFluxCapData[55] = 0.000000; ui5->fFluxCapData[56] = 0.000000; ui5->fFluxCapData[57] = 0.000000; ui5->fFluxCapData[58] = 0.000000; ui5->fFluxCapData[59] = 0.000000; ui5->fFluxCapData[60] = 0.000000; ui5->fFluxCapData[61] = 0.000000; ui5->fFluxCapData[62] = 0.000000; ui5->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui5);


	m_delay_wet = 0.400000;
	CUICtrl* ui6 = new CUICtrl;
	ui6->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui6->uControlId = 7;
	ui6->bLogSlider = false;
	ui6->bExpSlider = false;
	ui6->fUserDisplayDataLoLimit = 0.000000;
	ui6->fUserDisplayDataHiLimit = 1.000000;
	ui6->uUserDataType = floatData;
	ui6->fInitUserIntValue = 0;
	ui6->fInitUserFloatValue = 0.400000;
	ui6->fInitUserDoubleValue = 0;
	ui6->fInitUserUINTValue = 0;
	ui6->m_pUserCookedIntData = NULL;
	ui6->m_pUserCookedFloatData = &m_delay_wet;
	ui6->m_pUserCookedDoubleData = NULL;
	ui6->m_pUserCookedUINTData = NULL;
	ui6->cControlUnits = "";
	ui6->cVariableName = "m_delay_wet";
	ui6->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui6->dPresetData[0] = 0.400000;ui6->dPresetData[1] = 0.000000;ui6->dPresetData[2] = 0.000000;ui6->dPresetData[3] = 0.000000;ui6->dPresetData[4] = 0.000000;ui6->dPresetData[5] = 0.000000;ui6->dPresetData[6] = 0.000000;ui6->dPresetData[7] = 0.000000;ui6->dPresetData[8] = 0.000000;ui6->dPresetData[9] = 0.000000;ui6->dPresetData[10] = 0.000000;ui6->dPresetData[11] = 0.000000;ui6->dPresetData[12] = 0.000000;ui6->dPresetData[13] = 0.000000;ui6->dPresetData[14] = 0.000000;ui6->dPresetData[15] = 0.000000;
	ui6->cControlName = "Delay Wet";
	ui6->bOwnerControl = false;
	ui6->bMIDIControl = false;
	ui6->uMIDIControlCommand = 176;
	ui6->uMIDIControlName = 3;
	ui6->uMIDIControlChannel = 0;
	ui6->nGUIRow = nIndexer++;
	ui6->nGUIColumn = -1;
	ui6->bEnableParamSmoothing = false;
	ui6->fSmoothingTimeInMs = 100.00;
	ui6->uControlTheme[0] = 0; ui6->uControlTheme[1] = 0; ui6->uControlTheme[2] = 0; ui6->uControlTheme[3] = 0; ui6->uControlTheme[4] = 0; ui6->uControlTheme[5] = 0; ui6->uControlTheme[6] = 0; ui6->uControlTheme[7] = 0; ui6->uControlTheme[8] = 0; ui6->uControlTheme[9] = 0; ui6->uControlTheme[10] = 0; ui6->uControlTheme[11] = 0; ui6->uControlTheme[12] = 0; ui6->uControlTheme[13] = 0; ui6->uControlTheme[14] = 0; ui6->uControlTheme[15] = 0; ui6->uControlTheme[16] = 2; ui6->uControlTheme[17] = 0; ui6->uControlTheme[18] = 0; ui6->uControlTheme[19] = 0; ui6->uControlTheme[20] = 0; ui6->uControlTheme[21] = 0; ui6->uControlTheme[22] = 0; ui6->uControlTheme[23] = 0; ui6->uControlTheme[24] = 0; ui6->uControlTheme[25] = 0; ui6->uControlTheme[26] = 0; ui6->uControlTheme[27] = 0; ui6->uControlTheme[28] = 0; ui6->uControlTheme[29] = 0; ui6->uControlTheme[30] = 0; ui6->uControlTheme[31] = 0; 
	ui6->uFluxCapControl[0] = 0; ui6->uFluxCapControl[1] = 0; ui6->uFluxCapControl[2] = 0; ui6->uFluxCapControl[3] = 0; ui6->uFluxCapControl[4] = 0; ui6->uFluxCapControl[5] = 0; ui6->uFluxCapControl[6] = 0; ui6->uFluxCapControl[7] = 0; ui6->uFluxCapControl[8] = 0; ui6->uFluxCapControl[9] = 0; ui6->uFluxCapControl[10] = 0; ui6->uFluxCapControl[11] = 0; ui6->uFluxCapControl[12] = 0; ui6->uFluxCapControl[13] = 0; ui6->uFluxCapControl[14] = 0; ui6->uFluxCapControl[15] = 0; ui6->uFluxCapControl[16] = 0; ui6->uFluxCapControl[17] = 0; ui6->uFluxCapControl[18] = 0; ui6->uFluxCapControl[19] = 0; ui6->uFluxCapControl[20] = 0; ui6->uFluxCapControl[21] = 0; ui6->uFluxCapControl[22] = 0; ui6->uFluxCapControl[23] = 0; ui6->uFluxCapControl[24] = 0; ui6->uFluxCapControl[25] = 0; ui6->uFluxCapControl[26] = 0; ui6->uFluxCapControl[27] = 0; ui6->uFluxCapControl[28] = 0; ui6->uFluxCapControl[29] = 0; ui6->uFluxCapControl[30] = 0; ui6->uFluxCapControl[31] = 0; ui6->uFluxCapControl[32] = 0; ui6->uFluxCapControl[33] = 0; ui6->uFluxCapControl[34] = 0; ui6->uFluxCapControl[35] = 0; ui6->uFluxCapControl[36] = 0; ui6->uFluxCapControl[37] = 0; ui6->uFluxCapControl[38] = 0; ui6->uFluxCapControl[39] = 0; ui6->uFluxCapControl[40] = 0; ui6->uFluxCapControl[41] = 0; ui6->uFluxCapControl[42] = 0; ui6->uFluxCapControl[43] = 0; ui6->uFluxCapControl[44] = 0; ui6->uFluxCapControl[45] = 0; ui6->uFluxCapControl[46] = 0; ui6->uFluxCapControl[47] = 0; ui6->uFluxCapControl[48] = 0; ui6->uFluxCapControl[49] = 0; ui6->uFluxCapControl[50] = 0; ui6->uFluxCapControl[51] = 0; ui6->uFluxCapControl[52] = 0; ui6->uFluxCapControl[53] = 0; ui6->uFluxCapControl[54] = 0; ui6->uFluxCapControl[55] = 0; ui6->uFluxCapControl[56] = 0; ui6->uFluxCapControl[57] = 0; ui6->uFluxCapControl[58] = 0; ui6->uFluxCapControl[59] = 0; ui6->uFluxCapControl[60] = 0; ui6->uFluxCapControl[61] = 0; ui6->uFluxCapControl[62] = 0; ui6->uFluxCapControl[63] = 0; 
	ui6->fFluxCapData[0] = 0.000000; ui6->fFluxCapData[1] = 0.000000; ui6->fFluxCapData[2] = 0.000000; ui6->fFluxCapData[3] = 0.000000; ui6->fFluxCapData[4] = 0.000000; ui6->fFluxCapData[5] = 0.000000; ui6->fFluxCapData[6] = 0.000000; ui6->fFluxCapData[7] = 0.000000; ui6->fFluxCapData[8] = 0.000000; ui6->fFluxCapData[9] = 0.000000; ui6->fFluxCapData[10] = 0.000000; ui6->fFluxCapData[11] = 0.000000; ui6->fFluxCapData[12] = 0.000000; ui6->fFluxCapData[13] = 0.000000; ui6->fFluxCapData[14] = 0.000000; ui6->fFluxCapData[15] = 0.000000; ui6->fFluxCapData[16] = 0.000000; ui6->fFluxCapData[17] = 0.000000; ui6->fFluxCapData[18] = 0.000000; ui6->fFluxCapData[19] = 0.000000; ui6->fFluxCapData[20] = 0.000000; ui6->fFluxCapData[21] = 0.000000; ui6->fFluxCapData[22] = 0.000000; ui6->fFluxCapData[23] = 0.000000; ui6->fFluxCapData[24] = 0.000000; ui6->fFluxCapData[25] = 0.000000; ui6->fFluxCapData[26] = 0.000000; ui6->fFluxCapData[27] = 0.000000; ui6->fFluxCapData[28] = 0.000000; ui6->fFluxCapData[29] = 0.000000; ui6->fFluxCapData[30] = 0.000000; ui6->fFluxCapData[31] = 0.000000; ui6->fFluxCapData[32] = 0.000000; ui6->fFluxCapData[33] = 0.000000; ui6->fFluxCapData[34] = 0.000000; ui6->fFluxCapData[35] = 0.000000; ui6->fFluxCapData[36] = 0.000000; ui6->fFluxCapData[37] = 0.000000; ui6->fFluxCapData[38] = 0.000000; ui6->fFluxCapData[39] = 0.000000; ui6->fFluxCapData[40] = 0.000000; ui6->fFluxCapData[41] = 0.000000; ui6->fFluxCapData[42] = 0.000000; ui6->fFluxCapData[43] = 0.000000; ui6->fFluxCapData[44] = 0.000000; ui6->fFluxCapData[45] = 0.000000; ui6->fFluxCapData[46] = 0.000000; ui6->fFluxCapData[47] = 0.000000; ui6->fFluxCapData[48] = 0.000000; ui6->fFluxCapData[49] = 0.000000; ui6->fFluxCapData[50] = 0.000000; ui6->fFluxCapData[51] = 0.000000; ui6->fFluxCapData[52] = 0.000000; ui6->fFluxCapData[53] = 0.000000; ui6->fFluxCapData[54] = 0.000000; ui6->fFluxCapData[55] = 0.000000; ui6->fFluxCapData[56] = 0.000000; ui6->fFluxCapData[57] = 0.000000; ui6->fFluxCapData[58] = 0.000000; ui6->fFluxCapData[59] = 0.000000; ui6->fFluxCapData[60] = 0.000000; ui6->fFluxCapData[61] = 0.000000; ui6->fFluxCapData[62] = 0.000000; ui6->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui6);


	m_delay_feedback = 0.000000;
	CUICtrl* ui7 = new CUICtrl;
	ui7->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui7->uControlId = 8;
	ui7->bLogSlider = false;
	ui7->bExpSlider = false;
	ui7->fUserDisplayDataLoLimit = 0.000000;
	ui7->fUserDisplayDataHiLimit = 1.000000;
	ui7->uUserDataType = floatData;
	ui7->fInitUserIntValue = 0;
	ui7->fInitUserFloatValue = 0.000000;
	ui7->fInitUserDoubleValue = 0;
	ui7->fInitUserUINTValue = 0;
	ui7->m_pUserCookedIntData = NULL;
	ui7->m_pUserCookedFloatData = &m_delay_feedback;
	ui7->m_pUserCookedDoubleData = NULL;
	ui7->m_pUserCookedUINTData = NULL;
	ui7->cControlUnits = "";
	ui7->cVariableName = "m_delay_feedback";
	ui7->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui7->dPresetData[0] = 0.000000;ui7->dPresetData[1] = 0.000000;ui7->dPresetData[2] = 0.000000;ui7->dPresetData[3] = 0.000000;ui7->dPresetData[4] = 0.000000;ui7->dPresetData[5] = 0.000000;ui7->dPresetData[6] = 0.000000;ui7->dPresetData[7] = 0.000000;ui7->dPresetData[8] = 0.000000;ui7->dPresetData[9] = 0.000000;ui7->dPresetData[10] = 0.000000;ui7->dPresetData[11] = 0.000000;ui7->dPresetData[12] = 0.000000;ui7->dPresetData[13] = 0.000000;ui7->dPresetData[14] = 0.000000;ui7->dPresetData[15] = 0.000000;
	ui7->cControlName = "Delay Feedback";
	ui7->bOwnerControl = false;
	ui7->bMIDIControl = false;
	ui7->uMIDIControlCommand = 176;
	ui7->uMIDIControlName = 3;
	ui7->uMIDIControlChannel = 0;
	ui7->nGUIRow = nIndexer++;
	ui7->nGUIColumn = -1;
	ui7->bEnableParamSmoothing = false;
	ui7->fSmoothingTimeInMs = 100.00;
	ui7->uControlTheme[0] = 0; ui7->uControlTheme[1] = 0; ui7->uControlTheme[2] = 0; ui7->uControlTheme[3] = 0; ui7->uControlTheme[4] = 0; ui7->uControlTheme[5] = 0; ui7->uControlTheme[6] = 0; ui7->uControlTheme[7] = 0; ui7->uControlTheme[8] = 0; ui7->uControlTheme[9] = 0; ui7->uControlTheme[10] = 0; ui7->uControlTheme[11] = 0; ui7->uControlTheme[12] = 0; ui7->uControlTheme[13] = 0; ui7->uControlTheme[14] = 0; ui7->uControlTheme[15] = 0; ui7->uControlTheme[16] = 2; ui7->uControlTheme[17] = 0; ui7->uControlTheme[18] = 0; ui7->uControlTheme[19] = 0; ui7->uControlTheme[20] = 0; ui7->uControlTheme[21] = 0; ui7->uControlTheme[22] = 0; ui7->uControlTheme[23] = 0; ui7->uControlTheme[24] = 0; ui7->uControlTheme[25] = 0; ui7->uControlTheme[26] = 0; ui7->uControlTheme[27] = 0; ui7->uControlTheme[28] = 0; ui7->uControlTheme[29] = 0; ui7->uControlTheme[30] = 0; ui7->uControlTheme[31] = 0; 
	ui7->uFluxCapControl[0] = 0; ui7->uFluxCapControl[1] = 0; ui7->uFluxCapControl[2] = 0; ui7->uFluxCapControl[3] = 0; ui7->uFluxCapControl[4] = 0; ui7->uFluxCapControl[5] = 0; ui7->uFluxCapControl[6] = 0; ui7->uFluxCapControl[7] = 0; ui7->uFluxCapControl[8] = 0; ui7->uFluxCapControl[9] = 0; ui7->uFluxCapControl[10] = 0; ui7->uFluxCapControl[11] = 0; ui7->uFluxCapControl[12] = 0; ui7->uFluxCapControl[13] = 0; ui7->uFluxCapControl[14] = 0; ui7->uFluxCapControl[15] = 0; ui7->uFluxCapControl[16] = 0; ui7->uFluxCapControl[17] = 0; ui7->uFluxCapControl[18] = 0; ui7->uFluxCapControl[19] = 0; ui7->uFluxCapControl[20] = 0; ui7->uFluxCapControl[21] = 0; ui7->uFluxCapControl[22] = 0; ui7->uFluxCapControl[23] = 0; ui7->uFluxCapControl[24] = 0; ui7->uFluxCapControl[25] = 0; ui7->uFluxCapControl[26] = 0; ui7->uFluxCapControl[27] = 0; ui7->uFluxCapControl[28] = 0; ui7->uFluxCapControl[29] = 0; ui7->uFluxCapControl[30] = 0; ui7->uFluxCapControl[31] = 0; ui7->uFluxCapControl[32] = 0; ui7->uFluxCapControl[33] = 0; ui7->uFluxCapControl[34] = 0; ui7->uFluxCapControl[35] = 0; ui7->uFluxCapControl[36] = 0; ui7->uFluxCapControl[37] = 0; ui7->uFluxCapControl[38] = 0; ui7->uFluxCapControl[39] = 0; ui7->uFluxCapControl[40] = 0; ui7->uFluxCapControl[41] = 0; ui7->uFluxCapControl[42] = 0; ui7->uFluxCapControl[43] = 0; ui7->uFluxCapControl[44] = 0; ui7->uFluxCapControl[45] = 0; ui7->uFluxCapControl[46] = 0; ui7->uFluxCapControl[47] = 0; ui7->uFluxCapControl[48] = 0; ui7->uFluxCapControl[49] = 0; ui7->uFluxCapControl[50] = 0; ui7->uFluxCapControl[51] = 0; ui7->uFluxCapControl[52] = 0; ui7->uFluxCapControl[53] = 0; ui7->uFluxCapControl[54] = 0; ui7->uFluxCapControl[55] = 0; ui7->uFluxCapControl[56] = 0; ui7->uFluxCapControl[57] = 0; ui7->uFluxCapControl[58] = 0; ui7->uFluxCapControl[59] = 0; ui7->uFluxCapControl[60] = 0; ui7->uFluxCapControl[61] = 0; ui7->uFluxCapControl[62] = 0; ui7->uFluxCapControl[63] = 0; 
	ui7->fFluxCapData[0] = 0.000000; ui7->fFluxCapData[1] = 0.000000; ui7->fFluxCapData[2] = 0.000000; ui7->fFluxCapData[3] = 0.000000; ui7->fFluxCapData[4] = 0.000000; ui7->fFluxCapData[5] = 0.000000; ui7->fFluxCapData[6] = 0.000000; ui7->fFluxCapData[7] = 0.000000; ui7->fFluxCapData[8] = 0.000000; ui7->fFluxCapData[9] = 0.000000; ui7->fFluxCapData[10] = 0.000000; ui7->fFluxCapData[11] = 0.000000; ui7->fFluxCapData[12] = 0.000000; ui7->fFluxCapData[13] = 0.000000; ui7->fFluxCapData[14] = 0.000000; ui7->fFluxCapData[15] = 0.000000; ui7->fFluxCapData[16] = 0.000000; ui7->fFluxCapData[17] = 0.000000; ui7->fFluxCapData[18] = 0.000000; ui7->fFluxCapData[19] = 0.000000; ui7->fFluxCapData[20] = 0.000000; ui7->fFluxCapData[21] = 0.000000; ui7->fFluxCapData[22] = 0.000000; ui7->fFluxCapData[23] = 0.000000; ui7->fFluxCapData[24] = 0.000000; ui7->fFluxCapData[25] = 0.000000; ui7->fFluxCapData[26] = 0.000000; ui7->fFluxCapData[27] = 0.000000; ui7->fFluxCapData[28] = 0.000000; ui7->fFluxCapData[29] = 0.000000; ui7->fFluxCapData[30] = 0.000000; ui7->fFluxCapData[31] = 0.000000; ui7->fFluxCapData[32] = 0.000000; ui7->fFluxCapData[33] = 0.000000; ui7->fFluxCapData[34] = 0.000000; ui7->fFluxCapData[35] = 0.000000; ui7->fFluxCapData[36] = 0.000000; ui7->fFluxCapData[37] = 0.000000; ui7->fFluxCapData[38] = 0.000000; ui7->fFluxCapData[39] = 0.000000; ui7->fFluxCapData[40] = 0.000000; ui7->fFluxCapData[41] = 0.000000; ui7->fFluxCapData[42] = 0.000000; ui7->fFluxCapData[43] = 0.000000; ui7->fFluxCapData[44] = 0.000000; ui7->fFluxCapData[45] = 0.000000; ui7->fFluxCapData[46] = 0.000000; ui7->fFluxCapData[47] = 0.000000; ui7->fFluxCapData[48] = 0.000000; ui7->fFluxCapData[49] = 0.000000; ui7->fFluxCapData[50] = 0.000000; ui7->fFluxCapData[51] = 0.000000; ui7->fFluxCapData[52] = 0.000000; ui7->fFluxCapData[53] = 0.000000; ui7->fFluxCapData[54] = 0.000000; ui7->fFluxCapData[55] = 0.000000; ui7->fFluxCapData[56] = 0.000000; ui7->fFluxCapData[57] = 0.000000; ui7->fFluxCapData[58] = 0.000000; ui7->fFluxCapData[59] = 0.000000; ui7->fFluxCapData[60] = 0.000000; ui7->fFluxCapData[61] = 0.000000; ui7->fFluxCapData[62] = 0.000000; ui7->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui7);


	m_delay_time = 1.000000;
	CUICtrl* ui8 = new CUICtrl;
	ui8->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui8->uControlId = 9;
	ui8->bLogSlider = false;
	ui8->bExpSlider = true;
	ui8->fUserDisplayDataLoLimit = 0.010000;
	ui8->fUserDisplayDataHiLimit = 5.000000;
	ui8->uUserDataType = floatData;
	ui8->fInitUserIntValue = 0;
	ui8->fInitUserFloatValue = 1.000000;
	ui8->fInitUserDoubleValue = 0;
	ui8->fInitUserUINTValue = 0;
	ui8->m_pUserCookedIntData = NULL;
	ui8->m_pUserCookedFloatData = &m_delay_time;
	ui8->m_pUserCookedDoubleData = NULL;
	ui8->m_pUserCookedUINTData = NULL;
	ui8->cControlUnits = "s";
	ui8->cVariableName = "m_delay_time";
	ui8->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui8->dPresetData[0] = 1.000000;ui8->dPresetData[1] = 0.000000;ui8->dPresetData[2] = 0.000000;ui8->dPresetData[3] = 0.000000;ui8->dPresetData[4] = 0.000000;ui8->dPresetData[5] = 0.000000;ui8->dPresetData[6] = 0.000000;ui8->dPresetData[7] = 0.000000;ui8->dPresetData[8] = 0.000000;ui8->dPresetData[9] = 0.000000;ui8->dPresetData[10] = 0.000000;ui8->dPresetData[11] = 0.000000;ui8->dPresetData[12] = 0.000000;ui8->dPresetData[13] = 0.000000;ui8->dPresetData[14] = 0.000000;ui8->dPresetData[15] = 0.000000;
	ui8->cControlName = "Delay Time";
	ui8->bOwnerControl = false;
	ui8->bMIDIControl = false;
	ui8->uMIDIControlCommand = 176;
	ui8->uMIDIControlName = 3;
	ui8->uMIDIControlChannel = 0;
	ui8->nGUIRow = nIndexer++;
	ui8->nGUIColumn = -1;
	ui8->bEnableParamSmoothing = false;
	ui8->fSmoothingTimeInMs = 100.00;
	ui8->uControlTheme[0] = 0; ui8->uControlTheme[1] = 0; ui8->uControlTheme[2] = 0; ui8->uControlTheme[3] = 0; ui8->uControlTheme[4] = 0; ui8->uControlTheme[5] = 0; ui8->uControlTheme[6] = 0; ui8->uControlTheme[7] = 0; ui8->uControlTheme[8] = 0; ui8->uControlTheme[9] = 0; ui8->uControlTheme[10] = 0; ui8->uControlTheme[11] = 0; ui8->uControlTheme[12] = 0; ui8->uControlTheme[13] = 0; ui8->uControlTheme[14] = 0; ui8->uControlTheme[15] = 0; ui8->uControlTheme[16] = 2; ui8->uControlTheme[17] = 0; ui8->uControlTheme[18] = 0; ui8->uControlTheme[19] = 0; ui8->uControlTheme[20] = 0; ui8->uControlTheme[21] = 0; ui8->uControlTheme[22] = 0; ui8->uControlTheme[23] = 0; ui8->uControlTheme[24] = 0; ui8->uControlTheme[25] = 0; ui8->uControlTheme[26] = 0; ui8->uControlTheme[27] = 0; ui8->uControlTheme[28] = 0; ui8->uControlTheme[29] = 0; ui8->uControlTheme[30] = 0; ui8->uControlTheme[31] = 0; 
	ui8->uFluxCapControl[0] = 0; ui8->uFluxCapControl[1] = 0; ui8->uFluxCapControl[2] = 0; ui8->uFluxCapControl[3] = 0; ui8->uFluxCapControl[4] = 0; ui8->uFluxCapControl[5] = 0; ui8->uFluxCapControl[6] = 0; ui8->uFluxCapControl[7] = 0; ui8->uFluxCapControl[8] = 0; ui8->uFluxCapControl[9] = 0; ui8->uFluxCapControl[10] = 0; ui8->uFluxCapControl[11] = 0; ui8->uFluxCapControl[12] = 0; ui8->uFluxCapControl[13] = 0; ui8->uFluxCapControl[14] = 0; ui8->uFluxCapControl[15] = 0; ui8->uFluxCapControl[16] = 0; ui8->uFluxCapControl[17] = 0; ui8->uFluxCapControl[18] = 0; ui8->uFluxCapControl[19] = 0; ui8->uFluxCapControl[20] = 0; ui8->uFluxCapControl[21] = 0; ui8->uFluxCapControl[22] = 0; ui8->uFluxCapControl[23] = 0; ui8->uFluxCapControl[24] = 0; ui8->uFluxCapControl[25] = 0; ui8->uFluxCapControl[26] = 0; ui8->uFluxCapControl[27] = 0; ui8->uFluxCapControl[28] = 0; ui8->uFluxCapControl[29] = 0; ui8->uFluxCapControl[30] = 0; ui8->uFluxCapControl[31] = 0; ui8->uFluxCapControl[32] = 0; ui8->uFluxCapControl[33] = 0; ui8->uFluxCapControl[34] = 0; ui8->uFluxCapControl[35] = 0; ui8->uFluxCapControl[36] = 0; ui8->uFluxCapControl[37] = 0; ui8->uFluxCapControl[38] = 0; ui8->uFluxCapControl[39] = 0; ui8->uFluxCapControl[40] = 0; ui8->uFluxCapControl[41] = 0; ui8->uFluxCapControl[42] = 0; ui8->uFluxCapControl[43] = 0; ui8->uFluxCapControl[44] = 0; ui8->uFluxCapControl[45] = 0; ui8->uFluxCapControl[46] = 0; ui8->uFluxCapControl[47] = 0; ui8->uFluxCapControl[48] = 0; ui8->uFluxCapControl[49] = 0; ui8->uFluxCapControl[50] = 0; ui8->uFluxCapControl[51] = 0; ui8->uFluxCapControl[52] = 0; ui8->uFluxCapControl[53] = 0; ui8->uFluxCapControl[54] = 0; ui8->uFluxCapControl[55] = 0; ui8->uFluxCapControl[56] = 0; ui8->uFluxCapControl[57] = 0; ui8->uFluxCapControl[58] = 0; ui8->uFluxCapControl[59] = 0; ui8->uFluxCapControl[60] = 0; ui8->uFluxCapControl[61] = 0; ui8->uFluxCapControl[62] = 0; ui8->uFluxCapControl[63] = 0; 
	ui8->fFluxCapData[0] = 0.000000; ui8->fFluxCapData[1] = 0.000000; ui8->fFluxCapData[2] = 0.000000; ui8->fFluxCapData[3] = 0.000000; ui8->fFluxCapData[4] = 0.000000; ui8->fFluxCapData[5] = 0.000000; ui8->fFluxCapData[6] = 0.000000; ui8->fFluxCapData[7] = 0.000000; ui8->fFluxCapData[8] = 0.000000; ui8->fFluxCapData[9] = 0.000000; ui8->fFluxCapData[10] = 0.000000; ui8->fFluxCapData[11] = 0.000000; ui8->fFluxCapData[12] = 0.000000; ui8->fFluxCapData[13] = 0.000000; ui8->fFluxCapData[14] = 0.000000; ui8->fFluxCapData[15] = 0.000000; ui8->fFluxCapData[16] = 0.000000; ui8->fFluxCapData[17] = 0.000000; ui8->fFluxCapData[18] = 0.000000; ui8->fFluxCapData[19] = 0.000000; ui8->fFluxCapData[20] = 0.000000; ui8->fFluxCapData[21] = 0.000000; ui8->fFluxCapData[22] = 0.000000; ui8->fFluxCapData[23] = 0.000000; ui8->fFluxCapData[24] = 0.000000; ui8->fFluxCapData[25] = 0.000000; ui8->fFluxCapData[26] = 0.000000; ui8->fFluxCapData[27] = 0.000000; ui8->fFluxCapData[28] = 0.000000; ui8->fFluxCapData[29] = 0.000000; ui8->fFluxCapData[30] = 0.000000; ui8->fFluxCapData[31] = 0.000000; ui8->fFluxCapData[32] = 0.000000; ui8->fFluxCapData[33] = 0.000000; ui8->fFluxCapData[34] = 0.000000; ui8->fFluxCapData[35] = 0.000000; ui8->fFluxCapData[36] = 0.000000; ui8->fFluxCapData[37] = 0.000000; ui8->fFluxCapData[38] = 0.000000; ui8->fFluxCapData[39] = 0.000000; ui8->fFluxCapData[40] = 0.000000; ui8->fFluxCapData[41] = 0.000000; ui8->fFluxCapData[42] = 0.000000; ui8->fFluxCapData[43] = 0.000000; ui8->fFluxCapData[44] = 0.000000; ui8->fFluxCapData[45] = 0.000000; ui8->fFluxCapData[46] = 0.000000; ui8->fFluxCapData[47] = 0.000000; ui8->fFluxCapData[48] = 0.000000; ui8->fFluxCapData[49] = 0.000000; ui8->fFluxCapData[50] = 0.000000; ui8->fFluxCapData[51] = 0.000000; ui8->fFluxCapData[52] = 0.000000; ui8->fFluxCapData[53] = 0.000000; ui8->fFluxCapData[54] = 0.000000; ui8->fFluxCapData[55] = 0.000000; ui8->fFluxCapData[56] = 0.000000; ui8->fFluxCapData[57] = 0.000000; ui8->fFluxCapData[58] = 0.000000; ui8->fFluxCapData[59] = 0.000000; ui8->fFluxCapData[60] = 0.000000; ui8->fFluxCapData[61] = 0.000000; ui8->fFluxCapData[62] = 0.000000; ui8->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui8);


	m_arp1_semitones = 0;
	CUICtrl* ui9 = new CUICtrl;
	ui9->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui9->uControlId = 10;
	ui9->bLogSlider = false;
	ui9->bExpSlider = false;
	ui9->fUserDisplayDataLoLimit = -12.000000;
	ui9->fUserDisplayDataHiLimit = 12.000000;
	ui9->uUserDataType = intData;
	ui9->fInitUserIntValue = 0.000000;
	ui9->fInitUserFloatValue = 0;
	ui9->fInitUserDoubleValue = 0;
	ui9->fInitUserUINTValue = 0;
	ui9->m_pUserCookedIntData = &m_arp1_semitones;
	ui9->m_pUserCookedFloatData = NULL;
	ui9->m_pUserCookedDoubleData = NULL;
	ui9->m_pUserCookedUINTData = NULL;
	ui9->cControlUnits = "";
	ui9->cVariableName = "m_arp1_semitones";
	ui9->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui9->dPresetData[0] = 0.000000;ui9->dPresetData[1] = 0.000000;ui9->dPresetData[2] = 0.000000;ui9->dPresetData[3] = 0.000000;ui9->dPresetData[4] = 0.000000;ui9->dPresetData[5] = 0.000000;ui9->dPresetData[6] = 0.000000;ui9->dPresetData[7] = 0.000000;ui9->dPresetData[8] = 0.000000;ui9->dPresetData[9] = 0.000000;ui9->dPresetData[10] = 0.000000;ui9->dPresetData[11] = 0.000000;ui9->dPresetData[12] = 0.000000;ui9->dPresetData[13] = 0.000000;ui9->dPresetData[14] = 0.000000;ui9->dPresetData[15] = 0.000000;
	ui9->cControlName = "arp1_semi";
	ui9->bOwnerControl = false;
	ui9->bMIDIControl = false;
	ui9->uMIDIControlCommand = 176;
	ui9->uMIDIControlName = 3;
	ui9->uMIDIControlChannel = 0;
	ui9->nGUIRow = nIndexer++;
	ui9->nGUIColumn = -1;
	ui9->bEnableParamSmoothing = false;
	ui9->fSmoothingTimeInMs = 100.00;
	ui9->uControlTheme[0] = 0; ui9->uControlTheme[1] = 0; ui9->uControlTheme[2] = 0; ui9->uControlTheme[3] = 0; ui9->uControlTheme[4] = 0; ui9->uControlTheme[5] = 0; ui9->uControlTheme[6] = 0; ui9->uControlTheme[7] = 0; ui9->uControlTheme[8] = 0; ui9->uControlTheme[9] = 0; ui9->uControlTheme[10] = 0; ui9->uControlTheme[11] = 0; ui9->uControlTheme[12] = 0; ui9->uControlTheme[13] = 0; ui9->uControlTheme[14] = 0; ui9->uControlTheme[15] = 0; ui9->uControlTheme[16] = 2; ui9->uControlTheme[17] = 0; ui9->uControlTheme[18] = 0; ui9->uControlTheme[19] = 0; ui9->uControlTheme[20] = 0; ui9->uControlTheme[21] = 0; ui9->uControlTheme[22] = 0; ui9->uControlTheme[23] = 0; ui9->uControlTheme[24] = 0; ui9->uControlTheme[25] = 0; ui9->uControlTheme[26] = 0; ui9->uControlTheme[27] = 0; ui9->uControlTheme[28] = 0; ui9->uControlTheme[29] = 0; ui9->uControlTheme[30] = 0; ui9->uControlTheme[31] = 0; 
	ui9->uFluxCapControl[0] = 0; ui9->uFluxCapControl[1] = 0; ui9->uFluxCapControl[2] = 0; ui9->uFluxCapControl[3] = 0; ui9->uFluxCapControl[4] = 0; ui9->uFluxCapControl[5] = 0; ui9->uFluxCapControl[6] = 0; ui9->uFluxCapControl[7] = 0; ui9->uFluxCapControl[8] = 0; ui9->uFluxCapControl[9] = 0; ui9->uFluxCapControl[10] = 0; ui9->uFluxCapControl[11] = 0; ui9->uFluxCapControl[12] = 0; ui9->uFluxCapControl[13] = 0; ui9->uFluxCapControl[14] = 0; ui9->uFluxCapControl[15] = 0; ui9->uFluxCapControl[16] = 0; ui9->uFluxCapControl[17] = 0; ui9->uFluxCapControl[18] = 0; ui9->uFluxCapControl[19] = 0; ui9->uFluxCapControl[20] = 0; ui9->uFluxCapControl[21] = 0; ui9->uFluxCapControl[22] = 0; ui9->uFluxCapControl[23] = 0; ui9->uFluxCapControl[24] = 0; ui9->uFluxCapControl[25] = 0; ui9->uFluxCapControl[26] = 0; ui9->uFluxCapControl[27] = 0; ui9->uFluxCapControl[28] = 0; ui9->uFluxCapControl[29] = 0; ui9->uFluxCapControl[30] = 0; ui9->uFluxCapControl[31] = 0; ui9->uFluxCapControl[32] = 0; ui9->uFluxCapControl[33] = 0; ui9->uFluxCapControl[34] = 0; ui9->uFluxCapControl[35] = 0; ui9->uFluxCapControl[36] = 0; ui9->uFluxCapControl[37] = 0; ui9->uFluxCapControl[38] = 0; ui9->uFluxCapControl[39] = 0; ui9->uFluxCapControl[40] = 0; ui9->uFluxCapControl[41] = 0; ui9->uFluxCapControl[42] = 0; ui9->uFluxCapControl[43] = 0; ui9->uFluxCapControl[44] = 0; ui9->uFluxCapControl[45] = 0; ui9->uFluxCapControl[46] = 0; ui9->uFluxCapControl[47] = 0; ui9->uFluxCapControl[48] = 0; ui9->uFluxCapControl[49] = 0; ui9->uFluxCapControl[50] = 0; ui9->uFluxCapControl[51] = 0; ui9->uFluxCapControl[52] = 0; ui9->uFluxCapControl[53] = 0; ui9->uFluxCapControl[54] = 0; ui9->uFluxCapControl[55] = 0; ui9->uFluxCapControl[56] = 0; ui9->uFluxCapControl[57] = 0; ui9->uFluxCapControl[58] = 0; ui9->uFluxCapControl[59] = 0; ui9->uFluxCapControl[60] = 0; ui9->uFluxCapControl[61] = 0; ui9->uFluxCapControl[62] = 0; ui9->uFluxCapControl[63] = 0; 
	ui9->fFluxCapData[0] = 0.000000; ui9->fFluxCapData[1] = 0.000000; ui9->fFluxCapData[2] = 0.000000; ui9->fFluxCapData[3] = 0.000000; ui9->fFluxCapData[4] = 0.000000; ui9->fFluxCapData[5] = 0.000000; ui9->fFluxCapData[6] = 0.000000; ui9->fFluxCapData[7] = 0.000000; ui9->fFluxCapData[8] = 0.000000; ui9->fFluxCapData[9] = 0.000000; ui9->fFluxCapData[10] = 0.000000; ui9->fFluxCapData[11] = 0.000000; ui9->fFluxCapData[12] = 0.000000; ui9->fFluxCapData[13] = 0.000000; ui9->fFluxCapData[14] = 0.000000; ui9->fFluxCapData[15] = 0.000000; ui9->fFluxCapData[16] = 0.000000; ui9->fFluxCapData[17] = 0.000000; ui9->fFluxCapData[18] = 0.000000; ui9->fFluxCapData[19] = 0.000000; ui9->fFluxCapData[20] = 0.000000; ui9->fFluxCapData[21] = 0.000000; ui9->fFluxCapData[22] = 0.000000; ui9->fFluxCapData[23] = 0.000000; ui9->fFluxCapData[24] = 0.000000; ui9->fFluxCapData[25] = 0.000000; ui9->fFluxCapData[26] = 0.000000; ui9->fFluxCapData[27] = 0.000000; ui9->fFluxCapData[28] = 0.000000; ui9->fFluxCapData[29] = 0.000000; ui9->fFluxCapData[30] = 0.000000; ui9->fFluxCapData[31] = 0.000000; ui9->fFluxCapData[32] = 0.000000; ui9->fFluxCapData[33] = 0.000000; ui9->fFluxCapData[34] = 0.000000; ui9->fFluxCapData[35] = 0.000000; ui9->fFluxCapData[36] = 0.000000; ui9->fFluxCapData[37] = 0.000000; ui9->fFluxCapData[38] = 0.000000; ui9->fFluxCapData[39] = 0.000000; ui9->fFluxCapData[40] = 0.000000; ui9->fFluxCapData[41] = 0.000000; ui9->fFluxCapData[42] = 0.000000; ui9->fFluxCapData[43] = 0.000000; ui9->fFluxCapData[44] = 0.000000; ui9->fFluxCapData[45] = 0.000000; ui9->fFluxCapData[46] = 0.000000; ui9->fFluxCapData[47] = 0.000000; ui9->fFluxCapData[48] = 0.000000; ui9->fFluxCapData[49] = 0.000000; ui9->fFluxCapData[50] = 0.000000; ui9->fFluxCapData[51] = 0.000000; ui9->fFluxCapData[52] = 0.000000; ui9->fFluxCapData[53] = 0.000000; ui9->fFluxCapData[54] = 0.000000; ui9->fFluxCapData[55] = 0.000000; ui9->fFluxCapData[56] = 0.000000; ui9->fFluxCapData[57] = 0.000000; ui9->fFluxCapData[58] = 0.000000; ui9->fFluxCapData[59] = 0.000000; ui9->fFluxCapData[60] = 0.000000; ui9->fFluxCapData[61] = 0.000000; ui9->fFluxCapData[62] = 0.000000; ui9->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui9);


	m_arp2_semitones = 0;
	CUICtrl* ui10 = new CUICtrl;
	ui10->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui10->uControlId = 11;
	ui10->bLogSlider = false;
	ui10->bExpSlider = false;
	ui10->fUserDisplayDataLoLimit = -12.000000;
	ui10->fUserDisplayDataHiLimit = 12.000000;
	ui10->uUserDataType = intData;
	ui10->fInitUserIntValue = 0.000000;
	ui10->fInitUserFloatValue = 0;
	ui10->fInitUserDoubleValue = 0;
	ui10->fInitUserUINTValue = 0;
	ui10->m_pUserCookedIntData = &m_arp2_semitones;
	ui10->m_pUserCookedFloatData = NULL;
	ui10->m_pUserCookedDoubleData = NULL;
	ui10->m_pUserCookedUINTData = NULL;
	ui10->cControlUnits = "";
	ui10->cVariableName = "m_arp2_semitones";
	ui10->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui10->dPresetData[0] = 0.000000;ui10->dPresetData[1] = 0.000000;ui10->dPresetData[2] = 0.000000;ui10->dPresetData[3] = 0.000000;ui10->dPresetData[4] = 0.000000;ui10->dPresetData[5] = 0.000000;ui10->dPresetData[6] = 0.000000;ui10->dPresetData[7] = 0.000000;ui10->dPresetData[8] = 0.000000;ui10->dPresetData[9] = 0.000000;ui10->dPresetData[10] = 0.000000;ui10->dPresetData[11] = 0.000000;ui10->dPresetData[12] = 0.000000;ui10->dPresetData[13] = 0.000000;ui10->dPresetData[14] = 0.000000;ui10->dPresetData[15] = 0.000000;
	ui10->cControlName = "arp2_semi";
	ui10->bOwnerControl = false;
	ui10->bMIDIControl = false;
	ui10->uMIDIControlCommand = 176;
	ui10->uMIDIControlName = 3;
	ui10->uMIDIControlChannel = 0;
	ui10->nGUIRow = nIndexer++;
	ui10->nGUIColumn = -1;
	ui10->bEnableParamSmoothing = false;
	ui10->fSmoothingTimeInMs = 100.00;
	ui10->uControlTheme[0] = 0; ui10->uControlTheme[1] = 0; ui10->uControlTheme[2] = 0; ui10->uControlTheme[3] = 0; ui10->uControlTheme[4] = 0; ui10->uControlTheme[5] = 0; ui10->uControlTheme[6] = 0; ui10->uControlTheme[7] = 0; ui10->uControlTheme[8] = 0; ui10->uControlTheme[9] = 0; ui10->uControlTheme[10] = 0; ui10->uControlTheme[11] = 0; ui10->uControlTheme[12] = 0; ui10->uControlTheme[13] = 0; ui10->uControlTheme[14] = 0; ui10->uControlTheme[15] = 0; ui10->uControlTheme[16] = 2; ui10->uControlTheme[17] = 0; ui10->uControlTheme[18] = 0; ui10->uControlTheme[19] = 0; ui10->uControlTheme[20] = 0; ui10->uControlTheme[21] = 0; ui10->uControlTheme[22] = 0; ui10->uControlTheme[23] = 0; ui10->uControlTheme[24] = 0; ui10->uControlTheme[25] = 0; ui10->uControlTheme[26] = 0; ui10->uControlTheme[27] = 0; ui10->uControlTheme[28] = 0; ui10->uControlTheme[29] = 0; ui10->uControlTheme[30] = 0; ui10->uControlTheme[31] = 0; 
	ui10->uFluxCapControl[0] = 0; ui10->uFluxCapControl[1] = 0; ui10->uFluxCapControl[2] = 0; ui10->uFluxCapControl[3] = 0; ui10->uFluxCapControl[4] = 0; ui10->uFluxCapControl[5] = 0; ui10->uFluxCapControl[6] = 0; ui10->uFluxCapControl[7] = 0; ui10->uFluxCapControl[8] = 0; ui10->uFluxCapControl[9] = 0; ui10->uFluxCapControl[10] = 0; ui10->uFluxCapControl[11] = 0; ui10->uFluxCapControl[12] = 0; ui10->uFluxCapControl[13] = 0; ui10->uFluxCapControl[14] = 0; ui10->uFluxCapControl[15] = 0; ui10->uFluxCapControl[16] = 0; ui10->uFluxCapControl[17] = 0; ui10->uFluxCapControl[18] = 0; ui10->uFluxCapControl[19] = 0; ui10->uFluxCapControl[20] = 0; ui10->uFluxCapControl[21] = 0; ui10->uFluxCapControl[22] = 0; ui10->uFluxCapControl[23] = 0; ui10->uFluxCapControl[24] = 0; ui10->uFluxCapControl[25] = 0; ui10->uFluxCapControl[26] = 0; ui10->uFluxCapControl[27] = 0; ui10->uFluxCapControl[28] = 0; ui10->uFluxCapControl[29] = 0; ui10->uFluxCapControl[30] = 0; ui10->uFluxCapControl[31] = 0; ui10->uFluxCapControl[32] = 0; ui10->uFluxCapControl[33] = 0; ui10->uFluxCapControl[34] = 0; ui10->uFluxCapControl[35] = 0; ui10->uFluxCapControl[36] = 0; ui10->uFluxCapControl[37] = 0; ui10->uFluxCapControl[38] = 0; ui10->uFluxCapControl[39] = 0; ui10->uFluxCapControl[40] = 0; ui10->uFluxCapControl[41] = 0; ui10->uFluxCapControl[42] = 0; ui10->uFluxCapControl[43] = 0; ui10->uFluxCapControl[44] = 0; ui10->uFluxCapControl[45] = 0; ui10->uFluxCapControl[46] = 0; ui10->uFluxCapControl[47] = 0; ui10->uFluxCapControl[48] = 0; ui10->uFluxCapControl[49] = 0; ui10->uFluxCapControl[50] = 0; ui10->uFluxCapControl[51] = 0; ui10->uFluxCapControl[52] = 0; ui10->uFluxCapControl[53] = 0; ui10->uFluxCapControl[54] = 0; ui10->uFluxCapControl[55] = 0; ui10->uFluxCapControl[56] = 0; ui10->uFluxCapControl[57] = 0; ui10->uFluxCapControl[58] = 0; ui10->uFluxCapControl[59] = 0; ui10->uFluxCapControl[60] = 0; ui10->uFluxCapControl[61] = 0; ui10->uFluxCapControl[62] = 0; ui10->uFluxCapControl[63] = 0; 
	ui10->fFluxCapData[0] = 0.000000; ui10->fFluxCapData[1] = 0.000000; ui10->fFluxCapData[2] = 0.000000; ui10->fFluxCapData[3] = 0.000000; ui10->fFluxCapData[4] = 0.000000; ui10->fFluxCapData[5] = 0.000000; ui10->fFluxCapData[6] = 0.000000; ui10->fFluxCapData[7] = 0.000000; ui10->fFluxCapData[8] = 0.000000; ui10->fFluxCapData[9] = 0.000000; ui10->fFluxCapData[10] = 0.000000; ui10->fFluxCapData[11] = 0.000000; ui10->fFluxCapData[12] = 0.000000; ui10->fFluxCapData[13] = 0.000000; ui10->fFluxCapData[14] = 0.000000; ui10->fFluxCapData[15] = 0.000000; ui10->fFluxCapData[16] = 0.000000; ui10->fFluxCapData[17] = 0.000000; ui10->fFluxCapData[18] = 0.000000; ui10->fFluxCapData[19] = 0.000000; ui10->fFluxCapData[20] = 0.000000; ui10->fFluxCapData[21] = 0.000000; ui10->fFluxCapData[22] = 0.000000; ui10->fFluxCapData[23] = 0.000000; ui10->fFluxCapData[24] = 0.000000; ui10->fFluxCapData[25] = 0.000000; ui10->fFluxCapData[26] = 0.000000; ui10->fFluxCapData[27] = 0.000000; ui10->fFluxCapData[28] = 0.000000; ui10->fFluxCapData[29] = 0.000000; ui10->fFluxCapData[30] = 0.000000; ui10->fFluxCapData[31] = 0.000000; ui10->fFluxCapData[32] = 0.000000; ui10->fFluxCapData[33] = 0.000000; ui10->fFluxCapData[34] = 0.000000; ui10->fFluxCapData[35] = 0.000000; ui10->fFluxCapData[36] = 0.000000; ui10->fFluxCapData[37] = 0.000000; ui10->fFluxCapData[38] = 0.000000; ui10->fFluxCapData[39] = 0.000000; ui10->fFluxCapData[40] = 0.000000; ui10->fFluxCapData[41] = 0.000000; ui10->fFluxCapData[42] = 0.000000; ui10->fFluxCapData[43] = 0.000000; ui10->fFluxCapData[44] = 0.000000; ui10->fFluxCapData[45] = 0.000000; ui10->fFluxCapData[46] = 0.000000; ui10->fFluxCapData[47] = 0.000000; ui10->fFluxCapData[48] = 0.000000; ui10->fFluxCapData[49] = 0.000000; ui10->fFluxCapData[50] = 0.000000; ui10->fFluxCapData[51] = 0.000000; ui10->fFluxCapData[52] = 0.000000; ui10->fFluxCapData[53] = 0.000000; ui10->fFluxCapData[54] = 0.000000; ui10->fFluxCapData[55] = 0.000000; ui10->fFluxCapData[56] = 0.000000; ui10->fFluxCapData[57] = 0.000000; ui10->fFluxCapData[58] = 0.000000; ui10->fFluxCapData[59] = 0.000000; ui10->fFluxCapData[60] = 0.000000; ui10->fFluxCapData[61] = 0.000000; ui10->fFluxCapData[62] = 0.000000; ui10->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui10);


	m_arp3_semitones = 0;
	CUICtrl* ui11 = new CUICtrl;
	ui11->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui11->uControlId = 12;
	ui11->bLogSlider = false;
	ui11->bExpSlider = false;
	ui11->fUserDisplayDataLoLimit = -12.000000;
	ui11->fUserDisplayDataHiLimit = 12.000000;
	ui11->uUserDataType = intData;
	ui11->fInitUserIntValue = 0.000000;
	ui11->fInitUserFloatValue = 0;
	ui11->fInitUserDoubleValue = 0;
	ui11->fInitUserUINTValue = 0;
	ui11->m_pUserCookedIntData = &m_arp3_semitones;
	ui11->m_pUserCookedFloatData = NULL;
	ui11->m_pUserCookedDoubleData = NULL;
	ui11->m_pUserCookedUINTData = NULL;
	ui11->cControlUnits = "";
	ui11->cVariableName = "m_arp3_semitones";
	ui11->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui11->dPresetData[0] = 0.000000;ui11->dPresetData[1] = 0.000000;ui11->dPresetData[2] = 0.000000;ui11->dPresetData[3] = 0.000000;ui11->dPresetData[4] = 0.000000;ui11->dPresetData[5] = 0.000000;ui11->dPresetData[6] = 0.000000;ui11->dPresetData[7] = 0.000000;ui11->dPresetData[8] = 0.000000;ui11->dPresetData[9] = 0.000000;ui11->dPresetData[10] = 0.000000;ui11->dPresetData[11] = 0.000000;ui11->dPresetData[12] = 0.000000;ui11->dPresetData[13] = 0.000000;ui11->dPresetData[14] = 0.000000;ui11->dPresetData[15] = 0.000000;
	ui11->cControlName = "arp3_semi";
	ui11->bOwnerControl = false;
	ui11->bMIDIControl = false;
	ui11->uMIDIControlCommand = 176;
	ui11->uMIDIControlName = 3;
	ui11->uMIDIControlChannel = 0;
	ui11->nGUIRow = nIndexer++;
	ui11->nGUIColumn = -1;
	ui11->bEnableParamSmoothing = false;
	ui11->fSmoothingTimeInMs = 100.00;
	ui11->uControlTheme[0] = 0; ui11->uControlTheme[1] = 0; ui11->uControlTheme[2] = 0; ui11->uControlTheme[3] = 0; ui11->uControlTheme[4] = 0; ui11->uControlTheme[5] = 0; ui11->uControlTheme[6] = 0; ui11->uControlTheme[7] = 0; ui11->uControlTheme[8] = 0; ui11->uControlTheme[9] = 0; ui11->uControlTheme[10] = 0; ui11->uControlTheme[11] = 0; ui11->uControlTheme[12] = 0; ui11->uControlTheme[13] = 0; ui11->uControlTheme[14] = 0; ui11->uControlTheme[15] = 0; ui11->uControlTheme[16] = 2; ui11->uControlTheme[17] = 0; ui11->uControlTheme[18] = 0; ui11->uControlTheme[19] = 0; ui11->uControlTheme[20] = 0; ui11->uControlTheme[21] = 0; ui11->uControlTheme[22] = 0; ui11->uControlTheme[23] = 0; ui11->uControlTheme[24] = 0; ui11->uControlTheme[25] = 0; ui11->uControlTheme[26] = 0; ui11->uControlTheme[27] = 0; ui11->uControlTheme[28] = 0; ui11->uControlTheme[29] = 0; ui11->uControlTheme[30] = 0; ui11->uControlTheme[31] = 0; 
	ui11->uFluxCapControl[0] = 0; ui11->uFluxCapControl[1] = 0; ui11->uFluxCapControl[2] = 0; ui11->uFluxCapControl[3] = 0; ui11->uFluxCapControl[4] = 0; ui11->uFluxCapControl[5] = 0; ui11->uFluxCapControl[6] = 0; ui11->uFluxCapControl[7] = 0; ui11->uFluxCapControl[8] = 0; ui11->uFluxCapControl[9] = 0; ui11->uFluxCapControl[10] = 0; ui11->uFluxCapControl[11] = 0; ui11->uFluxCapControl[12] = 0; ui11->uFluxCapControl[13] = 0; ui11->uFluxCapControl[14] = 0; ui11->uFluxCapControl[15] = 0; ui11->uFluxCapControl[16] = 0; ui11->uFluxCapControl[17] = 0; ui11->uFluxCapControl[18] = 0; ui11->uFluxCapControl[19] = 0; ui11->uFluxCapControl[20] = 0; ui11->uFluxCapControl[21] = 0; ui11->uFluxCapControl[22] = 0; ui11->uFluxCapControl[23] = 0; ui11->uFluxCapControl[24] = 0; ui11->uFluxCapControl[25] = 0; ui11->uFluxCapControl[26] = 0; ui11->uFluxCapControl[27] = 0; ui11->uFluxCapControl[28] = 0; ui11->uFluxCapControl[29] = 0; ui11->uFluxCapControl[30] = 0; ui11->uFluxCapControl[31] = 0; ui11->uFluxCapControl[32] = 0; ui11->uFluxCapControl[33] = 0; ui11->uFluxCapControl[34] = 0; ui11->uFluxCapControl[35] = 0; ui11->uFluxCapControl[36] = 0; ui11->uFluxCapControl[37] = 0; ui11->uFluxCapControl[38] = 0; ui11->uFluxCapControl[39] = 0; ui11->uFluxCapControl[40] = 0; ui11->uFluxCapControl[41] = 0; ui11->uFluxCapControl[42] = 0; ui11->uFluxCapControl[43] = 0; ui11->uFluxCapControl[44] = 0; ui11->uFluxCapControl[45] = 0; ui11->uFluxCapControl[46] = 0; ui11->uFluxCapControl[47] = 0; ui11->uFluxCapControl[48] = 0; ui11->uFluxCapControl[49] = 0; ui11->uFluxCapControl[50] = 0; ui11->uFluxCapControl[51] = 0; ui11->uFluxCapControl[52] = 0; ui11->uFluxCapControl[53] = 0; ui11->uFluxCapControl[54] = 0; ui11->uFluxCapControl[55] = 0; ui11->uFluxCapControl[56] = 0; ui11->uFluxCapControl[57] = 0; ui11->uFluxCapControl[58] = 0; ui11->uFluxCapControl[59] = 0; ui11->uFluxCapControl[60] = 0; ui11->uFluxCapControl[61] = 0; ui11->uFluxCapControl[62] = 0; ui11->uFluxCapControl[63] = 0; 
	ui11->fFluxCapData[0] = 0.000000; ui11->fFluxCapData[1] = 0.000000; ui11->fFluxCapData[2] = 0.000000; ui11->fFluxCapData[3] = 0.000000; ui11->fFluxCapData[4] = 0.000000; ui11->fFluxCapData[5] = 0.000000; ui11->fFluxCapData[6] = 0.000000; ui11->fFluxCapData[7] = 0.000000; ui11->fFluxCapData[8] = 0.000000; ui11->fFluxCapData[9] = 0.000000; ui11->fFluxCapData[10] = 0.000000; ui11->fFluxCapData[11] = 0.000000; ui11->fFluxCapData[12] = 0.000000; ui11->fFluxCapData[13] = 0.000000; ui11->fFluxCapData[14] = 0.000000; ui11->fFluxCapData[15] = 0.000000; ui11->fFluxCapData[16] = 0.000000; ui11->fFluxCapData[17] = 0.000000; ui11->fFluxCapData[18] = 0.000000; ui11->fFluxCapData[19] = 0.000000; ui11->fFluxCapData[20] = 0.000000; ui11->fFluxCapData[21] = 0.000000; ui11->fFluxCapData[22] = 0.000000; ui11->fFluxCapData[23] = 0.000000; ui11->fFluxCapData[24] = 0.000000; ui11->fFluxCapData[25] = 0.000000; ui11->fFluxCapData[26] = 0.000000; ui11->fFluxCapData[27] = 0.000000; ui11->fFluxCapData[28] = 0.000000; ui11->fFluxCapData[29] = 0.000000; ui11->fFluxCapData[30] = 0.000000; ui11->fFluxCapData[31] = 0.000000; ui11->fFluxCapData[32] = 0.000000; ui11->fFluxCapData[33] = 0.000000; ui11->fFluxCapData[34] = 0.000000; ui11->fFluxCapData[35] = 0.000000; ui11->fFluxCapData[36] = 0.000000; ui11->fFluxCapData[37] = 0.000000; ui11->fFluxCapData[38] = 0.000000; ui11->fFluxCapData[39] = 0.000000; ui11->fFluxCapData[40] = 0.000000; ui11->fFluxCapData[41] = 0.000000; ui11->fFluxCapData[42] = 0.000000; ui11->fFluxCapData[43] = 0.000000; ui11->fFluxCapData[44] = 0.000000; ui11->fFluxCapData[45] = 0.000000; ui11->fFluxCapData[46] = 0.000000; ui11->fFluxCapData[47] = 0.000000; ui11->fFluxCapData[48] = 0.000000; ui11->fFluxCapData[49] = 0.000000; ui11->fFluxCapData[50] = 0.000000; ui11->fFluxCapData[51] = 0.000000; ui11->fFluxCapData[52] = 0.000000; ui11->fFluxCapData[53] = 0.000000; ui11->fFluxCapData[54] = 0.000000; ui11->fFluxCapData[55] = 0.000000; ui11->fFluxCapData[56] = 0.000000; ui11->fFluxCapData[57] = 0.000000; ui11->fFluxCapData[58] = 0.000000; ui11->fFluxCapData[59] = 0.000000; ui11->fFluxCapData[60] = 0.000000; ui11->fFluxCapData[61] = 0.000000; ui11->fFluxCapData[62] = 0.000000; ui11->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui11);


	m_arp_speed = 20.000000;
	CUICtrl* ui12 = new CUICtrl;
	ui12->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui12->uControlId = 13;
	ui12->bLogSlider = false;
	ui12->bExpSlider = false;
	ui12->fUserDisplayDataLoLimit = 1.000000;
	ui12->fUserDisplayDataHiLimit = 100.000000;
	ui12->uUserDataType = floatData;
	ui12->fInitUserIntValue = 0;
	ui12->fInitUserFloatValue = 20.000000;
	ui12->fInitUserDoubleValue = 0;
	ui12->fInitUserUINTValue = 0;
	ui12->m_pUserCookedIntData = NULL;
	ui12->m_pUserCookedFloatData = &m_arp_speed;
	ui12->m_pUserCookedDoubleData = NULL;
	ui12->m_pUserCookedUINTData = NULL;
	ui12->cControlUnits = "";
	ui12->cVariableName = "m_arp_speed";
	ui12->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui12->dPresetData[0] = 20.000000;ui12->dPresetData[1] = 0.000000;ui12->dPresetData[2] = 0.000000;ui12->dPresetData[3] = 0.000000;ui12->dPresetData[4] = 0.000000;ui12->dPresetData[5] = 0.000000;ui12->dPresetData[6] = 0.000000;ui12->dPresetData[7] = 0.000000;ui12->dPresetData[8] = 0.000000;ui12->dPresetData[9] = 0.000000;ui12->dPresetData[10] = 0.000000;ui12->dPresetData[11] = 0.000000;ui12->dPresetData[12] = 0.000000;ui12->dPresetData[13] = 0.000000;ui12->dPresetData[14] = 0.000000;ui12->dPresetData[15] = 0.000000;
	ui12->cControlName = "arp_speed";
	ui12->bOwnerControl = false;
	ui12->bMIDIControl = false;
	ui12->uMIDIControlCommand = 176;
	ui12->uMIDIControlName = 3;
	ui12->uMIDIControlChannel = 0;
	ui12->nGUIRow = nIndexer++;
	ui12->nGUIColumn = -1;
	ui12->bEnableParamSmoothing = false;
	ui12->fSmoothingTimeInMs = 100.00;
	ui12->uControlTheme[0] = 0; ui12->uControlTheme[1] = 0; ui12->uControlTheme[2] = 0; ui12->uControlTheme[3] = 0; ui12->uControlTheme[4] = 0; ui12->uControlTheme[5] = 0; ui12->uControlTheme[6] = 0; ui12->uControlTheme[7] = 0; ui12->uControlTheme[8] = 8; ui12->uControlTheme[9] = 0; ui12->uControlTheme[10] = 0; ui12->uControlTheme[11] = 0; ui12->uControlTheme[12] = 0; ui12->uControlTheme[13] = 0; ui12->uControlTheme[14] = 0; ui12->uControlTheme[15] = 0; ui12->uControlTheme[16] = 2; ui12->uControlTheme[17] = 0; ui12->uControlTheme[18] = 0; ui12->uControlTheme[19] = 0; ui12->uControlTheme[20] = 0; ui12->uControlTheme[21] = 0; ui12->uControlTheme[22] = 0; ui12->uControlTheme[23] = 0; ui12->uControlTheme[24] = 0; ui12->uControlTheme[25] = 0; ui12->uControlTheme[26] = 0; ui12->uControlTheme[27] = 0; ui12->uControlTheme[28] = 0; ui12->uControlTheme[29] = 0; ui12->uControlTheme[30] = 0; ui12->uControlTheme[31] = 0; 
	ui12->uFluxCapControl[0] = 0; ui12->uFluxCapControl[1] = 0; ui12->uFluxCapControl[2] = 0; ui12->uFluxCapControl[3] = 0; ui12->uFluxCapControl[4] = 0; ui12->uFluxCapControl[5] = 0; ui12->uFluxCapControl[6] = 0; ui12->uFluxCapControl[7] = 0; ui12->uFluxCapControl[8] = 0; ui12->uFluxCapControl[9] = 0; ui12->uFluxCapControl[10] = 0; ui12->uFluxCapControl[11] = 0; ui12->uFluxCapControl[12] = 0; ui12->uFluxCapControl[13] = 0; ui12->uFluxCapControl[14] = 0; ui12->uFluxCapControl[15] = 0; ui12->uFluxCapControl[16] = 0; ui12->uFluxCapControl[17] = 0; ui12->uFluxCapControl[18] = 0; ui12->uFluxCapControl[19] = 0; ui12->uFluxCapControl[20] = 0; ui12->uFluxCapControl[21] = 0; ui12->uFluxCapControl[22] = 0; ui12->uFluxCapControl[23] = 0; ui12->uFluxCapControl[24] = 0; ui12->uFluxCapControl[25] = 0; ui12->uFluxCapControl[26] = 0; ui12->uFluxCapControl[27] = 0; ui12->uFluxCapControl[28] = 0; ui12->uFluxCapControl[29] = 0; ui12->uFluxCapControl[30] = 0; ui12->uFluxCapControl[31] = 0; ui12->uFluxCapControl[32] = 0; ui12->uFluxCapControl[33] = 0; ui12->uFluxCapControl[34] = 0; ui12->uFluxCapControl[35] = 0; ui12->uFluxCapControl[36] = 0; ui12->uFluxCapControl[37] = 0; ui12->uFluxCapControl[38] = 0; ui12->uFluxCapControl[39] = 0; ui12->uFluxCapControl[40] = 0; ui12->uFluxCapControl[41] = 0; ui12->uFluxCapControl[42] = 0; ui12->uFluxCapControl[43] = 0; ui12->uFluxCapControl[44] = 0; ui12->uFluxCapControl[45] = 0; ui12->uFluxCapControl[46] = 0; ui12->uFluxCapControl[47] = 0; ui12->uFluxCapControl[48] = 0; ui12->uFluxCapControl[49] = 0; ui12->uFluxCapControl[50] = 0; ui12->uFluxCapControl[51] = 0; ui12->uFluxCapControl[52] = 0; ui12->uFluxCapControl[53] = 0; ui12->uFluxCapControl[54] = 0; ui12->uFluxCapControl[55] = 0; ui12->uFluxCapControl[56] = 0; ui12->uFluxCapControl[57] = 0; ui12->uFluxCapControl[58] = 0; ui12->uFluxCapControl[59] = 0; ui12->uFluxCapControl[60] = 0; ui12->uFluxCapControl[61] = 0; ui12->uFluxCapControl[62] = 0; ui12->uFluxCapControl[63] = 0; 
	ui12->fFluxCapData[0] = 0.000000; ui12->fFluxCapData[1] = 0.000000; ui12->fFluxCapData[2] = 0.000000; ui12->fFluxCapData[3] = 0.000000; ui12->fFluxCapData[4] = 0.000000; ui12->fFluxCapData[5] = 0.000000; ui12->fFluxCapData[6] = 0.000000; ui12->fFluxCapData[7] = 0.000000; ui12->fFluxCapData[8] = 0.000000; ui12->fFluxCapData[9] = 0.000000; ui12->fFluxCapData[10] = 0.000000; ui12->fFluxCapData[11] = 0.000000; ui12->fFluxCapData[12] = 0.000000; ui12->fFluxCapData[13] = 0.000000; ui12->fFluxCapData[14] = 0.000000; ui12->fFluxCapData[15] = 0.000000; ui12->fFluxCapData[16] = 0.000000; ui12->fFluxCapData[17] = 0.000000; ui12->fFluxCapData[18] = 0.000000; ui12->fFluxCapData[19] = 0.000000; ui12->fFluxCapData[20] = 0.000000; ui12->fFluxCapData[21] = 0.000000; ui12->fFluxCapData[22] = 0.000000; ui12->fFluxCapData[23] = 0.000000; ui12->fFluxCapData[24] = 0.000000; ui12->fFluxCapData[25] = 0.000000; ui12->fFluxCapData[26] = 0.000000; ui12->fFluxCapData[27] = 0.000000; ui12->fFluxCapData[28] = 0.000000; ui12->fFluxCapData[29] = 0.000000; ui12->fFluxCapData[30] = 0.000000; ui12->fFluxCapData[31] = 0.000000; ui12->fFluxCapData[32] = 0.000000; ui12->fFluxCapData[33] = 0.000000; ui12->fFluxCapData[34] = 0.000000; ui12->fFluxCapData[35] = 0.000000; ui12->fFluxCapData[36] = 0.000000; ui12->fFluxCapData[37] = 0.000000; ui12->fFluxCapData[38] = 0.000000; ui12->fFluxCapData[39] = 0.000000; ui12->fFluxCapData[40] = 0.000000; ui12->fFluxCapData[41] = 0.000000; ui12->fFluxCapData[42] = 0.000000; ui12->fFluxCapData[43] = 0.000000; ui12->fFluxCapData[44] = 0.000000; ui12->fFluxCapData[45] = 0.000000; ui12->fFluxCapData[46] = 0.000000; ui12->fFluxCapData[47] = 0.000000; ui12->fFluxCapData[48] = 0.000000; ui12->fFluxCapData[49] = 0.000000; ui12->fFluxCapData[50] = 0.000000; ui12->fFluxCapData[51] = 0.000000; ui12->fFluxCapData[52] = 0.000000; ui12->fFluxCapData[53] = 0.000000; ui12->fFluxCapData[54] = 0.000000; ui12->fFluxCapData[55] = 0.000000; ui12->fFluxCapData[56] = 0.000000; ui12->fFluxCapData[57] = 0.000000; ui12->fFluxCapData[58] = 0.000000; ui12->fFluxCapData[59] = 0.000000; ui12->fFluxCapData[60] = 0.000000; ui12->fFluxCapData[61] = 0.000000; ui12->fFluxCapData[62] = 0.000000; ui12->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui12);


	m_arp_step3_active = 1;
	CUICtrl* ui13 = new CUICtrl;
	ui13->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui13->uControlId = 22;
	ui13->bLogSlider = false;
	ui13->bExpSlider = false;
	ui13->fUserDisplayDataLoLimit = 0.000000;
	ui13->fUserDisplayDataHiLimit = 1.000000;
	ui13->uUserDataType = intData;
	ui13->fInitUserIntValue = 1.000000;
	ui13->fInitUserFloatValue = 0;
	ui13->fInitUserDoubleValue = 0;
	ui13->fInitUserUINTValue = 0;
	ui13->m_pUserCookedIntData = &m_arp_step3_active;
	ui13->m_pUserCookedFloatData = NULL;
	ui13->m_pUserCookedDoubleData = NULL;
	ui13->m_pUserCookedUINTData = NULL;
	ui13->cControlUnits = "";
	ui13->cVariableName = "m_arp_step3_active";
	ui13->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui13->dPresetData[0] = 1.000000;ui13->dPresetData[1] = 0.000000;ui13->dPresetData[2] = 0.000000;ui13->dPresetData[3] = 0.000000;ui13->dPresetData[4] = 0.000000;ui13->dPresetData[5] = 0.000000;ui13->dPresetData[6] = 0.000000;ui13->dPresetData[7] = 0.000000;ui13->dPresetData[8] = 0.000000;ui13->dPresetData[9] = 0.000000;ui13->dPresetData[10] = 0.000000;ui13->dPresetData[11] = 0.000000;ui13->dPresetData[12] = 0.000000;ui13->dPresetData[13] = 0.000000;ui13->dPresetData[14] = 0.000000;ui13->dPresetData[15] = 0.000000;
	ui13->cControlName = "arp_step3_active";
	ui13->bOwnerControl = false;
	ui13->bMIDIControl = false;
	ui13->uMIDIControlCommand = 176;
	ui13->uMIDIControlName = 3;
	ui13->uMIDIControlChannel = 0;
	ui13->nGUIRow = nIndexer++;
	ui13->nGUIColumn = -1;
	ui13->bEnableParamSmoothing = false;
	ui13->fSmoothingTimeInMs = 100.00;
	ui13->uControlTheme[0] = 0; ui13->uControlTheme[1] = 0; ui13->uControlTheme[2] = 0; ui13->uControlTheme[3] = 0; ui13->uControlTheme[4] = 0; ui13->uControlTheme[5] = 0; ui13->uControlTheme[6] = 0; ui13->uControlTheme[7] = 0; ui13->uControlTheme[8] = 16; ui13->uControlTheme[9] = 0; ui13->uControlTheme[10] = 0; ui13->uControlTheme[11] = 0; ui13->uControlTheme[12] = 0; ui13->uControlTheme[13] = 0; ui13->uControlTheme[14] = 0; ui13->uControlTheme[15] = 0; ui13->uControlTheme[16] = 2; ui13->uControlTheme[17] = 0; ui13->uControlTheme[18] = 0; ui13->uControlTheme[19] = 0; ui13->uControlTheme[20] = 0; ui13->uControlTheme[21] = 0; ui13->uControlTheme[22] = 0; ui13->uControlTheme[23] = 0; ui13->uControlTheme[24] = 0; ui13->uControlTheme[25] = 0; ui13->uControlTheme[26] = 0; ui13->uControlTheme[27] = 0; ui13->uControlTheme[28] = 0; ui13->uControlTheme[29] = 0; ui13->uControlTheme[30] = 0; ui13->uControlTheme[31] = 0; 
	ui13->uFluxCapControl[0] = 0; ui13->uFluxCapControl[1] = 0; ui13->uFluxCapControl[2] = 0; ui13->uFluxCapControl[3] = 0; ui13->uFluxCapControl[4] = 0; ui13->uFluxCapControl[5] = 0; ui13->uFluxCapControl[6] = 0; ui13->uFluxCapControl[7] = 0; ui13->uFluxCapControl[8] = 0; ui13->uFluxCapControl[9] = 0; ui13->uFluxCapControl[10] = 0; ui13->uFluxCapControl[11] = 0; ui13->uFluxCapControl[12] = 0; ui13->uFluxCapControl[13] = 0; ui13->uFluxCapControl[14] = 0; ui13->uFluxCapControl[15] = 0; ui13->uFluxCapControl[16] = 0; ui13->uFluxCapControl[17] = 0; ui13->uFluxCapControl[18] = 0; ui13->uFluxCapControl[19] = 0; ui13->uFluxCapControl[20] = 0; ui13->uFluxCapControl[21] = 0; ui13->uFluxCapControl[22] = 0; ui13->uFluxCapControl[23] = 0; ui13->uFluxCapControl[24] = 0; ui13->uFluxCapControl[25] = 0; ui13->uFluxCapControl[26] = 0; ui13->uFluxCapControl[27] = 0; ui13->uFluxCapControl[28] = 0; ui13->uFluxCapControl[29] = 0; ui13->uFluxCapControl[30] = 0; ui13->uFluxCapControl[31] = 0; ui13->uFluxCapControl[32] = 0; ui13->uFluxCapControl[33] = 0; ui13->uFluxCapControl[34] = 0; ui13->uFluxCapControl[35] = 0; ui13->uFluxCapControl[36] = 0; ui13->uFluxCapControl[37] = 0; ui13->uFluxCapControl[38] = 0; ui13->uFluxCapControl[39] = 0; ui13->uFluxCapControl[40] = 0; ui13->uFluxCapControl[41] = 0; ui13->uFluxCapControl[42] = 0; ui13->uFluxCapControl[43] = 0; ui13->uFluxCapControl[44] = 0; ui13->uFluxCapControl[45] = 0; ui13->uFluxCapControl[46] = 0; ui13->uFluxCapControl[47] = 0; ui13->uFluxCapControl[48] = 0; ui13->uFluxCapControl[49] = 0; ui13->uFluxCapControl[50] = 0; ui13->uFluxCapControl[51] = 0; ui13->uFluxCapControl[52] = 0; ui13->uFluxCapControl[53] = 0; ui13->uFluxCapControl[54] = 0; ui13->uFluxCapControl[55] = 0; ui13->uFluxCapControl[56] = 0; ui13->uFluxCapControl[57] = 0; ui13->uFluxCapControl[58] = 0; ui13->uFluxCapControl[59] = 0; ui13->uFluxCapControl[60] = 0; ui13->uFluxCapControl[61] = 0; ui13->uFluxCapControl[62] = 0; ui13->uFluxCapControl[63] = 0; 
	ui13->fFluxCapData[0] = 0.000000; ui13->fFluxCapData[1] = 0.000000; ui13->fFluxCapData[2] = 0.000000; ui13->fFluxCapData[3] = 0.000000; ui13->fFluxCapData[4] = 0.000000; ui13->fFluxCapData[5] = 0.000000; ui13->fFluxCapData[6] = 0.000000; ui13->fFluxCapData[7] = 0.000000; ui13->fFluxCapData[8] = 0.000000; ui13->fFluxCapData[9] = 0.000000; ui13->fFluxCapData[10] = 0.000000; ui13->fFluxCapData[11] = 0.000000; ui13->fFluxCapData[12] = 0.000000; ui13->fFluxCapData[13] = 0.000000; ui13->fFluxCapData[14] = 0.000000; ui13->fFluxCapData[15] = 0.000000; ui13->fFluxCapData[16] = 0.000000; ui13->fFluxCapData[17] = 0.000000; ui13->fFluxCapData[18] = 0.000000; ui13->fFluxCapData[19] = 0.000000; ui13->fFluxCapData[20] = 0.000000; ui13->fFluxCapData[21] = 0.000000; ui13->fFluxCapData[22] = 0.000000; ui13->fFluxCapData[23] = 0.000000; ui13->fFluxCapData[24] = 0.000000; ui13->fFluxCapData[25] = 0.000000; ui13->fFluxCapData[26] = 0.000000; ui13->fFluxCapData[27] = 0.000000; ui13->fFluxCapData[28] = 0.000000; ui13->fFluxCapData[29] = 0.000000; ui13->fFluxCapData[30] = 0.000000; ui13->fFluxCapData[31] = 0.000000; ui13->fFluxCapData[32] = 0.000000; ui13->fFluxCapData[33] = 0.000000; ui13->fFluxCapData[34] = 0.000000; ui13->fFluxCapData[35] = 0.000000; ui13->fFluxCapData[36] = 0.000000; ui13->fFluxCapData[37] = 0.000000; ui13->fFluxCapData[38] = 0.000000; ui13->fFluxCapData[39] = 0.000000; ui13->fFluxCapData[40] = 0.000000; ui13->fFluxCapData[41] = 0.000000; ui13->fFluxCapData[42] = 0.000000; ui13->fFluxCapData[43] = 0.000000; ui13->fFluxCapData[44] = 0.000000; ui13->fFluxCapData[45] = 0.000000; ui13->fFluxCapData[46] = 0.000000; ui13->fFluxCapData[47] = 0.000000; ui13->fFluxCapData[48] = 0.000000; ui13->fFluxCapData[49] = 0.000000; ui13->fFluxCapData[50] = 0.000000; ui13->fFluxCapData[51] = 0.000000; ui13->fFluxCapData[52] = 0.000000; ui13->fFluxCapData[53] = 0.000000; ui13->fFluxCapData[54] = 0.000000; ui13->fFluxCapData[55] = 0.000000; ui13->fFluxCapData[56] = 0.000000; ui13->fFluxCapData[57] = 0.000000; ui13->fFluxCapData[58] = 0.000000; ui13->fFluxCapData[59] = 0.000000; ui13->fFluxCapData[60] = 0.000000; ui13->fFluxCapData[61] = 0.000000; ui13->fFluxCapData[62] = 0.000000; ui13->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui13);


	m_analog_drift = 0.000000;
	CUICtrl* ui14 = new CUICtrl;
	ui14->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui14->uControlId = 30;
	ui14->bLogSlider = false;
	ui14->bExpSlider = false;
	ui14->fUserDisplayDataLoLimit = 0.000000;
	ui14->fUserDisplayDataHiLimit = 1.000000;
	ui14->uUserDataType = floatData;
	ui14->fInitUserIntValue = 0;
	ui14->fInitUserFloatValue = 0.000000;
	ui14->fInitUserDoubleValue = 0;
	ui14->fInitUserUINTValue = 0;
	ui14->m_pUserCookedIntData = NULL;
	ui14->m_pUserCookedFloatData = &m_analog_drift;
	ui14->m_pUserCookedDoubleData = NULL;
	ui14->m_pUserCookedUINTData = NULL;
	ui14->cControlUnits = "";
	ui14->cVariableName = "m_analog_drift";
	ui14->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui14->dPresetData[0] = 0.000000;ui14->dPresetData[1] = 0.000000;ui14->dPresetData[2] = 0.000000;ui14->dPresetData[3] = 0.000000;ui14->dPresetData[4] = 0.000000;ui14->dPresetData[5] = 0.000000;ui14->dPresetData[6] = 0.000000;ui14->dPresetData[7] = 0.000000;ui14->dPresetData[8] = 0.000000;ui14->dPresetData[9] = 0.000000;ui14->dPresetData[10] = 0.000000;ui14->dPresetData[11] = 0.000000;ui14->dPresetData[12] = 0.000000;ui14->dPresetData[13] = 0.000000;ui14->dPresetData[14] = 0.000000;ui14->dPresetData[15] = 0.000000;
	ui14->cControlName = "Analog Drift";
	ui14->bOwnerControl = false;
	ui14->bMIDIControl = false;
	ui14->uMIDIControlCommand = 176;
	ui14->uMIDIControlName = 3;
	ui14->uMIDIControlChannel = 0;
	ui14->nGUIRow = nIndexer++;
	ui14->nGUIColumn = -1;
	ui14->bEnableParamSmoothing = false;
	ui14->fSmoothingTimeInMs = 100.00;
	ui14->uControlTheme[0] = 0; ui14->uControlTheme[1] = 0; ui14->uControlTheme[2] = 0; ui14->uControlTheme[3] = 0; ui14->uControlTheme[4] = 0; ui14->uControlTheme[5] = 0; ui14->uControlTheme[6] = 0; ui14->uControlTheme[7] = 0; ui14->uControlTheme[8] = 0; ui14->uControlTheme[9] = 0; ui14->uControlTheme[10] = 0; ui14->uControlTheme[11] = 0; ui14->uControlTheme[12] = 0; ui14->uControlTheme[13] = 0; ui14->uControlTheme[14] = 0; ui14->uControlTheme[15] = 0; ui14->uControlTheme[16] = 2; ui14->uControlTheme[17] = 0; ui14->uControlTheme[18] = 0; ui14->uControlTheme[19] = 0; ui14->uControlTheme[20] = 0; ui14->uControlTheme[21] = 0; ui14->uControlTheme[22] = 0; ui14->uControlTheme[23] = 0; ui14->uControlTheme[24] = 0; ui14->uControlTheme[25] = 0; ui14->uControlTheme[26] = 0; ui14->uControlTheme[27] = 0; ui14->uControlTheme[28] = 0; ui14->uControlTheme[29] = 0; ui14->uControlTheme[30] = 0; ui14->uControlTheme[31] = 0; 
	ui14->uFluxCapControl[0] = 0; ui14->uFluxCapControl[1] = 0; ui14->uFluxCapControl[2] = 0; ui14->uFluxCapControl[3] = 0; ui14->uFluxCapControl[4] = 0; ui14->uFluxCapControl[5] = 0; ui14->uFluxCapControl[6] = 0; ui14->uFluxCapControl[7] = 0; ui14->uFluxCapControl[8] = 0; ui14->uFluxCapControl[9] = 0; ui14->uFluxCapControl[10] = 0; ui14->uFluxCapControl[11] = 0; ui14->uFluxCapControl[12] = 0; ui14->uFluxCapControl[13] = 0; ui14->uFluxCapControl[14] = 0; ui14->uFluxCapControl[15] = 0; ui14->uFluxCapControl[16] = 0; ui14->uFluxCapControl[17] = 0; ui14->uFluxCapControl[18] = 0; ui14->uFluxCapControl[19] = 0; ui14->uFluxCapControl[20] = 0; ui14->uFluxCapControl[21] = 0; ui14->uFluxCapControl[22] = 0; ui14->uFluxCapControl[23] = 0; ui14->uFluxCapControl[24] = 0; ui14->uFluxCapControl[25] = 0; ui14->uFluxCapControl[26] = 0; ui14->uFluxCapControl[27] = 0; ui14->uFluxCapControl[28] = 0; ui14->uFluxCapControl[29] = 0; ui14->uFluxCapControl[30] = 0; ui14->uFluxCapControl[31] = 0; ui14->uFluxCapControl[32] = 0; ui14->uFluxCapControl[33] = 0; ui14->uFluxCapControl[34] = 0; ui14->uFluxCapControl[35] = 0; ui14->uFluxCapControl[36] = 0; ui14->uFluxCapControl[37] = 0; ui14->uFluxCapControl[38] = 0; ui14->uFluxCapControl[39] = 0; ui14->uFluxCapControl[40] = 0; ui14->uFluxCapControl[41] = 0; ui14->uFluxCapControl[42] = 0; ui14->uFluxCapControl[43] = 0; ui14->uFluxCapControl[44] = 0; ui14->uFluxCapControl[45] = 0; ui14->uFluxCapControl[46] = 0; ui14->uFluxCapControl[47] = 0; ui14->uFluxCapControl[48] = 0; ui14->uFluxCapControl[49] = 0; ui14->uFluxCapControl[50] = 0; ui14->uFluxCapControl[51] = 0; ui14->uFluxCapControl[52] = 0; ui14->uFluxCapControl[53] = 0; ui14->uFluxCapControl[54] = 0; ui14->uFluxCapControl[55] = 0; ui14->uFluxCapControl[56] = 0; ui14->uFluxCapControl[57] = 0; ui14->uFluxCapControl[58] = 0; ui14->uFluxCapControl[59] = 0; ui14->uFluxCapControl[60] = 0; ui14->uFluxCapControl[61] = 0; ui14->uFluxCapControl[62] = 0; ui14->uFluxCapControl[63] = 0; 
	ui14->fFluxCapData[0] = 0.000000; ui14->fFluxCapData[1] = 0.000000; ui14->fFluxCapData[2] = 0.000000; ui14->fFluxCapData[3] = 0.000000; ui14->fFluxCapData[4] = 0.000000; ui14->fFluxCapData[5] = 0.000000; ui14->fFluxCapData[6] = 0.000000; ui14->fFluxCapData[7] = 0.000000; ui14->fFluxCapData[8] = 0.000000; ui14->fFluxCapData[9] = 0.000000; ui14->fFluxCapData[10] = 0.000000; ui14->fFluxCapData[11] = 0.000000; ui14->fFluxCapData[12] = 0.000000; ui14->fFluxCapData[13] = 0.000000; ui14->fFluxCapData[14] = 0.000000; ui14->fFluxCapData[15] = 0.000000; ui14->fFluxCapData[16] = 0.000000; ui14->fFluxCapData[17] = 0.000000; ui14->fFluxCapData[18] = 0.000000; ui14->fFluxCapData[19] = 0.000000; ui14->fFluxCapData[20] = 0.000000; ui14->fFluxCapData[21] = 0.000000; ui14->fFluxCapData[22] = 0.000000; ui14->fFluxCapData[23] = 0.000000; ui14->fFluxCapData[24] = 0.000000; ui14->fFluxCapData[25] = 0.000000; ui14->fFluxCapData[26] = 0.000000; ui14->fFluxCapData[27] = 0.000000; ui14->fFluxCapData[28] = 0.000000; ui14->fFluxCapData[29] = 0.000000; ui14->fFluxCapData[30] = 0.000000; ui14->fFluxCapData[31] = 0.000000; ui14->fFluxCapData[32] = 0.000000; ui14->fFluxCapData[33] = 0.000000; ui14->fFluxCapData[34] = 0.000000; ui14->fFluxCapData[35] = 0.000000; ui14->fFluxCapData[36] = 0.000000; ui14->fFluxCapData[37] = 0.000000; ui14->fFluxCapData[38] = 0.000000; ui14->fFluxCapData[39] = 0.000000; ui14->fFluxCapData[40] = 0.000000; ui14->fFluxCapData[41] = 0.000000; ui14->fFluxCapData[42] = 0.000000; ui14->fFluxCapData[43] = 0.000000; ui14->fFluxCapData[44] = 0.000000; ui14->fFluxCapData[45] = 0.000000; ui14->fFluxCapData[46] = 0.000000; ui14->fFluxCapData[47] = 0.000000; ui14->fFluxCapData[48] = 0.000000; ui14->fFluxCapData[49] = 0.000000; ui14->fFluxCapData[50] = 0.000000; ui14->fFluxCapData[51] = 0.000000; ui14->fFluxCapData[52] = 0.000000; ui14->fFluxCapData[53] = 0.000000; ui14->fFluxCapData[54] = 0.000000; ui14->fFluxCapData[55] = 0.000000; ui14->fFluxCapData[56] = 0.000000; ui14->fFluxCapData[57] = 0.000000; ui14->fFluxCapData[58] = 0.000000; ui14->fFluxCapData[59] = 0.000000; ui14->fFluxCapData[60] = 0.000000; ui14->fFluxCapData[61] = 0.000000; ui14->fFluxCapData[62] = 0.000000; ui14->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui14);


	m_pwm_duty = 0.500000;
	CUICtrl* ui15 = new CUICtrl;
	ui15->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui15->uControlId = 31;
	ui15->bLogSlider = false;
	ui15->bExpSlider = false;
	ui15->fUserDisplayDataLoLimit = 0.000000;
	ui15->fUserDisplayDataHiLimit = 1.000000;
	ui15->uUserDataType = floatData;
	ui15->fInitUserIntValue = 0;
	ui15->fInitUserFloatValue = 0.500000;
	ui15->fInitUserDoubleValue = 0;
	ui15->fInitUserUINTValue = 0;
	ui15->m_pUserCookedIntData = NULL;
	ui15->m_pUserCookedFloatData = &m_pwm_duty;
	ui15->m_pUserCookedDoubleData = NULL;
	ui15->m_pUserCookedUINTData = NULL;
	ui15->cControlUnits = "";
	ui15->cVariableName = "m_pwm_duty";
	ui15->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui15->dPresetData[0] = 0.500000;ui15->dPresetData[1] = 0.000000;ui15->dPresetData[2] = 0.000000;ui15->dPresetData[3] = 0.000000;ui15->dPresetData[4] = 0.000000;ui15->dPresetData[5] = 0.000000;ui15->dPresetData[6] = 0.000000;ui15->dPresetData[7] = 0.000000;ui15->dPresetData[8] = 0.000000;ui15->dPresetData[9] = 0.000000;ui15->dPresetData[10] = 0.000000;ui15->dPresetData[11] = 0.000000;ui15->dPresetData[12] = 0.000000;ui15->dPresetData[13] = 0.000000;ui15->dPresetData[14] = 0.000000;ui15->dPresetData[15] = 0.000000;
	ui15->cControlName = "PWM duty";
	ui15->bOwnerControl = false;
	ui15->bMIDIControl = false;
	ui15->uMIDIControlCommand = 176;
	ui15->uMIDIControlName = 3;
	ui15->uMIDIControlChannel = 0;
	ui15->nGUIRow = nIndexer++;
	ui15->nGUIColumn = -1;
	ui15->bEnableParamSmoothing = false;
	ui15->fSmoothingTimeInMs = 100.00;
	ui15->uControlTheme[0] = 0; ui15->uControlTheme[1] = 0; ui15->uControlTheme[2] = 0; ui15->uControlTheme[3] = 0; ui15->uControlTheme[4] = 0; ui15->uControlTheme[5] = 0; ui15->uControlTheme[6] = 0; ui15->uControlTheme[7] = 0; ui15->uControlTheme[8] = 0; ui15->uControlTheme[9] = 0; ui15->uControlTheme[10] = 0; ui15->uControlTheme[11] = 0; ui15->uControlTheme[12] = 0; ui15->uControlTheme[13] = 0; ui15->uControlTheme[14] = 0; ui15->uControlTheme[15] = 0; ui15->uControlTheme[16] = 2; ui15->uControlTheme[17] = 0; ui15->uControlTheme[18] = 0; ui15->uControlTheme[19] = 0; ui15->uControlTheme[20] = 0; ui15->uControlTheme[21] = 0; ui15->uControlTheme[22] = 0; ui15->uControlTheme[23] = 0; ui15->uControlTheme[24] = 0; ui15->uControlTheme[25] = 0; ui15->uControlTheme[26] = 0; ui15->uControlTheme[27] = 0; ui15->uControlTheme[28] = 0; ui15->uControlTheme[29] = 0; ui15->uControlTheme[30] = 0; ui15->uControlTheme[31] = 0; 
	ui15->uFluxCapControl[0] = 0; ui15->uFluxCapControl[1] = 0; ui15->uFluxCapControl[2] = 0; ui15->uFluxCapControl[3] = 0; ui15->uFluxCapControl[4] = 0; ui15->uFluxCapControl[5] = 0; ui15->uFluxCapControl[6] = 0; ui15->uFluxCapControl[7] = 0; ui15->uFluxCapControl[8] = 0; ui15->uFluxCapControl[9] = 0; ui15->uFluxCapControl[10] = 0; ui15->uFluxCapControl[11] = 0; ui15->uFluxCapControl[12] = 0; ui15->uFluxCapControl[13] = 0; ui15->uFluxCapControl[14] = 0; ui15->uFluxCapControl[15] = 0; ui15->uFluxCapControl[16] = 0; ui15->uFluxCapControl[17] = 0; ui15->uFluxCapControl[18] = 0; ui15->uFluxCapControl[19] = 0; ui15->uFluxCapControl[20] = 0; ui15->uFluxCapControl[21] = 0; ui15->uFluxCapControl[22] = 0; ui15->uFluxCapControl[23] = 0; ui15->uFluxCapControl[24] = 0; ui15->uFluxCapControl[25] = 0; ui15->uFluxCapControl[26] = 0; ui15->uFluxCapControl[27] = 0; ui15->uFluxCapControl[28] = 0; ui15->uFluxCapControl[29] = 0; ui15->uFluxCapControl[30] = 0; ui15->uFluxCapControl[31] = 0; ui15->uFluxCapControl[32] = 0; ui15->uFluxCapControl[33] = 0; ui15->uFluxCapControl[34] = 0; ui15->uFluxCapControl[35] = 0; ui15->uFluxCapControl[36] = 0; ui15->uFluxCapControl[37] = 0; ui15->uFluxCapControl[38] = 0; ui15->uFluxCapControl[39] = 0; ui15->uFluxCapControl[40] = 0; ui15->uFluxCapControl[41] = 0; ui15->uFluxCapControl[42] = 0; ui15->uFluxCapControl[43] = 0; ui15->uFluxCapControl[44] = 0; ui15->uFluxCapControl[45] = 0; ui15->uFluxCapControl[46] = 0; ui15->uFluxCapControl[47] = 0; ui15->uFluxCapControl[48] = 0; ui15->uFluxCapControl[49] = 0; ui15->uFluxCapControl[50] = 0; ui15->uFluxCapControl[51] = 0; ui15->uFluxCapControl[52] = 0; ui15->uFluxCapControl[53] = 0; ui15->uFluxCapControl[54] = 0; ui15->uFluxCapControl[55] = 0; ui15->uFluxCapControl[56] = 0; ui15->uFluxCapControl[57] = 0; ui15->uFluxCapControl[58] = 0; ui15->uFluxCapControl[59] = 0; ui15->uFluxCapControl[60] = 0; ui15->uFluxCapControl[61] = 0; ui15->uFluxCapControl[62] = 0; ui15->uFluxCapControl[63] = 0; 
	ui15->fFluxCapData[0] = 0.000000; ui15->fFluxCapData[1] = 0.000000; ui15->fFluxCapData[2] = 0.000000; ui15->fFluxCapData[3] = 0.000000; ui15->fFluxCapData[4] = 0.000000; ui15->fFluxCapData[5] = 0.000000; ui15->fFluxCapData[6] = 0.000000; ui15->fFluxCapData[7] = 0.000000; ui15->fFluxCapData[8] = 0.000000; ui15->fFluxCapData[9] = 0.000000; ui15->fFluxCapData[10] = 0.000000; ui15->fFluxCapData[11] = 0.000000; ui15->fFluxCapData[12] = 0.000000; ui15->fFluxCapData[13] = 0.000000; ui15->fFluxCapData[14] = 0.000000; ui15->fFluxCapData[15] = 0.000000; ui15->fFluxCapData[16] = 0.000000; ui15->fFluxCapData[17] = 0.000000; ui15->fFluxCapData[18] = 0.000000; ui15->fFluxCapData[19] = 0.000000; ui15->fFluxCapData[20] = 0.000000; ui15->fFluxCapData[21] = 0.000000; ui15->fFluxCapData[22] = 0.000000; ui15->fFluxCapData[23] = 0.000000; ui15->fFluxCapData[24] = 0.000000; ui15->fFluxCapData[25] = 0.000000; ui15->fFluxCapData[26] = 0.000000; ui15->fFluxCapData[27] = 0.000000; ui15->fFluxCapData[28] = 0.000000; ui15->fFluxCapData[29] = 0.000000; ui15->fFluxCapData[30] = 0.000000; ui15->fFluxCapData[31] = 0.000000; ui15->fFluxCapData[32] = 0.000000; ui15->fFluxCapData[33] = 0.000000; ui15->fFluxCapData[34] = 0.000000; ui15->fFluxCapData[35] = 0.000000; ui15->fFluxCapData[36] = 0.000000; ui15->fFluxCapData[37] = 0.000000; ui15->fFluxCapData[38] = 0.000000; ui15->fFluxCapData[39] = 0.000000; ui15->fFluxCapData[40] = 0.000000; ui15->fFluxCapData[41] = 0.000000; ui15->fFluxCapData[42] = 0.000000; ui15->fFluxCapData[43] = 0.000000; ui15->fFluxCapData[44] = 0.000000; ui15->fFluxCapData[45] = 0.000000; ui15->fFluxCapData[46] = 0.000000; ui15->fFluxCapData[47] = 0.000000; ui15->fFluxCapData[48] = 0.000000; ui15->fFluxCapData[49] = 0.000000; ui15->fFluxCapData[50] = 0.000000; ui15->fFluxCapData[51] = 0.000000; ui15->fFluxCapData[52] = 0.000000; ui15->fFluxCapData[53] = 0.000000; ui15->fFluxCapData[54] = 0.000000; ui15->fFluxCapData[55] = 0.000000; ui15->fFluxCapData[56] = 0.000000; ui15->fFluxCapData[57] = 0.000000; ui15->fFluxCapData[58] = 0.000000; ui15->fFluxCapData[59] = 0.000000; ui15->fFluxCapData[60] = 0.000000; ui15->fFluxCapData[61] = 0.000000; ui15->fFluxCapData[62] = 0.000000; ui15->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui15);


	m_distortion_type = 0;
	CUICtrl* ui16 = new CUICtrl;
	ui16->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui16->uControlId = 36;
	ui16->bLogSlider = false;
	ui16->bExpSlider = false;
	ui16->fUserDisplayDataLoLimit = 0.000000;
	ui16->fUserDisplayDataHiLimit = 1.000000;
	ui16->uUserDataType = intData;
	ui16->fInitUserIntValue = 0.000000;
	ui16->fInitUserFloatValue = 0;
	ui16->fInitUserDoubleValue = 0;
	ui16->fInitUserUINTValue = 0;
	ui16->m_pUserCookedIntData = &m_distortion_type;
	ui16->m_pUserCookedFloatData = NULL;
	ui16->m_pUserCookedDoubleData = NULL;
	ui16->m_pUserCookedUINTData = NULL;
	ui16->cControlUnits = "";
	ui16->cVariableName = "m_distortion_type";
	ui16->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui16->dPresetData[0] = 0.000000;ui16->dPresetData[1] = 0.000000;ui16->dPresetData[2] = 0.000000;ui16->dPresetData[3] = 0.000000;ui16->dPresetData[4] = 0.000000;ui16->dPresetData[5] = 0.000000;ui16->dPresetData[6] = 0.000000;ui16->dPresetData[7] = 0.000000;ui16->dPresetData[8] = 0.000000;ui16->dPresetData[9] = 0.000000;ui16->dPresetData[10] = 0.000000;ui16->dPresetData[11] = 0.000000;ui16->dPresetData[12] = 0.000000;ui16->dPresetData[13] = 0.000000;ui16->dPresetData[14] = 0.000000;ui16->dPresetData[15] = 0.000000;
	ui16->cControlName = "Distortion Type";
	ui16->bOwnerControl = false;
	ui16->bMIDIControl = false;
	ui16->uMIDIControlCommand = 176;
	ui16->uMIDIControlName = 3;
	ui16->uMIDIControlChannel = 0;
	ui16->nGUIRow = nIndexer++;
	ui16->nGUIColumn = -1;
	ui16->bEnableParamSmoothing = false;
	ui16->fSmoothingTimeInMs = 100.00;
	ui16->uControlTheme[0] = 0; ui16->uControlTheme[1] = 0; ui16->uControlTheme[2] = 0; ui16->uControlTheme[3] = 0; ui16->uControlTheme[4] = 0; ui16->uControlTheme[5] = 0; ui16->uControlTheme[6] = 0; ui16->uControlTheme[7] = 0; ui16->uControlTheme[8] = 0; ui16->uControlTheme[9] = 0; ui16->uControlTheme[10] = 0; ui16->uControlTheme[11] = 0; ui16->uControlTheme[12] = 0; ui16->uControlTheme[13] = 0; ui16->uControlTheme[14] = 0; ui16->uControlTheme[15] = 0; ui16->uControlTheme[16] = 2; ui16->uControlTheme[17] = 0; ui16->uControlTheme[18] = 0; ui16->uControlTheme[19] = 0; ui16->uControlTheme[20] = 0; ui16->uControlTheme[21] = 0; ui16->uControlTheme[22] = 0; ui16->uControlTheme[23] = 0; ui16->uControlTheme[24] = 0; ui16->uControlTheme[25] = 0; ui16->uControlTheme[26] = 0; ui16->uControlTheme[27] = 0; ui16->uControlTheme[28] = 0; ui16->uControlTheme[29] = 0; ui16->uControlTheme[30] = 0; ui16->uControlTheme[31] = 0; 
	ui16->uFluxCapControl[0] = 0; ui16->uFluxCapControl[1] = 0; ui16->uFluxCapControl[2] = 0; ui16->uFluxCapControl[3] = 0; ui16->uFluxCapControl[4] = 0; ui16->uFluxCapControl[5] = 0; ui16->uFluxCapControl[6] = 0; ui16->uFluxCapControl[7] = 0; ui16->uFluxCapControl[8] = 0; ui16->uFluxCapControl[9] = 0; ui16->uFluxCapControl[10] = 0; ui16->uFluxCapControl[11] = 0; ui16->uFluxCapControl[12] = 0; ui16->uFluxCapControl[13] = 0; ui16->uFluxCapControl[14] = 0; ui16->uFluxCapControl[15] = 0; ui16->uFluxCapControl[16] = 0; ui16->uFluxCapControl[17] = 0; ui16->uFluxCapControl[18] = 0; ui16->uFluxCapControl[19] = 0; ui16->uFluxCapControl[20] = 0; ui16->uFluxCapControl[21] = 0; ui16->uFluxCapControl[22] = 0; ui16->uFluxCapControl[23] = 0; ui16->uFluxCapControl[24] = 0; ui16->uFluxCapControl[25] = 0; ui16->uFluxCapControl[26] = 0; ui16->uFluxCapControl[27] = 0; ui16->uFluxCapControl[28] = 0; ui16->uFluxCapControl[29] = 0; ui16->uFluxCapControl[30] = 0; ui16->uFluxCapControl[31] = 0; ui16->uFluxCapControl[32] = 0; ui16->uFluxCapControl[33] = 0; ui16->uFluxCapControl[34] = 0; ui16->uFluxCapControl[35] = 0; ui16->uFluxCapControl[36] = 0; ui16->uFluxCapControl[37] = 0; ui16->uFluxCapControl[38] = 0; ui16->uFluxCapControl[39] = 0; ui16->uFluxCapControl[40] = 0; ui16->uFluxCapControl[41] = 0; ui16->uFluxCapControl[42] = 0; ui16->uFluxCapControl[43] = 0; ui16->uFluxCapControl[44] = 0; ui16->uFluxCapControl[45] = 0; ui16->uFluxCapControl[46] = 0; ui16->uFluxCapControl[47] = 0; ui16->uFluxCapControl[48] = 0; ui16->uFluxCapControl[49] = 0; ui16->uFluxCapControl[50] = 0; ui16->uFluxCapControl[51] = 0; ui16->uFluxCapControl[52] = 0; ui16->uFluxCapControl[53] = 0; ui16->uFluxCapControl[54] = 0; ui16->uFluxCapControl[55] = 0; ui16->uFluxCapControl[56] = 0; ui16->uFluxCapControl[57] = 0; ui16->uFluxCapControl[58] = 0; ui16->uFluxCapControl[59] = 0; ui16->uFluxCapControl[60] = 0; ui16->uFluxCapControl[61] = 0; ui16->uFluxCapControl[62] = 0; ui16->uFluxCapControl[63] = 0; 
	ui16->fFluxCapData[0] = 0.000000; ui16->fFluxCapData[1] = 0.000000; ui16->fFluxCapData[2] = 0.000000; ui16->fFluxCapData[3] = 0.000000; ui16->fFluxCapData[4] = 0.000000; ui16->fFluxCapData[5] = 0.000000; ui16->fFluxCapData[6] = 0.000000; ui16->fFluxCapData[7] = 0.000000; ui16->fFluxCapData[8] = 0.000000; ui16->fFluxCapData[9] = 0.000000; ui16->fFluxCapData[10] = 0.000000; ui16->fFluxCapData[11] = 0.000000; ui16->fFluxCapData[12] = 0.000000; ui16->fFluxCapData[13] = 0.000000; ui16->fFluxCapData[14] = 0.000000; ui16->fFluxCapData[15] = 0.000000; ui16->fFluxCapData[16] = 0.000000; ui16->fFluxCapData[17] = 0.000000; ui16->fFluxCapData[18] = 0.000000; ui16->fFluxCapData[19] = 0.000000; ui16->fFluxCapData[20] = 0.000000; ui16->fFluxCapData[21] = 0.000000; ui16->fFluxCapData[22] = 0.000000; ui16->fFluxCapData[23] = 0.000000; ui16->fFluxCapData[24] = 0.000000; ui16->fFluxCapData[25] = 0.000000; ui16->fFluxCapData[26] = 0.000000; ui16->fFluxCapData[27] = 0.000000; ui16->fFluxCapData[28] = 0.000000; ui16->fFluxCapData[29] = 0.000000; ui16->fFluxCapData[30] = 0.000000; ui16->fFluxCapData[31] = 0.000000; ui16->fFluxCapData[32] = 0.000000; ui16->fFluxCapData[33] = 0.000000; ui16->fFluxCapData[34] = 0.000000; ui16->fFluxCapData[35] = 0.000000; ui16->fFluxCapData[36] = 0.000000; ui16->fFluxCapData[37] = 0.000000; ui16->fFluxCapData[38] = 0.000000; ui16->fFluxCapData[39] = 0.000000; ui16->fFluxCapData[40] = 0.000000; ui16->fFluxCapData[41] = 0.000000; ui16->fFluxCapData[42] = 0.000000; ui16->fFluxCapData[43] = 0.000000; ui16->fFluxCapData[44] = 0.000000; ui16->fFluxCapData[45] = 0.000000; ui16->fFluxCapData[46] = 0.000000; ui16->fFluxCapData[47] = 0.000000; ui16->fFluxCapData[48] = 0.000000; ui16->fFluxCapData[49] = 0.000000; ui16->fFluxCapData[50] = 0.000000; ui16->fFluxCapData[51] = 0.000000; ui16->fFluxCapData[52] = 0.000000; ui16->fFluxCapData[53] = 0.000000; ui16->fFluxCapData[54] = 0.000000; ui16->fFluxCapData[55] = 0.000000; ui16->fFluxCapData[56] = 0.000000; ui16->fFluxCapData[57] = 0.000000; ui16->fFluxCapData[58] = 0.000000; ui16->fFluxCapData[59] = 0.000000; ui16->fFluxCapData[60] = 0.000000; ui16->fFluxCapData[61] = 0.000000; ui16->fFluxCapData[62] = 0.000000; ui16->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui16);


	m_bitcrush_dry_wet = 1.000000;
	CUICtrl* ui17 = new CUICtrl;
	ui17->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui17->uControlId = 37;
	ui17->bLogSlider = false;
	ui17->bExpSlider = false;
	ui17->fUserDisplayDataLoLimit = 0.000000;
	ui17->fUserDisplayDataHiLimit = 1.000000;
	ui17->uUserDataType = floatData;
	ui17->fInitUserIntValue = 0;
	ui17->fInitUserFloatValue = 1.000000;
	ui17->fInitUserDoubleValue = 0;
	ui17->fInitUserUINTValue = 0;
	ui17->m_pUserCookedIntData = NULL;
	ui17->m_pUserCookedFloatData = &m_bitcrush_dry_wet;
	ui17->m_pUserCookedDoubleData = NULL;
	ui17->m_pUserCookedUINTData = NULL;
	ui17->cControlUnits = "";
	ui17->cVariableName = "m_bitcrush_dry_wet";
	ui17->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui17->dPresetData[0] = 1.000000;ui17->dPresetData[1] = 0.000000;ui17->dPresetData[2] = 0.000000;ui17->dPresetData[3] = 0.000000;ui17->dPresetData[4] = 0.000000;ui17->dPresetData[5] = 0.000000;ui17->dPresetData[6] = 0.000000;ui17->dPresetData[7] = 0.000000;ui17->dPresetData[8] = 0.000000;ui17->dPresetData[9] = 0.000000;ui17->dPresetData[10] = 0.000000;ui17->dPresetData[11] = 0.000000;ui17->dPresetData[12] = 0.000000;ui17->dPresetData[13] = 0.000000;ui17->dPresetData[14] = 0.000000;ui17->dPresetData[15] = 0.000000;
	ui17->cControlName = "bitcrush dry_wet";
	ui17->bOwnerControl = false;
	ui17->bMIDIControl = false;
	ui17->uMIDIControlCommand = 176;
	ui17->uMIDIControlName = 3;
	ui17->uMIDIControlChannel = 0;
	ui17->nGUIRow = nIndexer++;
	ui17->nGUIColumn = -1;
	ui17->bEnableParamSmoothing = false;
	ui17->fSmoothingTimeInMs = 100.00;
	ui17->uControlTheme[0] = 0; ui17->uControlTheme[1] = 0; ui17->uControlTheme[2] = 0; ui17->uControlTheme[3] = 0; ui17->uControlTheme[4] = 0; ui17->uControlTheme[5] = 0; ui17->uControlTheme[6] = 0; ui17->uControlTheme[7] = 0; ui17->uControlTheme[8] = 0; ui17->uControlTheme[9] = 0; ui17->uControlTheme[10] = 0; ui17->uControlTheme[11] = 0; ui17->uControlTheme[12] = 0; ui17->uControlTheme[13] = 0; ui17->uControlTheme[14] = 0; ui17->uControlTheme[15] = 0; ui17->uControlTheme[16] = 2; ui17->uControlTheme[17] = 0; ui17->uControlTheme[18] = 0; ui17->uControlTheme[19] = 0; ui17->uControlTheme[20] = 0; ui17->uControlTheme[21] = 0; ui17->uControlTheme[22] = 0; ui17->uControlTheme[23] = 0; ui17->uControlTheme[24] = 0; ui17->uControlTheme[25] = 0; ui17->uControlTheme[26] = 0; ui17->uControlTheme[27] = 0; ui17->uControlTheme[28] = 0; ui17->uControlTheme[29] = 0; ui17->uControlTheme[30] = 0; ui17->uControlTheme[31] = 0; 
	ui17->uFluxCapControl[0] = 0; ui17->uFluxCapControl[1] = 0; ui17->uFluxCapControl[2] = 0; ui17->uFluxCapControl[3] = 0; ui17->uFluxCapControl[4] = 0; ui17->uFluxCapControl[5] = 0; ui17->uFluxCapControl[6] = 0; ui17->uFluxCapControl[7] = 0; ui17->uFluxCapControl[8] = 0; ui17->uFluxCapControl[9] = 0; ui17->uFluxCapControl[10] = 0; ui17->uFluxCapControl[11] = 0; ui17->uFluxCapControl[12] = 0; ui17->uFluxCapControl[13] = 0; ui17->uFluxCapControl[14] = 0; ui17->uFluxCapControl[15] = 0; ui17->uFluxCapControl[16] = 0; ui17->uFluxCapControl[17] = 0; ui17->uFluxCapControl[18] = 0; ui17->uFluxCapControl[19] = 0; ui17->uFluxCapControl[20] = 0; ui17->uFluxCapControl[21] = 0; ui17->uFluxCapControl[22] = 0; ui17->uFluxCapControl[23] = 0; ui17->uFluxCapControl[24] = 0; ui17->uFluxCapControl[25] = 0; ui17->uFluxCapControl[26] = 0; ui17->uFluxCapControl[27] = 0; ui17->uFluxCapControl[28] = 0; ui17->uFluxCapControl[29] = 0; ui17->uFluxCapControl[30] = 0; ui17->uFluxCapControl[31] = 0; ui17->uFluxCapControl[32] = 0; ui17->uFluxCapControl[33] = 0; ui17->uFluxCapControl[34] = 0; ui17->uFluxCapControl[35] = 0; ui17->uFluxCapControl[36] = 0; ui17->uFluxCapControl[37] = 0; ui17->uFluxCapControl[38] = 0; ui17->uFluxCapControl[39] = 0; ui17->uFluxCapControl[40] = 0; ui17->uFluxCapControl[41] = 0; ui17->uFluxCapControl[42] = 0; ui17->uFluxCapControl[43] = 0; ui17->uFluxCapControl[44] = 0; ui17->uFluxCapControl[45] = 0; ui17->uFluxCapControl[46] = 0; ui17->uFluxCapControl[47] = 0; ui17->uFluxCapControl[48] = 0; ui17->uFluxCapControl[49] = 0; ui17->uFluxCapControl[50] = 0; ui17->uFluxCapControl[51] = 0; ui17->uFluxCapControl[52] = 0; ui17->uFluxCapControl[53] = 0; ui17->uFluxCapControl[54] = 0; ui17->uFluxCapControl[55] = 0; ui17->uFluxCapControl[56] = 0; ui17->uFluxCapControl[57] = 0; ui17->uFluxCapControl[58] = 0; ui17->uFluxCapControl[59] = 0; ui17->uFluxCapControl[60] = 0; ui17->uFluxCapControl[61] = 0; ui17->uFluxCapControl[62] = 0; ui17->uFluxCapControl[63] = 0; 
	ui17->fFluxCapData[0] = 0.000000; ui17->fFluxCapData[1] = 0.000000; ui17->fFluxCapData[2] = 0.000000; ui17->fFluxCapData[3] = 0.000000; ui17->fFluxCapData[4] = 0.000000; ui17->fFluxCapData[5] = 0.000000; ui17->fFluxCapData[6] = 0.000000; ui17->fFluxCapData[7] = 0.000000; ui17->fFluxCapData[8] = 0.000000; ui17->fFluxCapData[9] = 0.000000; ui17->fFluxCapData[10] = 0.000000; ui17->fFluxCapData[11] = 0.000000; ui17->fFluxCapData[12] = 0.000000; ui17->fFluxCapData[13] = 0.000000; ui17->fFluxCapData[14] = 0.000000; ui17->fFluxCapData[15] = 0.000000; ui17->fFluxCapData[16] = 0.000000; ui17->fFluxCapData[17] = 0.000000; ui17->fFluxCapData[18] = 0.000000; ui17->fFluxCapData[19] = 0.000000; ui17->fFluxCapData[20] = 0.000000; ui17->fFluxCapData[21] = 0.000000; ui17->fFluxCapData[22] = 0.000000; ui17->fFluxCapData[23] = 0.000000; ui17->fFluxCapData[24] = 0.000000; ui17->fFluxCapData[25] = 0.000000; ui17->fFluxCapData[26] = 0.000000; ui17->fFluxCapData[27] = 0.000000; ui17->fFluxCapData[28] = 0.000000; ui17->fFluxCapData[29] = 0.000000; ui17->fFluxCapData[30] = 0.000000; ui17->fFluxCapData[31] = 0.000000; ui17->fFluxCapData[32] = 0.000000; ui17->fFluxCapData[33] = 0.000000; ui17->fFluxCapData[34] = 0.000000; ui17->fFluxCapData[35] = 0.000000; ui17->fFluxCapData[36] = 0.000000; ui17->fFluxCapData[37] = 0.000000; ui17->fFluxCapData[38] = 0.000000; ui17->fFluxCapData[39] = 0.000000; ui17->fFluxCapData[40] = 0.000000; ui17->fFluxCapData[41] = 0.000000; ui17->fFluxCapData[42] = 0.000000; ui17->fFluxCapData[43] = 0.000000; ui17->fFluxCapData[44] = 0.000000; ui17->fFluxCapData[45] = 0.000000; ui17->fFluxCapData[46] = 0.000000; ui17->fFluxCapData[47] = 0.000000; ui17->fFluxCapData[48] = 0.000000; ui17->fFluxCapData[49] = 0.000000; ui17->fFluxCapData[50] = 0.000000; ui17->fFluxCapData[51] = 0.000000; ui17->fFluxCapData[52] = 0.000000; ui17->fFluxCapData[53] = 0.000000; ui17->fFluxCapData[54] = 0.000000; ui17->fFluxCapData[55] = 0.000000; ui17->fFluxCapData[56] = 0.000000; ui17->fFluxCapData[57] = 0.000000; ui17->fFluxCapData[58] = 0.000000; ui17->fFluxCapData[59] = 0.000000; ui17->fFluxCapData[60] = 0.000000; ui17->fFluxCapData[61] = 0.000000; ui17->fFluxCapData[62] = 0.000000; ui17->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui17);


	m_bitcrush_bits = 4;
	CUICtrl* ui18 = new CUICtrl;
	ui18->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui18->uControlId = 38;
	ui18->bLogSlider = false;
	ui18->bExpSlider = false;
	ui18->fUserDisplayDataLoLimit = 1.000000;
	ui18->fUserDisplayDataHiLimit = 12.000000;
	ui18->uUserDataType = intData;
	ui18->fInitUserIntValue = 4.000000;
	ui18->fInitUserFloatValue = 0;
	ui18->fInitUserDoubleValue = 0;
	ui18->fInitUserUINTValue = 0;
	ui18->m_pUserCookedIntData = &m_bitcrush_bits;
	ui18->m_pUserCookedFloatData = NULL;
	ui18->m_pUserCookedDoubleData = NULL;
	ui18->m_pUserCookedUINTData = NULL;
	ui18->cControlUnits = "";
	ui18->cVariableName = "m_bitcrush_bits";
	ui18->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui18->dPresetData[0] = 4.000000;ui18->dPresetData[1] = 0.000000;ui18->dPresetData[2] = 0.000000;ui18->dPresetData[3] = 0.000000;ui18->dPresetData[4] = 0.000000;ui18->dPresetData[5] = 0.000000;ui18->dPresetData[6] = 0.000000;ui18->dPresetData[7] = 0.000000;ui18->dPresetData[8] = 0.000000;ui18->dPresetData[9] = 0.000000;ui18->dPresetData[10] = 0.000000;ui18->dPresetData[11] = 0.000000;ui18->dPresetData[12] = 0.000000;ui18->dPresetData[13] = 0.000000;ui18->dPresetData[14] = 0.000000;ui18->dPresetData[15] = 0.000000;
	ui18->cControlName = "bitcrush_bits";
	ui18->bOwnerControl = false;
	ui18->bMIDIControl = false;
	ui18->uMIDIControlCommand = 176;
	ui18->uMIDIControlName = 3;
	ui18->uMIDIControlChannel = 0;
	ui18->nGUIRow = nIndexer++;
	ui18->nGUIColumn = -1;
	ui18->bEnableParamSmoothing = false;
	ui18->fSmoothingTimeInMs = 100.00;
	ui18->uControlTheme[0] = 0; ui18->uControlTheme[1] = 0; ui18->uControlTheme[2] = 0; ui18->uControlTheme[3] = 0; ui18->uControlTheme[4] = 0; ui18->uControlTheme[5] = 0; ui18->uControlTheme[6] = 0; ui18->uControlTheme[7] = 0; ui18->uControlTheme[8] = 0; ui18->uControlTheme[9] = 0; ui18->uControlTheme[10] = 0; ui18->uControlTheme[11] = 0; ui18->uControlTheme[12] = 0; ui18->uControlTheme[13] = 0; ui18->uControlTheme[14] = 0; ui18->uControlTheme[15] = 0; ui18->uControlTheme[16] = 2; ui18->uControlTheme[17] = 0; ui18->uControlTheme[18] = 0; ui18->uControlTheme[19] = 0; ui18->uControlTheme[20] = 0; ui18->uControlTheme[21] = 0; ui18->uControlTheme[22] = 0; ui18->uControlTheme[23] = 0; ui18->uControlTheme[24] = 0; ui18->uControlTheme[25] = 0; ui18->uControlTheme[26] = 0; ui18->uControlTheme[27] = 0; ui18->uControlTheme[28] = 0; ui18->uControlTheme[29] = 0; ui18->uControlTheme[30] = 0; ui18->uControlTheme[31] = 0; 
	ui18->uFluxCapControl[0] = 0; ui18->uFluxCapControl[1] = 0; ui18->uFluxCapControl[2] = 0; ui18->uFluxCapControl[3] = 0; ui18->uFluxCapControl[4] = 0; ui18->uFluxCapControl[5] = 0; ui18->uFluxCapControl[6] = 0; ui18->uFluxCapControl[7] = 0; ui18->uFluxCapControl[8] = 0; ui18->uFluxCapControl[9] = 0; ui18->uFluxCapControl[10] = 0; ui18->uFluxCapControl[11] = 0; ui18->uFluxCapControl[12] = 0; ui18->uFluxCapControl[13] = 0; ui18->uFluxCapControl[14] = 0; ui18->uFluxCapControl[15] = 0; ui18->uFluxCapControl[16] = 0; ui18->uFluxCapControl[17] = 0; ui18->uFluxCapControl[18] = 0; ui18->uFluxCapControl[19] = 0; ui18->uFluxCapControl[20] = 0; ui18->uFluxCapControl[21] = 0; ui18->uFluxCapControl[22] = 0; ui18->uFluxCapControl[23] = 0; ui18->uFluxCapControl[24] = 0; ui18->uFluxCapControl[25] = 0; ui18->uFluxCapControl[26] = 0; ui18->uFluxCapControl[27] = 0; ui18->uFluxCapControl[28] = 0; ui18->uFluxCapControl[29] = 0; ui18->uFluxCapControl[30] = 0; ui18->uFluxCapControl[31] = 0; ui18->uFluxCapControl[32] = 0; ui18->uFluxCapControl[33] = 0; ui18->uFluxCapControl[34] = 0; ui18->uFluxCapControl[35] = 0; ui18->uFluxCapControl[36] = 0; ui18->uFluxCapControl[37] = 0; ui18->uFluxCapControl[38] = 0; ui18->uFluxCapControl[39] = 0; ui18->uFluxCapControl[40] = 0; ui18->uFluxCapControl[41] = 0; ui18->uFluxCapControl[42] = 0; ui18->uFluxCapControl[43] = 0; ui18->uFluxCapControl[44] = 0; ui18->uFluxCapControl[45] = 0; ui18->uFluxCapControl[46] = 0; ui18->uFluxCapControl[47] = 0; ui18->uFluxCapControl[48] = 0; ui18->uFluxCapControl[49] = 0; ui18->uFluxCapControl[50] = 0; ui18->uFluxCapControl[51] = 0; ui18->uFluxCapControl[52] = 0; ui18->uFluxCapControl[53] = 0; ui18->uFluxCapControl[54] = 0; ui18->uFluxCapControl[55] = 0; ui18->uFluxCapControl[56] = 0; ui18->uFluxCapControl[57] = 0; ui18->uFluxCapControl[58] = 0; ui18->uFluxCapControl[59] = 0; ui18->uFluxCapControl[60] = 0; ui18->uFluxCapControl[61] = 0; ui18->uFluxCapControl[62] = 0; ui18->uFluxCapControl[63] = 0; 
	ui18->fFluxCapData[0] = 0.000000; ui18->fFluxCapData[1] = 0.000000; ui18->fFluxCapData[2] = 0.000000; ui18->fFluxCapData[3] = 0.000000; ui18->fFluxCapData[4] = 0.000000; ui18->fFluxCapData[5] = 0.000000; ui18->fFluxCapData[6] = 0.000000; ui18->fFluxCapData[7] = 0.000000; ui18->fFluxCapData[8] = 0.000000; ui18->fFluxCapData[9] = 0.000000; ui18->fFluxCapData[10] = 0.000000; ui18->fFluxCapData[11] = 0.000000; ui18->fFluxCapData[12] = 0.000000; ui18->fFluxCapData[13] = 0.000000; ui18->fFluxCapData[14] = 0.000000; ui18->fFluxCapData[15] = 0.000000; ui18->fFluxCapData[16] = 0.000000; ui18->fFluxCapData[17] = 0.000000; ui18->fFluxCapData[18] = 0.000000; ui18->fFluxCapData[19] = 0.000000; ui18->fFluxCapData[20] = 0.000000; ui18->fFluxCapData[21] = 0.000000; ui18->fFluxCapData[22] = 0.000000; ui18->fFluxCapData[23] = 0.000000; ui18->fFluxCapData[24] = 0.000000; ui18->fFluxCapData[25] = 0.000000; ui18->fFluxCapData[26] = 0.000000; ui18->fFluxCapData[27] = 0.000000; ui18->fFluxCapData[28] = 0.000000; ui18->fFluxCapData[29] = 0.000000; ui18->fFluxCapData[30] = 0.000000; ui18->fFluxCapData[31] = 0.000000; ui18->fFluxCapData[32] = 0.000000; ui18->fFluxCapData[33] = 0.000000; ui18->fFluxCapData[34] = 0.000000; ui18->fFluxCapData[35] = 0.000000; ui18->fFluxCapData[36] = 0.000000; ui18->fFluxCapData[37] = 0.000000; ui18->fFluxCapData[38] = 0.000000; ui18->fFluxCapData[39] = 0.000000; ui18->fFluxCapData[40] = 0.000000; ui18->fFluxCapData[41] = 0.000000; ui18->fFluxCapData[42] = 0.000000; ui18->fFluxCapData[43] = 0.000000; ui18->fFluxCapData[44] = 0.000000; ui18->fFluxCapData[45] = 0.000000; ui18->fFluxCapData[46] = 0.000000; ui18->fFluxCapData[47] = 0.000000; ui18->fFluxCapData[48] = 0.000000; ui18->fFluxCapData[49] = 0.000000; ui18->fFluxCapData[50] = 0.000000; ui18->fFluxCapData[51] = 0.000000; ui18->fFluxCapData[52] = 0.000000; ui18->fFluxCapData[53] = 0.000000; ui18->fFluxCapData[54] = 0.000000; ui18->fFluxCapData[55] = 0.000000; ui18->fFluxCapData[56] = 0.000000; ui18->fFluxCapData[57] = 0.000000; ui18->fFluxCapData[58] = 0.000000; ui18->fFluxCapData[59] = 0.000000; ui18->fFluxCapData[60] = 0.000000; ui18->fFluxCapData[61] = 0.000000; ui18->fFluxCapData[62] = 0.000000; ui18->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui18);


	m_bitcrush_edge = 0.800000;
	CUICtrl* ui19 = new CUICtrl;
	ui19->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui19->uControlId = 39;
	ui19->bLogSlider = false;
	ui19->bExpSlider = false;
	ui19->fUserDisplayDataLoLimit = 0.050000;
	ui19->fUserDisplayDataHiLimit = 1.000000;
	ui19->uUserDataType = floatData;
	ui19->fInitUserIntValue = 0;
	ui19->fInitUserFloatValue = 0.800000;
	ui19->fInitUserDoubleValue = 0;
	ui19->fInitUserUINTValue = 0;
	ui19->m_pUserCookedIntData = NULL;
	ui19->m_pUserCookedFloatData = &m_bitcrush_edge;
	ui19->m_pUserCookedDoubleData = NULL;
	ui19->m_pUserCookedUINTData = NULL;
	ui19->cControlUnits = "";
	ui19->cVariableName = "m_bitcrush_edge";
	ui19->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui19->dPresetData[0] = 0.800000;ui19->dPresetData[1] = 0.000000;ui19->dPresetData[2] = 0.000000;ui19->dPresetData[3] = 0.000000;ui19->dPresetData[4] = 0.000000;ui19->dPresetData[5] = 0.000000;ui19->dPresetData[6] = 0.000000;ui19->dPresetData[7] = 0.000000;ui19->dPresetData[8] = 0.000000;ui19->dPresetData[9] = 0.000000;ui19->dPresetData[10] = 0.000000;ui19->dPresetData[11] = 0.000000;ui19->dPresetData[12] = 0.000000;ui19->dPresetData[13] = 0.000000;ui19->dPresetData[14] = 0.000000;ui19->dPresetData[15] = 0.000000;
	ui19->cControlName = "bitcrush_edge";
	ui19->bOwnerControl = false;
	ui19->bMIDIControl = false;
	ui19->uMIDIControlCommand = 176;
	ui19->uMIDIControlName = 3;
	ui19->uMIDIControlChannel = 0;
	ui19->nGUIRow = nIndexer++;
	ui19->nGUIColumn = -1;
	ui19->bEnableParamSmoothing = false;
	ui19->fSmoothingTimeInMs = 100.00;
	ui19->uControlTheme[0] = 0; ui19->uControlTheme[1] = 0; ui19->uControlTheme[2] = 0; ui19->uControlTheme[3] = 0; ui19->uControlTheme[4] = 0; ui19->uControlTheme[5] = 0; ui19->uControlTheme[6] = 0; ui19->uControlTheme[7] = 0; ui19->uControlTheme[8] = 0; ui19->uControlTheme[9] = 0; ui19->uControlTheme[10] = 0; ui19->uControlTheme[11] = 0; ui19->uControlTheme[12] = 0; ui19->uControlTheme[13] = 0; ui19->uControlTheme[14] = 0; ui19->uControlTheme[15] = 0; ui19->uControlTheme[16] = 2; ui19->uControlTheme[17] = 0; ui19->uControlTheme[18] = 0; ui19->uControlTheme[19] = 0; ui19->uControlTheme[20] = 0; ui19->uControlTheme[21] = 0; ui19->uControlTheme[22] = 0; ui19->uControlTheme[23] = 0; ui19->uControlTheme[24] = 0; ui19->uControlTheme[25] = 0; ui19->uControlTheme[26] = 0; ui19->uControlTheme[27] = 0; ui19->uControlTheme[28] = 0; ui19->uControlTheme[29] = 0; ui19->uControlTheme[30] = 0; ui19->uControlTheme[31] = 0; 
	ui19->uFluxCapControl[0] = 0; ui19->uFluxCapControl[1] = 0; ui19->uFluxCapControl[2] = 0; ui19->uFluxCapControl[3] = 0; ui19->uFluxCapControl[4] = 0; ui19->uFluxCapControl[5] = 0; ui19->uFluxCapControl[6] = 0; ui19->uFluxCapControl[7] = 0; ui19->uFluxCapControl[8] = 0; ui19->uFluxCapControl[9] = 0; ui19->uFluxCapControl[10] = 0; ui19->uFluxCapControl[11] = 0; ui19->uFluxCapControl[12] = 0; ui19->uFluxCapControl[13] = 0; ui19->uFluxCapControl[14] = 0; ui19->uFluxCapControl[15] = 0; ui19->uFluxCapControl[16] = 0; ui19->uFluxCapControl[17] = 0; ui19->uFluxCapControl[18] = 0; ui19->uFluxCapControl[19] = 0; ui19->uFluxCapControl[20] = 0; ui19->uFluxCapControl[21] = 0; ui19->uFluxCapControl[22] = 0; ui19->uFluxCapControl[23] = 0; ui19->uFluxCapControl[24] = 0; ui19->uFluxCapControl[25] = 0; ui19->uFluxCapControl[26] = 0; ui19->uFluxCapControl[27] = 0; ui19->uFluxCapControl[28] = 0; ui19->uFluxCapControl[29] = 0; ui19->uFluxCapControl[30] = 0; ui19->uFluxCapControl[31] = 0; ui19->uFluxCapControl[32] = 0; ui19->uFluxCapControl[33] = 0; ui19->uFluxCapControl[34] = 0; ui19->uFluxCapControl[35] = 0; ui19->uFluxCapControl[36] = 0; ui19->uFluxCapControl[37] = 0; ui19->uFluxCapControl[38] = 0; ui19->uFluxCapControl[39] = 0; ui19->uFluxCapControl[40] = 0; ui19->uFluxCapControl[41] = 0; ui19->uFluxCapControl[42] = 0; ui19->uFluxCapControl[43] = 0; ui19->uFluxCapControl[44] = 0; ui19->uFluxCapControl[45] = 0; ui19->uFluxCapControl[46] = 0; ui19->uFluxCapControl[47] = 0; ui19->uFluxCapControl[48] = 0; ui19->uFluxCapControl[49] = 0; ui19->uFluxCapControl[50] = 0; ui19->uFluxCapControl[51] = 0; ui19->uFluxCapControl[52] = 0; ui19->uFluxCapControl[53] = 0; ui19->uFluxCapControl[54] = 0; ui19->uFluxCapControl[55] = 0; ui19->uFluxCapControl[56] = 0; ui19->uFluxCapControl[57] = 0; ui19->uFluxCapControl[58] = 0; ui19->uFluxCapControl[59] = 0; ui19->uFluxCapControl[60] = 0; ui19->uFluxCapControl[61] = 0; ui19->uFluxCapControl[62] = 0; ui19->uFluxCapControl[63] = 0; 
	ui19->fFluxCapData[0] = 0.000000; ui19->fFluxCapData[1] = 0.000000; ui19->fFluxCapData[2] = 0.000000; ui19->fFluxCapData[3] = 0.000000; ui19->fFluxCapData[4] = 0.000000; ui19->fFluxCapData[5] = 0.000000; ui19->fFluxCapData[6] = 0.000000; ui19->fFluxCapData[7] = 0.000000; ui19->fFluxCapData[8] = 0.000000; ui19->fFluxCapData[9] = 0.000000; ui19->fFluxCapData[10] = 0.000000; ui19->fFluxCapData[11] = 0.000000; ui19->fFluxCapData[12] = 0.000000; ui19->fFluxCapData[13] = 0.000000; ui19->fFluxCapData[14] = 0.000000; ui19->fFluxCapData[15] = 0.000000; ui19->fFluxCapData[16] = 0.000000; ui19->fFluxCapData[17] = 0.000000; ui19->fFluxCapData[18] = 0.000000; ui19->fFluxCapData[19] = 0.000000; ui19->fFluxCapData[20] = 0.000000; ui19->fFluxCapData[21] = 0.000000; ui19->fFluxCapData[22] = 0.000000; ui19->fFluxCapData[23] = 0.000000; ui19->fFluxCapData[24] = 0.000000; ui19->fFluxCapData[25] = 0.000000; ui19->fFluxCapData[26] = 0.000000; ui19->fFluxCapData[27] = 0.000000; ui19->fFluxCapData[28] = 0.000000; ui19->fFluxCapData[29] = 0.000000; ui19->fFluxCapData[30] = 0.000000; ui19->fFluxCapData[31] = 0.000000; ui19->fFluxCapData[32] = 0.000000; ui19->fFluxCapData[33] = 0.000000; ui19->fFluxCapData[34] = 0.000000; ui19->fFluxCapData[35] = 0.000000; ui19->fFluxCapData[36] = 0.000000; ui19->fFluxCapData[37] = 0.000000; ui19->fFluxCapData[38] = 0.000000; ui19->fFluxCapData[39] = 0.000000; ui19->fFluxCapData[40] = 0.000000; ui19->fFluxCapData[41] = 0.000000; ui19->fFluxCapData[42] = 0.000000; ui19->fFluxCapData[43] = 0.000000; ui19->fFluxCapData[44] = 0.000000; ui19->fFluxCapData[45] = 0.000000; ui19->fFluxCapData[46] = 0.000000; ui19->fFluxCapData[47] = 0.000000; ui19->fFluxCapData[48] = 0.000000; ui19->fFluxCapData[49] = 0.000000; ui19->fFluxCapData[50] = 0.000000; ui19->fFluxCapData[51] = 0.000000; ui19->fFluxCapData[52] = 0.000000; ui19->fFluxCapData[53] = 0.000000; ui19->fFluxCapData[54] = 0.000000; ui19->fFluxCapData[55] = 0.000000; ui19->fFluxCapData[56] = 0.000000; ui19->fFluxCapData[57] = 0.000000; ui19->fFluxCapData[58] = 0.000000; ui19->fFluxCapData[59] = 0.000000; ui19->fFluxCapData[60] = 0.000000; ui19->fFluxCapData[61] = 0.000000; ui19->fFluxCapData[62] = 0.000000; ui19->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui19);


	m_2D_position = 0.000000;
	CUICtrl* ui20 = new CUICtrl;
	ui20->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui20->uControlId = 100;
	ui20->bLogSlider = false;
	ui20->bExpSlider = false;
	ui20->fUserDisplayDataLoLimit = 0.000000;
	ui20->fUserDisplayDataHiLimit = 1.000000;
	ui20->uUserDataType = floatData;
	ui20->fInitUserIntValue = 0;
	ui20->fInitUserFloatValue = 0.000000;
	ui20->fInitUserDoubleValue = 0;
	ui20->fInitUserUINTValue = 0;
	ui20->m_pUserCookedIntData = NULL;
	ui20->m_pUserCookedFloatData = &m_2D_position;
	ui20->m_pUserCookedDoubleData = NULL;
	ui20->m_pUserCookedUINTData = NULL;
	ui20->cControlUnits = "";
	ui20->cVariableName = "m_2D_position";
	ui20->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui20->dPresetData[0] = 0.000000;ui20->dPresetData[1] = 0.000000;ui20->dPresetData[2] = 0.000000;ui20->dPresetData[3] = 0.000000;ui20->dPresetData[4] = 0.000000;ui20->dPresetData[5] = 0.000000;ui20->dPresetData[6] = 0.000000;ui20->dPresetData[7] = 0.000000;ui20->dPresetData[8] = 0.000000;ui20->dPresetData[9] = 0.000000;ui20->dPresetData[10] = 0.000000;ui20->dPresetData[11] = 0.000000;ui20->dPresetData[12] = 0.000000;ui20->dPresetData[13] = 0.000000;ui20->dPresetData[14] = 0.000000;ui20->dPresetData[15] = 0.000000;
	ui20->cControlName = "m_2D_position";
	ui20->bOwnerControl = false;
	ui20->bMIDIControl = false;
	ui20->uMIDIControlCommand = 176;
	ui20->uMIDIControlName = 3;
	ui20->uMIDIControlChannel = 0;
	ui20->nGUIRow = nIndexer++;
	ui20->nGUIColumn = -1;
	ui20->bEnableParamSmoothing = false;
	ui20->fSmoothingTimeInMs = 100.00;
	ui20->uControlTheme[0] = 0; ui20->uControlTheme[1] = 0; ui20->uControlTheme[2] = 0; ui20->uControlTheme[3] = 0; ui20->uControlTheme[4] = 0; ui20->uControlTheme[5] = 0; ui20->uControlTheme[6] = 0; ui20->uControlTheme[7] = 0; ui20->uControlTheme[8] = 20; ui20->uControlTheme[9] = 0; ui20->uControlTheme[10] = 0; ui20->uControlTheme[11] = 0; ui20->uControlTheme[12] = 0; ui20->uControlTheme[13] = 0; ui20->uControlTheme[14] = 0; ui20->uControlTheme[15] = 0; ui20->uControlTheme[16] = 2; ui20->uControlTheme[17] = 0; ui20->uControlTheme[18] = 0; ui20->uControlTheme[19] = 0; ui20->uControlTheme[20] = 0; ui20->uControlTheme[21] = 0; ui20->uControlTheme[22] = 0; ui20->uControlTheme[23] = 0; ui20->uControlTheme[24] = 0; ui20->uControlTheme[25] = 0; ui20->uControlTheme[26] = 0; ui20->uControlTheme[27] = 0; ui20->uControlTheme[28] = 0; ui20->uControlTheme[29] = 0; ui20->uControlTheme[30] = 0; ui20->uControlTheme[31] = 0; 
	ui20->uFluxCapControl[0] = 0; ui20->uFluxCapControl[1] = 0; ui20->uFluxCapControl[2] = 0; ui20->uFluxCapControl[3] = 0; ui20->uFluxCapControl[4] = 0; ui20->uFluxCapControl[5] = 0; ui20->uFluxCapControl[6] = 0; ui20->uFluxCapControl[7] = 0; ui20->uFluxCapControl[8] = 0; ui20->uFluxCapControl[9] = 0; ui20->uFluxCapControl[10] = 0; ui20->uFluxCapControl[11] = 0; ui20->uFluxCapControl[12] = 0; ui20->uFluxCapControl[13] = 0; ui20->uFluxCapControl[14] = 0; ui20->uFluxCapControl[15] = 0; ui20->uFluxCapControl[16] = 0; ui20->uFluxCapControl[17] = 0; ui20->uFluxCapControl[18] = 0; ui20->uFluxCapControl[19] = 0; ui20->uFluxCapControl[20] = 0; ui20->uFluxCapControl[21] = 0; ui20->uFluxCapControl[22] = 0; ui20->uFluxCapControl[23] = 0; ui20->uFluxCapControl[24] = 0; ui20->uFluxCapControl[25] = 0; ui20->uFluxCapControl[26] = 0; ui20->uFluxCapControl[27] = 0; ui20->uFluxCapControl[28] = 0; ui20->uFluxCapControl[29] = 0; ui20->uFluxCapControl[30] = 0; ui20->uFluxCapControl[31] = 0; ui20->uFluxCapControl[32] = 0; ui20->uFluxCapControl[33] = 0; ui20->uFluxCapControl[34] = 0; ui20->uFluxCapControl[35] = 0; ui20->uFluxCapControl[36] = 0; ui20->uFluxCapControl[37] = 0; ui20->uFluxCapControl[38] = 0; ui20->uFluxCapControl[39] = 0; ui20->uFluxCapControl[40] = 0; ui20->uFluxCapControl[41] = 0; ui20->uFluxCapControl[42] = 0; ui20->uFluxCapControl[43] = 0; ui20->uFluxCapControl[44] = 0; ui20->uFluxCapControl[45] = 0; ui20->uFluxCapControl[46] = 0; ui20->uFluxCapControl[47] = 0; ui20->uFluxCapControl[48] = 0; ui20->uFluxCapControl[49] = 0; ui20->uFluxCapControl[50] = 0; ui20->uFluxCapControl[51] = 0; ui20->uFluxCapControl[52] = 0; ui20->uFluxCapControl[53] = 0; ui20->uFluxCapControl[54] = 0; ui20->uFluxCapControl[55] = 0; ui20->uFluxCapControl[56] = 0; ui20->uFluxCapControl[57] = 0; ui20->uFluxCapControl[58] = 0; ui20->uFluxCapControl[59] = 0; ui20->uFluxCapControl[60] = 0; ui20->uFluxCapControl[61] = 0; ui20->uFluxCapControl[62] = 0; ui20->uFluxCapControl[63] = 0; 
	ui20->fFluxCapData[0] = 0.000000; ui20->fFluxCapData[1] = 0.000000; ui20->fFluxCapData[2] = 0.000000; ui20->fFluxCapData[3] = 0.000000; ui20->fFluxCapData[4] = 0.000000; ui20->fFluxCapData[5] = 0.000000; ui20->fFluxCapData[6] = 0.000000; ui20->fFluxCapData[7] = 0.000000; ui20->fFluxCapData[8] = 0.000000; ui20->fFluxCapData[9] = 0.000000; ui20->fFluxCapData[10] = 0.000000; ui20->fFluxCapData[11] = 0.000000; ui20->fFluxCapData[12] = 0.000000; ui20->fFluxCapData[13] = 0.000000; ui20->fFluxCapData[14] = 0.000000; ui20->fFluxCapData[15] = 0.000000; ui20->fFluxCapData[16] = 0.000000; ui20->fFluxCapData[17] = 0.000000; ui20->fFluxCapData[18] = 0.000000; ui20->fFluxCapData[19] = 0.000000; ui20->fFluxCapData[20] = 0.000000; ui20->fFluxCapData[21] = 0.000000; ui20->fFluxCapData[22] = 0.000000; ui20->fFluxCapData[23] = 0.000000; ui20->fFluxCapData[24] = 0.000000; ui20->fFluxCapData[25] = 0.000000; ui20->fFluxCapData[26] = 0.000000; ui20->fFluxCapData[27] = 0.000000; ui20->fFluxCapData[28] = 0.000000; ui20->fFluxCapData[29] = 0.000000; ui20->fFluxCapData[30] = 0.000000; ui20->fFluxCapData[31] = 0.000000; ui20->fFluxCapData[32] = 0.000000; ui20->fFluxCapData[33] = 0.000000; ui20->fFluxCapData[34] = 0.000000; ui20->fFluxCapData[35] = 0.000000; ui20->fFluxCapData[36] = 0.000000; ui20->fFluxCapData[37] = 0.000000; ui20->fFluxCapData[38] = 0.000000; ui20->fFluxCapData[39] = 0.000000; ui20->fFluxCapData[40] = 0.000000; ui20->fFluxCapData[41] = 0.000000; ui20->fFluxCapData[42] = 0.000000; ui20->fFluxCapData[43] = 0.000000; ui20->fFluxCapData[44] = 0.000000; ui20->fFluxCapData[45] = 0.000000; ui20->fFluxCapData[46] = 0.000000; ui20->fFluxCapData[47] = 0.000000; ui20->fFluxCapData[48] = 0.000000; ui20->fFluxCapData[49] = 0.000000; ui20->fFluxCapData[50] = 0.000000; ui20->fFluxCapData[51] = 0.000000; ui20->fFluxCapData[52] = 0.000000; ui20->fFluxCapData[53] = 0.000000; ui20->fFluxCapData[54] = 0.000000; ui20->fFluxCapData[55] = 0.000000; ui20->fFluxCapData[56] = 0.000000; ui20->fFluxCapData[57] = 0.000000; ui20->fFluxCapData[58] = 0.000000; ui20->fFluxCapData[59] = 0.000000; ui20->fFluxCapData[60] = 0.000000; ui20->fFluxCapData[61] = 0.000000; ui20->fFluxCapData[62] = 0.000000; ui20->fFluxCapData[63] = 0.000000; 
	uiCtrls.push_back(ui20);


	m_multiosc_detune = 0.000000;
	CUICtrl* ui21 = new CUICtrl;
	ui21->uControlType = FILTER_CONTROL_CONTINUOUSLY_VARIABLE;
	ui21->uControlId = 101;
	ui21->bLogSlider = false;
	ui21->bExpSlider = false;
	ui21->fUserDisplayDataLoLimit = 0.000000;
	ui21->fUserDisplayDataHiLimit = 1.000000;
	ui21->uUserDataType = floatData;
	ui21->fInitUserIntValue = 0;
	ui21->fInitUserFloatValue = 0.000000;
	ui21->fInitUserDoubleValue = 0;
	ui21->fInitUserUINTValue = 0;
	ui21->m_pUserCookedIntData = NULL;
	ui21->m_pUserCookedFloatData = &m_multiosc_detune;
	ui21->m_pUserCookedDoubleData = NULL;
	ui21->m_pUserCookedUINTData = NULL;
	ui21->cControlUnits = "";
	ui21->cVariableName = "m_multiosc_detune";
	ui21->cEnumeratedList = "SEL1,SEL2,SEL3";
	ui21->dPresetData[0] = 0.000000;ui21->dPresetData[1] = 0.000000;ui21->dPresetData[2] = 0.000000;ui21->dPresetData[3] = 0.000000;ui21->dPresetData[4] = 0.000000;ui21->dPresetData[5] = 0.000000;ui21->dPresetData[6] = 0.000000;ui21->dPresetData[7] = 0.000000;ui21->dPresetData[8] = 0.000000;ui21->dPresetData[9] = 0.000000;ui21->dPresetData[10] = 0.000000;ui21->dPresetData[11] = 0.000000;ui21->dPresetData[12] = 0.000000;ui21->dPresetData[13] = 0.000000;ui21->dPresetData[14] = 0.000000;ui21->dPresetData[15] = 0.000000;
	ui21->cControlName = "Detune";
	ui21->bOwnerControl = false;
	ui21->bMIDIControl = false;
	ui21->uMIDIControlCommand = 176;
	ui21->uMIDIControlName = 3;
	ui21->uMIDIControlChannel = 0;
	ui21->nGUIRow = nIndexer++;
	ui21->nGUIColumn = -1;
	ui21->bEnableParamSmoothing = false;
	ui21->fSmoothingTimeInMs = 100.00;
	ui21->uControlTheme[0
