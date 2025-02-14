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

#include "DrawableSlider.h"
#include "../ConfigFileManager.h"
#include "../GlobalIncludes.h"
#include "../PluginProcessor.h"
#include "UIAssetManager.h"

OdinAudioProcessor *DrawableSlider::m_processor;

DrawableSlider::DrawableSlider() {
	setLookAndFeel(&m_feels);

	setPopupDisplayEnabled(true, false, nullptr);
	setNumDecimalPlacesToDisplay(3);

	setVelocityModeParameters(1.0, 1, 0.0, true, ModifierKeys::shiftModifier);

	setTextBoxStyle(NoTextBox, true, 0, 0);
}

DrawableSlider::~DrawableSlider() {
	setLookAndFeel(nullptr);
}

void DrawableSlider::paint(Graphics &g) {
	SET_INTERPOLATION_QUALITY(g)

	const auto handle = UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::fader_cap, ConfigFileManager::getInstance().getOptionGuiScale());
	const auto y      = (1.f - valueToProportionOfLength(getValue())) * (getHeight() - juce::roundToInt(float(handle.getHeight()) * 0.9f));
	g.drawImageAt(handle, 0, y);
}

String DrawableSlider::getTextFromValue(double value) {
	//apparently there's a better solution:
	//https://forum.juce.com/t/setnumdecimalplacestodisplay-not-behaving-solved/33686/2
	return String(value, getNumDecimalPlacesToDisplay()) + getTextValueSuffix();
}