/*
  ==============================================================================

    Source/Effects/WhammyEffect.h
    Created: 23 Apr 2026
    Description: RubberBand 엔진 기반의 실시간 피치 시프터(Whammy)입니다.

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"
#include "../../Dependencies/rubberband/rubberband/RubberBandStretcher.h"
#include <memory>

class WhammyEffect : public BaseEffect
{
public:
    WhammyEffect()
    {
        parameters.set("Pitch", 0.0f); // -12 to +12 semitones or specific range
        parameters.set("Mix", 1.0f);
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        currentSampleRate = sampleRate;
        
        // Real-time mode, R2 (Faster) engine for low latency
        stretcher = std::make_unique<RubberBand::RubberBandStretcher>(
            (size_t)sampleRate, 
            2, 
            RubberBand::RubberBandStretcher::OptionProcessRealTime | 
            RubberBand::RubberBandStretcher::OptionPitchHighConsistency |
            RubberBand::RubberBandStretcher::OptionEngineFaster
        );
        
        stretcher->setMaxProcessSize(samplesPerBlock);
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        if (!stretcher) return;

        float pitchSemitones = (float)parameters.getWithDefault("Pitch", 0.0f);
        float mix = (float)parameters.getWithDefault("Mix", 1.0f);
        
        // Convert semitones to pitch scale
        double pitchScale = std::pow(2.0, (double)pitchSemitones / 12.0);
        stretcher->setPitchScale(pitchScale);

        int numSamples = buffer.getNumSamples();
        int numChannels = std::min(2, buffer.getNumChannels());

        const float* inputs[2] = { buffer.getReadPointer(0), numChannels > 1 ? buffer.getReadPointer(1) : buffer.getReadPointer(0) };
        
        // RubberBand real-time processing
        stretcher->process(inputs, (size_t)numSamples, false);
        
        int available = stretcher->available();
        if (available >= numSamples)
        {
            float* outputs[2] = { buffer.getWritePointer(0), numChannels > 1 ? buffer.getWritePointer(1) : buffer.getWritePointer(0) };
            
            // Temporary buffer for dry/wet mix
            juce::AudioBuffer<float> dryBuffer;
            dryBuffer.makeCopyOf(buffer);
            
            stretcher->retrieve(outputs, (size_t)numSamples);
            
            // Mix dry/wet
            if (mix < 1.0f)
            {
                for (int ch = 0; ch < numChannels; ++ch)
                    buffer.addFrom(ch, 0, dryBuffer, ch, 0, numSamples, 1.0f - mix);
            }
        }
    }

    juce::String getName() const override { return "Whammy"; }
    juce::String getCategory() const override { return "Pitch"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(juce::Colours::red.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);

        auto center = r.getCentre();
        g.setColour(juce::Colours::red);
        
        // Draw a pedal-like shape
        juce::Path p;
        p.addRoundedRectangle(center.x - 10, center.y - 15, 20, 30, 2.0f);
        g.fillPath(p);
        
        g.setColour(iconColor.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("WHAMMY", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    std::unique_ptr<RubberBand::RubberBandStretcher> stretcher;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WhammyEffect)
};
