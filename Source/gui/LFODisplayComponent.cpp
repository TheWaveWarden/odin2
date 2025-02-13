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

#include "LFODisplayComponent.h"

LFODisplayComponent::LFODisplayComponent() {
	setMouseCursor(juce::MouseCursor::StandardCursorType::UpDownResizeCursor);
}

LFODisplayComponent::~LFODisplayComponent() {
}

void LFODisplayComponent::paint(Graphics &g) {
	if (m_highlight) {
		if (m_value < m_waves_highlight.size())
			g.drawImageAt((m_waves_highlight[m_value]), 0, 0);
	} else {
		if (m_value < m_waves.size())
			g.drawImageAt((m_waves[m_value]), 0, 0);
	}
}

void LFODisplayComponent::mouseDown(const MouseEvent &event) {
	toParentMouseDown(event);
	Component::mouseDown(event);
}

void LFODisplayComponent::mouseDrag(const MouseEvent &event) {
	toParentMouseDrag(event);
	Component::mouseDrag(event);
}

void LFODisplayComponent::mouseUp(const MouseEvent &event) {
	toParentMouseUp(event);
	Component::mouseUp(event);

	if (contains(event.getPosition()))
		onClick();
}

void LFODisplayComponent::mouseEnter(const MouseEvent &event) {
	m_highlight = true;
	repaint();
}

void LFODisplayComponent::mouseExit(const MouseEvent &event) {
	m_highlight = false;
	repaint();
}

void LFODisplayComponent::resized() {
	clearWaves();

	auto highlight_colour = (COL_TEXT_BLUE).interpolatedWith(juce::Colours::white, 0.5f);

	// sine
	juce::Image sine_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_sine(sine_image);
	drawSineImage(graphics_sine, COL_TEXT_BLUE);
	m_waves.push_back(sine_image.createCopy());
	sine_image.clear(sine_image.getBounds());
	drawSineImage(graphics_sine, highlight_colour);
	m_waves_highlight.push_back(sine_image.createCopy());

	// saw
	juce::Image saw_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_saw(saw_image);
	drawSawImage(graphics_saw, COL_TEXT_BLUE);
	m_waves.push_back((saw_image.createCopy()));
	saw_image.clear(saw_image.getBounds());
	drawSawImage(graphics_saw, highlight_colour);
	m_waves_highlight.push_back(saw_image.createCopy());

	// tri
	juce::Image triangle_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_triangle(triangle_image);
	drawTriangleImage(graphics_triangle, COL_TEXT_BLUE);
	m_waves.push_back((triangle_image.createCopy()));
	triangle_image.clear(triangle_image.getBounds());
	drawTriangleImage(graphics_triangle, highlight_colour);
	m_waves_highlight.push_back(triangle_image.createCopy());

	// square50
	juce::Image square_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_square(square_image);
	drawSquareImage(graphics_square, COL_TEXT_BLUE, 0.5f);
	m_waves.push_back((square_image.createCopy()));
	square_image.clear(square_image.getBounds());
	drawSquareImage(graphics_square, highlight_colour, 0.5f);
	m_waves_highlight.push_back(square_image.createCopy());

	// square25
	juce::Image square25_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_square25(square25_image);
	drawSquareImage(graphics_square25, COL_TEXT_BLUE, 0.25f);
	m_waves.push_back((square25_image.createCopy()));
	square_image.clear(square_image.getBounds());
	drawSquareImage(graphics_square, highlight_colour, 0.25f);
	m_waves_highlight.push_back(square_image.createCopy());

	// square12
	juce::Image square12_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_square12(square12_image);
	drawSquareImage(graphics_square12, COL_TEXT_BLUE, 0.125f);
	m_waves.push_back((square12_image.createCopy()));
	square_image.clear(square_image.getBounds());
	drawSquareImage(graphics_square, highlight_colour, 0.125f);
	m_waves_highlight.push_back(square_image.createCopy());

	// pointy roof
	juce::Image pointy_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_pointy(pointy_image);
	drawPointyImage(graphics_pointy, COL_TEXT_BLUE);
	m_waves.push_back((pointy_image.createCopy()));
	pointy_image.clear(pointy_image.getBounds());
	drawPointyImage(graphics_pointy, highlight_colour);
	m_waves_highlight.push_back(pointy_image.createCopy());

	// S&H
	juce::Image SH_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_SH(SH_image);
	drawSHImage(graphics_SH, COL_TEXT_BLUE);
	m_waves.push_back((SH_image.createCopy()));
	SH_image.clear(SH_image.getBounds());
	drawSHImage(graphics_SH, highlight_colour);
	m_waves_highlight.push_back(SH_image.createCopy());

	// chippyramid 4
	juce::Image pyramid_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_pyramid(pyramid_image);
	drawPyramidImage(graphics_pyramid, COL_TEXT_BLUE, 4);
	m_waves.push_back((pyramid_image.createCopy()));
	pyramid_image.clear(pyramid_image.getBounds());
	drawPyramidImage(graphics_pyramid, highlight_colour, 4);
	m_waves_highlight.push_back(pyramid_image.createCopy());

	// chippyramid 6
	juce::Image pyramid6_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_pyramid6(pyramid6_image);
	drawPyramidImage(graphics_pyramid6, COL_TEXT_BLUE, 6);
	m_waves.push_back((pyramid6_image.createCopy()));
	pyramid6_image.clear(pyramid6_image.getBounds());
	drawPyramidImage(graphics_pyramid6, highlight_colour, 6);
	m_waves_highlight.push_back(pyramid6_image.createCopy());

	// chippyramid 8
	juce::Image pyramid8_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_pyramid8(pyramid8_image);
	drawPyramidImage(graphics_pyramid8, COL_TEXT_BLUE, 8);
	m_waves.push_back((pyramid8_image.createCopy()));
	pyramid8_image.clear(pyramid8_image.getBounds());
	drawPyramidImage(graphics_pyramid8, highlight_colour, 8);
	m_waves_highlight.push_back(pyramid8_image.createCopy());

	// chippyramid 12
	juce::Image pyramid12_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_pyramid12(pyramid12_image);
	drawPyramidImage(graphics_pyramid12, COL_TEXT_BLUE, 12);
	m_waves.push_back((pyramid12_image.createCopy()));
	pyramid12_image.clear(pyramid12_image.getBounds());
	drawPyramidImage(graphics_pyramid12, highlight_colour, 12);
	m_waves_highlight.push_back(pyramid12_image.createCopy());

	// step 3
	juce::Image step3_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_step3(step3_image);
	drawStepImage(graphics_step3, COL_TEXT_BLUE, 3);
	m_waves.push_back((step3_image.createCopy()));
	step3_image.clear(step3_image.getBounds());
	drawStepImage(graphics_step3, highlight_colour, 3);
	m_waves_highlight.push_back(step3_image.createCopy());

	// step 4
	juce::Image step4_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_step4(step4_image);
	drawStepImage(graphics_step4, COL_TEXT_BLUE, 4);
	m_waves.push_back((step4_image.createCopy()));
	step4_image.clear(step4_image.getBounds());
	drawStepImage(graphics_step4, highlight_colour, 4);
	m_waves_highlight.push_back(step4_image.createCopy());

	// step 6
	juce::Image step6_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_step6(step6_image);
	drawStepImage(graphics_step6, COL_TEXT_BLUE, 6);
	m_waves.push_back((step6_image.createCopy()));
	step6_image.clear(step6_image.getBounds());
	drawStepImage(graphics_step6, highlight_colour, 6);
	m_waves_highlight.push_back(step6_image.createCopy());

	// step 8
	juce::Image step8_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_step8(step8_image);
	drawStepImage(graphics_step8, COL_TEXT_BLUE, 8);
	m_waves.push_back((step8_image.createCopy()));
	step8_image.clear(step8_image.getBounds());
	drawStepImage(graphics_step8, highlight_colour, 8);
	m_waves_highlight.push_back(step8_image.createCopy());

	// step 12
	juce::Image step12_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_step12(step12_image);
	drawStepImage(graphics_step12, COL_TEXT_BLUE, 12);
	m_waves.push_back((step12_image.createCopy()));
	step12_image.clear(step12_image.getBounds());
	drawStepImage(graphics_step12, highlight_colour, 12);
	m_waves_highlight.push_back(step12_image.createCopy());

	// WD1
	juce::Image WD1_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_WD1(WD1_image);
	drawTextImage(graphics_WD1, COL_TEXT_BLUE, "WaDraw1");
	m_waves.push_back((WD1_image.createCopy()));
	WD1_image.clear(WD1_image.getBounds());
	drawTextImage(graphics_WD1, highlight_colour, "WaDraw1");
	m_waves_highlight.push_back(WD1_image.createCopy());

	// WD2
	juce::Image WD2_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_WD2(WD2_image);
	drawTextImage(graphics_WD2, COL_TEXT_BLUE, "WaDraw2");
	m_waves.push_back((WD2_image.createCopy()));
	WD2_image.clear(WD2_image.getBounds());
	drawTextImage(graphics_WD2, highlight_colour, "WaDraw2");
	m_waves_highlight.push_back(WD2_image.createCopy());

	// WD3
	juce::Image WD3_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_WD3(WD3_image);
	drawTextImage(graphics_WD3, COL_TEXT_BLUE, "WaDraw3");
	m_waves.push_back((WD3_image.createCopy()));
	WD3_image.clear(WD3_image.getBounds());
	drawTextImage(graphics_WD3, highlight_colour, "WaDraw3");
	m_waves_highlight.push_back(WD3_image.createCopy());

	// CD1
	juce::Image CD1_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_CD1(CD1_image);
	drawTextImage(graphics_CD1, COL_TEXT_BLUE, "ChDraw1");
	m_waves.push_back((CD1_image.createCopy()));
	CD1_image.clear(CD1_image.getBounds());
	drawTextImage(graphics_CD1, highlight_colour, "ChDraw1");
	m_waves_highlight.push_back(CD1_image.createCopy());

	// CD2
	juce::Image CD2_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_CD2(CD2_image);
	drawTextImage(graphics_CD2, COL_TEXT_BLUE, "ChDraw2");
	m_waves.push_back((CD2_image.createCopy()));
	CD2_image.clear(CD2_image.getBounds());
	drawTextImage(graphics_CD2, highlight_colour, "ChDraw2");
	m_waves_highlight.push_back(CD2_image.createCopy());

	// CD3
	juce::Image CD3_image(juce::Image::PixelFormat::ARGB, W, H, true);
	juce::Graphics graphics_CD3(CD3_image);
	drawTextImage(graphics_CD3, COL_TEXT_BLUE, "ChDraw3");
	m_waves.push_back((CD3_image.createCopy()));
	CD3_image.clear(CD3_image.getBounds());
	drawTextImage(graphics_CD3, highlight_colour, "ChDraw3");
	m_waves_highlight.push_back(CD3_image.createCopy());
}

void LFODisplayComponent::drawSineImage(juce::Graphics &g, juce::Colour p_col) {
	auto bounds = g.getClipBounds().toFloat();
	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));

	static constexpr auto num_steps = 17;
	static constexpr auto stroke    = 1.0f;
	juce::Path p;
	p.startNewSubPath(bounds.getX(), bounds.getCentreY());
	for (int step = 1; step < num_steps; ++step) {
		const auto x = float(step) / float(num_steps - 1);
		p.lineTo(bounds.getRelativePoint(x, 0.5f - 0.5f * std::sin(x * juce::MathConstants<float>::twoPi)));
	}

	auto fill_path = p;
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));
}

void LFODisplayComponent::drawSawImage(juce::Graphics &g, juce::Colour p_col) {
	auto bounds                  = g.getClipBounds().toFloat();
	static constexpr auto stroke = 1.0f;

	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));

	juce::Path p;
	p.startNewSubPath(bounds.getX(), bounds.getCentreY());
	p.lineTo(bounds.getRelativePoint(0.0f, 0.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 1.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 0.5f));

	auto fill_path = p;
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));
}

void LFODisplayComponent::drawTriangleImage(juce::Graphics &g, juce::Colour p_col) {
	auto bounds                  = g.getClipBounds().toFloat();
	static constexpr auto stroke = 1.0f;

	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));

	juce::Path p;
	p.startNewSubPath(bounds.getX(), bounds.getCentreY());
	p.lineTo(bounds.getRelativePoint(0.25f, 0.0f));
	p.lineTo(bounds.getRelativePoint(0.5f, 0.5f));
	p.lineTo(bounds.getRelativePoint(0.75f, 1.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 0.5f));

	auto fill_path = p;
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));
}

void LFODisplayComponent::drawSquareImage(juce::Graphics &g, juce::Colour p_col, float p_width) {
	auto bounds                  = g.getClipBounds().toFloat();
	static constexpr auto stroke = 1.0f;

	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));

	juce::Path p;
	p.startNewSubPath(bounds.getX(), bounds.getCentreY());
	p.lineTo(bounds.getRelativePoint(0.0f, 0.0f));
	p.lineTo(bounds.getRelativePoint(p_width, 0.0f));
	p.lineTo(bounds.getRelativePoint(p_width, 1.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 1.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 0.5f));

	auto fill_path = p;
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));
}

void LFODisplayComponent::drawPointyImage(juce::Graphics &g, juce::Colour p_col) {
	auto bounds = g.getClipBounds().toFloat();
	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));

	static constexpr auto num_steps = 17;
	static constexpr auto stroke    = 1.0f;
	juce::Path p;
	p.startNewSubPath(bounds.getX(), bounds.getBottom());
	for (int step = 1; step < num_steps; ++step) {
		const auto x      = float(step) / float(num_steps - 1);
		const auto x_func = x > 0.5f ? 1.0f - x : x;
		p.lineTo(bounds.getRelativePoint(x, 1.0f - 8.0f * (x_func * x_func * x_func)));
	}

	auto fill_path = p;
	fill_path.lineTo(bounds.getRelativePoint(1.0f, 0.5f));
	fill_path.lineTo(bounds.getRelativePoint(0.0f, 0.5f));
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));
}

void LFODisplayComponent::drawSHImage(juce::Graphics &g, juce::Colour p_col) {
	auto bounds                  = g.getClipBounds().toFloat();
	static constexpr auto stroke = 1.0f;

	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));
	bounds.removeFromRight(bounds.proportionOfWidth(0.3f));

	juce::Path p;
	p.startNewSubPath(bounds.getRelativePoint(0.0f, 0.35f));
	p.lineTo(bounds.getRelativePoint(0.25f, 0.35f));
	p.lineTo(bounds.getRelativePoint(0.25f, 0.65f));
	p.lineTo(bounds.getRelativePoint(0.5f, 0.65f));
	p.lineTo(bounds.getRelativePoint(0.5f, 0.0f));
	p.lineTo(bounds.getRelativePoint(0.75f, 0.0f));
	p.lineTo(bounds.getRelativePoint(0.75f, 1.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 1.0f));

	auto fill_path = p;
	fill_path.lineTo(bounds.getRelativePoint(1.0f, 0.5f));
	fill_path.lineTo(bounds.getRelativePoint(0.0f, 0.5f));
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));

	g.setFont(Helpers::getAldrichFont(bounds.getHeight() * 0.9f));
	g.drawText("SH", g.getClipBounds().withLeft(bounds.getRight()), juce::Justification::centred);
}

void LFODisplayComponent::drawPyramidImage(juce::Graphics &g, juce::Colour p_col, int p_steps) {
	auto bounds                  = g.getClipBounds().toFloat();
	static constexpr auto stroke = 1.0f;

	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));
	bounds.removeFromRight(bounds.proportionOfWidth(0.3f));

	juce::Path p;
	p.startNewSubPath(bounds.getRelativePoint(0.0f, 1.0f));
	p.lineTo(bounds.getRelativePoint(0.2f, 1.0f));
	p.lineTo(bounds.getRelativePoint(0.2f, 0.5f));
	p.lineTo(bounds.getRelativePoint(0.4f, 0.5f));
	p.lineTo(bounds.getRelativePoint(0.4f, 0.0f));
	p.lineTo(bounds.getRelativePoint(0.6f, 0.0f));
	p.lineTo(bounds.getRelativePoint(0.6f, 0.5f));
	p.lineTo(bounds.getRelativePoint(0.8f, 0.5f));
	p.lineTo(bounds.getRelativePoint(0.8f, 1.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 1.0f));

	auto fill_path = p;
	fill_path.lineTo(bounds.getRelativePoint(1.0f, 0.5f));
	fill_path.lineTo(bounds.getRelativePoint(0.0f, 0.5f));
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));

	g.setFont(Helpers::getAldrichFont(bounds.getHeight() * 0.9f));
	g.drawText(juce::String(p_steps), g.getClipBounds().withLeft(bounds.getRight()), juce::Justification::centred);
}

void LFODisplayComponent::drawStepImage(juce::Graphics &g, juce::Colour p_col, int p_steps) {
	auto bounds                  = g.getClipBounds().toFloat();
	static constexpr auto stroke = 1.0f;

	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));
	bounds.removeFromRight(bounds.proportionOfWidth(0.3f));

	juce::Path p;
	p.startNewSubPath(bounds.getRelativePoint(0.0f, 1.0f));
	p.lineTo(bounds.getRelativePoint(0.25f, 1.0f));
	p.lineTo(bounds.getRelativePoint(0.25f, 0.66f));
	p.lineTo(bounds.getRelativePoint(0.5f, 0.66f));
	p.lineTo(bounds.getRelativePoint(0.5f, 0.33f));
	p.lineTo(bounds.getRelativePoint(0.75f, 0.33f));
	p.lineTo(bounds.getRelativePoint(0.75f, 0.0f));
	p.lineTo(bounds.getRelativePoint(1.0f, 0.0f));

	auto fill_path = p;
	fill_path.lineTo(bounds.getRelativePoint(1.0f, 0.5f));
	fill_path.lineTo(bounds.getRelativePoint(0.0f, 0.5f));
	fill_path.closeSubPath();
	g.setColour(COL_TEXT_BLUE.withAlpha(0.2f));
	g.fillPath(fill_path);

	g.setColour(p_col);
	g.strokePath(p, juce::PathStrokeType(stroke));

	g.setFont(Helpers::getAldrichFont(bounds.getHeight() * 0.9f));
	g.drawText(juce::String(p_steps), g.getClipBounds().withLeft(bounds.getRight()), juce::Justification::centred);
}

void LFODisplayComponent::drawTextImage(juce::Graphics &g, juce::Colour p_col, juce::String p_text) {
	auto bounds = g.getClipBounds().toFloat();
	bounds.reduce(bounds.proportionOfWidth(0.15f), bounds.proportionOfHeight(0.2f));

	g.setColour(p_col);
	g.setFont(Helpers::getAldrichFont(bounds.getHeight() * 0.8f));
	g.drawText(p_text, g.getClipBounds(), juce::Justification::centred, false);
}
