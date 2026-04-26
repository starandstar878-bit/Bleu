#pragma once

#include "BaseEffect.h"

class WaveFolderEffect : public BaseEffect {
public:
    WaveFolderEffect() {
        parameters.set("Gain", 1.0f);
        parameters.set("Folds", 0.5f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto gain = getParameter("Gain") * 5.0f;
        auto folds = getParameter("Folds") * 4.0f + 1.0f;
        
        int numSamples = buffer.getNumSamples();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s) {
                float x = data[s] * gain;
                // Sine-based folding
                for (int f = 0; f < (int)folds; ++f) {
                    x = std::sin(x * juce::MathConstants<float>::pi * 0.5f);
                }
                data[s] = x;
            }
        }
    }

    juce::String getName() const override { return "Wave Folder"; }

private:
    double sampleRate = 44100.0;
};
