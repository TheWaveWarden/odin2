#include "BrowserEntry.h"
#include <JuceHeader.h>

BrowserEntry::BrowserEntry(String p_text, bool p_GUI_big) : m_text(p_text), m_GUI_big(p_GUI_big) {
	addChildComponent(m_rename_editor);
	m_rename_editor.setColour(TextEditor::ColourIds::backgroundColourId, PATCH_BROWSER_INPUT_FIELD_BACKGROUND_COLOR);
	m_rename_editor.onFocusLost = [&]() {
		DBG("RENAME FOCUS LOST");
		hideRenameEditor();
	};
	m_rename_editor.onEscapeKey = [&]() { hideRenameEditor(); };
	if (m_GUI_big) {
		m_rename_editor.setFont(Font(18.f));
		m_rename_editor.setIndents(TEXT_INLAY_BROWSER_150, 3);
	} else {
		m_rename_editor.setFont(Font(15.f));
		m_rename_editor.setIndents(TEXT_INLAY_BROWSER_100, 2);
	}
	m_rename_editor.onReturnKey = [&]() {
		if (m_rename_editor.getText().isEmpty()) {
			AlertWindow::showMessageBox(
			    AlertWindow::AlertIconType::WarningIcon, "Empty Name", "Please input a valid name!", "Ok");
			return;
		}
		hideRenameEditor();
		applyRenaming(getText(), m_rename_editor.getText());
	};
	//m_rename_editor.setWantsKeyboardFocus(true);
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
		g.setFont(18.0f);
	} else {
		g.setFont(15.0f);
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

	m_rename_editor.setBounds(0, 0, getWidth(), getHeight());

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
		setEntryActive(true);
	} else if (e.mods.isRightButtonDown()) {
		onRightClick();
	}
	Component::mouseDown(e);
	repaint();
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
