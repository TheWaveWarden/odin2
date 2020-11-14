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
	DBG("ConfigFileManager()");
	loadDataFromFile();
}

void ConfigFileManager::loadDataFromFile() {
	DBG("loadDataFromFile()");
	String path_absolute    = CONFIG_FILE_PATH;
	File configuration_file = File(path_absolute);

	if (configuration_file.existsAsFile()) {
		auto config_xml = XmlDocument::parse(configuration_file);

		//parse() returns nullptr for any problem
		if (config_xml.get()) {
			DBG_VAR(config_xml->toString());
			if (config_xml->hasTagName(XML_ATTRIBUTE_ODIN_CONFIG)) {
				DBG("Found Odin Config Xml Tag");
				forEachXmlChildElement(*config_xml.get(), child) {
					if (child->hasTagName(XML_ATTRIBUTE_BIG_GUI)) {
						DBG("Found Config Element: " << XML_ATTRIBUTE_BIG_GUI << ": "
						                             << (int)child->getBoolAttribute("data"));
						m_big_gui = child->getBoolAttribute("data");
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

	DBG_VAR(config_xml->toString());

	String path_absolute    = CONFIG_FILE_PATH;
	File configuration_file = File(path_absolute);

	if (config_xml->writeTo(configuration_file)) {
		DBG("Wrote configuration Xml to " << path_absolute);
		DBG_VAR(config_xml->toString());
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
