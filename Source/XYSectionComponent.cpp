/*
  ==============================================================================

    XYSectionComponent.cpp
    Created: 27 Feb 2019 8:44:56pm
    Author:  frot

  ==============================================================================
*/

#include "XYSectionComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
XYSectionComponent::XYSectionComponent(AudioProcessorValueTreeState &vts,
                                       std::string p_section_name)
    : m_value_tree(vts), m_section_name(p_section_name),
      m_xy_pad(m_modx, m_mody) {

  juce::Image glas_panel = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/vectorpanel.png"));
  m_xy_pad.setTopLeftPosition(0, XY_PAD_POSIITON_Y);
  m_xy_pad.setSize(glas_panel.getWidth(), glas_panel.getHeight());
  m_xy_pad.setImage(glas_panel);
  m_xy_pad.setInlay(1);
  m_xy_pad.setTooltip(
      "An XY pad to be used as a modulation source in the modmatrix.");
  addAndMakeVisible(m_xy_pad);

  juce::Image black_knob_very_small = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/knobs/black1/"
                                 "black_knob_very_small.png"));

  m_modx.setStrip(black_knob_very_small, N_KNOB_FRAMES);
  m_modx.setSliderStyle(Slider::RotaryVerticalDrag);
  m_modx.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_modx.setRange(0, 1);
  m_modx.onValueChange = [&] { m_xy_pad.setX(m_modx.getValue()); };
  m_modx.setTooltip("The X value\nof the XY pad");
  addAndMakeVisible(m_modx);

  m_mody.setStrip(black_knob_very_small, N_KNOB_FRAMES);
  m_mody.setSliderStyle(Slider::RotaryVerticalDrag);
  m_mody.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_mody.setRange(0, 1);
  m_mody.onValueChange = [&] { m_xy_pad.setY(m_mody.getValue()); };
  m_mody.setTooltip("The Y value\nof the XY pad");
  addAndMakeVisible(m_mody);

  m_modx.setNumDecimalPlacesToDisplay(3);
  m_mody.setNumDecimalPlacesToDisplay(3);
}

XYSectionComponent::~XYSectionComponent() {}

void XYSectionComponent::paint(Graphics &g) {}

void XYSectionComponent::resized() {
  m_modx.setBounds(MODX_POS_X - BLACK_KNOB_VERY_SMALL_OFFSET_X,
                   MODX_POS_Y - BLACK_KNOB_VERY_SMALL_OFFSET_Y,
                   BLACK_KNOB_VERY_SMALL_SIZE_X, BLACK_KNOB_VERY_SMALL_SIZE_Y);
  m_mody.setBounds(MODY_POS_X - BLACK_KNOB_VERY_SMALL_OFFSET_X,
                   MODY_POS_Y - BLACK_KNOB_VERY_SMALL_OFFSET_Y,
                   BLACK_KNOB_VERY_SMALL_SIZE_X, BLACK_KNOB_VERY_SMALL_SIZE_Y);
}
