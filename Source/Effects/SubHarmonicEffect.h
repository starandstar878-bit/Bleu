#pragma once

#include "BaseEffect.h"

class SubHarmonicEffect : public BaseEffect {
public:
  SubHarmonicEffect() {
    parameters.set("Sub Mix", 0.4f);
    parameters.set("Tone", 0.5f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    phase = false;
    lastIn = 0;
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float mix = (float)parameters.getWithDefault("Sub Mix", 0.4f);
    float tone = (float)parameters.getWithDefault("Tone", 0.5f);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        float* data = buffer.getWritePointer(ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s) {
            float in = data[s];
            
            // Simple Zero-Crossing Octave Down
            if (in > 0 && lastIn <= 0) {
                phase = !phase;
            }
            lastIn = in;

            float sub = phase ? 1.0f : -1.0f;
            sub *= std::abs(in); // Envelope match

            // Low pass for sub
            float filterAlpha = juce::jmap(tone, 0.0f, 1.0f, 0.01f, 0.2f);
            subOut[ch] = (1.0f - filterAlpha) * subOut[ch] + filterAlpha * sub;

            data[s] = in + subOut[ch] * mix;
        }
    }
  }

  juce::String getName() const override { return "Sub-Generator"; }
  juce::String getCategory() const override { return "Dynamics"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    g.drawRect(r, 1.0f);
    g.drawHorizontalLine((int)r.getBottom() - 5, r.getX(), r.getRight());
    g.drawHorizontalLine((int)r.getBottom() - 10, r.getX() + 5, r.getRight() - 5);
  }

private:
  bool phase = false;
  float lastIn = 0.0f;
  float subOut[2] = {0, 0};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubHarmonicEffect)
};
