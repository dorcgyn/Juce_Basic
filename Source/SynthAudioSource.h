#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioReader.h"

namespace juce
{

	class SynthAudioSource : public AudioSource
	{
	public:
		SynthAudioSource(MidiKeyboardState& keyState);

		void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

		void releaseResources() override;

		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

		MidiMessageCollector* getMidiCollector();

		void load(String& dirPath, String& groupName);

	private:
		MidiKeyboardState& keyboardState;
		AudioReader audioReader;
		Synthesiser synth;
		MidiMessageCollector midiCollector;
	};

}