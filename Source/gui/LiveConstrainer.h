#pragma once

#include "../GlobalIncludes.h"
#include "JsonGuiProvider.h"

class OdinEditor;

class LiveConstrainer : public juce::MouseListener {
public:
	enum class DragMode { None, Center, Left, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft };

	LiveConstrainer(OdinEditor &p_editor);
	~LiveConstrainer();

	void mouseDown(const juce::MouseEvent &p_event) override;
	void mouseMove(const juce::MouseEvent &p_event) override;
	void mouseDrag(const juce::MouseEvent &p_event) override;
	void mouseUp(const juce::MouseEvent &p_event) override;

	void paintOverlay(juce::Graphics &g);

	bool isConstraining();

protected:
	static constexpr auto SNAP_DISTANCE = 10;

	void updateComponent();
	void unregisterCurrentComponent();

	juce::MouseCursor positionToMouseCursor(juce::Point<float> p_pos);
	DragMode cursorToDragMode(const juce::MouseCursor &p_cursor);

	juce::Component *m_component = nullptr;

	OdinEditor &m_main_editor;

	std::string m_name;
	juce::Rectangle<int> m_bounds;
	juce::Rectangle<int> m_bounds_scaled;
	juce::Rectangle<int> m_new_bounds;
	juce::Point<float> m_drag_start;
	DragMode m_drag_mode = DragMode::None;

	bool m_was_enabled;

	juce::String m_last_event;
};