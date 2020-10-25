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

class OdinMidiLearnBase {
public:
	virtual ~OdinMidiLearnBase() {
	}

	void setParameterID(const String &p_ID) {
		m_parameter_ID = p_ID;
		//DBG("Set parameter ID in Knob: " + p_ID);
	}

	virtual void setMidiControlActive() = 0;
	virtual void stopMidiLearn()        = 0;

	void disableMidiLearn() {
		m_midi_learn_possible = false;
	}

protected:
	bool m_midi_learn_possible = true;
	bool m_midi_learn          = false;
	bool m_midi_control        = false;
	String m_parameter_ID;
};
