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

#include <JuceHeader.h>
#include "BrowserEntry.h"
#include "../GlobalIncludes.h"
#include "PatchBrowserScrollBar.h"
#include "OdinFeels.h"

#define FACTORY_PRESETS_SOUNDBANK_CODE "F_A_C_T_O_R_Y"

#define ENTRY_HEIGHT_150 24
#define ENTRY_HEIGHT_100 18

#define MOUSE_WHEEL_FACTOR_PATCH_BROWSER_150 90.f
#define MOUSE_WHEEL_FACTOR_PATCH_BROWSER_100 60.f

#define BUTTON_HEIGHT_BROWSER 18
#define BUTTON_HEIGHT_BROWSER_150 25

#define PATCH_BROWSER_MENU_ENTRY_RENAME 10
#define PATCH_BROWSER_MENU_ENTRY_DELETE 20
#define PATCH_BROWSER_MENU_MOVE_OFFSET 1000
#define PATCH_BROWSER_MENU_COPY_OFFSET 3000

#define WARNING_INLAY_X_150 10
#define WARNING_INLAY_X_100 6

#define WARNING_OFFSET_Y_150 50
#define WARNING_OFFSET_Y_100 33

#define SCROLL_BAR_WIDTH_150 15
#define SCROLL_BAR_WIDTH_100 10

class OdinBrowserButtonFeels : public LookAndFeel_V4 {
public:
	void setButtonFontSize(float p_size) {
		m_font_size = p_size;
	}

protected:
	void drawButtonBackground(Graphics &g,
	                          Button &button,
	                          const Colour &backgroundColour,
	                          bool shouldDrawButtonAsHighlighted,
	                          bool shouldDrawButtonAsDown) override {
		auto cornerSize = 0.f;
		auto bounds     = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

		auto baseColour = Colour(20, 20, 20);

		if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
			baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

		g.setColour(baseColour);

		g.fillRoundedRectangle(bounds, cornerSize);
		if (shouldDrawButtonAsHighlighted) {
			g.setColour(ODIN_BLUE);
		} else {
			g.setColour(button.findColour(ComboBox::outlineColourId));
		}
		g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
	}

	void drawButtonText(Graphics &g, TextButton &button, bool p_highlighted, bool /*shouldDrawButtonAsDown*/) override {
		Font font(m_font_size);
		g.setFont(font);

		if (p_highlighted) {
			g.setColour(ODIN_BLUE);
		} else {
			g.setColour(
			    button.findColour(button.getToggleState() ? TextButton::textColourOnId : TextButton::textColourOffId)
			        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));
		}

		const int yIndent    = jmin(4, button.proportionOfHeight(0.3f));
		const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

		const int fontHeight  = roundToInt(font.getHeight() * 0.6f);
		const int leftIndent  = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
		const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
		const int textWidth   = button.getWidth() - leftIndent - rightIndent;

		if (textWidth > 0)
			g.drawFittedText(button.getButtonText(),
			                 leftIndent,
			                 yIndent,
			                 textWidth,
			                 button.getHeight() - yIndent * 2,
			                 Justification::centred,
			                 2);
	}

	float m_font_size = 15.f;
};

class PatchBrowserSelector : public Component {
public:
	enum class DirectoryStatus { Ok, Empty, Nonexistent };
	enum class BrowserType { Soundbank, Category, Patch };

	PatchBrowserSelector(File::TypesOfFileToFind p_file_or_dir,
	                     String p_left_button,
	                     String p_mid_button,
	                     String p_right_button);
	~PatchBrowserSelector();

	void paint(Graphics &) override;

	void setGUIBig();
	void setGUISmall();

	void resetInputFieldAndShow();
	void showInputField();
	void hideInputField();

	void setDirectory(String p_absolute_path);
	void setDirectoryFactoryPresetCategory();
	void setDirectoryFactoryPresetPreset(const std::string& p_category);
	String getDirectory();
	void setWildCard(String p_wildcard);
	void resetScrollPosition();
	void highlightFirstEntry();
	String getFirstSubDirectoryAndHighlightIt();
	String getSubDirectoryAndHighlightItFromName(String p_name);
	void setType(BrowserType p_type);

	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

	std::function<void(String)> passValueToPatchBrowser;
	std::function<void(String)> passDeleteToPatchBrowser;
	std::function<void(String)> onCreateNewFile;
	std::function<void(String)> onExport;
	std::function<void(String)> onImport;
	std::function<void(String, String)> onMove;
	std::function<void(String, String)> onCopy;
	std::function<void(String, String, String)> applyRenamingSelector;

	void regenerateContent();
	void positionEntries();

	void applyInputField();

	void setButtonTooltips(String p_left, String p_mid, String p_right);
	//void focusLost(FocusChangeType p_cause) override;

	void setWarningTexts(String p_empty, String p_nonexistent);
	void setCopyTargetName(String p_name);
	void setCopyMoveEnabled(bool p_enabled);
	void enablePassActiveNameToParent(bool p_enable);

	String getCopyFileString(int p_popupmenu_index);
	String getMoveFileString(int p_popupmenu_index);
	String getCopyMoveMap(int p_index);


private:
	void generateContent();
	void generateContentFactoryPresetCategories();
	void unhighlightAllEntries();
	void enforceScrollLimits();
	void showButtons(bool p_show);
	void checkDirectoryStatus();
	void recreatePopupMenu();

	float m_scroll_position = 0.f;
	float m_max_scroll_position = 0.f;
	float m_scroll_bar_height = 0.f;
	float m_scroll_bar_position = 0.f;
	float m_available_scroll_height = 0.f;


	String m_absolute_path;
	File::TypesOfFileToFind m_file_or_dir;

	FileElementComparatorAlphabetical m_file_comparator;

	String m_wildcard = "*";

	std::vector<std::unique_ptr<BrowserEntry>> m_entries;

	PopupMenu m_menu;
	OdinMenuFeels m_menu_feels;
	OdinBrowserButtonFeels m_button_feels;

	TextEditor m_input_field;

	TextButton m_left_button;
	TextButton m_mid_button;
	TextButton m_right_button;

	PatchBrowserScrollBar m_scroll_bar;

	bool m_GUI_big = true;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowserSelector)

	bool m_show_left_button = false;
	bool m_show_mid_button  = false;

	bool m_input_field_active = false;

	bool m_copy_move_enabled = true;
	std::map<int, String> m_copy_move_map;

	String m_empty_text       = "";
	String m_nonexistent_text = "";
	String m_copy_target_name = "";

	bool m_pass_active_element_to_parent = false;
	BrowserType m_browser_type;

	//auto generated header with the factory preset names
    #include "FactoryPresetNames.h"

	DirectoryStatus m_directory_status = DirectoryStatus::Nonexistent;
};
