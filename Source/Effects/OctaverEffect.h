/*
  ==============================================================================

    Source/Effects/OctaverEffect.h
    Created: 23 Apr 2026
    Description: RubberBand 기반의 정밀 옥타버입니다. (-1, -2, +1 옥타브 지원)

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"
#include "../../Dependencies/rubberband/rubberband/RubberBandStretcher.h"

class OctaverEffect : public BaseEffect
{
public:
    OctaverEffect()
    {
        parameters.set("Mode", 0.0f); // 0: -1 Oct, 1: -2 Oct, 2: +1 Oct
        parameters.set("Mix", 0.5f);
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        stretcher = std::make_unique<RubberBand::RubberBandStretcher>(
            (size_t)sampleRate, 2,
            RubberBand::RubberBandStretcher::OptionProcessRealTime |
            RubberBand::RubberBandStretcher::OptionEngineFaster
        );
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        if (!stretcher) return;

        int mode = (int)parameters.getWithDefault("Mode", 0.0f);
        float mix = (float)parameters.getWithDefault("Mix", 0.5f);
        
        double pitchScale = 0.5; // Default -1 Oct
        if (mode == 1) pitchScale = 0.25; // -2 Oct
        else if (mode == 2) pitchScale = 2.0; // +1 Oct
        
        stretcher->setPitchScale(pitchScale);

        int numSamples = buffer.getNumSamples();
        int numChannels = std::min(2, buffer.getNumChannels());
        const float* inputs[2] = { buffer.getReadPointer(0), numChannels > 1 ? buffer.getReadPointer(1) : buffer.getReadPointer(0) };
        
        stretcher->process(inputs, (size_t)numSamples, false);
        
        if (stretcher->available() >= numSamples)
        {
            juce::AudioBuffer<float> dryBuffer;
            dryBuffer.makeCopyOf(buffer);
            
            float* outputs[2] = { buffer.getWritePointer(0), numChannels > 1 ? buffer.getWritePointer(1) : buffer.getWritePointer(0) };
            stretcher->retrieve(outputs, (size_t)numSamples);
            
            for (int ch = 0; ch < numChannels; ++ch)
                buffer.addFrom(ch, 0, dryBuffer, ch, 0, numSamples, 1.0f - mix);
        }
    }

    juce::String getName() const override { return "Octaver"; }
    juce::String getCategory() const override { return "Pitch"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(juce::Colours::blueviolet.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);
        
        g.setColour(juce::Colours::blueviolet);
        g.drawEllipse(r.getCentreX() - 8, r.getCentreY() - 8, 16, 16, 2.0f);
        g.drawEllipse(r.getCentreX() - 4, r.getCentreY() - 4, 8, 8, 1.5f);

        g.setColour(iconColor.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("OCTAVER", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    std::unique_ptr<RubberBand::RubberBandStretcher> stretcher;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OctaverEffect)
};
