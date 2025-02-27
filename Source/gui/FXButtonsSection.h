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

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
//#define FX_BUTTON_OFFSET 48

static bool g_GUI_big = false;

class FXButtonsSection : public Component {
public:
	FXButtonsSection(AudioProcessorValueTreeState &vts, OdinAudioProcessor &p_processor) :
	    m_processor(p_processor),
	    m_value_tree(vts),
	    m_flanger_button("Flanger", &m_constrainer),
	    m_phaser_button("Phaser", &m_constrainer),
	    m_chorus_button("Chorus", &m_constrainer),
	    m_delay_button("Delay", &m_constrainer),
	    m_reverb_button("Reverb", &m_constrainer) {

		m_phaser_button.setTooltip("Shows the phaser.\nDrag this to\nrearrange the FX order");
		addAndMakeVisible(m_phaser_button);

		m_flanger_button.setTooltip("Shows the flanger.\nDrag this to\nrearrange the FX order");
		addAndMakeVisible(m_flanger_button);

		m_chorus_button.setTooltip("Shows the chorus.\nDrag this to\nrearrange the FX order");
		addAndMakeVisible(m_chorus_button);

		m_delay_button.setTooltip("Shows the delay.\nDrag this to\nrearrange the FX order");
		addAndMakeVisible(m_delay_button);

		m_reverb_button.setTooltip("Shows the reverb.\nDrag this to\nrearrange the FX order");
		addAndMakeVisible(m_reverb_button);

		m_phaser_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("phaser", p_position); };
		m_phaser_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_phaser_button.lambdaMouseDown = [&]() {
			setHighlighted("phaser");
			setHighlightedToValueTree(1);
			m_delay_button.setIsPressed(false);
			m_flanger_button.setIsPressed(false);
			m_chorus_button.setIsPressed(false);
			m_reverb_button.setIsPressed(false);
		};

		m_flanger_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("flanger", p_position); };
		m_flanger_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_flanger_button.lambdaMouseDown = [&]() {
			setHighlighted("flanger");
			setHighlightedToValueTree(2);
			m_delay_button.setIsPressed(false);
			m_phaser_button.setIsPressed(false);
			m_chorus_button.setIsPressed(false);
			m_reverb_button.setIsPressed(false);
		};

		m_chorus_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("chorus", p_position); };
		m_chorus_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_chorus_button.lambdaMouseDown = [&]() {
			setHighlighted("chorus");
			setHighlightedToValueTree(3);
			m_delay_button.setIsPressed(false);
			m_phaser_button.setIsPressed(false);
			m_flanger_button.setIsPressed(false);
			m_reverb_button.setIsPressed(false);
		};

		m_delay_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("delay", p_position); };
		m_delay_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_delay_button.lambdaMouseDown = [&]() {
			setHighlighted("delay");
			setHighlightedToValueTree(0);
			m_phaser_button.setIsPressed(false);
			m_flanger_button.setIsPressed(false);
			m_chorus_button.setIsPressed(false);
			m_reverb_button.setIsPressed(false);
		};

		m_reverb_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("reverb", p_position); };
		m_reverb_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_reverb_button.lambdaMouseDown = [&]() {
			setHighlighted("reverb");
			setHighlightedToValueTree(4);
			m_delay_button.setIsPressed(false);
			m_phaser_button.setIsPressed(false);
			m_flanger_button.setIsPressed(false);
			m_chorus_button.setIsPressed(false);
		};

		forceValueTreeOntoComponents(m_value_tree.state);
	}

	void parentSizeChanged() override {
		auto newBounds = getBoundsInParent();

		m_constrainer.checkBounds(newBounds, getBoundsInParent(), getParentComponent()->getLocalBounds(), false, false, false, false);
		setBounds(newBounds);

		m_constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());

		positionButtons();
	}

	void positionButtons(const std::string &p_spare_button = "") {
		const auto width = W / 5;
		if (p_spare_button != "flanger")
			m_flanger_button.setTopLeftPosition(m_position_map.find("flanger")->second * width, 0);
		if (p_spare_button != "phaser")
			m_phaser_button.setTopLeftPosition(m_position_map.find("phaser")->second * width, 0);
		if (p_spare_button != "chorus")
			m_chorus_button.setTopLeftPosition(m_position_map.find("chorus")->second * width, 0);
		if (p_spare_button != "delay")
			m_delay_button.setTopLeftPosition(m_position_map.find("delay")->second * width, 0);
		if (p_spare_button != "reverb")
			m_reverb_button.setTopLeftPosition(m_position_map.find("reverb")->second * width, 0);

		onButtonArrange(m_position_map);
	}

	void changeMapPosition(const std::string &p_button_name, int p_new_position) {

		// return if already in right position
		if (m_position_map.find(p_button_name)->second == p_new_position) {
			return;
		}

		// move other button to old position first
		if (m_position_map.find("flanger")->second == p_new_position) {
			m_position_map.find("flanger")->second = m_position_map.find(p_button_name)->second;
		} else if (m_position_map.find("phaser")->second == p_new_position) {
			m_position_map.find("phaser")->second = m_position_map.find(p_button_name)->second;
		} else if (m_position_map.find("chorus")->second == p_new_position) {
			m_position_map.find("chorus")->second = m_position_map.find(p_button_name)->second;
		} else if (m_position_map.find("delay")->second == p_new_position) {
			m_position_map.find("delay")->second = m_position_map.find(p_button_name)->second;
		} else if (m_position_map.find("reverb")->second == p_new_position) {
			m_position_map.find("reverb")->second = m_position_map.find(p_button_name)->second;
		}
		// move button to new position
		m_position_map.find(p_button_name)->second = p_new_position;
		positionButtons(p_button_name);
	}

	void setHighlightedToValueTree(int p_fx_number) {
		//DBG(p_fx_number);
		switch (p_fx_number) {
		case 0:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 1.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("reverb_selected", 0.f, nullptr);
			break;
		case 1:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 1.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("reverb_selected", 0.f, nullptr);
			break;
		case 2:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 1.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("reverb_selected", 0.f, nullptr);
			break;
		case 3:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 1.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("reverb_selected", 0.f, nullptr);
			break;
		case 4:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("reverb_selected", 1.f, nullptr);
			break;
		default:
			break;
		}
	}

	void forceValueTreeOntoComponents(ValueTree p_tree) {
		if ((int)m_value_tree.state.getChildWithName("fx")["delay_selected"] == 1) {
			setHighlighted("delay");
			m_delay_button.setIsPressed(true);
			m_phaser_button.setIsPressed(false);
			m_flanger_button.setIsPressed(false);
			m_chorus_button.setIsPressed(false);
			m_reverb_button.setIsPressed(false);
		} else if ((int)m_value_tree.state.getChildWithName("fx")["phaser_selected"] == 1) {
			setHighlighted("phaser");
			m_delay_button.setIsPressed(false);
			m_phaser_button.setIsPressed(true);
			m_flanger_button.setIsPressed(false);
			m_chorus_button.setIsPressed(false);
			m_reverb_button.setIsPressed(false);
		} else if ((int)m_value_tree.state.getChildWithName("fx")["flanger_selected"] == 1) {
			setHighlighted("flanger");
			m_delay_button.setIsPressed(false);
			m_phaser_button.setIsPressed(false);
			m_flanger_button.setIsPressed(true);
			m_chorus_button.setIsPressed(false);
			m_reverb_button.setIsPressed(false);
		} else if ((int)m_value_tree.state.getChildWithName("fx")["chorus_selected"] == 1) {
			setHighlighted("chorus");
			m_delay_button.setIsPressed(false);
			m_phaser_button.setIsPressed(false);
			m_flanger_button.setIsPressed(false);
			m_chorus_button.setIsPressed(true);
			m_reverb_button.setIsPressed(false);
		} else if ((int)m_value_tree.state.getChildWithName("fx")["reverb_selected"] == 1) {
			setHighlighted("reverb");
			m_delay_button.setIsPressed(false);
			m_phaser_button.setIsPressed(false);
			m_flanger_button.setIsPressed(false);
			m_chorus_button.setIsPressed(false);
			m_reverb_button.setIsPressed(true);
		}

		m_position_map.find("delay")->second   = m_value_tree.state.getChildWithName("fx")["delay_position"];
		m_position_map.find("phaser")->second  = m_value_tree.state.getChildWithName("fx")["phaser_position"];
		m_position_map.find("flanger")->second = m_value_tree.state.getChildWithName("fx")["flanger_position"];
		m_position_map.find("chorus")->second  = m_value_tree.state.getChildWithName("fx")["chorus_position"];
		m_position_map.find("reverb")->second  = m_value_tree.state.getChildWithName("fx")["reverb_position"];

		positionButtons();
	}

	std::function<void(std::map<std::string, int>)> onButtonArrange = [](std::map<std::string, int>) {};
	std::function<void(std::string)> setHighlighted                 = [](std::string) {};

	void resized() override {
		auto bounds      = getLocalBounds();
		const auto width = bounds.getWidth() / 5;

		m_phaser_button.setBounds(bounds.removeFromLeft(width));
		m_flanger_button.setBounds(bounds.removeFromLeft(width));
		m_chorus_button.setBounds(bounds.removeFromLeft(width));
		m_delay_button.setBounds(bounds.removeFromLeft(width));
		m_reverb_button.setBounds(bounds.removeFromLeft(width));
	}

private:
	OdinAudioProcessor &m_processor;

	DragButton m_flanger_button;
	DragButton m_phaser_button;
	DragButton m_chorus_button;
	DragButton m_delay_button;
	DragButton m_reverb_button;

	AudioProcessorValueTreeState &m_value_tree;

	std::map<std::string, int> m_position_map = {{"reverb", 4}, {"flanger", 3}, {"phaser", 1}, {"chorus", 2}, {"delay", 0}};

	juce::ComponentBoundsConstrainer m_constrainer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXButtonsSection)
};
