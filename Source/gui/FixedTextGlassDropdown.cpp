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

#include "FixedTextGlassDropdown.h"
#include "../ConfigFileManager.h"
#include "UIAssetManager.h"

FixedTextGlassDropdown::FixedTextGlassDropdown(const std::string &p_fixed_text) : m_fixed_text(p_fixed_text) {
}

void FixedTextGlassDropdown::paint(Graphics &g) {
	g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::screen_dropdown_14x4, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);

	const auto col = isMouseOver() ? juce::Colours::white : COL_TEXT_BLUE;
	g.setColour(col);
	g.setFont(Helpers::getAldrichFont(H * 0.5f));

	auto text_bounds = getLocalBounds();
	text_bounds.removeFromRight(H).reduced(proportionOfHeight(0.4f));

	paintTriangle(g);

	g.drawText(m_fixed_text, text_bounds, Justification::centred);
}
