/*
  ==============================================================================

    SaveLoadComponent.h
    Created: 27 Feb 2019 11:01:28am
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"
#include "GlasDisplay.h"

#define LOAD_POS_X 134
#define LOAD_POS_Y 5
#define SAVE_POS_X 3
#define SAVE_POS_Y LOAD_POS_Y
#define PATCH_POS_X 39
#define PATCH_POS_Y 4
#define RESET_TOP_POS_X 170
#define RESET_TOP_POS_Y LOAD_POS_Y
#define RANDOM_POS_X 218
#define RANDOM_POS_Y LOAD_POS_Y


//==============================================================================
/*
 */
class SaveLoadComponent : public Component {
public:
  SaveLoadComponent(AudioProcessorValueTreeState &vts);
  ~SaveLoadComponent();

  void paint(Graphics &) override {}
  void resized() override;

  std::function<void()> forceValueTreeLambda;

private:

  std::unique_ptr<FileChooser> m_filechooser;

  AudioProcessorValueTreeState &m_value_tree;
  

  juce::DrawableButton m_save;
  juce::DrawableButton m_load;
  juce::DrawableButton m_reset;
  juce::DrawableButton m_random;

  GlasDisplay m_patch;
  int m_patch_size_x;
  int m_patch_size_y;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SaveLoadComponent)
};
