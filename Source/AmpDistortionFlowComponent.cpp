/*
  ==============================================================================

    AmpDistortionFlowComponent.cpp
    Created: 25 Feb 2019 8:09:19pm
    Author:  frot

  ==============================================================================
*/

#include "AmpDistortionFlowComponent.h"
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
AmpDistortionFlowComponent::AmpDistortionFlowComponent(
    AudioProcessorValueTreeState &vts)
    : m_flow_left("flow_left", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_flow_right("flow_right", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_distortion("distortion", juce::DrawableButton::ButtonStyle::ImageRaw),
      m_value_tree(vts) {

  juce::Image flow_left_1 = ImageCache::getFromMemory(
      BinaryData::buttonleft_1_png, BinaryData::buttonleft_1_pngSize);
  juce::Image flow_left_2 = ImageCache::getFromMemory(
      BinaryData::buttonleft_2_png, BinaryData::buttonleft_2_pngSize);
  juce::Image flow_left_3 = ImageCache::getFromMemory(
      BinaryData::buttonleft_3_png, BinaryData::buttonleft_3_pngSize);
  juce::Image flow_left_4 = ImageCache::getFromMemory(
      BinaryData::buttonleft_4_png, BinaryData::buttonleft_4_pngSize);

  juce::DrawableImage flow_left_draw1;
  juce::DrawableImage flow_left_draw2;
  juce::DrawableImage flow_left_draw3;
  juce::DrawableImage flow_left_draw4;

  flow_left_draw1.setImage(flow_left_1);
  flow_left_draw2.setImage(flow_left_2);
  flow_left_draw3.setImage(flow_left_3);
  flow_left_draw4.setImage(flow_left_4);

  m_flow_left.setImages(&flow_left_draw2, &flow_left_draw2, &flow_left_draw1,
                        &flow_left_draw1, &flow_left_draw4, &flow_left_draw4,
                        &flow_left_draw3, &flow_left_draw3);
  m_flow_left.setClickingTogglesState(true);
  m_flow_left.setBounds(FLOW_LEFT_POS_X, FLOW_POS_Y, flow_left_1.getWidth(),
                        flow_left_1.getHeight());
  addAndMakeVisible(m_flow_left);
//   m_flow_left.setTooltip("Routes filter 2 into the amplifier");
  m_flow_left.setTooltip("");
  m_flow_left.setAlwaysOnTop(true);
  m_flow_left.setTriggeredOnMouseDown(true);
  m_flow_left.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                        juce::Colour());

  juce::Image flow_right_1 = ImageCache::getFromMemory(
      BinaryData::buttonright_1_png, BinaryData::buttonright_1_pngSize);
  juce::Image flow_right_2 = ImageCache::getFromMemory(
      BinaryData::buttonright_2_png, BinaryData::buttonright_2_pngSize);
  juce::Image flow_right_3 = ImageCache::getFromMemory(
      BinaryData::buttonright_3_png, BinaryData::buttonright_3_pngSize);
  juce::Image flow_right_4 = ImageCache::getFromMemory(
      BinaryData::buttonright_4_png, BinaryData::buttonright_4_pngSize);

  juce::DrawableImage flow_right_draw1;
  juce::DrawableImage flow_right_draw2;
  juce::DrawableImage flow_right_draw3;
  juce::DrawableImage flow_right_draw4;

  flow_right_draw1.setImage(flow_right_1);
  flow_right_draw2.setImage(flow_right_2);
  flow_right_draw3.setImage(flow_right_3);
  flow_right_draw4.setImage(flow_right_4);

  m_flow_right.setImages(&flow_right_draw2, &flow_right_draw2,
                         &flow_right_draw1, &flow_right_draw1,
                         &flow_right_draw4, &flow_right_draw4,
                         &flow_right_draw3, &flow_right_draw3);
  m_flow_right.setClickingTogglesState(true);
  m_flow_right.setBounds(FLOW_RIGHT_POS_X, FLOW_POS_Y, flow_right_1.getWidth(),
                         flow_right_1.getHeight());
  addAndMakeVisible(m_flow_right);
  m_flow_right.setTooltip("Routes filter 1 into the amplifier.\n Note that "
                          "filter 1 can also\nbe routed into filter 2");
  m_flow_right.setAlwaysOnTop(true);
  m_flow_right.setTriggeredOnMouseDown(true);
  m_flow_right.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                         juce::Colour());

  juce::Image distortion_1 = ImageCache::getFromMemory(
      BinaryData::buttondistortion_1_png, BinaryData::buttondistortion_1_pngSize);
  juce::Image distortion_2 = ImageCache::getFromMemory(
      BinaryData::buttondistortion_2_png, BinaryData::buttondistortion_2_pngSize);
  juce::Image distortion_3 = ImageCache::getFromMemory(
      BinaryData::buttondistortion_3_png, BinaryData::buttondistortion_3_pngSize);
  juce::Image distortion_4 = ImageCache::getFromMemory(
      BinaryData::buttondistortion_4_png, BinaryData::buttondistortion_4_pngSize);

  juce::DrawableImage distortion_draw1;
  juce::DrawableImage distortion_draw2;
  juce::DrawableImage distortion_draw3;
  juce::DrawableImage distortion_draw4;

  distortion_draw1.setImage(distortion_1);
  distortion_draw2.setImage(distortion_2);
  distortion_draw3.setImage(distortion_3);
  distortion_draw4.setImage(distortion_4);

  m_distortion.setImages(&distortion_draw2, &distortion_draw2,
                         &distortion_draw1, &distortion_draw1,
                         &distortion_draw4, &distortion_draw4,
                         &distortion_draw3, &distortion_draw3);
  m_distortion.setClickingTogglesState(true);
  m_distortion.setBounds(DISTORTION_POS_X, DISTORTION_POS_Y,
                         distortion_1.getWidth(), distortion_1.getHeight());
  addAndMakeVisible(m_distortion);
  m_distortion.setTooltip("Enables the distortion section");
  m_distortion.setAlwaysOnTop(true);
  m_distortion.setTriggeredOnMouseDown(true);
  m_distortion.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId,
                         juce::Colour());

  juce::Image metal_knob_big = ImageCache::getFromMemory(
      BinaryData::metal_knob_big_png, BinaryData::metal_knob_big_pngSize);
  juce::Image black_knob_mid = ImageCache::getFromMemory(
      BinaryData::black_knob_mid_png, BinaryData::black_knob_mid_pngSize);
  juce::Image round_knob = ImageCache::getFromMemory(
      BinaryData::round_knob_png, BinaryData::round_knob_pngSize);

  m_amp_gain.setStrip(metal_knob_big, N_KNOB_FRAMES);
  m_amp_gain.setSliderStyle(Slider::RotaryVerticalDrag);
  m_amp_gain.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_amp_gain.setRange(AMP_GAIN_MIN, AMP_GAIN_MAX);
  //m_amp_gain.setValue(AMP_GAIN_DEFAULT);
  m_amp_gain.setTextValueSuffix(" dB");
  m_amp_gain.setNumDecimalPlacesToDisplay(1);
  m_amp_gain.setKnobTooltip("The volume of\nthe amplifier");
  addAndMakeVisible(m_amp_gain);

  m_amp_pan.setStrip(black_knob_mid, N_KNOB_FRAMES);
  m_amp_pan.setSliderStyle(Slider::RotaryVerticalDrag);
  m_amp_pan.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_amp_pan.setRange(PAN_MIN, PAN_MAX);
  //m_amp_pan.setValue(PAN_DEFAULT);
  m_amp_pan.setNumDecimalPlacesToDisplay(3);
  m_amp_pan.setKnobTooltip("Pans the sound to the\nleft or the right");
  addAndMakeVisible(m_amp_pan);

  m_amp_vel.setStrip(black_knob_mid, N_KNOB_FRAMES);
  m_amp_vel.setSliderStyle(Slider::RotaryVerticalDrag);
  m_amp_vel.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  m_amp_vel.setKnobTooltip(
      "How much the MIDI velocity\naffects the amplifier gain");
  addAndMakeVisible(m_amp_vel);

  m_boost.setStrip(round_knob, N_KNOB_FRAMES);
  m_boost.setSliderStyle(Slider::RotaryVerticalDrag);
  m_boost.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
//   m_boost.setDoubleClickReturnValue(true, THRESHOLD_DEFAULT,
                   //                     ModifierKeys::ctrlModifier);
  //m_boost.setValue(THRESHOLD_DEFAULT);
  m_boost.setKnobTooltip("Boosts the signal to apply more damage");
  m_boost.setNumDecimalPlacesToDisplay(3);
  addAndMakeVisible(m_boost);


  m_dry_wet.setStrip(round_knob, N_KNOB_FRAMES);
  m_dry_wet.setSliderStyle(Slider::RotaryVerticalDrag);
  m_dry_wet.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
  //m_dry_wet.setValue(DRYWET_DIST_DEFAULT);
//   m_dry_wet.setDoubleClickReturnValue(true, DRYWET_DIST_DEFAULT,
                           //           ModifierKeys::ctrlModifier);
  m_dry_wet.setKnobTooltip("The ratio of processed\nand unprocessed sound");
  addAndMakeVisible(m_dry_wet);

  juce::Image glas_panel = ImageCache::getFromMemory(
      BinaryData::glaspanel_midbig_png, BinaryData::glaspanel_midbig_pngSize);
  m_distortion_algo.setImage(glas_panel);
  m_distortion_algo.setInlay(1);
  m_distortion_algo.addItem("Clamp", 1);
  m_distortion_algo.addItem("Fold", 2);
  m_distortion_algo.addItem("Zero", 3);
  m_distortion_algo.setEditableText(false);
  m_distortion_algo.setSelectedId(1, dontSendNotification);
  m_distortion_algo.setBounds(DIST_ALGO_POS_X, DIST_ALGO_POS_Y,
                              glas_panel.getWidth(), glas_panel.getHeight());

  m_distortion_algo.setColor(juce::STANDARD_DISPLAY_COLOR);
  m_distortion_algo.setTooltip("Select the distortion\nalgorithm to be used");
  m_distortion_algo.onChange = [&](){
      m_value_tree.state.setProperty("dist_algo", m_distortion_algo.getSelectedId(), nullptr);
  };
  addAndMakeVisible(m_distortion_algo);

  m_amp_vel_attach.reset(
      new SliderAttachment(m_value_tree, "amp_vel", m_amp_vel));
  m_amp_gain_attach.reset(
      new SliderAttachment(m_value_tree, "amp_gain", m_amp_gain));
  m_amp_pan_attach.reset(
      new SliderAttachment(m_value_tree, "amp_pan", m_amp_pan));
  m_dist_threshold_attach.reset(
      new SliderAttachment(m_value_tree, "dist_boost", m_boost));
  m_dist_drywet_attach.reset(
      new SliderAttachment(m_value_tree, "dist_drywet", m_dry_wet));

  m_dist_on_attach.reset(
      new ButtonAttachment(m_value_tree, "dist_on", m_distortion));
  m_fil1_to_amp_attach.reset(
      new ButtonAttachment(m_value_tree, "fil1_to_amp", m_flow_right));
  m_fil2_to_amp_attach.reset(
      new ButtonAttachment(m_value_tree, "fil2_to_amp", m_flow_left));


  //m_boost.textFromValueFunction = nullptr;
  m_boost.setNumDecimalPlacesToDisplay(3);
  m_amp_gain.setNumDecimalPlacesToDisplay(2);
  m_dry_wet.setNumDecimalPlacesToDisplay(3);
  m_amp_vel.setNumDecimalPlacesToDisplay(3);
  m_amp_pan.setNumDecimalPlacesToDisplay(3);

  SET_CTR_KEY(m_amp_gain);
  SET_CTR_KEY(m_amp_pan);
  SET_CTR_KEY(m_amp_vel);
  SET_CTR_KEY(m_boost);
  SET_CTR_KEY(m_dry_wet);
      
  forceValueTreeOntoComponents(m_value_tree.state);
}

AmpDistortionFlowComponent::~AmpDistortionFlowComponent() {}

void AmpDistortionFlowComponent::resized() {
  Rectangle<int> area_amp(METAL_KNOB_BIG_SIZE_X, METAL_KNOB_BIG_SIZE_Y);
  Rectangle<int> area_pan(BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
  Rectangle<int> area_vel(BLACK_KNOB_MID_SIZE_X, BLACK_KNOB_MID_SIZE_Y);
  Rectangle<int> area_bias(ROUND_KNOB_SIZE_X, ROUND_KNOB_SIZE_Y);
  Rectangle<int> area_threshold(ROUND_KNOB_SIZE_X, ROUND_KNOB_SIZE_Y);

  area_amp.setPosition(AMP_GAIN_POS_X - METAL_KNOB_BIG_OFFSET_X,
                       AMP_GAIN_POS_Y - METAL_KNOB_BIG_OFFSET_Y);
  area_pan.setPosition(AMP_PAN_POS_X - BLACK_KNOB_MID_OFFSET_X,
                       AMP_PAN_POS_Y - BLACK_KNOB_MID_OFFSET_Y);
  area_vel.setPosition(AMP_VEL_POS_X - BLACK_KNOB_MID_OFFSET_X,
                       AMP_VEL_POS_Y - BLACK_KNOB_MID_OFFSET_Y);
  area_bias.setPosition(BIAS_POS_X - ROUND_KNOB_OFFSET_X,
                        BIAS_POS_Y - ROUND_KNOB_OFFSET_Y);
  area_threshold.setPosition(THRESHOLD_POS_X - ROUND_KNOB_OFFSET_X,
                             THRESHOLD_POS_Y - ROUND_KNOB_OFFSET_Y);

  m_amp_gain.setBounds(area_amp);
  m_amp_pan.setBounds(area_pan);
  m_amp_vel.setBounds(area_vel);
  m_boost.setBounds(area_bias);
  m_dry_wet.setBounds(area_threshold);
}

void AmpDistortionFlowComponent::forceValueTreeOntoComponents(ValueTree p_tree) {

    m_distortion_algo.setValue(m_value_tree.state["dist_algo"]);
    //DBG((int)m_value_tree.state["dist_algo"]);

}
