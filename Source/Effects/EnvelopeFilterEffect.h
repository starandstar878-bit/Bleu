#pragma once

#include "BaseEffect.h"

class EnvelopeFilterEffect : public BaseEffect {
public:
  EnvelopeFilterEffect() {
    parameters.set("Sensitivity", 0.5f);
    parameters.set("Range", 0.6f);
    parameters.set("Resonance", 0.4f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    envelope = 0.0f;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    filter.prepare(spec);
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float sens = (float)parameters.getWithDefault("Sensitivity", 0.5f);
    float range = (float)parameters.getWithDefault("Range", 0.6f);
    float res = (float)parameters.getWithDefault("Resonance", 0.4f);

    float att = std::exp(-1.0f / (10.0f * 0.001f * currentSampleRate)); // 10ms attack
    float rel = std::exp(-1.0f / (100.0f * 0.001f * currentSampleRate)); // 100ms release

    auto* left = buffer.getReadPointer(0);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getReadPointer(1) : left;

    for (int s = 0; s < buffer.getNumSamples(); ++s) {
        float input = std::max(std::abs(left[s]), std::abs(right[s]));
        if (input > envelope) envelope = att * envelope + (1.0f - att) * input;
        else envelope = rel * envelope + (1.0f - rel) * input;

        float freq = juce::jmap(envelope * sens, 0.0f, 1.0f, 200.0f, 5000.0f * range);
        filter.setCutoffFrequency(freq);
        filter.setResonance(juce::jmap(res, 0.0f, 1.0f, 0.707f, 5.0f));

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            buffer.setSample(ch, s, filter.processSample(ch, buffer.getSample(ch, s)));
        }
    }
  }

  juce::String getName() const override { return "Auto-Wah"; }
  juce::String getCategory() const override { return "Filter"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    g.drawEllipse(r, 2.0f);
    g.fillEllipse(r.getCentreX() - 5, r.getCentreY() - 5, 10, 10);
  }

private:
  float envelope = 0.0f;
  juce::dsp::StateVariableTPTFilter<float> filter;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeFilterEffect)
};
