#include "GroupSamplerVoice.h"
#include "GroupSamplerSound.h"


GroupSamplerVoice::GroupSamplerVoice():SamplerVoice()
{
}

void GroupSamplerVoice::renderNextBlock(AudioBuffer<float>& audioBuffer, int startSample, int numSamples)
{
	SamplerVoice::renderNextBlock(audioBuffer, startSample, numSamples);

	if (auto * groupSamplerSound = static_cast<GroupSamplerSound*> (getCurrentlyPlayingSound().get())) {
		groupSamplerSound->applyEffect(&audioBuffer, startSample, numSamples);
	}
}