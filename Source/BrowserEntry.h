#pragma once

#include "GlobalIncludes.h"
#include <JuceHeader.h>
#include <functional>

#define TEXT_INLAY_BROWSER_150 20
#define TEXT_INLAY_BROWSER_100 13

class BrowserEntry : public Component {
public:
	BrowserEntry(String p_text);
	~BrowserEntry();

	void paint(Graphics &) override;
	void resized() override;

	void setGUIBig();
	void setGUISmall();

	void mouseEnter(const MouseEvent &e) override;
	void mouseExit(const MouseEvent &e) override;
	void mouseDown(const MouseEvent &e) override;
	void setEntryActive(bool p_active);

	String getText();

	std::function<void()> onLeftClick;
	std::function<void()> onRightClick;

private:

	String m_text;

	bool m_GUI_big = false;
	bool m_is_highlighted = false;
	bool m_is_active = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrowserEntry)
};
