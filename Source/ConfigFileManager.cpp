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

#include "ConfigFileManager.h"

ConfigFileManager::ConfigFileManager() {
	loadDataFromFile();

	createDirIfNeeded();
}

void ConfigFileManager::createDirIfNeeded() {
	//actually this should be done already by the installer
	File storage_path(ODIN_STORAGE_PATH);
	if (!storage_path.isDirectory()) {
		DBG("Storage path" << ODIN_STORAGE_PATH " was not found! Try to create it...");
		auto result = storage_path.createDirectory();
		if (result.wasOk()) {
			DBG("Sucessfully created storage path");
		} else {
			DBG("Failed to create storage path");
		}
	}
}

void ConfigFileManager::loadDataFromFile() {
	String path_absolute    = CONFIG_FILE_PATH;
	File configuration_file = File(path_absolute);

	if (configuration_file.existsAsFile()) {
		auto config_xml = XmlDocument::parse(configuration_file);

		//parse() returns nullptr for any problem
		if (config_xml.get()) {
			if (config_xml->hasTagName(XML_ATTRIBUTE_ODIN_CONFIG)) {
				for (auto *child : config_xml->getChildIterator()) {
					if (child->hasTagName(XML_ATTRIBUTE_BIG_GUI)) {
						m_big_gui = child->getBoolAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_SPLINE_AD1)) {
						m_spline_ad_1_seen = child->getBoolAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_SPLINE_AD2)) {
						m_spline_ad_2_seen = child->getBoolAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_SHOW_TOOLTIP)) {
						m_show_tooltip = child->getBoolAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_TUNING_DIR)) {
						m_tuning_dir = child->getStringAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_SOUNDBANK_DIR)) {
						m_soundbank_dir = child->getStringAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_PATCH_DIR)) {
						m_patch_dir = child->getStringAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_GUI_SCALE)) {
						m_gui_scale = std::stoi(child->getStringAttribute("data").toStdString());
					} else if (child->hasTagName(XML_ATTRIBUTE_GUI_OPEN)) {
						m_num_gui_opens = std::stoi(child->getStringAttribute("data").toStdString());
					}
				}
			}
		} else {
			DBG("ConfigFile exists, but could not be parsed!");
		}
	} else {
		DBG("ConfigFile does not exist!");
	}
}

void ConfigFileManager::saveDataToFile() {
	std::unique_ptr<XmlElement> config_xml = std::make_unique<XmlElement>(XML_ATTRIBUTE_ODIN_CONFIG);

	XmlElement *big_gui = new XmlElement(XML_ATTRIBUTE_BIG_GUI);
	big_gui->setAttribute("data", m_big_gui);
	config_xml->addChildElement(big_gui);

	XmlElement *spline_ad1 = new XmlElement(XML_ATTRIBUTE_SPLINE_AD1);
	spline_ad1->setAttribute("data", m_spline_ad_1_seen);
	config_xml->addChildElement(spline_ad1);

	XmlElement *spline_ad2 = new XmlElement(XML_ATTRIBUTE_SPLINE_AD2);
	spline_ad2->setAttribute("data", m_spline_ad_2_seen);
	config_xml->addChildElement(spline_ad2);

	XmlElement *show_tooltip = new XmlElement(XML_ATTRIBUTE_SHOW_TOOLTIP);
	show_tooltip->setAttribute("data", m_show_tooltip);
	config_xml->addChildElement(show_tooltip);

	XmlElement *tuning_dir = new XmlElement(XML_ATTRIBUTE_TUNING_DIR);
	tuning_dir->setAttribute("data", m_tuning_dir);
	config_xml->addChildElement(tuning_dir);

	XmlElement *soundbank_dir = new XmlElement(XML_ATTRIBUTE_SOUNDBANK_DIR);
	soundbank_dir->setAttribute("data", m_soundbank_dir);
	config_xml->addChildElement(soundbank_dir);

	XmlElement *patch_dir = new XmlElement(XML_ATTRIBUTE_PATCH_DIR);
	patch_dir->setAttribute("data", m_patch_dir);
	config_xml->addChildElement(patch_dir);

	XmlElement *gui_scale = new XmlElement(XML_ATTRIBUTE_GUI_SCALE);
	gui_scale->setAttribute("data", std::to_string(m_gui_scale));
	config_xml->addChildElement(gui_scale);

	XmlElement *num_gui_open = new XmlElement(XML_ATTRIBUTE_GUI_OPEN);
	num_gui_open->setAttribute("data", std::to_string(m_num_gui_opens));
	config_xml->addChildElement(num_gui_open);

	//DBG_VAR(config_xml->toString());

	String path_absolute    = CONFIG_FILE_PATH;
	File configuration_file = File(path_absolute);

	if (config_xml->writeTo(configuration_file)) {
		DBG("Wrote configuration Xml to " << path_absolute);
	} else {
		DBG("Failed to write configuration file!");
	}
}

void ConfigFileManager::setOptionBigGUI(bool p_GUI_big) {
	m_big_gui = p_GUI_big;
}

bool ConfigFileManager::getOptionBigGUI() {
	return m_big_gui;
}

void ConfigFileManager::setOptionShowTooltip(bool p_show) {
	m_show_tooltip = p_show;
}

bool ConfigFileManager::getOptionShowTooltip() {
	return m_show_tooltip;
}

void ConfigFileManager::setOptionSplineAd1Seen(bool p_seen) {
	m_spline_ad_1_seen = p_seen;
}

bool ConfigFileManager::getOptionSplineAd1Seen() {
	return m_spline_ad_1_seen;
}

void ConfigFileManager::setOptionSplineAd2Seen(bool p_seen) {
	m_spline_ad_2_seen = p_seen;
}

bool ConfigFileManager::getOptionSplineAd2Seen() {
	return m_spline_ad_2_seen;
}

void ConfigFileManager::setOptionGuiScale(int p_scale) {
	m_gui_scale = p_scale;
}

int ConfigFileManager::getOptionGuiScale() {
	return m_gui_scale;
}

int ConfigFileManager::getNumGuiOpens() {
	return m_num_gui_opens;
}

void ConfigFileManager::incrementNumGuiOpens() {
	m_num_gui_opens++;
	DBG_VAR(m_num_gui_opens);
}

void ConfigFileManager::setOptionTuningDir(String p_dir) {
	m_tuning_dir = p_dir;
}

String ConfigFileManager::getOptionTuningDir() {
	return m_tuning_dir;
}

void ConfigFileManager::setOptionSoundbankDir(String p_dir) {
	m_soundbank_dir = p_dir;
}

String ConfigFileManager::getOptionSoundbankDir() {
	return m_soundbank_dir;
}

void ConfigFileManager::setOptionPatchDir(String p_dir) {
	m_patch_dir = p_dir;
}

String ConfigFileManager::getOptionPatchDir() {
	return m_patch_dir;
}
