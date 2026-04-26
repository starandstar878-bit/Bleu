#pragma once

#include "BaseEffect.h"

class BitcrusherEffect : public BaseEffect {
public:
  BitcrusherEffect() {
    parameters.set("Bits", 0.8f);
    parameters.set("Downsample", 0.2f);
    parameters.set("Mix", 0.5f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float bits = (float)parameters.getWithDefault("Bits", 0.8f);
    float downsample = (float)parameters.getWithDefault("Downsample", 0.2f);
    float mix = (float)parameters.getWithDefault("Mix", 0.5f);

    int bitLevels = (int)std::pow(2, juce::jmap(bits, 0.0f, 1.0f, 2.0f, 24.0f));
    int dsFactor = (int)juce::jmap(downsample, 0.0f, 1.0f, 1.0f, 50.0f);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        float* data = buffer.getWritePointer(ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s) {
            float in = data[s];
            
            // 1. Downsample (Sample & Hold)
            if (s % dsFactor == 0) {
                lastSample[ch] = in;
            }
            float x = lastSample[ch];

            // 2. Bitcrush (Quantization)
            x = std::round(x * bitLevels) / bitLevels;

            // 3. Mix
            data[s] = in * (1.0f - mix) + x * mix;
        }
    }
  }

  juce::String getName() const override { return "Bitcrusher"; }
  juce::String getCategory() const override { return "Lo-Fi"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    for (int i = 0; i < 4; ++i) {
        g.fillRect((float)(r.getX() + i * 10), (float)(r.getBottom() - (i+1)*5), 8.0f, (float)((i+1)*5));
    }
  }

private:
  float lastSample[2] = {0, 0};
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitcrusherEffect)
};
