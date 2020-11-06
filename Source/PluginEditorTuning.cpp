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
}

void OdinAudioProcessorEditor::importKBMFile() {
	DBG("importKBMFile()");
}

void OdinAudioProcessorEditor::exportSCLFile() {
	DBG("exportSCLFile()");

	DBG(m_processor.m_tuning.scale.rawText);
}

void OdinAudioProcessorEditor::exportKBMFile() {
	DBG("exportKBMFile()");
	DBG(m_processor.m_tuning.keyboardMapping.rawText);
}

void OdinAudioProcessorEditor::restoreSCL() {
	DBG("restoreSCL()");
}

void OdinAudioProcessorEditor::restoreKBM() {
	DBG("restoreKBM()");
}
