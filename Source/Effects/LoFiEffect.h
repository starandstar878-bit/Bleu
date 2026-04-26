#pragma once

#include "BaseEffect.h"

class LoFiEffect : public BaseEffect {
public:
    LoFiEffect() {
        parameters.set("Crush", 1.0f);
        parameters.set("Noise", 0.1f);
        parameters.set("Flutter", 0.2f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        phase = 0.0f;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto crush = getParameter("Crush");
        auto noise = getParameter("Noise");
        auto flutter = getParameter("Flutter");
        
        float step = 1.0f / crush;
        float flutterPhaseInc = (float)(0.5f / sampleRate); // Very slow LFO for flutter
        
        int numSamples = buffer.getNumSamples();
        
        for (int s = 0; s < numSamples; ++s) {
            float mod = std::sin(phase * juce::MathConstants<float>::twoPi) * flutter * 0.01f;
            
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                float x = buffer.getSample(ch, s);
                
                // 1. Bit Crush
                x = std::round(x / step) * step;
                
                // 2. Noise
                x += (random.nextFloat() * 2.0f - 1.0f) * noise * 0.05f;
                
                buffer.setSample(ch, s, x);
            }
            
            phase += flutterPhaseInc;
            if (phase >= 1.0f) phase -= 1.0f;
        }
    }

    juce::String getName() const override { return "Lo-Fi"; }

private:
    double sampleRate = 44100.0;
    float phase = 0.0f;
    juce::Random random;
};
