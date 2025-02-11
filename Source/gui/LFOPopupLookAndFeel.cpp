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

#include "LFOPopupLookAndFeel.h"
#include "LFODisplayComponent.h"

void LFOPopupLookAndFeel::drawPopupMenuItem(Graphics &g,
                                            const Rectangle<int> &area,
                                            bool isSeparator,
                                            bool isActive,
                                            bool isHighlighted,
                                            bool isTicked,
                                            bool hasSubMenu,
                                            const String &text,
                                            const String &shortcutKeyText,
                                            const Drawable *icon,
                                            const Colour *textColour) {
	if (isHighlighted && !isSeparator)
		g.setColour(MENU_HIGHLIGHT_BACKGROUND_COLOR);
	else
		g.setColour(MENU_BACKGROUND_COLOR);

	//draw background
	g.fillRect(area);

	if (isSeparator) {
		g.setColour(MENU_FONT_COLOR);
		int padding_x = 3;
		g.drawLine(area.getX() + padding_x, (area.getY() + area.getBottom()) * 0.5f, area.getRight() - padding_x, (area.getY() + area.getBottom()) * 0.5f, 1);
	}

	//select proper image and draw it
	juce::Image waveform(juce::Image::PixelFormat::ARGB, area.getWidth(), area.getHeight(), true);
	juce::Graphics waveform_g(waveform);

	if (!isSeparator) {
		if (text == "si") {
			LFODisplayComponent::drawSineImage(waveform_g, COL_TEXT_BLUE);
		} else if (text == "sa") {
			LFODisplayComponent::drawSawImage(waveform_g, COL_TEXT_BLUE);
		} else if (text == "tr") {
			LFODisplayComponent::drawTriangleImage(waveform_g, COL_TEXT_BLUE);
		} else if (text == "sq50") {
			LFODisplayComponent::drawSquareImage(waveform_g, COL_TEXT_BLUE, 0.5f);
		} else if (text == "sq25") {
			LFODisplayComponent::drawSquareImage(waveform_g, COL_TEXT_BLUE, 0.25f);
		} else if (text == "sq12") {
			LFODisplayComponent::drawSquareImage(waveform_g, COL_TEXT_BLUE, 0.125f);
		} else if (text == "pe") {
			LFODisplayComponent::drawPointyImage(waveform_g, COL_TEXT_BLUE);
		} else if (text == "sh") {
		} else if (text == "p4") {
		} else if (text == "p6") {
		} else if (text == "p8") {
		} else if (text == "p12") {
		} else if (text == "s3") {
		} else if (text == "s4") {
		} else if (text == "s6") {
		} else if (text == "s8") {
		} else if (text == "s12") {
		} else if (text == "w1") {
		} else if (text == "w2") {
		} else if (text == "w3") {
		} else if (text == "c1") {
		} else if (text == "c2") {
		} else if (text == "c3") {
		}

		g.drawImageAt(waveform, area.getX(), area.getY());
	}

	//draw edge
	g.setColour(MENU_FONT_COLOR);
	if (isHighlighted && !isSeparator) {
		g.drawRect(area, 1);
	} else {
		g.drawLine(area.getX(), area.getY(), area.getX(), area.getBottom(), 1);
		g.drawLine(area.getRight(), area.getY(), area.getRight(), area.getBottom(), 1);
	}
}

void LFOPopupLookAndFeel::getIdealPopupMenuItemSize(const String &text, bool isSeparator, int standardMenuItemHeight, int &idealWidth, int &idealHeight) {

	if (isSeparator) {
		return LookAndFeel_V4::getIdealPopupMenuItemSize(text, isSeparator, standardMenuItemHeight, idealWidth, idealHeight);
	}
}