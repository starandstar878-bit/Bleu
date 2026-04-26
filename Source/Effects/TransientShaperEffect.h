#pragma once

#include "BaseEffect.h"

class TransientShaperEffect : public BaseEffect {
public:
    TransientShaperEffect() {
        parameters.set("Attack", 0.0f);
        parameters.set("Sustain", 0.0f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        envFast = 0.0f;
        envSlow = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto att = getParameter("Attack");
        auto sus = getParameter("Sustain");
        
        float attCoeff = 1.0f - std::exp(-1.0f / (sampleRate * 0.001f)); // 1ms
        float susCoeff = 1.0f - std::exp(-1.0f / (sampleRate * 0.05f));  // 50ms
        
        int numSamples = buffer.getNumSamples();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            for (int s = 0; s < numSamples; ++s) {
                float input = std::abs(data[s]);
                
                envFast += attCoeff * (input - envFast);
                envSlow += susCoeff * (input - envSlow);
                
                float trans = envFast - envSlow;
                float gain = 1.0f + att * trans * 5.0f + sus * envSlow * 2.0f;
                
                data[s] *= std::max(0.0f, gain);
            }
        }
    }

    juce::String getName() const override { return "Trans Shape"; }

private:
    double sampleRate = 44100.0;
    float envFast = 0.0f, envSlow = 0.0f;
};
