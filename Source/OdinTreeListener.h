/*
** Odin 2 Synthesizer Plugin
** Copyright (C) 2020 TheWaveWarden
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

#include "../JuceLibraryCode/JuceHeader.h"

#include <functional>
#pragma once

class OdinTreeListener : public AudioProcessorValueTreeState::Listener {
public:
	std::function<void(const String &, float)> onValueChange;

	void parameterChanged(const String &parameterID, float newValue) override {
		onValueChange(parameterID, newValue);
	}
};

class OdinTreeListenerNonParam : public ValueTree::Listener {
public:
	std::function<void(ValueTree &, const Identifier &)> onValueChange = [](ValueTree &, const Identifier &) {};

	void valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged, const Identifier &property) override {
		onValueChange(treeWhosePropertyHasChanged, property);
	}

	// need to define these
	void valueTreeChildAdded(ValueTree &parentTree, ValueTree &childWhichHasBeenAdded) override {
	}
	void valueTreeChildRemoved(ValueTree &parentTree,
	                           ValueTree &childWhichHasBeenRemoved,
	                           int indexFromWhichChildWasRemoved) override {
	}
	void valueTreeChildOrderChanged(ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {
	}
	void valueTreeParentChanged(ValueTree &treeWhoseParentHasChanged) override {
	}
};