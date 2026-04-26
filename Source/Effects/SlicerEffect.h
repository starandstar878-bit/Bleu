#pragma once

#include "BaseEffect.h"

class SlicerEffect : public BaseEffect {
public:
    SlicerEffect() {
        parameters.set("Rate", 4.0f);
        parameters.set("Depth", 1.0f);
        parameters.set("Sharpness", 0.5f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        phase = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto rate = getParameter("Rate");
        auto depth = getParameter("Depth");
        auto sharp = getParameter("Sharpness");
        
        // Simulating 120BPM for now if host tempo is missing
        float freq = (rate * 120.0f) / 60.0f;
        float phaseInc = (float)(freq / sampleRate);
        
        int numSamples = buffer.getNumSamples();
        
        for (int s = 0; s < numSamples; ++s) {
            float gate = std::sin(phase * juce::MathConstants<float>::twoPi) > (sharp * 2.0f - 1.0f) ? 1.0f : (1.0f - depth);
            
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                buffer.setSample(ch, s, buffer.getSample(ch, s) * gate);
            }
            
            phase += phaseInc;
            if (phase >= 1.0f) phase -= 1.0f;
        }
    }

    juce::String getName() const override { return "Slicer"; }

private:
    double sampleRate = 44100.0;
    float phase = 0.0f;
};
