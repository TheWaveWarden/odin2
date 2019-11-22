/*
  ==============================================================================

    SaveLoadComponent.cpp
    Created: 27 Feb 2019 11:01:28am
    Author:  frot

  ==============================================================================
*/

#include "SaveLoadComponent.h"
#include "../JuceLibraryCode/BinaryData.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <fstream>

std::string getFileNameFromAbsolute(const std::string &s) {

	char sep = '/';

#ifdef ODIN_WIN
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		std::string ret = s.substr(i + 1, s.length() - i);

		std::string erase = ".odin";
		size_t pos        = ret.find(erase);
		if (pos != std::string::npos) {
			ret.erase(pos, erase.length());
		}
		return ret;
	}

	return ("");
}

//==============================================================================
SaveLoadComponent::SaveLoadComponent(AudioProcessorValueTreeState &vts, OdinAudioProcessor &p_processor) :
    m_save("save", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_load("load", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
    /// m_random("random", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_value_tree(vts), m_audio_processor(p_processor) {
	juce::Image save_1 = ImageCache::getFromMemory(BinaryData::buttonsave_2_png, BinaryData::buttonsave_2_pngSize);
	juce::Image save_2 = ImageCache::getFromMemory(BinaryData::buttonsave_1_png, BinaryData::buttonsave_1_pngSize);

	juce::DrawableImage save_draw1;
	juce::DrawableImage save_draw2;

	save_draw1.setImage(save_1);
	save_draw2.setImage(save_2);

	m_save.setImages(
	    &save_draw2, &save_draw2, &save_draw1, &save_draw1, &save_draw2, &save_draw2, &save_draw1, &save_draw1);
	m_save.setClickingTogglesState(true);
	m_save.setBounds(SAVE_POS_X, SAVE_POS_Y, save_1.getWidth(), save_1.getHeight());
	addAndMakeVisible(m_save);
	m_save.setTooltip("Save a patch to your harddrive");
	m_save.setTriggeredOnMouseDown(false);
	m_save.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	juce::Image load_1 = ImageCache::getFromMemory(BinaryData::buttonload_2_png, BinaryData::buttonload_2_pngSize);
	juce::Image load_2 = ImageCache::getFromMemory(BinaryData::buttonload_1_png, BinaryData::buttonload_1_pngSize);

	juce::DrawableImage load_draw1;
	juce::DrawableImage load_draw2;

	load_draw1.setImage(load_1);
	load_draw2.setImage(load_2);

	m_load.setImages(
	    &load_draw2, &load_draw2, &load_draw1, &load_draw1, &load_draw2, &load_draw2, &load_draw1, &load_draw1);
	m_load.setClickingTogglesState(true);
	m_load.setBounds(LOAD_POS_X, LOAD_POS_Y, load_1.getWidth(), load_1.getHeight());
	addAndMakeVisible(m_load);
	m_load.setTriggeredOnMouseDown(false);
	m_load.setTooltip("Load a patch from\n your harddrive");
	m_load.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	juce::Image reset_1 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_global_2_png, BinaryData::buttonreset_global_2_pngSize);
	juce::Image reset_2 =
	    ImageCache::getFromMemory(BinaryData::buttonreset_global_1_png, BinaryData::buttonreset_global_1_pngSize);

	juce::DrawableImage reset_draw1;
	juce::DrawableImage reset_draw2;

	reset_draw1.setImage(reset_1);
	reset_draw2.setImage(reset_2);

	m_reset.setImages(
	    &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1, &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1);
	m_reset.setClickingTogglesState(true);
	m_reset.setBounds(RESET_TOP_POS_X, RESET_TOP_POS_Y, reset_1.getWidth(), reset_1.getHeight());
	m_reset.setTooltip("Reset the synth to its initial state");
	addAndMakeVisible(m_reset);
	m_reset.setTriggeredOnMouseDown(false);
	m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_big_png, BinaryData::glaspanel_big_pngSize);

	m_patch.setImage(glas_panel);
	m_patch.setColor(DARKGREY);
	m_patch_size_x = glas_panel.getWidth();
	m_patch_size_y = glas_panel.getHeight();
	//m_patch.setText("init_patch");
	addAndMakeVisible(m_patch);

	//set initial save/load location to "Documents"
	m_last_directory = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName();

	m_save.onClick = [&]() {
		//suggestion where to save
		File fileToSave(m_last_directory + "/my_patch.odin");

		// set up filechooser
		m_filechooser.reset(new FileChooser("Choose a file to save...", fileToSave, "*.odin", true));

		//launch filechooser
		m_filechooser->launchAsync(
		    FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
		    [fileToSave, this](const FileChooser &chooser) {
			    auto result    = chooser.getURLResult();
			    auto file_name = result.isEmpty() ? String()
			                                      : (result.isLocalFile() ? result.getLocalFile().getFullPathName()
			                                                              : result.toString(true));

			    File file_to_write(file_name);

			    //check whether file already exists
			    if (file_to_write.existsAsFile()) {
				    if (!(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon,
				                                       "File already exists!",
				                                       "Are you sure you want to overwrite it?",
				                                       {},
				                                       {},
				                                       {}))) {
					    //user selected cancel
					    return;
				    }
			    }
			    FileOutputStream file_stream(file_to_write);
			    if (file_stream.openedOk()) {
				    // use this to overwrite old content
				    file_stream.setPosition(0);
				    file_stream.truncate();

				    //write patch name onto valuetree
				    m_value_tree.state.getChildWithName("misc").setProperty(
				        "patch_name", file_to_write.getFileNameWithoutExtension(), nullptr);
				    DBG(m_value_tree.state.getChildWithName("misc")["patch_name"].toString());

				    //make a deep copy and remove the midi_learn part
				    ValueTree copy_without_midi_learn = m_value_tree.state.createCopy();
				    copy_without_midi_learn.removeChild(copy_without_midi_learn.getChildWithName("midi_learn"),
				                                        nullptr);

				    //write valuetree into file
				    copy_without_midi_learn.writeToStream(file_stream);

				    //set label
				    m_patch.setText(m_value_tree.state.getChildWithName("misc")["patch_name"].toString().toStdString());

				    //save load directory
				    m_last_directory = file_to_write.getParentDirectory().getFullPathName();

				    DBG(copy_without_midi_learn.toXmlString());
				    DBG("Wrote above patch to " + file_name);
			    }
		    });
	};

	m_load.onClick = [&]() {
		m_filechooser.reset(new FileChooser("Choose a file to open...", m_last_directory, "*.odin", true));

		m_filechooser->launchAsync(
		    FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles, [this](const FileChooser &chooser) {
			    String file_name;
			    auto results = chooser.getURLResults();

			    for (auto result : results)
				    file_name << (result.isLocalFile() ? result.getLocalFile().getFullPathName()
				                                       : result.toString(false));

			    File file_to_read(file_name);

			    FileInputStream file_stream(file_to_read);
			    if (file_stream.openedOk()) {

				    //first see if the patch is of a higher version than we know about:
				   // if (checkForBiggerVersion(ValueTree::readFromStream(file_stream))) {
					    //abort with icon
				    //}

				    //save midi learn tree
				    ValueTree midi_learn_tree = m_value_tree.state.getChildWithName("midi_learn");

				    //read tree from file
				    m_value_tree.replaceState(ValueTree::readFromStream(file_stream));

				    //if we read an old patch, we might need to add parameters
				    versionMigrate();

				    //reappend midi learn tree
				    m_value_tree.state.appendChild(midi_learn_tree, nullptr);

				    //reattach the non_param listeners
				    m_audio_processor.attachNonParamListeners();

				    //set the correct Version number again
				    m_value_tree.state.getChildWithName("misc").setProperty(
				        "version_minor", ODIN_MINOR_VERSION, nullptr);
				    m_value_tree.state.getChildWithName("misc").setProperty(
				        "version_patch", ODIN_PATCH_VERSION, nullptr);
				    m_value_tree.state.getChildWithName("misc").setProperty(
				        "patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);

				    //this forces values onto the GUI (patch label as well)
				    forceValueTreeLambda();

				    //retrigger all listeners to distribute values to DSP engine
				    m_audio_processor.retriggerAllListeners();

				    //save load directory
				    m_last_directory = file_to_read.getParentDirectory().getFullPathName();

				    DBG(m_value_tree.state.toXmlString());
				    DBG("Loaded above patch " + m_value_tree.state.getChildWithName("misc")["patch_name"].toString() +
				        "  from " + file_name);
			    } else {
				    if (file_name != "") {
					    AlertWindow::showMessageBoxAsync(
					        AlertWindow::InfoIcon, "File not found!", "Path: " + file_name, "Ok");
				    }
			    }
			    //DBG(m_value_tree.state.toXmlString());
		    });
	};

	m_reset.onClick = [&]() {
		if (/*m_reset_warning_was_shown ||*/
		    AlertWindow::showOkCancelBox(AlertWindow::WarningIcon,
		                                 "Reset Synth",
		                                 "This will reset the synth to its initial state and you will lose "
		                                 "your patch!",
		                                 {},
		                                 {},
		                                 {})) {

			//save midi learn tree
			ValueTree midi_learn_tree = m_value_tree.state.getChildWithName("midi_learn");

			// replace stream with patch from binary data
			MemoryInputStream init_stream(BinaryData::init_patch_odin, BinaryData::init_patch_odinSize, false);
			m_value_tree.replaceState(ValueTree::readFromStream(init_stream));

			//reappend midi learn tree
			m_value_tree.state.appendChild(midi_learn_tree, nullptr);

			//reattach non param listeners
			m_audio_processor.attachNonParamListeners();

			//this forces values onto the GUI (patch label as well)
			forceValueTreeLambda();

			//retrigger all listeners to distribute values to DSP engine
			m_audio_processor.retriggerAllListeners();

			DBG("Loaded init patch");
		}
	};

	m_patch.setBounds(PATCH_POS_X, PATCH_POS_Y, m_patch_size_x, m_patch_size_y);
}

SaveLoadComponent::~SaveLoadComponent() {
}

void SaveLoadComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	DBG("SAVELOADFORCEVALUETREE\n\n\n");
	m_patch.setText((p_tree.getChildWithName("misc")["patch_name"]).toString().toStdString());
}

void SaveLoadComponent::versionMigrate() {
	//we just replaced the value tree with the patch
	//we also read the version from the patch which is now in m_value_tree

	int minor_version           = m_value_tree.state.getChildWithName("misc")["version_minor"];
	int patch_version           = m_value_tree.state.getChildWithName("misc")["version_patch"];
	int patch_migration_version = m_value_tree.state.getChildWithName("misc")["patch_migration_version"];

	DBG("Read patch from version 2." + std::to_string(minor_version) + "." + std::to_string(patch_version));
	DBG("Current version is: 2." + std::to_string(ODIN_MINOR_VERSION) + "." + std::to_string(ODIN_PATCH_VERSION));
	DBG("Read patch migration version " + std::to_string(patch_migration_version) + " current version is " +
	    std::to_string(ODIN_PATCH_MIGRATION_VERSION));
}

bool SaveLoadComponent::checkForBiggerVersion(const ValueTree &p_tree) {
	int patch_version = p_tree.getChildWithName("misc")["patch_migration_version"];
	return (patch_version > ODIN_PATCH_MIGRATION_VERSION);
}