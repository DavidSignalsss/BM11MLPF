#include "BM11MLPFAudioProcessor.h"
#include "BM11MLPFAudioProcessorEditor.h"

BM11MLPFAudioProcessor::BM11MLPFAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       parameters(*this, nullptr, "PARAMETERS", {
           std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 0.0f, 10.0f, 4.0f),
           std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff", 
                                                       juce::NormalisableRange<float>(250.0f, 12000.0f, 0.0f, 0.3f), 
                                                       1000.0f),
           std::make_unique<juce::AudioParameterChoice>("filterMode", "Filter Mode", 
                                                        juce::StringArray{"2-Pole", "4-Pole"}, 1),
           std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", 0.0f, 10.0f, 0.0f),
           std::make_unique<juce::AudioParameterFloat>("amount", "Amount", 0.0f, 10.0f, 0.0f),
           std::make_unique<juce::AudioParameterChoice>("envelope", "Envelope", 
                                                        juce::StringArray{"Smooth", "Fast"}, 0),
           std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 10.0f, 10.0f),
           std::make_unique<juce::AudioParameterChoice>("envMode", "Env Mode", 
                                                        juce::StringArray{"Bypass", "Level"}, 0)
       })
{
    smoothedCutoff.reset(1000.0f);
    currentDrive = 4.0f;
    currentResonance = 0.0f;
    currentMix = 10.0f;
    currentAmount = 0.0f;
    
    envelopeValue = 0.0f;
    currentSampleRate = 44100.0;
}

BM11MLPFAudioProcessor::~BM11MLPFAudioProcessor()
{
}

const juce::String BM11MLPFAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BM11MLPFAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BM11MLPFAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BM11MLPFAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BM11MLPFAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BM11MLPFAudioProcessor::getNumPrograms()
{
    return 1;
}

int BM11MLPFAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BM11MLPFAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BM11MLPFAudioProcessor::getProgramName (int index)
{
    return {};
}

void BM11MLPFAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void BM11MLPFAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    #if JUCE_MODULE_AVAILABLE_juce_dsp
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    ladderFilter.prepare(spec);
    inputGain.prepare(spec);
    
    envelopeFollower.prepare(spec);
    envelopeFollower.setAttack(1.0f);    // Muy rápido
    envelopeFollower.setRelease(10.0f);  // Muy rápido
    envelopeFollower.setThreshold(0.0f);
    envelopeFollower.setRatio(1000.0f);
    #endif
    
    // Suavizado MUY rápido solo para cutoff (2ms)
    smoothedCutoff.reset(sampleRate, 0.002);
    
    envelopeValue = 0.0f;
}

void BM11MLPFAudioProcessor::releaseResources()
{
}

bool BM11MLPFAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void BM11MLPFAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    #if JUCE_MODULE_AVAILABLE_juce_dsp
    // Obtener parámetros actuales - mayoría SIN suavizado
    float drive = parameters.getRawParameterValue("drive")->load();
    float cutoffTarget = parameters.getRawParameterValue("cutoff")->load();
    float filterMode = parameters.getRawParameterValue("filterMode")->load();
    float resonance = parameters.getRawParameterValue("resonance")->load();
    float amount = parameters.getRawParameterValue("amount")->load();
    float envelopeMode = parameters.getRawParameterValue("envelope")->load();
    float mix = parameters.getRawParameterValue("mix")->load();
    float envMode = parameters.getRawParameterValue("envMode")->load();
    
    // Actualizar suavizado SOLO para cutoff (evita clicks en frecuencia)
    smoothedCutoff.setTargetValue(cutoffTarget);
    float cutoff = smoothedCutoff.getNextValue();
    
    // PARÁMETROS INSTANTÁNEOS (sin suavizado)
    float driveGain = juce::Decibels::decibelsToGain(drive * 2.0f);
    inputGain.setGainLinear(driveGain);
    
    ladderFilter.setMode(filterMode > 0.5f ? 
                        juce::dsp::LadderFilterMode::LPF24 : 
                        juce::dsp::LadderFilterMode::LPF12);
    
    ladderFilter.setResonance((resonance / 10.0f) * 0.7f);
    
    // Envolvente optimizada para velocidad
    float currentEnvelope = 0.0f;
    if (totalNumInputChannels > 0) {
        auto* channelData = buffer.getReadPointer(0);
        // Revisar cada 4 muestras para mayor velocidad
        int step = 4;
        for (int i = 0; i < buffer.getNumSamples(); i += step)
        {
            float absSample = std::abs(channelData[i]);
            if (absSample > currentEnvelope)
                currentEnvelope = absSample;
        }
        // Suavizado muy ligero para estabilidad
        envelopeValue = 0.85f * envelopeValue + 0.15f * currentEnvelope;
    }
    
    // Aplicar cutoff (con suavizado mínimo) y modulación
    if (envMode > 0.5f) {
        float envelopeMod = envelopeValue * (amount / 10.0f);
        float modulatedCutoff = cutoff * (1.0f + envelopeMod * 5.0f);
        ladderFilter.setCutoffFrequencyHz(juce::jmin(modulatedCutoff, 20000.0f));
    } else {
        ladderFilter.setCutoffFrequencyHz(cutoff);
    }
    
    // Envelope follower rápido
    if (envelopeMode > 0.5f) {
        envelopeFollower.setAttack(1.0f);
        envelopeFollower.setRelease(10.0f);
    } else {
        envelopeFollower.setAttack(5.0f);
        envelopeFollower.setRelease(50.0f);
    }
    
    // Procesamiento DSP
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    
    inputGain.process(context);
    envelopeFollower.process(context);
    ladderFilter.process(context);
    
    // Mix instantáneo (sin suavizado)
    float wetMixValue = mix / 10.0f;
    if (wetMixValue < 0.999f) {
        for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
            auto* dry = buffer.getArrayOfReadPointers()[channel];
            auto* wet = buffer.getArrayOfWritePointers()[channel];
            
            // Procesar en bloques para mejor rendimiento
            int samples = buffer.getNumSamples();
            for (int sample = 0; sample < samples; ++sample) {
                float drySample = dry[sample];
                float wetSample = wet[sample];
                wet[sample] = drySample * (1.0f - wetMixValue) + wetSample * wetMixValue;
            }
        }
    }
    #endif
}

juce::AudioProcessorEditor* BM11MLPFAudioProcessor::createEditor()
{
    return new BM11MLPFAudioProcessorEditor (*this);
}

bool BM11MLPFAudioProcessor::hasEditor() const
{
    return true;
}

void BM11MLPFAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void BM11MLPFAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BM11MLPFAudioProcessor();
}