#include "NumberSelectorWithText.h"
#include <JuceHeader.h>

//==============================================================================
NumberSelectorWithText::NumberSelectorWithText() : NumberSelector(true) {
	m_display.setInlay(1);

	m_display.toParentMouseDown = [&](const MouseEvent e) {
		// mouse_reference_value = e.getScreenY();
		// m_drag_initial_value  = m_value;
		// Component::mouseDown(e);
	};

	m_display.toParentMouseDrag = [&](const MouseEvent e) {
		// float mouse_moved = mouse_reference_value - e.getScreenY();

		// if (mouse_moved > m_mouse_drag_divisor) {
		// 	increment();
		// 	mouse_reference_value = e.getScreenY();
		// } else if (mouse_moved < -m_mouse_drag_divisor) {
		// 	decrement();
		// 	mouse_reference_value = e.getScreenY();
		// }

		//		Component::mouseDrag(e);
	};

	// m_display.toParentMouseUp = [&](const MouseEvent e) {};
	m_display.onMouseDown = [&]() {
		m_dropdown.clear();
		for (int index = m_legal_values.size() - 1; index >= 0; --index) {
			//ugly hack to 
			// a) avoid 0 being used as value (reserved for "nothing selected")
			// b) make ordering inside menu the same as up/down buttons
			m_dropdown.addItem(9999 - m_legal_values[index], valueToText(m_legal_values[index]));
		}
		int selected = 9999 - m_dropdown.show();
		if (selected != 9999) {
			DBG(selected);
			setValue(selected);
		}
	};

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

	m_buttons_right = true;

	m_up.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);
	m_down.setRepeatSpeed(BUTTON_REPEAT_INITIAL_DELAY, BUTTON_REPEAT_DELAY);

	m_dropdown.setLookAndFeel(&m_menu_feels);

	m_value = 1;
}

NumberSelectorWithText::~NumberSelectorWithText() {
	m_dropdown.setLookAndFeel(nullptr);
}

void NumberSelectorWithText::resized() {
	// This method is where you should set the bounds of any child
	// components that your component contains..
}

void NumberSelectorWithText::increment() {
	setValue(m_increment_map[m_value]);
}

void NumberSelectorWithText::decrement() {
	setValue(m_decrement_map[m_value]);
}

void NumberSelectorWithText::setValue(int p_value) {
	//just some safety...
	bool value_legal = false;
	for (auto value : m_legal_values) {
		value_legal = (p_value == value) ? true : value_legal;
	}
	if (!value_legal) {
		//this should never happen (unless patch with unison = 12 is loaded)
		int nearest_value = 0;
		float nearest_dist = 999;
		for (auto value : m_legal_values) {
			if(fabs(value - p_value) < nearest_dist){
				nearest_value = value;
				nearest_dist = fabs(value - p_value);
			}
		}
		m_value = nearest_value;
	} else {
		m_value = p_value;
	}
	//m_display.setText("Unison: " + std::to_string(m_value));
	m_display.setText(valueToText(m_value));
	OnValueChange(m_value);
}

void NumberSelectorWithText::setGUIBig() {
	m_GUI_big = true;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_150_png, BinaryData::glaspanel_midbig_150_pngSize);
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

	juce::Image down_1 =
	    ImageCache::getFromMemory(BinaryData::buttondown_2_150_png, BinaryData::buttondown_2_150_pngSize);
	juce::Image down_2 =
	    ImageCache::getFromMemory(BinaryData::buttondown_1_150_png, BinaryData::buttondown_1_150_pngSize);

	juce::DrawableImage down_draw1;
	juce::DrawableImage down_draw2;

	down_draw1.setImage(down_1);
	down_draw2.setImage(down_2);

	m_down.setImages(
	    &down_draw2, &down_draw2, &down_draw1, &down_draw1, &down_draw2, &down_draw2, &down_draw1, &down_draw1);
	m_down.setBounds(glas_panel.getWidth(), down_1.getHeight(), down_1.getWidth(), down_1.getHeight());
	setSize(glas_panel.getWidth() + down_1.getWidth(), glas_panel.getHeight() + 2);

	m_display.setTopLeftPosition(0, 1);
	m_up.setTopLeftPosition(glas_panel.getWidth() - 2, 1);
	m_down.setTopLeftPosition(glas_panel.getWidth() - 2, up_1.getHeight() + 1);

	m_display_width = glas_panel.getWidth();
	m_display.setInlayTop(1);
	m_display.setTextOffsetTop(1);
	m_display.setGUIBig();

	m_menu_feels.setGUIBig();
}
void NumberSelectorWithText::setGUISmall() {
	m_GUI_big = false;

	juce::Image glas_panel =
	    ImageCache::getFromMemory(BinaryData::glaspanel_midbig_png, BinaryData::glaspanel_midbig_pngSize);
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

	m_display.setTopLeftPosition(0, 0);
	m_up.setTopLeftPosition(glas_panel.getWidth() - 1, 1);
	m_down.setTopLeftPosition(glas_panel.getWidth() - 1, down_1.getHeight() + 1);

	m_display_width = glas_panel.getWidth();
	m_display.setInlayTop(0);
	m_display.setTextOffsetTop(0);
	m_display.setGUISmall();

	m_menu_feels.setGUISmall();
}
