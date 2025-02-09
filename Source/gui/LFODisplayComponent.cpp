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

#include "LFODisplayComponent.h"

LFODisplayComponent::LFODisplayComponent() {
}

LFODisplayComponent::~LFODisplayComponent() {
}

void LFODisplayComponent::paint(Graphics &g) {

	juce::Point<int> top_left = getLocalBounds().getTopLeft();
	top_left.addXY(m_inlay + 1 + m_inlay_top, m_inlay + m_inlay_left);
	juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
	bottom_right.addXY(-m_inlay - 1, -m_inlay);

	if (m_value < m_waves.size()) {
		g.drawImageAt((m_waves[m_value]), 0, 1);
	}
}

void LFODisplayComponent::addWave(juce::Image p_wave) {
	m_waves.push_back(p_wave.createCopy());
}

void LFODisplayComponent::mouseDown(const MouseEvent &event) {
	//onMouseDown();
	toParentMouseDown(event);
	Component::mouseDown(event);
}

void LFODisplayComponent::mouseDrag(const MouseEvent &event) {
	toParentMouseDrag(event);
	Component::mouseDrag(event);
}

void LFODisplayComponent::mouseUp(const MouseEvent &event) {
	toParentMouseUp(event);
	Component::mouseUp(event);

	if (contains(event.getPosition())) {
		onClick();
	}
}