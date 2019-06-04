#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioReader.h"

namespace juce
{

	class SynthAudioSource : public AudioSource
	{
	public:
		SynthAudioSource(MidiKeyboardState& keyState, MidiKeyboardComponent& keyboardComponent);

		void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

		void releaseResources() override;

		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

		MidiMessageCollector* getMidiCollector();

		void load(const String& dirPath, const String& groupName);

	private:
		MidiKeyboardState& keyboardState;
		MidiKeyboardComponent& keyboardComponent;
		AudioReader audioReader;
		Synthesiser synth;
		MidiMessageCollector midiCollector;
	};

}