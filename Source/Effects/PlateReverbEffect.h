#pragma once

#include "BaseEffect.h"

class PlateReverbEffect : public BaseEffect {
public:
  PlateReverbEffect() {
    parameters.set("Decay", 0.6f);
    parameters.set("Brightness", 0.7f);
    parameters.set("Mix", 0.3f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    reverb.setSampleRate(sampleRate);
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    juce::Reverb::Parameters p;
    p.roomSize = (float)parameters.getWithDefault("Decay", 0.6f);
    p.damping = 1.0f - (float)parameters.getWithDefault("Brightness", 0.7f);
    p.wetLevel = (float)parameters.getWithDefault("Mix", 0.3f);
    p.dryLevel = 1.0f - p.wetLevel;
    p.width = 1.0f;
    reverb.setParameters(p);

    if (buffer.getNumChannels() == 1) {
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    } else {
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }
  }

  juce::String getName() const override { return "Plate Reverb"; }
  juce::String getCategory() const override { return "Spatial"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.2f);
    g.drawRect(r, 1.0f);
    g.setColour(iconColor.withAlpha(0.3f));
    g.fillRect(r.reduced(2));
  }

private:
  juce::Reverb reverb;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlateReverbEffect)
};
