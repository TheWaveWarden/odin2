#pragma once

#include "../GlobalIncludes.h"
#include "../JuceLibraryCode/JuceHeader.h"

class OdinMenuFeels : public LookAndFeel_V4 {
public:
	void drawPopupMenuBackground(Graphics &g, int width, int height) override {
		g.fillAll(MENU_BACKGROUND_COLOR);
		g.setColour(MENU_FONT_COLOR);
		g.drawRect(0, 0, width, height);
	}

	void setMenuWidth(int p_width) {
		m_width = p_width;
	}

	void getIdealPopupMenuItemSize(
	    const String &text, bool isSeparator, int standardMenuItemHeight, int &idealWidth, int &idealHeight) override {
		idealWidth = m_width;
		if (m_GUI_big) {
			if (isSeparator) {
				idealHeight = 10;
			} else {
				idealHeight = 30;
			}
		} else {
			if (isSeparator) {
				idealHeight = 10;
			} else {
				idealHeight = 25;
			}
		}
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

		Font font(m_font_size);

		if (!isHighlighted) {
			drawPopupMenuItemOdin(g,
			                      area,
			                      isSeparator,
			                      isActive,
			                      isHighlighted,
			                      isTicked,
			                      hasSubMenu,
			                      text,
			                      shortcutKeyText,
			                      icon,
			                      &m_text_color,
			                      font);
		} else {
			if (!isSeparator) {
				g.setColour(MENU_HIGHLIGHT_BACKGROUND_COLOR);
				g.fillRect(area);
				g.setColour(MENU_FONT_COLOR);
				g.drawRect(area);
			}
			drawPopupMenuItemOdin(g,
			                      area,
			                      isSeparator,
			                      isActive,
			                      false,
			                      isTicked,
			                      hasSubMenu,
			                      text,
			                      shortcutKeyText,
			                      icon,
			                      &m_highlight_text_color,
			                      font);
		}
	}

	//this function is copied from JUCE and modified
	void drawPopupMenuItemOdin(Graphics &g,
	                           const Rectangle<int> &area,
	                           const bool isSeparator,
	                           const bool isActive,
	                           const bool isHighlighted,
	                           const bool isTicked,
	                           const bool hasSubMenu,
	                           const String &text,
	                           const String &shortcutKeyText,
	                           const Drawable *icon,
	                           const Colour *const textColourToUse,
	                           Font p_font) {
		if (isSeparator) {
			auto r = area.reduced(5, 0);
			r.removeFromTop(roundToInt((r.getHeight() * 0.5f) - 0.5f));

			g.setColour(findColour(PopupMenu::textColourId).withAlpha(0.3f));
			g.fillRect(r.removeFromTop(1));
		} else {
			auto textColour = (textColourToUse == nullptr ? findColour(PopupMenu::textColourId) : *textColourToUse);

			auto r = area.reduced(1);

			if (isHighlighted && isActive) {
				g.setColour(findColour(PopupMenu::highlightedBackgroundColourId));
				g.fillRect(r);

				g.setColour(findColour(PopupMenu::highlightedTextColourId));
			} else {
				g.setColour(textColour.withMultipliedAlpha(isActive ? 1.0f : 0.5f));
			}

			r.reduce(jmin(5, area.getWidth() / 20), 0);

			auto font = getPopupMenuFont();

			auto maxFontHeight = r.getHeight() / 1.3f;

			if (font.getHeight() > maxFontHeight)
				font.setHeight(maxFontHeight);

			g.setFont(p_font);

			auto iconArea = r.removeFromLeft(roundToInt(maxFontHeight)).toFloat();

			if (icon != nullptr) {
				icon->drawWithin(g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
				r.removeFromLeft(roundToInt(maxFontHeight * 0.5f));
			} else if (isTicked) {
				auto tick = getTickShape(1.0f);
				g.fillPath(tick,
				           tick.getTransformToScaleToFit(iconArea.reduced(iconArea.getWidth() / 5, 0).toFloat(), true));
			}

			if (hasSubMenu) {
				auto arrowH = 0.6f * getPopupMenuFont().getAscent();

				auto x     = static_cast<float>(r.removeFromRight((int)arrowH).getX());
				auto halfH = static_cast<float>(r.getCentreY());

				Path path;
				path.startNewSubPath(x, halfH - arrowH * 0.5f);
				path.lineTo(x + arrowH * 0.6f, halfH);
				path.lineTo(x, halfH + arrowH * 0.5f);

				g.strokePath(path, PathStrokeType(2.0f));
			}

			r.removeFromRight(3);
			g.drawFittedText(text, r, Justification::centredLeft, 1);

			if (shortcutKeyText.isNotEmpty()) {
				auto f2 = font;
				f2.setHeight(f2.getHeight() * 0.75f);
				f2.setHorizontalScale(0.95f);
				g.setFont(f2);

				g.drawText(shortcutKeyText, r, Justification::centredRight, true);
			}
		}
	}
	Colour m_text_color           = MENU_FONT_COLOR;
	Colour m_highlight_text_color = MENU_HIGHLIGHT_FONT_COLOR;

	void setGUIBig() {
		m_GUI_big   = true;
		m_width     = 240;
		m_font_size = 21.f;
	}
	void setGUISmall() {
		m_GUI_big   = false;
		m_width     = 170;
		m_font_size = 17.f;
	}

	void setFontSize(float p_size) {
		m_font_size = p_size;
	}

	void setWidth(float p_width) {
		m_width = p_width;
	}

	float m_font_size = 17.f;
	float m_width     = 150;
	bool m_GUI_big    = true;
};