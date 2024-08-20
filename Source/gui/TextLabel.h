#pragma once

class TextLabel : public juce::Component {
public:
	TextLabel(juce::String p_text) : m_text(p_text) {
	}

	void setFontHeightRelative(float p_val) {
		m_font_height_relative = p_val;
		repaint();
	}

	void paint(juce::Graphics &g) {
		g.setColour(COL_LIGHT);
		g.setFont(H * m_font_height_relative);
		g.drawText(m_text, getLocalBounds(), m_justification);
	}

	void setJustification(juce::Justification p_justification) {
		m_justification = p_justification;
	}

private:
	juce::Justification m_justification = juce::Justification::centred;
	juce::String m_text;
	float m_font_height_relative = 1.0f / 1.4f;
};