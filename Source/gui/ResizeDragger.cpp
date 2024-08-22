#include "ResizeDragger.h"

ResizeDragger::ResizeDragger() {
	setMouseCursor(juce::MouseCursor::StandardCursorType::BottomRightCornerResizeCursor);

	m_colour_dragger   = COL_LIGHT.withAlpha(0.5f);
	m_colour_highlight = COL_LIGHT;
}

ResizeDragger::~ResizeDragger() {
}

void ResizeDragger::paint(juce::Graphics &g) {
	g.setColour(m_highlight ? m_colour_highlight : m_colour_dragger.withAlpha(0.5f));

	constexpr auto margin   = 1;
	constexpr auto num_rows = 3;
	const auto s            = W / num_rows;

	auto bounds = getLocalBounds();

	for (int y = 0; y < num_rows; ++y) {
		auto row = bounds.removeFromBottom(s);
		for (int x = 0; x < num_rows; ++x) {
			if (num_rows - y > x)
				g.fillRect(row.removeFromRight(s).reduced(margin));
			else
				row.removeFromRight(s);
		}
	}
}

void ResizeDragger::resized() {
}

bool ResizeDragger::hitTest(int p_x, int p_y) {
	return W - p_x < p_y * 1.2f;
}

void ResizeDragger::mouseDrag(const juce::MouseEvent &p_event) {
	static constexpr auto drag_scale = 70;

	const auto diff_x = p_event.getPosition().getX() - p_event.getMouseDownX();
	const auto diff_y = p_event.getPosition().getY() - p_event.getMouseDownY();

	int drag_value = (diff_x + diff_y) / drag_scale;

	if (drag_value > m_drag_reference) {
		m_drag_reference = drag_value;
		onIncrease();
	} else if (drag_value < m_drag_reference) {
		m_drag_reference = drag_value;
		onDecrease();
	}
}

void ResizeDragger::mouseDown(const juce::MouseEvent &) {
	m_drag_reference = 0;
}

void ResizeDragger::mouseEnter(const juce::MouseEvent &) {
	m_highlight = true;
	repaint();
}
void ResizeDragger::mouseExit(const juce::MouseEvent &) {
	m_highlight = false;
	repaint();
}