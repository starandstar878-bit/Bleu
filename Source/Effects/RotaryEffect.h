#pragma once

#include "BaseEffect.h"

class RotaryEffect : public BaseEffect {
public:
    RotaryEffect() {
        parameters.set("Speed", 2.0f);
        parameters.set("Depth", 0.5f);
        parameters.set("Spread", 0.7f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        phase = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto speed = getParameter("Speed");
        auto depth = getParameter("Depth");
        auto spread = getParameter("Spread");
        
        float phaseInc = (float)(speed / sampleRate);
        int numSamples = buffer.getNumSamples();
        
        for (int s = 0; s < numSamples; ++s) {
            float modL = std::sin(phase * juce::MathConstants<float>::twoPi);
            float modR = std::sin((phase + spread * 0.25f) * juce::MathConstants<float>::twoPi);
            
            float gainL = 1.0f - depth * (0.5f + 0.5f * modL);
            float gainR = 1.0f - depth * (0.5f + 0.5f * modR);
            
            if (buffer.getNumChannels() > 0) buffer.setSample(0, s, buffer.getSample(0, s) * gainL);
            if (buffer.getNumChannels() > 1) buffer.setSample(1, s, buffer.getSample(1, s) * gainR);
            
            phase += phaseInc;
            if (phase >= 1.0f) phase -= 1.0f;
        }
    }

    juce::String getName() const override { return "Rotary"; }

private:
    double sampleRate = 44100.0;
    float phase = 0.0f;
};
