#include "JsonGuiProvider.h"
#include "../ConfigFileManager.h"

JsonGuiProvider::JsonGuiProvider() : m_alert_window("Create GUI Data", "", juce::MessageBoxIconType::InfoIcon) {

	//load the JSON data:
	loadData();

	m_gui_editor.setBounds(0, 0, GUI_BASE_WIDTH, GUI_BASE_HEIGHT);
	//m_gui_editor.setTopBottom (getBoundsUnscaled ("TopSection"), getBoundsUnscaled ("BottomSection"));
	m_alert_window.addCustomComponent(&m_gui_editor);
	m_alert_window.addButton("Create", 0, juce::KeyPress(juce::KeyPress::returnKey));
}

void JsonGuiProvider::loadData() {

	m_data = nlohmann::json::parse(
	    juce::String::createStringFromData(BinaryData::GuiData_json, BinaryData::GuiData_jsonSize).toStdString());
#ifdef ODIN_DEBUG
#define GUI_DATA_PATH CMAKE_SOURCE_DIRECTORY "/Source/GuiData.json"
	const auto file = juce::File(GUI_DATA_PATH);
	if (file.existsAsFile()) {
		m_data = nlohmann::json::parse(file.loadFileAsString().toStdString());
	} else
		DBG("\n\nError: Can't find local GuiData.json\n\n");
#endif
}

void JsonGuiProvider::saveToFile() {
#ifdef ODIN_DEBUG
	juce::File file(GUI_DATA_PATH);
	file.replaceWithText(m_data.dump(2));
#endif
}

juce::Rectangle<int> JsonGuiProvider::getBounds(const std::string &p_name, bool p_for_knob) {
	if (!m_data.contains(p_name)) {
		m_alert_window.setMessage(p_name);
		m_alert_window.runModalLoop();
		setBoundsUnscaled(p_name, m_gui_editor.getData());
	}

	auto zoom_size = ConfigFileManager::getInstance().getOptionGuiScale();
	auto ret       = getBoundsUnscaled(p_name);

	return ret * int(zoom_size);
}

juce::Rectangle<int> JsonGuiProvider::getBoundsMirrored(const std::string &p_name, const std::string &p_parent) {
	jassert(m_data.contains(p_name));
	jassert(m_data.contains(p_parent));

	auto zoom_size = ConfigFileManager::getInstance().getOptionGuiScale();
	auto ret       = getBoundsUnscaled(p_name);
	auto parent    = getBoundsUnscaled(p_parent);

	// mirror x on parent width
	ret.setX(parent.getWidth() - ret.getRight());

	return ret * int(zoom_size);
}

juce::Rectangle<int> JsonGuiProvider::getBoundsUnscaled(const std::string &p_name) {
	jassert(m_data.contains(p_name));

	return juce::Rectangle<int>((m_data[p_name]["X"].get<int>()),
	                            (m_data[p_name]["Y"].get<int>()),
	                            (m_data[p_name]["SizeX"].get<int>()),
	                            (m_data[p_name]["SizeY"].get<int>()));
}

juce::Rectangle<float> JsonGuiProvider::getFloatBounds(const std::string &p_name) {
	jassert(m_data.contains(p_name));

	auto zoom_size = ConfigFileManager::getInstance().getOptionGuiScale();
	return juce::Rectangle<float>((m_data[p_name]["X"].get<float>()),
	                              (m_data[p_name]["Y"].get<float>()),
	                              (m_data[p_name]["SizeX"].get<float>()),
	                              (m_data[p_name]["SizeY"].get<float>())) *
	       float(zoom_size);
}

juce::Rectangle<int> JsonGuiProvider::getFloatBoundsWithIntSize(const std::string &p_name, bool p_for_knob) {
	jassert(m_data.contains(p_name));

	auto zoom_size = ConfigFileManager::getInstance().getOptionGuiScale();
	auto ret = juce::Rectangle<int>((juce::roundToInt(m_data[p_name]["X"].get<float>() * static_cast<int>(zoom_size))),
	                                (juce::roundToInt(m_data[p_name]["Y"].get<float>() * static_cast<int>(zoom_size))),
	                                (m_data[p_name]["SizeX"].get<int>()) * static_cast<int>(zoom_size),
	                                (m_data[p_name]["SizeY"].get<int>()) * static_cast<int>(zoom_size));

	if (p_for_knob) {
		const auto knob_inlay = getKnobInlay();
		ret.expand(knob_inlay.getX(), knob_inlay.getY());
	}

	return ret;
}

juce::Image JsonGuiProvider::getImage(const std::string &p_name) {

	//auto zoom_size = ConfigFileManager::getInstance().getOptionGuiScale();

	// todo resource name is does not correspond to our "zoom" name here... Currently Z100 is myImage150.png
	//const std::string resource_name = p_name + std::to_string (int (zoom_size) * 50) + "_png";
	const std::string resource_name = p_name + "300_png";

	if (!m_data.contains(p_name)) {
		//jassertfalse;
		//DBG("No size data for image found in GUIData: " << resource_name);
	}

	int size;
	const char *data = BinaryData::getNamedResource((resource_name).c_str(), size);

	if (!data) {
		jassertfalse;
		DBG("Can't find binary resource " << resource_name);
	}

	return juce::ImageCache::getFromMemory(data, size);
}

juce::Point<int> JsonGuiProvider::getKnobInlay() {

	const auto zoom_size = ConfigFileManager::getInstance().getOptionGuiScale();
	return juce::Point<int>(1 * static_cast<int>(zoom_size), 1 * static_cast<int>(zoom_size));
}

void JsonGuiProvider::setBoundsUnscaled(const std::string &p_name, const juce::Rectangle<int> &p_bounds) {
	m_data[p_name]["X"]     = p_bounds.getX();
	m_data[p_name]["Y"]     = p_bounds.getY();
	m_data[p_name]["SizeX"] = p_bounds.getWidth();
	m_data[p_name]["SizeY"] = p_bounds.getHeight();
}