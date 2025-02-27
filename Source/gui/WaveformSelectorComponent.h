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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlasDisplay.h"
#include "OdinButton.h"
#include <map>

#define N_SUBMENUS 5

class WaveformSelectorComponent : public Component {
public:
	WaveformSelectorComponent() = delete;
	WaveformSelectorComponent(bool p_buttons_right);
	~WaveformSelectorComponent();

	void paint(Graphics &) override;
	void resized() override;

	void setTooltip(const String p_text) {
		m_display.setTooltip(p_text);
	}

	void setColor(juce::Colour p_color) {
		m_display.setColor(p_color);
	}

	void addWave(int p_index, const std::string &p_name) {
		m_menu.addItem(p_index, p_name);
		m_menu_map.insert(std::pair<int, std::string>(p_index, p_name));
		m_inc_map.insert(std::pair<int, int>(p_index, p_index + 1));
		m_dec_map.insert(std::pair<int, int>(p_index, p_index - 1));
	}

	void addWaveToSubmenu(int p_index, const std::string &p_name, int p_submenu) {
		if (p_submenu >= 0 && p_submenu < N_SUBMENUS) {
			m_submenu[p_submenu].addItem(p_index, p_name);
			m_menu_map.insert(std::pair<int, std::string>(p_index, p_name));
			m_inc_map.insert(std::pair<int, int>(p_index, p_index + 1));
			m_dec_map.insert(std::pair<int, int>(p_index, p_index - 1));
		}
	}

	void addSeparator() {
		m_menu.addSeparator();
	}

	void addMapEntry(const std::string &p_name, int p_index, int p_next, int p_last) {
		m_menu_map.insert(std::pair<int, std::string>(p_index, p_name));
		m_inc_map.insert(std::pair<int, int>(p_index, p_next));
		m_dec_map.insert(std::pair<int, int>(p_index, p_last));
	}

	void applySubmenu(int p_submenu, const std::string &p_name) {
		if (p_submenu >= 0 && p_submenu < N_SUBMENUS) {
			m_menu.addSubMenu(p_name, m_submenu[p_submenu]);
		}
	}

	void setIncrementValue(int p_index, int p_next_value) {
		auto it = m_inc_map.find(p_index);
		if (it != m_inc_map.end()) {
			m_inc_map.erase(it);
		}
		m_inc_map.insert(std::pair<int, int>(p_index, p_next_value));
	}

	void setDecrementValue(int p_index, int p_next_value) {
		auto it = m_dec_map.find(p_index);
		if (it != m_dec_map.end()) {
			m_dec_map.erase(it);
		}
		m_dec_map.insert(std::pair<int, int>(p_index, p_next_value));
	}

	void setValue(int p_value) {
		auto it = m_menu_map.find(p_value);
		if (it != m_menu_map.end()) {
			m_display.setText(it->second);
			m_value = p_value;
			OnValueChange(m_value);
		}
	}

	std::function<void(int)> OnValueChange = [](int) {};

	juce::PopupMenu m_menu;

private:
	void increment() {
		auto it = m_inc_map.find(m_value);
		if (it != m_inc_map.end()) {
			setValue(it->second);
		}
	}

	void decrement() {
		auto it = m_dec_map.find(m_value);
		if (it != m_dec_map.end()) {
			setValue(it->second);
		}
	}

	GlasDisplay m_display;
	juce::PopupMenu m_submenu[N_SUBMENUS];
	std::map<int, std::string> m_menu_map; //to get string names from menu indices
	std::map<int, int> m_inc_map;          //which value to go to on increment()
	std::map<int, int> m_dec_map;          //which value to go to on decrement()

	OdinButton m_up;
	OdinButton m_down;

	int m_value = 0;
	bool m_buttons_right;
	int m_display_width;
	OdinMenuFeels m_menu_feels;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformSelectorComponent)
};
