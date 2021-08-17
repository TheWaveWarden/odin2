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
#include "../JuceLibraryCode/JuceHeader.h"

// these 3 belong to the profiling macro
#include "ctime"
#include <iomanip> // setprecision
#include <sstream> // stringstream

#include "GitCommitId.h"

#ifdef ODIN_LINUX
#endif
#ifdef ODIN_WIN
#define M_PI 3.14159265359
#pragma warning(disable : 4244) //disable conversion warnings
#pragma warning(disable : 4100) //disable unreferenced parameter warning (VS really misses the point here...)
#pragma warning(disable : 4305) //disable double to float truncation warning
#pragma warning(disable : 4267) //disable size_t to int truncation warning
#endif
#ifdef ODIN_MAC
#endif

// shows the spectrum and wave display for wavetable osc 1
//#define WTGEN

#define ODIN_MAJOR_VERSION 2
#define ODIN_MINOR_VERSION 3
#define ODIN_PATCH_VERSION 1

#define ODIN_VERSION_STRING ("Odin 2." + std::to_string(ODIN_MINOR_VERSION) + "." + std::to_string(ODIN_PATCH_VERSION))

// ! this is for migrating patches from older version, not to be confused with above
#define ODIN_PATCH_MIGRATION_VERSION 5
//1 -> 2: replaced "amp width" by "amp velocity"
//2 -> 3: made carrier and modulator ratio, as well as pitchbend amount nonaudio values (but wrote migration code)

#define DBG_VAR(var) DBG(#var << ": " << var)

#define MENU_BACKGROUND_COLOR Colour(35, 35, 35)
#define MENU_FONT_COLOR Colours::lightgrey
#define MENU_HIGHLIGHT_FONT_COLOR Colour(62, 103, 117)
#define MENU_HIGHLIGHT_BACKGROUND_COLOR Colour(50, 50, 50)
#define STANDARD_DISPLAY_COLOR Colour(10, 40, 50)
#define MODMATRIX_COLOR Colour(30, 30, 30)
#define ODIN_BLUE Colour(0xff3c9bc7)
#define DISTORTION_ON_COLOR juce::Colour(14, 35, 50)
#define PATCH_BROWSER_INPUT_FIELD_BACKGROUND_COLOR Colour(20, 30, 30)
//#define STANDARD_DISPLAY_COLOR Colour(21, 45, 56)
//#define STANDARD_DISPLAY_COLOR Colour(30, 30, 30)
#define PHASER_DISPLAY_COLOR Colour(11, 41, 19)
#define FLANGER_DISPLAY_COLOR Colour(40, 20, 53)
#define MULTI_DROPDOWN_COLOR juce::Colour(39, 58, 48)
#define WAVETABLE_DROPDOWN_COLOR juce::Colour(66, 81, 85)
#define DARKGREY juce::Colour(30, 30, 30)
#define CHORUS_DISPLAY_COLOR Colour(69, 39, 38)
#define VOICES 24

#define MATRIX_SECTION_INDEX_PRESETS 10
#define MATRIX_SECTION_INDEX_ARP 1
#define MATRIX_SECTION_INDEX_MATRIX 0

#ifdef ODIN_LINUX
#define ODIN_STORAGE_PATH                                                                                              \
	File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory).getFullPathName() +                         \
	    File::getSeparatorString() + ".local" + File::getSeparatorString() + "share" + File::getSeparatorString() +    \
	    "odin2"
#endif
#ifdef ODIN_MAC
#define ODIN_STORAGE_PATH                                                                                              \
	File::getSpecialLocation(File::SpecialLocationType::commonApplicationDataDirectory).getFullPathName() +            \
	    File::getSeparatorString() + "Audio" + File::getSeparatorString() + "Presets" + File::getSeparatorString() +   \
	    "odin2"
#endif
#ifdef ODIN_WIN
#define ODIN_STORAGE_PATH                                                                                              \
	File::getSpecialLocation(File::SpecialLocationType::commonApplicationDataDirectory).getFullPathName() +            \
	    File::getSeparatorString() + "odin2"
#endif
//#endif
#define CONFIG_FILE_PATH ODIN_STORAGE_PATH + File::getSeparatorString() + "odin2.conf"

// leave spare values for future additions :hype:
#define OSC_TYPE_ANALOG 2
#define OSC_TYPE_WAVETABLE 3
#define OSC_TYPE_MULTI 4
#define OSC_TYPE_VECTOR 5
#define OSC_TYPE_CHIPTUNE 6
#define OSC_TYPE_FM 7
#define OSC_TYPE_PM 8
#define OSC_TYPE_NOISE 15
#define OSC_TYPE_WAVEDRAW 20
#define OSC_TYPE_CHIPDRAW 21
#define OSC_TYPE_SPECDRAW 22

#define FILTER_TYPE_NONE 1
#define FILTER_TYPE_LP24 2
#define FILTER_TYPE_LP12 3
#define FILTER_TYPE_BP24 4
#define FILTER_TYPE_BP12 5
#define FILTER_TYPE_HP24 6
#define FILTER_TYPE_HP12 7
#define FILTER_TYPE_SEM12 15
#define FILTER_TYPE_DIODE 19
#define FILTER_TYPE_KORG_LP 24
#define FILTER_TYPE_KORG_HP 25
#define FILTER_TYPE_COMB 30
#define FILTER_TYPE_FORMANT 35
#define FILTER_TYPE_RINGMOD 40

#define BROWSER_INLAY_X_150 6
#define BROWSER_INLAY_Y_150 5
#define BROWSER_POS_X_150 411
#define BROWSER_POS_Y_150 701
#define BROWSER_SIZE_X_150 (738 + 2 * BROWSER_INLAY_X_150)
#define BROWSER_SIZE_Y_150 (213 + 2 * BROWSER_INLAY_Y_150)

#define BROWSER_INLAY_X 4
#define BROWSER_INLAY_Y 4
#define BROWSER_POS_X 274
#define BROWSER_POS_Y 467
#define BROWSER_SIZE_X (492 + 2 * BROWSER_INLAY_X)
#define BROWSER_SIZE_Y (141 + 2 * BROWSER_INLAY_Y)

// midpoint for filters:
// https://www.wolframalpha.com/input/?i=80*e%5E%28ln%2818000%2F80%29*0.5%29
// skew for filters:
// https://www.wolframalpha.com/input/?i=ln%280.5%29%2F%28ln%28%281200-80%29%2F%2818000-80%29%29%29
#define FILTER_SKEW_DEFAULT 0.25f

#define GAIN_SMOOTHIN_FACTOR 0.995f
#define PAN_SMOOTHIN_FACTOR 0.998f
#define PITCHBEND_SMOOTHIN_FACTOR 0.998f
#define FILTER_FREQ_SMOOTHING_FACTOR 0.998f
#define PAD_SMOOTHIN_FACTOR 0.998f
#define PWM_SMOOTHIN_FACTOR 0.998f
#define THRESHOLD_SMOOTHIN_FACTOR 0.998f

#define MINUS_12_dB_GAIN 0.251189f //needed for volume modulation threshold
#define PLUS_12_dB_GAIN 3.981072f

#define INPUT_LABEL_SIZE_X 70
#define INPUT_LABEL_SIZE_Y 20

#define BUTTON_REPEAT_INITIAL_DELAY 500
#define BUTTON_REPEAT_DELAY 200

#define WAVEDRAW_STEPS_X 193
//#define SPECDRAW_STEPS_X 193 //(DISPLAY_WIDTH_SPEC - DRAW_INLAY_LEFT_SPEC - DRAW_INLAY_RIGHT_SPEC)
#define SPECDRAW_STEPS_X 49 //just enough so it fits the display
#define CHIPDRAW_STEPS_X 32

#define MODMATRIX_ROWS 9

#define VOL_MOD_UPPER_LIMIT 100

#define GETAUDIO(name) m_value_tree.getParameterAsValue(name).getValue()

#define SETAUDIO0TO1(name, value) m_value_tree.getParameter(name)->setValueNotifyingHost((float)value)

#define SETAUDIOFULLRANGE(name, value) /*m_value_tree.getParameter(name)->beginChangeGesture();*/                      \
	m_value_tree.getParameter(name)->setValueNotifyingHost(                                                            \
	    m_value_tree.getParameter(name)->convertTo0to1((float)value));                                                 \
	//m_value_tree.getParameter(name)->endChangeGesture();

#define SETAUDIOFULLRANGESAFE(name, value)                                                                             \
	if (m_value_tree.getParameter(name)) {                                                                             \
		SETAUDIOFULLRANGE(name, value)                                                                                 \
	} else {                                                                                                           \
		DBG("Tried to access unknown audio-param:");                                                                   \
		DBG(name);                                                                                                     \
	}

#define DEBUG_COMPONENT_OUTLINE                                                                                        \
	g.setColour(Colours::grey);                                                                                        \
	g.drawRect(getLocalBounds(), 1);

#define RETRIGGERAUDIO(name) SETAUDIOFULLRANGE(name, GETAUDIO(name))
#define SETAUDIOVALUEPATCH(name)

#define INTERPOLATION_QUALITY_LOW Graphics::ResamplingQuality::lowResamplingQuality
#define INTERPOLATION_QUALITY_HIGH Graphics::ResamplingQuality::highResamplingQuality

//set interpolation value to 'none' for integer scaling, else to high
#define SET_INTERPOLATION_QUALITY(graphics)                                                                            \
	float desktop_scale = Desktop::getInstance().Desktop::getInstance().getDisplays().getPrimaryDisplay()->scale;      \
	if ((fabs(desktop_scale - 1.f) < 1e-4) || (fabs(desktop_scale - 2.f) < 1e-4)) {                                    \
		graphics.setImageResamplingQuality(INTERPOLATION_QUALITY_LOW);                                                 \
	} else {                                                                                                           \
		graphics.setImageResamplingQuality(INTERPOLATION_QUALITY_HIGH);                                                \
	}

#define CLAMP(low, clamp, high) clamp = clamp < low ? low : clamp > high ? high : clamp

#define DBGAUDIO(string)                                                                                               \
	if (sample == 0) {                                                                                                 \
		DBG(string);                                                                                                   \
	}

#define SETSKEWREPLACEMENT(component, mid_value)                                                                       \
	component.setSkewFactorFromMidPoint(mid_value);                                                                    \
	DBG("REPLACE SKEW:  NormalisableRange<float>(" + std::to_string(component.getMinimum()) + ", " +                   \
	    std::to_string(component.getMaximum()) + ", 0, " + std::to_string(component.getSkewFactor()) + ")");

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

// note: these are not the values stored in the value tree, since historically the param was a button (mono 0, poly 1) and is now a dropdown (can't be 0)
enum class PlayModes { Legato = 1, Poly = 2, Retrig = 3 };
#define PLAYMODETOVALUETREE(playmode) ((int)playmode - 1)
#define VALUETREETOPLAYMODE(mode) ((PlayModes)(mode + 1))

//Used to convert positions and sizes to 150% GUI scaling
class OdinHelper {
public:
	//convert an iteger value to 150%
	static int c150(int input) {
		float x = input * 1.5f;
		return x + 0.5 - (x < 0);
	}
};

//sets the Ctrl-key as "reset" key for a given control
#define SET_CTR_KEY(name)                                                                                              \
	name.setDoubleClickReturnValue(true, name.getDoubleClickReturnValue(), ModifierKeys::ctrlModifier)

#define TIMESTART(name)                                                                                                \
	{                                                                                                                  \
		std::string clock_name = name;                                                                                 \
		std::vector<std::clock_t> collected_times;                                                                     \
		std::vector<std::string> collected_names;                                                                      \
		std::clock_t clock_begin = std::clock();

#define TIMEBARS 40
#define BARCHARACTER "▄"

#define TIMEADD(name)                                                                                                  \
	collected_times.push_back(std::clock());                                                                           \
	collected_names.push_back(name);

#define TIMEEND                                                                                                        \
	std::clock_t clock_end = std::clock();                                                                             \
	DBG("\n===================================================================="                                       \
	    "==");                                                                                                         \
	double total_time = double(clock_end - clock_begin) / CLOCKS_PER_SEC;                                              \
	if (collected_times.size() > 0) {                                                                                  \
		int longest_name    = 4;                                                                                       \
		double longest_time = double(clock_end - collected_times[collected_times.size() - 1]) / CLOCKS_PER_SEC;        \
		for (int i = 0; i < collected_times.size(); ++i) {                                                             \
			if (collected_names[i].length() > longest_name) {                                                          \
				longest_name = collected_names[i].length();                                                            \
			}                                                                                                          \
			double time_compare;                                                                                       \
			if (i == 0) {                                                                                              \
				time_compare = double(collected_times[i] - clock_begin) / CLOCKS_PER_SEC;                              \
			} else {                                                                                                   \
				time_compare = double(collected_times[i] - collected_times[i - 1]) / CLOCKS_PER_SEC;                   \
			}                                                                                                          \
			if (time_compare > longest_time) {                                                                         \
				longest_time = time_compare;                                                                           \
			}                                                                                                          \
		}                                                                                                              \
		int longest_bar = longest_time / total_time * TIMEBARS;                                                        \
		for (int i = 0; i < collected_times.size(); ++i) {                                                             \
			std::string message = collected_names[i] + " ";                                                            \
			int spacing         = longest_name - collected_names[i].length();                                          \
			for (int i = 0; i < spacing; ++i) {                                                                        \
				message += " ";                                                                                        \
			}                                                                                                          \
			double elapsed;                                                                                            \
			if (i == 0) {                                                                                              \
				elapsed = double(collected_times[i] - clock_begin) / CLOCKS_PER_SEC;                                   \
			} else {                                                                                                   \
				elapsed = double(collected_times[i] - collected_times[i - 1]) / CLOCKS_PER_SEC;                        \
			}                                                                                                          \
			double percent     = elapsed / total_time;                                                                 \
			int number_of_bars = percent * TIMEBARS;                                                                   \
			for (int i = 0; i < number_of_bars; ++i) {                                                                 \
				message += BARCHARACTER;                                                                               \
			}                                                                                                          \
			int missing_bars = longest_bar - number_of_bars;                                                           \
			for (int i = 0; i < missing_bars; ++i) {                                                                   \
				message += " ";                                                                                        \
			}                                                                                                          \
			if (percent * 100 < 10) {                                                                                  \
				message += " ";                                                                                        \
			}                                                                                                          \
			std::stringstream stream;                                                                                  \
			stream << std::fixed << std::setprecision(2) << percent * 100;                                             \
			std::string percent_decimal = stream.str();                                                                \
			std::stringstream stream_elapsed;                                                                          \
			stream_elapsed << std::fixed << std::setprecision(4) << elapsed;                                           \
			std::string elapsed_decimal = stream_elapsed.str();                                                        \
			message += " " + percent_decimal + "%, " + elapsed_decimal + " sek";                                       \
			DBG(message);                                                                                              \
		}                                                                                                              \
		std::string message = "rest ";                                                                                 \
		int spacing         = longest_name - 4;                                                                        \
		for (int i = 0; i < spacing; ++i) {                                                                            \
			message += " ";                                                                                            \
		}                                                                                                              \
		double elapsed     = double(clock_end - collected_times[collected_times.size() - 1]) / CLOCKS_PER_SEC;         \
		double percent     = elapsed / total_time;                                                                     \
		int number_of_bars = percent * TIMEBARS;                                                                       \
		for (int i = 0; i < number_of_bars; ++i) {                                                                     \
			message += BARCHARACTER;                                                                                   \
		}                                                                                                              \
		int missing_bars = longest_bar - number_of_bars;                                                               \
		for (int i = 0; i < missing_bars; ++i) {                                                                       \
			message += " ";                                                                                            \
		}                                                                                                              \
		if (percent * 100 < 10) {                                                                                      \
			message += " ";                                                                                            \
		}                                                                                                              \
		std::stringstream stream;                                                                                      \
		stream << std::fixed << std::setprecision(2) << percent * 100;                                                 \
		std::string percent_decimal = stream.str();                                                                    \
		std::stringstream stream_elapsed;                                                                              \
		stream_elapsed << std::fixed << std::setprecision(4) << elapsed;                                               \
		std::string elapsed_decimal = stream_elapsed.str();                                                            \
		message += " " + percent_decimal + "%, " + elapsed_decimal + " sek";                                           \
		DBG(message);                                                                                                  \
		DBG("--------------------------------------------------------------------"                                     \
		    "--");                                                                                                     \
	}                                                                                                                  \
	DBG("Overall time for " + clock_name + ": " + std::to_string(total_time) + " sek");                                \
	DBG("======================================================================"                                       \
	    "\n");                                                                                                         \
	}

class FileElementComparatorAlphabetical {
public:
	static int compareElements(const File &first, const File &second) {
		return first.getFileName().compareIgnoreCase(second.getFileName());
	}
};