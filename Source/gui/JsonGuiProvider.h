#pragma once

#include "../GlobalIncludes.h"
#include "GuiDataEditor.h"
#include "../../libs/json/include/nlohmann/json.hpp"

namespace tww {


class JsonGuiProvider {
public:
	JsonGuiProvider();
	~JsonGuiProvider() = default;

	void loadData();
	juce::Rectangle<int> getBounds(const std::string &p_name, bool p_for_knob = false);
	juce::Rectangle<int> getBoundsMirrored(const std::string &p_name, const std::string& p_parent);
	juce::Rectangle<int> getBoundsUnscaled(const std::string &p_name);
	juce::Rectangle<float> getFloatBounds(const std::string &p_name);
	juce::Rectangle<int> getFloatBoundsWithIntSize(const std::string &p_name, bool p_for_knob = false);
	juce::Image getImage(const std::string &p_name);
	juce::Point<int> getKnobInlay();

	void setBoundsUnscaled(const std::string &p_name, const juce::Rectangle<int>& p_bounds);

	void saveToFile();

protected:
	nlohmann::json m_data;

	GuiDataEditor m_gui_editor;
	juce::AlertWindow m_alert_window;

	const std::string m_name;

};
} // namespace tww