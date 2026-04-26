/*
  ==============================================================================

    Source/Effects/CompEffect.h
    Created: 23 Apr 2026
    Description: JUCE DSP를 기반으로 한 고성능 컴프레서 모듈입니다.

  ==============================================================================
*/

#pragma once

#include "BaseEffect.h"

class CompEffect : public BaseEffect
{
public:
    CompEffect()
    {
        parameters.set("Threshold", -20.0f); // dB
        parameters.set("Ratio", 4.0f);
        parameters.set("Attack", 5.0f);      // ms
        parameters.set("Release", 100.0f);   // ms
        parameters.set("Gain", 0.0f);        // Makeup Gain (dB)
    }

    void prepare(double sampleRate, int samplesPerBlock) override
    {
        currentSampleRate = sampleRate;
        currentBlockSize = samplesPerBlock;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = 2;

        compressor.prepare(spec);
        updateParameters();
    }

    void process(juce::AudioBuffer<float>& buffer) override
    {
        updateParameters();

        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        compressor.process(context);
        
        // Apply makeup gain
        float gain = juce::Decibels::decibelsToGain((float)parameters.getWithDefault("Gain", 0.0f));
        buffer.applyGain(gain);
    }

    juce::String getName() const override { return "Comp"; }
    juce::String getCategory() const override { return "Dynamics"; }

    void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override
    {
        auto r = area.reduced(5);
        g.setColour(iconColor.withAlpha(0.2f));
        g.drawRoundedRectangle(r, 4.0f, 1.5f);

        auto center = r.getCentre();
        auto w = r.getWidth();
        auto h = r.getHeight();

        g.setColour(iconColor.withAlpha(0.8f));
        
        // Compressor visualization: Squashing lines
        float lineY1 = center.y - h * 0.2f;
        float lineY2 = center.y + h * 0.2f;
        
        g.drawLine(center.x - w * 0.3f, lineY1, center.x + w * 0.3f, lineY1, 2.0f);
        g.drawLine(center.x - w * 0.3f, lineY2, center.x + w * 0.3f, lineY2, 2.0f);

        // Arrows pointing inwards
        g.drawArrow(juce::Line<float>(center.x, lineY1 - 10, center.x, lineY1), 1.5f, 5.0f, 5.0f);
        g.drawArrow(juce::Line<float>(center.x, lineY2 + 10, center.x, lineY2), 1.5f, 5.0f, 5.0f);

        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
        g.drawText("COMP", area.removeFromBottom(15), juce::Justification::centred);
    }

private:
    void updateParameters()
    {
        compressor.setThreshold((float)parameters.getWithDefault("Threshold", -20.0f));
        compressor.setRatio((float)parameters.getWithDefault("Ratio", 4.0f));
        compressor.setAttack((float)parameters.getWithDefault("Attack", 5.0f));
        compressor.setRelease((float)parameters.getWithDefault("Release", 100.0f));
    }

    juce::dsp::Compressor<float> compressor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompEffect)
};
