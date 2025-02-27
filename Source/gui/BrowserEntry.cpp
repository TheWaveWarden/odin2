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

#include "BrowserEntry.h"
#include <JuceHeader.h>

BrowserEntry::BrowserEntry(String p_text) : m_text(p_text) {
	addChildComponent(m_rename_editor);
	m_rename_editor.setColour(TextEditor::ColourIds::backgroundColourId, PATCH_BROWSER_INPUT_FIELD_BACKGROUND_COLOR);
	m_rename_editor.setColour(TextEditor::ColourIds::textColourId, COL_LIGHT);
	m_rename_editor.setSelectAllWhenFocused(true);
	m_rename_editor.onFocusLost = [&]() { hideRenameEditor(); };
	m_rename_editor.onEscapeKey = [&]() { hideRenameEditor(); };

	m_rename_editor.onReturnKey = [&]() {
		if (m_rename_editor.getText().isEmpty()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "Empty Name", "Please input a valid name!", "Ok");
			return;
		}
		hideRenameEditor();
		applyRenaming(getText(), m_rename_editor.getText());
	};

	setWantsKeyboardFocus(true);
}

BrowserEntry::~BrowserEntry() {
}

void BrowserEntry::paint(Graphics &g) {

	const auto background_colour = juce::Colours::black.withAlpha(0.25f);
	const auto highlight_colour  = COL_TEXT_BLUE_DARK;
	static constexpr auto corner = 2.0f;

	if (!m_is_active) {
		g.setColour(juce::Colours::white.withAlpha(0.7f));
		if (m_is_highlighted) {
			g.setColour(background_colour);
			g.fillRoundedRectangle(getLocalBounds().toFloat(), corner);
			g.setColour(highlight_colour);
		}
	} else {
		g.setColour(background_colour);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), corner);
		g.setColour(highlight_colour);
	}

	g.setFont(Helpers::getAldrichFont(H / 1.2f));
	const auto text_area = getLocalBounds().reduced(proportionOfWidth(0.05f), 0);
	g.drawText(m_text, text_area, Justification::centredLeft, true); // draw some placeholder text
}

void BrowserEntry::resized() {
	m_rename_editor.setBounds(0, 0, getWidth(), getHeight());
	m_rename_editor.setFont(H / 1.4f);
	m_rename_editor.setIndents(W / 20.0f, H / 10.0f);
}

void BrowserEntry::mouseEnter(const MouseEvent &e) {
	m_is_highlighted = true;
	Component::mouseEnter(e);
	repaint();
}
void BrowserEntry::mouseExit(const MouseEvent &e) {
	m_is_highlighted = false;
	Component::mouseExit(e);
	repaint();
}

void BrowserEntry::mouseDown(const MouseEvent &e) {
	if (e.mods.isLeftButtonDown()) {
		onLeftClick();
		setEntryActive(true);
		repaint();
	} else if (e.mods.isRightButtonDown()) {
		onRightClick();
	}
	Component::mouseDown(e);
	DBG((int)e.mods.isLeftButtonDown());
}

void BrowserEntry::setEntryActive(bool p_active) {
	m_is_active = p_active;
	if (m_pass_active_element_to_parent) {
		passActiveNameToParent(getText());
	}
	repaint();
}

String BrowserEntry::getText() {
	return m_text;
}

void BrowserEntry::showRenameEditor() {
	m_rename_editor.setVisible(true);
	m_rename_editor.setText(getText());
	m_rename_editor.grabKeyboardFocus();

	//hack: grab keyboardfocus again in 50msec...
	m_timer_counter = 1;
	startTimer(50);
}

void BrowserEntry::hideRenameEditor() {
	m_rename_editor.setVisible(false);
}

void BrowserEntry::setBoundsWithInputField(int p_x, int p_y, int p_width, int p_height) {
	Component::setBounds(p_x, p_y, p_width, p_height);
	m_rename_editor.setBounds(0, 0, p_width, p_height);
}

void BrowserEntry::timerCallback() {
	//hack: grab keyboardfocus after 50msec...
	//doing so directly won't work...

	DBG(m_timer_counter);

	m_rename_editor.grabKeyboardFocus();
	if (--m_timer_counter <= 0) {
		stopTimer();
	}
}

void BrowserEntry::enablePassActiveNameToParent(bool p_enable) {
	m_pass_active_element_to_parent = p_enable;
}
