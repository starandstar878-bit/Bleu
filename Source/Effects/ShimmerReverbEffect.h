#pragma once

#include "BaseEffect.h"

class ShimmerReverbEffect : public BaseEffect {
public:
  ShimmerReverbEffect() {
    parameters.set("Decay", 0.7f);
    parameters.set("Shimmer", 0.5f);
    parameters.set("Mix", 0.3f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    reverb.setSampleRate(sampleRate);
    currentSampleRate = sampleRate;
    
    // Tiny delay for shimmer feedback
    shimmerBuffer.setSize(2, (int)(sampleRate * 0.1f));
    shimmerBuffer.clear();
    writePos = 0;
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float decay = (float)parameters.getWithDefault("Decay", 0.7f);
    float shimmer = (float)parameters.getWithDefault("Shimmer", 0.5f);
    float mix = (float)parameters.getWithDefault("Mix", 0.3f);

    juce::Reverb::Parameters p;
    p.roomSize = decay;
    p.damping = 0.2f;
    p.wetLevel = mix;
    p.dryLevel = 1.0f - mix;
    p.width = 1.0f;
    reverb.setParameters(p);

    // 1. Basic Reverb
    if (buffer.getNumChannels() == 1) {
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    } else {
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }

    // 2. Shimmer Effect (Simple pitch up simulation via resampling)
    if (shimmer > 0.01f) {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            float* data = buffer.getWritePointer(ch);
            for (int s = 0; s < buffer.getNumSamples(); ++s) {
                // Read at 2x speed for octave up
                float readPos = (float)writePos * 2.0f;
                while (readPos >= shimmerBuffer.getNumSamples()) readPos -= shimmerBuffer.getNumSamples();
                
                float shim = shimmerBuffer.getSample(ch, (int)readPos);
                data[s] += shim * shimmer * 0.5f; // Add shimmer to output
                
                // Write back to shimmer buffer with feedback
                shimmerBuffer.setSample(ch, writePos, data[s] * 0.8f);
                writePos = (writePos + 1) % shimmerBuffer.getNumSamples();
            }
        }
    }
  }

  juce::String getName() const override { return "Shimmer Reverb"; }
  juce::String getCategory() const override { return "Spatial"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    for (int i = 0; i < 10; ++i) {
        g.fillEllipse(r.getX() + std::rand() % (int)r.getWidth(), r.getY() + std::rand() % (int)r.getHeight(), 2, 2);
    }
    g.drawRect(r, 1.0f);
  }

private:
  juce::Reverb reverb;
  juce::AudioBuffer<float> shimmerBuffer;
  int writePos = 0;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShimmerReverbEffect)
};
