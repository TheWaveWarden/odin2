#pragma once

#include "../GlobalIncludes.h"

class RescaleProgressComponent : public juce::Component {
public:
	RescaleProgressComponent();
	~RescaleProgressComponent() = default;

	void setProgress (float p_progress);
	void paint (juce::Graphics& g);

protected:
	float m_progress = 0.0f;
};