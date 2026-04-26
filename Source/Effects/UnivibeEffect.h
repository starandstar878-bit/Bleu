#pragma once

#include "BaseEffect.h"

class UnivibeEffect : public BaseEffect {
public:
  UnivibeEffect() {
    parameters.set("Speed", 0.3f);
    parameters.set("Intensity", 0.6f);
    parameters.set("Chorus/Vibrato", 0.0f); // 0: Chorus, 1: Vibrato
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    phase = 0.0f;
    for (auto& ap : allPass) {
        ap.prepare({sampleRate, (juce::uint32)samplesPerBlock, 1});
    }
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float speed = (float)parameters.getWithDefault("Speed", 0.3f);
    float intensity = (float)parameters.getWithDefault("Intensity", 0.6f);
    float mode = (float)parameters.getWithDefault("Chorus/Vibrato", 0.0f);

    float lfoIncr = (speed * 8.0f) / currentSampleRate;

    for (int s = 0; s < buffer.getNumSamples(); ++s) {
        // Asymmetric LFO (bulb behavior)
        float lfo = 0.5f + 0.5f * std::sin(juce::MathConstants<float>::twoPi * phase);
        lfo = std::pow(lfo, 1.5f); // Asymmetry
        phase = std::fmod(phase + lfoIncr, 1.0f);

        float freq = juce::jmap(lfo * intensity, 0.0f, 1.0f, 100.0f, 2000.0f);
        auto coeffs = juce::dsp::IIR::Coefficients<float>::makeAllPass(currentSampleRate, freq);
        
        for (auto& ap : allPass) {
            ap.coefficients = coeffs;
        }

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            float in = buffer.getSample(ch, s);
            float out = in;
            for (auto& ap : allPass) {
                out = ap.processSample(out);
            }
            
            if (mode < 0.5f) {
                // Chorus mode (Dry + Wet)
                buffer.setSample(ch, s, (in + out) * 0.5f);
            } else {
                // Vibrato mode (Wet only)
                buffer.setSample(ch, s, out);
            }
        }
    }
  }

  juce::String getName() const override { return "Univibe"; }
  juce::String getCategory() const override { return "Modulation"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    g.drawEllipse(r, 2.0f);
    g.fillEllipse(r.reduced(r.getWidth() * 0.3f));
  }

private:
  float phase = 0.0f;
  juce::dsp::IIR::Filter<float> allPass[4];

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnivibeEffect)
};
