/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthAudioSource.h"
#include "ColourMidiKeyboardComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
	TextButton loadAudioButton;
	void loadAudioButtonClicked();
	TextEditor groupNameTextEditor;
	Label groupNameLabel;

	ComboBox groupList;
	Label groupListLabel;

	MidiKeyboardState keyboardState;  
	ColourMidiKeyboardComponent keyboardComponent;

	SynthAudioSource audioSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
