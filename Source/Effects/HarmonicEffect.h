#pragma once

#include "BaseEffect.h"

class HarmonicEffect : public BaseEffect {
public:
    HarmonicEffect() {
        parameters.set("Sub", 0.5f);
        parameters.set("Drive", 0.3f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        lp.prepare({sr, (juce::uint32)samplesPerBlock, 2});
        *lp.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sr, 150.0f);
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto sub = getParameter("Sub");
        auto drive = getParameter("Drive");
        
        juce::AudioBuffer<float> subBuffer;
        subBuffer.makeCopyOf(buffer);
        
        juce::dsp::AudioBlock<float> block(subBuffer);
        juce::dsp::ProcessContextReplacing<float> ctx(block);
        lp.process(ctx);
        
        int numSamples = buffer.getNumSamples();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            auto* sData = subBuffer.getReadPointer(ch);
            for (int s = 0; s < numSamples; ++s) {
                // Simplified Sub-Octave generation via flip-flop style simulation (abs + divide)
                float sOct = std::sin(std::asin(sData[s]) * 0.5f); 
                data[s] += sOct * sub * (1.0f + drive);
            }
        }
    }

    juce::String getName() const override { return "Sub-Synth"; }

private:
    double sampleRate = 44100.0;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lp;
};
