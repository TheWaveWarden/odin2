/*
  ==============================================================================

    SyncTimeSelector.cpp
    Created: 6 Mar 2019 10:31:49pm
    Author:  frot

  ==============================================================================
*/

#include "SyncTimeSelector.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalIncludes.h"

//==============================================================================
SyncTimeSelector::SyncTimeSelector() :
    m_up_left("up_left", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_down_left("down_left", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_up_right("up_right", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_down_right("down_right", juce::DrawableButton::ButtonStyle::ImageRaw) {

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_mid_png, BinaryData::glaspanel_mid_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(up_1.getWidth(), 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);
	m_display.setColor(STANDARD_DISPLAY_COLOR);
	addAndMakeVisible(m_display);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up_left.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up_left.setClickingTogglesState(true);
	m_up_left.setBounds(3, 1, up_1.getWidth(), up_1.getHeight());
	addAndMakeVisible(m_up_left);
	m_up_left.setTriggeredOnMouseDown(false);
	m_up_left.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up_left.onClick = [&]() { incrementLeft(); };
	m_up_left.setAlwaysOnTop(true);

	m_up_right.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up_right.setClickingTogglesState(true);
	m_up_right.setBounds(up_1.getWidth() + glas_panel.getWidth() - 2, 1, up_1.getWidth(), up_1.getHeight());
	addAndMakeVisible(m_up_right);
	m_up_right.setTriggeredOnMouseDown(false);
	m_up_right.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up_right.onClick = [&]() { incrementRight(); };
	m_up_right.setAlwaysOnTop(true);

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down_left.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down_left.setClickingTogglesState(true);
	m_down_left.setBounds(3, 1 + down_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	addAndMakeVisible(m_down_left);
	m_down_left.setTriggeredOnMouseDown(false);
	m_down_left.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down_left.onClick = [&]() { decrementLeft(); };
	m_down_left.setAlwaysOnTop(true);

	//setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight());

	m_down_right.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down_right.setClickingTogglesState(true);
	m_down_right.setBounds(
	    up_1.getWidth() + glas_panel.getWidth() - 2, 1 + down_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	addAndMakeVisible(m_down_right);
	m_down_right.setTriggeredOnMouseDown(false);
	m_down_right.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down_right.onClick = [&]() { decrementRight(); };
	m_down_right.setAlwaysOnTop(true);

	updateDisplay();
	setSize(glas_panel.getWidth() + 2 * down_1.getWidth(), glas_panel.getHeight());
}

SyncTimeSelector::~SyncTimeSelector() {
}


void SyncTimeSelector::setGUIBig(){
	m_GUI_big = true;

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_150_png, BinaryData::buttonup_2_150_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_150_png, BinaryData::buttonup_1_150_pngSize);

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_mid_150_png, BinaryData::glaspanel_mid_150_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(up_1.getWidth() - 2 , - 1, glas_panel.getWidth(), glas_panel.getHeight() - 1);
	m_display.setInlay(1);
	m_display.setInlayTop(2);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up_left.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up_left.setBounds(3, 1, up_1.getWidth(), up_1.getHeight());

	m_up_right.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up_right.setBounds(up_1.getWidth() + glas_panel.getWidth() - 4, 1, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_150_png, BinaryData::buttondown_2_150_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_150_png, BinaryData::buttondown_1_150_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down_left.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down_left.setBounds(3, 1 + up_1.getHeight(), down_1.getWidth(), down_1.getHeight());

	m_down_right.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down_right.setBounds(
	    up_1.getWidth() + glas_panel.getWidth() - 4, 1 + up_1.getHeight(), down_1.getWidth(), down_1.getHeight());

	m_display.setTextOffsetTop(2);
	m_display.setTextOffsetLeft(2);
	m_display.setGUIBig();

	updateDisplay();
	setSize(glas_panel.getWidth() + 2 * down_1.getWidth(), glas_panel.getHeight());


}
void SyncTimeSelector::setGUISmall(){
	m_GUI_big = false;


	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_mid_png, BinaryData::glaspanel_mid_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(up_1.getWidth(), 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);
	m_display.setInlayTop(0);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up_left.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up_left.setBounds(3, 1, up_1.getWidth(), up_1.getHeight());

	m_up_right.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up_right.setBounds(up_1.getWidth() + glas_panel.getWidth() - 2, 1, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down_left.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down_left.setBounds(3, 1 + down_1.getHeight(), down_1.getWidth(), down_1.getHeight());

	m_down_right.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down_right.setBounds(
	    up_1.getWidth() + glas_panel.getWidth() - 2, 1 + down_1.getHeight(), down_1.getWidth(), down_1.getHeight());

	m_display.setTextOffsetTop(0);
	m_display.setTextOffsetLeft(0);
	m_display.setGUISmall();

	updateDisplay();
	setSize(glas_panel.getWidth() + 2 * down_1.getWidth(), glas_panel.getHeight());

}