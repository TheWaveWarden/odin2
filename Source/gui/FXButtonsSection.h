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
#include <map>
#define FX_BUTTON_OFFSET 61

static bool g_GUI_big = false;

class FXButtonsSection : public Component {
public:
	FXButtonsSection(AudioProcessorValueTreeState &vts, OdinAudioProcessor &p_processor) :
	    m_processor(p_processor), m_value_tree(vts),
	    m_flanger_button("flanger", juce::DrawableButton::ButtonStyle::ImageRaw, &m_constrainer, "flanger"),
	    m_phaser_button("phaser", juce::DrawableButton::ButtonStyle::ImageRaw, &m_constrainer, "phaser"),
	    m_chorus_button("chorus", juce::DrawableButton::ButtonStyle::ImageRaw, &m_constrainer, "chorus"),
	    m_delay_button("delay", juce::DrawableButton::ButtonStyle::ImageRaw, &m_constrainer, "delay") {

		m_phaser_button.setClickingTogglesState(true);
		m_phaser_button.setTooltip("Shows the phaser.\nDrag and Drop this to\nrearrange the FX order");
		addAndMakeVisible(m_phaser_button);
		// m_phaser_button.setAlwaysOnTop(true);
		m_phaser_button.setTriggeredOnMouseDown(true);
		m_phaser_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
		m_phaser_button.setRadioGroupId(1221);

		m_flanger_button.setClickingTogglesState(true);
		m_flanger_button.setTooltip("Shows the flanger.\nDrag and Drop this to\nrearrange the FX order");
		addAndMakeVisible(m_flanger_button);
		// m_flanger_button.setAlwaysOnTop(true);
		m_flanger_button.setTriggeredOnMouseDown(true);
		m_flanger_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
		m_flanger_button.setRadioGroupId(1221);

		m_chorus_button.setClickingTogglesState(true);
		m_chorus_button.setTooltip("Shows the chorus.\nDrag and Drop this to\nrearrange the FX order");
		addAndMakeVisible(m_chorus_button);
		// m_chorus_button.setOnTop(true);
		m_chorus_button.setTriggeredOnMouseDown(true);
		m_chorus_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
		m_chorus_button.setRadioGroupId(1221);

		m_delay_button.setClickingTogglesState(true);
		m_delay_button.setTooltip("Shows the delay.\nDrag and Drop this to\nrearrange the FX order");
		addAndMakeVisible(m_delay_button);
		// m_delay_button.setAlwaysOnTop(true);
		m_delay_button.setTriggeredOnMouseDown(true);
		m_delay_button.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
		m_delay_button.setRadioGroupId(1221);
		m_delay_button.setToggleState(true, dontSendNotification);

		m_phaser_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("phaser", p_position); };
		m_phaser_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_phaser_button.lambdaMouseDown = [&]() {
			setHighlighted("phaser");
			setHighlightedToValueTree(1);
		};

		m_flanger_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("flanger", p_position); };
		m_flanger_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_flanger_button.lambdaMouseDown = [&]() {
			setHighlighted("flanger");
			setHighlightedToValueTree(2);
		};

		m_chorus_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("chorus", p_position); };
		m_chorus_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_chorus_button.lambdaMouseDown = [&]() {
			setHighlighted("chorus");
			setHighlightedToValueTree(3);
		};

		m_delay_button.lambdaMouseDrag = [&](int p_position) { changeMapPosition("delay", p_position); };
		m_delay_button.lambdaMouseUp   = [&]() { positionButtons(); };
		m_delay_button.lambdaMouseDown = [&]() {
			setHighlighted("delay");
			setHighlightedToValueTree(0);
		};

		forceValueTreeOntoComponents(m_value_tree.state);
	}

	void parentSizeChanged() override {
		auto newBounds = getBoundsInParent();

		m_constrainer.checkBounds(
		    newBounds, getBoundsInParent(), getParentComponent()->getLocalBounds(), false, false, false, false);
		setBounds(newBounds);

		m_constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());

		positionButtons();
	}

	void positionButtons(const std::string &p_spare_button = "") {
		float GUI_factor = (m_GUI_big) ? 1.5f : 1.f;
		if (p_spare_button != "flanger")
			m_flanger_button.setTopLeftPosition(m_position_map.find("flanger")->second * FX_BUTTON_OFFSET * GUI_factor,
			                                    0);
		if (p_spare_button != "phaser")
			m_phaser_button.setTopLeftPosition(m_position_map.find("phaser")->second * FX_BUTTON_OFFSET * GUI_factor,
			                                   0);
		if (p_spare_button != "chorus")
			m_chorus_button.setTopLeftPosition(m_position_map.find("chorus")->second * FX_BUTTON_OFFSET * GUI_factor,
			                                   0);
		if (p_spare_button != "delay")
			m_delay_button.setTopLeftPosition(m_position_map.find("delay")->second * FX_BUTTON_OFFSET * GUI_factor, 0);
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
		}
		if (m_position_map.find("phaser")->second == p_new_position) {
			m_position_map.find("phaser")->second = m_position_map.find(p_button_name)->second;
		}
		if (m_position_map.find("chorus")->second == p_new_position) {
			m_position_map.find("chorus")->second = m_position_map.find(p_button_name)->second;
		}
		if (m_position_map.find("delay")->second == p_new_position) {
			m_position_map.find("delay")->second = m_position_map.find(p_button_name)->second;
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
			break;
		case 1:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 1.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 0.f, nullptr);
			break;
		case 2:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 1.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 0.f, nullptr);
			break;
		case 3:
			m_value_tree.state.getChildWithName("fx").setProperty("delay_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("phaser_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("flanger_selected", 0.f, nullptr);
			m_value_tree.state.getChildWithName("fx").setProperty("chorus_selected", 1.f, nullptr);
			break;
		default:
			break;
		}
	}

	void forceValueTreeOntoComponents(ValueTree p_tree) {
		if ((int)m_value_tree.state.getChildWithName("fx")["delay_selected"] == 1) {
			setHighlighted("delay");
			m_delay_button.setToggleState(true, dontSendNotification);
			m_phaser_button.setToggleState(false, dontSendNotification);
			m_flanger_button.setToggleState(false, dontSendNotification);
			m_chorus_button.setToggleState(false, dontSendNotification);
		} else if ((int)m_value_tree.state.getChildWithName("fx")["phaser_selected"] == 1) {
			setHighlighted("phaser");
			m_delay_button.setToggleState(false, dontSendNotification);
			m_phaser_button.setToggleState(true, dontSendNotification);
			m_flanger_button.setToggleState(false, dontSendNotification);
			m_chorus_button.setToggleState(false, dontSendNotification);
		} else if ((int)m_value_tree.state.getChildWithName("fx")["flanger_selected"] == 1) {
			setHighlighted("flanger");
			m_delay_button.setToggleState(false, dontSendNotification);
			m_phaser_button.setToggleState(false, dontSendNotification);
			m_flanger_button.setToggleState(true, dontSendNotification);
			m_chorus_button.setToggleState(false, dontSendNotification);
		} else if ((int)m_value_tree.state.getChildWithName("fx")["chorus_selected"] == 1) {
			setHighlighted("chorus");
			m_delay_button.setToggleState(false, dontSendNotification);
			m_phaser_button.setToggleState(false, dontSendNotification);
			m_flanger_button.setToggleState(false, dontSendNotification);
			m_chorus_button.setToggleState(true, dontSendNotification);
		}

		m_position_map.find("delay")->second   = m_value_tree.state.getChildWithName("fx")["delay_position"];
		m_position_map.find("phaser")->second  = m_value_tree.state.getChildWithName("fx")["phaser_position"];
		m_position_map.find("flanger")->second = m_value_tree.state.getChildWithName("fx")["flanger_position"];
		m_position_map.find("chorus")->second  = m_value_tree.state.getChildWithName("fx")["chorus_position"];

		positionButtons();
	}

	std::function<void(std::map<std::string, int>)> onButtonArrange = [](std::map<std::string, int>) {};
	std::function<void(std::string)> setHighlighted                 = [](std::string) {};

	void setGUIBig() {
		m_GUI_big = true;
		juce::Image phaser_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_1_150_png, BinaryData::buttonphaser_1_150_pngSize);
		juce::Image phaser_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_2_150_png, BinaryData::buttonphaser_2_150_pngSize);
		juce::Image phaser_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_3_150_png, BinaryData::buttonphaser_3_150_pngSize);
		juce::Image phaser_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_4_150_png, BinaryData::buttonphaser_4_150_pngSize);

		juce::DrawableImage phaser_button_draw1;
		juce::DrawableImage phaser_button_draw2;
		juce::DrawableImage phaser_button_draw3;
		juce::DrawableImage phaser_button_draw4;

		phaser_button_draw1.setImage(phaser_button_1);
		phaser_button_draw2.setImage(phaser_button_2);
		phaser_button_draw3.setImage(phaser_button_3);
		phaser_button_draw4.setImage(phaser_button_4);

		m_phaser_button.setImages(&phaser_button_draw2,
		                          &phaser_button_draw2,
		                          &phaser_button_draw1,
		                          &phaser_button_draw1,
		                          &phaser_button_draw3,
		                          &phaser_button_draw3,
		                          &phaser_button_draw3,
		                          &phaser_button_draw3);
		m_phaser_button.setBounds(0, 0, phaser_button_1.getWidth(), phaser_button_1.getHeight());

		juce::Image flanger_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_1_150_png, BinaryData::buttonflanger_1_150_pngSize);
		juce::Image flanger_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_2_150_png, BinaryData::buttonflanger_2_150_pngSize);
		juce::Image flanger_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_3_150_png, BinaryData::buttonflanger_3_150_pngSize);
		juce::Image flanger_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_4_150_png, BinaryData::buttonflanger_4_150_pngSize);

		juce::DrawableImage flanger_button_draw1;
		juce::DrawableImage flanger_button_draw2;
		juce::DrawableImage flanger_button_draw3;
		juce::DrawableImage flanger_button_draw4;

		flanger_button_draw1.setImage(flanger_button_1);
		flanger_button_draw2.setImage(flanger_button_2);
		flanger_button_draw3.setImage(flanger_button_3);
		flanger_button_draw4.setImage(flanger_button_4);

		m_flanger_button.setImages(&flanger_button_draw2,
		                           &flanger_button_draw2,
		                           &flanger_button_draw1,
		                           &flanger_button_draw1,
		                           &flanger_button_draw3,
		                           &flanger_button_draw3,
		                           &flanger_button_draw3,
		                           &flanger_button_draw3);
		m_flanger_button.setBounds(
		    OdinHelper::c150(FX_BUTTON_OFFSET), 0, flanger_button_1.getWidth(), flanger_button_1.getHeight());

		juce::Image chorus_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_1_150_png, BinaryData::buttonchorus_1_150_pngSize);
		juce::Image chorus_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_2_150_png, BinaryData::buttonchorus_2_150_pngSize);
		juce::Image chorus_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_3_150_png, BinaryData::buttonchorus_3_150_pngSize);
		juce::Image chorus_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_4_150_png, BinaryData::buttonchorus_4_150_pngSize);

		juce::DrawableImage chorus_button_draw1;
		juce::DrawableImage chorus_button_draw2;
		juce::DrawableImage chorus_button_draw3;
		juce::DrawableImage chorus_button_draw4;

		chorus_button_draw1.setImage(chorus_button_1);
		chorus_button_draw2.setImage(chorus_button_2);
		chorus_button_draw3.setImage(chorus_button_3);
		chorus_button_draw4.setImage(chorus_button_4);

		m_chorus_button.setImages(&chorus_button_draw2,
		                          &chorus_button_draw2,
		                          &chorus_button_draw1,
		                          &chorus_button_draw1,
		                          &chorus_button_draw3,
		                          &chorus_button_draw3,
		                          &chorus_button_draw3,
		                          &chorus_button_draw3);
		m_chorus_button.setBounds(
		    2 * OdinHelper::c150(FX_BUTTON_OFFSET), 0, chorus_button_1.getWidth(), chorus_button_1.getHeight());

		juce::Image delay_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_1_150_png, BinaryData::buttondelay_1_150_pngSize);
		juce::Image delay_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_2_150_png, BinaryData::buttondelay_2_150_pngSize);
		juce::Image delay_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_3_150_png, BinaryData::buttondelay_3_150_pngSize);
		juce::Image delay_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_4_150_png, BinaryData::buttondelay_4_150_pngSize);

		juce::DrawableImage delay_button_draw1;
		juce::DrawableImage delay_button_draw2;
		juce::DrawableImage delay_button_draw3;
		juce::DrawableImage delay_button_draw4;

		delay_button_draw1.setImage(delay_button_1);
		delay_button_draw2.setImage(delay_button_2);
		delay_button_draw3.setImage(delay_button_3);
		delay_button_draw4.setImage(delay_button_4);

		m_delay_button.setImages(&delay_button_draw2,
		                         &delay_button_draw2,
		                         &delay_button_draw1,
		                         &delay_button_draw1,
		                         &delay_button_draw3,
		                         &delay_button_draw3,
		                         &delay_button_draw3,
		                         &delay_button_draw3);
		m_delay_button.setBounds(
		    3 * OdinHelper::c150(FX_BUTTON_OFFSET), 0, delay_button_1.getWidth(), delay_button_1.getHeight());
		setSize(OdinHelper::c150(FX_BUTTON_OFFSET) * 4 + 1, delay_button_1.getHeight());

		m_phaser_button.setGUIBig();
		m_flanger_button.setGUIBig();
		m_delay_button.setGUIBig();
		m_chorus_button.setGUIBig();

		forceValueTreeOntoComponents(m_value_tree.state);
	}
	void setGUISmall() {
		m_GUI_big = false;
		juce::Image phaser_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_1_png, BinaryData::buttonphaser_1_pngSize);
		juce::Image phaser_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_2_png, BinaryData::buttonphaser_2_pngSize);
		juce::Image phaser_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_3_png, BinaryData::buttonphaser_3_pngSize);
		juce::Image phaser_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttonphaser_4_png, BinaryData::buttonphaser_4_pngSize);

		juce::DrawableImage phaser_button_draw1;
		juce::DrawableImage phaser_button_draw2;
		juce::DrawableImage phaser_button_draw3;
		juce::DrawableImage phaser_button_draw4;

		phaser_button_draw1.setImage(phaser_button_1);
		phaser_button_draw2.setImage(phaser_button_2);
		phaser_button_draw3.setImage(phaser_button_3);
		phaser_button_draw4.setImage(phaser_button_4);

		m_phaser_button.setImages(&phaser_button_draw2,
		                          &phaser_button_draw2,
		                          &phaser_button_draw1,
		                          &phaser_button_draw1,
		                          &phaser_button_draw3,
		                          &phaser_button_draw3,
		                          &phaser_button_draw3,
		                          &phaser_button_draw3);
		m_phaser_button.setBounds(0, 0, phaser_button_1.getWidth(), phaser_button_1.getHeight());

		juce::Image flanger_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_1_png, BinaryData::buttonflanger_1_pngSize);
		juce::Image flanger_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_2_png, BinaryData::buttonflanger_2_pngSize);
		juce::Image flanger_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_3_png, BinaryData::buttonflanger_3_pngSize);
		juce::Image flanger_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttonflanger_4_png, BinaryData::buttonflanger_4_pngSize);

		juce::DrawableImage flanger_button_draw1;
		juce::DrawableImage flanger_button_draw2;
		juce::DrawableImage flanger_button_draw3;
		juce::DrawableImage flanger_button_draw4;

		flanger_button_draw1.setImage(flanger_button_1);
		flanger_button_draw2.setImage(flanger_button_2);
		flanger_button_draw3.setImage(flanger_button_3);
		flanger_button_draw4.setImage(flanger_button_4);

		m_flanger_button.setImages(&flanger_button_draw2,
		                           &flanger_button_draw2,
		                           &flanger_button_draw1,
		                           &flanger_button_draw1,
		                           &flanger_button_draw3,
		                           &flanger_button_draw3,
		                           &flanger_button_draw3,
		                           &flanger_button_draw3);
		m_flanger_button.setBounds(FX_BUTTON_OFFSET, 0, flanger_button_1.getWidth(), flanger_button_1.getHeight());

		juce::Image chorus_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_1_png, BinaryData::buttonchorus_1_pngSize);
		juce::Image chorus_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_2_png, BinaryData::buttonchorus_2_pngSize);
		juce::Image chorus_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_3_png, BinaryData::buttonchorus_3_pngSize);
		juce::Image chorus_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttonchorus_4_png, BinaryData::buttonchorus_4_pngSize);

		juce::DrawableImage chorus_button_draw1;
		juce::DrawableImage chorus_button_draw2;
		juce::DrawableImage chorus_button_draw3;
		juce::DrawableImage chorus_button_draw4;

		chorus_button_draw1.setImage(chorus_button_1);
		chorus_button_draw2.setImage(chorus_button_2);
		chorus_button_draw3.setImage(chorus_button_3);
		chorus_button_draw4.setImage(chorus_button_4);

		m_chorus_button.setImages(&chorus_button_draw2,
		                          &chorus_button_draw2,
		                          &chorus_button_draw1,
		                          &chorus_button_draw1,
		                          &chorus_button_draw3,
		                          &chorus_button_draw3,
		                          &chorus_button_draw3,
		                          &chorus_button_draw3);
		m_chorus_button.setBounds(2 * FX_BUTTON_OFFSET, 0, chorus_button_1.getWidth(), chorus_button_1.getHeight());

		juce::Image delay_button_1 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_1_png, BinaryData::buttondelay_1_pngSize);
		juce::Image delay_button_2 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_2_png, BinaryData::buttondelay_2_pngSize);
		juce::Image delay_button_3 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_3_png, BinaryData::buttondelay_3_pngSize);
		juce::Image delay_button_4 =
		    ImageCache::getFromMemory(BinaryData::buttondelay_4_png, BinaryData::buttondelay_4_pngSize);

		juce::DrawableImage delay_button_draw1;
		juce::DrawableImage delay_button_draw2;
		juce::DrawableImage delay_button_draw3;
		juce::DrawableImage delay_button_draw4;

		delay_button_draw1.setImage(delay_button_1);
		delay_button_draw2.setImage(delay_button_2);
		delay_button_draw3.setImage(delay_button_3);
		delay_button_draw4.setImage(delay_button_4);

		m_delay_button.setImages(&delay_button_draw2,
		                         &delay_button_draw2,
		                         &delay_button_draw1,
		                         &delay_button_draw1,
		                         &delay_button_draw3,
		                         &delay_button_draw3,
		                         &delay_button_draw3,
		                         &delay_button_draw3);
		m_delay_button.setBounds(3 * FX_BUTTON_OFFSET, 0, delay_button_1.getWidth(), delay_button_1.getHeight());
		setSize(FX_BUTTON_OFFSET * 4 + 1, delay_button_1.getHeight());

		m_phaser_button.setGUISmall();
		m_flanger_button.setGUISmall();
		m_delay_button.setGUISmall();
		m_chorus_button.setGUISmall();

		forceValueTreeOntoComponents(m_value_tree.state);
	}

private:
	bool m_GUI_big = false;

	OdinAudioProcessor &m_processor;

	DragButton m_flanger_button;
	DragButton m_phaser_button;
	DragButton m_chorus_button;
	DragButton m_delay_button;

	AudioProcessorValueTreeState &m_value_tree;

	std::map<std::string, int> m_position_map = {{"flanger", 3}, {"phaser", 1}, {"chorus", 2}, {"delay", 0}};

	juce::ComponentBoundsConstrainer m_constrainer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FXButtonsSection)
};
