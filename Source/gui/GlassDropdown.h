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

#include <functional>

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "OdinFeels.h"

class GlassDropdown : public juce::ComboBox {
public:
	enum class Type { unassigned, dropdown_12x4, dropdown_14x4 };

	GlassDropdown(Type p_type = Type::unassigned);
	virtual ~GlassDropdown();

	virtual void paint(Graphics &) override;
	void resized() override {
	} //note without this function, screen text will be drawn double.... no idea..
	void setImage(juce::Image p_panel) {
		m_glaspanel = p_panel;
	}
	void setColor(juce::Colour p_color) {
		m_color = p_color;
	}
	void setGreyColor(juce::Colour p_color) {
		m_grey_color = p_color;
	}
	void setGreyBackgroundColor(juce::Colour p_color) {
		m_grey_background_color = p_color;
	}
	void setFontColor(juce::Colour p_color) {
		m_font_color = p_color;
	}
	void setGreyFirstElement(bool p_grey) {
		m_grey_first_element = p_grey;
	}
	void setInlay(int p_inlay) {
		m_inlay = p_inlay;
	}
	void setDefaultText(const std::string &p_text) {
		m_default_text = p_text;
	}
	int m_inlay = 0;
	void setInlayTop(int p_inlay) {
		m_inlay_top = p_inlay;
	}

	void mouseDown(const MouseEvent &event) override {
		rearrangeMenu();
		ComboBox::mouseDown(event);
	}

	void setValue(int p_value) {
		if (m_setOneToZero && p_value == 1) {
			p_value = 0;
		}
		setSelectedId(p_value, sendNotification);
		repaint();
	}

	void reset() {
		setSelectedId(0, sendNotification);
		repaint();
	}

	std::function<void()> rearrangeMenu = []() {};

	void enableOneToZero(bool p_enable) {
		m_setOneToZero = p_enable;
	}

	void setInlayLeft(int p_inlay) {
		m_inlay_left = p_inlay;
	}
	void showTriangle(bool p_show = true) {
		m_show_triangle = p_show;
	}

protected:
	void paintTriangle(juce::Graphics &g);

    Type m_type;
    UIAssets::Indices m_asset;

	int m_inlay_top  = 0;
	int m_inlay_left = 0;

	bool m_setOneToZero        = false;
	bool m_show_triangle       = false;
	std::string m_default_text = "undefined";

	bool m_grey_first_element = false;

	juce::Image m_glaspanel;
	juce::Colour m_color;
	juce::Colour m_grey_color            = juce::Colours::lightgrey;
	juce::Colour m_grey_background_color = juce::Colours::black;
	juce::Colour m_font_color            = juce::Colours::white;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlassDropdown)

	OdinMenuFeels m_menu_feels;
};
