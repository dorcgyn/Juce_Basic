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

		void load(Synthesiser& synth, const String& dirPath, const String& groupName, MidiKeyboardComponent& keyboardComponent);
		
	private:
		AudioFormatManager formatManager;
		GroupManager groupManager;


		Array<File> getAllFiles(const String& path);
		GroupSamplerSound* parseFileName(File& file, const Group& group);
	};
}