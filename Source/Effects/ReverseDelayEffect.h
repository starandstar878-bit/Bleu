#pragma once

#include "BaseEffect.h"

class ReverseDelayEffect : public BaseEffect {
public:
    ReverseDelayEffect() {
        parameters.set("Time", 0.5f);
        parameters.set("Feedback", 0.3f);
        parameters.set("Mix", 0.5f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        delayBuffer.setSize(2, (int)(sr * 2.0f)); // 2s max
        delayBuffer.clear();
        writePos = 0;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto time = getParameter("Time");
        auto fb = getParameter("Feedback");
        auto mix = getParameter("Mix");
        
        int delaySamples = (int)(time * sampleRate);
        int numSamples = buffer.getNumSamples();
        
        for (int s = 0; s < numSamples; ++s) {
            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                float dry = buffer.getSample(ch, s);
                
                // Read backwards from the current position
                int readPos = (writePos - 1 + delayBuffer.getNumSamples()) % delayBuffer.getNumSamples();
                float wet = delayBuffer.getSample(ch, readPos);
                
                delayBuffer.setSample(ch, writePos, dry + wet * fb);
                buffer.setSample(ch, s, dry * (1.0f - mix) + wet * mix);
            }
            writePos = (writePos + 1) % delayBuffer.getNumSamples();
        }
    }

    juce::String getName() const override { return "Reverse Delay"; }

private:
    double sampleRate = 44100.0;
    juce::AudioBuffer<float> delayBuffer;
    int writePos = 0;
};
