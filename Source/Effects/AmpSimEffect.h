#pragma once

#ifndef NAM_SAMPLE_FLOAT
#define NAM_SAMPLE_FLOAT
#endif

#ifndef EIGEN_MAX_ALIGN_BYTES
#define EIGEN_MAX_ALIGN_BYTES 0
#endif

#include "../../Dependencies/NeuralAmpModelerCore/NAM/dsp.h"
#include "../../Dependencies/NeuralAmpModelerCore/NAM/get_dsp.h"
#include "BaseEffect.h"
#include <atomic>

class AmpSimEffect : public BaseEffect {
public:
  enum class ModelStatus { None, Loading, Ready };

  AmpSimEffect() {
    parameters.set("Input Gain", 0.0f);  // dB
    parameters.set("Output Gain", 3.0f); // dB
    parameters.set("Amp Mix", 1.0f);     // 0 to 1
    parameters.set("Cab Mix", 1.0f);     // 0 to 1
    parameters.set("Bass", 0.5f);
    parameters.set("Mid", 0.5f);
    parameters.set("Treble", 0.5f);
    parameters.set("Presence", 0.5f);
    
    // New functional parameters
    parameters.set("Bright", 0.0f);   // 0 or 1
    parameters.set("Deep", 0.0f);     // 0 or 1
    parameters.set("Channel", 0.0f);  // 0: Normal, 1: Soft, 2: Rough

    ampProfilePath = "";
    irPath = "./Cabinets/Marshall Box - 57 C - 24bit - 48kHz - 500ms.wav";

    modelStatus = ModelStatus::None;
  }

  ~AmpSimEffect() { stopThread(2000); }

  void loadAmpProfile(const juce::File &file, bool isFull = false) {
    if (file.existsAsFile() && file.getFileExtension() == ".nam") {
      ampProfilePath = file.getFullPathName();
      pendingFile = file;
      isFullRig = isFull;
      modelStatus = ModelStatus::Loading;
      startThread();
    } else if (file.getFileName() == "None") {
      ampProfilePath = "";
      isFullRig = false;
      modelStatus = ModelStatus::None;
      
      // Thread-safe clear
      nam::DSP* expected = modelHandOff.load();
      while (!modelHandOff.compare_exchange_weak(expected, nullptr));
      if (expected) delete expected;
      
      shouldStartCrossfade = true;
    } else if (file.existsAsFile()) // Legacy IR loading for amp (fallback)
    {
      ampProfilePath = file.getFullPathName();
      isFullRig = false;
      ampConvolution.loadImpulseResponse(
          file, juce::dsp::Convolution::Stereo::yes,
          juce::dsp::Convolution::Trim::yes, 0,
          juce::dsp::Convolution::Normalise::yes);
    }
  }

  void loadIR(const juce::File &file) {
    if (file.existsAsFile()) {
      irPath = file.getFullPathName();
      irConvolution.loadImpulseResponse(file,
                                        juce::dsp::Convolution::Stereo::yes,
                                        juce::dsp::Convolution::Trim::yes, 0,
                                        juce::dsp::Convolution::Normalise::yes);
    }
  }

  void prepare(double sampleRate, int samplesPerBlock) override {
    currentSampleRate = sampleRate;
    currentBlockSize = samplesPerBlock;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    ampConvolution.prepare(spec);
    irConvolution.prepare(spec);
    toneStack.prepare(spec);
    
    brightFilter.prepare(spec);
    deepFilter.prepare(spec);
    
    updateToneStack(true);

    if (currentModel != nullptr)
      currentModel->ResetAndPrewarm(sampleRate, samplesPerBlock);

    nextBuffer.setSize(2, samplesPerBlock);
    nextBuffer.clear();

    // Load defaults if they exist
    if (ampProfilePath.isNotEmpty() && ampProfilePath != "None")
      loadAmpProfile(juce::File(ampProfilePath));
    if (irPath.isNotEmpty() && irPath != "None")
      loadIR(juce::File(irPath));
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    // Check for new model hand-off from loader thread
    nam::DSP* newModelPtr = modelHandOff.exchange(nullptr);
    if (newModelPtr != nullptr) {
      nextModel.reset(newModelPtr);
      shouldStartCrossfade = true;
    }

    float inGainDb = (float)parameters.getWithDefault("Input Gain", 0.0f);
    
    // Channel Offset Logic
    int channel = (int)parameters.getWithDefault("Channel", 0.0f);
    if (channel == 1) inGainDb -= 6.0f; // Soft
    else if (channel == 2) inGainDb += 6.0f; // Rough
    
    float inGain = juce::Decibels::decibelsToGain(inGainDb);
    float outGain = juce::Decibels::decibelsToGain(
        (float)parameters.getWithDefault("Output Gain", 3.0f));

    // 1. Input Gain
    buffer.applyGain(inGain);

    // NAM Processing with Crossfade
    if (shouldStartCrossfade) {
      fadePos = 0;
      isCrossfading = true;
      shouldStartCrossfade = false;
    }

    if (isCrossfading) {
      nextBuffer.copyFrom(0, 0, buffer, 0, 0, numSamples);
      if (numChannels > 1)
        nextBuffer.copyFrom(1, 0, buffer, 1, 0, numSamples);

      // Process current
      if (currentModel != nullptr) {
        float *const *channels = buffer.getArrayOfWritePointers();
        float* monoPtr[1] = { channels[0] };
        currentModel->process((NAM_SAMPLE**)monoPtr, (NAM_SAMPLE**)monoPtr, numSamples);
        if (numChannels > 1) buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
      }

      // Process next
      if (nextModel != nullptr) {
        float *const *nextChannels = nextBuffer.getArrayOfWritePointers();
        float* monoNextPtr[1] = { nextChannels[0] };
        nextModel->process((NAM_SAMPLE**)monoNextPtr, (NAM_SAMPLE**)monoNextPtr, numSamples);
        if (numChannels > 1) nextBuffer.copyFrom(1, 0, nextBuffer, 0, 0, numSamples);
      }

      // Crossfade
      for (int s = 0; s < numSamples; ++s) {
        float gainNext =
            juce::jlimit(0.0f, 1.0f, (float)(fadePos + s) / (float)fadeLength);
        float gainCurr = 1.0f - gainNext;

        for (int ch = 0; ch < numChannels; ++ch) {
          buffer.setSample(ch, s,
                           buffer.getSample(ch, s) * gainCurr +
                                nextBuffer.getSample(ch, s) * gainNext);
        }
      }

      fadePos += numSamples;
      if (fadePos >= fadeLength) {
        // Move current model to a cleanup list if we wanted to be perfectly smooth,
        // but for now we just swap. The deletion will happen in audio thread,
        // which isn't ideal but better than the race condition.
        currentModel = std::move(nextModel);
        isCrossfading = false;
        modelStatus = currentModel ? ModelStatus::Ready : ModelStatus::None;
      }
    } else if (currentModel != nullptr) {
      float *const *channels = buffer.getArrayOfWritePointers();
      float* monoPtr[1] = { channels[0] };
      currentModel->process((NAM_SAMPLE**)monoPtr, (NAM_SAMPLE**)monoPtr, numSamples);
      if (numChannels > 1) buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
    }

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // 2. Amp Profile (FIR Fallback)
    float ampMix = (float)parameters.getWithDefault("Amp Mix", 1.0f);
    if (ampMix > 0.0f && currentModel == nullptr &&
        ampProfilePath.isNotEmpty()) {
      ampConvolution.process(context);
    }

    // 3. Cab IR (FIR) - Skip if full rig
    float cabMix = (float)parameters.getWithDefault("Cab Mix", 1.0f);
    if (cabMix > 0.0f && !isFullRig) {
      irConvolution.process(context);
    }

    // 4. Tone Stack & Special Filters
    updateToneStack();
    
    if ((float)parameters.getWithDefault("Bright", 0.0f) > 0.5f) brightFilter.process(context);
    if ((float)parameters.getWithDefault("Deep", 0.0f) > 0.5f) deepFilter.process(context);
    
    toneStack.process(context);

    // 5. Output Gain
    buffer.applyGain(outGain);
  }

  ModelStatus getModelStatus() const { return modelStatus; }
  double getExpectedSampleRate() const { return expectedSampleRate; }

  juce::String getName() const override { return "Amp Sim"; }
  juce::String getCategory() const override { return "Dynamics"; }
  int getLatencySamples() const override {
    return ampConvolution.getLatency() + irConvolution.getLatency();
  }

  juce::ValueTree getState() const override {
    auto state = this->BaseEffect::getState();
    state.setProperty("ampProfilePath", ampProfilePath, nullptr);
    state.setProperty("irPath", irPath, nullptr);
    state.setProperty("isFullRig", (bool)isFullRig, nullptr);
    return state;
  }

  void setState(const juce::ValueTree &state) override {
    this->BaseEffect::setState(state);
    if (state.hasProperty("ampProfilePath"))
      ampProfilePath = state.getProperty("ampProfilePath").toString();
    if (state.hasProperty("irPath"))
      irPath = state.getProperty("irPath").toString();
    if (state.hasProperty("isFullRig"))
      isFullRig = (bool)state.getProperty("isFullRig");
  }

  void drawIcon(juce::Graphics &g, juce::Rectangle<float> area,
                juce::Colour iconColor) override {
    g.setColour(juce::Colours::cyan.withAlpha(0.8f));
    auto r = area.reduced(5);
    g.drawRoundedRectangle(r, 3.0f, 2.0f);

    auto topHalf = r.removeFromTop(r.getHeight() * 0.4f);
    g.setColour(iconColor.withAlpha(0.15f));
    g.fillRect(topHalf);

    g.setColour(juce::Colours::cyan.withAlpha(0.6f));
    for (int i = 0; i < 4; ++i) {
      float x = topHalf.getX() + (topHalf.getWidth() * (i + 1) / 5.0f);
      g.fillEllipse(x - 2, topHalf.getCentreY() - 2, 4, 4);
    }

    g.setColour(iconColor.withAlpha(0.9f));
    g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));
    g.drawText("NAM AMP", area.removeFromBottom(15),
               juce::Justification::centred);
  }

public:
  juce::String ampProfilePath;
  juce::String irPath;
  std::atomic<bool> isFullRig { false };

private:
  class ModelLoaderThread : public juce::Thread {
  public:
    ModelLoaderThread(AmpSimEffect &owner)
        : Thread("NAMLoader"), owner(owner) {}
    void run() override {
      try {
        std::filesystem::path modelPath(owner.pendingFile.getFullPathName().toStdString());
        auto newModel = nam::get_dsp(modelPath);
        if (newModel) {
          newModel->ResetAndPrewarm(owner.currentSampleRate,
                                    owner.currentBlockSize);
          owner.expectedSampleRate = newModel->GetExpectedSampleRate();
          
          // Hand-off to audio thread safely
          nam::DSP* rawPtr = newModel.release();
          nam::DSP* existing = owner.modelHandOff.exchange(rawPtr);
          if (existing) delete existing; // If we pushed two in one block, clean up old one
        }
      } catch (...) {
        owner.modelStatus = ModelStatus::None;
      }
    }

  private:
    AmpSimEffect &owner;
  } loaderThread{*this};

  void startThread() { loaderThread.startThread(); }
  void stopThread(int timeout) { loaderThread.stopThread(timeout); }

  juce::File pendingFile;
  std::atomic<ModelStatus> modelStatus{ModelStatus::None};
  std::atomic<double> expectedSampleRate{-1.0};

  std::unique_ptr<nam::DSP> currentModel;
  std::unique_ptr<nam::DSP> nextModel;
  std::atomic<nam::DSP*> modelHandOff { nullptr };
  
  juce::AudioBuffer<float> nextBuffer;

  std::atomic<bool> shouldStartCrossfade{false};
  bool isCrossfading = false;
  int fadePos = 0;
  const int fadeLength = 2048; // samples

  juce::dsp::Convolution ampConvolution;
  juce::dsp::Convolution irConvolution;

  // Tone Stack
  struct ToneStack
      : public juce::dsp::ProcessorChain<
            juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>, // Bass
            juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>, // Mid
            juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>, // Treble
            juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>  // Presence
            > {} toneStack;

  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> brightFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> deepFilter;

  float lastBass = -1.0f, lastMid = -1.0f, lastTreble = -1.0f, lastPresence = -1.0f;

  void updateToneStack(bool force = false) {
    float bass = (float)parameters.getWithDefault("Bass", 0.5f);
    float mid = (float)parameters.getWithDefault("Mid", 0.5f);
    float treble = (float)parameters.getWithDefault("Treble", 0.5f);
    float presence = (float)parameters.getWithDefault("Presence", 0.5f);

    if (!force && bass == lastBass && mid == lastMid && treble == lastTreble && presence == lastPresence)
      return;

    lastBass = bass; lastMid = mid; lastTreble = treble; lastPresence = presence;
    auto sr = currentSampleRate > 0 ? currentSampleRate : 44100.0;

    *toneStack.get<0>().state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sr, 150.0f, 0.707f, juce::jmap(bass, 0.0f, 1.0f, 0.5f, 2.0f));
    *toneStack.get<1>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sr, 800.0f, 0.707f, juce::jmap(mid, 0.0f, 1.0f, 0.5f, 2.0f));
    *toneStack.get<2>().state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sr, 2500.0f, 0.707f, juce::jmap(treble, 0.0f, 1.0f, 0.5f, 2.0f));
    *toneStack.get<3>().state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sr, 6000.0f, 0.707f, juce::jmap(presence, 0.0f, 1.0f, 0.5f, 2.0f));
    
    // Fixed Bright/Deep curves
    *brightFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sr, 3000.0f, 0.707f, 2.0f); // +6dB
    *deepFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sr, 80.0f, 0.707f, 2.0f);    // +6dB
  }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpSimEffect)
};

