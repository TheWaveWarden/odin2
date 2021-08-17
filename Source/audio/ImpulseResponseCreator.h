#pragma once

#include <functional>
#include <JuceHeader.h>

class ImpulseResponseCreator {
public:
	static void saveImpulseResponse(std::function<float(float)> p_function, size_t p_length, const std::string& p_path);	
};
