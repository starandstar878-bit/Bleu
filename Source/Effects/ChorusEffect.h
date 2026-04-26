/*
  ==============================================================================

    Source/Effects/ChorusEffect.h
    Created: 23 Apr 2026
    Description: SignalSmith Delay를 기반으로 한 부드러운 스테레오 코러스 모듈입니다.

  ==============================================================================
*/

#pragma once

#include "../../Dependencies/dsp/delay.h"
#include "BaseEffect.h"

class ChorusEffect : public BaseEffect
{
public:
    ChorusEffect()
    {
        parameters.set("Rate", 1.0f);    // Hz
        parameters.set("Depth", 0.5f);   // 0.0 to 1.0
        parameters.set("Feedback", 0.0f);
        parameters.set("Mix", 0.5f);
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        currentSampleRate = sampleRate;
        currentBlockSize = samplesPerBlock;

        auto maxDelaySamples = (int)(sampleRate * 0.1); // 100ms max for chorus

        delayLines.clear();
        for (int i = 0; i < 2; ++i)
        {
            delayLines.emplace_back(maxDelaySamples);
            delayLines.back().reset();
        }

        lfoPhase = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        auto numChannels = buffer.getNumChannels();
        auto numSamples = buffer.getNumSamples();

        float rate = (float)parameters.getWithDefault("Rate", 1.0f);
        float depth = (float)parameters.getWithDefault("Depth", 0.5f);
        float feedback = (float)parameters.getWithDefault("Feedback", 0.0f);
        float mix = (float)parameters.getWithDefault("Mix", 0.5f);

        float phaseIncrement = (float)(juce::MathConstants<double>::twoPi * rate / currentSampleRate);

        for (int s = 0; s < numSamples; ++s)
        {
            // LFO for modulation (Sine wave)
            float modulation = (std::sin(lfoPhase) + 1.0f) * 0.5f; // 0 to 1
            
            // Base delay: 20ms, modulation depth: up to 20ms
            float delayMs = 20.0f + modulation * depth * 20.0f;
            float delaySamples = delayMs * (float)currentSampleRate / 1000.0f;

            for (int ch = 0; ch < numChannels; ++ch)
            {
                if (ch >= (int)delayLines.size()) break;

                auto& delayLine = delayLines[ch];
                auto* channelData = buffer.getWritePointer(ch);

                float input = channelData[s];
                
                // Read from delay line with cubic interpolation
                // Offset phase for stereo width
                float chDelaySamples = delaySamples;
                if (ch == 1) 
                {
                    float stereoMod = (std::cos(lfoPhase) + 1.0f) * 0.5f;
                    float stereoDelayMs = 20.0f + stereoMod * depth * 20.0f;
                    chDelaySamples = stereoDelayMs * (float)currentSampleRate / 1000.0f;
                }

                float delayedSample = delayLine.read(chDelaySamples);
                delayLine.write(input + (delayedSample * feedback));

                channelData[s] = (input * (1.0f - mix)) + (delayedSample * mix);
            }

            lfoPhase += phaseIncrement;
            if (lfoPhase >= juce::MathConstants<float>::twoPi)
                lfoPhase -= juce::MathConstants<float>::twoPi;
        }
    }

    juce::String getName() const override { return "Chorus"; }
    juce::String getCategory() const override { return "Modulation"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(juce::Colours::lavender.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);

        auto center = r.getCentre();
        auto w = r.getWidth();

        // Waveform icon for modulation
        g.setColour(juce::Colours::lavender);
        juce::Path path;
        path.startNewSubPath(center.x - w * 0.3f, center.y);
        for (float x = -w * 0.3f; x <= w * 0.3f; x += 1.0f)
        {
            float y = std::sin(x * 0.2f) * 8.0f;
            path.lineTo(center.x + x, center.y + y);
            path.lineTo(center.x + x + 2, center.y + y + 2); // Double line for "Chorus" feel
        }
        g.strokePath(path, juce::PathStrokeType(1.5f));

        g.setColour(iconColor.withAlpha(0.9f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("CHORUS", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    std::vector<signalsmith::delay::Delay<float, signalsmith::delay::InterpolatorCubic>> delayLines;
    float lfoPhase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChorusEffect)
};
