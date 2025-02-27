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
#include <JuceHeader.h>
#include <functional>

class BrowserEntry : public Component, public Timer {
public:
	BrowserEntry(String p_text);
	~BrowserEntry();

	void paint(Graphics &) override;
	void resized() override;

	void mouseEnter(const MouseEvent &e) override;
	void mouseExit(const MouseEvent &e) override;
	void mouseDown(const MouseEvent &e) override;
	void setEntryActive(bool p_active);
	void showRenameEditor();
	void hideRenameEditor();
	void setBoundsWithInputField(int p_x, int p_y, int p_width, int p_height);
	void enablePassActiveNameToParent(bool p_enable);

	bool isActive() const {
		return m_is_active;
	}

	String getText();

	std::function<void()> onLeftClick;
	std::function<void()> onRightClick;
	std::function<void(String, String)> applyRenaming;
	std::function<void(String)> passActiveNameToParent;

	TextEditor m_rename_editor;

	//hack: grab keyboardfocus again in 50msec...
	void timerCallback() override;

private:
	int m_timer_counter;

	String m_text;

	bool m_is_highlighted                = false;
	bool m_is_active                     = false;
	bool m_pass_active_element_to_parent = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrowserEntry)
};
