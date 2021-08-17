#include "ImpulseResponseCreator.h"

void ImpulseResponseCreator::saveImpulseResponse(std::function<float(float)> p_function,
                                                 size_t p_length,
                                                 const std::string &p_path) {

	AudioBuffer<float> buffer(1, p_length);
	WavAudioFormat format;
	std::unique_ptr<AudioFormatWriter> writer;

  time_t rawtime;
	struct tm *timeinfo;
	char bufferino[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(bufferino, sizeof(bufferino), "%d-%m-%Y_%H:%M:%S", timeinfo);
	std::string str(bufferino);

	for (size_t sample = 0; sample < p_length; ++sample) {
		buffer.setSample(0, sample, p_function((int)(sample == 0)));
    //DBG("smpl: " << buffer.getSample(0, sample));
	}

	writer.reset(
	    format.createWriterFor(new FileOutputStream(File(p_path + "_" + str + ".wav")), 48000.0, buffer.getNumChannels(), 24, {}, 0));
	if (writer != nullptr)
		writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
}