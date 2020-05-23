#pragma once

#include "BrowserEntry.h"
#include "GlobalIncludes.h"
#include <JuceHeader.h>

#define ENTRY_HEIGHT_150 24
#define ENTRY_HEIGHT_100 16

#define MOUSE_WHEEL_FACTOR_PATCH_BROWSER_150 90.f
#define MOUSE_WHEEL_FACTOR_PATCH_BROWSER_100 60.f

#define BUTTON_HEIGHT_BROWSER_100 18
#define BUTTON_HEIGHT_BROWSER_150 25

#define PATCH_BROWSER_MENU_ENTRY_RENAME 10
#define PATCH_BROWSER_MENU_ENTRY_DELETE 20

#define WARNING_INLAY_X_150 10
#define WARNING_INLAY_X_100 6

#define WARNING_OFFSET_Y_150 50
#define WARNING_OFFSET_Y_100 33

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
	                          bool shouldDrawButtonAsDown) {
		auto cornerSize = 0.f;
		auto bounds     = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

		auto baseColour = Colour(20, 20, 20);

		if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
			baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

		g.setColour(baseColour);

		auto flatOnLeft   = button.isConnectedOnLeft();
		auto flatOnRight  = button.isConnectedOnRight();
		auto flatOnTop    = button.isConnectedOnTop();
		auto flatOnBottom = button.isConnectedOnBottom();

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

	enum class DirectoryStatus{
		Ok,
		Empty,
		Nonexistent
	};

	PatchBrowserSelector(File::TypesOfFileToFind p_file_or_dir,
	                     String p_left_button,
	                     String p_mid_button,
	                     String p_right_button);
	~PatchBrowserSelector();

	void paint(Graphics &) override;

	void setGUIBig();
	void setGUISmall();

	void resetInputFieldAndShow();
	void hideInputField();

	void setDirectory(String p_absolute_path);
	String getDirectory();
	void setWildCard(String p_wildcard);
	void resetScrollPosition();
	String getFirstSubDirectoryAndHighlightIt();
	String getSubDirectoryAndHighlightItFromName(String p_name);

	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

	std::function<void(String)> passValueToPatchBrowser;
	std::function<void(String)> passDeleteToPatchBrowser;
	std::function<void(String)> onCreateNewFile;
	std::function<void(String)> onExport;
	std::function<void(String)> onImport;

	void regenerateContent();
	void positionEntries();

	void applyInputField();

	void setButtonTooltips(String p_left, String p_mid, String p_right );
	//void focusLost(FocusChangeType p_cause) override;

	void setWarningTexts(String p_empty, String p_nonexistent);

private:
	void generateContent();
	void unhighlightAllEntries();
	void enforceScrollLimits();
	void showButtons(bool p_show);
	void checkDirectoryStatus();

	float m_scroll_position = 0.f;

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

	bool m_GUI_big = false;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowserSelector)

	bool m_show_left_button = false;
	bool m_show_mid_button  = false;

	bool m_input_field_active = false;

	String m_empty_text = "";
	String m_nonexistent_text = "";

	DirectoryStatus m_directory_status = DirectoryStatus::Nonexistent;
};
