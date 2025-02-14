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

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableSlider.h"
#include "LeftRightButton.h"
#include "OdinButton.h"
#include "OdinKnob.h"

class OdinKnobAttachment : public SliderAttachment {
public:
	OdinKnobAttachment(AudioProcessorValueTreeState &p_audio_tree_state, const String &p_parameter_ID, OdinKnob &p_slider_to_control) :
	    SliderAttachment(p_audio_tree_state, p_parameter_ID, (Slider &)p_slider_to_control) {
	}
};

class OdinSliderAttachment : public SliderAttachment {
public:
	OdinSliderAttachment(AudioProcessorValueTreeState &p_audio_tree_state, const String &p_parameter_ID, DrawableSlider &p_slider_to_control) :
	    SliderAttachment(p_audio_tree_state, p_parameter_ID, (Slider &)p_slider_to_control) {
	}
};

class OdinButtonAttachment : public ButtonAttachment {
public:
	OdinButtonAttachment(AudioProcessorValueTreeState &p_audio_tree_state, const String &p_parameter_ID, OdinButton &p_button_to_control) :
	    ButtonAttachment(p_audio_tree_state, p_parameter_ID, (Button &)p_button_to_control) {
	}

	OdinButtonAttachment(AudioProcessorValueTreeState &p_audio_tree_state, const String &p_parameter_ID, LeftRightButton &p_button_to_control) :
	    ButtonAttachment(p_audio_tree_state, p_parameter_ID, (Button &)p_button_to_control) {
	}
};
