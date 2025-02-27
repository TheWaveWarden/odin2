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

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class TooltipFeels : public LookAndFeel_V4 {
public:
	TooltipFeels() {
		setColour(TooltipWindow::ColourIds::textColourId, MENU_FONT_COLOR);
		setColour(TooltipWindow::ColourIds::outlineColourId, MENU_FONT_COLOR);
		setColour(TooltipWindow::ColourIds::backgroundColourId, MENU_BACKGROUND_COLOR);
	}

	void drawTooltip(Graphics &g, const String &text, int width, int height) override {
		Rectangle<int> bounds(width, height);
		auto cornerSize = 0.0f;

		g.setColour(findColour(TooltipWindow::backgroundColourId));
		g.fillRoundedRectangle(bounds.toFloat(), cornerSize);

		g.setColour(findColour(TooltipWindow::outlineColourId));
		g.drawRoundedRectangle(bounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 1.0f);

		layoutTooltipText(text, findColour(TooltipWindow::textColourId)).draw(g, {static_cast<float>(width), static_cast<float>(height)});
	}

	Rectangle<int> getTooltipBounds(const String &tipText, Point<int> screenPos, Rectangle<int> parentArea) override {
		const TextLayout tl(layoutTooltipText(tipText, Colours::black));

		auto w = (int)(tl.getWidth() + 14.0f);
		auto h = (int)(tl.getHeight() + 6.0f);

		return Rectangle<int>(screenPos.x > parentArea.getCentreX() ? screenPos.x - (w + 12) : screenPos.x + 24,
		                      screenPos.y > parentArea.getCentreY() ? screenPos.y - (h + 6) : screenPos.y + 6,
		                      w,
		                      h)
		    .constrainedWithin(parentArea);
	}

	static TextLayout layoutTooltipText(const String &text, Colour colour) noexcept {
		const float tooltipFontSize = 17.0f;
		const int maxToolTipWidth   = 400;

		AttributedString s;
		s.setJustification(Justification::centred);
		s.append(text, Helpers::getAldrichFont(tooltipFontSize), colour);

		TextLayout tl;
		tl.createLayoutWithBalancedLineLengths(s, (float)maxToolTipWidth);
		return tl;
	}
};