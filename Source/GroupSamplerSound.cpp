#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GroupSamplerSound.h"

namespace juce
{

GroupSamplerSound::GroupSamplerSound(const String& soundName,
									 AudioFormatReader& source,
									 const BigInteger& notes,
									 int midiNoteForNormalPitch,
									 double attackTimeSecs,
									 double releaseTimeSecs,
									 double maxSampleLengthSeconds,
									 Group& group,
									 const String& filePath)
	: SamplerSound::SamplerSound(soundName, source, notes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds),
	group(group), 
	filePath(filePath), 
	attackTimeSecs(attackTimeSecs), 
	releaseTimeSecs(releaseTimeSecs), 
	maxSampleLengthSeconds(maxSampleLengthSeconds)
{
	int startIndex = notes.findNextSetBit(0);
	while (startIndex != -1) {
		group.addActiveMidiNote(startIndex);
		startIndex = notes.findNextSetBit(startIndex + 1);
	}
}

GroupSamplerSound::~GroupSamplerSound()
{
}

bool GroupSamplerSound::appliesToNote(int midiNoteNumber)
{
	if (group.isEnable()) {
		return SamplerSound::appliesToNote(midiNoteNumber);
	}
	return false;
}

ValueTree* GroupSamplerSound::serailzeToValueTree() const
{
	ValueTree* soundValueTree = new ValueTree{"Sound"};
	soundValueTree->setProperty("name", this->getName(), nullptr);
	soundValueTree->setProperty("filePath", filePath, nullptr);
	soundValueTree->setProperty("attackTimeSecs", attackTimeSecs, nullptr);
	soundValueTree->setProperty("releaseTimeSecs", releaseTimeSecs, nullptr);
	soundValueTree->setProperty("maxSampleLengthSeconds", maxSampleLengthSeconds, nullptr);
	soundValueTree->setProperty("group", group.getGroupName(), nullptr);
	return soundValueTree;
}

// Implementation of Group
Group::Group(const String& name) : 
	name(name), 
	enable(true), 
	keyColour(Colours::lightblue)
{};

Group::Group() : Group::Group(String()) {};

Group::~Group() {};

bool Group::isEnable()
{
	return enable;
}

void Group::setEnable(bool enableFlag)
{
	this->enable = enableFlag;
}

String Group::getGroupName() const
{
	return name;
}

void Group::setKeyColour(Colour newColour)
{
	this->keyColour = newColour;
}

Colour Group::getkeyColour() const
{
	return this->keyColour;
}

void Group::addActiveMidiNote(int midiNote)
{
	this->activeMidiNotes.add(midiNote);
}

bool Group::isActiveMidiKey(int midiNote) const
{
	return this->activeMidiNotes.contains(midiNote);
}

ValueTree* Group::serailzeToValueTree() const
{
	ValueTree* groupValueTree = new ValueTree{"Group"};
	groupValueTree->setProperty("name", name, nullptr);
	return groupValueTree;
}


// Implementation of Group Manager
GroupManager::GroupManager() {};
GroupManager::~GroupManager() {};

Group* GroupManager::getGroup(const String& name)
{
	if (!groupMap.contains(name))
	{
		Group* group = new Group(name);
		groupMap.set(name, group);
	}
	return groupMap[name];
}

void GroupManager::enableAll()
{
	for (auto group : groupMap)
	{
		group->setEnable(true);
	}
}

void GroupManager::disableAll()
{
	for (auto group : groupMap)
	{
		group->setEnable(false);
	}
}

void GroupManager::enableByName(String& name)
{
	jassert(groupMap.contains(name));
	groupMap[name]->setEnable(true);
}

void GroupManager::disableByName(String& name)
{
	jassert(groupMap.contains(name));
	groupMap[name]->setEnable(false);
}

Group* GroupManager::getFirstActiveGroup(int midiNote) const
{
	for (auto group : groupMap)
	{
		if (group->isEnable() && group->isActiveMidiKey(midiNote))
			return group;
	}
	return nullptr;
}

Array<String> GroupManager::getAllGroupNames() const
{
	Array<String> result;
	for (auto begin = groupMap.begin(); begin != groupMap.end(); begin.next())
	{
		result.add(begin.getKey());
	}
	return result;
}

void GroupManager::cleanup()
{
	groupMap.clear();
}

}


