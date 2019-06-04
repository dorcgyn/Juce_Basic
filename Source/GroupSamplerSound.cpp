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
									 const Group& group)
	: SamplerSound::SamplerSound(soundName, source, notes, midiNoteForNormalPitch, attackTimeSecs, releaseTimeSecs, maxSampleLengthSeconds),
	group(group)
{
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

// Implementation of Group
Group::Group(const String& name) : name(name), enable(true) {};
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

// Implementation of Group Manager
GroupManager::GroupManager() {};
GroupManager::~GroupManager() {};

Group GroupManager::getGroup(const String& name)
{
	if (groupMap.find(name) == groupMap.end())
	{
		Group group(name);
		groupMap[name] = name;
	}
	return groupMap.find(name)->second;
}

void GroupManager::enableAll()
{
	for (auto& const groupIter : groupMap) 
	{
		groupIter.second.setEnable(true);
	}
}

void GroupManager::disableAll()
{
	for (auto& const groupIter : groupMap)
	{
		groupIter.second.setEnable(false);
	}
}

void GroupManager::enableByName(String& name)
{
	const auto& groupIter = groupMap.find(name);
	if (groupIter != groupMap.end())
	{
		groupIter->second.setEnable(true);
	}
	else 
	{
		// No group in this name
		jassertfalse;
	}
}

void GroupManager::disableByName(String& name)
{
	const auto& groupIter = groupMap.find(name);
	if (groupIter != groupMap.end())
	{
		groupIter->second.setEnable(false);
	}
	else
	{
		// No group in this name
		jassertfalse;
	}
}

}


