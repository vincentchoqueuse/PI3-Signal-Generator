/*
  ==============================================================================

    SynthComponent.h
    Created: 25 Mar 2018 5:06:59pm
    Author:  Vincent Choqueuse

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FourierSynthComponent.h"
#include "LineInSynthComponent.h"
#include "AMSynthComponent.h"

//==============================================================================
/*
*/
class SynthComponent    : public Component,
                          public ComboBox::Listener
{
public:
    SynthComponent():
        fourierSynth(),lineinSynth(),amSynth()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        

        //first row
        typeLabel.setText("Type:", dontSendNotification);
        addAndMakeVisible(typeLabel);
        typeCombo.addItem ("Fourier Synthesizer", 1);
        typeCombo.addItem ("Line In", 2);
        typeCombo.addItem ("AM Synthesizer", 3);
        typeCombo.setSelectedId(1);
        typeCombo.addListener(this);
        addAndMakeVisible(typeCombo);
        addAndMakeVisible(fourierSynth);
        addChildComponent(lineinSynth);
        addChildComponent(amSynth);
        typeGenerator=1;
        
    }

    ~SynthComponent()
    {

    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        auto area = getLocalBounds();
        auto headerFooterHeight = 36;
        
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);
        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("Generator Component", area.removeFromTop(headerFooterHeight),Justification::centred, true);
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        
        int x_offset = 20;
        int y_min=36;
        int y_offset=y_min;
        int slider_width;
    
        slider_width=getWidth()-(25+50+4*x_offset);
    
        typeLabel.setBounds (x_offset,y_offset,50, 20);
        typeCombo.setBounds (3*x_offset+75, y_offset,slider_width, 20);
        y_offset=y_offset+30;
        
        fourierSynth.setBounds(0,y_offset,getWidth(),getHeight()-y_offset);
        lineinSynth.setBounds(0,y_offset,getWidth(),getHeight()-y_offset);
        amSynth.setBounds(0,y_offset,getWidth(),getHeight()-y_offset);
        
        switch(typeGenerator)
        {
            case 1: {
                    fourierSynth.setVisible(true);
                    lineinSynth.setVisible(false);
                    amSynth.setVisible(false);
                    }break;
            case 2: {
                    fourierSynth.setVisible(false);
                    lineinSynth.setVisible(true);
                    amSynth.setVisible(false);
                    }break;
            case 3: {
                    fourierSynth.setVisible(false);
                    lineinSynth.setVisible(false);
                    amSynth.setVisible(true);
                    }break;
        }


    }
    
    //GUI interaction

    
    void comboBoxChanged(ComboBox *comboBox) override
    {
        if (comboBox==&typeCombo)
        {
            typeGenerator=typeCombo.getSelectedId();
            resized();
        }

    }
    
    //Audio Processing
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.
        
        fourierSynth.prepareToPlay(samplesPerBlockExpected,sampleRate);
        lineinSynth.prepareToPlay(samplesPerBlockExpected,sampleRate);
        amSynth.prepareToPlay(samplesPerBlockExpected,sampleRate);
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        // Your audio-processing code goes here!
        
        switch(typeGenerator)
        {
            case 1: {
                    fourierSynth.getNextAudioBlock(bufferToFill);
                    } break;
            case 2: {
                    lineinSynth.getNextAudioBlock(bufferToFill);
                    } break;
            case 3: {
                    amSynth.getNextAudioBlock(bufferToFill);
                    } break;
        }
        
    }
    
    void handleIncomingMidiMessage (MidiInput* midi_input,const MidiMessage& message)
    {
        fourierSynth.handleIncomingMidiMessage(midi_input,message);
        
    }

private:

    Label typeLabel;
    ComboBox typeCombo;
    FourierSynthComponent fourierSynth;
    int typeGenerator;
    LineInSynthComponent lineinSynth;
    AMSynthComponent amSynth;

    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthComponent)
};
