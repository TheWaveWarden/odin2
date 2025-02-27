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

#include "PatchBrowserScrollBar.h"
#include <JuceHeader.h>
#include "../ConfigFileManager.h"
#include "UIAssetManager.h"

PatchBrowserScrollBar::PatchBrowserScrollBar() {
}

PatchBrowserScrollBar::~PatchBrowserScrollBar() {
}

void PatchBrowserScrollBar::paint(juce::Graphics &g) {
	g.drawImage(UIAssetManager::getInstance()->getUIAsset(UIAssets::Indices::slider, ConfigFileManager::getInstance().getOptionGuiScale()), getLocalBounds().toFloat());
}

void PatchBrowserScrollBar::mouseDown(const MouseEvent &e) {
	m_drag_reference_y = e.getScreenY() - getY();
}

void PatchBrowserScrollBar::mouseDrag(const MouseEvent &e) {
	reportMouseDrag(m_drag_reference_y - e.getScreenY());
}
