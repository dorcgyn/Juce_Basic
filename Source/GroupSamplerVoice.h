#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace juce
{
	class GroupSamplerVoice: public SamplerVoice
	{
	public:
		GroupSamplerVoice();

		void renderNextBlock(AudioBuffer<float>&, int startSample, int numSamples) override;
	};
}