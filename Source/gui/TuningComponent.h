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

#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"
#include "../PluginProcessor.h"
#include "FixedTextGlassDropdown.h"

#define TUNING_IMPORT_SCL 10
#define TUNING_IMPORT_KBM 50
#define TUNING_EXPORT_SCL 100
#define TUNING_EXPORT_KBM 150
#define TUNING_RESTORE_SCL 1000
#define TUNING_RESTORE_KBM 1050
#define TUNING_RESET_ENTIRE_TUNING 2000

class TuningComponent : public Component {
public:
	TuningComponent(OdinAudioProcessor &processor);

    void resized() override;

protected:
	void importSCLFile();
	void importKBMFile();
	void exportSCLFileWithFileBrowser();
	void exportKBMFileWithFileBrowser();
	void restoreSCL();
	void restoreKBM();
	void resetEntireTuning();
	void importSCLFromFileBrowser(String p_directory, String p_extension, String p_title_text, int p_flags);
	void importKBMFromFileBrowser(String p_directory, String p_extension, String p_title_text, int p_flags);

	FixedTextGlassDropdown m_tuning_dropdown;
    OdinAudioProcessor &m_processor;

	std::unique_ptr<FileChooser> m_filechooser;
};