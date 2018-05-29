/*
  ==============================================================================

    FilterComponent.h
    Created: 25 Mar 2018 5:07:14pm
    Author:  Vincent Choqueuse

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FilterComponent    : public Component,
                           public Slider::Listener,
                           public ComboBox::Listener,
                           public Button::Listener
{
public:
    FilterComponent()
    {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
        
    //first row
    enableLabel.setText("Active:", dontSendNotification);
    addAndMakeVisible(enableLabel);
    enableButton.setToggleState(false, dontSendNotification);
    enableButton.addListener(this);
    addAndMakeVisible(enableButton);
        
    //second row
    typeLabel.setText("Type:", dontSendNotification);
    addAndMakeVisible(typeLabel);
    typeCombo.addItem ("Lowpass", 1);
    typeCombo.addItem ("Highpass", 2);
    typeCombo.addItem ("Bandpass", 3);
    typeCombo.setSelectedId(3);
    typeCombo.addListener (this);
    addAndMakeVisible (typeCombo);
    
    //third row
    frequencyLabel.setText("Cutoff:", dontSendNotification);
    addAndMakeVisible(frequencyLabel);
    cutoffSlider.setRange (50, 3000);
    cutoffSlider.setTextValueSuffix ("Hz");
    cutoffSlider.addListener (this);
    cutoffSlider.setValue (1000);
    cutoffSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, cutoffSlider.getTextBoxHeight());
    addAndMakeVisible(cutoffSlider);
    
    //fourth row
    QLabel.setText("Q:", dontSendNotification);
    addAndMakeVisible(QLabel);
    QSlider.setRange (1, 100);
    QSlider.setValue(1);
    QSlider.setTextValueSuffix ("");
    QSlider.addListener (this);
    QSlider.setTextBoxStyle (Slider::TextBoxRight, false, 100, QSlider.getTextBoxHeight());
    addAndMakeVisible(QSlider);
    }

    ~FilterComponent()
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

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("Filter Component", area.removeFromTop(headerFooterHeight),
                    Justification::centred, true);   // draw some placeholder text
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
        enableLabel.setBounds (x_offset,y_offset,100,20);
        enableButton.setBounds (3*x_offset+75,y_offset,ui_width,20);
        
        //second row
        y_offset=y_min+60;
        typeLabel.setBounds (x_offset,y_offset,100,20);
        typeCombo.setBounds (3*x_offset+75,y_offset,ui_width,20);
    
        //third row
        y_offset=y_offset+30;
        frequencyLabel.setBounds(x_offset,y_offset,100,20);
        cutoffSlider.setBounds (3*x_offset+75,y_offset,ui_width, 20);
    
        //fourth row
        y_offset=y_offset+30;
        QLabel.setBounds(x_offset,y_offset,100,20);
        QSlider.setBounds(3*x_offset+75,y_offset,ui_width, 20);

    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if ((slider==&cutoffSlider) || (slider==&QSlider))
        {
        changeFilterCoeff();
        }
    }

    void comboBoxChanged(ComboBox *comboBox) override
    {
        if (comboBox==&typeCombo)
        {
        changeFilterCoeff();
        }
    }
    
    void buttonClicked(Button* button) override
    {
        if (button==&enableButton)
        {
            enable=(int) enableButton.getToggleState();
        }
    }

    void changeFilterCoeff()
    {
        IIRCoefficients coef;
        double Q, cutoff;
    
        Q=QSlider.getValue();
        cutoff=cutoffSlider.getValue();
    
        switch(typeCombo.getSelectedId())
        {
            case 1: coef=IIRCoefficients::makeLowPass(Fe,cutoff,Q); break;
            case 2: coef=IIRCoefficients::makeHighPass(Fe,cutoff,Q); break;
            case 3: coef=IIRCoefficients::makeBandPass(Fe,cutoff,Q); break;        
        }
    
        filterL.setCoefficients(coef);
        filterR.setCoefficients(coef);
    }

   
    
    // Audio processing
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.
    
    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.
    
    Fe=sampleRate;
    changeFilterCoeff();
    }
    
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
    {
        // Your audio-processing code goes here!
        
        // For more details, see the help for AudioProcessor::getNextAudioBlock()
        if (enable==1)
        {
            filterL.processSamples(bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample), bufferToFill.numSamples);
            filterR.processSamples(bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample), bufferToFill.numSamples);
        }
    }

private:
    Label enableLabel, frequencyLabel, typeLabel, QLabel;
    ToggleButton enableButton;
    Slider cutoffSlider, QSlider;
    ComboBox typeCombo;
    IIRFilter filterL,filterR;
    int enable;
    double Fe;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
