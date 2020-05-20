#pragma once

#include "BrowserEntry.h"
#include "GlobalIncludes.h"
#include <JuceHeader.h>

#define ENTRY_HEIGHT_150 24
#define ENTRY_HEIGHT_100 16

#define MOUSE_WHEEL_FACTOR_PATCH_BROWSER_150 90.f
#define MOUSE_WHEEL_FACTOR_PATCH_BROWSER_100 60.f

class PatchBrowserSelector : public Component {
public:
	PatchBrowserSelector(File::TypesOfFileToFind p_file_or_dir);
	~PatchBrowserSelector();

	void paint(Graphics &) override;
	void resized() override;

	void setGUIBig();
	void setGUISmall();

	void setDirectory(String p_absolute_path);
	String getDirectory();
	void setWildCard(String p_wildcard);
	void resetScrollPosition();
	String getFirstSubDirectoryAndHighlightIt();


	void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;

	std::function<void(String)> passValueToPatchBrowser;

private:
	void generateContent();
	void positionEntries();
	void unhighlightAllEntries();

	float m_scroll_position = 0.f;

	String m_absolute_path;
	File::TypesOfFileToFind m_file_or_dir;

	FileElementComparatorAlphabetical m_file_comparator;

	String m_wildcard = "*";

	std::vector<std::unique_ptr<BrowserEntry>> m_entries;

	PopupMenu m_menu;
	OdinMenuFeels m_menu_feels;

	bool m_GUI_big = false;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowserSelector)
};
