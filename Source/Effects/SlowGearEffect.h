#pragma once

#include "BaseEffect.h"

class SlowGearEffect : public BaseEffect {
public:
    SlowGearEffect() {
        parameters.set("Attack", 0.5f);
        parameters.set("Threshold", 0.1f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        currentGain = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto attack = getParameter("Attack");
        auto thresh = getParameter("Threshold");
        
        float attackCoeff = 1.0f - std::exp(-1.0f / (sampleRate * attack * 0.5f));
        int numSamples = buffer.getNumSamples();
        
        for (int s = 0; s < numSamples; ++s) {
            float maxIn = 0.0f;
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                maxIn = std::max(maxIn, std::abs(buffer.getSample(ch, s)));
            }
            
            if (maxIn < thresh * 0.5f) {
                currentGain = 0.0f;
            } else {
                currentGain += attackCoeff * (1.0f - currentGain);
            }
            
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                buffer.setSample(ch, s, buffer.getSample(ch, s) * currentGain);
            }
        }
    }

    juce::String getName() const override { return "Slow Gear"; }

private:
    double sampleRate = 44100.0;
    float currentGain = 0.0f;
};
