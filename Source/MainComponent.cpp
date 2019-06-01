/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
	 , audioSource(keyboardState)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

	addAndMakeVisible(&openButton);
	openButton.setButtonText("Play...");
	openButton.onClick = [this] { openButtonClicked(); };
	
	position = 0;

	addAndMakeVisible(keyboardComponent);
	// play = false;

	// load audioGroup
	audioSource.load(String("C:\\Users\\dorcg\\Desktop\\test"), String("group1"));

	// VERY VERY important
	setAudioChannels(0, 2);

	auto list = MidiInput::getDevices();
	auto newInput = list[0];
	if (!deviceManager.isMidiInputEnabled(newInput))
		deviceManager.setMidiInputEnabled(newInput, true);
	deviceManager.addMidiInputCallback(newInput, audioSource.getMidiCollector());
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
//	keyboardState.removeListener(this);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
 
	audioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
	/*
	if (!play) return;

	auto numInputChannels = fileBuffer.getNumChannels();
	auto numOutputChannels = bufferToFill.buffer->getNumChannels();
	auto outputSamplesRemaining = bufferToFill.numSamples;                                  // [8]
	auto outputSamplesOffset = bufferToFill.startSample;                                    // [9]
	auto bufferSamplesRemaining = fileBuffer.getNumSamples() - position;                // [10]
	auto samplesThisTime = jmin(outputSamplesRemaining, bufferSamplesRemaining);       // [11]
	
	for (auto channel = 0; channel < numOutputChannels; ++channel)
	{
		bufferToFill.buffer->copyFrom(channel,                                         // [12]
			outputSamplesOffset,                             //  [12.1]
			fileBuffer,                                      //  [12.2]
			channel % numInputChannels,                      //  [12.3]
			position,                                        //  [12.4]
			samplesThisTime);                                //  [12.5]
	}
	position += samplesThisTime;                                                        // [15]
	*/

	audioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	openButton.setBounds(10, 10, getWidth() - 20, 20);

	auto area = getLocalBounds();
	keyboardComponent.setBounds(area.removeFromBottom(80).reduced(8));
}

void MainComponent::openButtonClicked ()
{
	shutdownAudio();
	/*
	File file("C:\\Users\\dorcg\\Desktop\\1_A2_5SlowMP.wav");
	
	std::unique_ptr<AudioFormatReader> reader(formatManager.createReaderFor(file));

	if (reader.get() != nullptr) {
		fileBuffer.setSize(reader->numChannels, (int)reader->lengthInSamples);
		reader->read(&fileBuffer, 0, (int)reader->lengthInSamples, 0, true, true);
		setAudioChannels(0, 2);
	}

	position = 0;
	*/
}


// These methods handle callbacks from the midi device + on-screen keyboard..
/*
void MainComponent::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	keyboardState.processNextMidiEvent(message);
	audioSource.getMidiCollector()->addMessageToQueue(message);
}

void MainComponent::handleNoteOn(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
	auto m = MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
	audioSource.getMidiCollector()->addMessageToQueue(m);
	play = true;
}

void MainComponent::handleNoteOff(MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
	auto m = MidiMessage::noteOff(midiChannel, midiNoteNumber);
	audioSource.getMidiCollector()->addMessageToQueue(m);
	play = false;
}
*/
