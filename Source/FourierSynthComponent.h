/*
  ==============================================================================

    FourierSynthComponent.h
    Created: 25 Mar 2018 9:54:41pm
    Author:  Vincent Choqueuse

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FourierSynthComponent    : public Component,
                                 public Slider::Listener,
                                 public Button::Listener,
                                public Timer
{
public:
    FourierSynthComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        int indice;
        
        for (indice=0;indice<8;indice++)
        {
            coefficientSlider[indice].setRange (0, 10);
            coefficientSlider[indice].setTextValueSuffix ("");
            coefficientSlider[indice].addListener (this);
            coefficientSlider[indice].setValue (0);
            enableButton[indice].setToggleState(false, dontSendNotification);
            enableButton[indice].addListener(this);
            coefficientSlider[indice].setTextBoxStyle (Slider::NoTextBox, false, 100, coefficientSlider[indice].getTextBoxHeight());
            coefficientLabel[indice].setText ("OSC "+String(indice)+":", dontSendNotification);
            addAndMakeVisible(coefficientLabel[indice]);
            addAndMakeVisible(coefficientSlider[indice]);
            addAndMakeVisible(enableButton[indice]);
        }
        
        enableButton[0].setToggleState(true, dontSendNotification);
        startTimer(40);
    }

    ~FourierSynthComponent()
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
        
        int indice;
        int x_offset = 20;
        int y_min=36;
        int y_offset=y_min;
        int slider_width;
        
        slider_width=getWidth()-(25+50+4*x_offset);

        for (indice=0;indice<8;indice++)
        {
            coefficientLabel[indice].setBounds (x_offset,y_offset,50, 20);
            enableButton[indice].setBounds (2*x_offset+50, y_offset,25, 20);
            coefficientSlider[indice].setBounds (3*x_offset+75,y_offset,slider_width, 20);
            y_offset=y_offset+30;
        }

    }
    
    //Audio Processing
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.
        int indice;
        
        angleDelta = (freq * 2.0 * MathConstants<double>::pi) /sampleRate;
        for (indice=0;indice<8;indice++)
        {
            amp_tab[indice]=0;
            enable_tab[indice]=0;
        }
        enable_tab[0]=1;
    }
    
    void handleIncomingMidiMessage (MidiInput* midi_input,const MidiMessage& message)
    {
        int value =message.getControllerValue();
        int CC= message.getControllerNumber();
        
        if ((CC >= 1) && (CC <= 8))
        {
            amp_tab[CC-1]=10*value/127.;
        }
        if ((CC >= 11) && (CC <= 18))
        {
            enable_tab[CC-11]=(value>64);
        }
        
    }
    
    void timerCallback() override
    {
     
        for (auto i = 0; i < 8; ++i)
        {
            coefficientSlider[i].setValue(amp_tab[i], dontSendNotification);
            enableButton[i].setToggleState(enable_tab[i], dontSendNotification);
        }
     
    }
     
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        // Your audio-processing code goes here!
        int indice;
        
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        

        
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            leftBuffer[sample]=0;
            rightBuffer[sample]=0;
            
            for (indice=0;indice<8;indice++)
            {
                if (enable_tab[indice]==1)
                {
                    auto currentSample = (float) std::sin ((indice+1)*currentAngle);
                    
                    currentSample=amp*currentSample * amp_tab[indice];
                    leftBuffer[sample]  += currentSample;
                    rightBuffer[sample] += currentSample;
                }
            }
            currentAngle += angleDelta;
        }
    }
    
    //GUI interaction
    void sliderValueChanged (Slider* slider) override
    {
        int indice;
        
        for (indice=0;indice<8;indice++)
        {
            if (slider == &coefficientSlider[indice])
            {
                amp_tab[indice]=coefficientSlider[indice].getValue();
            }
        }
    }
    
    void buttonClicked(Button* button) override
    {
        int indice;
        
        for (indice=0;indice<8;indice++)
        {
            if (button == &enableButton[indice])
            {
                enable_tab[indice]=(int) enableButton[indice].getToggleState();
            }
        }
    }

private:
    
    Slider coefficientSlider[8];
    ToggleButton enableButton[8];
    Label coefficientLabel[8];
    
    double angleDelta=0.0, currentAngle=0.0;
    double amp_tab[8];
    bool enable_tab[8];
    float amp=0.05;
    float freq=100;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FourierSynthComponent)
};
