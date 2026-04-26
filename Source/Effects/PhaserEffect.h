#pragma once

#include "BaseEffect.h"

class PhaserEffect : public BaseEffect {
public:
  PhaserEffect() {
    parameters.set("Rate", 0.5f);
    parameters.set("Depth", 0.5f);
    parameters.set("Feedback", 0.3f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    phase = 0.0f;
    for (auto& ap : allPass) {
        ap.prepare({sampleRate, (juce::uint32)samplesPerBlock, 2});
    }
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float rate = (float)parameters.getWithDefault("Rate", 0.5f);
    float depth = (float)parameters.getWithDefault("Depth", 0.5f);
    float feedback = (float)parameters.getWithDefault("Feedback", 0.3f);

    float lfoIncr = (rate * 5.0f) / currentSampleRate;
    
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int s = 0; s < buffer.getNumSamples(); ++s) {
        float lfo = 0.5f + 0.5f * std::sin(juce::MathConstants<float>::twoPi * phase);
        phase = std::fmod(phase + lfoIncr, 1.0f);

        float freq = juce::jmap(lfo * depth, 0.0f, 1.0f, 200.0f, 4000.0f);
        auto coeffs = juce::dsp::IIR::Coefficients<float>::makeAllPass(currentSampleRate, freq);
        
        for (auto& ap : allPass) {
            ap.coefficients = coeffs;
        }

        float inL = left[s] + lastOutL * feedback;
        float outL = inL;
        
        // Manual processing of all-pass chain
        for (auto& ap : allPass) {
            outL = ap.processSample(outL);
        }
        lastOutL = outL;
        left[s] = left[s] + outL * depth;

        if (right) {
            float inR = right[s] + lastOutR * feedback;
            float outR = inR;
            for (auto& ap : allPass) {
                outR = ap.processSample(outR);
            }
            lastOutR = outR;
            right[s] = right[s] + outR * depth;
        }
    }
  }

  juce::String getName() const override { return "Phaser"; }
  juce::String getCategory() const override { return "Modulation"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    juce::Path p;
    p.addEllipse(r);
    for (int i = 0; i < 4; ++i) {
        p.addEllipse(r.reduced(i * 5));
    }
    g.strokePath(p, juce::PathStrokeType(1.5f));
  }

private:
  float phase = 0.0f;
  float lastOutL = 0.0f, lastOutR = 0.0f;
  juce::dsp::IIR::Filter<float> allPass[4];

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaserEffect)
};
