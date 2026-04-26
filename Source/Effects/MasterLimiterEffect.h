#pragma once

#include "BaseEffect.h"

class MasterLimiterEffect : public BaseEffect {
public:
    MasterLimiterEffect() {
        parameters.set("Ceiling", -0.1f);
        parameters.set("Gain", 0.0f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto ceil = juce::Decibels::decibelsToGain((float)parameters.getWithDefault("Ceiling", -0.1f));
        auto gain = juce::Decibels::decibelsToGain((float)parameters.getWithDefault("Gain", 0.0f));
        
        int numSamples = buffer.getNumSamples();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s) {
                float x = data[s] * gain;
                if (std::abs(x) > ceil) {
                    x = x > 0 ? ceil : -ceil;
                }
                data[s] = x;
            }
        }
    }

    juce::String getName() const override { return "Limiter"; }

private:
    double sampleRate = 44100.0;
};
