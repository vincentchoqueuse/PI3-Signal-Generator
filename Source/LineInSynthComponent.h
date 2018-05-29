/*
  ==============================================================================

    LineInSynthComponent.h
    Created: 25 Mar 2018 9:55:02pm
    Author:  Vincent Choqueuse

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class LineInSynthComponent    : public Component
{
public:
    LineInSynthComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~LineInSynthComponent()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }
    
    //Audio Processing
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {

    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LineInSynthComponent)
};
