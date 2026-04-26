/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../Dependencies/dsp/fft.h"
#include "Effects/AmpSimEffect.h"
#include "Effects/ChorusEffect.h"
#include "Effects/CompEffect.h"
#include "Effects/DelayEffect.h"
#include "Effects/EffectRegistry.h"
#include "Effects/PowerAmpEffect.h"
#include "Effects/TubeDriveEffect.h"
#include "Utils/OversamplingHandler.h"


//==============================================================================
/**
 */
class BleuAudioProcessor : public juce::AudioProcessor {
public:
  // Thread-safe Chain Container
  struct EffectChain : public juce::ReferenceCountedObject {
    using Ptr = juce::ReferenceCountedObjectPtr<EffectChain>;
    juce::ReferenceCountedArray<BaseEffect> effects;
  };

  //==============================================================================
  BleuAudioProcessor();
  ~BleuAudioProcessor() override;

  //==============================================================================
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  juce::ReferenceCountedArray<BaseEffect> getActiveEffects() const {
    return activeChain ? activeChain->effects
                       : juce::ReferenceCountedArray<BaseEffect>();
  }

  juce::ReferenceCountedArray<BaseEffect> getTargetEffects() const {
    if (transitionState.load() != TransitionState::Idle && pendingChain)
      return pendingChain->effects;
    return getActiveEffects();
  }

  // Shared FFT Data for effects
  const float *getLatestFFTMagnitudes() const { return fftMagnitudes; }
  int getFFTSize() const { return fftSize / 2; }

  void addEffect(const juce::String &effectName, int index = -1,
                 bool autoCommit = true);
  void removeEffect(int index, bool autoCommit = true);
  void moveEffect(int oldIndex, int newIndex, bool autoCommit = true);

  enum class TransitionState {
    Idle,
    Preparing, // Shadow chain is ready/warming
    Crossover  // Currently cross-fading
  };

  void prepareShadowChain(
      const juce::ReferenceCountedObjectPtr<EffectChain> &nextChain);
  void commitTransition(float durationMs = 50.0f);
  void cancelTransition();
  TransitionState getTransitionState() const { return transitionState.load(); }

  void setupDefaultChain();

  // FFT 관련 공개 인터페이스
  static constexpr int fftOrder = 11; // 2^11 = 2048
  static constexpr int fftSize = 1 << fftOrder;

  void pushNextSampleIntoFifo(float sample) noexcept;
  void getNextFFTData(float *outputData) noexcept;
  bool getNextResultsReady() const noexcept { return nextFFTBlockReady; }
  void setNextResultsReady(bool ready) noexcept { nextFFTBlockReady = ready; }

  // Master EQ Interface
  void updateMasterEQ(int bandIdx, float frequency, float gainDb,
                      float q = 0.707f);
  float getMasterEQResponse(double frequency) const;

  void setMasterGain(float gainDecibels) {
    masterGain = juce::Decibels::decibelsToGain(gainDecibels);
  }
  float getLeftLevel() const { return leftLevel; }
  float getRightLevel() const { return rightLevel; }
  float getInputLeftLevel() const { return inputLeftLevel; }
  float getInputRightLevel() const { return inputRightLevel; }

  void setGlobalOversamplingFactor(int factor);
  int getGlobalOversamplingFactor() const { return globalOversamplingFactor; }

  void setPhaseInvert(bool invert) { phaseInvert = invert; }
  bool isPhaseInverted() const { return phaseInvert; }

  void toggleABComparison();
  bool isComparisonStateB() const { return useComparisonStateB; }

  juce::ValueTree getCurrentState();
  void setCurrentState(const juce::ValueTree& state);

  void updateLatency();

  void setSpectralTilt(float tilt) { spectralTilt = tilt; }
  float getSpectralTilt() const { return spectralTilt; }

  void setHDMode(bool enabled) { hdEnhancementEnabled = enabled; }
  bool isHDModeEnabled() const { return hdEnhancementEnabled; }

  const float* getWaveform() const { return waveBuffer.getReadPointer(0); }
  int getWaveformSize() const { return waveBuffer.getNumSamples(); }

  void setStereoWidth(float width) { stereoWidth = width; }
  float getStereoWidth() const { return stereoWidth; }

private:
  void processEffect(BaseEffect* effect, juce::AudioBuffer<float>& buffer);
  void initRegistry();

  // Spectrum Analysis
  signalsmith::fft::RealFFT<float> forwardFFT{fftSize};
  juce::dsp::WindowingFunction<float> window{
      fftSize, juce::dsp::WindowingFunction<float>::hann};

  float fifo[fftSize];
  float fftData[fftSize * 2];
  float fftMagnitudes[fftSize / 2];
  int fifoIndex = 0;
  bool nextFFTBlockReady = false;

  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
                                 juce::dsp::IIR::Coefficients<float>>
      dcBlocker;

  // 6-Band Master EQ
  struct MasterEQ
      : public juce::dsp::ProcessorChain<
            juce::dsp::ProcessorDuplicator<
                juce::dsp::IIR::Filter<float>,
                juce::dsp::IIR::Coefficients<float>>, // 0: Low Shelf
            juce::dsp::ProcessorDuplicator<
                juce::dsp::IIR::Filter<float>,
                juce::dsp::IIR::Coefficients<float>>, // 1: Peaking
            juce::dsp::ProcessorDuplicator<
                juce::dsp::IIR::Filter<float>,
                juce::dsp::IIR::Coefficients<float>>, // 2: Peaking
            juce::dsp::ProcessorDuplicator<
                juce::dsp::IIR::Filter<float>,
                juce::dsp::IIR::Coefficients<float>>, // 3: Peaking
            juce::dsp::ProcessorDuplicator<
                juce::dsp::IIR::Filter<float>,
                juce::dsp::IIR::Coefficients<float>>, // 4: Peaking
            juce::dsp::ProcessorDuplicator<
                juce::dsp::IIR::Filter<float>,
                juce::dsp::IIR::Coefficients<float>> // 5: High Shelf
            > {
  } masterEQ;

private:
  EffectChain::Ptr activeChain;
  EffectChain::Ptr pendingChain;

  std::atomic<TransitionState> transitionState{TransitionState::Idle};
  float transitionProgress = 0.0f;
  float transitionDelta = 0.0f;
  juce::AudioBuffer<float> shadowBuffer;

  float masterGain = 1.0f;
  float leftLevel = 0, rightLevel = 0;
  float inputLeftLevel = 0, inputRightLevel = 0;

  bool isFirstRun = true;
  int globalOversamplingFactor = 1;
  OversamplingHandler oversamplingHandler;

  bool hdEnhancementEnabled = true;
  float hdSaturation = 0.0f;
  float stereoWidth = 1.0f;
  float spectralTilt = 0.0f;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> tiltFilterLow, tiltFilterHigh, airFilter;
  juce::AudioBuffer<float> waveBuffer;

  bool phaseInvert = false;
  bool useComparisonStateB = false;
  juce::ValueTree stateA, stateB;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BleuAudioProcessor)
};
