#pragma once

#include "BaseEffect.h"

class GlitchEffect : public BaseEffect {
public:
    GlitchEffect() {
        parameters.set("Chance", 0.3f);
        parameters.set("Size", 0.2f);
    }

    void prepare(double sr, int samplesPerBlock) override {
        sampleRate = sr;
        buffer.setSize(2, (int)(sr * 0.5f));
        buffer.clear();
        writePos = 0;
        readPos = 0;
        isGlitching = false;
        glitchCounter = 0;
    }

    void process(juce::AudioBuffer<float>& data) override {
        auto chance = getParameter("Chance");
        auto size = getParameter("Size");
        
        int numSamples = data.getNumSamples();
        for (int s = 0; s < numSamples; ++s) {
            if (!isGlitching && random.nextFloat() < (chance * 0.0001f)) {
                isGlitching = true;
                glitchLength = (int)(size * sampleRate * 0.5f) + 100;
                glitchCounter = glitchLength;
                readPos = (writePos - glitchLength + buffer.getNumSamples()) % buffer.getNumSamples();
            }
            
            for (int ch = 0; ch < data.getNumChannels(); ++ch) {
                float in = data.getSample(ch, s);
                buffer.setSample(ch, writePos, in);
                
                if (isGlitching) {
                    data.setSample(ch, s, buffer.getSample(ch, readPos));
                }
            }
            
            writePos = (writePos + 1) % buffer.getNumSamples();
            if (isGlitching) {
                readPos = (readPos + 1) % buffer.getNumSamples();
                if (--glitchCounter <= 0) isGlitching = false;
            }
        }
    }

    juce::String getName() const override { return "Glitcher"; }

private:
    double sampleRate = 44100.0;
    juce::AudioBuffer<float> buffer;
    int writePos, readPos;
    bool isGlitching;
    int glitchLength, glitchCounter;
    juce::Random random;
};
