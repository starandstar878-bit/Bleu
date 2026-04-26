/*
  ==============================================================================

    Source/Effects/TubeDriveEffect.h
    Created: 23 Apr 2026
    Description: 따뜻한 튜브 오버드라이브 사운드를 위한 소프트 클리퍼 모듈입니다.

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"
#include "../Utils/OversamplingHandler.h"

class TubeDriveEffect : public BaseEffect
{
public:
    // ... (constructor unchanged)
    TubeDriveEffect()
    {
        parameters.set("Drive", 0.5f);
        parameters.set("Tone", 0.5f);
        parameters.set("Output", 0.0f); // dB
        oversamplingEnabled = true;
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        currentSampleRate = sampleRate;
        currentBlockSize = samplesPerBlock;
        
        filter.prepare({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        float drive = (float)parameters.getWithDefault("Drive", 0.5f);
        float tone = (float)parameters.getWithDefault("Tone", 0.5f);
        float outGain = juce::Decibels::decibelsToGain((float)parameters.getWithDefault("Output", 0.0f));

        float gainFactor = 1.0f + drive * 9.0f;
        float cutoff = juce::jmap(tone, 0.0f, 1.0f, 500.0f, 15000.0f);
        filter.setCutoffFrequency(cutoff);

        auto numChannels = buffer.getNumChannels();
        auto numSamples = buffer.getNumSamples();

        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* channelData = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s)
            {
                float x = channelData[s] * gainFactor;
                
                if (x > 0.0f)
                    x = std::tanh(x);
                else
                    x = std::tanh(x * 0.8f) / 0.8f;
                
                channelData[s] = x;
            }
        }

        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        filter.process(context);

        buffer.applyGain(outGain);
    }

    // ... (rest unchanged)
    juce::String getName() const override { return "Tube Drive"; }
    juce::String getCategory() const override { return "Distortion"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(juce::Colours::yellow.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);

        auto center = r.getCentre();
        
        // Draw a simple vacuum tube icon
        g.setColour(juce::Colours::orange.withAlpha(0.8f));
        
        // Tube glass body
        juce::Rectangle<float> tube(center.x - 8, center.y - 12, 16, 20);
        g.drawRoundedRectangle(tube, 4.0f, 1.5f);
        
        // Bottom base
        g.fillRect(center.x - 10.0f, center.y + 8.0f, 20.0f, 4.0f);
        
        // Internal glowing filament
        g.setColour(juce::Colours::orange);
        g.drawLine(center.x, center.y - 6, center.x, center.y + 4, 2.0f);

        g.setColour(iconColor.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("DRIVE", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    juce::dsp::StateVariableTPTFilter<float> filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TubeDriveEffect)
};

