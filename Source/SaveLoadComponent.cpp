#include "SaveLoadComponent.h"
#include "../JuceLibraryCode/BinaryData.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <fstream>
#include <unistd.h>

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
			    auto result      = chooser.getURLResult();
			    String file_name = result.isEmpty() ? String()
			                                        : (result.isLocalFile() ? result.getLocalFile().getFullPathName()
			                                                                : result.toString(true));
			    //append .odin if not already there
				file_name = file_name.endsWith(".odin") ? file_name : file_name + ".odin";

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
				    std::string version_string;
				    if (checkForBiggerVersion(file_stream, version_string)) {
					    //abort with icon
					    AlertWindow::showMessageBox(
					        AlertWindow::AlertIconType::WarningIcon,
					        "Cannot load patch!",
					        "The bad news: You cannot load this patch, because you are on version " +
					            ODIN_VERSION_STRING +
					            ".\nThe good news: The patch you're trying to load was created on version " +
					            version_string +
					            ". So go to TODO.com already and download the latest version of Odin2!",
					        "Thanks, I will!");
					    return;
				    }

				    // if we're reading an older patch version we set the default patch first
				    // since new params might not exist and will only be set
				    if (checkForSmallerVersion(file_stream, version_string)) {
					    DBG("Reading older patch, setting init patch first");
					    MemoryInputStream init_stream(
					        BinaryData::init_patch_odin, BinaryData::init_patch_odinSize, false);
					    m_audio_processor.readPatch(ValueTree::readFromStream(init_stream));
				    }

				    //DBG("PATCH TO BE LOADED:\n\n\n\n");
				    //file_stream.setPosition(0);
				    //ValueTree copy_read = ValueTree::readFromStream(file_stream).createCopy();
				    //DBG(copy_read.toXmlString());
				    //DBG("PATCH AFTER LOAD:");

				    //reset stream position
				    file_stream.setPosition(0);
				    m_audio_processor.readPatch(ValueTree::readFromStream(file_stream).createCopy());
				    forceValueTreeLambda();

				    //set the correct Version number again
				    m_value_tree.state.getChildWithName("misc").setProperty(
				        "version_minor", ODIN_MINOR_VERSION, nullptr);
				    m_value_tree.state.getChildWithName("misc").setProperty(
				        "version_patch", ODIN_PATCH_VERSION, nullptr);
				    m_value_tree.state.getChildWithName("misc").setProperty(
				        "patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);

				    //this forces values onto the GUI (patch label as well)
					//DBG("Modmatrix: " + std::to_string((float)GETAUDIO("amount_1_row_0")));
					//DBG("Attack: " + std::to_string((float)GETAUDIO("env2_attack")));
				    forceValueTreeLambda();

				    //save load directory
				    m_last_directory = file_to_read.getParentDirectory().getFullPathName();

				    //DBG(m_value_tree.state.toXmlString());
				    //DBG("Loaded above patch " + m_value_tree.state.getChildWithName("misc")["patch_name"].toString() +
				    //    "  from " + file_name);

					//usleep(1000*1000);
					//DBG("Modmatrix: " + std::to_string((float)GETAUDIO("amount_1_row_0")));
					//DBG("Attack: " + std::to_string((float)GETAUDIO("env2_attack")));
			    } else {
				    if (file_name != "") {
					    AlertWindow::showMessageBoxAsync(
					        AlertWindow::InfoIcon, "File not found!", "Path: " + file_name, "Ok");
				    }
			    }
				
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

			// replace stream with patch from binary data
			MemoryInputStream init_stream(BinaryData::init_patch_odin, BinaryData::init_patch_odinSize, false);
			m_audio_processor.readPatch(ValueTree::readFromStream(init_stream));

			//this forces values onto the GUI (patch label as well)
			forceValueTreeLambda();

			DBG("Loaded init patch");
		}
		//usleep(1000*1000);
		//DBG("Modmatrix: " + std::to_string((float)GETAUDIO("amount_1_row_0")));
  		//DBG("Attack: " + std::to_string((float)GETAUDIO("env2_attack")));
	};

	m_patch.setBounds(PATCH_POS_X, PATCH_POS_Y, m_patch_size_x, m_patch_size_y);
}

SaveLoadComponent::~SaveLoadComponent() {
}

void SaveLoadComponent::forceValueTreeOntoComponents(ValueTree p_tree) {
	//DBG("SAVELOADFORCEVALUETREE\n\n\n");
	m_patch.setText((p_tree.getChildWithName("misc")["patch_name"]).toString().toStdString());
}

bool SaveLoadComponent::checkForBiggerVersion(FileInputStream &p_file_stream, std::string &p_version_string) {
	p_file_stream.setPosition(0);
	auto value_tree_read = ValueTree::readFromStream(p_file_stream);
	int patch_version    = value_tree_read.getChildWithName("misc")["patch_migration_version"];
	if (patch_version > ODIN_PATCH_MIGRATION_VERSION) {
		p_version_string = "2." + std::to_string((int)value_tree_read.getChildWithName("misc")["version_minor"]) + "." +
		                   std::to_string((int)value_tree_read.getChildWithName("misc")["version_patch"]);
		DBG("Trying to load PMV " + std::to_string(patch_version) +  ", current PMV is " + std::to_string(ODIN_PATCH_MIGRATION_VERSION));
		return true;
	}
	return false;
}

bool SaveLoadComponent::checkForSmallerVersion(FileInputStream &p_file_stream, std::string &p_version_string) {
	p_file_stream.setPosition(0);
	auto value_tree_read = ValueTree::readFromStream(p_file_stream);
	int patch_version    = value_tree_read.getChildWithName("misc")["patch_migration_version"];
	if (patch_version < ODIN_PATCH_MIGRATION_VERSION) {
		p_version_string = "2." + std::to_string((int)value_tree_read.getChildWithName("misc")["version_minor"]) + "." +
		                   std::to_string((int)value_tree_read.getChildWithName("misc")["version_patch"]);
		return true;
	}
	return false;
}