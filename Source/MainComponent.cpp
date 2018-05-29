/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "SynthComponent.h"
#include "FilterComponent.h"

//==============================================================================
MainComponent::MainComponent():
    synthComponent(),filterComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    Colour backgroundColour=getLookAndFeel().findColour (ResizableWindow::backgroundColourId);
    
    setSize (800, 600);
    addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabbedComponent->setTabBarDepth (31);
        
    tabbedComponent->addTab (TRANS("Generator"), backgroundColour, &synthComponent, false);
    tabbedComponent->addTab (TRANS("Filter"),backgroundColour, &filterComponent, false);
    tabbedComponent->addTab (TRANS("Audio IO"),backgroundColour,new AudioDeviceSelectorComponent(deviceManager, 0, 256, 0, 256, true,false, false, false),true);
    
    
    deviceManager.addMidiInputCallback ({}, this);
    tabbedComponent->setCurrentTabIndex (0);
    tabbedComponent->setBounds (0,0,800,600);

    // specify the number of input and output channels that we want to open
    setAudioChannels (2, 2);

    for (auto& midi : MidiInput::getDevices()) {
        deviceManager.setMidiInputEnabled (midi, true);
    }

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
    
    Fe=sampleRate;
    
    synthComponent.prepareToPlay(samplesPerBlockExpected,sampleRate);
    filterComponent.prepareToPlay(samplesPerBlockExpected,sampleRate);
}


void MainComponent::handleIncomingMidiMessage (MidiInput* midi_input,const MidiMessage& message)
{
    
    synthComponent.handleIncomingMidiMessage(midi_input,message);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    
    
    synthComponent.getNextAudioBlock(bufferToFill);
    filterComponent.getNextAudioBlock(bufferToFill);
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
    g.setColour (Colours::lightgrey);
    g.setFont (14.0f);
    g.drawText ("IUT de Brest, Dept GEII",0,7,775,20,Justification::right, true);
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
