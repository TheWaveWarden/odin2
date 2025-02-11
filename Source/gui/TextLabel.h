#pragma once

class TextLabel : public juce::Component {
public:
	TextLabel(juce::String p_text) : m_text(p_text) {
	}

	void setFontHeightRelative(float p_val) {
		m_font_height_relative = p_val;
		repaint();
	}

	void enablementChanged() override {
		repaint();
	}

	void paint(juce::Graphics &g) {
		const auto disabled_alpha = isEnabled() ? 1.0f : 0.6f;
		g.setColour(juce::Colours::white.withAlpha(0.7f * disabled_alpha));
		if (m_rotate_90_degrees)
			g.setFont(W * m_font_height_relative);
		else
			g.setFont(H * m_font_height_relative);

		if (m_rotate_90_degrees)
			g.addTransform(juce::AffineTransform::rotation(-juce::MathConstants<float>::pi / 2.0f, W / 2.0f, H / 2.0f));

		g.drawText(m_text, getLocalBounds(), m_justification, false);
	}

	void setJustification(juce::Justification p_justification) {
		m_justification = p_justification;
	}

	void setText(juce::String p_text) {
		m_text = p_text;
		repaint();
	}

	void setRotate90Degrees(bool p_draw) {
		m_rotate_90_degrees = p_draw;
	}

private:
	bool m_rotate_90_degrees = false;

	juce::Justification m_justification = juce::Justification::centred;
	juce::String m_text;
	float m_font_height_relative = 1.0f / 1.4f;
};