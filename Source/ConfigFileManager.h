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

#pragma once

#include "GlobalIncludes.h"

#define DEFAULT_TUNING_DIRECTORY (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName())
#define DEFAULT_SOUNDBANK_IO_LOCATION_STRING (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName())
#define DEFAULT_PATCH_LOCATION_STRING (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName())

#define XML_ATTRIBUTE_ODIN_CONFIG ("odin_config")
#define XML_ATTRIBUTE_BIG_GUI ("big_gui")
#define XML_ATTRIBUTE_TUNING_DIR ("tuning_dir")
#define XML_ATTRIBUTE_SOUNDBANK_DIR ("soundbank_dir")
#define XML_ATTRIBUTE_PATCH_DIR ("patch_dir")
#define XML_ATTRIBUTE_GUI_SCALE ("gui_scale")
#define XML_ATTRIBUTE_SHOW_TOOLTIP ("show_tooltip")
#define XML_ATTRIBUTE_GUI_OPEN ("num_gui_opens")
#define XML_ATTRIBUTE_SPLINE_AD1 ("spline_ad1_seen")
#define XML_ATTRIBUTE_SPLINE_AD2 ("spline_ad2_seen")

/**
 * This class manages the config file. Data is loaded from the file in constructor if possible. Then the individual 
 * attributes can be written and read. After writing, the state must be explicitly saved to file.
 */
class ConfigFileManager {
public:
	static ConfigFileManager &getInstance() {
		static ConfigFileManager instance;
		return instance;
	}
	ConfigFileManager(ConfigFileManager const &) = delete;
	void operator=(ConfigFileManager const &)    = delete;

	void saveDataToFile();

	void setOptionBigGUI(bool p_GUI_big);
	bool getOptionBigGUI();
	void setOptionShowTooltip(bool p_show);
	bool getOptionShowTooltip();
	void setOptionGuiScale(int p_scale);
	int getOptionGuiScale();
	void setOptionTuningDir(String p_dir);
	String getOptionTuningDir();
	void setOptionSoundbankDir(String p_dir);
	String getOptionSoundbankDir();
	void setOptionPatchDir(String p_dir);
	String getOptionPatchDir();
	int getNumGuiOpens();
	void incrementNumGuiOpens();
	void setOptionSplineAd1Seen(bool p_seen);
	bool getOptionSplineAd1Seen();
	void setOptionSplineAd2Seen(bool p_seen);
	bool getOptionSplineAd2Seen();

private:
	ConfigFileManager();

	int m_gui_scale = int(GuiScale::Z150);
	void createDirIfNeeded();
	void loadDataFromFile();
	//the actual config values with default values
	bool m_big_gui          = true;
	bool m_show_tooltip     = false;
	bool m_spline_ad_1_seen = false;
	bool m_spline_ad_2_seen = false;

	String m_tuning_dir     = DEFAULT_TUNING_DIRECTORY;
	String m_soundbank_dir  = DEFAULT_SOUNDBANK_IO_LOCATION_STRING;
	String m_patch_dir      = DEFAULT_PATCH_LOCATION_STRING;

	int m_num_gui_opens = 0;
};
