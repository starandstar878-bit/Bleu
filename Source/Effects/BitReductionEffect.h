#pragma once

#include "BaseEffect.h"

class BitReductionEffect : public BaseEffect {
public:
    BitReductionEffect() {
        parameters.set("Bits", 0.5f); // 1 to 16 bits
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto bits = getParameter("Bits") * 15.0f + 1.0f;
        float levels = std::pow(2.0f, bits);
        
        int numSamples = buffer.getNumSamples();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s) {
                float x = data[s];
                // Quantize
                data[s] = std::round(x * levels) / levels;
            }
        }
    }

    juce::String getName() const override { return "Bit Reducer"; }

private:
    double sampleRate = 44100.0;
};
