#pragma once

#include "BaseEffect.h"

class StereoEnhancerEffect : public BaseEffect {
public:
    StereoEnhancerEffect() {
        parameters.set("Width", 0.5f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        delayBuffer.setSize(2, (int)(sr * 0.1f)); // 100ms max
        delayBuffer.clear();
        writePos = 0;
    }

    void process(juce::AudioBuffer<float>& buffer) override {
        auto width = getParameter("Width");
        float delayMs = width * 30.0f + 5.0f; // 5-35ms delay for Haas effect
        int delaySamples = (int)(delayMs * sampleRate / 1000.0);
        
        int numSamples = buffer.getNumSamples();
        if (buffer.getNumChannels() < 2) return;

        for (int s = 0; s < numSamples; ++s) {
            float inL = buffer.getSample(0, s);
            float inR = buffer.getSample(1, s);
            
            delayBuffer.setSample(0, writePos, inL);
            delayBuffer.setSample(1, writePos, inR);
            
            int readPos = (writePos - delaySamples + delayBuffer.getNumSamples()) % delayBuffer.getNumSamples();
            
            // Apply delay to Right channel only for width
            buffer.setSample(1, s, delayBuffer.getSample(1, readPos));
            
            writePos = (writePos + 1) % delayBuffer.getNumSamples();
        }
    }

    juce::String getName() const override { return "Stereo Wide"; }

private:
    double sampleRate = 44100.0;
    juce::AudioBuffer<float> delayBuffer;
    int writePos = 0;
};
