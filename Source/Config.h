#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthAudioSource.h"

namespace juce
{
	class Config
	{
	public:
		Config();
		~Config();

		void serialize(SynthAudioSource& synthAudioSource, File& file) const;
		void deserialize(SynthAudioSource& synthAudioSource, File& file) const;
	private:
	};
}