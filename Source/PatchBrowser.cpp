#include "PatchBrowser.h"
#include <JuceHeader.h>

PatchBrowser::PatchBrowser(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &p_vts) :
    m_soundbank_selector(File::TypesOfFileToFind::findDirectories),
    m_category_selector(File::TypesOfFileToFind::findDirectories), m_patch_selector(File::TypesOfFileToFind::findFiles),
    m_audio_processor(p_processor), m_value_tree(p_vts)

{
	addAndMakeVisible(m_soundbank_selector);
	addAndMakeVisible(m_category_selector);
	addAndMakeVisible(m_patch_selector);

	m_soundbank_selector.setDirectory(DEFAULT_SOUNDBANK_LOCATION_STRING);
	m_category_selector.setDirectory(m_soundbank_selector.getFirstSubDirectoryAndHighlightIt());
	m_patch_selector.setWildCard("*.odin");
	m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());

	m_soundbank_selector.passValueToPatchBrowser = [&](String p_string) {
		DBG(p_string + " was pressed in soundbank");
		m_category_selector.setDirectory(m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_string);
		m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
	};

	m_category_selector.passValueToPatchBrowser = [&](String p_string) {
		DBG(p_string + " was pressed in category");
		m_patch_selector.setDirectory(m_category_selector.getDirectory() + File::getSeparatorString() + p_string);
	};

	m_patch_selector.passValueToPatchBrowser = [&](String p_string) {
		DBG("Try to open patch: " + m_patch_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_patch_selector.getDirectory() + File::getSeparatorString() + p_string;

		File file_to_open(absolute_path);

		FileInputStream file_stream(file_to_open);
		if (file_stream.openedOk()) {
			loadPatchFromOpenedFileStream(file_stream);
			m_value_tree.state.getChildWithName("misc").setProperty(
			    "current_patch_filename", file_to_open.getFileName(), nullptr);
			DBG("set filename in valuetree: " +
			    m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
		}
	};
}

PatchBrowser::~PatchBrowser() {
}

void PatchBrowser::paint(Graphics &g) {
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

	g.setColour(Colours::white);
	g.setFont(14.0f);
	g.drawText("PatchBrowser", getLocalBounds(), Justification::centred, true); // draw some placeholder text
}

void PatchBrowser::paintOverChildren(Graphics &g) {
	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds().expanded(0,1).translated(0,-1), 1); // draw an outline around the component
}

void PatchBrowser::setGUIBig() {
	m_GUI_big = true;

	m_soundbank_selector.setBounds(0, 0, BROWSER_SIZE_X_150 / 3, 213);
	m_category_selector.setBounds(BROWSER_SIZE_X_150 / 3, 0, BROWSER_SIZE_X_150 / 3, 213);
	m_patch_selector.setBounds((BROWSER_SIZE_X_150 / 3) * 2, 0, BROWSER_SIZE_X_150 / 3, 213);

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

void PatchBrowser::loadPatchFromOpenedFileStream(juce::FileInputStream &p_file_stream) {
	//first see if the patch is of a higher version than we know about:
	std::string version_string;
	if (checkForBiggerVersion(p_file_stream, version_string)) {
		//abort with icon
		AlertWindow::showMessageBox(
		    AlertWindow::AlertIconType::WarningIcon,
		    "Cannot load patch!",
		    "The bad news: You cannot load this patch, because you are on version " + ODIN_VERSION_STRING +
		        ".\nThe good news: The patch you're trying to load was created on version " + version_string +
		        ". So go to www.thewavewarden.com and download the latest version of Odin2!",
		    "Thanks, I will!");
		return;
	}

	// if we're reading an older patch version we set the default patch first
	// since new params might not exist and will only be set
	if (checkForSmallerVersion(p_file_stream, version_string)) {
		DBG("Reading older patch, setting init patch first");
		MemoryInputStream init_stream(BinaryData::init_patch_odin, BinaryData::init_patch_odinSize, false);
		m_audio_processor.readPatch(ValueTree::readFromStream(init_stream));
	}

	//reset stream position
	p_file_stream.setPosition(0);
	m_audio_processor.readPatch(ValueTree::readFromStream(p_file_stream).createCopy());

	//note: 99% percent of patch-loading time is spent here:
	forceValueTreeLambda();

	//set the correct Version number again
	m_value_tree.state.getChildWithName("misc").setProperty("version_minor", ODIN_MINOR_VERSION, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty("version_patch", ODIN_PATCH_VERSION, nullptr);
	m_value_tree.state.getChildWithName("misc").setProperty(
	    "patch_migration_version", ODIN_PATCH_MIGRATION_VERSION, nullptr);

	//this forces values onto the GUI (patch label as well)
	//	forceValueTreeLambda();


}

bool PatchBrowser::checkForBiggerVersion(FileInputStream &p_file_stream, std::string &p_version_string) {
	p_file_stream.setPosition(0);
	auto value_tree_read = ValueTree::readFromStream(p_file_stream);
	int patch_version    = value_tree_read.getChildWithName("misc")["patch_migration_version"];
	if (patch_version > ODIN_PATCH_MIGRATION_VERSION) {
		p_version_string = "2." + std::to_string((int)value_tree_read.getChildWithName("misc")["version_minor"]) + "." +
		                   std::to_string((int)value_tree_read.getChildWithName("misc")["version_patch"]);
		DBG("Trying to load PMV " + std::to_string(patch_version) + ", current PMV is " +
		    std::to_string(ODIN_PATCH_MIGRATION_VERSION));
		return true;
	}
	return false;
}

bool PatchBrowser::checkForSmallerVersion(FileInputStream &p_file_stream, std::string &p_version_string) {
	p_file_stream.setPosition(0);
	auto value_tree_read = ValueTree::readFromStream(p_file_stream);
	int patch_version    = value_tree_read.getChildWithName("misc")["patch_migration_version"];
	if (patch_version < ODIN_PATCH_MIGRATION_VERSION) {
		p_version_string = "2." + std::to_string((int)value_tree_read.getChildWithName("misc")["version_minor"]) + "." +
		                   std::to_string((int)value_tree_read.getChildWithName("misc")["version_patch"]);
		return true;
	}
	return false;
}