#pragma once

#include <JuceHeader.h>

class BM11MLPFAudioProcessor : public juce::AudioProcessor
{
public:
    BM11MLPFAudioProcessor();
    ~BM11MLPFAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;

private:
    #if JUCE_MODULE_AVAILABLE_juce_dsp
    juce::dsp::LadderFilter<float> ladderFilter;
    juce::dsp::Gain<float> inputGain;
    juce::dsp::Compressor<float> envelopeFollower;
    #else
    struct DummyFilter { void prepare(const juce::dsp::ProcessSpec&) {} void process(juce::dsp::ProcessContextReplacing<float>&) {} };
    struct DummyGain { void prepare(const juce::dsp::ProcessSpec&) {} void setGainLinear(float) {} void process(juce::dsp::ProcessContextReplacing<float>&) {} };
    struct DummyCompressor { void prepare(const juce::dsp::ProcessSpec&) {} void setAttack(float) {} void setRelease(float) {} void setThreshold(float) {} void setRatio(float) {} void process(juce::dsp::ProcessContextReplacing<float>&) {} };
    
    DummyFilter ladderFilter;
    DummyGain inputGain;
    DummyCompressor envelopeFollower;
    #endif
    
    float envelopeValue;
    double currentSampleRate;
    
    // Suavizado mínimo solo para parámetros críticos
    juce::SmoothedValue<float> smoothedCutoff; // Solo cutoff necesita suavizado
    float currentDrive, currentResonance, currentMix, currentAmount;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BM11MLPFAudioProcessor)
};