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

#include "../ConfigFileManager.h"
#include "UIAssetManager.h"
#include <JuceHeader.h>

class StepLED : public Component {
public:
	StepLED() {
	}

	~StepLED() {
	}

	void enablementChanged() override {
		repaint();
	}

	void paint(Graphics &g) override {
		const auto asset = m_LED_on ? UIAssets::Indices::LED_on : UIAssets::Indices::LED_off;
		g.setColour(Colours::white.withAlpha(isEnabled() ? 1.0f : MODULE_DISABLED_ALPHA));
		g.drawImageAt(UIAssetManager::getInstance()->getUIAsset(asset, ConfigFileManager::getInstance().getOptionGuiScale()), 0, 0);
	}

	void resized() override {
	}

	void setLEDOn(bool p_on) {
		if (m_LED_on != p_on) {
			m_LED_on = p_on;
			//DBG("REPAINT");
			repaint();
		}
	}

private:
	bool m_LED_on = false;

	juce::Image m_image_on;
	juce::Image m_image_off;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StepLED)
};
