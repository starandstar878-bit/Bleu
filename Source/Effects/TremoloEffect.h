#pragma once

#include "BaseEffect.h"

class TremoloEffect : public BaseEffect {
public:
  TremoloEffect() {
    parameters.set("Rate", 0.4f);
    parameters.set("Depth", 0.5f);
    parameters.set("Shape", 0.0f); // 0: Sine, 1: Square
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    phase = 0.0f;
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float rate = (float)parameters.getWithDefault("Rate", 0.4f);
    float depth = (float)parameters.getWithDefault("Depth", 0.5f);
    float shape = (float)parameters.getWithDefault("Shape", 0.0f);

    float lfoIncr = (rate * 10.0f) / currentSampleRate;

    for (int s = 0; s < buffer.getNumSamples(); ++s) {
        float lfo = 0.0f;
        if (shape < 0.5f) {
            lfo = 0.5f + 0.5f * std::sin(juce::MathConstants<float>::twoPi * phase);
        } else {
            lfo = phase < 0.5f ? 1.0f : 0.0f;
        }
        phase = std::fmod(phase + lfoIncr, 1.0f);

        float gain = 1.0f - (lfo * depth);
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            buffer.setSample(ch, s, buffer.getSample(ch, s) * gain);
        }
    }
  }

  juce::String getName() const override { return "Tremolo"; }
  juce::String getCategory() const override { return "Modulation"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    juce::Path p;
    p.startNewSubPath(r.getX(), r.getCentreY());
    for (int i = 0; i < 20; ++i) {
        p.lineTo(r.getX() + i * r.getWidth() / 20, r.getCentreY() + std::sin(i * 0.5f) * 10);
    }
    g.strokePath(p, juce::PathStrokeType(2.0f));
  }

private:
  float phase = 0.0f;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TremoloEffect)
};
