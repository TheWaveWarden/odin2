/*
  ==============================================================================

    SaveLoadComponent.cpp
    Created: 27 Feb 2019 11:01:28am
    Author:  frot

  ==============================================================================
*/

#include "SaveLoadComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <fstream>

std::string getFileNameFromAbsolute(const std::string &s) {

  char sep = '/';

#ifdef ODIN_WIN
  sep = '\\';
#endif

  size_t i = s.rfind(sep, s.length());
  if (i != std::string::npos) {
    std::string ret = s.substr(i + 1, s.length() - i);

    std::string erase = ".odin";
    size_t pos = ret.find(erase);
    if (pos != std::string::npos) {
      ret.erase(pos, erase.length());
    }
    return ret;
  }

  return ("");
}

//==============================================================================
SaveLoadComponent::SaveLoadComponent(AudioProcessorValueTreeState &vts)
    : m_save("save", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_load("load", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_reset("reset", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_random("random", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_value_tree(vts){
  juce::Image save_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonsave_2.png"));
  juce::Image save_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonsave_1.png"));

  juce::DrawableImage save_draw1;
  juce::DrawableImage save_draw2;

  save_draw1.setImage(save_1);
  save_draw2.setImage(save_2);

  m_save.setImages(&save_draw2, &save_draw2, &save_draw1, &save_draw1,
                   &save_draw2, &save_draw2, &save_draw1, &save_draw1);
  m_save.setClickingTogglesState(true);
  m_save.setBounds(SAVE_POS_X, SAVE_POS_Y, save_1.getWidth(),
                   save_1.getHeight());
  addAndMakeVisible(m_save);
  m_save.setTooltip("Save a patch to your harddrive");
  m_save.setTriggeredOnMouseDown(false);
  m_save.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());

  juce::Image load_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonload_2.png"));
  juce::Image load_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonload_1.png"));

  juce::DrawableImage load_draw1;
  juce::DrawableImage load_draw2;

  load_draw1.setImage(load_1);
  load_draw2.setImage(load_2);

  m_load.setImages(&load_draw2, &load_draw2, &load_draw1, &load_draw1,
                   &load_draw2, &load_draw2, &load_draw1, &load_draw1);
  m_load.setClickingTogglesState(true);
  m_load.setBounds(LOAD_POS_X, LOAD_POS_Y, load_1.getWidth(),
                   load_1.getHeight());
  addAndMakeVisible(m_load);
  m_load.setTriggeredOnMouseDown(false);
  m_load.setTooltip("Load a patch from\n your harddrive");
  m_load.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                   juce::Colour());

  juce::Image reset_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_global_2.png"));
  juce::Image reset_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonreset_global_1.png"));

  juce::DrawableImage reset_draw1;
  juce::DrawableImage reset_draw2;

  reset_draw1.setImage(reset_1);
  reset_draw2.setImage(reset_2);

  m_reset.setImages(&reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1,
                    &reset_draw2, &reset_draw2, &reset_draw1, &reset_draw1);
  m_reset.setClickingTogglesState(true);
  m_reset.setBounds(RESET_TOP_POS_X, RESET_TOP_POS_Y, reset_1.getWidth(),
                    reset_1.getHeight());
  m_reset.setTooltip("Reset the synth to its initial state");
  addAndMakeVisible(m_reset);
  m_reset.setTriggeredOnMouseDown(false);
  m_reset.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                    juce::Colour());

  juce::Image random_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonrandom_2.png"));
  juce::Image random_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonrandom_1.png"));

  juce::DrawableImage random_draw1;
  juce::DrawableImage random_draw2;

  random_draw1.setImage(random_1);
  random_draw2.setImage(random_2);

  m_random.setImages(&random_draw2, &random_draw2, &random_draw1, &random_draw1,
                     &random_draw2, &random_draw2, &random_draw1,
                     &random_draw1);
  m_random.setClickingTogglesState(true);
  m_random.setBounds(RANDOM_POS_X, RANDOM_POS_Y, random_1.getWidth(),
                     random_1.getHeight());
  addAndMakeVisible(m_random);
  m_random.setTriggeredOnMouseDown(false);
  m_random.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                     juce::Colour());
  m_random.setTooltip("Generate a random patch");

  juce::Image glas_panel = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/glaspanel_big.png"));

  m_patch.setImage(glas_panel);
  m_patch.setColor(STANDARD_DISPLAY_COLOR);
  m_patch_size_x = glas_panel.getWidth();
  m_patch_size_y = glas_panel.getHeight();
  m_patch.setText("Init Patch");
  addAndMakeVisible(m_patch);

  m_save.onClick = [&]() {
     DBG(m_value_tree.state.toXmlString());
    File fileToSave(File::getCurrentWorkingDirectory().getFullPathName() +
                    "/my_patch.odin");

    m_filechooser.reset(
        new FileChooser("Choose a file to save...",
                        File::getCurrentWorkingDirectory().getChildFile(
                            fileToSave.getFileName()),
                        "*", true));

    m_filechooser->launchAsync(
        FileBrowserComponent::saveMode | FileBrowserComponent::canSelectFiles,
        [fileToSave, this](const FileChooser &chooser) {
          auto result = chooser.getURLResult();
          auto file_name = result.isEmpty()
                               ? String()
                               : (result.isLocalFile()
                                      ? result.getLocalFile().getFullPathName()
                                      : result.toString(true));

          File file_to_write(file_name);
          FileOutputStream file_stream(file_to_write);
          m_value_tree.state.writeToStream(file_stream);

          m_patch.setText(getFileNameFromAbsolute(file_name.toStdString()));

          DBG("Wrote patch to " + file_name);
        });
  };

  m_load.onClick = [&]() {
    m_filechooser.reset(new FileChooser("Choose a file to open...",
                                        File::getCurrentWorkingDirectory(), "*",
                                        true));

    m_filechooser->launchAsync(
        FileBrowserComponent::canSelectMultipleItems |
            FileBrowserComponent::openMode |
            FileBrowserComponent::canSelectFiles,
        [this](const FileChooser &chooser) {
          String file_name;
          auto results = chooser.getURLResults();

          for (auto result : results)
            file_name << (result.isLocalFile()
                              ? result.getLocalFile().getFullPathName()
                              : result.toString(false));

          File file_to_read(file_name);

          FileInputStream file_stream(file_to_read);
          if (file_stream.openedOk()) {
            m_value_tree.state.copyPropertiesAndChildrenFrom(m_value_tree.state.readFromStream(file_stream), nullptr);
            m_patch.setText(
                file_to_read.getFileNameWithoutExtension().toStdString());
            DBG("Loaded patch " + file_name);
          } else {
            DBG("Failed to open stream. Error message: " +
                file_stream.getStatus().getErrorMessage().toStdString());
            DBG(file_name);
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
                                             "File Chooser...",
                                             "Failed to open file!");
          }
          DBG(m_value_tree.state.toXmlString());

          forceValueTreeLambda();
        });
  };
}

SaveLoadComponent::~SaveLoadComponent() {}

void SaveLoadComponent::resized() {
  m_patch.setBounds(PATCH_POS_X, PATCH_POS_Y, m_patch_size_x, m_patch_size_y);
}
