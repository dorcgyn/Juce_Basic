#pragma once

#include "Config.h"

Config::Config() {}

Config::~Config() {}

void Config::serialize(SynthAudioSource& synthAudioSource, File& file) const
{
	GroupManager* groupManager = synthAudioSource.getGroupManager();
	Array<String> groupNameList = groupManager->getAllGroupNames();

	ValueTree configValueTree{"Config"};
	for (const String& name : groupNameList) {
		Group*  group = groupManager->getGroup(name);
		ValueTree* groupValueTree = group->serailzeToValueTree();
		configValueTree.appendChild(*groupValueTree, nullptr);
	}
	
	GroupSynthesiser& synth = synthAudioSource.getSynthesiser();
	const ReferenceCountedArray<SynthesiserSound>&  allSounds = synth.getAllSounds();
	
	for (const auto sound : allSounds)
	{
		GroupSamplerSound* groupSamplerSound = (GroupSamplerSound*) sound;
		configValueTree.appendChild(*(groupSamplerSound->serailzeToValueTree()), nullptr);
	}

	FileOutputStream stream(file);
	if (stream.openedOk())
	{
		stream.setPosition(0);
		stream.truncate();
		configValueTree.writeToStream(stream);
		stream.flush();
	}
}

void Config::deserialize(SynthAudioSource& synthAudioSource, File& file) const
{
	FileInputStream* inputStream = file.createInputStream();
	ValueTree configValueTree = ValueTree::readFromStream(*inputStream);

	synthAudioSource.cleanup();

	GroupManager* groupManager = synthAudioSource.getGroupManager();

	int childCount = configValueTree.getNumChildren();
	for (int i = 0; i < childCount; i++) {
		ValueTree valueTree = configValueTree.getChild(i);

		String idName = valueTree.getType().toString();
		if (idName == "Group") {
			groupManager->getGroup(valueTree.getProperty("name"));
		}
		else if (idName == "Sound") {
			String groupName = valueTree.getProperty("group");

			String soundName = valueTree.getProperty("name");
			String filePath = valueTree.getProperty("filePath");
			String attackTimeSecs = valueTree.getProperty("attackTimeSecs");
			String releaseTimeSecs = valueTree.getProperty("releaseTimeSecs");
			String maxSampleLengthSeconds = valueTree.getProperty("maxSampleLengthSeconds");
			
			synthAudioSource.loadFile(filePath, groupName);
		}
	}
}
