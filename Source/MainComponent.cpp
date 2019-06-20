/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
	, audioSource(keyboardState, keyboardComponent)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

	addAndMakeVisible(&loadAudioButton);
	loadAudioButton.setButtonText("Load Audio Group");
	loadAudioButton.onClick = [this] { loadAudioButtonClicked(); };
	
	addAndMakeVisible(&groupNameTextEditor);
	addAndMakeVisible(&groupNameLabel);
	groupNameLabel.setText("New Group Name", dontSendNotification);
	groupNameLabel.attachToComponent(&groupNameTextEditor, true);

	addAndMakeVisible(keyboardComponent);

	// VERY VERY important
	setAudioChannels(0, 2);

	auto list = MidiInput::getDevices();
	auto newInput = list[0];
	if (!deviceManager.isMidiInputEnabled(newInput))
		deviceManager.setMidiInputEnabled(newInput, true);
	deviceManager.addMidiInputCallback(newInput, audioSource.getMidiCollector());

	// keyboard color
	keyboardComponent.setGroupManager(audioSource.getGroupManager());

	// pick group combobox
	addAndMakeVisible(groupListLabel);
	groupListLabel.setText("Active Group:", dontSendNotification);
	groupListLabel.attachToComponent(&groupList, true);

	addAndMakeVisible(groupList);
	groupList.setTextWhenNoChoicesAvailable("No Group Available");
	Array<String> groupNameList = audioSource.getGroupManager()->getAllGroupNames();
	groupList.addItemList(StringArray(groupNameList), 1);

	groupList.onChange = [this] {
		int selectedIndex = groupList.getSelectedItemIndex();
		String itemText = groupList.getItemText(selectedIndex);
		audioSource.getGroupManager()->disableAll();
		audioSource.getGroupManager()->enableByName(itemText);
		keyboardComponent.repaint();
	};
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
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
	groupNameTextEditor.setBounds(10, 10, getWidth() - 210, 20);
	loadAudioButton.setBounds(10, 50, getWidth() - 20, 20);

	groupList.setBounds(200, 500, getWidth() - 210, 20);

	auto area = getLocalBounds();
	keyboardComponent.setBounds(area.removeFromBottom(80).reduced(8));
}

void MainComponent::loadAudioButtonClicked()
{
	String newGroupName = groupNameTextEditor.getText();
	if (newGroupName.isEmpty()) {

		return;
	}

	Array<String> groupNameList = audioSource.getGroupManager()->getAllGroupNames();
	if (groupNameList.contains(newGroupName)) {

		return;
	}

	FileChooser audioChooser("Please select the directory to load .wav file.",
		File::getSpecialLocation(File::userHomeDirectory));
	if (audioChooser.browseForDirectory())
	{
		audioSource.load(audioChooser.getResult().getFullPathName(), newGroupName);
		audioSource.getGroupManager()->disableByName(newGroupName);

		groupList.clear();
		groupNameList = audioSource.getGroupManager()->getAllGroupNames();
		groupList.addItemList(StringArray(groupNameList), 1);

		groupNameTextEditor.clear();
		groupNameTextEditor.repaint();
	}
}
