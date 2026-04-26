/*
  ==============================================================================

    Source/Effects/HarmonizerEffect.h
    Created: 23 Apr 2026
    Description: RubberBand 기반의 3성부(Dry + 2 Voices) 하모나이저입니다.

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"
#include "../../Dependencies/rubberband/rubberband/RubberBandStretcher.h"

class HarmonizerEffect : public BaseEffect
{
public:
    HarmonizerEffect()
    {
        parameters.set("Voice 1", 4.0f); // +4 semitones (Major 3rd)
        parameters.set("Voice 2", 7.0f); // +7 semitones (Perfect 5th)
        parameters.set("Mix", 0.4f);
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        for (int i = 0; i < 2; ++i)
        {
            stretchers[i] = std::make_unique<RubberBand::RubberBandStretcher>(
                (size_t)sampleRate, 2,
                RubberBand::RubberBandStretcher::OptionProcessRealTime |
                RubberBand::RubberBandStretcher::OptionEngineFaster
            );
        }
        
        tempBuffer.setSize(2, samplesPerBlock);
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        float v1 = (float)parameters.getWithDefault("Voice 1", 4.0f);
        float v2 = (float)parameters.getWithDefault("Voice 2", 7.0f);
        float mix = (float)parameters.getWithDefault("Mix", 0.4f);

        int numSamples = buffer.getNumSamples();
        int numChannels = std::min(2, buffer.getNumChannels());
        const float* inputs[2] = { buffer.getReadPointer(0), numChannels > 1 ? buffer.getReadPointer(1) : buffer.getReadPointer(0) };

        juce::AudioBuffer<float> dryBuffer;
        dryBuffer.makeCopyOf(buffer);
        buffer.applyGain(1.0f - mix); // Lower dry signal

        float scales[2] = { (float)std::pow(2.0, (double)v1 / 12.0), (float)std::pow(2.0, (double)v2 / 12.0) };

        for (int i = 0; i < 2; ++i)
        {
            stretchers[i]->setPitchScale(scales[i]);
            stretchers[i]->process(inputs, (size_t)numSamples, false);
            
            if (stretchers[i]->available() >= numSamples)
            {
                tempBuffer.setSize(numChannels, numSamples, false, false, true);
                float* outputs[2] = { tempBuffer.getWritePointer(0), numChannels > 1 ? tempBuffer.getWritePointer(1) : tempBuffer.getWritePointer(0) };
                stretchers[i]->retrieve(outputs, (size_t)numSamples);
                
                for (int ch = 0; ch < numChannels; ++ch)
                    buffer.addFrom(ch, 0, tempBuffer, ch, 0, numSamples, mix * 0.5f);
            }
        }
    }

    juce::String getName() const override { return "Harmonizer"; }
    juce::String getCategory() const override { return "Pitch"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(juce::Colours::gold.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);
        
        g.setColour(juce::Colours::gold);
        auto center = r.getCentre();
        for (int i = 0; i < 3; ++i)
            g.fillEllipse(center.x - 4, center.y - 12 + (i * 10), 8, 8);

        g.setColour(iconColor.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("HARMONY", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    std::unique_ptr<RubberBand::RubberBandStretcher> stretchers[2];
    juce::AudioBuffer<float> tempBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HarmonizerEffect)
};
