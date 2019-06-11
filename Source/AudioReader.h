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

		void load(Synthesiser& synth, const String& dirPath, Group& group, MidiKeyboardComponent& keyboardComponent);
		
	private:
		AudioFormatManager formatManager;

		Array<File> getAllFiles(const String& path);
		GroupSamplerSound* parseFileName(File& file, Group& group);
	};
}