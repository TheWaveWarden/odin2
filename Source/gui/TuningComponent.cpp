/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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

#include "TuningComponent.h"

TuningComponent::TuningComponent(OdinAudioProcessor &p_processor) :
    m_processor(p_processor), m_tuning_dropdown("Tuning") {
	m_tuning_dropdown.setInlay(1);
	m_tuning_dropdown.setEditableText(false);
	m_tuning_dropdown.showTriangle();
	m_tuning_dropdown.setColor(juce::STANDARD_DISPLAY_COLOR);
	m_tuning_dropdown.setTooltip(
	    "Load or export custom tunings. A tuning is comprised of a .scl file a .kbm file. The .kbm file maps keys on "
	    "the keyboard to arbitrary note indices and sets the base note. The .scl file maps those indices to actual "
	    "frequencies in relation to the base note.");
	m_tuning_dropdown.addItem("Import .scl file", TUNING_IMPORT_SCL);
	m_tuning_dropdown.addItem("Import .kbm file", TUNING_IMPORT_KBM);
	m_tuning_dropdown.addSeparator();
	m_tuning_dropdown.addItem("Export current .scl file", TUNING_EXPORT_SCL);
	m_tuning_dropdown.addItem("Export current .kbm file", TUNING_EXPORT_KBM);
	m_tuning_dropdown.addSeparator();
	m_tuning_dropdown.addItem("Reset to standard .scl", TUNING_RESTORE_SCL);
	m_tuning_dropdown.addItem("Reset to standard .kbm", TUNING_RESTORE_KBM);
	m_tuning_dropdown.addSeparator();
	m_tuning_dropdown.addItem("Reset entire tuning", TUNING_RESET_ENTIRE_TUNING);
	m_tuning_dropdown.onChange = [&]() {
		DBG_VAR(m_tuning_dropdown.getSelectedId());
		switch (m_tuning_dropdown.getSelectedId()) {
		case TUNING_IMPORT_SCL:
			importSCLFile();
			break;
		case TUNING_IMPORT_KBM:
			importKBMFile();
			break;
		case TUNING_EXPORT_SCL:
			exportSCLFile();
			break;
		case TUNING_EXPORT_KBM:
			exportKBMFile();
			break;
		case TUNING_RESTORE_SCL:
			restoreSCL();
			break;
		case TUNING_RESTORE_KBM:
			restoreKBM();
			break;
		case TUNING_RESET_ENTIRE_TUNING:
			resetEntireTuning();
		default:
			break;
		}

		//reset id, so the checkmark doesn't appear on next dropdown
		m_tuning_dropdown.setSelectedId(-1, NotificationType::dontSendNotification);
	};
	addAndMakeVisible(m_tuning_dropdown);
}

void TuningComponent::importSCLFile() {
	DBG("importSCLFile()");
	importSCLFromFileBrowser("",
	                         "*.scl",
	                         "Load Scala tuning file...",
	                         FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles);
}

void TuningComponent::importKBMFile() {
	DBG("importKBMFile()");
	importKBMFromFileBrowser("",
	                         "*.kbm",
	                         "Load KBM keyboard file...",
	                         FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles);
}

void TuningComponent::exportSCLFile() {
	DBG("exportSCLFile()");

	DBG(m_processor.m_tuning.scale.rawText);
	//todo

	File file_suggestion(m_scl_import_dir + File::getSeparatorString() + "tuning.scl");

	// set up filechooser
	m_filechooser.reset(new FileChooser("Choose a file to save...", file_suggestion, "*.odin", true));

	//launch filechooser
	m_filechooser->launchAsync(FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
	                           [file_suggestion, this](const FileChooser &chooser) {
		                           auto result      = chooser.getURLResult();
		                           String file_name = result.isEmpty() ? String()
		                                                               : (result.isLocalFile()
		                                                                      ? result.getLocalFile().getFullPathName()
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
				                           DBG("Saving tuning file in " << file_to_write.getFullPathName());
				                           //savePatchInOpenedFileStream(file_stream);
			                           }
		                           }
	                           });
}

void TuningComponent::exportKBMFile() {
	DBG("exportKBMFile()");
	DBG(m_processor.m_tuning.keyboardMapping.rawText);
	//todo
}

void TuningComponent::restoreSCL() {
	DBG("restoreSCL()");
	m_processor.m_tuning = Tunings::Tuning(Tunings::evenTemperament12NoteScale(), m_processor.m_tuning.keyboardMapping);
}

void TuningComponent::restoreKBM() {
	DBG("restoreKBM()");
	m_processor.m_tuning =
	    Tunings::Tuning(m_processor.m_tuning.scale, Tunings::tuneNoteTo(60, Tunings::MIDI_0_FREQ * 32.0));
}

void TuningComponent::resetEntireTuning() {
	m_processor.m_tuning = Tunings::Tuning(Tunings::tuneNoteTo(60, Tunings::MIDI_0_FREQ * 32.0));
}

void TuningComponent::importKBMFromFileBrowser(String p_directory,
                                               String p_extension,
                                               String p_title_text,
                                               int p_flags) {

	File file(m_kbm_import_dir);

	m_filechooser.reset(new FileChooser(p_title_text, file, p_extension, true));

	m_filechooser->launchAsync(p_flags, [this](const FileChooser &chooser) {
		String file_name;
		auto results = chooser.getURLResults();

		for (auto result : results)
			file_name << (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(false));

		DBG_VAR(file_name);

		// dialog was cancelled?
		if (file_name == "") {
			return;
		}

		// store filename for next opening
		File file(file_name);
		m_kbm_import_dir = file.getParentDirectory().getFullPathName().toStdString();

		Tunings::KeyboardMapping kbm_from_file;
		try {
			kbm_from_file = Tunings::readKBMFile(file_name.toStdString());
		} catch (...) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Unable to read Scala!",
			                            "Please make sure the file is a proper keyboard mapping file!");
			return;
		}

		m_processor.m_tuning = Tunings::Tuning(m_processor.m_tuning.scale, kbm_from_file);
	});
}

void TuningComponent::importSCLFromFileBrowser(String p_directory,
                                               String p_extension,
                                               String p_title_text,
                                               int p_flags) {

	File file(m_scl_import_dir);

	m_filechooser.reset(new FileChooser(p_title_text, file, p_extension, true));

	m_filechooser->launchAsync(p_flags, [this](const FileChooser &chooser) {
		String file_name;
		auto results = chooser.getURLResults();

		for (auto result : results)
			file_name << (result.isLocalFile() ? result.getLocalFile().getFullPathName() : result.toString(false));

		DBG_VAR(file_name);

		// dialog was cancelled?
		if (file_name == "") {
			return;
		}

		// store filename for next opening
		File file(file_name);
		m_scl_import_dir = file.getParentDirectory().getFullPathName().toStdString();

		Tunings::Scale scale_from_file;
		try {
			scale_from_file = Tunings::readSCLFile(file_name.toStdString());
		} catch (...) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
			                            "Unable to read Scala file!",
			                            "Please make sure the file is a proper Scala file!");
			return;
		}

		m_processor.m_tuning =
		    Tunings::Tuning(Tunings::readSCLFile(file_name.toStdString()), m_processor.m_tuning.keyboardMapping);
	});
}

void TuningComponent::setGUIBig() {
	m_tuning_dropdown.setGUIBig();

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_150_png, BinaryData::glaspanel_midbig_150_pngSize);
	m_tuning_dropdown.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_tuning_dropdown.setImage(glas_panel);
	m_tuning_dropdown.setInlay(1);
	setSize(m_tuning_dropdown.getWidth(), m_tuning_dropdown.getHeight());
}
void TuningComponent::setGUISmall() {
	m_tuning_dropdown.setGUISmall();

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_png, BinaryData::glaspanel_midbig_pngSize);
	m_tuning_dropdown.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_tuning_dropdown.setImage(glas_panel);
	m_tuning_dropdown.setInlay(1);
	setSize(m_tuning_dropdown.getWidth(), m_tuning_dropdown.getHeight());
}