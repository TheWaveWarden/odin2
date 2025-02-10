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

#include "NumberSelector.h"

#include <JuceHeader.h>

//==============================================================================
/*
*/
class NumberSelectorWithText : public NumberSelector {
public:
	NumberSelectorWithText(NumberSelector::Type p_type = NumberSelector::Type::selector_14x4);
	~NumberSelectorWithText();

	virtual void increment() override;
	virtual void decrement() override;
	virtual void setValue(int p_value) override;

	std::function<std::string(int)> valueToText = [](int p_value) { return std::to_string(p_value); };

	void setIncrementMap(std::map<int, int> p_map) {
		m_increment_map = p_map;
	}

	void setDecrementMap(std::map<int, int> p_map) {
		m_decrement_map = p_map;
	}

	void setLegalValues(std::vector<int> p_vector) {
		m_legal_values = p_vector;
		std::map<int, int> inc_map;
		std::map<int, int> dec_map;
		for (int i = 0; i < p_vector.size(); ++i) {

			if (i == p_vector.size() - 1) {
				inc_map.insert({p_vector[i], p_vector[i]});
			} else {
				inc_map.insert({p_vector[i], p_vector[i + 1]});
			}

			if (i == 0) {
				dec_map.insert({p_vector[i], p_vector[i]});
			} else {
				dec_map.insert({p_vector[i], p_vector[i - 1]});
			}
		}
		setIncrementMap(inc_map);
		setDecrementMap(dec_map);
	}

private:
	OdinMenuFeels m_menu_feels;
	PopupMenu m_dropdown;

	std::map<int, int> m_increment_map;
	std::map<int, int> m_decrement_map;
	std::vector<int> m_legal_values;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NumberSelectorWithText)
};
