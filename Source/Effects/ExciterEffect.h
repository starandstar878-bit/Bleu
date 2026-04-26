#pragma once

#include "BaseEffect.h"

class ExciterEffect : public BaseEffect {
public:
  ExciterEffect() {
    parameters.set("Amount", 0.4f);
    parameters.set("Freq", 0.6f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    hpFilter.prepare(spec);
    hpFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float amount = (float)parameters.getWithDefault("Amount", 0.4f);
    float freq = (float)parameters.getWithDefault("Freq", 0.6f);

    hpFilter.setCutoffFrequency(juce::jmap(freq, 0.0f, 1.0f, 2000.0f, 8000.0f));
    
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        float* data = buffer.getWritePointer(ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s) {
            float in = data[s];
            float high = hpFilter.processSample(ch, in);
            
            // Harmonic generation (odd harmonics via soft saturation)
            float harmonics = std::tanh(high * 2.0f);
            
            data[s] = in + (harmonics - high) * amount;
        }
    }
  }

  juce::String getName() const override { return "Exciter"; }
  juce::String getCategory() const override { return "Utility"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    juce::Path p;
    p.startNewSubPath(r.getX(), r.getBottom());
    p.lineTo(r.getCentreX(), r.getY());
    p.lineTo(r.getRight(), r.getBottom());
    g.strokePath(p, juce::PathStrokeType(2.0f));
  }

private:
  juce::dsp::StateVariableTPTFilter<float> hpFilter;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExciterEffect)
};
