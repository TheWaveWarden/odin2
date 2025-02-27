/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

//this file is included from PluginProcessor.cpp to split the class implementation

#include "PluginProcessor.h"

const String OdinAudioProcessor::getName() const {
	return JucePlugin_Name;
}

double OdinAudioProcessor::getTailLengthSeconds() const {
	return 0.0;
}

int OdinAudioProcessor::getNumPrograms() {
	return 1; // NB: some hosts don't cope very well if you tell them there are 0
	          // programs, so this should be at least 1, even if you're not really
	          // implementing programs.
}

int OdinAudioProcessor::getCurrentProgram() {
	return 0;
}

void OdinAudioProcessor::setCurrentProgram(int index) {
}

const String OdinAudioProcessor::getProgramName(int index) {
	return {};
}

void OdinAudioProcessor::changeProgramName(int index, const String &newName) {
}

//==============================================================================
void OdinAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	setSampleRate(sampleRate);
}

void OdinAudioProcessor::releaseResources() {
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OdinAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {

	//only support stereo output
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	return true;
}
#endif

//==============================================================================
bool OdinAudioProcessor::hasEditor() const {
	return true;
}

AudioProcessorEditor *OdinAudioProcessor::createEditor() {

	OdinEditor *editor = new OdinEditor(*this, m_value_tree, m_is_standalone_plugin);
	m_editor_pointer                 = editor;

	return editor;
}

//==============================================================================
void OdinAudioProcessor::getStateInformation(MemoryBlock &destData) {
	// this is called when DAW saves a file

	auto state = m_value_tree.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());

	//add tuning to xml
	//! XmlElement::addChildElement takes ownership of these, using delete causes segfault
	XmlElement *scl = new XmlElement("tuning_scl");
	XmlElement *kbm = new XmlElement("tuning_kbm");

	scl->setAttribute("data", m_tuning.scale.rawText);
	kbm->setAttribute("data", m_tuning.keyboardMapping.rawText);

	xml->addChildElement(scl);
	xml->addChildElement(kbm);

	copyXmlToBinary(*xml, destData);
}

//this is called when DAW restores a file
void OdinAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {

	// disable for standalone plugins
	//if (wrapperType == wrapperType_Standalone) {
	//		return;
	//	}

	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	if (xmlState.get() != nullptr) {

		//DBG(xmlState->toString());

		//read tunings and remove them from xmltree:
		Tunings::Scale scl           = Tunings::evenTemperament12NoteScale();
		Tunings::KeyboardMapping kbm = Tunings::tuneNoteTo(60, Tunings::MIDI_0_FREQ * 32.0);
		for (auto *child : xmlState->getChildIterator()) {
			if (child->hasTagName("tuning_scl")) {
				try {
					//DBG(child->getStringAttribute("data"));
					scl = Tunings::parseSCLData(child->getStringAttribute("data").toStdString());
				} catch (...) {
					DBG("Failed to load .scl from binary state... resorting to default");
					scl = Tunings::evenTemperament12NoteScale();
				}
			}
			if (child->hasTagName("tuning_kbm")) {
				try {
					//DBG(child->getStringAttribute("data"));
					kbm = Tunings::parseKBMData(child->getStringAttribute("data").toStdString());
				} catch (...) {
					DBG("Failed to load .kbm from binary state... resorting to default");
					kbm = Tunings::tuneNoteTo(60, Tunings::MIDI_0_FREQ * 32.0);
				}
			}
		}
		xmlState->deleteAllChildElementsWithTagName("tuning_kbm");
		xmlState->deleteAllChildElementsWithTagName("tuning_scl");
		jassert(xmlState->getChildByName("tuning_kbm") == nullptr);
		jassert(xmlState->getChildByName("tuning_scl") == nullptr);

		m_tuning = Tunings::Tuning(scl, kbm);

		if (xmlState->hasTagName(m_value_tree.state.getType())) {

			//avoid reading from newer patch versions
			int patch_migration_version_read =
			    xmlState->getChildByName("misc")->getIntAttribute("patch_migration_version");
			if (patch_migration_version_read > ODIN_PATCH_MIGRATION_VERSION) {
				AlertWindow::showMessageBox(
				    AlertWindow::AlertIconType::WarningIcon,
				    "You are trying to load a project which was saved with a newer Version of Odin2. Please go to "
				    "www.thewavewarden.com and download the newest version to properly use this project!",
				    "Thanks, I will!");
				return;
			}

			//load data
			readPatch(ValueTree::fromXml(*xmlState));

			//set the correct version since an old one was maybe set from patch
			m_value_tree.state.getChildWithName("misc").setProperty("version_minor", ODIN_MINOR_VERSION, nullptr);
			m_value_tree.state.getChildWithName("misc").setProperty("version_patch", ODIN_PATCH_VERSION, nullptr);
			m_value_tree.state.getChildWithName("misc").setProperty(
			    "patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);

			m_force_values_onto_gui = true;

			createDrawTablesFromValueTree();

			if (m_editor_pointer) {
				m_editor_pointer->forceValueTreeOntoComponents(false);
			}
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
	return new OdinAudioProcessor();
}

void OdinAudioProcessor::setSampleRate(float p_samplerate) {
	DBG("SampleRate was set to " + std::to_string((int)p_samplerate) + " Hz!");

	m_samplerate = p_samplerate;

	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].setSampleRate(p_samplerate);
	}
	for (int stereo = 0; stereo < 2; ++stereo) {
		m_flanger[stereo].setSampleRate(p_samplerate);
		m_chorus[stereo].setSampleRate(p_samplerate);

		m_ladder_filter[stereo].setSampleRate(p_samplerate);
		m_korg_filter[stereo].setSampleRate(p_samplerate);
		m_diode_filter[stereo].setSampleRate(p_samplerate);
		m_comb_filter[stereo].setSampleRate(p_samplerate);
		m_formant_filter[stereo].setSampleRate(p_samplerate);
		m_SEM_filter_12[stereo].setSampleRate(p_samplerate);
		m_ring_mod[stereo].setSampleRate(p_samplerate);
	}

	m_delay.setSampleRate(p_samplerate);
	m_phaser.setSampleRate(p_samplerate);
	m_reverb_zita.setSampleRate(p_samplerate);
	m_reverb_zita.prepare();
	m_global_env.setSampleRate(p_samplerate);
	m_global_lfo.setSampleRate(p_samplerate);
	m_arpeggiator.setSampleRate(p_samplerate);

	//update glide
	float glide_samplerate_adjusted = pow(GETAUDIO("glide"), 44800. / m_samplerate);
	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].setGlide(glide_samplerate_adjusted);
	}
}

void OdinAudioProcessor::initializeModules() {
	m_global_env.reset();
	m_global_env.setEnvelopeOff(); // so it doesn't start by itself

	m_mod_matrix.setSourcesAndDestinations(&m_mod_sources, &m_mod_destinations);

	m_phaser.setRadiusBase(1.25f);

	// make "stereo" chorus & flanger
	m_chorus[1].setLFOResetPos(0.25);
	m_flanger[1].setLFOResetPos(0.5);

	setModulationPointers();

	// set analog osc as sync osc for start
	for (int voice = 0; voice < VOICES; ++voice) {
		//todo these are probably set wrongly on plugin load....
		m_voice[voice].setOscSyncOscillator(&(m_voice[voice].analog_osc[0]));
	}
}

void OdinAudioProcessor::setPitchWheelValue(int p_value) {
	*m_pitchbend = (float)(p_value - 8192) / 8192.f;
	updatePitchWheelGUI(*m_pitchbend);
}

void OdinAudioProcessor::setModWheelValue(int p_value) {
	*m_modwheel = (float)(p_value) / 128.f;
	updateModWheelGUI(*m_modwheel);
}

void OdinAudioProcessor::checkEndGlobalEnvelope() {
	for (int voice = 0; voice < VOICES; ++voice) {
		if (m_voice[voice] && m_voice[voice].env[0].isBeforeRelease()) {
			// dont kill it
			return;
		}
	}
	// kill it
	m_global_env.startRelease();
	//DBG("kill global env");
}

void OdinAudioProcessor::resetAudioEngine() {

	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].hardReset();
	}
	for (int stereo = 0; stereo < 2; ++stereo) {
		m_ladder_filter[stereo].reset();
		m_SEM_filter_12[stereo].reset();
		m_korg_filter[stereo].reset();
		m_diode_filter[stereo].reset();
		m_formant_filter[stereo].reset();
		m_comb_filter[stereo].reset();

		m_flanger[stereo].reset();
		m_chorus[stereo].reset();
	}

	m_delay.reset();
	m_phaser.reset();
	m_global_lfo.reset();
	m_global_env.reset();

	m_voice_manager.reset();
}

void OdinAudioProcessor::setBPM(float p_BPM) {
	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].setBPM(p_BPM, m_lfo1_sync, m_lfo2_sync, m_lfo3_sync);
	}
	if (m_delay_sync) {
		m_delay.setFreqBPM(p_BPM);
	}
	if (m_phaser_sync) {
		m_phaser.setFreqBPM(p_BPM);
	}
	if (m_flanger_sync) {
		m_flanger[0].setFreqBPM(p_BPM);
		m_flanger[1].setFreqBPM(p_BPM);
	}
	if (m_chorus_sync) {
		m_chorus[0].setFreqBPM(p_BPM);
		m_chorus[1].setFreqBPM(p_BPM);
	}
	if (m_lfo4_sync) {
		m_global_lfo.setFreqBPM(p_BPM);
	}
	m_arpeggiator.setBPM(p_BPM);
}

void OdinAudioProcessor::setFXButtonsPosition(int p_delay, int p_phaser, int p_flanger, int p_chorus, int p_reverb) {

	m_delay_position   = p_delay;
	m_phaser_position  = p_phaser;
	m_flanger_position = p_flanger;
	m_chorus_position  = p_chorus;
	m_reverb_position  = p_reverb;
}

void OdinAudioProcessor::setFilter3EnvValue() {

	// get adsr value newest voice filterenv
	float env_value = m_adsr[m_voice_manager.getNewestVoiceIndex()][1];

	m_ladder_filter[0].m_env_value  = env_value;
	m_SEM_filter_12[0].m_env_value  = env_value;
	m_korg_filter[0].m_env_value    = env_value;
	m_diode_filter[0].m_env_value   = env_value;
	m_comb_filter[0].m_env_value    = env_value;
	m_formant_filter[0].m_env_value = env_value;
	m_ring_mod[0].m_env_value       = env_value;
	m_ladder_filter[1].m_env_value  = env_value;
	m_SEM_filter_12[1].m_env_value  = env_value;
	m_korg_filter[1].m_env_value    = env_value;
	m_diode_filter[1].m_env_value   = env_value;
	m_comb_filter[1].m_env_value    = env_value;
	m_formant_filter[1].m_env_value = env_value;
	m_ring_mod[1].m_env_value       = env_value;
}

void OdinAudioProcessor::attachNonParamListeners() {
	m_value_tree_fx         = m_value_tree.state.getChildWithName("fx");
	m_value_tree_osc        = m_value_tree.state.getChildWithName("osc");
	m_value_tree_mod        = m_value_tree.state.getChildWithName("mod");
	m_value_tree_misc       = m_value_tree.state.getChildWithName("misc");
	m_value_tree_lfo        = m_value_tree.state.getChildWithName("lfo");
	m_value_tree_draw       = m_value_tree.state.getChildWithName("draw");
	m_value_tree_midi_learn = m_value_tree.state.getChildWithName("midi_learn");
}

void OdinAudioProcessor::onEditorDestruction() {
	m_editor_pointer = nullptr;
}

void OdinAudioProcessor::setMonoPolyLegato(PlayModes p_mode) {
	m_mono_poly_legato = p_mode;

	bool legato_was_changed = m_voice_manager.setMonoPolyLegato(p_mode);
	for (int voice = 0; voice < VOICES; ++voice) {
		m_voice[voice].setMonoPolyLegato(p_mode);
	}

	if (legato_was_changed) {
		resetAudioEngine();
		m_playmode_mono_note_list.clear();
	}
}