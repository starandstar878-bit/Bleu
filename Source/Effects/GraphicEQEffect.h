#pragma once

#include "BaseEffect.h"

class GraphicEQEffect : public BaseEffect {
public:
  GraphicEQEffect() {
    float freqs[] = { 31.25f, 62.5f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f };
    for (int i = 0; i < 10; ++i) {
        parameters.set(juce::String(freqs[i]) + "Hz", 0.5f);
    }
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    for (auto& band : bands) {
        band.prepare(spec);
    }
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float freqs[] = { 31.25f, 62.5f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 16000.0f };
    
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);

    for (int i = 0; i < 10; ++i) {
        float gain = (float)parameters.getWithDefault(juce::String(freqs[i]) + "Hz", 0.5f);
        float gainDB = juce::jmap(gain, 0.0f, 1.0f, -12.0f, 12.0f);
        
        *bands[i].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(currentSampleRate, freqs[i], 1.414f, juce::Decibels::decibelsToGain(gainDB));
        bands[i].process(ctx);
    }
  }

  juce::String getName() const override { return "10-Band EQ"; }
  juce::String getCategory() const override { return "EQ"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    for (int i = 0; i < 5; ++i) {
        g.drawVerticalLine((int)(r.getX() + i * r.getWidth() / 4), r.getY(), r.getBottom());
        g.fillEllipse(r.getX() + i * r.getWidth() / 4 - 2, r.getCentreY() - 2, 4, 4);
    }
  }

private:
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> bands[10];

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphicEQEffect)
};
