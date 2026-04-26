#pragma once

#include "BaseEffect.h"

class OctaveFuzzEffect : public BaseEffect {
public:
    OctaveFuzzEffect() {
        parameters.set("Fuzz", 0.7f);
        parameters.set("Octave", 0.5f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto fuzz = getParameter("Fuzz") * 20.0f + 1.0f;
        auto oct = getParameter("Octave");
        
        int numSamples = buffer.getNumSamples();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s) {
                float x = data[s];
                float upper = std::abs(x); // Full-wave rectification creates upper octave
                
                float wet = (x + upper * oct) * fuzz;
                data[s] = std::tanh(wet);
            }
        }
    }

    juce::String getName() const override { return "Octave Fuzz"; }

private:
    double sampleRate = 44100.0;
};
