/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthComponent.h"
#include "FilterComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, private MidiInputCallback
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void handleIncomingMidiMessage (MidiInput* source,const MidiMessage& message) override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    double Fe;

private:
    //==============================================================================
    // Your private member variables go here...
    
    ScopedPointer<TabbedComponent> tabbedComponent;
    SynthComponent synthComponent;
    FilterComponent filterComponent;
    MidiMessageCollector midiCollector;  


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
