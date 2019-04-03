/*
  ==============================================================================

    FilterComponent.cpp
    Created: 25 Feb 2019 10:06:59am
    Author:  frot

  ==============================================================================
*/

#include "FilterComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
FilterComponent::FilterComponent(AudioProcessorValueTreeState &vts,
                                 std::string p_filter_number)
    : m_value_tree(vts), m_filter_number(p_filter_number), m_vowel_left(false), m_vowel_right(true),
    m_vowel_left_identifier("fil" + p_filter_number + "_vowel_left"),
    m_vowel_right_identifier("fil" + p_filter_number + "_vowel_right")
     {
  juce::Image metal_knob_big = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/knobs/metal3/metal_knob_big.png"));
  juce::Image metal_knob_mid = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/knobs/metal2/metal_knob_mid.png"));
  juce::Image black_knob_small = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "cropped/knobs/black2/black_knob_small.png"));

  m_vel.setStrip(black_knob_small, N_KNOB_FRAMES);
  m_vel.setSliderStyle(Slider::RotaryVerticalDrag);
  m_vel.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_vel.setBounds(VEL_POS_X, VEL_POS_Y, black_knob_small.getWidth(),
                  black_knob_small.getHeight() / N_KNOB_FRAMES);
  m_vel.setKnobTooltip("Velocity sensitivity. Sets how much the MIDI velocity "
                       "value is added to the filter freqency");
  addChildComponent(m_vel);

  m_kbd.setStrip(black_knob_small, N_KNOB_FRAMES);
  m_kbd.setSliderStyle(Slider::RotaryVerticalDrag);
  m_kbd.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_kbd.setBounds(KBD_POS_X, KBD_POS_Y, black_knob_small.getWidth(),
                  black_knob_small.getHeight() / N_KNOB_FRAMES);
  m_kbd.setKnobTooltip("Keyboard follow.\nSets how much the MIDI note\n is "
                       "added to the\nfilter frequency");
  addChildComponent(m_kbd);

  m_env.setStrip(black_knob_small, N_KNOB_FRAMES);
  m_env.setSliderStyle(Slider::RotaryVerticalDrag);
  m_env.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_env.setBounds(ENV_POS_X, ENV_POS_Y, black_knob_small.getWidth(),
                  black_knob_small.getHeight() / N_KNOB_FRAMES);
  m_env.setRange(ENV_MIN, ENV_MAX);
  m_env.setValue(ENV_DEFAULT);
  m_env.setSkewFactor(ENV_SKEW, true);
  m_env.setNumDecimalPlacesToDisplay(3);
  m_env.setKnobTooltip("Filter envelope amount.\nSets how much the "
                       "filter\nenvelope modulates the filter");
  addChildComponent(m_env);

  m_gain.setStrip(black_knob_small, N_KNOB_FRAMES);
  m_gain.setSliderStyle(Slider::RotaryVerticalDrag);
  m_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_gain.setBounds(GAIN_POS_X, GAIN_POS_Y, black_knob_small.getWidth(),
                   black_knob_small.getHeight() / N_KNOB_FRAMES);
  m_gain.setRange(GAIN_MIN, GAIN_MAX);
  m_gain.setValue(GAIN_DEFAULT);
  m_gain.setTextValueSuffix(" dB");
  m_gain.setNumDecimalPlacesToDisplay(1);
  m_gain.setKnobTooltip("The volume of\n the filter");
  addChildComponent(m_gain);

  m_freq.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_freq.setSliderStyle(Slider::RotaryVerticalDrag);
  m_freq.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_freq.setSize(metal_knob_big.getWidth(),
                 metal_knob_big.getHeight() / N_KNOB_FRAMES);
  m_freq.setRange(FREQ_MIN, FREQ_MAX);
  m_freq.setSkewFactorFromMidPoint(FREQ_MID);
  m_freq.setTextValueSuffix(" Hz");
  m_freq.setValue(FREQ_DEFAULT);
  m_freq.setDoubleClickReturnValue(true, FREQ_DEFAULT,
                                   ModifierKeys::ctrlModifier);
  m_freq.setNumDecimalPlacesToDisplay(1);
  m_freq.setKnobTooltip("The filter cutoff frequency");
  addChildComponent(m_freq);

  m_res.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_res.setSliderStyle(Slider::RotaryVerticalDrag);
  m_res.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_res.setSize(metal_knob_mid.getWidth(),
                metal_knob_mid.getHeight() / N_KNOB_FRAMES);
  m_res.setKnobTooltip("Creates a resonant peak at the cutoff "
                       "frequency.\nFilters will not\n self oscillate");
  addChildComponent(m_res);

  m_saturation.setStrip(black_knob_small, N_KNOB_FRAMES);
  m_saturation.setSliderStyle(Slider::RotaryVerticalDrag);
  m_saturation.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_saturation.setSize(black_knob_small.getWidth(),
                       black_knob_small.getHeight() / N_KNOB_FRAMES);
  m_saturation.setKnobTooltip(
      "Saturates the filter\nand introduces a\nslight distortion");
  addChildComponent(m_saturation);

  m_sem_transition.setStrip(metal_knob_mid, N_KNOB_FRAMES);
  m_sem_transition.setSliderStyle(Slider::RotaryVerticalDrag);
  m_sem_transition.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_sem_transition.setSize(metal_knob_mid.getWidth(),
                           metal_knob_mid.getHeight() / N_KNOB_FRAMES);
  m_sem_transition.setTopLeftPosition(TRANSITION_POS_X, TRANSITION_POS_Y);
  m_sem_transition.setRange(TRANSITION_MIN, TRANSITION_MAX);
  m_sem_transition.setValue(TRANSITION_DEFAULT);
  m_sem_transition.setDoubleClickReturnValue(true, TRANSITION_DEFAULT,
                                             ModifierKeys::ctrlModifier);
  m_sem_transition.setNumDecimalPlacesToDisplay(3);
  m_sem_transition.setKnobTooltip("Sets the filter characteristic\nfrom a "
                                  "lowpass over a\nnotch to a highpass");
  addChildComponent(m_sem_transition);

  m_formant_transition.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_formant_transition.setSliderStyle(Slider::RotaryVerticalDrag);
  m_formant_transition.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_formant_transition.setSize(metal_knob_big.getWidth(),
                               metal_knob_big.getHeight() / N_KNOB_FRAMES);
  m_formant_transition.setTopLeftPosition(FORMANT_TRANSITION_X,
                                          FORMANT_TRANSITION_Y);
  m_formant_transition.setKnobTooltip(
      "Transitions from the vowel on the\nleft to the one on the right");
  addChildComponent(m_formant_transition);

  m_vowel_left.OnValueChange = [&](int p_new_value){
      m_value_tree.getParameter(m_vowel_left_identifier)->setValueNotifyingHost(((float)p_new_value) / 7.f);    
  };
  m_vowel_left.setTopLeftPosition(FORMANT_VOW_LEFT_POS_X,
                                  FORMANT_VOW_LEFT_POS_Y);
  m_vowel_left.setTooltip(
      "The vowel to\nthe left side of\nthe transition knob");
  addChildComponent(m_vowel_left);
  m_vowel_left.setValue(0);
  m_vowel_left.setColor(Colour(70, 30, 40));

  m_vowel_right.OnValueChange = [&](int p_new_value){
      m_value_tree.getParameter(m_vowel_right_identifier)->setValueNotifyingHost(((float)p_new_value) / 7.f);    
  };
  m_vowel_right.setTopLeftPosition(FORMANT_VOW_RIGHT_POS_X,
                                   FORMANT_VOW_RIGHT_POS_Y);
  m_vowel_right.setTooltip(
      "The vowel to\nthe right side of\nthe transition knob");
  addChildComponent(m_vowel_right);
  m_vowel_right.setValue(2);
  m_vowel_right.setColor(Colour(60, 20, 18));

  m_vel_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_vel", m_vel));
  m_kbd_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_kbd", m_kbd));
  m_gain_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_gain", m_gain));
  m_freq_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_freq", m_freq));
  m_res_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_res", m_res));
  m_saturation_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_saturation", m_saturation));
  m_formant_transition_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_formant_transition", m_formant_transition));
  m_sem_transition_attach.reset (new SliderAttachment (m_value_tree, "osc"+m_filter_number+"_sem_transition", m_sem_transition));
  


}

FilterComponent::~FilterComponent() {}

void FilterComponent::paint(Graphics &g) { g.drawImageAt(m_background, 0, 0); }

void FilterComponent::resized() {
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void FilterComponent::setFilterType(int p_filter_type) {
  if (p_filter_type == 0) {
    return;
  }
  if (m_filter_type == p_filter_type) {
    return;
  }
  m_filter_type = p_filter_type;
  hideAllComponents();
  switch (p_filter_type) {
  case 1:
    setFilterBypass();
    break;
  case 2:
    setFilterLP24();
    break;
  case 3:
    setFilterLP12();
    break;
  case 4:
    setFilterBP24();
    break;
  case 5:
    setFilterBP12();
    break;
  case 6:
    setFilterHP24();
    break;
  case 7:
    setFilterHP12();
    break;
  case 8:
    setFilterSEM24();
    break;
  case 9:
    setFilterSEM12();
    break;
  case 10:
    setFilterDiode();
    break;
  case 11:
    setFilterKorg();
    break;
  case 12:
    setFilterComb();
    break;
  case 13:
    setFilterFormant();
    break;
  default:
    setFilterBypass();
    break;
  }
  repaint();
}

void FilterComponent::hideAllComponents() {
  m_vel.setVisible(false);
  m_kbd.setVisible(false);
  m_env.setVisible(false);
  m_gain.setVisible(false);
  m_freq.setVisible(false);
  m_res.setVisible(false);
  m_saturation.setVisible(false);
  m_sem_transition.setVisible(false);
  m_formant_transition.setVisible(false);
  m_vowel_left.setVisible(false);
  m_vowel_right.setVisible(false);
}

void FilterComponent::setFilterBypass() { m_background = m_background_bypass; }

void FilterComponent::setFilterLP24() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/LP24_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterLP12() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/LP12_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterBP24() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/BP24_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterBP12() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/BP12_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterHP24() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/HP24_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterHP12() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/HP12_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterSEM24() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/SEM24_backdrop.png"));
  showSEMFilterComponents();
}

void FilterComponent::setFilterSEM12() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/SEM12_backdrop.png"));
  showSEMFilterComponents();
}

void FilterComponent::setFilterDiode() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/diode_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterKorg() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/korg_backdrop.png"));
  showStandardFilterComponents();
}

void FilterComponent::setFilterComb() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/comb_backdrop.png"));
  showCombFilterComponents();
}

void FilterComponent::setFilterFormant() {
  m_background = ImageCache::getFromFile(
      juce::File(GRAPHICS_PATH + "applied/formant_backdrop.png"));
  showFormantFilterComponents();
}

void FilterComponent::showSEMFilterComponents() {
  m_vel.setVisible(true);
  m_kbd.setVisible(true);
  m_env.setVisible(true);
  m_gain.setVisible(true);
  m_freq.setVisible(true);
  m_res.setVisible(true);
  m_sem_transition.setVisible(true);

  m_freq.setTopLeftPosition(SEM_FREQ_POS_X, SEM_FREQ_POS_Y);
  m_res.setTopLeftPosition(SEM_RES_POS_X, SEM_RES_POS_Y);
}

void FilterComponent::showStandardFilterComponents() {
  m_vel.setVisible(true);
  m_kbd.setVisible(true);
  m_env.setVisible(true);
  m_gain.setVisible(true);
  m_freq.setVisible(true);
  m_res.setVisible(true);
  m_saturation.setVisible(true);

  m_freq.setTopLeftPosition(FILTER_FREQ_POS_X, FILTER_FREQ_POS_Y);
  m_res.setTopLeftPosition(RES_POS_X, RES_POS_Y);
  m_saturation.setTopLeftPosition(SATURATION_POS_X, SATURATION_POS_Y);
}

void FilterComponent::showCombFilterComponents() {
  m_vel.setVisible(true);
  m_kbd.setVisible(true);
  m_env.setVisible(true);
  m_gain.setVisible(true);
  m_freq.setVisible(true);
  m_res.setVisible(true);

  m_freq.setTopLeftPosition(COMB_FREQ_POS_X, FILTER_FREQ_POS_Y);
  m_res.setTopLeftPosition(COMB_RES_POS_X, RES_POS_Y);
}

void FilterComponent::showFormantFilterComponents() {
  m_vel.setVisible(true);
  m_env.setVisible(true);
  m_gain.setVisible(true);
  m_vowel_right.setVisible(true);
  m_vowel_left.setVisible(true);
  m_formant_transition.setVisible(true);
  // todo set smaller filter bounds
}
