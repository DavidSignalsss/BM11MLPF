#include "BM11MLPFAudioProcessorEditor.h"

BM11MLPFAudioProcessorEditor::BM11MLPFAudioProcessorEditor (BM11MLPFAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    titleLabel.setText("BM-11M LOW PASS FILTER", juce::dontSendNotification);
    titleLabel.setFont(juce::FontOptions(20.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xffffd700));
    addAndMakeVisible(titleLabel);

    creditLabel.setText("by David Signals", juce::dontSendNotification);
    creditLabel.setFont(juce::FontOptions(12.0f, juce::Font::plain));
    creditLabel.setJustificationType(juce::Justification::bottomRight);
    creditLabel.setColour(juce::Label::textColourId, juce::Colour(0xffaaaaaa));
    addAndMakeVisible(creditLabel);

    auto setupSlider = [this](juce::Slider& slider, const juce::String& name) {
        slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 18);
        slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffd2691e));
        slider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff8b4513));
        slider.setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xffffd700));
        slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff2f1b14));
        addAndMakeVisible(slider);
    };

    setupSlider(driveSlider, "Drive");
    setupSlider(cutoffSlider, "Cutoff");
    setupSlider(resonanceSlider, "Resonance");
    setupSlider(amountSlider, "Amount");
    setupSlider(mixSlider, "Mix");

    auto setupLabel = [this](juce::Label& label, const juce::String& text) {
        label.setText(text, juce::dontSendNotification);
        label.setFont(juce::FontOptions(14.0f, juce::Font::bold));
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colour(0xffffd700));
        addAndMakeVisible(label);
    };

    setupLabel(driveLabel, "DRIVE");
    setupLabel(cutoffLabel, "CUTOFF");
    setupLabel(resonanceLabel, "RESONANCE");
    setupLabel(amountLabel, "AMOUNT");
    setupLabel(mixLabel, "MIX");
    setupLabel(filterModeLabel, "FILTER");
    setupLabel(envelopeLabel, "ENVELOPE");
    setupLabel(envModeLabel, "ENV");

    auto setupCombo = [this](juce::ComboBox& combo) {
        combo.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2f1b14));
        combo.setColour(juce::ComboBox::textColourId, juce::Colour(0xffffd700));
        combo.setColour(juce::ComboBox::arrowColourId, juce::Colour(0xffffd700));
        combo.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(combo);
    };

    filterModeCombo.addItem("2-POLE", 1);
    filterModeCombo.addItem("4-POLE", 2);
    setupCombo(filterModeCombo);

    envelopeCombo.addItem("SMOOTH", 1);
    envelopeCombo.addItem("FAST", 2);
    setupCombo(envelopeCombo);

    envModeCombo.addItem("BYPASS", 1);
    envModeCombo.addItem("LEVEL", 2);
    setupCombo(envModeCombo);

    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "drive", driveSlider);
    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "cutoff", cutoffSlider);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "resonance", resonanceSlider);
    amountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "amount", amountSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "mix", mixSlider);
    
    filterModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.parameters, "filterMode", filterModeCombo);
    envelopeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.parameters, "envelope", envelopeCombo);
    envModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.parameters, "envMode", envModeCombo);

    setSize (650, 450);
}

BM11MLPFAudioProcessorEditor::~BM11MLPFAudioProcessorEditor()
{
}

void BM11MLPFAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::ColourGradient woodGradient(
        juce::Colour(0xff8b4513), 0, 0,
        juce::Colour(0xffd2691e), getWidth(), getHeight(), false
    );
    g.setGradientFill(woodGradient);
    g.fillAll();

    g.setColour(juce::Colour(0xff2f1b14));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 12.0f);

    auto innerArea = getLocalBounds().reduced(8);
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(0xff5d4037), 0, 0,
        juce::Colour(0xff3e2723), innerArea.getWidth(), innerArea.getHeight(), false
    ));
    g.fillRoundedRectangle(innerArea.toFloat(), 8.0f);

    g.setColour(juce::Colour(0x15ffffff));
    for (int i = 0; i < getHeight(); i += 4)
    {
        g.drawLine(0, i, getWidth(), i, 0.5f);
    }

    auto titleArea = getLocalBounds().removeFromTop(70).reduced(20, 10);
    g.setColour(juce::Colour(0xaa000000));
    g.fillRoundedRectangle(titleArea.toFloat(), 6.0f);
    
    g.setColour(juce::Colour(0xffffd700));
    g.drawRoundedRectangle(titleArea.toFloat(), 6.0f, 2.0f);

    auto controlsArea = getLocalBounds().withTrimmedTop(70).reduced(15);
    g.setColour(juce::Colour(0xaa1a1a1a));
    g.fillRoundedRectangle(controlsArea.toFloat(), 8.0f);
    
    g.setColour(juce::Colour(0x44ffffff));
    g.drawRoundedRectangle(controlsArea.toFloat(), 8.0f, 1.0f);
}

void BM11MLPFAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(15);
    
    auto titleArea = area.removeFromTop(70);
    titleLabel.setBounds(titleArea);
    titleLabel.setJustificationType(juce::Justification::centred);
    
    auto creditArea = area.removeFromBottom(25);
    creditLabel.setBounds(creditArea.reduced(10, 5));

    area.removeFromTop(10);

    auto rowHeight = area.getHeight() / 2;
    auto topRow = area.removeFromTop(rowHeight).reduced(15);
    auto bottomRow = area.reduced(15);

    auto sectionWidth = topRow.getWidth() / 4;

    auto driveSection = topRow.removeFromLeft(sectionWidth);
    driveLabel.setBounds(driveSection.removeFromTop(25));
    driveSlider.setBounds(driveSection.reduced(5));

    auto cutoffSection = topRow.removeFromLeft(sectionWidth);
    cutoffLabel.setBounds(cutoffSection.removeFromTop(25));
    cutoffSlider.setBounds(cutoffSection.reduced(5));

    auto filterSection = topRow.removeFromLeft(sectionWidth);
    filterModeLabel.setBounds(filterSection.removeFromTop(25));
    filterModeCombo.setBounds(filterSection.withSizeKeepingCentre(120, 28).reduced(5));

    auto resonanceSection = topRow.removeFromLeft(sectionWidth);
    resonanceLabel.setBounds(resonanceSection.removeFromTop(25));
    resonanceSlider.setBounds(resonanceSection.reduced(5));

    auto amountSection = bottomRow.removeFromLeft(sectionWidth);
    amountLabel.setBounds(amountSection.removeFromTop(25));
    amountSlider.setBounds(amountSection.reduced(5));

    auto envelopeSection = bottomRow.removeFromLeft(sectionWidth);
    envelopeLabel.setBounds(envelopeSection.removeFromTop(25));
    envelopeCombo.setBounds(envelopeSection.withSizeKeepingCentre(120, 28).reduced(5));

    auto mixSection = bottomRow.removeFromLeft(sectionWidth);
    mixLabel.setBounds(mixSection.removeFromTop(25));
    mixSlider.setBounds(mixSection.reduced(5));

    auto envModeSection = bottomRow.removeFromLeft(sectionWidth);
    envModeLabel.setBounds(envModeSection.removeFromTop(25));
    envModeCombo.setBounds(envModeSection.withSizeKeepingCentre(100, 28).reduced(5));
}