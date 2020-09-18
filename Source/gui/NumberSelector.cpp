#include "NumberSelector.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"

NumberSelector::NumberSelector(bool p_buttons_right) :
    m_up("up", juce::DrawableButton::ButtonStyle::ImageRaw),
    m_down("down", juce::DrawableButton::ButtonStyle::ImageRaw) {

	m_display.setInlay(1);
	m_display.setText(std::to_string(m_value));

	m_display.toParentMouseDown = [&](const MouseEvent e) {
		mouse_reference_value = e.getScreenY();
		m_drag_initial_value  = m_value;
		// Component::mouseDown(e);
	};

	m_display.toParentMouseDrag = [&](const MouseEvent e) {
		float mouse_moved = mouse_reference_value - e.getScreenY();

		int new_value = m_drag_initial_value + mouse_moved / m_mouse_drag_divisor;
		new_value     = new_value > m_max ? m_max : new_value;
		new_value     = new_value < m_min ? m_min : new_value;

		setValue(new_value);

		// Component::mouseDrag(e);
	};

	m_display.toParentMouseUp = [&](const MouseEvent e) {};

	addAndMakeVisible(m_display);

	m_up.setClickingTogglesState(true);
	addAndMakeVisible(m_up);
	m_up.setTriggeredOnMouseDown(false);
	m_up.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_up.onClick = [&]() { increment(); };

	m_down.setClickingTogglesState(true);
	addAndMakeVisible(m_down);
	m_down.setTriggeredOnMouseDown(false);
	m_down.setColour(juce::DrawableButton::ColourIds::backgroundOnColourId, juce::Colour());
	m_down.onClick = [&]() { decrement(); };

	m_display.onMouseDown = [&]() {
		// do nothing
	};

	m_buttons_right = p_buttons_right;

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	setValue(1);
}

NumberSelector::~NumberSelector() {
}

void NumberSelector::paint(Graphics &g) {
	if (m_GUI_big) {
		//draw an outline
		g.setColour(juce::Colour(30, 30, 30));
		juce::Rectangle<int> rect = getLocalBounds();
		if (m_buttons_right) {
			rect.setX(42);
			rect.setY(2);
			rect.setHeight(rect.getHeight() - 3);
			rect.setWidth(28);
			g.drawRect(rect, 2);
		} else {
			rect.setX(1);
			rect.setY(2);
			rect.setHeight(rect.getHeight() - 3);
			rect.setWidth(28);
			g.drawRect(rect, 2);
		}
	}
}

void NumberSelector::setGUIBig(){
	m_GUI_big = true;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_small_150_png, BinaryData::glaspanel_small_150_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_150_png, BinaryData::buttonup_2_150_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_150_png, BinaryData::buttonup_1_150_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(glas_panel.getWidth(), 0, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_150_png, BinaryData::buttondown_2_150_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_150_png, BinaryData::buttondown_1_150_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(glas_panel.getWidth(), down_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight() + 1);

	if (m_buttons_right) {
		m_display.setTopLeftPosition(0, 0);
		m_up.setTopLeftPosition(glas_panel.getWidth() - 1, 2);
		m_down.setTopLeftPosition(glas_panel.getWidth() - 1, up_1.getHeight() + 2);
	} else {
		m_display.setTopLeftPosition(down_1.getWidth() - 2, 0);
		m_up.setTopLeftPosition(1, 2);
		m_down.setTopLeftPosition(1, up_1.getHeight() + 2);
	}

	m_display_width = glas_panel.getWidth();
	m_display.setInlayTop(1);
	m_display.setTextOffsetTop(1);
	m_display.setGUIBig();
	
}
void NumberSelector::setGUISmall(){
	m_GUI_big = false;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_small_png, BinaryData::glaspanel_small_pngSize);
	m_display.setImage(glas_panel);
	m_display.setBounds(0, 0, glas_panel.getWidth(), glas_panel.getHeight());
	m_display.setInlay(1);

	juce::Image up_1 = ImageCache::getFromMemory(BinaryData::buttonup_2_png, BinaryData::buttonup_2_pngSize);
	juce::Image up_2 = ImageCache::getFromMemory(BinaryData::buttonup_1_png, BinaryData::buttonup_1_pngSize);

	juce::DrawableImage up_draw1;
	juce::DrawableImage up_draw2;

	up_draw1.setImage(up_1);
	up_draw2.setImage(up_2);

	m_up.setImages(&up_draw2, &up_draw2, &up_draw1, &up_draw1, &up_draw2, &up_draw2, &up_draw1, &up_draw1);
	m_up.setBounds(glas_panel.getWidth(), 0, up_1.getWidth(), up_1.getHeight());

	juce::Image down_1 = ImageCache::getFromMemory(BinaryData::buttondown_2_png, BinaryData::buttondown_2_pngSize);
	juce::Image down_2 = ImageCache::getFromMemory(BinaryData::buttondown_1_png, BinaryData::buttondown_1_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(glas_panel.getWidth(), down_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight() + 1);

	if (m_buttons_right) {
		m_display.setTopLeftPosition(0, 0);
		m_up.setTopLeftPosition(glas_panel.getWidth() - 1, 1);
		m_down.setTopLeftPosition(glas_panel.getWidth() - 1, down_1.getHeight() + 1);
	} else {
		m_display.setTopLeftPosition(down_1.getWidth(), 0);
		m_up.setTopLeftPosition(1, 1);
		m_down.setTopLeftPosition(1, down_1.getHeight() + 1);
	}

	m_display_width = glas_panel.getWidth();
	m_display.setInlayTop(0);
	m_display.setTextOffsetTop(0);
	m_display.setGUISmall();

}
