/*
  ==============================================================================

    ModMatrixComponent.cpp
    Created: 27 Feb 2019 1:27:55am
    Author:  frot

  ==============================================================================
*/

#include "ModMatrixComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
ModMatrixComponent::ModMatrixComponent(AudioProcessorValueTreeState &vts)
    : m_clear_button0("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button1("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button2("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button3("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button4("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button5("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button6("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button7("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_clear_button8("clear_button",
                      juce::DrawableButton::ButtonStyle::ImageRaw),
      m_value_tree(vts), m_amount_1_identifier0("amount_1_[0]"),
      m_amount_2_identifier0("amount_2_[0]"),
      m_amount_3_identifier0("amount_3_[0]"),
      m_amount_1_identifier1("amount_1_[1]"),
      m_amount_2_identifier1("amount_2_[1]"),
      m_amount_3_identifier1("amount_3_[1]"),
      m_amount_1_identifier2("amount_1_[2]"),
      m_amount_2_identifier2("amount_2_[2]"),
      m_amount_3_identifier2("amount_3_[2]"),
      m_amount_1_identifier3("amount_1_[3]"),
      m_amount_2_identifier3("amount_2_[3]"),
      m_amount_3_identifier3("amount_3_[3]"),
      m_amount_1_identifier4("amount_1_[4]"),
      m_amount_2_identifier4("amount_2_[4]"),
      m_amount_3_identifier4("amount_3_[4]"),
      m_amount_1_identifier5("amount_1_[5]"),
      m_amount_2_identifier5("amount_2_[5]"),
      m_amount_3_identifier5("amount_3_[5]"),
      m_amount_1_identifier6("amount_1_[6]"),
      m_amount_2_identifier6("amount_2_[6]"),
      m_amount_3_identifier6("amount_3_[6]"),
      m_amount_1_identifier7("amount_1_[7]"),
      m_amount_2_identifier7("amount_2_[7]"),
      m_amount_3_identifier7("amount_3_[7]"),
      m_amount_1_identifier8("amount_1_[8]"),
      m_amount_2_identifier8("amount_2_[8]"),
      m_amount_3_identifier8("amount_3_[8]") {

  // set identifiers
  // for (int i = 0; i < N_ROWS - 1; ++i) {
  //  m_source_identifier[i] = Identifier("source_[" + std::to_string(i) + "]");
  //  m_amount_1_identifier[i] =
  //      Identifier("amount_1_[" + std::to_string(i) + "]");
  //  m_amount_2_identifier[i] =
  //      Identifier("amount_2_[" + std::to_string(i) + "]");
  //  m_amount_3_identifier[i] =
  //      Identifier("amount_3_[" + std::to_string(i) + "]");
  //  m_dest_1_identifier[i] = Identifier("dest_1_[" + std::to_string(i) + "]");
  //  m_dest_2_identifier[i] = Identifier("dest_2__[" + std::to_string(i) +
  //  "]"); m_scale_identifier[i] = Identifier("scale_[" + std::to_string(i) +
  //  "]");
  //}

  juce::Image glas_left_down = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_left_down.png"));
  juce::Image glas_left_mid = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_left_mid.png"));
  juce::Image glas_left_up = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_left_up.png"));
  juce::Image glas_mid_down = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_mid_down.png"));
  juce::Image glas_mid_mid = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_mid_mid.png"));
  juce::Image glas_mid_up = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_mid_up.png"));
  juce::Image glas_right_down = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_right_down.png"));
  juce::Image glas_right_mid = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_right_mid.png"));
  juce::Image glas_right_up = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelbig_right_up.png"));
  juce::Image glas_small_down = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelsmall_down.png"));
  juce::Image glas_small_mid = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelsmall_mid.png"));
  juce::Image glas_small_up = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/modpanelsmall_up.png"));

  m_source[0].setImage(glas_left_up);
  m_amount_1[0].setImage(glas_small_up);
  m_dest_1[0].setImage(glas_mid_up);
  m_amount_2[0].setImage(glas_small_up);
  m_dest_2[0].setImage(glas_mid_up);
  m_amount_3[0].setImage(glas_small_up);
  m_scale[0].setImage(glas_right_up);
  for (int i = 1; i < N_ROWS - 1; ++i) {
    m_source[i].setImage(glas_left_mid);
    m_amount_1[i].setImage(glas_small_mid);
    m_dest_1[i].setImage(glas_mid_mid);
    m_amount_2[i].setImage(glas_small_mid);
    m_dest_2[i].setImage(glas_mid_mid);
    m_amount_3[i].setImage(glas_small_mid);
    m_scale[i].setImage(glas_right_mid);
  }
  m_source[N_ROWS - 1].setImage(glas_left_down);
  m_amount_1[N_ROWS - 1].setImage(glas_small_down);
  m_dest_1[N_ROWS - 1].setImage(glas_mid_down);
  m_amount_2[N_ROWS - 1].setImage(glas_small_down);
  m_dest_2[N_ROWS - 1].setImage(glas_mid_down);
  m_amount_3[N_ROWS - 1].setImage(glas_small_down);
  m_scale[N_ROWS - 1].setImage(glas_right_down);

  juce::Colour modmatrix_color = STANDARD_DISPLAY_COLOR;
  juce::Colour modmatrix_color_bar(90, 150, 180);
  m_color = modmatrix_color;

  for (int i = 0; i < N_ROWS; ++i) {
    m_source[i].setTooltip("Set the mod source");
    m_amount_1[i].setTooltip("Set the mod amount for the first destination");
    m_dest_1[i].setTooltip("Set the first mod destination");
    m_amount_2[i].setTooltip("Set the mod amount for the second destination");
    m_dest_2[i].setTooltip("Set the second mod destination");
    m_amount_3[i].setTooltip(
        "Set how much the scale signal scales the modulation");
    m_scale[i].setTooltip("Set the scale signal");

    m_source[i].addItem("Source", 1);
    m_source[i].addItem("henlo", 2);
    m_source[i].addItem("LFO", 3);
    m_source[i].addItem("last", 1000);
    m_source[i].setEditableText(false);
    m_source[i].setSelectedId(1, dontSendNotification);
    m_source[i].setColor(modmatrix_color);
    m_source[i].setGreyFirstElement(true);
    addAndMakeVisible(m_source[i]);

    m_amount_1[i].setColor(modmatrix_color);
    m_amount_1[i].setColorBar(modmatrix_color_bar);
    addAndMakeVisible(m_amount_1[i]);
    m_dest_1[i].addItem("Dest1", 1);
    m_dest_1[i].addItem("Freq", 2);
    m_dest_1[i].addItem("Pitch", 3);
    m_dest_1[i].setEditableText(false);
    m_dest_1[i].setSelectedId(1, dontSendNotification);
    m_dest_1[i].setColor(modmatrix_color);
    m_dest_1[i].setGreyFirstElement(true);
    addAndMakeVisible(m_dest_1[i]);

    m_amount_2[i].setColor(modmatrix_color);
    m_amount_2[i].setColorBar(modmatrix_color_bar);

    addAndMakeVisible(m_amount_2[i]);
    m_dest_2[i].addItem("Dest2", 1);
    m_dest_2[i].addItem("Freq", 2);
    m_dest_2[i].addItem("Pitch", 3);
    m_dest_2[i].setEditableText(false);
    m_dest_2[i].setSelectedId(1, dontSendNotification);
    m_dest_2[i].setColor(modmatrix_color);
    m_dest_2[i].setGreyFirstElement(true);
    addAndMakeVisible(m_dest_2[i]);

    m_amount_3[i].setColor(modmatrix_color);
    m_amount_3[i].setColorBar(modmatrix_color_bar);
    addAndMakeVisible(m_amount_3[i]);

    m_scale[i].addItem("Scale", 1);
    m_scale[i].addItem("henlo", 2);
    m_scale[i].addItem("LFO", 3);
    m_scale[i].setEditableText(false);
    m_scale[i].setSelectedId(1, dontSendNotification);
    m_scale[i].setColor(modmatrix_color);
    m_scale[i].setGreyFirstElement(true);
    addAndMakeVisible(m_scale[i]);
  }

  m_amount_1[0].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier0)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[0].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier0)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[0].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier0)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[1].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier1)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[1].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier1)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[1].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier1)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[2].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier2)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[2].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier2)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[2].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier2)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[3].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier3)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[3].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier3)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[3].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier3)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[4].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier4)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[4].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier4)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[4].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier4)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[5].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier5)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[5].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier5)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[5].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier5)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[6].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier6)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[6].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier6)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[6].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier6)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[7].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier7)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[7].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier7)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[7].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier7)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_amount_1[8].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_1_identifier8)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_2[8].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_2_identifier8)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };
  m_amount_3[8].onValueChange = [&](float p_value) {
    m_value_tree.getParameter(m_amount_3_identifier8)
        ->setValueNotifyingHost(((float)p_value + 1.f) / 2.f);
  };

  m_small_panel_width = glas_small_down.getWidth();
  m_big_panel_width = glas_mid_down.getWidth();
  m_panel_height = glas_left_down.getHeight();

  juce::Image clear_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonclear_1.png"));
  juce::Image clear_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonclear_2.png"));
  juce::Image clearlast_1 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonclearlast_1.png"));
  juce::Image clearlast_2 = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/buttons/buttonclearlast_2.png"));

  juce::DrawableImage clear_draw1;
  juce::DrawableImage clear_draw2;
  juce::DrawableImage clearlast_draw1;
  juce::DrawableImage clearlast_draw2;

  clear_draw1.setImage(clear_1);
  clear_draw2.setImage(clear_2);
  clearlast_draw1.setImage(clearlast_1);
  clearlast_draw2.setImage(clearlast_2);

  m_clear_button0.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button0.setClickingTogglesState(true);
  m_clear_button0.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            0 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button0);
  m_clear_button0.setTriggeredOnMouseDown(false);
  m_clear_button0.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button1.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button1.setClickingTogglesState(true);
  m_clear_button1.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            1 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button1);
  m_clear_button1.setTriggeredOnMouseDown(false);
  m_clear_button1.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button2.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button2.setClickingTogglesState(true);
  m_clear_button2.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            2 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button2);
  m_clear_button2.setTriggeredOnMouseDown(false);
  m_clear_button2.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button3.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button3.setClickingTogglesState(true);
  m_clear_button3.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            3 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button3);
  m_clear_button3.setTriggeredOnMouseDown(false);
  m_clear_button3.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button4.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button4.setClickingTogglesState(true);
  m_clear_button4.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            4 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button4);
  m_clear_button4.setTriggeredOnMouseDown(false);
  m_clear_button4.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button5.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button5.setClickingTogglesState(true);
  m_clear_button5.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            5 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button5);
  m_clear_button5.setTriggeredOnMouseDown(false);
  m_clear_button5.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button6.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button6.setClickingTogglesState(true);
  m_clear_button6.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            6 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button6);
  m_clear_button6.setTriggeredOnMouseDown(false);
  m_clear_button6.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button7.setImages(&clear_draw2, &clear_draw2, &clear_draw1,
                            &clear_draw1, &clear_draw2, &clear_draw2,
                            &clear_draw1, &clear_draw1);
  m_clear_button7.setClickingTogglesState(true);
  m_clear_button7.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            7 * m_panel_height, clear_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button7);
  m_clear_button7.setTriggeredOnMouseDown(false);
  m_clear_button7.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button8.setImages(
      &clearlast_draw2, &clearlast_draw2, &clearlast_draw1, &clearlast_draw1,
      &clearlast_draw2, &clearlast_draw2, &clearlast_draw1, &clearlast_draw1);
  m_clear_button8.setClickingTogglesState(true);
  m_clear_button8.setBounds(4 * m_big_panel_width + 3 * m_small_panel_width,
                            8 * m_panel_height, clearlast_1.getWidth(),
                            m_panel_height);
  addAndMakeVisible(m_clear_button8);
  m_clear_button8.setTriggeredOnMouseDown(false);
  m_clear_button8.setColour(
      juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());

  m_clear_button0.onClick = [&]() { clearRow(0); };
  m_clear_button1.onClick = [&]() { clearRow(1); };
  m_clear_button2.onClick = [&]() { clearRow(2); };
  m_clear_button3.onClick = [&]() { clearRow(3); };
  m_clear_button4.onClick = [&]() { clearRow(4); };
  m_clear_button5.onClick = [&]() { clearRow(5); };
  m_clear_button6.onClick = [&]() { clearRow(6); };
  m_clear_button7.onClick = [&]() { clearRow(7); };
  m_clear_button8.onClick = [&]() { clearRow(8); };

  m_clear_button0.setTooltip("Clear the entire modmatrix row");
  m_clear_button1.setTooltip("Clear the entire modmatrix row");
  m_clear_button2.setTooltip("Clear the entire modmatrix row");
  m_clear_button3.setTooltip("Clear the entire modmatrix row");
  m_clear_button4.setTooltip("Clear the entire modmatrix row");
  m_clear_button5.setTooltip("Clear the entire modmatrix row");
  m_clear_button6.setTooltip("Clear the entire modmatrix row");
  m_clear_button7.setTooltip("Clear the entire modmatrix row");
  m_clear_button8.setTooltip("Clear the entire modmatrix row");

  for (int i = 0; i < N_ROWS; ++i) {
    m_source_attach[i].reset(new ComboBoxAttachment(
        m_value_tree, "source_[" + std::to_string(i) + "]", m_source[i]));
    m_dest_1_attach[i].reset(new ComboBoxAttachment(
        m_value_tree, "dest_1_[" + std::to_string(i) + "]", m_dest_1[i]));
    m_dest_2_attach[i].reset(new ComboBoxAttachment(
        m_value_tree, "dest_2_[" + std::to_string(i) + "]", m_dest_2[i]));
    m_scale_attach[i].reset(new ComboBoxAttachment(
        m_value_tree, "scale_[" + std::to_string(i) + "]", m_scale[i]));
  }
}

ModMatrixComponent::~ModMatrixComponent() {}

void ModMatrixComponent::paint(Graphics &g) {
  g.setColour(m_color);
  auto background = getLocalBounds();
  background.removeFromRight(19);
  g.fillRect(background);
}

void ModMatrixComponent::resized() {

  for (int i = 0; i < N_ROWS; ++i) {
    m_source[i].setBounds(0, i * m_panel_height, m_big_panel_width,
                          m_panel_height);
    m_amount_1[i].setBounds(m_big_panel_width, i * m_panel_height,
                            m_small_panel_width, m_panel_height);
    m_dest_1[i].setBounds(m_big_panel_width + m_small_panel_width,
                          i * m_panel_height, m_big_panel_width,
                          m_panel_height);
    m_amount_2[i].setBounds(2 * m_big_panel_width + m_small_panel_width,
                            i * m_panel_height, m_small_panel_width,
                            m_panel_height);
    m_dest_2[i].setBounds(2 * m_big_panel_width + 2 * m_small_panel_width,
                          i * m_panel_height, m_big_panel_width,
                          m_panel_height);
    m_amount_3[i].setBounds(3 * m_big_panel_width + 2 * m_small_panel_width,
                            i * m_panel_height, m_small_panel_width,
                            m_panel_height);
    m_scale[i].setBounds(3 * m_big_panel_width + 3 * m_small_panel_width,
                         i * m_panel_height, m_big_panel_width, m_panel_height);
  }
}

void ModMatrixComponent::clearRow(int p_row) {
  m_source[p_row].reset();
  m_amount_1[p_row].reset();
  m_dest_1[p_row].reset();
  m_amount_2[p_row].reset();
  m_dest_2[p_row].reset();
  m_amount_3[p_row].reset();
  m_scale[p_row].reset();
}
