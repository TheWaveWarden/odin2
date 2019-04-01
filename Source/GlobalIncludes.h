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
  #define GRAPHICS_PATH std::string("/home/frot/Odin/Gimp/")
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


typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;


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