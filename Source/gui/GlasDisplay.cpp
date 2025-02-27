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

#include "GlasDisplay.h"
#include "../JuceLibraryCode/JuceHeader.h"

GlasDisplay::GlasDisplay() {
	setLookAndFeel(&m_menu_feels);
	setRepaintsOnMouseActivity(true);
}

GlasDisplay::~GlasDisplay() {
	setLookAndFeel(nullptr);
}

void GlasDisplay::paint(Graphics &g) {
	const auto alpha = isEnabled() ? 1.0f : MODULE_DISABLED_ALPHA;
	const auto col   = (m_highlight && isEnabled()) ? juce::Colours::white : COL_TEXT_BLUE;
	g.setColour(col.withAlpha(alpha));
	g.setFont(Helpers::getAldrichFont(H * 0.5f));
	g.drawText(m_text, getLocalBounds(), Justification::centred, false);
}

void GlasDisplay::mouseDown(const MouseEvent &event) {
	if (isEnabled()) {
		onMouseDown();
		toParentMouseDown(event);
	}

	Component::mouseDown(event);
}

void GlasDisplay::mouseDrag(const MouseEvent &event) {
	if (isEnabled())
		toParentMouseDrag(event);

	Component::mouseDrag(event);
}

void GlasDisplay::mouseUp(const MouseEvent &event) {
	if (isEnabled())
		toParentMouseUp(event);

	Component::mouseUp(event);
}
