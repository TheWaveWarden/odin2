#include "BrowserEntry.h"
#include <JuceHeader.h>

BrowserEntry::BrowserEntry(String p_text) : m_text(p_text) {
	addChildComponent(m_rename_editor);
	m_rename_editor.setColour(TextEditor::ColourIds::backgroundColourId, PATCH_BROWSER_INPUT_FIELD_BACKGROUND_COLOR);
	m_rename_editor.onFocusLost = [&]() {
		DBG("RENAME FOCUS LOST");
		hideRenameEditor();
	};
	m_rename_editor.onEscapeKey = [&]() { hideRenameEditor(); };
	if (m_GUI_big) {
		m_rename_editor.setFont(Font(24.f));
		m_rename_editor.setIndents(TEXT_INLAY_BROWSER_150, 3);
	} else {
		m_rename_editor.setFont(Font(17.f));
		m_rename_editor.setIndents(TEXT_INLAY_BROWSER_100, 2);
	}
	//m_rename_editor.onReturnKey = [&]() { };
	setWantsKeyboardFocus(true);
}

BrowserEntry::~BrowserEntry() {
}

void BrowserEntry::paint(Graphics &g) {

	if (!m_is_active) {
		if (m_is_highlighted) {
			//g.fillAll(Colour(35, 35, 35));
			g.fillAll(Colour(22, 22, 22));
			g.setColour(Colours::grey);
			g.drawRect(getLocalBounds(), 1); // draw an outline around the component
		}
		g.setColour(Colours::white);
	} else {
		//g.fillAll(Colour(35, 35, 35));
		g.fillAll(Colour(22, 22, 22));
		g.setColour(ODIN_BLUE);
		g.drawRect(getLocalBounds(), 1); // draw an outline around the component
	}

	if (m_GUI_big) {
		g.setFont(24.0f);
	} else {
		g.setFont(18.0f);
	}
	auto text_area = getLocalBounds();

	if (m_GUI_big) {
		text_area.removeFromLeft(TEXT_INLAY_BROWSER_150);
		text_area.removeFromRight(TEXT_INLAY_BROWSER_150);
	} else {
		text_area.removeFromLeft(TEXT_INLAY_BROWSER_100);
		text_area.removeFromRight(TEXT_INLAY_BROWSER_100);
	}

	g.drawText(m_text, text_area, Justification::centredLeft, true); // draw some placeholder text
}

void BrowserEntry::resized() {
}

void BrowserEntry::setGUIBig() {
	m_GUI_big = true;

	m_rename_editor.setBounds(0, 0, getWidth(), getHeight());

	repaint();
}

void BrowserEntry::setGUISmall() {
	m_GUI_big = false;

	repaint();
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
		m_is_active = true;
	} else if (e.mods.isRightButtonDown()) {
		onRightClick();
	}
	Component::mouseDown(e);
	repaint();
}

void BrowserEntry::setEntryActive(bool p_active) {
	m_is_active = p_active;
	repaint();
}

String BrowserEntry::getText() {
	return m_text;
}

void BrowserEntry::showRenameEditor() {
	m_rename_editor.setVisible(true);
	m_rename_editor.setText(getText());
	m_rename_editor.grabKeyboardFocus();
}

void BrowserEntry::hideRenameEditor() {
	m_rename_editor.setVisible(false);
}

void BrowserEntry::setBoundsWithInputField(int p_x, int p_y, int p_width, int p_height) {
	Component::setBounds(p_x, p_y, p_width, p_height);
	m_rename_editor.setBounds(0, 0, p_width, p_height);
}
