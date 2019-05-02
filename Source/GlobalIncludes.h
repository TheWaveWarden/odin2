/*
  ==============================================================================

    GlobalIncludes.h
    Created: 7 Mar 2019 11:20:12pm
    Author:  frot

  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"



#ifdef ODIN_LINUX
  #define GRAPHICS_PATH std::string("/home/frot/odinvst/Gimp/")
  //#define GRAPHICS_PATH std::string("/home/frederik_siepe/odinvst/Gimp/")
#endif
#ifdef ODIN_WIN
  #define GRAPHICS_PATH std::string("E:/odin-gui/Gimp/")
  #define M_PI 3.14159265359
#endif

#define MENU_BACKGROUND_COLOR Colour(35, 35, 35)
#define MENU_FONT_COLOR Colours::lightgrey
#define MENU_HIGHLIGHT_FONT_COLOR Colour(62, 103, 117)
#define MENU_HIGHLIGHT_BACKGROUND_COLOR Colour(50,50,50)
//#define STANDARD_DISPLAY_COLOR Colour(10,40,50)
#define STANDARD_DISPLAY_COLOR Colour(25, 44, 53)
#define PHASER_DISPLAY_COLOR Colour(20, 60, 30)
#define FLANGER_DISPLAY_COLOR Colour(40, 20, 53)
#define CHORUS_DISPLAY_COLOR Colour(60, 20, 20)
#define VOICES 12


#define OSC_TYPE_ANALOG 2
#define OSC_TYPE_WAVETABLE 3
#define OSC_TYPE_MULTI 4
#define OSC_TYPE_VECTOR 5
#define OSC_TYPE_CHIPTUNE 6
#define OSC_TYPE_FM 7
#define OSC_TYPE_NOISE 8
#define OSC_TYPE_WAVEDRAW 9
#define OSC_TYPE_CHIPDRAW 10
#define OSC_TYPE_SPECDRAW 11

#define FILTER_TYPE_NONE 1
#define FILTER_TYPE_LP24 2
#define FILTER_TYPE_LP12 3
#define FILTER_TYPE_BP24 4
#define FILTER_TYPE_BP12 5
#define FILTER_TYPE_HP24 6
#define FILTER_TYPE_HP12 7
//#define FILTER_TYPE_SEM24 8
#define FILTER_TYPE_SEM12 8
#define FILTER_TYPE_DIODE 9
#define FILTER_TYPE_KORG_LP 10
#define FILTER_TYPE_KORG_HP 11
#define FILTER_TYPE_COMB 12
#define FILTER_TYPE_FORMANT 13

#define GAIN_SMOOTHIN_FACTOR 0.995
#define PITCHBEND_SMOOTHIN_FACTOR 0.998
#define FILTER_FREQ_SMOOTHING_FACTOR 0.998
#define PAD_SMOOTHIN_FACTOR 0.998

#define WAVEDRAW_STEPS_X 256
#define SPECDRAW_STEPS_X 193 //(DISPLAY_WIDTH_SPEC - DRAW_INLAY_LEFT_SPEC - DRAW_INLAY_RIGHT_SPEC)
#define CHIPDRAW_STEPS_X 16

#define MODMATRIX_ROWS 9


//#define WAVEDRAW_LENGTH 2 //todo
//#define CHIPDRAW_LENGTH 2 //todo
//#define SPECDRAW_LENGTH 2 //todo

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

  void drawPopupMenuItem(Graphics &g, const Rectangle<int> &area,
                         bool isSeparator, bool isActive, bool isHighlighted,
                         bool isTicked, bool hasSubMenu, const String &text,
                         const String &shortcutKeyText, const Drawable *icon,
                         const Colour *textColour) override {
    if (!isHighlighted) {
      LookAndFeel_V4::drawPopupMenuItem(
          g, area, isSeparator, isActive, isHighlighted, isTicked, hasSubMenu,
          text, shortcutKeyText, icon, &m_text_color);
    } else {
      if (!isSeparator) {
        g.setColour(MENU_HIGHLIGHT_BACKGROUND_COLOR);
        g.fillRect(area);
        g.setColour(MENU_FONT_COLOR);
        g.drawRect(area);        
      }
      LookAndFeel_V4::drawPopupMenuItem(
          g, area, isSeparator, isActive, false, isTicked, hasSubMenu, text,
          shortcutKeyText, icon, &m_highlight_text_color);
    }
  }
  Colour m_text_color = MENU_FONT_COLOR;
  Colour m_highlight_text_color = MENU_HIGHLIGHT_FONT_COLOR;
};


