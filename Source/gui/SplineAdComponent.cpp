/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 - 2021 TheWaveWarden
**
** Odin 2 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Odin 2 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/

#include <typeinfo>

#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "JsonGuiProvider.h"
#include "SplineAdComponent.h"

static constexpr auto DISCOUNT_CODE = "ODIN2SPLINE";

SplineAdComponent::SplineAdComponent() : m_close_button("Close", "X"), m_learn_more_button("LearnMore", "Learn More"), m_copy_clipboard_button("Copy", "Copy") {
	addAndMakeVisible(m_close_button);
	addAndMakeVisible(m_learn_more_button);
	addAndMakeVisible(m_discount_code);
	addAndMakeVisible(m_copy_clipboard_button);

	m_close_button.onClick = [this] { disableAd(); };

	m_learn_more_button.onClick = [this] {
		juce::URL("https://www.thewavewarden.com/spline").launchInDefaultBrowser();
		//disableAd();
	};

	m_copy_clipboard_button.onClick = [this] { juce::SystemClipboard::copyTextToClipboard(DISCOUNT_CODE); };

	m_discount_code.setMultiLine(false);
	m_discount_code.setReadOnly(true);
	m_discount_code.setText(DISCOUNT_CODE);
	m_discount_code.setJustification(juce::Justification::centred);

	m_discount_code.setColour(juce::TextEditor::textColourId, COL_LIGHT);
	m_discount_code.setColour(juce::TextEditor::backgroundColourId, COL_DARK);
	m_discount_code.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
}

SplineAdComponent::~SplineAdComponent() {
}

void SplineAdComponent::disableAd() {
	setVisible(false);

	const auto num_gui_opens = ConfigFileManager::getInstance().getNumGuiOpens();
	if (num_gui_opens >= NUM_SP_AD1)
		ConfigFileManager::getInstance().setOptionSplineAd1Seen(true);
	if (num_gui_opens >= NUM_SP_AD2)
		ConfigFileManager::getInstance().setOptionSplineAd2Seen(true);

	ConfigFileManager::getInstance().saveDataToFile();
}

void SplineAdComponent::paint(Graphics &g) {
	g.setColour(COL_DARK);
	g.fillRect(getLocalBounds());
	g.setColour(COL_LIGHT);

	auto bounds = getLocalBounds();
	bounds      = bounds.reduced(0, bounds.getHeight() * 0.1f);

	g.setFont(Helpers::getAldrichFont(H * 0.04f));
	g.drawText("Enjoying Odin 2? Take Your Sound to the Next Level!", bounds.removeFromTop(proportionOfHeight(0.1f)), Justification::centred);
	bounds.removeFromTop(proportionOfHeight(0.05f));

	const auto slice_height = 0.04f;
	const auto font_height  = slice_height * 0.8f;
	g.setFont(Helpers::getAldrichFont(proportionOfHeight(font_height)));
	g.drawText("If you love creating with Odin 2, you might be interested in ", bounds.removeFromTop(proportionOfHeight(slice_height)), juce::Justification::centred);
	g.drawText("exploring Spline - my latest synthesizer with even more powerful ", bounds.removeFromTop(proportionOfHeight(slice_height)), juce::Justification::centred);
	g.drawText("features and possibilities. Spline is the spiritual successor to Odin 2, ", bounds.removeFromTop(proportionOfHeight(slice_height)), juce::Justification::centred);
	g.drawText("building on top of its engine with much much more to discover.", bounds.removeFromTop(proportionOfHeight(slice_height)), juce::Justification::centred);

	bounds.removeFromTop(proportionOfHeight(slice_height));
	bounds.removeFromTop(proportionOfHeight(slice_height));

	g.drawText("Use the code", bounds.removeFromTop(proportionOfHeight(slice_height)), juce::Justification::centred);

	g.setColour(juce::Colours::lightblue);
	g.setFont(Helpers::getAldrichFont(proportionOfHeight(font_height * 1.3f)));
	//g.drawText(
	//    "ODIN2SPLINE", bounds.removeFromTop(proportionOfHeight(slice_height * 1.7f)), juce::Justification::centred);
	bounds.removeFromTop(proportionOfHeight(slice_height * 1.7f));

	g.setColour(COL_LIGHT);
	g.setFont(Helpers::getAldrichFont(proportionOfHeight(font_height)));
	g.drawText("to get a 10% discount on Spline!", bounds.removeFromTop(proportionOfHeight(slice_height)), juce::Justification::centred);
}

void SplineAdComponent::resized() {
	GET_LOCAL_AREA(m_close_button, "SplineAdCloseButton");
	GET_LOCAL_AREA(m_learn_more_button, "SplineAdLearnMore");
	GET_LOCAL_AREA(m_discount_code, "SplineAdDiscountCode");
	GET_LOCAL_AREA(m_copy_clipboard_button, "SplineAdCopyClipboard");

	m_discount_code.applyFontToAllText(juce::Font(H * 0.04f));
}
