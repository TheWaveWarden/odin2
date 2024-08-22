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
#include <JuceHeader.h>
#include "../GlobalIncludes.h"

class LFOPopupLookAndFeel : public LookAndFeel_V4 {
public:
	void drawPopupMenuItem(Graphics &,
	                       const Rectangle<int> &area,
	                       bool isSeparator,
	                       bool isActive,
	                       bool isHighlighted,
	                       bool isTicked,
	                       bool hasSubMenu,
	                       const String &text,
	                       const String &shortcutKeyText,
	                       const Drawable *icon,
	                       const Colour *textColour) override;

	void getIdealPopupMenuItemSize(
	    const String &text, bool isSeparator, int standardMenuItemHeight, int &idealWidth, int &idealHeight) override;

protected:
};