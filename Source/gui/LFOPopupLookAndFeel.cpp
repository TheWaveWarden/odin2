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
	if (isHighlighted && !isSeparator) {
		g.setColour(MENU_HIGHLIGHT_BACKGROUND_COLOR);
	} else {
		g.setColour(MENU_BACKGROUND_COLOR);
	}
	//draw background
	g.fillRect(area);

	if (isSeparator) {
		g.setColour(MENU_FONT_COLOR);
		int padding_x = 3;
		g.drawLine(area.getX() + padding_x,
		           (area.getY() + area.getBottom()) * 0.5f,
		           area.getRight() - padding_x,
		           (area.getY() + area.getBottom()) * 0.5f,
		           1);
	}

	//select proper image and draw it
	Image waveform;

	if (!isSeparator) {
		if (text == "si") {
			waveform = ImageCache::getFromMemory(BinaryData::sine_150_png, BinaryData::sine_150_pngSize);
		} else if (text == "sa") {
			waveform = ImageCache::getFromMemory(BinaryData::saw_150_png, BinaryData::saw_150_pngSize);
		} else if (text == "tr") {
			waveform = ImageCache::getFromMemory(BinaryData::triangle_150_png, BinaryData::triangle_150_pngSize);
		} else if (text == "sq50") {
			waveform = ImageCache::getFromMemory(BinaryData::square50_150_png, BinaryData::square50_150_pngSize);
		} else if (text == "sq25") {
			waveform = ImageCache::getFromMemory(BinaryData::square25_150_png, BinaryData::square25_150_pngSize);
		} else if (text == "sq12") {
			waveform = ImageCache::getFromMemory(BinaryData::square12_150_png, BinaryData::square12_150_pngSize);
		} else if (text == "pe") {
			waveform = ImageCache::getFromMemory(BinaryData::peak_150_png, BinaryData::peak_150_pngSize);
		} else if (text == "sh") {
			waveform = ImageCache::getFromMemory(BinaryData::SH_150_png, BinaryData::SH_150_pngSize);
		} else if (text == "p4") {
			waveform = ImageCache::getFromMemory(BinaryData::pyram4_150_png, BinaryData::pyram4_150_pngSize);
		} else if (text == "p6") {
			waveform = ImageCache::getFromMemory(BinaryData::pyram6_150_png, BinaryData::pyram6_150_pngSize);
		} else if (text == "p8") {
			waveform = ImageCache::getFromMemory(BinaryData::pyram8_150_png, BinaryData::pyram8_150_pngSize);
		} else if (text == "p12") {
			waveform = ImageCache::getFromMemory(BinaryData::pyram12_150_png, BinaryData::pyram12_150_pngSize);
		} else if (text == "s3") {
			waveform = ImageCache::getFromMemory(BinaryData::stair3_150_png, BinaryData::stair3_150_pngSize);
		} else if (text == "s4") {
			waveform = ImageCache::getFromMemory(BinaryData::stair4_150_png, BinaryData::stair4_150_pngSize);
		} else if (text == "s6") {
			waveform = ImageCache::getFromMemory(BinaryData::stair6_150_png, BinaryData::stair6_150_pngSize);
		} else if (text == "s8") {
			waveform = ImageCache::getFromMemory(BinaryData::stair8_150_png, BinaryData::stair8_150_pngSize);
		} else if (text == "s12") {
			waveform = ImageCache::getFromMemory(BinaryData::stair12_150_png, BinaryData::stair12_150_pngSize);
		} else if (text == "w1") {
			waveform = ImageCache::getFromMemory(BinaryData::wavedraw1_150_png, BinaryData::wavedraw1_150_pngSize);
		} else if (text == "w2") {
			waveform = ImageCache::getFromMemory(BinaryData::wavedraw2_150_png, BinaryData::wavedraw2_150_pngSize);
		} else if (text == "w3") {
			waveform = ImageCache::getFromMemory(BinaryData::wavedraw3_150_png, BinaryData::wavedraw3_150_pngSize);
		} else if (text == "c1") {
			waveform = ImageCache::getFromMemory(BinaryData::chipdraw1_150_png, BinaryData::chipdraw1_150_pngSize);
		} else if (text == "c2") {
			waveform = ImageCache::getFromMemory(BinaryData::chipdraw2_150_png, BinaryData::chipdraw2_150_pngSize);
		} else if (text == "c3") {
			waveform = ImageCache::getFromMemory(BinaryData::chipdraw3_150_png, BinaryData::chipdraw3_150_pngSize);
		}
	}
	if (!isSeparator) {
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

void LFOPopupLookAndFeel::getIdealPopupMenuItemSize(
    const String &text, bool isSeparator, int standardMenuItemHeight, int &idealWidth, int &idealHeight) {

	if (isSeparator) {
		return LookAndFeel_V4::getIdealPopupMenuItemSize(
		    text, isSeparator, standardMenuItemHeight, idealWidth, idealHeight);
	}
}