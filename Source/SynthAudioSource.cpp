#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthAudioSource.h"

SynthAudioSource::SynthAudioSource(MidiKeyboardState& keyboardState, MidiKeyboardComponent& keyboardComponent)
	: keyboardState(keyboardState)
	, keyboardComponent(keyboardComponent)
{
	for (auto i = 0; i < 4; ++i)
		synth.addVoice(new SamplerVoice());

	
	/*
	formatManager.registerBasicFormats();

	for (auto i = 0; i < 4; ++i)
		synth.addVoice(new SamplerVoice());

	File file("C:\\Users\\dorcg\\Desktop\\1_A2_5SlowMP.wav");
	// 0_128_66_    5_1_123
	std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));

	BigInteger note1;
	note1.setRange(0, 128, true);

	// finally, add our sound
	synth.addSound(new SamplerSound("1", *reader, note1, 66, 0, 0, 100.0));
	*/
	
}

void SynthAudioSource::load(const String& dirPath, const String& groupName)
{
	audioReader.load(synth, dirPath, *(this->groupManager.getGroup(groupName)), keyboardComponent);
}

void SynthAudioSource::prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate)
{
	synth.setCurrentPlaybackSampleRate(sampleRate); // [3]
	midiCollector.reset(sampleRate);
}

void SynthAudioSource::releaseResources() {}

void SynthAudioSource::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();
	MidiBuffer incomingMidi;
	midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
	keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
		bufferToFill.numSamples, true);       
	synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
		bufferToFill.startSample, bufferToFill.numSamples);
}

MidiMessageCollector* SynthAudioSource::getMidiCollector()
{
	return &midiCollector;
}

GroupManager* SynthAudioSource::getGroupManager()
{
	return &groupManager;
}