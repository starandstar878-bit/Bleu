#pragma once

#include "BaseEffect.h"

class OverdriveEffect : public BaseEffect {
public:
  OverdriveEffect() {
    parameters.set("Drive", 0.4f);
    parameters.set("Tone", 0.5f);
    parameters.set("Output", 0.0f); // dB
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    preFilter.prepare(spec);
    postFilter.prepare(spec);
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float drive = (float)parameters.getWithDefault("Drive", 0.4f);
    float tone = (float)parameters.getWithDefault("Tone", 0.5f);
    float outGain = juce::Decibels::decibelsToGain((float)parameters.getWithDefault("Output", 0.0f));

    // 1. Pre-filter (Mid hump)
    *preFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, 720.0f, 0.5f, 1.0f + drive * 2.0f);
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);
    preFilter.process(ctx);

    // 2. Soft Clipping
    float gain = 1.0f + drive * 10.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        float* data = buffer.getWritePointer(ch);
        for (int s = 0; s < buffer.getNumSamples(); ++s) {
            float x = data[s] * gain;
            // Soft clipping (tanh approximation)
            data[s] = std::tanh(x);
        }
    }

    // 3. Post-filter (Tone control)
    float toneFreq = juce::jmap(tone, 0.0f, 1.0f, 800.0f, 8000.0f);
    *postFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(currentSampleRate, toneFreq, 0.707f);
    postFilter.process(ctx);

    buffer.applyGain(outGain);
  }

  juce::String getName() const override { return "Overdrive"; }
  juce::String getCategory() const override { return "Drive"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    g.fillRoundedRectangle(r, 5.0f);
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillEllipse(r.getCentreX() - 10, r.getCentreY() - 10, 20, 20);
  }

private:
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> preFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> postFilter;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverdriveEffect)
};
