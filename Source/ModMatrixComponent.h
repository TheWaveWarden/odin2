/*
  ==============================================================================

    ModMatrixComponent.h
    Created: 27 Feb 2019 1:27:55am
    Author:  frot

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlasDropdown.h"
#include "ModAmountComponent.h"
#include "GlobalIncludes.h"

#define N_ROWS 9

//==============================================================================
/*
*/
class ModMatrixComponent    : public Component
{
public:
    ModMatrixComponent();
    ~ModMatrixComponent();

    void paint (Graphics&) override;
    void resized() override;



private:
    void clearRow(int p_row);


    GlasDropdown m_source[N_ROWS];
    ModAmountComponent m_amount_1[N_ROWS];
    GlasDropdown m_dest_1[N_ROWS];
    ModAmountComponent m_amount_2[N_ROWS];
    GlasDropdown m_dest_2[N_ROWS];
    ModAmountComponent m_amount_3[N_ROWS];
    GlasDropdown m_scale[N_ROWS];

    Identifier m_source_identifier[N_ROWS];
    Identifier m_amount_1_identifier[N_ROWS];
    Identifier m_dest_1_identifier[N_ROWS];
    Identifier m_amount_2_identifier[N_ROWS];
    Identifier m_dest_2_identifier[N_ROWS];
    Identifier m_amount_3_identifier[N_ROWS];
    Identifier m_scale_identifier[N_ROWS];

    //can't be array due to missing standard constructor..
    juce::DrawableButton m_clear_button0;
    juce::DrawableButton m_clear_button1;
    juce::DrawableButton m_clear_button2;
    juce::DrawableButton m_clear_button3;
    juce::DrawableButton m_clear_button4;
    juce::DrawableButton m_clear_button5;
    juce::DrawableButton m_clear_button6;
    juce::DrawableButton m_clear_button7;
    juce::DrawableButton m_clear_button8;

    int m_panel_height;
    int m_small_panel_width;
    int m_big_panel_width;

    juce::Colour m_color;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModMatrixComponent)
};
