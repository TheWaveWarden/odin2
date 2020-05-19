#include "PatchBrowser.h"
#include <JuceHeader.h>

PatchBrowser::PatchBrowser() :
    m_soundbank_selector(File::TypesOfFileToFind::findDirectories),
    m_category_selector(File::TypesOfFileToFind::findDirectories), m_patch_selector(File::TypesOfFileToFind::findFiles)

{
	addAndMakeVisible(m_soundbank_selector);
	addAndMakeVisible(m_category_selector);
	addAndMakeVisible(m_patch_selector);

	m_soundbank_selector.setDirectory(DEFAULT_SOUNDBANK_LOCATION_STRING);
	m_category_selector.setDirectory(DEFAULT_SOUNDBANK_LOCATION_STRING + "/Factory Presets");
	m_patch_selector.setWildCard("*.odin");
	m_patch_selector.setDirectory(DEFAULT_SOUNDBANK_LOCATION_STRING + "/Factory Presets/Arp");

	m_soundbank_selector.passValueToPatchBrowser = [&](String p_string){
		DBG(p_string + " was pressed in soundbank");
		m_category_selector.setDirectory(m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_string);
		m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
	};

	m_category_selector.passValueToPatchBrowser = [&](String p_string){
		DBG(p_string + " was pressed in category");
		m_patch_selector.setDirectory(m_category_selector.getDirectory() + File::getSeparatorString() + p_string);
	};

	m_patch_selector.passValueToPatchBrowser = [&](String p_string){
		DBG("Open patch: " + m_patch_selector.getDirectory() + File::getSeparatorString() + p_string);
	};
}

PatchBrowser::~PatchBrowser() {
}

void PatchBrowser::paint(Graphics &g) {
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1); // draw an outline around the component

	g.setColour(Colours::white);
	g.setFont(14.0f);
	g.drawText("PatchBrowser", getLocalBounds(), Justification::centred, true); // draw some placeholder text
}

void PatchBrowser::setGUIBig() {
	m_GUI_big = true;


	m_soundbank_selector.setBounds(0, 0, 250, 213);
	m_category_selector.setBounds(250, 0, 250, 213);
	m_patch_selector.setBounds(500, 0, 250, 213);
	
	m_soundbank_selector.setGUIBig();
	m_category_selector.setGUIBig();
	m_patch_selector.setGUIBig();
}
void PatchBrowser::setGUISmall() {
	m_GUI_big = false;

	m_soundbank_selector.setGUISmall();
	m_category_selector.setGUISmall();
	m_patch_selector.setGUISmall();
}