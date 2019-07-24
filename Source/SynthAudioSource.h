#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioReader.h"
#include "GroupSamplerSound.h"

namespace juce
{

	class GroupSynthesiser : public Synthesiser
	{
	public:
		const ReferenceCountedArray<SynthesiserSound>& getAllSounds() const;
	private:
	};


	class SynthAudioSource : public AudioSource
	{
	public:
		SynthAudioSource(MidiKeyboardState& keyState, MidiKeyboardComponent& keyboardComponent);

		void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override;

		void releaseResources() override;

		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

		MidiMessageCollector* getMidiCollector();

		void loadDir(const String& dirPath, const String& groupName);
		void loadFile(const String& filePath, const String& groupName);
		void cleanup();

		GroupManager* getGroupManager();

		GroupSynthesiser& getSynthesiser();

	private:
		MidiKeyboardState& keyboardState;
		MidiKeyboardComponent& keyboardComponent;
		AudioReader audioReader;
		GroupSynthesiser synth;
		MidiMessageCollector midiCollector;
		GroupManager groupManager;
	};

}