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

#include "TitleButton.h"
#include "../PluginProcessor.h"

TitleButton::TitleButton() : juce::Button("TitleButton") {
	onClick = [](){
		juce::URL url(TWW_HOMEPAGE_URL);
		url.launchInDefaultBrowser();
	};

	setMouseCursor(juce::MouseCursor::StandardCursorType::PointingHandCursor);

	setTooltip("Go to TheWaveWarden homepage");
}

void TitleButton::paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) {
	if(shouldDrawButtonAsHighlighted)
		g.drawImageAt(m_hover_image, 0, 0);
}