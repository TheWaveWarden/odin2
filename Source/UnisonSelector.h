#pragma once

#include "NumberSelector.h"

#include <JuceHeader.h>

//==============================================================================
/*
*/
class UnisonSelector : public NumberSelector {
public:
	UnisonSelector();
	~UnisonSelector();

	//void paint (Graphics&) override;
	void resized() override;

	void setGUIBig() override;
	void setGUISmall() override;

	virtual void increment() override;
	virtual void decrement() override;
	virtual void setValue(int p_value) override;

private:
	std::map<int, int> m_increment_map = {{1, 2}, {2, 3}, {3, 4}, {4, 6}, {6, 12}, {12, 12}};
	std::map<int, int> m_decrement_map = {{1, 1}, {2, 1}, {3, 2}, {4, 3}, {6, 4}, {12, 6}};

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnisonSelector)
};
