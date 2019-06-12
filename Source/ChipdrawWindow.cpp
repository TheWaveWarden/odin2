/*
  ==============================================================================

    ChipdrawWindow.cpp
    Created: 1 Mar 2019 11:07:05pm
    Author:  frot

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChipdrawWindow.h"
#include <cmath>
//==============================================================================
ChipdrawWindow::ChipdrawWindow()
{
    m_glaspanel = ImageCache::getFromMemory(BinaryData::drawpanel_png, BinaryData::drawpanel_pngSize);

    setSize(m_glaspanel.getWidth(), m_glaspanel.getHeight());

  setMouseCursor(MouseCursor::StandardCursorType::CrosshairCursor);
    

}

ChipdrawWindow::~ChipdrawWindow()
{
}

void ChipdrawWindow::paint(Graphics &g)
{
    g.setColour(m_color);
    juce::Point<int> top_left = getLocalBounds().getTopLeft();
    top_left.addXY(m_inlay + 1, m_inlay);
    juce::Point<int> bottom_right = getLocalBounds().getBottomRight();
    bottom_right.addXY(-m_inlay - 1, -m_inlay);
    g.fillRect(
        juce::Rectangle<int>(top_left, bottom_right)); //

    g.setColour(m_draw_color);

    float width = (float)(getWidth() - DRAW_INLAY_LEFT - DRAW_INLAY_RIGHT) / (float)CHIPDRAW_STEPS_X;
    float height = (float)(getHeight() - DRAW_INLAY_UP - DRAW_INLAY_DOWN) / 2.f;
    float mid = (float)getHeight() / 2.f;

    for (int i = 0; i < CHIPDRAW_STEPS_X; ++i)
    {
        g.drawLine(DRAW_INLAY_LEFT + i * width, mid - m_draw_values[i] * height, DRAW_INLAY_LEFT + (i + 1) * width, mid - m_draw_values[i] * height, CHIPDRAW_THICCNESS);
        if (i != CHIPDRAW_STEPS_X - 1)
        {
            g.drawLine(DRAW_INLAY_LEFT + (i + 1) * width, mid - m_draw_values[i] * height, DRAW_INLAY_LEFT + (i + 1) * width, mid - m_draw_values[i + 1] * height, CHIPDRAW_THICCNESS);
        }
    }

    g.drawImageAt(m_glaspanel, 0, 0);
}

void ChipdrawWindow::resized()
{
}

void ChipdrawWindow::mouseDrag(const MouseEvent &event) { mouseInteraction(); }

void ChipdrawWindow::mouseDown(const MouseEvent &event) { mouseInteraction(); }

void ChipdrawWindow::mouseInteraction()
{
    juce::Point<int> mouse_pos = getMouseXYRelative();
    float x = mouse_pos.getX();
    float y = mouse_pos.getY();

    if (x > DRAW_INLAY_LEFT && x < getWidth() - DRAW_INLAY_RIGHT )
    {
        y = y < DRAW_INLAY_UP ? DRAW_INLAY_UP : y;
        y = y > getHeight() - DRAW_INLAY_DOWN ? getHeight() - DRAW_INLAY_DOWN : y;

        float float_x = (x - DRAW_INLAY_LEFT) / (getWidth() - DRAW_INLAY_LEFT - DRAW_INLAY_RIGHT);
        int step_x = (int)(floor(float_x * CHIPDRAW_STEPS_X));

        float float_y = (y - DRAW_INLAY_UP) / (getHeight() - DRAW_INLAY_UP - DRAW_INLAY_DOWN);
        float_y = 2 * (0.5 - float_y);
        float_y = (round(float_y * CHIPDRAW_STEPS_Y))/CHIPDRAW_STEPS_Y;

        m_draw_values[step_x] = float_y;
    }
    onDraw();
    repaint();
}


float* ChipdrawWindow::getDrawnTable(){
  return m_draw_values;
}
