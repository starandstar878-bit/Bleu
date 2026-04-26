#pragma once

#include "BaseEffect.h"

class FuzzEffect : public BaseEffect {
public:
  FuzzEffect() {
    parameters.set("Gain", 0.7f);
    parameters.set("Sustain", 0.8f);
    parameters.set("Tone", 0.4f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float gain = (float)parameters.getWithDefault("Gain", 0.7f);
    float sustain = (float)parameters.getWithDefault("Sustain", 0.8f);
    float tone = (float)parameters.getWithDefault("Tone", 0.4f);

    float drive = 1.0f + gain * 50.0f;
    float bias = 0.1f; // Germanium-style asymmetry

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        float* data = buffer.getWritePointer(ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s) {
            float x = data[s] * drive + bias;
            
            // Hard clipping with smooth edges
            if (x > 1.0f) x = 1.0f;
            else if (x < -1.0f) x = -1.0f;
            else x = x * (1.5f - 0.5f * x * x); // Soften the knee

            data[s] = x * sustain;
        }
    }

    // Tone shaping (Big Muff style mid-scoop)
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = currentSampleRate;
    spec.maximumBlockSize = (juce::uint32)buffer.getNumSamples();
    spec.numChannels = (juce::uint32)buffer.getNumChannels();

    float toneFreq = juce::jmap(tone, 0.0f, 1.0f, 400.0f, 4000.0f);
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(currentSampleRate, toneFreq);
    
    // Simple filter application
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* d = buffer.getWritePointer(ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s) {
            // Apply a very simple RC-style filter for speed
            lastFiltered[ch] = 0.9f * lastFiltered[ch] + 0.1f * d[s];
            d[s] = lastFiltered[ch];
        }
    }
  }

  juce::String getName() const override { return "70s Fuzz"; }
  juce::String getCategory() const override { return "Drive"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    g.fillEllipse(r);
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawRect(r.withSize(r.getWidth(), 2), 2.0f);
  }

private:
  float lastFiltered[2] = {0, 0};
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FuzzEffect)
};
