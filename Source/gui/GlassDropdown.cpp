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

#include "GlassDropdown.h"
#include "../ConfigFileManager.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "UIAssetManager.h"

GlassDropdown::GlassDropdown(Type p_type) : m_type(p_type) {
	setLookAndFeel(&m_menu_feels);

	switch (m_type) {
	case Type::dropdown_12x4:
		m_asset = UIAssets::Indices::screen_dropdown_12x4;
		break;
	case Type::dropdown_14x4:
		m_asset = UIAssets::Indices::screen_dropdown_14x4;
		break;
	}
}

GlassDropdown::~GlassDropdown() {
	setLookAndFeel(nullptr);
}

void GlassDropdown::paint(Graphics &g) {
	if (m_type != Type::unassigned) {
		auto background = UIAssetManager::getInstance()->getUIAsset(m_asset, ConfigFileManager::getInstance().getOptionGuiScale());
		g.drawImageAt(background, 0, 0);
	}

	const auto col = isMouseOver() ? juce::Colours::white : COL_TEXT_BLUE;
	g.setColour(col);
	g.setFont(H * 0.54f);

	auto text_bounds = getLocalBounds();
	text_bounds.removeFromRight(H).reduced(proportionOfHeight(0.4f));

	paintTriangle(g);

	if (getSelectedId() == 0) {
		g.drawText(m_default_text, text_bounds, Justification::centred, true);
	} else {
		g.drawText(getText(), text_bounds, Justification::centred, true);
	}
	g.drawImageAt(m_glaspanel, 0, 0);
}

void GlassDropdown::paintTriangle(juce::Graphics &g) {
	auto triangle_bounds = getLocalBounds().removeFromRight(H).reduced(proportionOfHeight(0.32f), proportionOfHeight(0.4f));

	// draw the triangle
	juce::Path triangle;
	triangle.addTriangle(triangle_bounds.getX(),
	                     triangle_bounds.getY(),
	                     triangle_bounds.getRight(),
	                     triangle_bounds.getY(),
	                     triangle_bounds.getX() + triangle_bounds.getWidth() / 2,
	                     triangle_bounds.getBottom());
	g.fillPath(triangle);
}
