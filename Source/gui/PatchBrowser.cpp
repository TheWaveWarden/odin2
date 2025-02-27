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

#include "PatchBrowser.h"
#include "../ConfigFileManager.h"
#include "FactoryPresetBinaryMapping.h"
#include "UIAssetManager.h"
#include <JuceHeader.h>

PatchBrowser::PatchBrowser(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &p_vts) :
    m_soundbank_selector(File::TypesOfFileToFind::findDirectories, "Import", "Export", "New"),
    m_category_selector(File::TypesOfFileToFind::findDirectories, "", "", "New"),
    m_patch_selector(File::TypesOfFileToFind::findFiles, "Import", "Export", "Save"),
    m_audio_processor(p_processor),
    m_value_tree(p_vts)

{
	setWantsKeyboardFocus(true);

	addAndMakeVisible(m_soundbank_selector);
	addAndMakeVisible(m_category_selector);
	addAndMakeVisible(m_patch_selector);

	m_soundbank_selector.setType(PatchBrowserSelector::BrowserType::Soundbank);
	m_category_selector.setType(PatchBrowserSelector::BrowserType::Category);
	m_patch_selector.setType(PatchBrowserSelector::BrowserType::Patch);

	m_patch_selector.enablePassActiveNameToParent(true);

	m_soundbank_selector.setCopyMoveEnabled(false);
	m_category_selector.setCopyTargetName("Soundbank");
	m_patch_selector.setCopyTargetName("Category");

	File dir_to_create(DEFAULT_SOUNDBANK_LOCATION_STRING);
	if (!dir_to_create.exists()) {
		dir_to_create.createDirectory();
	}

	m_soundbank_selector.setDirectory(DEFAULT_SOUNDBANK_LOCATION_STRING);

	m_category_selector.setDirectoryFactoryPresetCategory();
	m_patch_selector.setDirectoryFactoryPresetPreset("Arps & Sequences");

	m_patch_selector.setWildCard("*.odin");
	m_patch_selector.setDirectoryFactoryPresetPreset("Arps & Sequences");

	m_soundbank_selector.passValueToPatchBrowser = [&](String p_string) {
		if (p_string == FACTORY_PRESETS_SOUNDBANK_CODE) {
			DBG("The Factory Preset Soundbank has been requested!");
			m_category_selector.setDirectoryFactoryPresetCategory();
			m_category_selector.highlightFirstEntry();
			m_patch_selector.setDirectoryFactoryPresetPreset("Arps & Sequences");

			const auto category = m_category_selector.getSelectedEntry();
			writeSelectedEntriesToValueTree("Static Factory Presets", category, "");
			return;
		}
		DBG(p_string + " was pressed in soundbank");
		m_category_selector.setDirectory(m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_string);
		m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());

		const auto category = m_category_selector.getSelectedEntry();
		writeSelectedEntriesToValueTree(p_string, category, "");
	};

	m_category_selector.passValueToPatchBrowser = [&](String p_string) {
		//handle factory sounds:
		if (p_string.toStdString().find(FACTORY_PRESETS_SOUNDBANK_CODE) != std::string::npos) {
			const auto category = p_string.toStdString().substr(std::string(FACTORY_PRESETS_SOUNDBANK_CODE).size());
			m_patch_selector.setDirectoryFactoryPresetPreset(category);

			writeSelectedEntriesToValueTree("Static Factory Presets", category, "");
			return;
		}
		DBG(p_string + " was pressed in category");
		m_patch_selector.setDirectory(m_category_selector.getDirectory() + File::getSeparatorString() + p_string);

		const auto soundbank = m_soundbank_selector.getSelectedEntry();
		writeSelectedEntriesToValueTree(soundbank, p_string, "");
	};

	m_patch_selector.passValueToPatchBrowser = [&](String p_string) {
		//handle factory sounds

		const auto soundbank = m_soundbank_selector.getSelectedEntry();
		const auto category  = m_category_selector.getSelectedEntry();
		const auto patch     = p_string;

		if (p_string.toStdString().find(FACTORY_PRESETS_SOUNDBANK_CODE) != std::string::npos) {
			auto binary_patch_key = p_string.toStdString().substr(std::string(FACTORY_PRESETS_SOUNDBANK_CODE).size());
			DBG("Loading Binary Patch:" + binary_patch_key);
			auto binary_patch_data = getFactoryPresetBinaryData(binary_patch_key);

			MemoryInputStream init_stream(binary_patch_data.first, binary_patch_data.second, false);
			m_audio_processor.readPatch(ValueTree::readFromStream(init_stream));

			//reset pitchbend and modwheel, since they are not loaded with patches
			SETAUDIOFULLRANGESAFE("modwheel", 0.f);
			SETAUDIOFULLRANGESAFE("pitchbend", 0.f);

			//force modmatrix to show
			m_value_tree.state.getChildWithName("misc").setProperty("arp_mod_selected", MATRIX_SECTION_INDEX_MATRIX, nullptr);

			m_value_tree.state.getChildWithName("misc").setProperty("arp_mod_selected", MATRIX_SECTION_INDEX_PRESETS, nullptr);

			writeSelectedEntriesToValueTree(soundbank, category, patch);

			juce::ScopedValueSetter<bool> setter(m_is_selected_from_internal, true);
			forceValueTreeLambda();

			return;
		}

		DBG("Try to open patch: " + m_patch_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_patch_selector.getDirectory() + File::getSeparatorString() + p_string;

		File file_to_open(absolute_path);

		FileInputStream file_stream(file_to_open);
		if (file_stream.openedOk()) {
			loadPatchFromOpenedFileStream(file_stream);
			m_value_tree.state.getChildWithName("misc").setProperty("current_patch_filename", file_to_open.getFileName(), nullptr);
			DBG("set filename in valuetree: " + m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());

			juce::ScopedValueSetter<bool> setter(m_is_selected_from_internal, true);
			writeSelectedEntriesToValueTree(soundbank, category, patch);
		}
	};

	m_soundbank_selector.passDeleteToPatchBrowser = [&](String p_string) {
		DBG("DELETE directory: " + m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_string;

		if (AlertWindow::showOkCancelBox(
		        AlertWindow::WarningIcon, "Delete Category", "Are you sure you want to delete the Soundbank " + p_string + " and ALL the presets in it?", {}, {}, {})) {
			File file_to_delete(absolute_path);
			if (file_to_delete.deleteRecursively()) {
				DBG("Directory was deleted!");

				//avoid segfault from pending mousedown on deleted browserentry every now and then
				//Time::waitForMillisecondCounter(20);

				m_soundbank_selector.regenerateContent();

				m_category_selector.setDirectory(m_soundbank_selector.getFirstSubDirectoryAndHighlightIt());
				m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());

				m_soundbank_selector.positionEntries();
			} else {
				DBG("file was NOT deleted!");
			}
		}
	};

	m_category_selector.passDeleteToPatchBrowser = [&](String p_string) {
		DBG("DELETE directory: " + m_category_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_category_selector.getDirectory() + File::getSeparatorString() + p_string;

		if (AlertWindow::showOkCancelBox(
		        AlertWindow::WarningIcon, "Delete Category", "Are you sure you want to delete the category " + p_string + " and ALL the presets in it?", {}, {}, {})) {
			File file_to_delete(absolute_path);
			if (file_to_delete.deleteRecursively()) {
				DBG("DBG: Directory was deleted!");

				//avoid segfault from pending mousedown on deleted browserentry every now and then
				//Time::waitForMillisecondCounter(20);

				m_category_selector.regenerateContent();
				m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
				m_category_selector.positionEntries();
			} else {
				DBG("file was NOT deleted!");
			}
		}
	};

	m_patch_selector.passDeleteToPatchBrowser = [&](String p_string) {
		DBG("DELETE patch: " + m_patch_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_patch_selector.getDirectory() + File::getSeparatorString() + p_string;

		if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Delete Preset", "Are you sure you want to delete preset " + p_string + "?", {}, {}, {})) {
			File file_to_delete(absolute_path);
			if (file_to_delete.deleteFile()) {
				DBG("file was deleted!");

				//avoid segfault from pending mousedown on deleted browserentry every now and then
				//Time::waitForMillisecondCounter(20);

				m_patch_selector.regenerateContent();
			} else {
				DBG("file was NOT deleted!");
			}
		}
	};

	m_patch_selector.onCreateNewFile = [&](String p_string) {
		DBG("Save patch: " + p_string);

		File file_to_write(p_string + ".odin");

		//check whether file already exists
		if (file_to_write.existsAsFile()) {
			if (!(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Preset already exists!", "Are you sure you want to overwrite it?", {}, {}, {}))) {
				//user selected cancel
				return;
			}
		}
		FileOutputStream file_stream(file_to_write);
		if (file_stream.openedOk()) {
			savePatchInOpenedFileStream(file_stream);
			DBG("Wrote above patch to " + p_string + ".odin");
			m_patch_selector.regenerateContent();
			m_patch_selector.getSubDirectoryAndHighlightItFromName(p_string);

			const auto soundbank = m_soundbank_selector.getSelectedEntry();
			const auto category  = m_category_selector.getSelectedEntry();
			const auto patch     = m_patch_selector.getSelectedEntry();
			writeSelectedEntriesToValueTree(soundbank, category, patch);
		}
	};

	m_category_selector.onCreateNewFile = [&](String p_string) {
		DBG("create directory: " + p_string);

		File dir_to_create(p_string);

		//check whether directory already exists
		if (dir_to_create.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Category Exists!", "The category you're trying to create already exists!", "Ok");
			return;
		}
		if (dir_to_create.createDirectory()) {
			DBG("Created directory " + p_string);
			m_category_selector.regenerateContent();
			m_category_selector.getSubDirectoryAndHighlightItFromName(p_string);
			m_patch_selector.setDirectory(p_string);
		}
	};

	m_soundbank_selector.onCreateNewFile = [&](String p_string) {
		DBG("create directory: " + p_string);

		File dir_to_create(p_string);

		//check whether directory already exists
		if (dir_to_create.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Soundbank Exists!", "The soundbank you're trying to create already exists!", "Ok");
			return;
		}
		if (dir_to_create.createDirectory()) {
			DBG("Created directory " + p_string);
			m_soundbank_selector.regenerateContent();
			m_soundbank_selector.getSubDirectoryAndHighlightItFromName(p_string);
			m_category_selector.setDirectory(p_string);
			//actually we know this one is empty...
			m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
		}
	};

	m_patch_selector.onExport = [&](String p_directory) {
		File file_suggestion;

		// if (File(p_directory).exists()) {
		// 	file_suggestion = File(p_directory + File::getSeparatorString() + "Preset.odin");
		// } else {
		// 	file_suggestion = File(DEFAULT_EXPORT_LOCATION_STRING + +File::getSeparatorString() + "Preset.odin");
		// }
		auto suggested_dir = ConfigFileManager::getInstance().getOptionPatchDir();
		file_suggestion    = File(suggested_dir + File::getSeparatorString() + "Preset.odin");

		// set up filechooser
		m_filechooser.reset(new FileChooser("Choose a file to save...", file_suggestion, "*.odin", true));

		//launch filechooser
		m_filechooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles, [file_suggestion, this](const FileChooser &chooser) {
			auto result      = chooser.getURLResult();
			String file_name = result.isEmpty() ? String() : (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(true));

			if (file_name != "") {
				//append .odin if not already there
				file_name = file_name.endsWith(".odin") ? file_name : file_name + ".odin";

				File file_to_write(file_name);

				ConfigFileManager::getInstance().setOptionPatchDir(file_to_write.getParentDirectory().getFullPathName());
				ConfigFileManager::getInstance().saveDataToFile();

				//check whether file already exists
				if (file_to_write.existsAsFile()) {
					if (!(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "File already exists!", "Are you sure you want to overwrite it?", {}, {}, {}))) {
						//user selected cancel
						return;
					}
				}
				FileOutputStream file_stream(file_to_write);
				if (file_stream.openedOk()) {
					savePatchInOpenedFileStream(file_stream);
					m_patch_selector.regenerateContent();
				}
			}
		});
	};

	m_patch_selector.onImport = [&](String p_directory) { loadPatchWithFileBrowserAndCopyToCategory(p_directory); };

	m_soundbank_selector.onExport = [&](String p_directory) {
		File soundbank_file(m_category_selector.getDirectory());
		if (!soundbank_file.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "No soundbank selected!", "You appear to not have selected any soundbank.", "Bummer");
		}

		auto suggested_dir = ConfigFileManager::getInstance().getOptionSoundbankDir();
		File file_suggestion(suggested_dir + File::getSeparatorString() + soundbank_file.getFileName() + ".osb");

		// set up filechooser
		m_filechooser.reset(new FileChooser("Choose a file to save...", file_suggestion, "*.osb", true));

		//launch filechooser
		m_filechooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles, [&](const FileChooser &chooser) {
			auto result      = chooser.getURLResult();
			String file_name = result.isEmpty() ? String() : (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(true));

			if (file_name != "") {
				//append .odin if not already there
				file_name = file_name.endsWith(".osb") ? file_name : file_name + ".osb";

				File file_to_write(file_name);

				ConfigFileManager::getInstance().setOptionSoundbankDir(file_to_write.getParentDirectory().getFullPathName());
				ConfigFileManager::getInstance().saveDataToFile();

				//check whether file already exists
				if (file_to_write.existsAsFile()) {
					if (!(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "File already exists!", "Are you sure you want to overwrite it?", {}, {}, {}))) {
						//user selected cancel
						return;
					}
				}

				File soundbank_file_lambda(m_category_selector.getDirectory());
				DBG(soundbank_file_lambda.getFullPathName() + " to");
				DBG(file_to_write.getFullPathName());
				//DBG("name:" + soundbank_file.getFileName());

				FileOutputStream file_stream(file_to_write);
				if (file_stream.openedOk() && soundbank_file_lambda.exists()) {

					ZipFile::Builder zip_builder;

					auto sub_folders = soundbank_file_lambda.findChildFiles(File::TypesOfFileToFind::findDirectories, false);
					for (int sub_index = 0; sub_index < sub_folders.size(); ++sub_index) {
						auto preset_files = sub_folders[sub_index].findChildFiles(File::TypesOfFileToFind::findFiles, false, "*.odin");
						for (int preset_index = 0; preset_index < preset_files.size(); ++preset_index) {
							//DBG(preset_files[preset_index].getFullPathName());
							zip_builder.addFile(preset_files[preset_index], 5, preset_files[preset_index].getRelativePathFrom(soundbank_file_lambda));
						}
					}

					zip_builder.writeToStream(file_stream, nullptr);

					DBG("saved " + file_to_write.getFullPathName());
				}
			}
		});
	};

	m_soundbank_selector.onImport = [&](String p_directory) { loadSoundbankWithFileBrowser(p_directory); };

	m_patch_selector.applyRenamingSelector = [&](String p_directory, String p_old_name, String p_new_name) {
		DBG(p_directory);
		DBG(p_old_name);
		DBG(p_new_name);

		if (p_old_name == p_new_name) {
			return;
		}

		File move_target(p_directory + File::getSeparatorString() + p_new_name + ".odin");
		if (move_target.existsAsFile()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Preset already exists!",
			                            "The preset you're trying to create already exists, please choose another name "
			                            "or remove the other one.",
			                            "Ok");
			return;
		}

		File move_source(p_directory + File::getSeparatorString() + p_old_name + ".odin");
		if (move_source.existsAsFile()) {
			//all set, now move
			move_source.moveFileTo(move_target);
		}

		m_patch_selector.regenerateContent();
	};

	m_category_selector.applyRenamingSelector = [&](String p_directory, String p_old_name, String p_new_name) {
		DBG(p_directory);
		DBG(p_old_name);
		DBG(p_new_name);

		if (p_old_name == p_new_name) {
			return;
		}

		File move_target(p_directory + File::getSeparatorString() + p_new_name);
		if (move_target.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Category already exists!",
			                            "The category you're trying to create already exists, please choose another name "
			                            "or remove the other one.",
			                            "Ok");
			return;
		}
		DBG(p_directory + File::getSeparatorString() + p_old_name);
		DBG(p_directory + File::getSeparatorString() + p_new_name);
		File move_source(p_directory + File::getSeparatorString() + p_old_name);
		if (move_source.isDirectory()) {
			//all set, now move
			if (move_source.copyDirectoryTo(move_target)) {
				move_source.deleteRecursively();
			}
		}

		m_category_selector.regenerateContent();

		m_patch_selector.setDirectory(m_category_selector.getSubDirectoryAndHighlightItFromName(m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_new_name));
	};

	m_soundbank_selector.applyRenamingSelector = [&](String p_directory, String p_old_name, String p_new_name) {
		DBG(p_directory);
		DBG(p_old_name);
		DBG(p_new_name);

		if (p_old_name == p_new_name) {
			return;
		}

		File move_target(p_directory + File::getSeparatorString() + p_new_name);
		if (move_target.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Soundbank already exists!",
			                            "The soundbank you're trying to create already exists, please choose another name "
			                            "or remove the other one.",
			                            "Ok");
			return;
		}
		DBG(p_directory + File::getSeparatorString() + p_old_name);
		DBG(p_directory + File::getSeparatorString() + p_new_name);
		File move_source(p_directory + File::getSeparatorString() + p_old_name);
		if (move_source.isDirectory()) {
			//all set, now move
			if (move_source.copyDirectoryTo(move_target)) {
				move_source.deleteRecursively();
			}
		}

		m_soundbank_selector.regenerateContent();
		m_category_selector.setDirectory(m_soundbank_selector.getSubDirectoryAndHighlightItFromName(DEFAULT_SOUNDBANK_LOCATION_STRING + File::getSeparatorString() + p_new_name));

		m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
	};

	m_soundbank_selector.onMove = [](String p_from, String p_to) {};

	m_category_selector.onMove = [&](String p_cat, String p_target_sb) {
		String source_absolute = m_category_selector.getDirectory() + File::getSeparatorString() + p_cat;
		String target_absolute = DEFAULT_SOUNDBANK_LOCATION_STRING + File::getSeparatorString() + p_target_sb + File::getSeparatorString() + p_cat;

		DBG("Copy category " + source_absolute + " to\n" + target_absolute);

		File move_target(target_absolute);
		if (move_target.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Category already exists!",
			                            "The category you're trying to create already exists, please choose another name "
			                            "or remove the other one.",
			                            "Ok");
			return;
		}

		File move_source(source_absolute);
		if (move_source.isDirectory()) {
			//all set, now move
			if (move_source.copyDirectoryTo(move_target)) {
				move_source.deleteRecursively();
			}
		}

		m_category_selector.regenerateContent();

		m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
	};

	m_patch_selector.onMove = [&](String p_file, String p_target_cat) {
		DBG(p_target_cat);
		DBG("Move Patch " + m_patch_selector.getDirectory() + File::getSeparatorString() + p_file + ".odin to " + m_category_selector.getDirectory() + File::getSeparatorString() +
		    p_target_cat + File::getSeparatorString() + p_file + ".odin");

		File move_target(m_category_selector.getDirectory() + File::getSeparatorString() + p_target_cat + File::getSeparatorString() + p_file + ".odin");

		if (move_target.existsAsFile()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Preset already exists!",
			                            "The preset you're trying to create already exists, please choose another name "
			                            "or remove the other one.",
			                            "Ok");
			return;
		}

		File move_source(m_patch_selector.getDirectory() + File::getSeparatorString() + p_file + ".odin");
		if (move_source.existsAsFile()) {
			//all set, now move
			move_source.moveFileTo(move_target);
		}

		m_patch_selector.regenerateContent();
	};
	m_soundbank_selector.onCopy = [](String p_from, String p_to) {};

	m_category_selector.onCopy = [&](String p_cat, String p_target_sb) {
		String source_absolute = m_category_selector.getDirectory() + File::getSeparatorString() + p_cat;
		String target_absolute = DEFAULT_SOUNDBANK_LOCATION_STRING + File::getSeparatorString() + p_target_sb + File::getSeparatorString() + p_cat;

		DBG("Copy category " + source_absolute + " to\n" + target_absolute);

		File move_target(target_absolute);
		if (move_target.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Category already exists!",
			                            "The category you're trying to create already exists, please choose another name "
			                            "or remove the other one.",
			                            "Ok");
			return;
		}

		File move_source(source_absolute);
		if (move_source.isDirectory()) {
			//all set, now move
			if (move_source.copyDirectoryTo(move_target)) {
				//move_source.deleteRecursively();
			}
		}

		m_category_selector.regenerateContent();

		m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
	};

	m_patch_selector.onCopy = [&](String p_file, String p_target_cat) {
		DBG("Copy Patch " + m_patch_selector.getDirectory() + File::getSeparatorString() + p_file + ".odin to " + m_category_selector.getDirectory() + File::getSeparatorString() +
		    p_target_cat + File::getSeparatorString() + p_file + ".odin");

		File copy_target(m_category_selector.getDirectory() + File::getSeparatorString() + p_target_cat + File::getSeparatorString() + p_file + ".odin");

		if (copy_target.existsAsFile()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Preset already exists!",
			                            "The preset you're trying to create already exists, please choose another name "
			                            "or remove the other one.",
			                            "Ok");
			return;
		}

		File copy_source(m_patch_selector.getDirectory() + File::getSeparatorString() + p_file + ".odin");
		if (copy_source.existsAsFile()) {
			//all set, now move
			if (copy_source.copyFileTo(copy_target)) {
				DBG("Success!");
			}
		}

		m_patch_selector.regenerateContent();
	};

	m_patch_selector.setButtonTooltips("Load a patch from your harddrive", "Export the current patch to your harddrive", "Save the current patch as a preset in this category");
	m_category_selector.setButtonTooltips("", "", "Create a new category for presets in this soundbank");
	m_soundbank_selector.setButtonTooltips("Import an entire soundbank from your harddrive", "Export the highlighted soundbank to your harddrive", "Create a new soundbank");

	m_patch_selector.setWarningTexts("The selected category contains no presets! Create a new one with the \"Save\" button below!",
	                                 "No category was selected. You need to select a category in order to save presets. You can still import / "
	                                 "export presets.");
	m_category_selector.setWarningTexts("The selected soundbank contains no category! Create a new one with the \"New\" button below!", "No soundbank was selected.");
	m_soundbank_selector.setWarningTexts("No soundbank was found on your computer! To use the Factory Presets, please reinstall the plugin, or create a "
	                                     "new Soundbank with the \"New\" button below!",
	                                     "Soundbank folder\n" + DEFAULT_SOUNDBANK_LOCATION_STRING + "\n\nnot found! Please create this folder or reinstall the plugin");
}

PatchBrowser::~PatchBrowser() {
}

void PatchBrowser::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::Presets_back, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void PatchBrowser::loadPatchFromOpenedFileStream(juce::FileInputStream &p_file_stream) {
	//first see if the patch is of a higher version than we know about:
	std::string version_string;
	if (checkForBiggerVersion(p_file_stream, version_string)) {
		//abort with icon
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
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

	//setPatchBrowser to be shown
	m_value_tree.state.getChildWithName("misc").setProperty("arp_mod_selected", MATRIX_SECTION_INDEX_PRESETS, nullptr);

	//note: 99% percent of patch-loading time is spent here:
	juce::ScopedValueSetter<bool> setter(m_is_selected_from_internal, true);
	forceValueTreeLambda();

	//set the correct Version number again
	m_value_tree.state.getChildWithName("misc").setProperty("version_minor", ODIN_MINOR_VERSION, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty("version_patch", ODIN_PATCH_VERSION, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty("patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);
}

bool PatchBrowser::checkForBiggerVersion(FileInputStream &p_file_stream, std::string &p_version_string) {
	p_file_stream.setPosition(0);
	auto value_tree_read = ValueTree::readFromStream(p_file_stream);
	int patch_version    = value_tree_read.getChildWithName("misc")["patch_migration_version"];
	if (patch_version > ODIN_PATCH_MIGRATION_VERSION) {
		p_version_string = "2." + std::to_string((int)value_tree_read.getChildWithName("misc")["version_minor"]) + "." +
		                   std::to_string((int)value_tree_read.getChildWithName("misc")["version_patch"]);
		DBG("Trying to load PMV " + std::to_string(patch_version) + ", current PMV is " + std::to_string(ODIN_PATCH_MIGRATION_VERSION));
		return true;
	}
	return false;
}

bool PatchBrowser::checkForSmallerVersion(FileInputStream &p_file_stream, std::string &p_version_string) {
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

void PatchBrowser::savePatchInOpenedFileStream(FileOutputStream &p_file_stream) {
	// use this to overwrite old content
	p_file_stream.setPosition(0);
	p_file_stream.truncate();

	//write patch name onto valuetree
	m_value_tree.state.getChildWithName("misc").setProperty("patch_name", p_file_stream.getFile().getFileNameWithoutExtension(), nullptr);
	DBG(m_value_tree.state.getChildWithName("misc")["patch_name"].toString());

	//make a deep copy and remove the midi_learn part and file name
	ValueTree copy_with_removed_params = m_value_tree.state.createCopy();
	copy_with_removed_params.removeChild(copy_with_removed_params.getChildWithName("midi_learn"), nullptr);
	copy_with_removed_params.getChildWithName("misc").removeProperty("current_patch_filename", nullptr);
	copy_with_removed_params.getChildWithName("misc").removeProperty("current_patch_directory", nullptr);
	copy_with_removed_params.getChildWithName("misc").removeProperty("arp_mod_selected", nullptr);

	//remove draw osc params if they aren't needed
	for (int osc = 1; osc < 4; ++osc) {
		std::string osc_string = std::to_string(osc);

		if (!usesWavedraw(osc)) {
			//DBG("uses wavedraw " + String(osc));
			for (int step = 0; step < WAVEDRAW_STEPS_X; ++step) {
				copy_with_removed_params.getChildWithName("draw").removeProperty(String("osc" + osc_string + "_wavedraw_values_" + std::to_string(step)), nullptr);
			}
		}
		if (!usesChipdraw(osc)) {
			//DBG("uses chipdraw " + String(osc));
			for (int step = 0; step < CHIPDRAW_STEPS_X; ++step) {
				copy_with_removed_params.getChildWithName("draw").removeProperty(String("osc" + osc_string + "_chipdraw_values_" + std::to_string(step)), nullptr);
			}
		}
		if (!usesSpecdraw(osc)) {
			//DBG("uses specdraw " + String(osc));
			for (int step = 0; step < SPECDRAW_STEPS_X; ++step) {
				copy_with_removed_params.getChildWithName("draw").removeProperty(String("osc" + osc_string + "_specdraw_values_" + std::to_string(step)), nullptr);
			}
		}
	}

	//write valuetree into file
	copy_with_removed_params.writeToStream(p_file_stream);

	//set label
	//m_patch.setText(m_value_tree.state.getChildWithName("misc")["patch_name"].toString().toStdString());

	//save load directory
	//m_last_directory = file_to_write.getParentDirectory().getFullPathName();

	DBG(copy_with_removed_params.toXmlString());

	m_value_tree.state.getChildWithName("misc").setProperty("current_patch_filename", p_file_stream.getFile().getFileName(),
	                                                        nullptr); //needed for up/down buttons in patch loading
	m_value_tree.state.getChildWithName("misc").setProperty("current_patch_directory",
	                                                        p_file_stream.getFile().getParentDirectory().getFullPathName(),
	                                                        nullptr); //needed for up/down buttons in patch loading
	DBG("set filename in valuetree: " + m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
	DBG("set filepath in valuetree: " + m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString());
}

bool PatchBrowser::usesWavedraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) == OSC_TYPE_WAVEDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use wavedraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 600 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 600 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 600 + p_osc) {
				return true;
			}
		}
	}

	//wavedraw can be used by LFOs as well
	for (int lfo = 1; lfo < 5; ++lfo) {
		if ((int)(m_value_tree.state.getChildWithName("lfo")[String("lfo" + std::to_string(lfo) + "_wave")]) == 16 + p_osc) {
			return true;
		}
	}

	return false;
}

bool PatchBrowser::usesChipdraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) == OSC_TYPE_CHIPDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use chipdraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_CHIPTUNE) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_chipwave")]) == 700 + p_osc) {
				return true;
			}
		}
	}

	return false;
}

bool PatchBrowser::usesSpecdraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) == OSC_TYPE_SPECDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use wavedraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 800 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 800 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) == OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 800 + p_osc) {
				return true;
			}
		}
	}

	return false;
}

void PatchBrowser::loadPatchWithFileBrowserAndCopyToCategory(String p_directory) {

	auto suggested_dir = ConfigFileManager::getInstance().getOptionPatchDir();
	File file(suggested_dir);

	m_filechooser.reset(new FileChooser("Choose a file to open...", file, "*.odin", true));

	m_filechooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles, [this](const FileChooser &chooser) {
		String file_name;
		auto results = chooser.getURLResults();

		for (auto result : results)
			file_name << (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(false));

		File file_to_read(file_name);

		FileInputStream file_stream(file_to_read);
		if (file_stream.openedOk()) {

			// First copy the patch to the current category
			if (!m_patch_selector.getDirectory().isEmpty()) {
				String copy_target_string = m_patch_selector.getDirectory() + File::getSeparatorString() + file_to_read.getFileName();
				DBG("Copy Patch " + file_name + " to \n" + copy_target_string);

				File copy_target(copy_target_string);
				ConfigFileManager::getInstance().setOptionPatchDir(file_to_read.getParentDirectory().getFullPathName());
				ConfigFileManager::getInstance().saveDataToFile();

				if (copy_target.existsAsFile()) {
					AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
					                            "Preset already exists!",
					                            "The preset you're trying to import already exists in this category, "
					                            "please choose a different category name "
					                            "or remove the other one.",
					                            "Ok");
					return;
				}

				//all set, now move
				if (file_to_read.copyFileTo(copy_target)) {
					DBG("Success!");
				}

				m_patch_selector.regenerateContent();
				m_patch_selector.getSubDirectoryAndHighlightItFromName(copy_target_string);
			}

			// now actually laod the patch

			loadPatchFromOpenedFileStream(file_stream);

			//save load directory
			//m_last_directory = file_to_read.getParentDirectory().getFullPathName();

			m_value_tree.state.getChildWithName("misc").setProperty("current_patch_filename", file_to_read.getFileName(),
			                                                        nullptr); //needed for up/down buttons in patch loading
			m_value_tree.state.getChildWithName("misc").setProperty("current_patch_directory",
			                                                        file_to_read.getParentDirectory().getFullPathName(),
			                                                        nullptr); //needed for up/down buttons in patch loading
			DBG("set filename in valuetree: " + m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
			DBG("set filepath in valuetree: " + m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString());

		} else {
			if (file_name != "") {
				AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "File not found!", "Path: " + file_name, "Ok");
			}
		}
	});
}

void PatchBrowser::loadSoundbankWithFileBrowser(String p_directory) {
	File file;

	auto suggested_dir = ConfigFileManager::getInstance().getOptionSoundbankDir();
	file               = File(suggested_dir);

	m_filechooser.reset(new FileChooser("Choose a Odin 2 soundbank to open...", file, "*.osb", true));

	m_filechooser->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles, [this](const FileChooser &chooser) {
		String file_name;
		auto results = chooser.getURLResults();

		for (auto result : results)
			file_name << (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(false));

		File file_to_read(file_name);

		FileInputStream file_stream(file_to_read);
		if (file_stream.openedOk()) {

			ConfigFileManager::getInstance().setOptionSoundbankDir(file_to_read.getParentDirectory().getFullPathName());
			ConfigFileManager::getInstance().saveDataToFile();

			String soundbank_name = file_to_read.getFileNameWithoutExtension();

			DBG("Trying to import soundbank \"" + soundbank_name + "\" from location " + file_to_read.getFullPathName() + " to folder\n" + DEFAULT_SOUNDBANK_LOCATION_STRING +
			    File::getSeparatorString() + soundbank_name);

			File dir_to_create(DEFAULT_SOUNDBANK_LOCATION_STRING + File::getSeparatorString() + soundbank_name);

			if (dir_to_create.isDirectory()) {
				AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
				                                 "Soundbank already there!",
				                                 "A soundbank with the same name already exists. Please rename it "
				                                 "or rename the soundbank you're trying to import!",
				                                 "Ok");
				return;
			}

			if (dir_to_create.createDirectory()) {

				ZipFile soundbank_zip(file_stream);
				if (soundbank_zip.uncompressTo(dir_to_create)) {
					m_soundbank_selector.regenerateContent();
					m_category_selector.setDirectory(m_soundbank_selector.getSubDirectoryAndHighlightItFromName(soundbank_name));
					m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
				} else {
					AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Something went wrong when creating the soundbank!", "Error: Couldn't decompress .osb file");
				}

			} else {
				AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Something went wrong when creating the soundbank!", "Error: Couldn't create Directory");
			}

		} else {
			if (file_name != "") {
				AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "File not found!", "Path: " + file_name, "Ok");
			}
		}
	});
}

void PatchBrowser::setFirstSoundbankActive() {
	//factory presets:
	m_soundbank_selector.highlightFirstEntry();
	m_category_selector.setDirectoryFactoryPresetCategory();
	m_category_selector.highlightFirstEntry();
	m_patch_selector.setDirectoryFactoryPresetPreset("Arps & Sequences");
}

void PatchBrowser::resized() {

	const auto grid = ConfigFileManager::getInstance().getOptionGuiScale();

	m_soundbank_selector.setBounds(juce::Rectangle<int>(grid * 2, grid * 2, grid * 42, grid * 34));
	m_category_selector.setBounds(juce::Rectangle<int>(grid * 45, grid * 2, grid * 37, grid * 34));
	m_patch_selector.setBounds(juce::Rectangle<int>(grid * 83, grid * 2, grid * 37, grid * 34));

	setFirstSoundbankActive();
}

void PatchBrowser::writeSelectedEntriesToValueTree(const juce::String &p_soundbank, const juce::String &p_category, const juce::String &p_patch) {
	auto soundbank = p_soundbank;
	if (soundbank.isEmpty())
		soundbank = "Static Factory Presets";

	m_value_tree.state.getChildWithName("misc").setProperty("preset_soundbank_selected", soundbank, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty("preset_category_selected", p_category, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty("preset_patch_selected", p_patch, nullptr);

	DBG("Write to value tree:");
	DBG(soundbank);
	DBG(p_category);
	DBG(p_patch);
}

void PatchBrowser::setSelectedEntriesFromValueTree() {
	if (m_is_selected_from_internal) {
		return;
	}

	auto soundbank = m_value_tree.state.getChildWithName("misc")["preset_soundbank_selected"].toString();
	auto category  = m_value_tree.state.getChildWithName("misc")["preset_category_selected"].toString();
	auto patch     = m_value_tree.state.getChildWithName("misc")["preset_patch_selected"].toString();
	if (patch.startsWith(FACTORY_PRESETS_SOUNDBANK_CODE))
		patch = patch.fromFirstOccurrenceOf(FACTORY_PRESETS_SOUNDBANK_CODE, false, true);
	if (patch.endsWith(".odin"))
		patch = patch.upToFirstOccurrenceOf(".odin", false, true);

	const auto is_factory = (soundbank == "Static Factory Presets");
	DBG("PresetEntries in value tree:");
	DBG(soundbank);
	DBG(category);
	DBG(patch);

	// soundbank
	auto dir = DEFAULT_SOUNDBANK_LOCATION_STRING;
	m_soundbank_selector.setDirectory(dir);
	if (!m_soundbank_selector.highlightSelectedEntryIfPossible(soundbank)) {
		return;
	}

	// category
	if (is_factory) {
		m_category_selector.setDirectoryFactoryPresetCategory();
	} else {
		dir += File::getSeparatorString() + soundbank;
		m_category_selector.setDirectory(dir);
	}
	if (!m_category_selector.highlightSelectedEntryIfPossible(category)) {
		return;
	}

	// patch
	if (is_factory) {
		m_patch_selector.setDirectoryFactoryPresetPreset(category.toStdString());
	} else {
		dir += File::getSeparatorString() + category;
		m_patch_selector.setDirectory(dir);
	}
	m_patch_selector.highlightSelectedEntryIfPossible(patch);
}
