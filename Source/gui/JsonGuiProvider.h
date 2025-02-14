#pragma once

#include "../../libs/json/include/nlohmann/json.hpp"
#include "../GlobalIncludes.h"
#include "GuiDataEditor.h"

class JsonGuiProvider {
public:
	static JsonGuiProvider &getInstance() {
		static JsonGuiProvider instance;
		return instance;
	}
	JsonGuiProvider(JsonGuiProvider const &) = delete;
	void operator=(JsonGuiProvider const &)  = delete;

	void loadData();
	juce::Rectangle<int> getBounds(const std::string &p_name, bool p_for_knob = false);
	juce::Rectangle<int> getBoundsMirrored(const std::string &p_name, const std::string &p_parent);
	juce::Rectangle<int> getBoundsUnscaled(const std::string &p_name);
	juce::Rectangle<float> getFloatBounds(const std::string &p_name);

	void setBoundsUnscaled(const std::string &p_name, const juce::Rectangle<int> &p_bounds);

	void saveToFile();

protected:
	JsonGuiProvider();
	nlohmann::json m_data;

	GuiDataEditor m_gui_editor;
	juce::AlertWindow m_alert_window;

	const std::string m_name;
};