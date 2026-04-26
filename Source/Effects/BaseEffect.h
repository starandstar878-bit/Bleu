/*
  ==============================================================================

    Source/Effects/BaseEffect.h
    Created: 23 Apr 2026
    Description: 이펙터 기본 클래스. DSP 로직, 메타데이터, 아이콘 드로잉,
                 및 선택적 커스텀 에디터 생성을 지원합니다.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class BaseEffect : public juce::ReferenceCountedObject {
public:
  using Ptr = juce::ReferenceCountedObjectPtr<BaseEffect>;
  virtual ~BaseEffect() = default;

  // DSP Interface
  virtual void prepare(double sampleRate, int samplesPerBlock) = 0;
  virtual void process(juce::AudioBuffer<float> &buffer) = 0;

  bool oversamplingEnabled = false;

  // Metadata Interface
  virtual juce::String getName() const = 0;
  virtual juce::String getCategory() const { return "General"; }
  virtual int getLatencySamples() const { return 0; }

  // Icon Drawing
  virtual void drawIcon(juce::Graphics &g, juce::Rectangle<float> area,
                        juce::Colour iconColor) {
    g.setColour(iconColor);
    g.drawFittedText(getName(), area.toNearestInt(),
                     juce::Justification::centred, 1);
  }

  // Optional Custom Editor
  virtual std::unique_ptr<juce::Component> createEditor() { return nullptr; }

  // Parameter Management
  virtual juce::StringArray getParameterList() const {
    juce::StringArray names;
    for (int i = 0; i < parameters.size(); ++i)
      names.add(parameters.getName(i).toString());
    return names;
  }

  void setParameter(const juce::String &paramID, float value) {
    parameters.set(paramID, value);
  }

  float getParameter(const juce::String &paramID) const {
    return (float)parameters.getWithDefault(paramID, 0.0f);
  }

  // State Management
  virtual juce::ValueTree getState() const {
    juce::ValueTree state("EffectState");
    state.setProperty("name", getName(), nullptr);
    state.setProperty("bypassed", isBypassed, nullptr);

    juce::ValueTree paramsTree("Parameters");
    for (int i = 0; i < parameters.size(); ++i) {
      juce::ValueTree p("Param");
      p.setProperty("id", parameters.getName(i).toString(), nullptr);
      p.setProperty("value", parameters.getValueAt(i), nullptr);
      paramsTree.addChild(p, -1, nullptr);
    }
    state.addChild(paramsTree, -1, nullptr);

    return state;
  }

  virtual void setState(const juce::ValueTree &state) {
    if (state.hasProperty("bypassed"))
      isBypassed = (bool)state.getProperty("bypassed");

    auto paramsTree = state.getChildWithName("Parameters");
    if (paramsTree.isValid()) {
      for (int i = 0; i < paramsTree.getNumChildren(); ++i) {
        auto p = paramsTree.getChild(i);
        if (p.hasType("Param")) {
          juce::String id = p.getProperty("id");
          float value = p.getProperty("value");
          parameters.set(id, value);
        }
      }
    }
  }
  void setBypassed(bool b) { isBypassed = b; }
  bool getBypassed() const { return isBypassed; }

  void setOversamplingEnabled(bool e) { oversamplingEnabled = e; }
  bool getOversamplingEnabled() const { return oversamplingEnabled; }

protected:
  juce::NamedValueSet parameters;
  bool isBypassed = false;
  double currentSampleRate = 44100.0;
  int currentBlockSize = 512;
};
