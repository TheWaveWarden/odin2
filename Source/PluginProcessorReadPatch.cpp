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

//this file is included from PluginProcessor.cpp to split the class implementation

#include "PluginProcessor.h"

// read patch by iterating over all attritubes,
// setting them if they are available and setting to default if not
void OdinAudioProcessor::readPatch(const ValueTree &newState) {
	//DBG(newStateMigrated.toXmlString());

	//create deep copy for modification
	auto newStateMigrated = newState.createCopy();

	migratePatch(newStateMigrated);


//avoid compiler warning unused variable
#if (JUCE_DEBUG && ! JUCE_DISABLE_ASSERTIONS) || DOXYGEN
	int patch_version           = newStateMigrated.getChildWithName("misc")["version_patch"];
	int minor_version           = newStateMigrated.getChildWithName("misc")["version_minor"];
	int patch_migration_version = newStateMigrated.getChildWithName("misc")["patch_migration_version"];
#endif

	DBG("Read patch from version 2." + std::to_string(minor_version) + "." + std::to_string(patch_version) +
	    ", current version is: 2." + std::to_string(ODIN_MINOR_VERSION) + "." + std::to_string(ODIN_PATCH_VERSION));
	DBG("Read patch migration version " + std::to_string(patch_migration_version) + ", current version is " +
	    std::to_string(ODIN_PATCH_MIGRATION_VERSION));

	const ValueTree &draw_tree = newStateMigrated.getChildWithName("draw");

	//if new value has no draw tree, create it from scratch
	for (int osc = 1; osc < 4; ++osc) {
		if (!(draw_tree.hasProperty(String("osc" + std::to_string(osc) + "_wavedraw_values_0")))) {
			DBG("Tree has no wavedraw" + std::to_string(osc) +" values, fallback to generation");
			writeDefaultWavedrawValuesToTree(osc);
		}

		if (!(draw_tree.hasProperty(String("osc" + std::to_string(osc) + "_chipdraw_values_0")))) {
			DBG("Tree has no chipdraw" + std::to_string(osc) +" values, fallback to generation");
			writeDefaultChipdrawValuesToTree(osc);
		}

		if (!(draw_tree.hasProperty(String("osc" + std::to_string(osc) + "_specdraw_values_0")))) {
			DBG("Tree has no specdraw" + std::to_string(osc) +" values, fallback to generation");
			writeDefaultSpecdrawValuesToTree(osc);
		}
	}

	for (int i = 0; i < m_value_tree_draw.getNumProperties(); ++i) {
		if (draw_tree.hasProperty(m_value_tree_draw.getPropertyName(i))) {
			m_value_tree_draw.setProperty(m_value_tree_draw.getPropertyName(i),
			                              draw_tree.getProperty(m_value_tree_draw.getPropertyName(i)),
			                              nullptr);
			m_value_tree_draw.sendPropertyChangeMessage((m_value_tree_draw.getPropertyName(i)));
		} else {
			//DBG("Didn't find non-audio property (draw) " +
			//    m_value_tree_draw.getPropertyName(i).toString().toStdString());
		}
	}
	const ValueTree &osc_tree = newStateMigrated.getChildWithName("osc");
	for (int i = 0; i < m_value_tree_osc.getNumProperties(); ++i) {
		if (osc_tree.hasProperty(m_value_tree_osc.getPropertyName(i))) {
			m_value_tree_osc.setProperty(m_value_tree_osc.getPropertyName(i),
			                             osc_tree.getProperty(m_value_tree_osc.getPropertyName(i)),
			                             nullptr);
			m_value_tree_osc.sendPropertyChangeMessage((m_value_tree_osc.getPropertyName(i)));
		} else {
			DBG("Didn't find non-audio property (osc) " + m_value_tree_osc.getPropertyName(i).toString().toStdString());
		}
	}
	const ValueTree &fx_tree = newStateMigrated.getChildWithName("fx");
	for (int i = 0; i < m_value_tree_fx.getNumProperties(); ++i) {
		if (fx_tree.hasProperty(m_value_tree_fx.getPropertyName(i))) {
			m_value_tree_fx.setProperty(
			    m_value_tree_fx.getPropertyName(i), fx_tree.getProperty(m_value_tree_fx.getPropertyName(i)), nullptr);
			m_value_tree_fx.sendPropertyChangeMessage((m_value_tree_fx.getPropertyName(i)));
		} else {
			DBG("Didn't find non-audio property (fx) " + m_value_tree_fx.getPropertyName(i).toString().toStdString());
		}
	}
	const ValueTree &lfo_tree = newStateMigrated.getChildWithName("lfo");
	for (int i = 0; i < m_value_tree_lfo.getNumProperties(); ++i) {
		if (lfo_tree.hasProperty(m_value_tree_lfo.getPropertyName(i))) {
			m_value_tree_lfo.setProperty(m_value_tree_lfo.getPropertyName(i),
			                             lfo_tree.getProperty(m_value_tree_lfo.getPropertyName(i)),
			                             nullptr);
			m_value_tree_lfo.sendPropertyChangeMessage((m_value_tree_lfo.getPropertyName(i)));
		} else {
			DBG("Didn't find non-audio property (lfo) " + m_value_tree_lfo.getPropertyName(i).toString().toStdString());
		}
	}
	const ValueTree &misc_tree = newStateMigrated.getChildWithName("misc");
	for (int i = 0; i < m_value_tree_misc.getNumProperties(); ++i) {
		if (misc_tree.hasProperty(m_value_tree_misc.getPropertyName(i))) {
			m_value_tree_misc.setProperty(m_value_tree_misc.getPropertyName(i),
			                              misc_tree.getProperty(m_value_tree_misc.getPropertyName(i)),
			                              nullptr);
			m_value_tree_misc.sendPropertyChangeMessage((m_value_tree_misc.getPropertyName(i)));
		} else {
			DBG("Didn't find non-audio property (misc)" +
			    m_value_tree_misc.getPropertyName(i).toString().toStdString());
		}
	}
	const ValueTree &mod_tree = newStateMigrated.getChildWithName("mod");
	for (int i = 0; i < m_value_tree_mod.getNumProperties(); ++i) {
		if (mod_tree.hasProperty(m_value_tree_mod.getPropertyName(i))) {
			m_value_tree_mod.setProperty(m_value_tree_mod.getPropertyName(i),
			                             mod_tree.getProperty(m_value_tree_mod.getPropertyName(i)),
			                             nullptr);
			m_value_tree_mod.sendPropertyChangeMessage((m_value_tree_mod.getPropertyName(i)));
		} else {
			DBG("Didn't find non-audio property (mod) " + m_value_tree_mod.getPropertyName(i).toString().toStdString());
		}
	}

	for (int i = 0; i < newStateMigrated.getNumChildren(); ++i) {
		// all children which are an audio param have two properties (name and value)
		if (newStateMigrated.getChild(i).getNumProperties() == 2) {

			//DBG(newStateMigrated.getChild(i).getProperty(newStateMigrated.getChild(i).getPropertyName(0)).toString());
			//DBG(newStateMigrated.getChild(i).getProperty(newStateMigrated.getChild(i).getPropertyName(1)).toString());

			String name =
			    newStateMigrated.getChild(i).getProperty(newStateMigrated.getChild(i).getPropertyName(0)).toString();

			if (checkLoadParameter(name)) {
				SETAUDIOFULLRANGESAFE(
				    name, newStateMigrated.getChild(i).getProperty(newStateMigrated.getChild(i).getPropertyName(1)));
			}
			//DBG("Value on tree is now: is now:" + m_value_tree.getParameterAsValue(name).getValue().toString());

			//DBG("");
		}
	}

	setMonoPolyLegato(VALUETREETOPLAYMODE((int)m_value_tree.state.getChildWithName("misc")["legato"]));
}

bool OdinAudioProcessor::checkLoadParameter(const String &p_name) {

	//return false for the params which are not to be loaded.
	//sadly not very efficient approach...
	if (p_name == "modwheel") {
		return false;
	} else if (p_name == "pitchbend") {
		return false;
	}
	return true;
}

void OdinAudioProcessor::createDrawTablesFromValueTree() {

	auto node = m_value_tree.state.getChildWithName("draw");

	for (int osc = 1; osc < 4; ++osc) {
		// wavedraw
		float wavedraw_values[WAVEDRAW_STEPS_X];
		for (int i = 0; i < WAVEDRAW_STEPS_X; ++i) {
			wavedraw_values[i] =
			    (float)node[String("osc" + std::to_string(osc) + "_wavedraw_values_" + std::to_string(i))];
		}
		m_WT_container.createWavedrawTable(osc - 1, wavedraw_values, 44100);

		// chipdraw
		for (int i = 0; i < CHIPDRAW_STEPS_X; ++i) {
			wavedraw_values[i] =
			    (float)node[String("osc" + std::to_string(osc) + "_chipdraw_values_" + std::to_string(i))];
		}
		m_WT_container.createChipdrawTable(osc - 1, wavedraw_values, 44100);

		// specdraw
		for (int i = 0; i < SPECDRAW_STEPS_X; ++i) {
			wavedraw_values[i] =
			    (float)node[String("osc" + std::to_string(osc) + "_specdraw_values_" + std::to_string(i))];
		}
		m_WT_container.createSpecdrawTable(osc - 1, wavedraw_values, 44100);
	}
}