#include "PatchBrowser.h"
#include <JuceHeader.h>

PatchBrowser::PatchBrowser(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &p_vts) :
    m_soundbank_selector(File::TypesOfFileToFind::findDirectories, "Import", "Export", "New"),
    m_category_selector(File::TypesOfFileToFind::findDirectories, "", "", "New"),
    m_patch_selector(File::TypesOfFileToFind::findFiles, "Import", "Export", "Save"), m_audio_processor(p_processor),
    m_value_tree(p_vts)

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

	m_soundbank_selector.passDeleteToPatchBrowser = [&](String p_string) {
		DBG("DELETE directory: " + m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_soundbank_selector.getDirectory() + File::getSeparatorString() + p_string;

		if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon,
		                                 "Delete Category",
		                                 "Are you sure you want to delete the Soundbank " + p_string +
		                                     " and ALL the presets in it?",
		                                 {},
		                                 {},
		                                 {})) {
			File file_to_delete(absolute_path);
			if (file_to_delete.deleteRecursively()) {
				DBG("Directory was deleted!");
				m_soundbank_selector.regenerateContent();

				m_category_selector.setDirectory(m_soundbank_selector.getFirstSubDirectoryAndHighlightIt());
				m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());

				m_soundbank_selector.positionEntries();
			} else {
				DBG("file was NOT deleted!");
			}
		}
	};

	m_category_selector.passDeleteToPatchBrowser = [&](String p_string) {
		DBG("DELETE directory: " + m_category_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_category_selector.getDirectory() + File::getSeparatorString() + p_string;

		if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon,
		                                 "Delete Category",
		                                 "Are you sure you want to delete the category " + p_string +
		                                     " and ALL the presets in it?",
		                                 {},
		                                 {},
		                                 {})) {
			File file_to_delete(absolute_path);
			if (file_to_delete.deleteRecursively()) {
				DBG("Directory was deleted!");
				m_category_selector.regenerateContent();
				m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
				m_category_selector.positionEntries();
			} else {
				DBG("file was NOT deleted!");
			}
		}
	};

	m_patch_selector.passDeleteToPatchBrowser = [&](String p_string) {
		DBG("DELETE patch: " + m_patch_selector.getDirectory() + File::getSeparatorString() + p_string);
		String absolute_path = m_patch_selector.getDirectory() + File::getSeparatorString() + p_string;

		if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon,
		                                 "Delete Preset",
		                                 "Are you sure you want to delete preset " + p_string + "?",
		                                 {},
		                                 {},
		                                 {})) {
			File file_to_delete(absolute_path);
			if (file_to_delete.deleteFile()) {
				DBG("file was deleted!");
				m_patch_selector.regenerateContent();
			} else {
				DBG("file was NOT deleted!");
			}
		}
	};

	m_patch_selector.onCreateNewFile = [&](String p_string) {
		DBG("Save patch: " + p_string);

		File file_to_write(p_string + ".odin");

		//check whether file already exists
		if (file_to_write.existsAsFile()) {
			if (!(AlertWindow::showOkCancelBox(AlertWindow::WarningIcon,
			                                   "Preset already exists!",
			                                   "Are you sure you want to overwrite it?",
			                                   {},
			                                   {},
			                                   {}))) {
				//user selected cancel
				return;
			}
		}
		FileOutputStream file_stream(file_to_write);
		if (file_stream.openedOk()) {
			savePatchInOpenedFileStream(file_stream);
			DBG("Wrote above patch to " + p_string + ".odin");
			m_patch_selector.regenerateContent();
			m_patch_selector.getSubDirectoryAndHighlightItFromName(p_string);
		}
	};

	m_category_selector.onCreateNewFile = [&](String p_string) {
		DBG("create directory: " + p_string);

		File dir_to_create(p_string);

		//check whether directory already exists
		if (dir_to_create.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
		                            "Category Exists!",
		                            "The category you're trying to create already exists!",
		                            "Ok");
			return;
		}
		if (dir_to_create.createDirectory()) {
			DBG("Created directory " + p_string);
			m_category_selector.regenerateContent();
			m_category_selector.getSubDirectoryAndHighlightItFromName(p_string);
			m_patch_selector.setDirectory(p_string);
		}
	};


	m_soundbank_selector.onCreateNewFile = [&](String p_string) {
		DBG("create directory: " + p_string);

		File dir_to_create(p_string);

		//check whether directory already exists
		if (dir_to_create.isDirectory()) {
			AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon,
		                            "Soundbank Exists!",
		                            "The soundbank you're trying to create already exists!",
		                            "Ok");
			return;
		}
		if (dir_to_create.createDirectory()) {
			DBG("Created directory " + p_string);
			m_soundbank_selector.regenerateContent();
			m_soundbank_selector.getSubDirectoryAndHighlightItFromName(p_string);
			m_category_selector.setDirectory(p_string);
			//actually we know this one is empty...
			m_patch_selector.setDirectory(m_category_selector.getFirstSubDirectoryAndHighlightIt());
		}
	};
}

PatchBrowser::~PatchBrowser() {
}

void PatchBrowser::paint(Graphics &g) {
	// g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

	// g.setColour(Colours::white);
	// g.setFont(14.0f);
	// g.drawText("PatchBrowser", getLocalBounds(), Justification::centred, true); // draw some placeholder text
}

void PatchBrowser::paintOverChildren(Graphics &g) {
	//g.setColour(Colours::grey);
	//g.drawRect(getLocalBounds().expanded(0, 1).translated(0, -1), 1); // draw an outline around the component
	g.drawImageAt(m_background, 0, 0);
}

void PatchBrowser::setGUIBig() {
	m_GUI_big = true;

	m_soundbank_selector.setBounds(
	    BROWSER_INLAY_X_150 - 1, BROWSER_INLAY_Y_150, (BROWSER_SIZE_X_150 - BROWSER_INLAY_X_150 * 2) / 3, 213);
	m_category_selector.setBounds(BROWSER_INLAY_X_150 - 1 + (BROWSER_SIZE_X_150 - BROWSER_INLAY_X_150 * 2) / 3,
	                              BROWSER_INLAY_Y_150,
	                              (BROWSER_SIZE_X_150 - BROWSER_INLAY_X_150 * 2) / 3,
	                              213);
	m_patch_selector.setBounds(BROWSER_INLAY_X_150 - 1 + ((BROWSER_SIZE_X_150 - BROWSER_INLAY_X_150 * 2) / 3) * 2,
	                           BROWSER_INLAY_Y_150,
	                           (BROWSER_INLAY_X_150 + BROWSER_SIZE_X_150 - BROWSER_INLAY_X_150 * 2) / 3,
	                           213);

	m_soundbank_selector.setGUIBig();
	m_category_selector.setGUIBig();
	m_patch_selector.setGUIBig();

	m_background = ImageCache::getFromMemory(BinaryData::patch_browser_window_150_png,
	                                         BinaryData::patch_browser_window_150_pngSize);
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

void PatchBrowser::savePatchInOpenedFileStream(FileOutputStream &p_file_stream) {
	// use this to overwrite old content
	p_file_stream.setPosition(0);
	p_file_stream.truncate();

	//write patch name onto valuetree
	m_value_tree.state.getChildWithName("misc").setProperty(
	    "patch_name", p_file_stream.getFile().getFileNameWithoutExtension(), nullptr);
	DBG(m_value_tree.state.getChildWithName("misc")["patch_name"].toString());

	//make a deep copy and remove the midi_learn part and file name
	ValueTree copy_with_removed_params = m_value_tree.state.createCopy();
	copy_with_removed_params.removeChild(copy_with_removed_params.getChildWithName("midi_learn"), nullptr);
	copy_with_removed_params.getChildWithName("misc").removeProperty("current_patch_filename", nullptr);
	copy_with_removed_params.getChildWithName("misc").removeProperty("current_patch_directory", nullptr);

	//remove draw osc params if they aren't needed
	for (int osc = 1; osc < 4; ++osc) {
		std::string osc_string = std::to_string(osc);

		if (!usesWavedraw(osc)) {
			//DBG("uses wavedraw " + String(osc));
			for (int step = 0; step < WAVEDRAW_STEPS_X; ++step) {
				copy_with_removed_params.getChildWithName("draw").removeProperty(
				    String("osc" + osc_string + "_wavedraw_values_" + std::to_string(step)), nullptr);
			}
		}
		if (!usesChipdraw(osc)) {
			//DBG("uses chipdraw " + String(osc));
			for (int step = 0; step < CHIPDRAW_STEPS_X; ++step) {
				copy_with_removed_params.getChildWithName("draw").removeProperty(
				    String("osc" + osc_string + "_chipdraw_values_" + std::to_string(step)), nullptr);
			}
		}
		if (!usesSpecdraw(osc)) {
			//DBG("uses specdraw " + String(osc));
			for (int step = 0; step < SPECDRAW_STEPS_X; ++step) {
				copy_with_removed_params.getChildWithName("draw").removeProperty(
				    String("osc" + osc_string + "_specdraw_values_" + std::to_string(step)), nullptr);
			}
		}
	}

	//write valuetree into file
	copy_with_removed_params.writeToStream(p_file_stream);

	//set label
	//m_patch.setText(m_value_tree.state.getChildWithName("misc")["patch_name"].toString().toStdString());

	//save load directory
	//m_last_directory = file_to_write.getParentDirectory().getFullPathName();

	DBG(copy_with_removed_params.toXmlString());

	m_value_tree.state.getChildWithName("misc").setProperty("current_patch_filename",
	                                                        p_file_stream.getFile().getFileName(),
	                                                        nullptr); //needed for up/down buttons in patch loading
	m_value_tree.state.getChildWithName("misc").setProperty("current_patch_directory",
	                                                        p_file_stream.getFile().getParentDirectory().getFullPathName(),
	                                                        nullptr); //needed for up/down buttons in patch loading
	DBG("set filename in valuetree: " +
	    m_value_tree.state.getChildWithName("misc")["current_patch_filename"].toString());
	DBG("set filepath in valuetree: " +
	    m_value_tree.state.getChildWithName("misc")["current_patch_directory"].toString());
}

bool PatchBrowser::usesWavedraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) ==
	    OSC_TYPE_WAVEDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use wavedraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 600 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 600 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 600 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 600 + p_osc) {
				return true;
			}
		}
	}

	//wavedraw can be used by LFOs as well
	for (int lfo = 1; lfo < 5; ++lfo) {
		if ((int)(m_value_tree.state.getChildWithName("lfo")[String("lfo" + std::to_string(lfo) + "_wave")]) ==
		    16 + p_osc) {
			return true;
		}
	}

	return false;
}

bool PatchBrowser::usesChipdraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) ==
	    OSC_TYPE_CHIPDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use chipdraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 700 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 700 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_CHIPTUNE) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_chipwave")]) == 700 + p_osc) {
				return true;
			}
		}
	}

	return false;
}

bool PatchBrowser::usesSpecdraw(int p_osc) {
	if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(p_osc) + "_type")]) ==
	    OSC_TYPE_SPECDRAW) {
		return true;
	}

	for (int osc_slot = 1; osc_slot < 4; ++osc_slot) {
		//these oscs can all use wavedraw:
		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_VECTOR) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_a")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_b")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_c")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_vec_d")]) == 800 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_FM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 800 + p_osc) {
				return true;
			}
		}

		if ((int)(m_value_tree.state.getChildWithName("osc")[String("osc" + std::to_string(osc_slot) + "_type")]) ==
		    OSC_TYPE_PM) {
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_modulator_wave")]) == 800 + p_osc) {
				return true;
			}
			if ((int)(m_value_tree.state.getChildWithName(
			        "osc")[String("osc" + std::to_string(osc_slot) + "_carrier_wave")]) == 800 + p_osc) {
				return true;
			}
		}
	}

	return false;
}