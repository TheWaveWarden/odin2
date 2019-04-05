/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OdinAudioProcessor::OdinAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
                         ),
#endif
     : m_parameters(*this, nullptr, Identifier("Odin"),
#include "AudioValueTree.h"
      )
{

  m_osc1_type = m_parameters.getRawParameterValue("osc1_type");
  m_osc1_oct = m_parameters.getRawParameterValue("osc1_oct");
  m_osc1_semi = m_parameters.getRawParameterValue("osc1_semi");
  m_osc1_fine = m_parameters.getRawParameterValue("osc1_fine");
  m_osc1_vol = m_parameters.getRawParameterValue("osc1_vol");
  m_osc1_position = m_parameters.getRawParameterValue("osc1_position");
  m_osc1_detune = m_parameters.getRawParameterValue("osc1_detune");
  m_osc1_multi_position =
      m_parameters.getRawParameterValue("osc1_multi_position");
  m_osc1_spread = m_parameters.getRawParameterValue("osc1_spread");
  m_osc1_pulsewidth = m_parameters.getRawParameterValue("osc1_pulsewidth");
  m_osc1_drift = m_parameters.getRawParameterValue("osc1_drift");
  m_osc1_arp_speed = m_parameters.getRawParameterValue("osc1_arp_speed");
  m_osc1_step_1 = m_parameters.getRawParameterValue("osc1_step_1");
  m_osc1_step_2 = m_parameters.getRawParameterValue("osc1_step_2");
  m_osc1_step_3 = m_parameters.getRawParameterValue("osc1_step_3");
  m_osc1_fm = m_parameters.getRawParameterValue("osc1_fm");
  m_osc1_carrier_ratio = m_parameters.getRawParameterValue("osc1_carrier_ratio");
  m_osc1_modulator_ratio = m_parameters.getRawParameterValue("osc1_modulator_ratio");
  m_osc1_lp = m_parameters.getRawParameterValue("osc1_lp");
  m_osc1_hp = m_parameters.getRawParameterValue("osc1_hp");
  m_osc1_reset = m_parameters.getRawParameterValue("osc1_reset");
  m_osc1_arp_on = m_parameters.getRawParameterValue("osc1_arp_on");
  m_osc1_step_3_on = m_parameters.getRawParameterValue("osc1_step_3_on");
  m_osc1_chipnoise = m_parameters.getRawParameterValue("osc1_chipnoise");
  m_osc1_exp_fm = m_parameters.getRawParameterValue("osc1_exp_fm");
  m_osc1_vec_a = m_parameters.getRawParameterValue("osc1_vec_a");
  m_osc1_vec_b = m_parameters.getRawParameterValue("osc1_vec_b");
  m_osc1_vec_c = m_parameters.getRawParameterValue("osc1_vec_c");
  m_osc1_vec_d = m_parameters.getRawParameterValue("osc1_vec_d");
  m_osc1_vec_x = m_parameters.getRawParameterValue("osc1_vec_x");
  m_osc1_vec_y = m_parameters.getRawParameterValue("osc1_vec_y");
  m_osc1_wavetable = m_parameters.getRawParameterValue("osc1_wavetable");

  m_osc2_type = m_parameters.getRawParameterValue("osc2_type");
  m_osc2_oct = m_parameters.getRawParameterValue("osc2_oct");
  m_osc2_semi = m_parameters.getRawParameterValue("osc2_semi");
  m_osc2_fine = m_parameters.getRawParameterValue("osc2_fine");
  m_osc2_vol = m_parameters.getRawParameterValue("osc2_vol");
  m_osc2_position = m_parameters.getRawParameterValue("osc2_position");
  m_osc2_detune = m_parameters.getRawParameterValue("osc2_detune");
  m_osc2_multi_position =
      m_parameters.getRawParameterValue("osc2_multi_position");
  m_osc2_spread = m_parameters.getRawParameterValue("osc2_spread");
  m_osc2_pulsewidth = m_parameters.getRawParameterValue("osc2_pulsewidth");
  m_osc2_drift = m_parameters.getRawParameterValue("osc2_drift");
  m_osc2_arp_speed = m_parameters.getRawParameterValue("osc2_arp_speed");
  m_osc2_step_1 = m_parameters.getRawParameterValue("osc2_step_1");
  m_osc2_step_2 = m_parameters.getRawParameterValue("osc2_step_2");
  m_osc2_step_3 = m_parameters.getRawParameterValue("osc2_step_3");
  m_osc2_fm = m_parameters.getRawParameterValue("osc2_fm");
  m_osc2_carrier_ratio = m_parameters.getRawParameterValue("osc2_carrier_ratio");
  m_osc2_modulator_ratio = m_parameters.getRawParameterValue("osc2_modulator_ratio");
  m_osc2_lp = m_parameters.getRawParameterValue("osc2_lp");
  m_osc2_hp = m_parameters.getRawParameterValue("osc2_hp");
  m_osc2_reset = m_parameters.getRawParameterValue("osc2_reset");
  m_osc2_arp_on = m_parameters.getRawParameterValue("osc2_arp_on");
  m_osc2_step_3_on = m_parameters.getRawParameterValue("osc2_step_3_on");
  m_osc2_chipnoise = m_parameters.getRawParameterValue("osc2_chipnoise");
  m_osc2_exp_fm = m_parameters.getRawParameterValue("osc2_exp_fm");
  m_osc2_vec_a = m_parameters.getRawParameterValue("osc2_vec_a");
  m_osc2_vec_b = m_parameters.getRawParameterValue("osc2_vec_b");
  m_osc2_vec_c = m_parameters.getRawParameterValue("osc2_vec_c");
  m_osc2_vec_d = m_parameters.getRawParameterValue("osc2_vec_d");
  m_osc2_vec_x = m_parameters.getRawParameterValue("osc2_vec_x");
  m_osc2_vec_y = m_parameters.getRawParameterValue("osc2_vec_y");
  m_osc2_wavetable = m_parameters.getRawParameterValue("osc2_wavetable");

  m_osc3_type = m_parameters.getRawParameterValue("osc3_type");
  m_osc3_oct = m_parameters.getRawParameterValue("osc3_oct");
  m_osc3_semi = m_parameters.getRawParameterValue("osc3_semi");
  m_osc3_fine = m_parameters.getRawParameterValue("osc3_fine");
  m_osc3_vol = m_parameters.getRawParameterValue("osc3_vol");
  m_osc3_position = m_parameters.getRawParameterValue("osc3_position");
  m_osc3_detune = m_parameters.getRawParameterValue("osc3_detune");
  m_osc3_multi_position =
      m_parameters.getRawParameterValue("osc3_multi_position");
  m_osc3_spread = m_parameters.getRawParameterValue("osc3_spread");
  m_osc3_pulsewidth = m_parameters.getRawParameterValue("osc3_pulsewidth");
  m_osc3_drift = m_parameters.getRawParameterValue("osc3_drift");
  m_osc3_arp_speed = m_parameters.getRawParameterValue("osc3_arp_speed");
  m_osc3_step_1 = m_parameters.getRawParameterValue("osc3_step_1");
  m_osc3_step_2 = m_parameters.getRawParameterValue("osc3_step_2");
  m_osc3_step_3 = m_parameters.getRawParameterValue("osc3_step_3");
  m_osc3_fm = m_parameters.getRawParameterValue("osc3_fm");
  m_osc3_carrier_ratio = m_parameters.getRawParameterValue("osc3_carrier_ratio");
  m_osc3_modulator_ratio = m_parameters.getRawParameterValue("osc3_modulator_ratio");
  m_osc3_lp = m_parameters.getRawParameterValue("osc3_lp");
  m_osc3_hp = m_parameters.getRawParameterValue("osc3_hp");
  m_osc3_reset = m_parameters.getRawParameterValue("osc3_reset");
  m_osc3_arp_on = m_parameters.getRawParameterValue("osc3_arp_on");
  m_osc3_step_3_on = m_parameters.getRawParameterValue("osc3_step_3_on");
  m_osc3_chipnoise = m_parameters.getRawParameterValue("osc3_chipnoise");
  m_osc3_exp_fm = m_parameters.getRawParameterValue("osc3_exp_fm");
  m_osc3_vec_a = m_parameters.getRawParameterValue("osc3_vec_a");
  m_osc3_vec_b = m_parameters.getRawParameterValue("osc3_vec_b");
  m_osc3_vec_c = m_parameters.getRawParameterValue("osc3_vec_c");
  m_osc3_vec_d = m_parameters.getRawParameterValue("osc3_vec_d");
  m_osc3_vec_x = m_parameters.getRawParameterValue("osc3_vec_x");
  m_osc3_vec_y = m_parameters.getRawParameterValue("osc3_vec_y");
  m_osc3_wavetable = m_parameters.getRawParameterValue("osc3_wavetable");

  m_fil1_type = m_parameters.getRawParameterValue("fil1_type");
  m_fil1_vel = m_parameters.getRawParameterValue("fil1_vel");
  m_fil1_kbd = m_parameters.getRawParameterValue("fil1_kbd");
  m_fil1_gain = m_parameters.getRawParameterValue("fil1_gain");
  m_fil1_freq = m_parameters.getRawParameterValue("fil1_freq");
  m_fil1_res = m_parameters.getRawParameterValue("fil1_res");
  m_fil1_saturation = m_parameters.getRawParameterValue("fil1_saturation");
  m_fil1_formant_transition =
      m_parameters.getRawParameterValue("fil1_formant_transition");
  m_fil1_sem_transition =
      m_parameters.getRawParameterValue("fil1_sem_transition");
  m_fil1_vowel_left = m_parameters.getRawParameterValue("fil1_vowel_left");
  m_fil1_vowel_right = m_parameters.getRawParameterValue("fil1_vowel_right");
  m_fil1_comb_polarity = m_parameters.getRawParameterValue("fil1_comb_polarity");

  m_fil2_type = m_parameters.getRawParameterValue("fil2_type");
  m_fil2_vel = m_parameters.getRawParameterValue("fil2_vel");
  m_fil2_kbd = m_parameters.getRawParameterValue("fil2_kbd");
  m_fil2_gain = m_parameters.getRawParameterValue("fil2_gain");
  m_fil2_freq = m_parameters.getRawParameterValue("fil2_freq");
  m_fil2_res = m_parameters.getRawParameterValue("fil2_res");
  m_fil2_saturation = m_parameters.getRawParameterValue("fil2_saturation");
  m_fil2_formant_transition =
      m_parameters.getRawParameterValue("fil2_formant_transition");
  m_fil2_sem_transition =
      m_parameters.getRawParameterValue("fil2_sem_transition");
  m_fil2_vowel_left = m_parameters.getRawParameterValue("fil2_vowel_left");
  m_fil2_vowel_right = m_parameters.getRawParameterValue("fil2_vowel_right");
  m_fil2_comb_polarity = m_parameters.getRawParameterValue("fil2_comb_polarity");

  m_fil3_type = m_parameters.getRawParameterValue("fil3_type");
  m_fil3_vel = m_parameters.getRawParameterValue("fil3_vel");
  m_fil3_kbd = m_parameters.getRawParameterValue("fil3_kbd");
  m_fil3_gain = m_parameters.getRawParameterValue("fil3_gain");
  m_fil3_freq = m_parameters.getRawParameterValue("fil3_freq");
  m_fil3_res = m_parameters.getRawParameterValue("fil3_res");
  m_fil3_saturation = m_parameters.getRawParameterValue("fil3_saturation");
  m_fil3_formant_transition =
      m_parameters.getRawParameterValue("fil3_formant_transition");
  m_fil3_sem_transition =
      m_parameters.getRawParameterValue("fil3_sem_transition");
  m_fil3_vowel_left = m_parameters.getRawParameterValue("fil3_vowel_left");
  m_fil3_vowel_right = m_parameters.getRawParameterValue("fil3_vowel_right");
  m_fil3_comb_polarity = m_parameters.getRawParameterValue("fil3_comb_polarity");

  m_amp_pan = m_parameters.getRawParameterValue("amp_pan");
  m_amp_gain = m_parameters.getRawParameterValue("amp_gain");
  m_amp_vel = m_parameters.getRawParameterValue("amp_vel");
  m_dist_threshold = m_parameters.getRawParameterValue("dist_threshold");
  m_dist_drywet = m_parameters.getRawParameterValue("dist_drywet");
  m_dist_on = m_parameters.getRawParameterValue("dist_on");
  m_dist_algo = m_parameters.getRawParameterValue("dist_algo");
  m_fil1_to_amp = m_parameters.getRawParameterValue("fil1_to_amp");
  m_fil2_to_amp = m_parameters.getRawParameterValue("fil2_to_amp");

  m_env1_attack = m_parameters.getRawParameterValue("env1_attack");
  m_env1_decay = m_parameters.getRawParameterValue("env1_decay");
  m_env1_sustain = m_parameters.getRawParameterValue("env1_sustain");
  m_env1_release = m_parameters.getRawParameterValue("env1_release");
  m_env1_loop = m_parameters.getRawParameterValue("env1_loop");

  m_env2_attack = m_parameters.getRawParameterValue("env2_attack");
  m_env2_decay = m_parameters.getRawParameterValue("env2_decay");
  m_env2_sustain = m_parameters.getRawParameterValue("env2_sustain");
  m_env2_release = m_parameters.getRawParameterValue("env2_release");
  m_env2_loop = m_parameters.getRawParameterValue("env2_loop");

  m_env3_attack = m_parameters.getRawParameterValue("env3_attack");
  m_env3_decay = m_parameters.getRawParameterValue("env3_decay");
  m_env3_sustain = m_parameters.getRawParameterValue("env3_sustain");
  m_env3_release = m_parameters.getRawParameterValue("env3_release");
  m_env3_loop = m_parameters.getRawParameterValue("env3_loop");

  m_env4_attack = m_parameters.getRawParameterValue("env4_attack");
  m_env4_decay = m_parameters.getRawParameterValue("env4_decay");
  m_env4_sustain = m_parameters.getRawParameterValue("env4_sustain");
  m_env4_release = m_parameters.getRawParameterValue("env4_release");
  m_env4_loop = m_parameters.getRawParameterValue("env4_loop");

  m_lfo1_freq = m_parameters.getRawParameterValue("lfo1_freq");
  m_lfo1_reset = m_parameters.getRawParameterValue("lfo1_reset");
  m_lfo1_sync = m_parameters.getRawParameterValue("lfo1_sync");
  m_lfo1_wave = m_parameters.getRawParameterValue("lfo1_wave");
  m_lfo1_synctime_numerator = m_parameters.getRawParameterValue("lfo1_synctime_numerator");
  m_lfo1_synctime_denominator = m_parameters.getRawParameterValue("lfo1_synctime_denominator");

  m_lfo2_freq = m_parameters.getRawParameterValue("lfo2_freq");
  m_lfo2_reset = m_parameters.getRawParameterValue("lfo2_reset");
  m_lfo2_sync = m_parameters.getRawParameterValue("lfo2_sync");
  m_lfo2_wave = m_parameters.getRawParameterValue("lfo2_wave");
  m_lfo2_synctime_numerator = m_parameters.getRawParameterValue("lfo2_synctime_numerator");
  m_lfo2_synctime_denominator = m_parameters.getRawParameterValue("lfo2_synctime_denominator");

  m_lfo3_freq = m_parameters.getRawParameterValue("lfo3_freq");
  m_lfo3_reset = m_parameters.getRawParameterValue("lfo3_reset");
  m_lfo3_sync = m_parameters.getRawParameterValue("lfo3_sync");
  m_lfo3_wave = m_parameters.getRawParameterValue("lfo3_wave");
  m_lfo3_synctime_numerator = m_parameters.getRawParameterValue("lfo3_synctime_numerator");
  m_lfo3_synctime_denominator = m_parameters.getRawParameterValue("lfo3_synctime_denominator");

  m_lfo4_freq = m_parameters.getRawParameterValue("lfo4_freq");
  m_lfo4_reset = m_parameters.getRawParameterValue("lfo4_reset");
  m_lfo4_sync = m_parameters.getRawParameterValue("lfo4_sync");
  m_lfo4_wave = m_parameters.getRawParameterValue("lfo4_wave");
  m_lfo4_synctime_numerator = m_parameters.getRawParameterValue("lfo4_synctime_numerator");
  m_lfo4_synctime_denominator = m_parameters.getRawParameterValue("lfo4_synctime_denominator");
  
  m_phaser_on = m_parameters.getRawParameterValue("phaser_on");
  m_flanger_on = m_parameters.getRawParameterValue("flanger_on");
  m_delay_on = m_parameters.getRawParameterValue("delay_on");
  m_chorus_on = m_parameters.getRawParameterValue("chorus_on");

  m_fil1_osc1 = m_parameters.getRawParameterValue("fil1_osc1");
  m_fil1_osc2 = m_parameters.getRawParameterValue("fil1_osc2");
  m_fil1_osc3 = m_parameters.getRawParameterValue("fil1_osc3");
  m_fil2_osc1 = m_parameters.getRawParameterValue("fil2_osc1");
  m_fil2_osc2 = m_parameters.getRawParameterValue("fil2_osc2");
  m_fil2_osc3 = m_parameters.getRawParameterValue("fil2_osc3");
  m_fil2_fil1 = m_parameters.getRawParameterValue("fil2_fil1");
  m_xy_x = m_parameters.getRawParameterValue("xy_x");
  m_xy_y = m_parameters.getRawParameterValue("xy_y");
  m_glide = m_parameters.getRawParameterValue("glide");
  m_master = m_parameters.getRawParameterValue("master");
  m_modwheel = m_parameters.getRawParameterValue("modwheel");
  m_pitchbend = m_parameters.getRawParameterValue("pitchbend");
  m_pitchbend_amount = m_parameters.getRawParameterValue("pitchbend_amount");

  m_delay_time = m_parameters.getRawParameterValue("delay_time");
  m_delay_feedback = m_parameters.getRawParameterValue("delay_feedback");
  m_delay_hp = m_parameters.getRawParameterValue("delay_hp");
  m_delay_ducking = m_parameters.getRawParameterValue("delay_ducking");
  m_delay_dry = m_parameters.getRawParameterValue("delay_dry");
  m_delay_wet = m_parameters.getRawParameterValue("delay_wet");
  m_delay_sync = m_parameters.getRawParameterValue("delay_sync");
  m_delay_synctime_numerator = m_parameters.getRawParameterValue("delay_synctime_numerator");
  m_delay_synctime_denominator = m_parameters.getRawParameterValue("delay_synctime_denominator");
  

  m_phaser_frequency = m_parameters.getRawParameterValue("phaser_frequency");
  m_phaser_amount = m_parameters.getRawParameterValue("phaser_amount");
  m_phaser_drywet = m_parameters.getRawParameterValue("phaser_drywet");
  m_phaser_sync = m_parameters.getRawParameterValue("phaser_sync");
  m_phaser_reset = m_parameters.getRawParameterValue("phaser_reset");
  m_phaser_synctime_numerator = m_parameters.getRawParameterValue("phaser_synctime_numerator");
  m_phaser_synctime_denominator = m_parameters.getRawParameterValue("phaser_synctime_denominator");

  m_flanger_frequency = m_parameters.getRawParameterValue("flanger_frequency");
  m_flanger_amount = m_parameters.getRawParameterValue("flanger_amount");
  m_flanger_drywet = m_parameters.getRawParameterValue("flanger_drywet");
  m_flanger_sync = m_parameters.getRawParameterValue("flanger_sync");
  m_flanger_reset = m_parameters.getRawParameterValue("flanger_reset");
  m_flanger_synctime_numerator = m_parameters.getRawParameterValue("flanger_synctime_numerator");
  m_flanger_synctime_denominator = m_parameters.getRawParameterValue("flanger_synctime_denominator");

  m_chorus_frequency = m_parameters.getRawParameterValue("chorus_frequency");
  m_chorus_amount = m_parameters.getRawParameterValue("chorus_amount");
  m_chorus_drywet = m_parameters.getRawParameterValue("chorus_drywet");
  m_chorus_sync = m_parameters.getRawParameterValue("chorus_sync");
  m_chorus_reset = m_parameters.getRawParameterValue("chorus_reset");
  m_chorus_synctime_numerator = m_parameters.getRawParameterValue("chorus_synctime_numerator");
  m_chorus_synctime_denominator = m_parameters.getRawParameterValue("chorus_synctime_denominator");

  m_delay_position = m_parameters.getRawParameterValue("delay_position");
  m_flanger_position = m_parameters.getRawParameterValue("flanger_position");
  m_phaser_position = m_parameters.getRawParameterValue("phaser_position");
  m_chorus_position = m_parameters.getRawParameterValue("chorus_position");
  
  for(int i = 0; i < 9; ++i){
    m_amount_1[i] = m_parameters.getRawParameterValue("amount_1_[" + std::to_string(i) + "]");
    m_amount_2[i] = m_parameters.getRawParameterValue("amount_2_[" + std::to_string(i) + "]");
    m_amount_3[i] = m_parameters.getRawParameterValue("amount_3_[" + std::to_string(i) + "]");
    m_source[i] = m_parameters.getRawParameterValue("source_[" + std::to_string(i) + "]");
    m_dest_1[i] = m_parameters.getRawParameterValue("dest_1_[" + std::to_string(i) + "]");
    m_dest_2[i] = m_parameters.getRawParameterValue("dest_2_[" + std::to_string(i) + "]");
    m_scale[i] = m_parameters.getRawParameterValue("scale_[" + std::to_string(i) + "]");
  }
  
}

OdinAudioProcessor::~OdinAudioProcessor() {}

//==============================================================================
const String OdinAudioProcessor::getName() const { return JucePlugin_Name; }

bool OdinAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool OdinAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool OdinAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double OdinAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int OdinAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int OdinAudioProcessor::getCurrentProgram() { return 0; }

void OdinAudioProcessor::setCurrentProgram(int index) {}

const String OdinAudioProcessor::getProgramName(int index) { return {}; }

void OdinAudioProcessor::changeProgramName(int index, const String &newName) {}

//==============================================================================
void OdinAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
}

void OdinAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OdinAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
    return false;

    // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void OdinAudioProcessor::processBlock(AudioBuffer<float> &buffer,
                                      MidiBuffer &midiMessages) {
  ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();
  
  //DBG(*m_dest_1[0]);
  //DBG(*m_dest_2[1]);
  //DBG(*m_scale[2]);
  //DBG("---------");
  //DBG(*m_delay_synctime_denominator);

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  // This is here to avoid people getting screaming feedback
  // when they first compile a plugin, but obviously you don't need to keep
  // this code if your algorithm always overwrites all the output channels.
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto *channelData = buffer.getWritePointer(channel);

    // ..do something to the data...
  }
}

//==============================================================================
bool OdinAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor *OdinAudioProcessor::createEditor() {
  return new OdinAudioProcessorEditor(*this, m_parameters);
}

//==============================================================================
void OdinAudioProcessor::getStateInformation(MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void OdinAudioProcessor::setStateInformation(const void *data,
                                             int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new OdinAudioProcessor();
}
