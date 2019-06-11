#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace juce
{
	class Group
	{
	public:
		Group(const String& name);
		Group();
		~Group();
		
		bool isEnable();
		void setEnable(bool enableFlag);
		
		String getGroupName() const;

		void setKeyColour(Colour newColour);
		Colour getkeyColour() const;

		void addActiveMidiNote(int midiNote);
		bool isActiveMidiKey(int midiNote) const;
	private:
		String name;
		bool enable;
		SortedSet<int> activeMidiNotes;
		Colour keyColour;
	};

	class GroupSamplerSound : public SamplerSound
	{
	public:
		/** Creates a sampled sound from an audio reader.

			This will attempt to load the audio from the source into memory and store
			it in this object.

			@param name         a name for the sample
			@param source       the audio to load. This object can be safely deleted by the
								caller after this constructor returns
			@param midiNotes    the set of midi keys that this sound should be played on. This
								is used by the SynthesiserSound::appliesToNote() method
			@param midiNoteForNormalPitch   the midi note at which the sample should be played
											with its natural rate. All other notes will be pitched
											up or down relative to this one
			@param attackTimeSecs   the attack (fade-in) time, in seconds
			@param releaseTimeSecs  the decay (fade-out) time, in seconds
			@param maxSampleLengthSeconds   a maximum length of audio to read from the audio
											source, in seconds
		*/
		GroupSamplerSound(const String& name,
			AudioFormatReader& source,
			const BigInteger& midiNotes,
			int midiNoteForNormalPitch,
			double attackTimeSecs,
			double releaseTimeSecs,
			double maxSampleLengthSeconds,
			Group& group);

		/** Destructor. */
		~GroupSamplerSound() override;

		bool appliesToNote(int midiNoteNumber) override;

	private:
		Group group;
	};

	class GroupManager
	{
	public:
		GroupManager();
		~GroupManager();

		void enableAll();
		void disableAll();
		
		void enableByName(String& name);
		void disableByName(String& name);

		Group* getGroup(const String& name);
		Array<String> getAllGroupNames() const;
		Group* getFirstActiveGroup(int midiNote) const;
	private:
		HashMap<String, Group*> groupMap;
	};
}