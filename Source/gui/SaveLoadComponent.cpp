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

#include "SaveLoadComponent.h"
#include "../JuceLibraryCode/BinaryData.h"
#include "../JuceLibraryCode/JuceHeader.h"
//#include <fstream>
//#include <unistd.h>

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

SaveLoadComponent::SaveLoadComponent(AudioProcessorValueTreeState &vts, OdinAudioProcessor &p_processor) :
    m_save("save", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_load("load", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_up("up", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_down("down", juce::DrawableButton::ButtonStyle::ImageRaw), m_value_tree(vts), m_audio_processor(p_processor) {

	m_up.setClickingTogglesState(true);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { incrementPatch(); };

	m_down.setClickingTogglesState(true);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { decrementPatch(); };

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
	//m_last_directory = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName();

	m_save.onClick = [&]() {
		// for (int osc = 1; osc < 4; ++osc) {
		// 	std::string osc_string = std::to_string(osc);

		// 	if (usesWavedraw(osc)) {
		// 		DBG("uses wavedraw " + String(osc));
		// 	}
		// 	if (usesChipdraw(osc)) {
		// 		DBG("uses chipdraw " + String(osc));
		// 	}
		// 	if (usesSpecdraw(osc)) {
		// 		DBG("uses specdraw " + String(osc));
		// 	}
		// }

		// return;

		//suggestion where to save
		String current_directory = m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString();
		File fileToSave(current_directory + "/my_patch.odin");

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

			    if (file_name != "") {
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

					    //make a deep copy and remove the midi_learn part and file name
					    ValueTree copy_with_removed_params = m_value_tree.state.createCopy();
					    copy_with_removed_params.removeChild(copy_with_removed_params.getChildWithName("midi_learn"),
					                                         nullptr);
					    copy_with_removed_params.getChildWithName("misc").removeProperty("current_patch_filename",
					                                                                     nullptr);
					    copy_with_removed_params.getChildWithName("misc").removeProperty("current_patch_directory",
					                                                                     nullptr);

					    //remove draw osc params if they aren't needed
					    for (int osc = 1; osc < 4; ++osc) {
						    std::string osc_string = std::to_string(osc);

						    if (!usesWavedraw(osc)) {
							    //DBG("uses wavedraw " + String(osc));
							    for (int step = 0; step < WAVEDRAW_STEPS_X; ++step) {
								    copy_with_removed_params.getChildWithName("draw").removeProperty(
								        String("osc" + osc_string + "_wavedraw_values_" + std::to_string(step)),
								        nullptr);
							    }
						    }
						    if (!usesChipdraw(osc)) {
							    //DBG("uses chipdraw " + String(osc));
							    for (int step = 0; step < CHIPDRAW_STEPS_X; ++step) {
								    copy_with_removed_params.getChildWithName("draw").removeProperty(
								        String("osc" + osc_string + "_chipdraw_values_" + std::to_string(step)),
								        nullptr);
							    }
						    }
						    if (!usesSpecdraw(osc)) {
							    //DBG("uses specdraw " + String(osc));
							    for (int step = 0; step < SPECDRAW_STEPS_X; ++step) {
								    copy_with_removed_params.getChildWithName("draw").removeProperty(
								        String("osc" + osc_string + "_specdraw_values_" + std::to_string(step)),
								        nullptr);
							    }
						    }
					    }

					    //write valuetree into file
					    copy_with_removed_params.writeToStream(file_stream);

					    //set label
					    m_patch.setText(
					        m_value_tree.state.getChildWithName("misc")["patch_name"].toString().toStdString());

					    //save load directory
					    //m_last_directory = file_to_write.getParentDirectory().getFullPathName();

					    DBG(copy_with_removed_params.toXmlString());
					    DBG("Wrote above patch to " + file_name);

					    m_value_tree.state.getChildWithName("misc").setProperty(
					        "current_patch_filename",
					        file_to_write.getFileName(),
					        nullptr); //needed for up/down buttons in patch loading
					    m_value_tree.state.getChildWithName("misc").setProperty(
					        "current_patch_directory",
					        file_to_write.getParentDirectory().getFullPathName(),
					        nullptr); //needed for up/down buttons in patch loading
					    DBG("set filename in valuetree: " +
					        m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
					    DBG("set filepath in valuetree: " +
					        m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString());
				    }
			    }
		    });
	};

	m_load.onClick = [&]() { loadPatchWithFileBrowser(); };

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

			//reset pitchbend and modwheel, since they are not loaded with patches
			SETAUDIOFULLRANGESAFE("modwheel", 0.f);
			SETAUDIOFULLRANGESAFE("pitchbend", 0.f);

			//this forces values onto the GUI (patch label as well)
			forceValueTreeLambda();

			DBG("Loaded init patch");
		}
	};

	m_patch_dropdown_menu.setLookAndFeel(&m_menu_feels);

	m_patch.onMouseDown = [&]() {
		DBG("OPENMENU");

		String last_directory_name = m_value_tree.state.getChildWithName("misc")["current_patch_directory"];
		DBG(last_directory_name);

		File current_dir(last_directory_name);
		if (current_dir.isDirectory()) {
			DBG("IS DIR");
			m_patch_dropdown_menu.clear();

			Array<File> file_array = current_dir.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.odin");

			if (file_array.size() > 0) {

				file_array.sort(m_file_comparator);

				for (int file_index = 0; file_index < file_array.size(); ++file_index) {
					m_patch_dropdown_menu.addItem(file_index + 1,
					                              file_array[file_index].getFileName().dropLastCharacters(5));
				}
				int file_index_chosen = m_patch_dropdown_menu.show();

				if (file_index_chosen != 0) { //means none was selected
					File file_chosen = file_array[file_index_chosen - 1];

					FileInputStream file_stream(file_chosen);
					if (file_stream.openedOk()) {
						loadPatchFromOpenedFileStream(file_stream);
						m_value_tree.state.getChildWithName("misc").setProperty(
						    "current_patch_filename", file_chosen.getFileName(), nullptr);
						DBG("set filename in valuetree: " +
						    m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
					}
				}
			} else {
				//no files in current dir -> open filebrowser
				loadPatchWithFileBrowser();
			}
		} else {
			//no dir??? whatever, open filebrowser
			loadPatchWithFileBrowser();
		}
	};

	m_patch.setBounds(PATCH_POS_X, PATCH_POS_Y, m_patch_size_x, m_patch_size_y);
}

SaveLoadComponent::~SaveLoadComponent() {
	m_patch_dropdown_menu.setLookAndFeel(nullptr);
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
		DBG("Trying to load PMV " + std::to_string(patch_version) + ", current PMV is " +
		    std::to_string(ODIN_PATCH_MIGRATION_VERSION));
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

void SaveLoadComponent::setGUIBig() {

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_150_png, BinaryData::buttonup_2_150_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_150_png, BinaryData::buttonup_1_150_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(
	    OdinHelper::c150(UP_DOWN_BUTTON_X), OdinHelper::c150(UP_BUTTON_Y), up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 =
	    ImageCache::getFromMemory(BinaryData::buttondown_2_150_png, BinaryData::buttondown_2_150_pngSize);
	juce::Image down_2 =
	    ImageCache::getFromMemory(BinaryData::buttondown_1_150_png, BinaryData::buttondown_1_150_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(
	    OdinHelper::c150(UP_DOWN_BUTTON_X), OdinHelper::c150(DOWN_BUTTON_Y), down_1.getWidth(), down_1.getHeight());

	juce::Image save_1 =
	    ImageCache::getFromMemory(BinaryData::buttonsave_2_150_png, BinaryData::buttonsave_2_150_pngSize);
	juce::Image save_2 =
	    ImageCache::getFromMemory(BinaryData::buttonsave_1_150_png, BinaryData::buttonsave_1_150_pngSize);

	juce::DrawableImage save_draw1;
	juce::DrawableImage save_draw2;

	save_draw1.setImage(save_1);
	save_draw2.setImage(save_2);

	m_save.setImages(
	    &save_draw2, &save_draw2, &save_draw1, &save_draw1, &save_draw2, &save_draw2, &save_draw1, &save_draw1);
	m_save.setBounds(OdinHelper::c150(SAVE_POS_X), OdinHelper::c150(SAVE_POS_Y), save_1.getWidth(), save_1.getHeight());

	juce::Image load_1 =
	    ImageCache::getFromMemory(BinaryData::buttonload_2_150_png, BinaryData::buttonload_2_150_pngSize);
	juce::Image load_2 =
	    ImageCache::getFromMemory(BinaryData::buttonload_1_150_png, BinaryData::buttonload_1_150_pngSize);

	juce::DrawableImage load_draw1;
	juce::DrawableImage load_draw2;

	load_draw1.setImage(load_1);
	load_draw2.setImage(load_2);

	m_load.setImages(
	    &load_draw2, &load_draw2, &load_draw1, &load_draw1, &load_draw2, &load_draw2, &load_draw1, &load_draw1);
	m_load.setBounds(OdinHelper::c150(LOAD_POS_X), OdinHelper::c150(LOAD_POS_Y), load_1.getWidth(), load_1.getHeight());
	juce::Image reset_1 = ImageCache::getFromMemory(BinaryData::buttonreset_global_2_150_png,
	                                                BinaryData::buttonreset_global_2_150_pngSize);
	juce::Image reset_2 = ImageCache::getFromMemory(BinaryData::buttonreset_global_1_150_png,
	                                                BinaryData::buttonreset_global_1_150_pngSize);

	juce::DrawableImage reset_draw1;
	juce::DrawableImage reset_draw2;

	reset_draw1.setImage(reset_1);
	reset_draw2.setImage(reset_2);

	m_reset.setImages(
	    &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1, &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1);
	m_reset.setBounds(
	    OdinHelper::c150(RESET_TOP_POS_X), OdinHelper::c150(RESET_TOP_POS_Y), reset_1.getWidth(), reset_1.getHeight());

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_big_150_png, BinaryData::glaspanel_big_150_pngSize);

	m_patch.setImage(glas_panel);
	m_patch_size_x = glas_panel.getWidth();
	m_patch_size_y = glas_panel.getHeight();

	m_patch.setBounds(OdinHelper::c150(PATCH_POS_X), OdinHelper::c150(PATCH_POS_Y) + 2, m_patch_size_x, m_patch_size_y);
	m_patch.setGUIBig();
}
void SaveLoadComponent::setGUISmall() {
	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(UP_DOWN_BUTTON_X + 1, UP_BUTTON_Y - 1, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(UP_DOWN_BUTTON_X + 1, DOWN_BUTTON_Y - 1, down_1.getWidth(), down_1.getHeight());

	juce::Image save_1 = ImageCache::getFromMemory(BinaryData::buttonsave_2_png, BinaryData::buttonsave_2_pngSize);
	juce::Image save_2 = ImageCache::getFromMemory(BinaryData::buttonsave_1_png, BinaryData::buttonsave_1_pngSize);

	juce::DrawableImage save_draw1;
	juce::DrawableImage save_draw2;

	save_draw1.setImage(save_1);
	save_draw2.setImage(save_2);

	m_save.setImages(
	    &save_draw2, &save_draw2, &save_draw1, &save_draw1, &save_draw2, &save_draw2, &save_draw1, &save_draw1);
	m_save.setBounds(SAVE_POS_X, SAVE_POS_Y, save_1.getWidth(), save_1.getHeight());

	juce::Image load_1 = ImageCache::getFromMemory(BinaryData::buttonload_2_png, BinaryData::buttonload_2_pngSize);
	juce::Image load_2 = ImageCache::getFromMemory(BinaryData::buttonload_1_png, BinaryData::buttonload_1_pngSize);

	juce::DrawableImage load_draw1;
	juce::DrawableImage load_draw2;

	load_draw1.setImage(load_1);
	load_draw2.setImage(load_2);

	m_load.setImages(
	    &load_draw2, &load_draw2, &load_draw1, &load_draw1, &load_draw2, &load_draw2, &load_draw1, &load_draw1);
	m_load.setBounds(LOAD_POS_X, LOAD_POS_Y, load_1.getWidth(), load_1.getHeight());
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
	m_reset.setBounds(RESET_TOP_POS_X, RESET_TOP_POS_Y, reset_1.getWidth(), reset_1.getHeight());

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_big_png, BinaryData::glaspanel_big_pngSize);

	m_patch.setImage(glas_panel);
	m_patch_size_x = glas_panel.getWidth();
	m_patch_size_y = glas_panel.getHeight();

	m_patch.setBounds(PATCH_POS_X, PATCH_POS_Y, m_patch_size_x, m_patch_size_y);

	m_patch.setGUISmall();
}

void SaveLoadComponent::loadPatchFromOpenedFileStream(juce::FileInputStream &p_file_stream) {
	//first see if the patch is of a higher version than we know about:
	std::string version_string;
	if (checkForBiggerVersion(p_file_stream, version_string)) {
		//abort with icon
		AlertWindow::showMessageBox(
		    AlertWindow::AlertIconType::WarningIcon,
		    "Cannot load patch!",
		    "The bad news: You cannot load this patch, because you are on version " + ODIN_VERSION_STRING +
		        ".\nThe good news: The patch you're trying to load was created on version " + version_string +
		        ". So go to www.thewavewarden.com and download the latest version of Odin2!",
		    "Thanks, I will!");
		return;
	}

	// if we're reading an older patch version we set the default patch first
	// since new params might not exist and will only be set
	if (checkForSmallerVersion(p_file_stream, version_string)) {
		DBG("Reading older patch, setting init patch first");
		MemoryInputStream init_stream(BinaryData::init_patch_odin, BinaryData::init_patch_odinSize, false);
		m_audio_processor.readPatch(ValueTree::readFromStream(init_stream));
	}

	//reset stream position
	p_file_stream.setPosition(0);
	m_audio_processor.readPatch(ValueTree::readFromStream(p_file_stream).createCopy());
	forceValueTreeLambda();

	//set the correct Version number again
	m_value_tree.state.getChildWithName("misc").setProperty("version_minor", ODIN_MINOR_VERSION, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty("version_patch", ODIN_PATCH_VERSION, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty(
	    "patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);

	//this forces values onto the GUI (patch label as well)
	//forceValueTreeLambda();
}

void SaveLoadComponent::incrementPatch() {
	DBG("INCREMENT starting points:");

	String current_filename  = m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString();
	String current_directory = m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString();

	DBG(current_directory);
	DBG(current_filename);

	File current_dir(current_directory);
	if (current_dir.isDirectory()) {
		DBG("IS DIR");

		Array<File> file_array = current_dir.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.odin");
		file_array.sort(m_file_comparator);

		if (file_array.size() > 0) {
			//if we find none, just start at the last...
			int next_file_index = file_array.size() - 1;

			for (int file_index = 0; file_index < file_array.size(); ++file_index) {
				if (file_array[file_index].getFileName() == current_filename) {
					next_file_index = file_index;
					break;
				}
			}

			if (next_file_index != 0) {
				--next_file_index;
			}

			File file_chosen = file_array[next_file_index];

			FileInputStream file_stream(file_chosen);
			if (file_stream.openedOk()) {
				loadPatchFromOpenedFileStream(file_stream);
				m_value_tree.state.getChildWithName("misc").setProperty(
				    "current_patch_filename", file_chosen.getFileName(), nullptr);
				DBG("set filename in valuetree: " +
				    m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
			}
		} else {
			//no files in current dir -> open filebrowser
			loadPatchWithFileBrowser();
		}
	} else {
		DBG("IS DIR N'T");
	}
}
void SaveLoadComponent::decrementPatch() {
	DBG("DECREMENT starting points:");

	String current_filename  = m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString();
	String current_directory = m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString();

	DBG(current_directory);
	DBG(current_filename);

	File current_dir(current_directory);
	if (current_dir.isDirectory()) {
		DBG("IS DIR");

		Array<File> file_array = current_dir.findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.odin");
		file_array.sort(m_file_comparator);

		if (file_array.size() > 0) {
			//if we find none, just start at the first
			int next_file_index = 0;

			for (int file_index = 0; file_index < file_array.size(); ++file_index) {
				if (file_array[file_index].getFileName() == current_filename) {
					next_file_index = file_index;
					break;
				}
			}

			if (next_file_index != file_array.size() - 1) {
				++next_file_index;
			}

			File file_chosen = file_array[next_file_index];

			FileInputStream file_stream(file_chosen);
			if (file_stream.openedOk()) {
				loadPatchFromOpenedFileStream(file_stream);
				m_value_tree.state.getChildWithName("misc").setProperty(
				    "current_patch_filename", file_chosen.getFileName(), nullptr);
				DBG("set filename in valuetree: " +
				    m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
			}
		} else {
			//no files in current dir -> open filebrowser
			loadPatchWithFileBrowser();
		}
	} else {
		DBG("IS DIR N'T");
		loadPatchWithFileBrowser();
	}
}

void SaveLoadComponent::loadPatchWithFileBrowser() {
	String current_directory = m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString();
	m_filechooser.reset(new FileChooser("Choose a file to open...", current_directory, "*.odin", true));

	m_filechooser->launchAsync(
	    FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles, [this](const FileChooser &chooser) {
		    String file_name;
		    auto results = chooser.getURLResults();

		    for (auto result : results)
			    file_name << (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(false));

		    File file_to_read(file_name);

		    FileInputStream file_stream(file_to_read);
		    if (file_stream.openedOk()) {

			    loadPatchFromOpenedFileStream(file_stream);

			    //save load directory
			    //m_last_directory = file_to_read.getParentDirectory().getFullPathName();

			    m_value_tree.state.getChildWithName("misc").setProperty(
			        "current_patch_filename",
			        file_to_read.getFileName(),
			        nullptr); //needed for up/down buttons in patch loading
			    m_value_tree.state.getChildWithName("misc").setProperty(
			        "current_patch_directory",
			        file_to_read.getParentDirectory().getFullPathName(),
			        nullptr); //needed for up/down buttons in patch loading
			    DBG("set filename in valuetree: " +
			        m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
			    DBG("set filepath in valuetree: " +
			        m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString());

		    } else {
			    if (file_name != "") {
				    AlertWindow::showMessageBoxAsync(
				        AlertWindow::InfoIcon, "File not found!", "Path: " + file_name, "Ok");
			    }
		    }
	    });
}

bool SaveLoadComponent::usesWavedraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) ==
	    OSC_TYPE_WAVEDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use wavedraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 600 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 600 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 600 + p_osc) {
				return true;
			}
		}
	}

	//wavedraw can be used by LFOs as well
	for (int lfo = 1; lfo < 5; ++lfo) {
		if ((int)(m_value_tree.state.getChildWithName("lfo")[String("lfo" + std::to_string(lfo) + "_wave")]) ==
		    16 + p_osc) {
			return true;
		}
	}

	return false;
}

bool SaveLoadComponent::usesChipdraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) ==
	    OSC_TYPE_CHIPDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use chipdraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_CHIPTUNE) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_chipwave")]) == 700 + p_osc) {
				return true;
			}
		}
	}

	return false;
}

bool SaveLoadComponent::usesSpecdraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) ==
	    OSC_TYPE_SPECDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use wavedraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 800 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 800 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 800 + p_osc) {
				return true;
			}
		}
	}

	return false;
}