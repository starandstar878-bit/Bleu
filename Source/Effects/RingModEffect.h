#pragma once

#include "BaseEffect.h"

class RingModEffect : public BaseEffect {
public:
    RingModEffect() {
        parameters.set("Freq", 440.0f);
        parameters.set("Mix", 0.5f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        phase = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto freq = getParameter("Freq");
        auto mix = getParameter("Mix");
        
        float phaseInc = (float)(freq / sampleRate);
        int numSamples = buffer.getNumSamples();
        
        for (int s = 0; s < numSamples; ++s) {
            float mod = std::sin(phase * juce::MathConstants<float>::twoPi);
            
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                float dry = buffer.getSample(ch, s);
                float wet = dry * mod;
                buffer.setSample(ch, s, dry * (1.0f - mix) + wet * mix);
            }
            
            phase += phaseInc;
            if (phase >= 1.0f) phase -= 1.0f;
        }
    }

    juce::String getName() const override { return "Ring Mod"; }

private:
    double sampleRate = 44100.0;
    float phase = 0.0f;
};
