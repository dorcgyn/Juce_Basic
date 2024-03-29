#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Eq.h"

namespace juce
{
	// Declare GroupSamplerSound first, required for using in Group
	class GroupSamplerSound;

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

		EqContainer* getEqContainer();

		ValueTree* serailzeToValueTree() const;

	private:
		// non-copy
		Group(const Group&);
		Group& operator=(const Group&);

		String name;
		bool enable;
		SortedSet<int> activeMidiNotes;
		Colour keyColour;
		EqContainer eqContainer;
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
			Group& group,
			const String& filePath);

		/** Destructor. */
		~GroupSamplerSound() override;

		// Overwrite methods
		bool appliesToNote(int midiNoteNumber) override;
		void applyEffect(AudioBuffer<float>* outputBuffer, int startSample, int numSamples) const noexcept;
		
		EqContainer* getEqContainer();
		ValueTree* serailzeToValueTree() const;

	private:
		Group& group;
		String filePath;
		double attackTimeSecs;
		double releaseTimeSecs;
		double maxSampleLengthSeconds;
		EqContainer eqContainer;
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

		void cleanup();
	private:
		HashMap<String, Group*> groupMap;
	};
}