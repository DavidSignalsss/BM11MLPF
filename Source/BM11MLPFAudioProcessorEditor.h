#pragma once

#include <JuceHeader.h>
#include "BM11MLPFAudioProcessor.h"

class BM11MLPFAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BM11MLPFAudioProcessorEditor (BM11MLPFAudioProcessor&);
    ~BM11MLPFAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    BM11MLPFAudioProcessor& audioProcessor;

    juce::Slider driveSlider, cutoffSlider, resonanceSlider, amountSlider, mixSlider;
    juce::ComboBox filterModeCombo, envelopeCombo, envModeCombo;
    
    juce::Label driveLabel, cutoffLabel, resonanceLabel, amountLabel, mixLabel;
    juce::Label filterModeLabel, envelopeLabel, envModeLabel;
    juce::Label titleLabel;
    juce::Label creditLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> amountAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterModeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envelopeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envModeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BM11MLPFAudioProcessorEditor)
};