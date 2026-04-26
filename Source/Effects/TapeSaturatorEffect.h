#pragma once

#include "BaseEffect.h"

class TapeSaturatorEffect : public BaseEffect {
public:
    TapeSaturatorEffect() {
        parameters.set("Drive", 0.5f);
        parameters.set("Warmth", 0.4f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto drive = getParameter("Drive") * 10.0f + 1.0f;
        auto warmth = getParameter("Warmth");
        
        int numSamples = buffer.getNumSamples();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s) {
                float x = data[s] * drive;
                // Soft clipping with warmth (asymmetric)
                x = std::tanh(x + x * x * warmth * 0.1f);
                data[s] = x;
            }
        }
    }

    juce::String getName() const override { return "Tape Sat"; }

private:
    double sampleRate = 44100.0;
};
