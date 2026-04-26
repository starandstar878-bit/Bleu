/*
  ==============================================================================

    Source/Effects/PowerAmpEffect.h
    Created: 23 Apr 2026
    Description: 진공관 파워 앰프의 특성(새츄레이션, 레조넌스, 프레젠스)을 모사한 모듈입니다.

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"

class PowerAmpEffect : public BaseEffect
{
public:
    PowerAmpEffect()
    {
        parameters.set("Saturation", 0.3f);
        parameters.set("Resonance", 0.5f);
        parameters.set("Presence", 0.5f);
        parameters.set("Master", 0.0f); // dB
        oversamplingEnabled = true;
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        currentSampleRate = sampleRate;
        currentBlockSize = samplesPerBlock;

        juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
        
        lowShelf.prepare(spec);
        highShelf.prepare(spec);
        
        updateFilters();
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        updateFilters();

        float sat = (float)parameters.getWithDefault("Saturation", 0.3f);
        float master = juce::Decibels::decibelsToGain((float)parameters.getWithDefault("Master", 0.0f));

        // 1. Resonance (Low Shelf)
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        lowShelf.process(context);

        // 2. Presence (High Shelf)
        highShelf.process(context);

        // 3. Saturation (Soft clipping)
        float drive = 1.0f + sat * 4.0f;
        auto numChannels = buffer.getNumChannels();
        auto numSamples = buffer.getNumSamples();

        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* channelData = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s)
            {
                float x = channelData[s] * drive;
                channelData[s] = std::tanh(x) / drive;
            }
        }

        // 4. Master Gain
        buffer.applyGain(master);
    }

    juce::String getName() const override { return "Power"; }
    juce::String getCategory() const override { return "Dynamics"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(juce::Colours::red.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);

        auto center = r.getCentre();
        
        // Power amp symbol: Three vertical bars (Power tubes)
        g.setColour(juce::Colours::red.withAlpha(0.7f));
        for (int i = -1; i <= 1; ++i)
        {
            g.fillRect(center.x + i * 10.0f - 3.0f, center.y - 10.0f, 6.0f, 20.0f);
        }

        g.setColour(iconColor.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("POWER", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    void updateFilters()
    {
        float res = (float)parameters.getWithDefault("Resonance", 0.5f);
        float pres = (float)parameters.getWithDefault("Presence", 0.5f);

        float resGain = juce::jmap(res, 0.0f, 1.0f, 1.0f, 4.0f);   // 0 to 12dB
        float presGain = juce::jmap(pres, 0.0f, 1.0f, 1.0f, 4.0f); // 0 to 12dB

        *lowShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate, 150.0f, 0.707f, resGain);
        *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(currentSampleRate, 3000.0f, 0.707f, presGain);
    }

    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowShelf, highShelf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerAmpEffect)
};
