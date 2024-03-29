#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthAudioSource.h"
#include "GroupSamplerVoice.h"

const ReferenceCountedArray<SynthesiserSound>& GroupSynthesiser::getAllSounds() const
{
	return sounds;
}

SynthAudioSource::SynthAudioSource(MidiKeyboardState& keyboardState, MidiKeyboardComponent& keyboardComponent)
	: keyboardState(keyboardState)
	, keyboardComponent(keyboardComponent)
{
	for (auto i = 0; i < 4; ++i)
		synth.addVoice(new GroupSamplerVoice());	
}

void SynthAudioSource::loadDir(const String& dirPath, const String& groupName)
{
	audioReader.loadDir(synth, dirPath, *(this->groupManager.getGroup(groupName)), keyboardComponent);
}

void SynthAudioSource::loadFile(const String& filePath, const String& groupName)
{
	audioReader.loadFile(synth, filePath, *(this->groupManager.getGroup(groupName)), keyboardComponent);
}

void SynthAudioSource::cleanup()
{
	synth.clearSounds();
	groupManager.cleanup();
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

GroupSynthesiser& SynthAudioSource::getSynthesiser()
{
	return synth;
}