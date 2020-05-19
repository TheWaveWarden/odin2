#include "PatchBrowserSelector.h"
#include <JuceHeader.h>

PatchBrowserSelector::PatchBrowserSelector(File::TypesOfFileToFind p_file_or_dir) : m_file_or_dir(p_file_or_dir) {
}

PatchBrowserSelector::~PatchBrowserSelector() {
}

void PatchBrowserSelector::paint(Graphics &g) {
	g.fillAll(MODMATRIX_COLOR); // clear the background
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1); // draw an outline around the component
}

void PatchBrowserSelector::resized() {
}

void PatchBrowserSelector::setGUIBig() {
	m_GUI_big = true;

	resetScrollPosition();
	positionEntries();
}

void PatchBrowserSelector::setGUISmall() {
	m_GUI_big = false;

	resetScrollPosition();
	positionEntries();
}

void PatchBrowserSelector::setWildCard(String p_wildcard) {
	m_wildcard = p_wildcard;
}

void PatchBrowserSelector::setDirectory(String p_absolute_path) {
	m_absolute_path = p_absolute_path;

	DBG("Set path to " + m_absolute_path);

	resetScrollPosition();
	generateContent();
	repaint();
}

void PatchBrowserSelector::generateContent() {

	removeAllChildren();
	m_entries.clear();

	File current_dir(m_absolute_path);
	if (current_dir.isDirectory()) {

		Array<File> file_array = current_dir.findChildFiles(m_file_or_dir, false, m_wildcard);
		if (file_array.size() > 0) {

			file_array.sort(m_file_comparator);

			for (int file_index = 0; file_index < file_array.size(); ++file_index) {
				m_entries.push_back(std::make_unique<BrowserEntry>(
				    file_array[file_index].getFileName().dropLastCharacters((int)m_wildcard.length() - 1)));
				String return_string           = file_array[file_index].getFileName();
				m_entries[file_index]->onLeftClick = [&, return_string]() {
					//DBG(return_string + " was clicked!");
					passValueToPatchBrowser(return_string);
					unhighlightAllEntries();
				};
			}

			for (int entry = 0; entry < m_entries.size(); ++entry) {
				addAndMakeVisible(*(m_entries[entry].get()));
			}

			positionEntries();

		} else {
			//no files in current dir
		}
	} else {
		//no dir??? whatever, open filebrowser
	}
}

void PatchBrowserSelector::positionEntries() {
	int entry_height = m_GUI_big ? ENTRY_HEIGHT_150 : ENTRY_HEIGHT_100;
	for (int entry = 0; entry < m_entries.size(); ++entry) {
		m_entries[entry]->setBounds(0, m_scroll_position + entry_height * entry, getWidth(), entry_height);
	}
}

void PatchBrowserSelector::resetScrollPosition() {
	m_scroll_position = 0.f;
}

void PatchBrowserSelector::mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) {

	if (m_GUI_big) {
		m_scroll_position += wheel.deltaY * MOUSE_WHEEL_FACTOR_PATCH_BROWSER_150;
	} else {
		m_scroll_position += wheel.deltaY * MOUSE_WHEEL_FACTOR_PATCH_BROWSER_100;
	}

	m_scroll_position = m_scroll_position > 0.f ? 0.f : m_scroll_position;

	int entry_height_single = m_GUI_big ? ENTRY_HEIGHT_150 : ENTRY_HEIGHT_100;
	float entry_height      = m_entries.size() * entry_height_single;

	float bottom = entry_height + m_scroll_position;
	if (bottom < getHeight()) {
		m_scroll_position -= (bottom - getHeight());
		m_scroll_position = m_scroll_position > 0.f ? 0.f : m_scroll_position;
	}

	Component::mouseWheelMove(event, wheel);

	positionEntries();
	repaint();
}

String PatchBrowserSelector::getDirectory() {
	return m_absolute_path;
}

String PatchBrowserSelector::getFirstSubDirectoryAndHighlightIt() {
	if (m_entries.size() > 0) {
		m_entries[0]->setEntryActive(true);
		return getDirectory() + File::getSeparatorString() + m_entries[0]->getText();
	} else {
		return "";
	}
}

void PatchBrowserSelector::unhighlightAllEntries() {
	for (int entry = 0; entry < m_entries.size(); ++entry) {
		m_entries[entry]->setEntryActive(false);
	}
}