#pragma once

#include "BaseEffect.h"

class AutoPanEffect : public BaseEffect {
public:
    AutoPanEffect() {
        parameters.set("Rate", 1.0f);
        parameters.set("Width", 0.8f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        phase = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto rate = getParameter("Rate");
        auto width = getParameter("Width");
        
        float phaseInc = (float)(rate / sampleRate);
        int numSamples = buffer.getNumSamples();
        
        if (buffer.getNumChannels() < 2) return;

        for (int s = 0; s < numSamples; ++s) {
            float pan = std::sin(phase * juce::MathConstants<float>::twoPi) * width;
            float gainL = std::sqrt(0.5f * (1.0f - pan));
            float gainR = std::sqrt(0.5f * (1.0f + pan));
            
            buffer.setSample(0, s, buffer.getSample(0, s) * gainL);
            buffer.setSample(1, s, buffer.getSample(1, s) * gainR);
            
            phase += phaseInc;
            if (phase >= 1.0f) phase -= 1.0f;
        }
    }

    juce::String getName() const override { return "Auto Pan"; }

private:
    double sampleRate = 44100.0;
    float phase = 0.0f;
};
