#pragma once

#include "BaseEffect.h"

class SpringReverbEffect : public BaseEffect {
public:
  SpringReverbEffect() {
    parameters.set("Size", 0.5f);
    parameters.set("Damping", 0.3f);
    parameters.set("Mix", 0.2f);
    parameters.set("Width", 0.8f);
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    reverb.setSampleRate(sampleRate);
    updateParams();
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    updateParams();
    
    if (buffer.getNumChannels() == 1) {
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    } else {
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
    }
  }

  juce::String getName() const override { return "Spring Reverb"; }
  juce::String getCategory() const override { return "Spatial"; }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area, juce::Colour iconColor) override {
    g.setColour(iconColor);
    auto r = area.reduced(area.getWidth() * 0.25f);
    juce::Path p;
    p.startNewSubPath(r.getX(), r.getBottom());
    for (int i = 0; i < 4; ++i) {
        p.quadraticTo(r.getCentreX() + (i%2 ? 10 : -10), r.getBottom() - (i+0.5f)*r.getHeight()/4,
                      r.getX(), r.getBottom() - (i+1)*r.getHeight()/4);
    }
    g.strokePath(p, juce::PathStrokeType(2.0f));
  }

private:
  void updateParams() {
    juce::Reverb::Parameters p;
    p.roomSize = (float)parameters.getWithDefault("Size", 0.5f);
    p.damping = (float)parameters.getWithDefault("Damping", 0.3f);
    p.wetLevel = (float)parameters.getWithDefault("Mix", 0.2f);
    p.dryLevel = 1.0f - p.wetLevel;
    p.width = (float)parameters.getWithDefault("Width", 0.8f);
    p.freezeMode = 0.0f;
    reverb.setParameters(p);
  }

  juce::Reverb reverb;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpringReverbEffect)
};
