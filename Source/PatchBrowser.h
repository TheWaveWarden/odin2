#pragma once

#include <JuceHeader.h>
#include "PatchBrowserSelector.h"
#include "PluginProcessor.h"

#define DEFAULT_SOUNDBANK_LOCATION_STRING (ODIN_STORAGE_PATH + File::getSeparatorString() + "Soundbanks")

#define DEFAULT_EXPORT_LOCATION_STRING (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName())
//make this one different as to not spark confusion between "folders" and "osb"
#define DEFAULT_SOUNDBANK_IMPORT_LOCATION_STRING (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName())

class PatchBrowser    : public Component
{
public:
    PatchBrowser(OdinAudioProcessor &p_processor, AudioProcessorValueTreeState &p_vts);
    ~PatchBrowser();

    void paint (Graphics&) override;
    void paintOverChildren (Graphics&) override;

    void setGUIBig();
    void setGUISmall();

	std::function<void()> forceValueTreeLambda;


private:
    void loadPatchWithFileBrowserAndCopyToCategory(String p_directory);
    void loadPatchFromOpenedFileStream(juce::FileInputStream &p_file_stream);
    void savePatchInOpenedFileStream(FileOutputStream &p_file_stream);
    void loadSoundbankWithFileBrowser(String p_directory);

    bool checkForBiggerVersion(FileInputStream &p_file_stream, std::string &p_version_string);
    bool checkForSmallerVersion(FileInputStream &p_file_stream, std::string &p_version_string);

    bool usesWavedraw(int p_osc);
    bool usesChipdraw(int p_osc);
    bool usesSpecdraw(int p_osc);

    void setFirstSoundbankActive();

	std::unique_ptr<FileChooser> m_filechooser;

    Image m_background;

    bool m_GUI_big = false;

	OdinAudioProcessor &m_audio_processor;
	AudioProcessorValueTreeState &m_value_tree;

    PatchBrowserSelector m_soundbank_selector;
    PatchBrowserSelector m_category_selector;
    PatchBrowserSelector m_patch_selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchBrowser)
};