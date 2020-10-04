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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../GlobalIncludes.h"
#include "Knob.h"
#include "DrawableSlider.h"
#include "OdinButton.h"
#include "LeftRightButton.h"

class OdinKnobAttachment : public SliderAttachment {
public:
	OdinKnobAttachment(AudioProcessorValueTreeState &p_audio_tree_state,
	                   const String &p_parameter_ID,
	                   Knob &p_slider_to_control) :
	    SliderAttachment(p_audio_tree_state, p_parameter_ID, (Slider &)p_slider_to_control) {
		p_slider_to_control.setParameterID(p_parameter_ID);
		if(p_audio_tree_state.state.getChildWithName("midi_learn").hasProperty(p_parameter_ID)){
			p_slider_to_control.setMidiControlActive();
		}
	}
};



class OdinSliderAttachment : public SliderAttachment {
public:
	OdinSliderAttachment(AudioProcessorValueTreeState &p_audio_tree_state,
	                   const String &p_parameter_ID,
	                   DrawableSlider &p_slider_to_control) :
	    SliderAttachment(p_audio_tree_state, p_parameter_ID, (Slider &)p_slider_to_control) {
		p_slider_to_control.setParameterID(p_parameter_ID);
		if(p_audio_tree_state.state.getChildWithName("midi_learn").hasProperty(p_parameter_ID)){
			p_slider_to_control.setMidiControlActive();
		}
	}
};


class OdinButtonAttachment : public ButtonAttachment {
public:
	OdinButtonAttachment(AudioProcessorValueTreeState &p_audio_tree_state,
	                   const String &p_parameter_ID,
	                   OdinButton &p_button_to_control) :
	    ButtonAttachment(p_audio_tree_state, p_parameter_ID, (Button &)p_button_to_control) {
		p_button_to_control.setParameterID(p_parameter_ID);
		if(p_audio_tree_state.state.getChildWithName("midi_learn").hasProperty(p_parameter_ID)){
			p_button_to_control.setMidiControlActive();
		}
	}

	OdinButtonAttachment(AudioProcessorValueTreeState &p_audio_tree_state,
	                   const String &p_parameter_ID,
	                   LeftRightButton &p_button_to_control) :
	    ButtonAttachment(p_audio_tree_state, p_parameter_ID, (Button &)p_button_to_control) {
		p_button_to_control.setParameterID(p_parameter_ID);
		if(p_audio_tree_state.state.getChildWithName("midi_learn").hasProperty(p_parameter_ID)){
			p_button_to_control.setMidiControlActive();
		}
	}
};
