#include "RescaleProgressComponent.h"

RescaleProgressComponent::RescaleProgressComponent() {
	setOpaque(true);
}

void RescaleProgressComponent::setProgress(float p_progress) {
	m_progress = p_progress;
	repaint();
}

void RescaleProgressComponent::paint(juce::Graphics &g) {
	auto colour_background = juce::Colour(0xff373838);
	g.setGradientFill(ColourGradient(colour_background.darker(0.4f), 0, H, colour_background.brighter(0.04f), W, 0, false));
	g.fillRect(getLocalBounds());

	g.setFont(Helpers::getAldrichFont(getHeight() * 0.03f));
	g.setColour(juce::Colours::white);

	auto progress_shell  = getLocalBounds().withSizeKeepingCentre(getWidth() * 0.6f, proportionOfHeight(0.03f));
	auto progress_bounds = progress_shell.withRight(progress_shell.getX() + progress_shell.proportionOfWidth(m_progress));

	g.setColour(juce::Colours::black);
	g.fillRect(progress_shell);
	g.setColour(juce::Colours::lightblue);
	g.fillRect(progress_bounds);
	g.setColour(juce::Colours::lightgrey);
	g.drawRect(progress_shell, 1);

	g.drawText("Rescaling UI Assets... ", progress_shell.translated(0, -progress_shell.proportionOfHeight(1.5f)), juce::Justification::centred, true);
	g.drawText(juce::String(m_progress * 100.0f, 1) + "%", progress_shell.translated(0, progress_shell.proportionOfHeight(1.5f)), juce::Justification::centred, true);
}