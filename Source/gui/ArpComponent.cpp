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

#include "ArpComponent.h"
#include "../ConfigFileManager.h"
#include "JsonGuiProvider.h"
#include "UIAssetManager.h"
#include <JuceHeader.h>

ArpComponent::ArpComponent(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &vts) :
    m_processor(p_processor),
    m_value_tree(vts),
    m_step_0(vts, 0),
    m_step_1(vts, 1),
    m_step_2(vts, 2),
    m_step_3(vts, 3),
    m_step_4(vts, 4),
    m_step_5(vts, 5),
    m_step_6(vts, 6),
    m_step_7(vts, 7),
    m_step_8(vts, 8),
    m_step_9(vts, 9),
    m_step_10(vts, 10),
    m_step_11(vts, 11),
    m_step_12(vts, 12),
    m_step_13(vts, 13),
    m_step_14(vts, 14),
    m_step_15(vts, 15),
    m_on("arp_on", "", OdinButton::Type::power_4x4),
    m_one_shot("arp_one_shot", "1-Shot", OdinButton::Type::button_7x5),
    m_mod_transpose("arp_mod_transpose", "Trans", "Mod 2"),
    m_mod1_label("Mod 1"),
    m_sync_time(UIAssets::Indices::screen_up_down_13x4_LR),
    m_octave_selector(NumberSelector::Type::selector_16x4),
    m_steps_selector(NumberSelector::Type::selector_16x4),
    m_direction(NumberSelector::Type::selector_16x4),
    m_gate(NumberSelector::Type::selector_16x4) {

	addAndMakeVisible(m_step_0);
	addAndMakeVisible(m_step_1);
	addAndMakeVisible(m_step_2);
	addAndMakeVisible(m_step_3);
	addAndMakeVisible(m_step_4);
	addAndMakeVisible(m_step_5);
	addAndMakeVisible(m_step_6);
	addAndMakeVisible(m_step_7);
	addAndMakeVisible(m_step_8);
	addAndMakeVisible(m_step_9);
	addAndMakeVisible(m_step_10);
	addAndMakeVisible(m_step_11);
	addAndMakeVisible(m_step_12);
	addAndMakeVisible(m_step_13);
	addAndMakeVisible(m_step_14);
	addAndMakeVisible(m_step_15);
	addAndMakeVisible(m_mod1_label);

	m_octave_selector.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("misc").setProperty("arp_octaves", p_new_value, nullptr); };
	m_octave_selector.valueToText   = [](int p_value) {
        if (p_value > 1) {
            return std::to_string(p_value) + " Octaves";
        } else {
            return std::to_string(p_value) + " Octave";
        }
	};
	m_octave_selector.setLegalValues({1, 2, 3, 4});

	addAndMakeVisible(m_octave_selector);
	m_octave_selector.setMouseDragDivisor(20.f);
	m_octave_selector.setTooltip("Sets how many octaves the arpeggio will play");

	m_steps_selector.OnValueChange = [&](int p_new_value) {
		setNumberLEDsToShow(p_new_value);
		m_value_tree.state.getChildWithName("misc").setProperty("arp_steps", p_new_value, nullptr);
	};
	m_steps_selector.valueToText = [](int p_value) {
		if (p_value > 1) {
			return std::to_string(p_value) + " Steps";
		} else {
			return std::to_string(p_value) + " Step";
		}
	};
	m_steps_selector.setLegalValues({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});

	addAndMakeVisible(m_steps_selector);
	m_steps_selector.setMouseDragDivisor(20.f);
	m_steps_selector.setTooltip("Sets how many steps the step sequence has before it wraps around");

	m_direction.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("misc").setProperty("arp_direction", p_new_value, nullptr); };
	m_direction.valueToText   = [&](int p_value) { return OdinArpeggiator::ArpPatternToString((OdinArpeggiator::ArpPattern)p_value).toStdString(); };
	m_direction.setLegalValues({(int)OdinArpeggiator::ArpPattern::Up,
	                            (int)OdinArpeggiator::ArpPattern::Down,
	                            (int)OdinArpeggiator::ArpPattern::UpAndDown,
	                            (int)OdinArpeggiator::ArpPattern::DownAndUp,
	                            (int)OdinArpeggiator::ArpPattern::Random,
	                            (int)OdinArpeggiator::ArpPattern::CrawlUp,
	                            (int)OdinArpeggiator::ArpPattern::CrawlDown});
	addAndMakeVisible(m_direction);
	m_direction.setMouseDragDivisor(20.f);
	m_direction.setTooltip("Sets the direction of the arpeggio being played");

	m_gate.OnValueChange = [&](int p_new_value) { m_value_tree.state.getChildWithName("misc").setProperty("arp_gate", p_new_value, nullptr); };
	m_gate.valueToText   = [&](int p_value) {
        if (p_value < 100) {
            return "Gate: " + std::to_string(p_value) + "%";
        } else {
            return "Gate " + std::to_string(p_value) + "%";
        }
	};
	m_gate.setLegalValues({10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 125, 150, 175, 200});
	addAndMakeVisible(m_gate);
	m_gate.setMouseDragDivisor(20.f);
	m_gate.setTooltip("Sets how long each note is triggered, before a note-off is sent");

	m_sync_time.OnValueChange = [&](int p_left, int p_right) {
		m_value_tree.state.getChildWithName("misc").setProperty("arp_synctime_numerator", p_left, nullptr);
		m_value_tree.state.getChildWithName("misc").setProperty("arp_synctime_denominator", p_right, nullptr);
	};
	m_sync_time.setTooltip("Sets the time for the arpeggiator synced to the track");
	addAndMakeVisible(m_sync_time);

	m_on.setClickingTogglesState(true);
	addAndMakeVisible(m_on);
	m_on.setTooltip("Turns the arpeggiator on or off");
	m_on.setTriggeredOnMouseDown(true);
	m_on.onClick = [&]() { setComponentsEnabled(); };

	m_mod_transpose.setClickingTogglesState(true);
	addAndMakeVisible(m_mod_transpose);
	m_mod_transpose.setTooltip("Toggles between the knob-rows for mod 2 or transpose");
	m_mod_transpose.setTriggeredOnMouseDown(true);
	m_mod_transpose.onClick = [&]() { setModTranspose(!m_mod_transpose.getToggleState()); };

	m_one_shot.setClickingTogglesState(true);
	addAndMakeVisible(m_one_shot);
	m_one_shot.setTooltip("Makes the sequence stop after it ran through every step once");
	m_one_shot.setTriggeredOnMouseDown(true);
	m_one_shot.setFontHeightRelative(0.4f);

	m_on_attach.reset(new OdinButtonAttachment(m_value_tree, "arp_on", m_on));
	m_one_shot_attach.reset(new OdinButtonAttachment(m_value_tree, "arp_one_shot", m_one_shot));
	m_mod_transpose_attach.reset(new OdinButtonAttachment(m_value_tree, "arp_mod_transpose", m_mod_transpose));
}

ArpComponent::~ArpComponent() {
}

void ArpComponent::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::Arpeggiator_back, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
}

void ArpComponent::resized() {
	GET_LOCAL_AREA(m_mod1_label, "ArpMod1Label");
	GET_LOCAL_AREA(m_octave_selector, "ArpOctaveSelector");
	GET_LOCAL_AREA(m_steps_selector, "ArpStepsSelector");
	GET_LOCAL_AREA(m_direction, "ArpDirection");
	GET_LOCAL_AREA(m_gate, "ArpGate");
	GET_LOCAL_AREA(m_on, "ArpOn");
	GET_LOCAL_AREA(m_one_shot, "ArpOneShot");
	GET_LOCAL_AREA(m_mod_transpose, "ArpModTranspose");
	GET_LOCAL_AREA(m_sync_time, "ArpSyncTime");
	GET_LOCAL_AREA(m_step_0, "ArpStep_0");

	const auto width                                  = m_step_0.getBounds().getWidth();
	std::array<juce::Component *, 16> step_components = {
	    &m_step_0,
	    &m_step_1,
	    &m_step_2,
	    &m_step_3,
	    &m_step_4,
	    &m_step_5,
	    &m_step_6,
	    &m_step_7,
	    &m_step_8,
	    &m_step_9,
	    &m_step_10,
	    &m_step_11,
	    &m_step_12,
	    &m_step_13,
	    &m_step_14,
	    &m_step_15,
	};
	for (int step = 1; step < 16; ++step) {
		step_components[step]->setBounds(step_components[step - 1]->getBounds().translated(width, 0));
	}
}

void ArpComponent::timerCallback() {
	int led_active = m_processor.m_step_led_active.get();

	if (led_active != -1) {
		setLEDActive(led_active);
	} else {
		setNoLEDActive();
	}
}

void ArpComponent::setLEDActive(int p_LED) {
	m_step_0.setLEDActive(p_LED);
	m_step_1.setLEDActive(p_LED);
	m_step_2.setLEDActive(p_LED);
	m_step_3.setLEDActive(p_LED);
	m_step_4.setLEDActive(p_LED);
	m_step_5.setLEDActive(p_LED);
	m_step_6.setLEDActive(p_LED);
	m_step_7.setLEDActive(p_LED);
	m_step_8.setLEDActive(p_LED);
	m_step_9.setLEDActive(p_LED);
	m_step_10.setLEDActive(p_LED);
	m_step_11.setLEDActive(p_LED);
	m_step_12.setLEDActive(p_LED);
	m_step_13.setLEDActive(p_LED);
	m_step_14.setLEDActive(p_LED);
	m_step_15.setLEDActive(p_LED);
}

void ArpComponent::setNoLEDActive() {
	m_step_0.setLEDActive(-1);
	m_step_1.setLEDActive(-1);
	m_step_2.setLEDActive(-1);
	m_step_3.setLEDActive(-1);
	m_step_4.setLEDActive(-1);
	m_step_5.setLEDActive(-1);
	m_step_6.setLEDActive(-1);
	m_step_7.setLEDActive(-1);
	m_step_8.setLEDActive(-1);
	m_step_9.setLEDActive(-1);
	m_step_10.setLEDActive(-1);
	m_step_11.setLEDActive(-1);
	m_step_12.setLEDActive(-1);
	m_step_13.setLEDActive(-1);
	m_step_14.setLEDActive(-1);
	m_step_15.setLEDActive(-1);
}

void ArpComponent::setNumberLEDsToShow(int p_number) {
	m_step_0.setShowLED(p_number);
	m_step_1.setShowLED(p_number);
	m_step_2.setShowLED(p_number);
	m_step_3.setShowLED(p_number);
	m_step_4.setShowLED(p_number);
	m_step_5.setShowLED(p_number);
	m_step_6.setShowLED(p_number);
	m_step_7.setShowLED(p_number);
	m_step_8.setShowLED(p_number);
	m_step_9.setShowLED(p_number);
	m_step_10.setShowLED(p_number);
	m_step_11.setShowLED(p_number);
	m_step_12.setShowLED(p_number);
	m_step_13.setShowLED(p_number);
	m_step_14.setShowLED(p_number);
	m_step_15.setShowLED(p_number);
}

void ArpComponent::forceValueTreeOntoComponents(ValueTree p_tree) {

	m_octave_selector.setValue((int)m_value_tree.state.getChildWithName("misc")["arp_octaves"]);

	setNumberLEDsToShow((int)m_value_tree.state.getChildWithName("misc")["arp_steps"]);
	m_steps_selector.setValue((int)m_value_tree.state.getChildWithName("misc")["arp_steps"]);

	m_direction.setValue((int)m_value_tree.state.getChildWithName("misc")["arp_direction"]);
	m_gate.setValue((int)m_value_tree.state.getChildWithName("misc")["arp_gate"]);
	m_sync_time.setValues((int)m_value_tree.state.getChildWithName("misc")["arp_synctime_numerator"], (int)m_value_tree.state.getChildWithName("misc")["arp_synctime_denominator"]);

	setModTranspose(!m_mod_transpose.getToggleState());
	setComponentsEnabled();
}

void ArpComponent::setVisibleAndStartTimer(bool p_set_visible) {
	if (p_set_visible) {
		startTimer(ARP_TIMER_MILLISECS);
	} else {
		stopTimer();
	}
	setVisible(p_set_visible);
}

void ArpComponent::setModTranspose(bool p_is_mod) {
	m_step_0.setModTranspose(p_is_mod);
	m_step_1.setModTranspose(p_is_mod);
	m_step_2.setModTranspose(p_is_mod);
	m_step_3.setModTranspose(p_is_mod);
	m_step_4.setModTranspose(p_is_mod);
	m_step_5.setModTranspose(p_is_mod);
	m_step_6.setModTranspose(p_is_mod);
	m_step_7.setModTranspose(p_is_mod);
	m_step_8.setModTranspose(p_is_mod);
	m_step_9.setModTranspose(p_is_mod);
	m_step_10.setModTranspose(p_is_mod);
	m_step_11.setModTranspose(p_is_mod);
	m_step_12.setModTranspose(p_is_mod);
	m_step_13.setModTranspose(p_is_mod);
	m_step_14.setModTranspose(p_is_mod);
	m_step_15.setModTranspose(p_is_mod);
}

void ArpComponent::setComponentsEnabled() {
	const auto enabled = m_on.getToggleState();

	m_one_shot.setEnabled(enabled);
	m_direction.setEnabled(enabled);
	m_gate.setEnabled(enabled);
	m_sync_time.setEnabled(enabled);
	m_octave_selector.setEnabled(enabled);
	m_mod1_label.setEnabled(enabled);
	m_mod_transpose.setEnabled(enabled);
	m_steps_selector.setEnabled(enabled);

	m_step_0.setEnabled(enabled);
	m_step_1.setEnabled(enabled);
	m_step_2.setEnabled(enabled);
	m_step_3.setEnabled(enabled);
	m_step_4.setEnabled(enabled);
	m_step_5.setEnabled(enabled);
	m_step_6.setEnabled(enabled);
	m_step_7.setEnabled(enabled);
	m_step_8.setEnabled(enabled);
	m_step_9.setEnabled(enabled);
	m_step_10.setEnabled(enabled);
	m_step_11.setEnabled(enabled);
	m_step_12.setEnabled(enabled);
	m_step_13.setEnabled(enabled);
	m_step_14.setEnabled(enabled);
	m_step_15.setEnabled(enabled);
}
