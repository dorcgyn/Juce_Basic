#pragma once

#include "AudioReader.h"

AudioReader::AudioReader()
{
	formatManager.registerBasicFormats();
}

AudioReader::~AudioReader() {}

void AudioReader::load(Synthesiser& synth, const String& dirPath, Group& group, MidiKeyboardComponent& /*keyboardComponent*/)
{
	const Array<File>& files = getAllFiles(dirPath);
	for (File file : files)
	{
		String extention = file.getFileExtension();
		if (extention != ".wav") {
			continue;
		}

		GroupSamplerSound* sound = parseFileName(file, group);
		synth.addSound(sound);
	}
}

Array<File> AudioReader::getAllFiles(const String& path)
{
	File dir(path);
	return dir.findChildFiles(dir.findFiles, false);
}


// assume file name is rootKey_startKey_numKey_???
GroupSamplerSound* AudioReader::parseFileName(File& file, Group& group)
{
	const String& fileName = file.getFileNameWithoutExtension();
	
	if (!fileName.contains("_")) {
		return nullptr;
	}

	int startIndex = 0;
	int rootKey;
	int startKey;
	int numKey;

	int matchIndex = fileName.indexOf(startIndex, "_");
	rootKey = fileName.substring(startIndex, matchIndex).getIntValue();
	startIndex = matchIndex + 1;
	matchIndex = fileName.indexOf(startIndex, "_");
	startKey = fileName.substring(startIndex, matchIndex).getIntValue();
	startIndex = matchIndex + 1;
	matchIndex = fileName.indexOf(startIndex, "_");
	numKey = fileName.substring(startIndex, matchIndex).getIntValue();

	std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));

	BigInteger note;
	note.setRange(startKey, numKey, true);

	// finally, add our sound
	return new GroupSamplerSound(group.getGroupName() + String(rootKey), *reader, note, rootKey, 0, 0, 100.0, group);
}