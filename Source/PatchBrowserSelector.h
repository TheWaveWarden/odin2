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

#define PATCH_BROWSER_MENU_ENTRY_DELETE 10

class OdinBrowserButtonFeels : public LookAndFeel_V4 {
	void drawButtonBackground(Graphics &g,
	                          Button &button,
	                          const Colour &backgroundColour,
	                          bool shouldDrawButtonAsHighlighted,
	                          bool shouldDrawButtonAsDown) {
		auto cornerSize = 0.f;
		auto bounds     = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

		auto baseColour = Colour(20,20,20);

		if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
			baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

		g.setColour(baseColour);

		auto flatOnLeft   = button.isConnectedOnLeft();
		auto flatOnRight  = button.isConnectedOnRight();
		auto flatOnTop    = button.isConnectedOnTop();
		auto flatOnBottom = button.isConnectedOnBottom();

		if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom) {
			Path path;
			path.addRoundedRectangle(bounds.getX(),
			                         bounds.getY(),
			                         bounds.getWidth(),
			                         bounds.getHeight(),
			                         cornerSize,
			                         cornerSize,
			                         !(flatOnLeft || flatOnTop),
			                         !(flatOnRight || flatOnTop),
			                         !(flatOnLeft || flatOnBottom),
			                         !(flatOnRight || flatOnBottom));

			g.fillPath(path);

			g.setColour(button.findColour(ComboBox::outlineColourId));
			g.strokePath(path, PathStrokeType(1.0f));
		} else {
			g.fillRoundedRectangle(bounds, cornerSize);

			g.setColour(button.findColour(ComboBox::outlineColourId));
			g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
		}
	}
};

class PatchBrowserSelector : public Component {
public:
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

	void regenerateContent();
	void positionEntries();

	void applyInputField();

private:
	void generateContent();
	void unhighlightAllEntries();
	void enforceScrollLimits();

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
};
