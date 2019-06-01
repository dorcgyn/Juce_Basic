#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GroupSamplerSound.h"

namespace juce
{
	class AudioReader
	{
	public:
		AudioReader();
		~AudioReader();

		void load(Synthesiser& synth, String& dirPath, String& groupName);
		
	private:
		String dirPath;
		AudioFormatManager formatManager;
		GroupManager groupManager;

		Array<File> getAllFiles(String& path);
		GroupSamplerSound* parseFileName(File& file, Group& group);
	};
}