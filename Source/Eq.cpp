#pragma once

#include "Eq.h"

namespace juce
{

Eq::Eq() {}

Eq::~Eq(){}

void Eq::apply(AudioBuffer<float>* outputAudio, int startSample, int numSamples) const
{
	int numChannels = outputAudio->getNumChannels();

	for (int channelIdx = 0; channelIdx < numChannels; channelIdx++) {
		float* audioPtr = outputAudio->getWritePointer(channelIdx, startSample);
		for (int sampleIdx = 0; sampleIdx < numSamples; sampleIdx++) {
			*audioPtr = this->apply(*audioPtr);
			audioPtr++;
		}
	}
}

// Begin EnhanceEq Implemention
EnhanceEq::EnhanceEq(float multiplex)
	:multiplex(multiplex)
{
}

float EnhanceEq::apply(float outputAudio) const {
	return outputAudio * multiplex;
}

void EnhanceEq::setMultiplex(float const multiplexNum)
{
	this->multiplex = multiplexNum;
}

float EnhanceEq::getMultiplex() const
{
	return multiplex;
}

// Begin EqContainer Implementation
void EqContainer::apply(AudioBuffer<float>* outputAudio, int startSample, int numSamples) const
{
	for (Eq* eq : eqList) {
		eq->apply(outputAudio, startSample, numSamples);
	}
}

void EqContainer::addEq(Eq& eq) 
{
	eqList.add(&eq);
}

Eq* EqContainer::getEq()
{
	for (Eq* eq : eqList) {
		if (dynamic_cast<EnhanceEq*>(eq) == nullptr) {
			continue;
		}
		return eq;
	}
	
	// If not exist, create one.
	Eq* eq = new EnhanceEq(1);
	eqList.add(eq);
	return eq;
}

}