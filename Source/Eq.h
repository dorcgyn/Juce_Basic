#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace juce
{
	class Eq
	{
	public:
		Eq();
		~Eq();

		void apply(AudioBuffer<float>* outputAudio, int startSample, int numSamples) const;
		virtual float apply(float outputAudio) const = 0;
	private:
		// non-copy
		Eq(const Eq&);
		Eq& operator=(const Eq&);
	};

	class EnhanceEq: public Eq
	{
	public:
		EnhanceEq(float multiplex);
		float apply(float outputAudio) const;

		void setMultiplex(float const multiplex);
		float getMultiplex() const;
	private:
		// non-copy
		EnhanceEq(const EnhanceEq&);
		EnhanceEq& operator=(const EnhanceEq&);

		float multiplex;
	};

	class EqContainer
	{
	public:
		EqContainer() {};
		~EqContainer() {};

		void apply(AudioBuffer<float>* outputAudio, int startSample, int numSamples) const;
		void addEq(Eq& eq);
		Eq* getEq();
	
	private:
		Array<Eq*> eqList;
	};


}
