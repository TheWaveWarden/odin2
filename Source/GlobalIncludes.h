/*
  ==============================================================================

    GlobalIncludes.h
    Created: 7 Mar 2019 11:20:12pm
    Author:  frot

  ==============================================================================
*/
#pragma once
#ifndef BENCHMARK
#include "../JuceLibraryCode/JuceHeader.h"
#endif

// these 3 belong to the profiling macro
#include "ctime"
#include <iomanip> // setprecision
#include <sstream> // stringstream

#ifdef ODIN_LINUX
#define GRAPHICS_PATH std::string("/home/frot/odinvst/Gimp/")
//#define GRAPHICS_PATH std::string("/home/frederik_siepe/odinvst/Gimp/")
#endif
#ifdef ODIN_WIN
#define GRAPHICS_PATH std::string("E:/odin-gui/Gimp/")
#define M_PI 3.14159265359
#endif

// shows the spectrum and wave display for wavetable osc 1
//#define WTGEN

#define ODIN_MAJOR_VERSION 2
#define ODIN_MINOR_VERSION 0
#define ODIN_PATCH_VERSION 13

#define ODIN_VERSION_STRING ("Odin 2." + std::to_string(ODIN_MINOR_VERSION) + "." + std::to_string(ODIN_PATCH_VERSION) + " beta")

// ! this is for migrating patches from older version, not to be confused with above
#define ODIN_PATCH_MIGRATION_VERSION 1

#define MENU_BACKGROUND_COLOR Colour(35, 35, 35)
#define MENU_FONT_COLOR Colours::lightgrey
#define MENU_HIGHLIGHT_FONT_COLOR Colour(62, 103, 117)
#define MENU_HIGHLIGHT_BACKGROUND_COLOR Colour(50, 50, 50)
#define STANDARD_DISPLAY_COLOR Colour(10,40,50)
//#define STANDARD_DISPLAY_COLOR Colour(21, 45, 56)
//#define STANDARD_DISPLAY_COLOR Colour(30, 30, 30)
#define PHASER_DISPLAY_COLOR Colour(11, 41, 19)
#define FLANGER_DISPLAY_COLOR Colour(40, 20, 53)
#define MULTI_DROPDOWN_COLOR juce::Colour(39, 58, 48)
#define WAVETABLE_DROPDOWN_COLOR juce::Colour(48, 65, 60)
#define DARKGREY juce::Colour(30, 30, 30)
#define CHORUS_DISPLAY_COLOR Colour(69, 39, 38)
#define VOICES 12

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

// midpoint for filters:
// https://www.wolframalpha.com/input/?i=80*e%5E%28ln%2818000%2F80%29*0.5%29
// skew for filters:
// https://www.wolframalpha.com/input/?i=ln%280.5%29%2F%28ln%28%281200-80%29%2F%2818000-80%29%29%29
#define FILTER_SKEW_DEFAULT 0.25f

#define GAIN_SMOOTHIN_FACTOR 0.995
#define PAN_SMOOTHIN_FACTOR 0.998
#define PITCHBEND_SMOOTHIN_FACTOR 0.998
#define FILTER_FREQ_SMOOTHING_FACTOR 0.998
#define PAD_SMOOTHIN_FACTOR 0.998
#define PWM_SMOOTHIN_FACTOR 0.998
#define THRESHOLD_SMOOTHIN_FACTOR 0.998

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

#define SETAUDIO0TO1(name, value) m_value_tree.getParameter(name)->setValueNotifyingHost(((float)value))

#define SETAUDIOFULLRANGE(name, value) /*m_value_tree.getParameter(name)->beginChangeGesture();*/\
m_value_tree.getParameter(name)->setValueNotifyingHost(m_value_tree.getParameter(name)->convertTo0to1((float)value));\
//m_value_tree.getParameter(name)->endChangeGesture();

#define SETAUDIOFULLRANGESAFE(name, value) if(m_value_tree.getParameter(name)){\
SETAUDIOFULLRANGE(name, value)}\
else {\
DBG("TRIED TO ACCESS UNKOWN PARAMETER:");\
DBG(name);\
} 

#define RETRIGGERAUDIO(name) SETAUDIOFULLRANGE(name, GETAUDIO(name))
#define SETAUDIOVALUEPATCH(name)

#define INTERPOLATION_QUALITY_LOW Graphics::ResamplingQuality::lowResamplingQuality
#define INTERPOLATION_QUALITY_HIGH Graphics::ResamplingQuality::highResamplingQuality

//set interpolation value to 'none' for integer scaling, else to high
#define SET_INTERPOLATION_QUALITY(graphics) \
float desktop_scale = Desktop::getInstance().Desktop::getInstance().getDisplays().getMainDisplay().scale;\
if((fabs(desktop_scale - 1.f) < 1e-4) || (fabs(desktop_scale - 2.f) < 1e-4)){\
graphics.setImageResamplingQuality(INTERPOLATION_QUALITY_LOW);\
} else {\
graphics.setImageResamplingQuality(INTERPOLATION_QUALITY_HIGH);\
}

#define CLAMP(low, clamp, high) clamp = clamp < low ? low : clamp > high ? high : clamp

#define DBGAUDIO(string) if(sample == 0){DBG(string);}

#define SETSKEWREPLACEMENT(component, mid_value)                                                                       \
	component.setSkewFactorFromMidPoint(mid_value);                                                                    \
	DBG("REPLACE SKEW:  NormalisableRange<float>(" + std::to_string(component.getMinimum()) + ", " +                   \
	    std::to_string(component.getMaximum()) + ", 0, " + std::to_string(component.getSkewFactor()) + ")");

#ifndef BENCHMARK

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

class OdinMenuFeels : public LookAndFeel_V4 {
public:
	void drawPopupMenuBackground(Graphics &g, int width, int height) override {
		g.fillAll(MENU_BACKGROUND_COLOR);
		g.setColour(MENU_FONT_COLOR);
		g.drawRect(0, 0, width, height);
	}

	void drawPopupMenuItem(Graphics &g,
	                       const Rectangle<int> &area,
	                       bool isSeparator,
	                       bool isActive,
	                       bool isHighlighted,
	                       bool isTicked,
	                       bool hasSubMenu,
	                       const String &text,
	                       const String &shortcutKeyText,
	                       const Drawable *icon,
	                       const Colour *textColour) override {
		if (!isHighlighted) {
			LookAndFeel_V4::drawPopupMenuItem(g,
			                                  area,
			                                  isSeparator,
			                                  isActive,
			                                  isHighlighted,
			                                  isTicked,
			                                  hasSubMenu,
			                                  text,
			                                  shortcutKeyText,
			                                  icon,
			                                  &m_text_color);
		} else {
			if (!isSeparator) {
				g.setColour(MENU_HIGHLIGHT_BACKGROUND_COLOR);
				g.fillRect(area);
				g.setColour(MENU_FONT_COLOR);
				g.drawRect(area);
			}
			LookAndFeel_V4::drawPopupMenuItem(g,
			                                  area,
			                                  isSeparator,
			                                  isActive,
			                                  false,
			                                  isTicked,
			                                  hasSubMenu,
			                                  text,
			                                  shortcutKeyText,
			                                  icon,
			                                  &m_highlight_text_color);
		}
	}
	Colour m_text_color           = MENU_FONT_COLOR;
	Colour m_highlight_text_color = MENU_HIGHLIGHT_FONT_COLOR;
};

#endif


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
