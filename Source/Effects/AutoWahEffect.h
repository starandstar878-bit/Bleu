/*
  ==============================================================================

    Source/Effects/AutoWahEffect.h
    Created: 23 Apr 2026
    Description: SignalSmith 필터 및 엔벨로프 로직 기반의 오토와우입니다.

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"
#include "../../Dependencies/dsp/filters.h"
#include "../../Dependencies/dsp/envelopes.h"

class AutoWahEffect : public BaseEffect
{
public:
    AutoWahEffect()
    {
        parameters.set("Sensitivity", 0.5f);
        parameters.set("Range", 0.5f);
        parameters.set("Resonance", 0.4f);
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        currentSampleRate = sampleRate;
        for (int i = 0; i < 2; ++i)
        {
            filters[i].reset();
            envFilters[i] = std::make_unique<signalsmith::envelopes::BoxFilter<float>>(sampleRate * 0.1);
            envFilters[i]->set(sampleRate * 0.05);  // 50ms smooth
        }
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        float sens = (float)parameters.getWithDefault("Sensitivity", 0.5f);
        float range = (float)parameters.getWithDefault("Range", 0.5f);
        float res = (float)parameters.getWithDefault("Resonance", 0.4f);

        int numSamples = buffer.getNumSamples();
        int numChannels = std::min(2, buffer.getNumChannels());

        for (int s = 0; s < numSamples; ++s)
        {
            for (int ch = 0; ch < numChannels; ++ch)
            {
                if (!envFilters[ch]) continue;

                auto* channelData = buffer.getWritePointer(ch);
                float input = channelData[s];

                // 1. Envelope Follower
                float absInput = std::abs(input);
                float env = (*envFilters[ch])(absInput);
                
                // 2. Map Envelope to Frequency
                float baseFreq = 400.0f;
                float targetFreq = baseFreq + (env * sens * range * 4000.0f);
                targetFreq = juce::jlimit(100.0f, 10000.0f, targetFreq);
                
                // 3. Apply Bandpass Filter
                float scaledFreq = targetFreq / (float)currentSampleRate;
                float q = 1.0f + res * 9.0f; // Q from 1 to 10
                
                filters[ch].bandpassQ(scaledFreq, q);
                channelData[s] = filters[ch](input);
            }
        }
    }

    juce::String getName() const override { return "Auto-Wah"; }
    juce::String getCategory() const override { return "Filter"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(juce::Colours::limegreen.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);
        
        auto center = r.getCentre();
        g.setColour(juce::Colours::limegreen);
        
        // Draw a circle that looks like a mouth or filter sweep
        g.drawEllipse(center.x - 10, center.y - 10, 20, 20, 2.0f);
        g.fillEllipse(center.x - 4, center.y - 4, 8, 8);

        g.setColour(iconColor.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("AUTO-WAH", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    signalsmith::filters::BiquadStatic<float> filters[2];
    std::unique_ptr<signalsmith::envelopes::BoxFilter<float>> envFilters[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutoWahEffect)
};
