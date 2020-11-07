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

#include "PluginEditor.h"

void OdinAudioProcessorEditor::importSCLFile() {
	DBG("importSCLFile()");
	importSCLFromFileBrowser("",
	                         "*.scl",
	                         "Load Scala tuning file...",
	                         FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles);
}

void OdinAudioProcessorEditor::importKBMFile() {
	DBG("importKBMFile()");
	importSCLFromFileBrowser("",
	                         "*.kbm",
	                         "Load KBM keyboard file...",
	                         FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles);
}

void OdinAudioProcessorEditor::exportSCLFile() {
	DBG("exportSCLFile()");

	DBG(m_processor.m_tuning.scale.rawText);
	//todo
}

void OdinAudioProcessorEditor::exportKBMFile() {
	DBG("exportKBMFile()");
	DBG(m_processor.m_tuning.keyboardMapping.rawText);
	//todo
}

void OdinAudioProcessorEditor::restoreSCL() {
	DBG("restoreSCL()");
	m_processor.m_tuning = Tunings::Tuning(Tunings::evenTemperament12NoteScale(), m_processor.m_tuning.keyboardMapping);
}

void OdinAudioProcessorEditor::restoreKBM() {
	DBG("restoreKBM()");
	m_processor.m_tuning = Tunings::Tuning(m_processor.m_tuning.scale, Tunings::tuneNoteTo( 60, Tunings::MIDI_0_FREQ * 32.0 ));
}

void OdinAudioProcessorEditor::resetEntireTuning() {
	m_processor.m_tuning = Tunings::Tuning(Tunings::tuneNoteTo( 60, Tunings::MIDI_0_FREQ * 32.0 ));
}

void OdinAudioProcessorEditor::importSCLFromFileBrowser(String p_directory,
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
			                            "Unable to read keyboard mapping!",
			                            "Please make sure the file is a proper keyboard mapping file!");
			return;
		}

		m_processor.m_tuning =
		    Tunings::Tuning(m_processor.m_tuning.scale, kbm_from_file);
	});
}

void OdinAudioProcessorEditor::importKBMFromFileBrowser(String p_directory,
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
