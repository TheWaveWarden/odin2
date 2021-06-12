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
		if(result.wasOk()){
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
				forEachXmlChildElement(*config_xml.get(), child) {
					if (child->hasTagName(XML_ATTRIBUTE_BIG_GUI)) {
						DBG("Found Config Element: " << XML_ATTRIBUTE_BIG_GUI << ": "
						                             << (int)child->getBoolAttribute("data"));
						m_big_gui = child->getBoolAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_TUNING_DIR)) {
						DBG("Found Config Element: " << XML_ATTRIBUTE_TUNING_DIR << ": "
						                             << child->getStringAttribute("data"));
						m_tuning_dir = child->getStringAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_SOUNDBANK_DIR)) {
						DBG("Found Config Element: " << XML_ATTRIBUTE_SOUNDBANK_DIR << ": "
						                             << child->getStringAttribute("data"));
						m_soundbank_dir = child->getStringAttribute("data");
					} else if (child->hasTagName(XML_ATTRIBUTE_PATCH_DIR)) {
						DBG("Found Config Element: " << XML_ATTRIBUTE_PATCH_DIR << ": "
						                             << child->getStringAttribute("data"));
						m_patch_dir = child->getStringAttribute("data");
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

	XmlElement *tuning_dir = new XmlElement(XML_ATTRIBUTE_TUNING_DIR);
	tuning_dir->setAttribute("data", m_tuning_dir);
	config_xml->addChildElement(tuning_dir);

	XmlElement *soundbank_dir = new XmlElement(XML_ATTRIBUTE_SOUNDBANK_DIR);
	soundbank_dir->setAttribute("data", m_soundbank_dir);
	config_xml->addChildElement(soundbank_dir);

	XmlElement *patch_dir = new XmlElement(XML_ATTRIBUTE_PATCH_DIR);
	patch_dir->setAttribute("data", m_patch_dir);
	config_xml->addChildElement(patch_dir);

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
