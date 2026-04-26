/*
  ==============================================================================

    Source/Effects/DelayEffect.h
    Created: 23 Apr 2026
    Description: 아이콘 드로잉과 메타데이터가 포함된 고성능 딜레이 이펙터입니다.

  ==============================================================================
*/

#pragma once

#include "../../Dependencies/dsp/delay.h" // SignalSmith DSP
#include "BaseEffect.h"
#include <vector>

class DelayEffect : public BaseEffect {
public:
  DelayEffect() {
    parameters.set("Time", 0.5f);
    parameters.set("Feedback", 0.3f);
    parameters.set("Mix", 0.5f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;

    auto maxDelaySamples = (int)(sampleRate * 2.0); // 2 seconds max

    delayLines.clear();
    for (int i = 0; i < 2; ++i) // Initial stereo setup
    {
      delayLines.emplace_back(maxDelaySamples);
      delayLines.back().reset();
    }

    timeSmoother.reset(sampleRate, 0.05);
    feedbackSmoother.reset(sampleRate, 0.05);
    mixSmoother.reset(sampleRate, 0.05);

    timeSmoother.setCurrentAndTargetValue(
        (float)parameters.getWithDefault("Time", 0.5f));
    feedbackSmoother.setCurrentAndTargetValue(
        (float)parameters.getWithDefault("Feedback", 0.3f));
    mixSmoother.setCurrentAndTargetValue(
        (float)parameters.getWithDefault("Mix", 0.5f));
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    if (delayLines.size() < (size_t)numChannels) {
      auto maxDelaySamples = (int)(currentSampleRate * 2.0);
      while (delayLines.size() < (size_t)numChannels) {
        delayLines.emplace_back(maxDelaySamples);
        delayLines.back().reset();
      }
    }

    timeSmoother.setTargetValue((float)parameters.getWithDefault("Time", 0.5f));
    feedbackSmoother.setTargetValue(
        (float)parameters.getWithDefault("Feedback", 0.3f));
    mixSmoother.setTargetValue((float)parameters.getWithDefault("Mix", 0.5f));

    for (int s = 0; s < numSamples; ++s) {
      float time = timeSmoother.getNextValue();
      float feedback = feedbackSmoother.getNextValue();
      float mix = mixSmoother.getNextValue();

      float delaySamples = time * (float)currentSampleRate;

      for (int ch = 0; ch < numChannels; ++ch) {
        auto &delayLine = delayLines[ch];
        float *channelData = buffer.getWritePointer(ch);

        float input = channelData[s];
        float delayedSample = delayLine.read(delaySamples);

        delayLine.write(input + (delayedSample * feedback));
        channelData[s] = (input * (1.0f - mix)) + (delayedSample * mix);
      }
    }
  }

  juce::String getName() const override { return "Delay"; }

  void drawIcon(juce::Graphics& g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(juce::Colours::cyan.withAlpha(0.7f));
    auto center = area.getCentre();
    float radius = area.getWidth() * 0.25f;

    // Modern concentric circles for delay feedback visualization
    for (int i = 0; i < 3; ++i) {
      float r = radius * (1.0f + i * 0.3f);
      g.setColour(juce::Colours::cyan.withAlpha(0.6f - i * 0.2f));
      g.drawEllipse(center.x - r, center.y - r, r * 2, r * 2, 1.5f);
    }

    g.setColour(iconColor.withAlpha(0.9f));
    g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
    g.drawText("DELAY", area.removeFromBottom(15),
               juce::Justification::centred);
  }

private:
  std::vector<
      signalsmith::delay::Delay<float, signalsmith::delay::InterpolatorCubic>>
      delayLines;

  juce::LinearSmoothedValue<float> timeSmoother;
  juce::LinearSmoothedValue<float> feedbackSmoother;
  juce::LinearSmoothedValue<float> mixSmoother;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEffect)
};
