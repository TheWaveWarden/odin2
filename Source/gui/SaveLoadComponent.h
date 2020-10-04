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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlasDisplay.h"
#include "../GlobalIncludes.h"
#include "../PluginProcessor.h"

#define SAVE_POS_X 6
#define SAVE_POS_Y LOAD_POS_Y
#define PATCH_POS_X 40
#define PATCH_POS_Y 4
#define UP_DOWN_BUTTON_X 129
#define UP_BUTTON_Y 6
#define DOWN_BUTTON_Y 14
#define LOAD_POS_X 152
#define LOAD_POS_Y 5
#define RESET_TOP_POS_X 186
#define RESET_TOP_POS_Y LOAD_POS_Y
#define RANDOM_POS_X 218
#define RANDOM_POS_Y LOAD_POS_Y

class SaveLoadComponent : public Component {
public:
	SaveLoadComponent(AudioProcessorValueTreeState &vts, OdinAudioProcessor &p_processor);
	~SaveLoadComponent();

	void paint(Graphics &g) override {
		if (m_GUI_big) {
			//draw an outline
			g.setColour(juce::Colour(30, 30, 30));
			juce::Rectangle<int> rect = getLocalBounds();
			rect.setX(UP_DOWN_BUTTON_X - 3);
			rect.setY(UP_BUTTON_Y - 3);
			rect.setHeight(rect.getHeight() - 3);
			rect.setWidth(30);
			g.drawRect(rect, 2);
		}
	}
	void forceValueTreeOntoComponents(ValueTree p_tree);

	std::function<void()> forceValueTreeLambda;

	void resetPatchText() {
		m_patch.setText(m_value_tree.state.getChildWithName("misc")["patch_name"].toString().toStdString());
	}

	void loadPatchFromOpenedFileStream(juce::FileInputStream &p_file);

	void setGUIBig();
	void setGUISmall();

private:
	bool m_GUI_big = false;

	void loadPatchWithFileBrowser();
	bool checkForBiggerVersion(FileInputStream &p_file_stream, std::string &p_version_string);
	bool checkForSmallerVersion(FileInputStream &p_file_stream, std::string &p_version_string);
	void versionMigrate();
	void incrementPatch();
	void decrementPatch();

	bool usesWavedraw(int p_osc);
	bool usesChipdraw(int p_osc);
	bool usesSpecdraw(int p_osc);

	OdinAudioProcessor &m_audio_processor;

	std::unique_ptr<FileChooser> m_filechooser;
	AudioProcessorValueTreeState &m_value_tree;

	//String m_last_directory;

	juce::DrawableButton m_save;
	juce::DrawableButton m_load;
	juce::DrawableButton m_reset;
	//juce::DrawableButton m_random;

	juce::DrawableButton m_up;
	juce::DrawableButton m_down;

	GlasDisplay m_patch;
	int m_patch_size_x;
	int m_patch_size_y;

	OdinMenuFeels m_menu_feels;
	juce::PopupMenu m_patch_dropdown_menu;

	FileElementComparatorAlphabetical m_file_comparator;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaveLoadComponent)
};
