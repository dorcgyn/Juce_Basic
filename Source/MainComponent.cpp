/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "Config.h"

//==============================================================================
MainComponent::MainComponent() : keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
, audioSource(keyboardState, keyboardComponent)
{
	// Make sure you set the size of the component after
	// you add any child components.
	setSize(800, 600);

	addAndMakeVisible(&loadAudioButton);
	loadAudioButton.setButtonText("Load Audio Group");
	loadAudioButton.onClick = [this] { loadAudioButtonClicked(); };

	addAndMakeVisible(&saveButton);
	saveButton.setButtonText("Save");
	saveButton.onClick = [this] { saveButtonClicked(); };

	addAndMakeVisible(&loadButton);
	loadButton.setButtonText("Load");
	loadButton.onClick = [this] { loadButtonClicked(); };

	addAndMakeVisible(&groupNameTextEditor);
	addAndMakeVisible(&groupNameLabel);
	groupNameLabel.setText("New Group Name", dontSendNotification);
	groupNameLabel.attachToComponent(&groupNameTextEditor, true);

	// VERY VERY important
	setAudioChannels(0, 2);

	auto list = MidiInput::getDevices();
	auto newInput = list[0];
	if (!deviceManager.isMidiInputEnabled(newInput))
		deviceManager.setMidiInputEnabled(newInput, true);
	deviceManager.addMidiInputCallback(newInput, audioSource.getMidiCollector());

	// keyboard color
	addAndMakeVisible(keyboardComponent);
	keyboardComponent.setGroupManager(audioSource.getGroupManager());

	// pick group combobox
	addAndMakeVisible(groupListLabel);
	groupListLabel.setText("Active Group:", dontSendNotification);
	groupListLabel.attachToComponent(&groupList, true);

	addAndMakeVisible(groupList);
	groupList.setTextWhenNoChoicesAvailable("No Group Available");
	reload();

	groupList.onChange = [this] {
		int selectedIndex = groupList.getSelectedItemIndex();
		if (selectedIndex < 0) return;
		String itemText = groupList.getItemText(selectedIndex);
		audioSource.getGroupManager()->disableAll();
		audioSource.getGroupManager()->enableByName(itemText);
		keyboardComponent.repaint();

		// setup eqSlider value
		Group* group = audioSource.getGroupManager()->getGroup(itemText);
		EnhanceEq* eq = dynamic_cast<EnhanceEq*>(group->getEqContainer()->getEq());
		eqSlider.setValue(eq->getMultiplex(), dontSendNotification);
	};

	// EQ Slider value update
	addAndMakeVisible(&eqSlider);
	addAndMakeVisible(&eqLabel);
	eqLabel.setText("Eq value", dontSendNotification);
	eqLabel.attachToComponent(&eqSlider, true);

	eqSlider.onValueChange = [this] {
		int selectedIndex = groupList.getSelectedItemIndex();
		if (selectedIndex < 0) return;
		String itemText = groupList.getItemText(selectedIndex);
		Group* group = audioSource.getGroupManager()->getGroup(itemText);
		EqContainer* eqContainer = group->getEqContainer();
		EnhanceEq* eq = dynamic_cast<EnhanceEq*>(eqContainer->getEq());

		double value = eqSlider.getValue();
		eq->setMultiplex(value);
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
	groupNameTextEditor.setBounds(200, 10, getWidth() - 210, 20);
	loadAudioButton.setBounds(10, 50, getWidth() - 20, 20);
	saveButton.setBounds(10, 80, getWidth() - 20, 20);
	loadButton.setBounds(10, 120, getWidth() - 20, 20);

	groupList.setBounds(200, 500, getWidth() - 210, 20);

	eqSlider.setBounds(10, 200, getWidth() - 20, 20);

	auto area = getLocalBounds();
	keyboardComponent.setBounds(area.removeFromBottom(80).reduced(8));
}

void MainComponent::loadAudioButtonClicked()
{
	String newGroupName = groupNameTextEditor.getText();
	if (newGroupName.isEmpty()) {
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, String("Error In Load Group"), String("Group Name should't be empty!"));
		return;
	}

	Array<String> groupNameList = audioSource.getGroupManager()->getAllGroupNames();
	if (groupNameList.contains(newGroupName)) {
		AlertWindow::showMessageBox(AlertWindow::WarningIcon, String("Error In Load Group"), String("Group Name already exists!"));
		return;
	}

	FileChooser audioChooser("Please select the directory to load .wav file.",
		File::getSpecialLocation(File::userHomeDirectory));
	if (audioChooser.browseForDirectory())
	{
		audioSource.loadDir(audioChooser.getResult().getFullPathName(), newGroupName);
		audioSource.getGroupManager()->disableByName(newGroupName);

		reload();

		AlertWindow::showMessageBox(AlertWindow::InfoIcon, String("New Group Created"), String("Create Group " + newGroupName + " successfully!"));
	}
}

void MainComponent::saveButtonClicked()
{
	FileChooser configFileChooser("Pick file to save", File(), "*.config");
	bool success = configFileChooser.browseForFileToSave(true);

	if (success) {
		Config config;
		File configFile = configFileChooser.getResult();
		config.serialize(audioSource, configFile);
	}
	else {
		AlertWindow::showMessageBox(AlertWindow::InfoIcon, String("Save Fail"), String("Fail to select file to save!"));
	}
}

void MainComponent::loadButtonClicked()
{
	FileChooser configFileChooser("Pick file to load", File(), "*.config");
	bool success = configFileChooser.browseForFileToOpen();

	if (success) {
		Config config;
		File configFile = configFileChooser.getResult();
		config.deserialize(audioSource, configFile);

		reload();
	}
	else {
		AlertWindow::showMessageBox(AlertWindow::InfoIcon, String("Save Load"), String("Fail to load selected file!"));
	}
}

void MainComponent::reload()
{
	groupList.clear();
	Array<String> groupNameList = audioSource.getGroupManager()->getAllGroupNames();
	groupList.addItemList(StringArray(groupNameList), 1);

	groupNameTextEditor.clear();
	groupNameTextEditor.repaint();
}