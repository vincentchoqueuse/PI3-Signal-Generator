/*
  ==============================================================================

    AMSynthComponent.h
    Created: 25 Mar 2018 10:33:21pm
    Author:  Vincent Choqueuse

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class AMSynthComponent    : public Component,
                            public Slider::Listener
{
public:
    AMSynthComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        
        
        
        carrier_freqLabel.setText("Carrier (freq):", dontSendNotification);
        carrier_freqSlider.setRange(50, 10000);
        carrier_freqSlider.setTextValueSuffix ("Hz");
        carrier_freqSlider.addListener (this);
        carrier_freqSlider.setValue (1000);
        carrier_freqSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, carrier_freqSlider.getTextBoxHeight());
        addAndMakeVisible(carrier_freqLabel);
        addAndMakeVisible(carrier_freqSlider);
        
        carrier_ampLabel.setText("Carrier (amp):", dontSendNotification);
        carrier_ampSlider.setRange(-1, 1);
        carrier_ampSlider.setTextValueSuffix ("");
        carrier_ampSlider.addListener (this);
        carrier_ampSlider.setValue (1);
        carrier_ampSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, carrier_ampSlider.getTextBoxHeight());
        addAndMakeVisible(carrier_ampLabel);
        addAndMakeVisible(carrier_ampSlider);
        
        modulator_freqLabel.setText("Modulator (freq):", dontSendNotification);
        modulator_freqSlider.setRange(0, 100);
        modulator_freqSlider.setTextValueSuffix ("");
        modulator_freqSlider.addListener (this);
        modulator_freqSlider.setValue (50);
        modulator_freqSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, modulator_freqSlider.getTextBoxHeight());
        addAndMakeVisible(modulator_freqLabel);
        addAndMakeVisible(modulator_freqSlider);
        
        modulator_ampLabel.setText("Modulator (amp):", dontSendNotification);
        modulator_ampSlider.setRange(-1, 1);
        modulator_ampSlider.setTextValueSuffix ("");
        modulator_ampSlider.addListener (this);
        modulator_ampSlider.setValue (0);
        modulator_ampSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, modulator_ampSlider.getTextBoxHeight());
        addAndMakeVisible(modulator_ampLabel);
        addAndMakeVisible(modulator_ampSlider);


    }

    ~AMSynthComponent()
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
        
        int x_offset = 20;
        int y_offset;
        int y_min=36;
        int ui_width;
        
        ui_width=getWidth()-(4*x_offset+75);
        
        //first row
        y_offset=y_min;
        carrier_freqLabel.setBounds (x_offset,y_offset,100,20);
        carrier_freqSlider.setBounds (3*x_offset+75,y_offset,ui_width,20);
        
        //second row
        y_offset=y_min+30;
        carrier_ampLabel.setBounds (x_offset,y_offset,100,20);
        carrier_ampSlider.setBounds (3*x_offset+75,y_offset,ui_width,20);
        
        //third row
        y_offset=y_offset+30;
        modulator_freqLabel.setBounds (x_offset,y_offset,100,20);
        modulator_freqSlider.setBounds (3*x_offset+75,y_offset,ui_width,20);
        
        //fourth row
        y_offset=y_offset+30;
        modulator_ampLabel.setBounds (x_offset,y_offset,100,20);
        modulator_ampSlider.setBounds (3*x_offset+75,y_offset,ui_width,20);

    }
                          
    //GUI management
    void sliderValueChanged (Slider* slider) override
    {

    }
    
    //Audio Processing
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        Fe=sampleRate;
        carrier_currentAngle=0;
        modulator_currentAngle=0;
        
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
    {
        
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        double carrier_angleDelta = (carrier_freqSlider.getValue() * 2.0 * MathConstants<double>::pi)/Fe;
        double modulator_angleDelta = (modulator_freqSlider.getValue() * 2.0 * MathConstants<double>::pi)/Fe;
        
        double carrier_amp=carrier_ampSlider.getValue();
        double modulator_amp=modulator_ampSlider.getValue();
        
        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            auto carrierSample = (float) std::sin (carrier_currentAngle);
            auto modulatorSample = (float) std::sin (modulator_currentAngle);
            auto currentSample =carrier_amp*(1+modulator_amp*modulatorSample)*carrierSample;
            
            leftBuffer[sample]=currentSample;
            rightBuffer[sample]=currentSample;
            
            carrier_currentAngle += carrier_angleDelta;
            modulator_currentAngle += modulator_angleDelta;
        }
        
    }

private:
    
    Label carrier_freqLabel, carrier_ampLabel, modulator_freqLabel, modulator_ampLabel;
    Slider carrier_freqSlider, carrier_ampSlider, modulator_freqSlider, modulator_ampSlider;
    
    double carrier_currentAngle,modulator_currentAngle;
    double Fe;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AMSynthComponent)
};
