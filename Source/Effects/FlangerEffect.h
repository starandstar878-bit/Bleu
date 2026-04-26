#pragma once

#include "BaseEffect.h"

class FlangerEffect : public BaseEffect {
public:
  FlangerEffect() {
    parameters.set("Rate", 0.2f);
    parameters.set("Depth", 0.5f);
    parameters.set("Feedback", 0.4f);
    parameters.set("Manual", 0.3f); // Offset
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    phase = 0.0f;
    delayLine.setSize(2, (int)(sampleRate * 0.05f)); // 50ms buffer
    delayLine.clear();
    writePos = 0;
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    float rate = (float)parameters.getWithDefault("Rate", 0.2f);
    float depth = (float)parameters.getWithDefault("Depth", 0.5f);
    float feedback = (float)parameters.getWithDefault("Feedback", 0.4f);
    float manual = (float)parameters.getWithDefault("Manual", 0.3f);

    float lfoIncr = rate / currentSampleRate;
    int bufferSize = delayLine.getNumSamples();

    for (int s = 0; s < buffer.getNumSamples(); ++s) {
        float lfo = 0.5f + 0.5f * std::sin(juce::MathConstants<float>::twoPi * phase);
        phase = std::fmod(phase + lfoIncr, 1.0f);

        // Flanger delay is very short: 0.1ms to 10ms
        float delayMs = 0.1f + (manual * 5.0f) + (lfo * depth * 5.0f);
        float delaySamples = delayMs * 0.001f * currentSampleRate;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            float in = buffer.getSample(ch, s);
            
            // Read from delay line with interpolation
            float readPos = (float)writePos - delaySamples;
            if (readPos < 0) readPos += bufferSize;
            
            int i1 = (int)readPos;
            int i2 = (i1 + 1) % bufferSize;
            float frac = readPos - i1;
            
            float delayed = delayLine.getSample(ch, i1) * (1.0f - frac) + delayLine.getSample(ch, i2) * frac;
            
            // Feedback
            delayLine.setSample(ch, writePos, in + delayed * feedback);
            
            // Mix
            buffer.setSample(ch, s, in + delayed);
        }
        
        writePos = (writePos + 1) % bufferSize;
    }
  }

  juce::String getName() const override { return "Flanger"; }
  juce::String getCategory() const override { return "Modulation"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    for (int i = 0; i < 6; ++i) {
        g.drawHorizontalLine((int)(r.getY() + i * r.getHeight() / 5), r.getX(), r.getRight());
    }
  }

private:
  float phase = 0.0f;
  juce::AudioBuffer<float> delayLine;
  int writePos = 0;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlangerEffect)
};
