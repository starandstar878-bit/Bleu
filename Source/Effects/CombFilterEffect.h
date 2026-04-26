#pragma once

#include "BaseEffect.h"

class CombFilterEffect : public BaseEffect {
public:
    CombFilterEffect() {
        parameters.set("Freq", 0.5f);
        parameters.set("Feedback", 0.8f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        delayBuffer.setSize(2, (int)(sr * 0.1f));
        delayBuffer.clear();
        writePos = 0;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto freq = getParameter("Freq");
        auto fb = getParameter("Feedback") * 0.95f;
        
        float delayMs = 1.0f / (freq * 1000.0f + 20.0f) * 1000.0f;
        int delaySamples = (int)(delayMs * sampleRate / 1000.0);
        
        int numSamples = buffer.getNumSamples();
        for (int s = 0; s < numSamples; ++s) {
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                int readPos = (writePos - delaySamples + delayBuffer.getNumSamples()) % delayBuffer.getNumSamples();
                float delayed = delayBuffer.getSample(ch, readPos);
                
                float in = buffer.getSample(ch, s);
                float out = in + delayed * fb;
                
                delayBuffer.setSample(ch, writePos, out);
                buffer.setSample(ch, s, out);
            }
            writePos = (writePos + 1) % delayBuffer.getNumSamples();
        }
    }

    juce::String getName() const override { return "Comb Filter"; }

private:
    double sampleRate = 44100.0;
    juce::AudioBuffer<float> delayBuffer;
    int writePos = 0;
};
