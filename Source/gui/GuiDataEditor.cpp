#include "GuiDataEditor.h"

GuiDataEditor::GuiDataEditor() {
	m_data = juce::Rectangle<int>(0, 0, 6, 6);
}

GuiDataEditor::~GuiDataEditor() {
}

void GuiDataEditor::paint(juce::Graphics &g) {
	g.fillAll(m_colour_background.withAlpha(0.5f));

	g.setColour(m_colour_border);
	g.drawRect(getLocalBounds(), 1);

	g.setColour(m_colour_text.withAlpha(0.5f));
	g.fillRect(m_data);

	auto textBounds = getLocalBounds().reduced(proportionOfHeight(0.2f));
	textBounds.reduce(textBounds.proportionOfHeight(0.1f), textBounds.proportionOfHeight(0.1f));
	const auto line_height = textBounds.proportionOfHeight(0.33f);
	g.setFont(Helpers::getAldrichFont(line_height * 0.8f));
	const auto just = juce::Justification::centred;

	g.setColour(juce::Colours::lightblue);
	g.drawText("X: " + juce::String(m_data.getX()), textBounds.removeFromTop(line_height), just);
	g.drawText("Y: " + juce::String(m_data.getY()), textBounds.removeFromTop(line_height), just);
	g.drawText(juce::String(m_data.getWidth()) + "x" + juce::String(m_data.getHeight()),
	           textBounds.removeFromTop(line_height),
	           just);
}

void GuiDataEditor::setParent(const juce::Rectangle<int> &p_parent) {
	m_parent = p_parent;
	repaint();
}

void GuiDataEditor::resized() {
}

void GuiDataEditor::mouseDown(const juce::MouseEvent &e) {
	m_data.setX(e.x);
	m_data.setY(e.y);
	m_data.setWidth(1);
	m_data.setHeight(1);

	repaint();
}

void GuiDataEditor::mouseDrag(const juce::MouseEvent &e) {
	m_data.setWidth(e.getDistanceFromDragStartX() / 2);
	m_data.setHeight(e.getDistanceFromDragStartY() / 2);

	repaint();
}

juce::Rectangle<int> GuiDataEditor::getData() const {
	return m_data;
}