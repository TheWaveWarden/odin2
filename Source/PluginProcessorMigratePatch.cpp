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

//this file is included from PluginProcessor.cpp to split the class implementation

#include "PluginProcessor.h"

void OdinAudioProcessor::migratePatch(ValueTree &p_patch) {

	//this is where the dirty patch hacking takes place!!

	for (int i = 0; i < p_patch.getNumChildren(); ++i) {
		// all children which are an audio param have two properties (name and value)
		if (p_patch.getChild(i).getNumProperties() == 2) {

			//HACK #1:
			// carrier ratio, modulator ratio, and pitchbend amount where audio-params for some reason
			// now we read them as audio params and set them back as non-audio-params where they belong

			if (p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(0)).toString() ==
			    "osc1_carrier_ratio") {
				p_patch.getChildWithName("osc").setProperty(
				    "osc1_carrier_ratio",
				    p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(1)),
				    nullptr);
			} else if (p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(0)).toString() ==
			    "osc2_carrier_ratio") {
				p_patch.getChildWithName("osc").setProperty(
				    "osc2_carrier_ratio",
				    p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(1)),
				    nullptr);
			} else if (p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(0)).toString() ==
			    "osc3_carrier_ratio") {
				p_patch.getChildWithName("osc").setProperty(
				    "osc3_carrier_ratio",
				    p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(1)),
				    nullptr);
			} else if (p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(0)).toString() ==
			    "osc1_modulator_ratio") {
				p_patch.getChildWithName("osc").setProperty(
				    "osc1_modulator_ratio",
				    p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(1)),
				    nullptr);
			} else if (p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(0)).toString() ==
			    "osc2_modulator_ratio") {
				p_patch.getChildWithName("osc").setProperty(
				    "osc2_modulator_ratio",
				    p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(1)),
				    nullptr);
			} else if (p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(0)).toString() ==
			    "osc3_modulator_ratio") {
				p_patch.getChildWithName("osc").setProperty(
				    "osc3_modulator_ratio",
				    p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(1)),
				    nullptr);
			} else if (p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(0)).toString() ==
			    "pitchbend_amount") {
				p_patch.getChildWithName("misc").setProperty(
				    "pitchbend_amount",
				    p_patch.getChild(i).getProperty(p_patch.getChild(i).getPropertyName(1)),
				    nullptr);
			}
		}
	}
}