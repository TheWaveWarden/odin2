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

	m_dropdown.addItem(10, "8 / 1");
	m_dropdown.addItem(20, "4 / 1");
	m_dropdown.addItem(30, "2 / 1");
	m_dropdown.addItem(40, "1 / 1");
	m_dropdown.addItem(50, "1 / 2");
	m_dropdown.addItem(60, "1 / 4");
	m_dropdown.addItem(70, "3 / 16");
	m_dropdown.addItem(80, "1 / 8");
	m_dropdown.addItem(90, "1 / 8T");
	m_dropdown.addItem(100, "1 / 16");
	m_dropdown.addItem(110, "1 / 16T");
	m_dropdown.addItem(120, "1 / 32");
	m_dropdown.addItem(130, "1 / 32T");

	m_display.onMouseDown = [&]() { setValuesFromDropdown(m_dropdown.show()); };

	m_dropdown.setLookAndFeel(&m_menu_feels);

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
	m_dropdown.setLookAndFeel(nullptr);
}

void SyncTimeSelector::setGUIBig() {
	m_GUI_big = true;

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_150_png, BinaryData::buttonup_2_150_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_150_png, BinaryData::buttonup_1_150_pngSize);

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_mid_150_png, BinaryData::glaspanel_mid_150_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(up_1.getWidth() - 2, -1, glas_panel.getWidth(), glas_panel.getHeight() - 1);
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

	juce::Image down_1 =
	    ImageCache::getFromMemory(BinaryData::buttondown_2_150_png, BinaryData::buttondown_2_150_pngSize);
	juce::Image down_2 =
	    ImageCache::getFromMemory(BinaryData::buttondown_1_150_png, BinaryData::buttondown_1_150_pngSize);

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

	m_menu_feels.setGUIBig();

	updateDisplay();
	setSize(glas_panel.getWidth() + 2 * down_1.getWidth(), glas_panel.getHeight());
}
void SyncTimeSelector::setGUISmall() {
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

	m_menu_feels.setGUISmall();

	updateDisplay();
	setSize(glas_panel.getWidth() + 2 * down_1.getWidth(), glas_panel.getHeight());
}

void SyncTimeSelector::setValuesFromDropdown(int p_dropdown_return) {
	switch (p_dropdown_return) {
	case 0:
		return;
		break;

		//std::vector<std::string> m_left_values  = {"1", "2", "3", "4", "5", "6", "7", "8"};
		//std::vector<std::string> m_right_values = {"1", "2", "4", "8", "8T", "16", "16T", "32", "32T"};

	case 10:
		//"8 / 1";
		m_value_left  = 7;
		m_value_right = 0;
		break;
	case 20:
		//"4 / 1";
		m_value_left  = 3;
		m_value_right = 0;
		break;
	case 30:
		//"2 / 1";
		m_value_left  = 1;
		m_value_right = 0;
		break;
	case 40:
		//"1 / 1";
		m_value_left  = 0;
		m_value_right = 0;
		break;
	case 50:
		//"1 / 2";
		m_value_left  = 0;
		m_value_right = 1;
		break;
	case 60:
		//"1 / 4";
		m_value_left  = 0;
		m_value_right = 2;
		break;
	case 70:
		//"3 / 16";
		m_value_left  = 2;
		m_value_right = 5;
		break;
	case 80:
		//"1 / 8";
		m_value_left  = 0;
		m_value_right = 3;
		break;
	case 90:
		//"1 / 8T";
		m_value_left  = 0;
		m_value_right = 4;
		break;
	case 100:
		//"1 / 16";
		m_value_left  = 0;
		m_value_right = 5;
		break;
	case 110:
		//"1 / 16T";
		m_value_left  = 0;
		m_value_right = 6;
		break;
	case 120:
		//"1 / 32";
		m_value_left  = 0;
		m_value_right = 7;
		break;
	case 130:
		//"1 / 32T";
		m_value_left  = 0;
		m_value_right = 8;
		break;
	default:
		return;
	}
	updateDisplay();
	OnValueChange(m_value_left, m_value_right);  
}
