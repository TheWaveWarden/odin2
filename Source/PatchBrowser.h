#pragma once

#include <JuceHeader.h>
#include "PatchBrowserSelector.h"

#define DEFAULT_SOUNDBANK_LOCATION_STRING (File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName() + "/Soundbanks")

class PatchBrowser    : public Component
{
public:
    PatchBrowser();
    ~PatchBrowser();

    void paint (Graphics&) override;

    void setGUIBig();
    void setGUISmall();

private:
    bool m_GUI_big = false;

    PatchBrowserSelector m_soundbank_selector;
    PatchBrowserSelector m_category_selector;
    PatchBrowserSelector m_patch_selector;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatchBrowser)
};
