#pragma once

#include <JuceHeader.h>
#include "PatchBrowserSelector.h"
#include "PluginProcessor.h"

#define DEFAULT_SOUNDBANK_LOCATION_STRING (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName() + "/Soundbanks")

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
    void loadPatchFromOpenedFileStream(juce::FileInputStream &p_file_stream);
    bool checkForBiggerVersion(FileInputStream &p_file_stream, std::string &p_version_string);
    bool checkForSmallerVersion(FileInputStream &p_file_stream, std::string &p_version_string);

    bool m_GUI_big = false;

	OdinAudioProcessor &m_audio_processor;
	AudioProcessorValueTreeState &m_value_tree;

    PatchBrowserSelector m_soundbank_selector;
    PatchBrowserSelector m_category_selector;
    PatchBrowserSelector m_patch_selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchBrowser)
};
