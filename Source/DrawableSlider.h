/*
  ==============================================================================

    DrawableSlider.h
    Created: 26 Feb 2019 9:46:41pm
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
/*
 */

#include "../JuceLibraryCode/JuceHeader.h"

#define ADSR_LABEL_OFFSET_Y -8

class ADSRFeels : public juce::LookAndFeel_V4
{
public:
  void drawBubble(Graphics &g, BubbleComponent &b,
                  const Point<float> &positionOfTip,
                  const Rectangle<float> &body) override
  {
    g.setColour(MENU_BACKGROUND_COLOR);
    g.fillRect(body); // pmai
    g.setColour(Colour(50, 50, 50));
    g.drawRect(body); // pmai
  }
  int getSliderPopupPlacement(Slider &slider) { return 2; }
/*
  drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height, float sliderPos,
                        float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider &) {

                        }*/
};

class DrawableSlider : public juce::Slider
{
public:
  DrawableSlider();
  ~DrawableSlider();

  void setTextValueSuffix(const String &suffix)
  {
    setNumDecimalPlacesToDisplay(3);
    Slider::setTextValueSuffix(suffix);
  }

  void paint(Graphics &) override;
  // void resized() override;

  void setImage(juce::Image p_handle) { m_handle = p_handle; }

  /*void mouseDown(const MouseEvent &event) override {
    Slider::mouseDown(event);
    //Component *popup_display = getCurrentPopupDisplay();
    //if (popup_display) {
      // static_cast<BubbleComponent*>(popup_display)->setAllowedPlacement (3);
    //  popup_display->setTopLeftPosition(
    //      getScreenX() + getWidth() / 2 - popup_display->getWidth() / 2,
    //      getScreenY() + getHeight() + ADSR_LABEL_OFFSET_Y);
    //}
  }
  void mouseDrag(const MouseEvent &event) override {
    Slider::mouseDrag(event);
    //Component *popup_display = getCurrentPopupDisplay();
    //if (popup_display) {
      // static_cast<BubbleComponent*>(popup_display)->setAllowedPlacement (3);
    //  popup_display->setTopLeftPosition(
    //      getScreenX() + getWidth() / 2 - popup_display->getWidth() / 2,
    //      getScreenY() + getHeight() + ADSR_LABEL_OFFSET_Y);
    //}
  }*/

private:
  ADSRFeels m_feels;
  juce::Image m_handle;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableSlider)
};
