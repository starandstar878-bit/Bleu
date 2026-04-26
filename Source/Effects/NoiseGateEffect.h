#pragma once

#include "BaseEffect.h"

class NoiseGateEffect : public BaseEffect {
public:
  NoiseGateEffect() {
    parameters.set("Threshold", -60.0f); // dB
    parameters.set("Attack", 1.0f);      // ms
    parameters.set("Release", 100.0f);   // ms
    parameters.set("Ratio", 10.0f);      // Downward expansion ratio
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;
    envelope = 0.0f;
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float thresholdGain = juce::Decibels::decibelsToGain((float)parameters.getWithDefault("Threshold", -60.0f));
    float attackMs = (float)parameters.getWithDefault("Attack", 1.0f);
    float releaseMs = (float)parameters.getWithDefault("Release", 100.0f);
    float ratio = (float)parameters.getWithDefault("Ratio", 10.0f);

    float att = std::exp(-1.0f / (attackMs * 0.001f * currentSampleRate));
    float rel = std::exp(-1.0f / (releaseMs * 0.001f * currentSampleRate));

    auto* left = buffer.getReadPointer(0);
    auto* right = buffer.getNumChannels() > 1 ? buffer.getReadPointer(1) : left;

    for (int s = 0; s < buffer.getNumSamples(); ++s) {
      float inputLevel = std::max(std::abs(left[s]), std::abs(right[s]));
      
      // Envelope follower
      if (inputLevel > envelope) envelope = att * envelope + (1.0f - att) * inputLevel;
      else envelope = rel * envelope + (1.0f - rel) * inputLevel;

      float gain = 1.0f;
      if (envelope < thresholdGain) {
          // Downward expansion
          float diffDB = juce::Decibels::gainToDecibels(envelope) - juce::Decibels::gainToDecibels(thresholdGain);
          gain = juce::Decibels::decibelsToGain(diffDB * (ratio - 1.0f));
      }

      for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        buffer.setSample(ch, s, buffer.getSample(ch, s) * gain);
      }
    }
  }

  juce::String getName() const override { return "Noise Gate"; }
  juce::String getCategory() const override { return "Dynamics"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    g.drawRect(r, 2.0f);
    g.drawLine(r.getX(), r.getCentreY() + 5, r.getCentreX(), r.getCentreY() + 5, 2.0f);
    g.drawLine(r.getCentreX(), r.getCentreY() + 5, r.getRight(), r.getY() + 5, 2.0f);
  }

private:
  float envelope = 0.0f;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGateEffect)
};
