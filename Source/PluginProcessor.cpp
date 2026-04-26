/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "Effects/AutoWahEffect.h"
#include "Effects/HarmonizerEffect.h"
#include "Effects/OctaverEffect.h"
#include "Effects/PowerAmpEffect.h"
#include "Effects/WhammyEffect.h"
#include "Effects/NoiseGateEffect.h"
#include "Effects/SpringReverbEffect.h"
#include "Effects/OverdriveEffect.h"
#include "Effects/PhaserEffect.h"
#include "Effects/FlangerEffect.h"
#include "Effects/TremoloEffect.h"
#include "Effects/UnivibeEffect.h"
#include "Effects/FuzzEffect.h"
#include "Effects/GraphicEQEffect.h"
#include "Effects/BitcrusherEffect.h"
#include "Effects/PlateReverbEffect.h"
#include "Effects/ShimmerReverbEffect.h"
#include "Effects/EnvelopeFilterEffect.h"
#include "Effects/SubHarmonicEffect.h"
#include "Effects/ExciterEffect.h"
#include "Effects/RotaryEffect.h"
#include "Effects/RingModEffect.h"
#include "Effects/LoFiEffect.h"
#include "Effects/SlicerEffect.h"
#include "Effects/AutoPanEffect.h"
#include "Effects/FormantFilterEffect.h"
#include "Effects/HarmonicEffect.h"
#include "Effects/TapeSaturatorEffect.h"
#include "Effects/TransientShaperEffect.h"
#include "Effects/WaveFolderEffect.h"
#include "Effects/SlowGearEffect.h"
#include "Effects/OctaveFuzzEffect.h"
#include "Effects/StereoEnhancerEffect.h"
#include "Effects/MasterLimiterEffect.h"
#include "Effects/ReverseDelayEffect.h"
#include "Effects/BitReductionEffect.h"
#include "Effects/CombFilterEffect.h"
#include "Effects/GlitchEffect.h"
#include "PluginEditor.h"
#include <cstring>

//==============================================================================
BleuAudioProcessor::BleuAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
{
  initRegistry();

  // Default chain for new users
  setupDefaultChain();
}

BleuAudioProcessor::~BleuAudioProcessor() {}

void BleuAudioProcessor::initRegistry() {
  REGISTER_EFFECT(DelayEffect, "Delay");
  REGISTER_EFFECT(AmpSimEffect, "Amp Sim");
  REGISTER_EFFECT(CompEffect, "Comp");
  REGISTER_EFFECT(TubeDriveEffect, "Tube Drive");
  REGISTER_EFFECT(ChorusEffect, "Chorus");
  REGISTER_EFFECT(PowerAmpEffect, "Power");
  REGISTER_EFFECT(WhammyEffect, "Whammy");
  REGISTER_EFFECT(OctaverEffect, "Octaver");
  REGISTER_EFFECT(HarmonizerEffect, "Harmonizer");
  REGISTER_EFFECT(AutoWahEffect, "Auto-Wah");
  REGISTER_EFFECT(NoiseGateEffect, "Noise Gate");
  REGISTER_EFFECT(SpringReverbEffect, "Spring Reverb");
  REGISTER_EFFECT(OverdriveEffect, "Overdrive");
  REGISTER_EFFECT(PhaserEffect, "Phaser");
  REGISTER_EFFECT(FlangerEffect, "Flanger");
  REGISTER_EFFECT(TremoloEffect, "Tremolo");
  REGISTER_EFFECT(UnivibeEffect, "Univibe");
  REGISTER_EFFECT(FuzzEffect, "70s Fuzz");
  REGISTER_EFFECT(GraphicEQEffect, "10-Band EQ");
  REGISTER_EFFECT(BitcrusherEffect, "Bitcrusher");
  REGISTER_EFFECT(PlateReverbEffect, "Plate Reverb");
  REGISTER_EFFECT(ShimmerReverbEffect, "Shimmer Reverb");
  REGISTER_EFFECT(EnvelopeFilterEffect, "Auto-Wah");
  REGISTER_EFFECT(SubHarmonicEffect, "Sub-Generator");
  REGISTER_EFFECT(ExciterEffect, "Exciter");
  REGISTER_EFFECT(RotaryEffect, "Rotary");
  REGISTER_EFFECT(RingModEffect, "Ring Mod");
  REGISTER_EFFECT(LoFiEffect, "Lo-Fi");
  REGISTER_EFFECT(SlicerEffect, "Slicer");
  REGISTER_EFFECT(AutoPanEffect, "Auto Pan");
  REGISTER_EFFECT(FormantFilterEffect, "Talkbox");
  REGISTER_EFFECT(HarmonicEffect, "Sub-Synth");
  REGISTER_EFFECT(TapeSaturatorEffect, "Tape Sat");
  REGISTER_EFFECT(TransientShaperEffect, "Trans Shape");
  REGISTER_EFFECT(WaveFolderEffect, "Wave Folder");
  REGISTER_EFFECT(SlowGearEffect, "Slow Gear");
  REGISTER_EFFECT(OctaveFuzzEffect, "Octave Fuzz");
  REGISTER_EFFECT(StereoEnhancerEffect, "Stereo Wide");
  REGISTER_EFFECT(MasterLimiterEffect, "Limiter");
  REGISTER_EFFECT(ReverseDelayEffect, "Reverse Delay");
  REGISTER_EFFECT(BitReductionEffect, "Bit Reducer");
  REGISTER_EFFECT(CombFilterEffect, "Comb Filter");
  REGISTER_EFFECT(GlitchEffect, "Glitcher");
}

//==============================================================================
const juce::String BleuAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool BleuAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool BleuAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool BleuAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double BleuAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int BleuAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int BleuAudioProcessor::getCurrentProgram() { return 0; }

void BleuAudioProcessor::setCurrentProgram(int index) {}

const juce::String BleuAudioProcessor::getProgramName(int index) { return {}; }

void BleuAudioProcessor::changeProgramName(int index,
                                           const juce::String &newName) {}

//==============================================================================
void BleuAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  juce::dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = samplesPerBlock;
  spec.numChannels = getTotalNumOutputChannels();

  if (activeChain) {
    for (auto *effect : activeChain->effects) {
      double sr = effect->getOversamplingEnabled() ? sampleRate * (double)globalOversamplingFactor : sampleRate;
      int bs = effect->getOversamplingEnabled() ? samplesPerBlock * globalOversamplingFactor : samplesPerBlock;
      effect->prepare(sr, bs);
    }
  }
  
  oversamplingHandler.setFactor(globalOversamplingFactor);
  oversamplingHandler.reset();

  // Pre-allocate shadowBuffer for safe transition
  shadowBuffer.setSize(getTotalNumOutputChannels(), samplesPerBlock);
  shadowBuffer.clear();

  dcBlocker.prepare(spec);
  *dcBlocker.state =
      *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 30.0f);

  masterEQ.prepare(spec);
  
  tiltFilterLow.prepare(spec);
  tiltFilterHigh.prepare(spec);
  airFilter.prepare(spec);
  *airFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 12000.0f, 0.707f, 1.25f); // 2dB air boost

  waveBuffer.setSize(1, 512);
  waveBuffer.clear();

  // Apply the EQ defaults now that we have a valid sample rate.
  // These are the same values as setupDefaultChain's sweet spot curve.
  updateMasterEQ(0, 100.0f,   -3.0f);  // Low Cut
  updateMasterEQ(1, 250.0f,   -1.5f);  // Clarity
  updateMasterEQ(2, 850.0f,    2.0f);  // Body
  updateMasterEQ(3, 2800.0f,   1.5f);  // Definition
  updateMasterEQ(4, 6500.0f,   1.0f);  // Air
  updateMasterEQ(5, 12000.0f, -6.0f);  // Tame Highs

  shadowBuffer.setSize(getTotalNumOutputChannels(), samplesPerBlock);
  shadowBuffer.clear();

  airFilter.prepare(spec);
  *airFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 12000.0f, 0.707f, 1.25f); // +2dB Air

  updateLatency();
}

void BleuAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BleuAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void BleuAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                      juce::MidiBuffer& midiMessages) {
  juce::ScopedNoDenormals noDenormals;

  const int numInCh   = getTotalNumInputChannels();
  const int numOutCh  = getTotalNumOutputChannels();
  const int numSamples = buffer.getNumSamples();

  // Clear pure output-only channels (e.g. stereo out from mono in)
  for (int i = numInCh; i < numOutCh; ++i)
    buffer.clear(i, 0, numSamples);

  inputLeftLevel = buffer.getMagnitude(0, 0, numSamples);
  inputRightLevel = (numInCh > 1) ? buffer.getMagnitude(1, 0, numSamples) : inputLeftLevel;

  // Snapshot chain pointers once — stable for this block
  EffectChain::Ptr currentChainPtr = activeChain;
  const auto state = transitionState.load();

  if (state == TransitionState::Idle || !pendingChain) {
    // ── Simple path: just run through the active chain ──────────────────────
    if (currentChainPtr) {
      for (auto* effect : currentChainPtr->effects) {
        if (!effect->getBypassed())
          effect->process(buffer);
      }
    }
  } else {
    // ── Transition path: Preparing or Crossover ──────────────────────────────
    EffectChain::Ptr pendingChainPtr = pendingChain;

    // Save DRY input first so both chains get the same source signal
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
      shadowBuffer.copyFrom(ch, 0, buffer.getReadPointer(ch), numSamples);

    // Process current chain on the main buffer
    if (currentChainPtr) {
      for (auto* effect : currentChainPtr->effects) {
        if (!effect->getBypassed())
          effect->process(buffer);
      }
    }

    // Process pending chain on the shadow buffer (pre-warm or crossfade source)
    if (pendingChainPtr) {
      for (auto* effect : pendingChainPtr->effects) {
        processEffect(effect, shadowBuffer);
      }
    }

    if (state == TransitionState::Crossover && pendingChainPtr) {
      // Crossfade sample-by-sample
      bool done = false;
      for (int s = 0; s < numSamples; ++s) {
        float gainNew = juce::jlimit(0.0f, 1.0f, transitionProgress);
        float gainOld = 1.0f - gainNew;
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
          buffer.setSample(ch, s,
                           buffer.getSample(ch, s)      * gainOld +
                           shadowBuffer.getSample(ch, s) * gainNew);
        transitionProgress += transitionDelta;
        if (transitionProgress >= 1.0f) done = true;
      }
      if (done) {
        activeChain  = pendingChainPtr;
        pendingChain = nullptr;
        transitionState.store(TransitionState::Idle);
        updateLatency();
      }
    }
    // In Preparing state: main buffer already has current chain output,
    // shadowBuffer is warmed — nothing else to do.
  }

  // ── DC Blocker & Master EQ ─────────────────────────────────────────────────
  {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);
    dcBlocker.process(ctx);
    masterEQ.process(ctx);
  }

  // ── Spectral Tilt ────────────────────────────────────────────────────────
  {
    float lowGain = juce::Decibels::decibelsToGain(-spectralTilt * 6.0f);
    float highGain = juce::Decibels::decibelsToGain(spectralTilt * 6.0f);
    
    *tiltFilterLow.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 200.0f, 0.707f, lowGain);
    *tiltFilterHigh.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), 3000.0f, 0.707f, highGain);

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);
    tiltFilterLow.process(ctx);
    tiltFilterHigh.process(ctx);
  }

  // ── FFT spectrum & Waveform feed ──────────────────────────────────────────
  if (const float* ch0 = buffer.getReadPointer(0)) {
    for (int i = 0; i < numSamples; ++i) {
      pushNextSampleIntoFifo(ch0[i]);
      if (i < waveBuffer.getNumSamples()) {
          waveBuffer.setSample(0, i, ch0[i]);
      }
    }
  }

  // ── Stereo Width (Mid-Side) ───────────────────────────────────────────────
  if (buffer.getNumChannels() >= 2 && std::abs(stereoWidth - 1.0f) > 0.001f) {
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int s = 0; s < numSamples; ++s) {
      float mid = (left[s] + right[s]) * 0.5f;
      float side = (left[s] - right[s]) * 0.5f;
      left[s] = mid + side * stereoWidth;
      right[s] = mid - side * stereoWidth;
    }
  }

  // ── Master Gain & level meters ─────────────────────────────────────────────
  buffer.applyGain(masterGain * (phaseInvert ? -1.0f : 1.0f));
  
  // ── HD Enhancement (Heart of Bleu) ─────────────────────────────────────────
  if (hdEnhancementEnabled) {
    // 1. Subtle Saturation (2nd Harmonics)
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
      float* data = buffer.getWritePointer(ch);
      for (int s = 0; s < numSamples; ++s) {
          float x = data[s];
          // Asymmetric soft clip for 2nd order harmonics
          data[s] = x > 0 ? std::tanh(x) : x * (1.0f + 0.1f * x); 
      }
    }
    
    // 2. Air Boost
    juce::dsp::AudioBlock<float> airBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> airCtx(airBlock);
    airFilter.process(airCtx);
    
    // 3. Subtle Stereo Width (Mid-Side)
    if (numOutCh >= 2) {
        for (int s = 0; s < numSamples; ++s) {
            float l = buffer.getSample(0, s);
            float r = buffer.getSample(1, s);
            float mid = (l + r) * 0.5f;
            float side = (l - r) * 0.5f;
            side *= 1.15f; // 15% width boost
            buffer.setSample(0, s, mid + side);
            buffer.setSample(1, s, mid - side);
        }
    }
  }

  // ── Soft Clipper (Safety) ────────────────────────────────────────────────
  for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
    float* data = buffer.getWritePointer(ch);
    for (int s = 0; s < numSamples; ++s) {
      // Soft saturation to prevent digital clipping
      float x = data[s];
      if (x > 1.0f) data[s] = 1.0f - std::exp(-x + 1.0f);
      else if (x < -1.0f) data[s] = -1.0f + std::exp(x + 1.0f);
      
      // Secondary tanh-like stage for extreme peaks
      data[s] = std::tanh(data[s]);
    }
  }

  leftLevel  = buffer.getMagnitude(0, 0, numSamples);
  rightLevel = (numInCh > 1) ? buffer.getMagnitude(1, 0, numSamples) : leftLevel;
}

void BleuAudioProcessor::updateLatency() {
    int totalLatency = 0;
    if (activeChain) {
        for (auto* e : activeChain->effects) {
            totalLatency += e->getLatencySamples();
        }
    }
    setLatencySamples(totalLatency);
}

void BleuAudioProcessor::setGlobalOversamplingFactor(int factor) {
    if (globalOversamplingFactor == factor) return;
    globalOversamplingFactor = factor;
    prepareToPlay(getSampleRate(), getBlockSize());
}

void BleuAudioProcessor::processEffect(BaseEffect* effect, juce::AudioBuffer<float>& buffer) {
  if (effect->getBypassed()) return;

  if (effect->getOversamplingEnabled()) {
    oversamplingHandler.process(buffer, [&](juce::AudioBuffer<float>& overBuffer) {
      effect->process(overBuffer);
    });
  } else {
    effect->process(buffer);
  }
}

//==============================================================================
void BleuAudioProcessor::addEffect(const juce::String &effectName, int index,
                                   bool autoCommit) {
  if (activeChain && activeChain->effects.size() >= 12)
    return;

  auto nextChain = new EffectChain();
  if (transitionState.load() != TransitionState::Idle && pendingChain)
    nextChain->effects = pendingChain->effects;
  else if (activeChain)
    nextChain->effects = activeChain->effects;

  if (auto effect = EffectRegistry::getInstance().createEffect(effectName)) {
    if (getSampleRate() > 0)
      effect->prepare(getSampleRate(), getBlockSize());

    if (index < 0 || index >= nextChain->effects.size())
      nextChain->effects.add(effect);
    else
      nextChain->effects.insert(index, effect);

    prepareShadowChain(nextChain);
    if (autoCommit)
      commitTransition(50.0f);
  }
}

void BleuAudioProcessor::removeEffect(int index, bool autoCommit) {
  if (activeChain && index >= 0 && index < activeChain->effects.size()) {
    auto nextChain = new EffectChain();
    if (transitionState.load() != TransitionState::Idle && pendingChain)
      nextChain->effects = pendingChain->effects;
    else
      nextChain->effects = activeChain->effects;

    nextChain->effects.remove(index);
    prepareShadowChain(nextChain);
    if (autoCommit)
      commitTransition(50.0f);
  }
}

void BleuAudioProcessor::moveEffect(int oldIndex, int newIndex,
                                    bool autoCommit) {
  if (activeChain && oldIndex >= 0 && oldIndex < activeChain->effects.size() &&
      newIndex >= 0 && newIndex < activeChain->effects.size()) {
    auto nextChain = new EffectChain();
    if (transitionState.load() != TransitionState::Idle && pendingChain)
      nextChain->effects = pendingChain->effects;
    else
      nextChain->effects = activeChain->effects;

    nextChain->effects.move(oldIndex, newIndex);
    prepareShadowChain(nextChain);
    if (autoCommit)
      commitTransition(50.0f);
  }
}

void BleuAudioProcessor::prepareShadowChain(
    const juce::ReferenceCountedObjectPtr<EffectChain> &nextChain) {
  // If we are already transitioning, snap to the target first to avoid stacking
  if (transitionState.load() == TransitionState::Crossover) {
    activeChain = pendingChain;
  }

  pendingChain = nextChain;
  transitionState.store(TransitionState::Preparing);
}

void BleuAudioProcessor::commitTransition(float durationMs) {
  if (transitionState.load() == TransitionState::Preparing) {
    transitionProgress = 0.0f;
    double sr = getSampleRate();
    if (sr <= 0)
      sr = 44100.0; // Fallback to avoid division by zero

    transitionDelta = 1.0f / (float)(sr * (durationMs / 1000.0f));

    if (std::isinf(transitionDelta) || std::isnan(transitionDelta) ||
        transitionDelta <= 0.0f)
      transitionDelta = 1.0f; // Instant transition if values are invalid

    transitionState.store(TransitionState::Crossover);
  }
}

void BleuAudioProcessor::cancelTransition() {
  transitionState.store(TransitionState::Idle);
  pendingChain = nullptr;
}

void BleuAudioProcessor::updateMasterEQ(int bandIdx, float frequency,
                                        float gainDb, float q) {
  auto sr = getSampleRate();
  if (sr <= 0)
    sr = 44100.0;

  // Clamp frequency to safe range for current sample rate
  frequency = juce::jlimit(20.0f, (float)sr * 0.45f, frequency);

  auto coeffs =
      (bandIdx == 0)
          ? juce::dsp::IIR::Coefficients<float>::makeLowShelf(
                sr, frequency, q, juce::Decibels::decibelsToGain(gainDb))
      : (bandIdx == 5)
          ? juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                sr, frequency, q, juce::Decibels::decibelsToGain(gainDb))
          : juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                sr, frequency, q, juce::Decibels::decibelsToGain(gainDb));

  if (bandIdx == 0)
    *masterEQ.get<0>().state = *coeffs;
  else if (bandIdx == 1)
    *masterEQ.get<1>().state = *coeffs;
  else if (bandIdx == 2)
    *masterEQ.get<2>().state = *coeffs;
  else if (bandIdx == 3)
    *masterEQ.get<3>().state = *coeffs;
  else if (bandIdx == 4)
    *masterEQ.get<4>().state = *coeffs;
  else if (bandIdx == 5)
    *masterEQ.get<5>().state = *coeffs;
}

float BleuAudioProcessor::getMasterEQResponse(double frequency) const {
  double combinedMag = 1.0;
  combinedMag *= masterEQ.get<0>().state->getMagnitudeForFrequency(
      frequency, getSampleRate());
  combinedMag *= masterEQ.get<1>().state->getMagnitudeForFrequency(
      frequency, getSampleRate());
  combinedMag *= masterEQ.get<2>().state->getMagnitudeForFrequency(
      frequency, getSampleRate());
  combinedMag *= masterEQ.get<3>().state->getMagnitudeForFrequency(
      frequency, getSampleRate());
  combinedMag *= masterEQ.get<4>().state->getMagnitudeForFrequency(
      frequency, getSampleRate());
  combinedMag *= masterEQ.get<5>().state->getMagnitudeForFrequency(
      frequency, getSampleRate());
  return (float)combinedMag;
}

//==============================================================================
void BleuAudioProcessor::pushNextSampleIntoFifo(float sample) noexcept {
  if (fifoIndex == fftSize) {
    if (!nextFFTBlockReady) {
      std::memset(fftData, 0, sizeof(fftData));
      std::memcpy(fftData, fifo, sizeof(fifo));
      nextFFTBlockReady = true;
    }

    fifoIndex = 0;
  }

  fifo[fifoIndex++] = sample;
}

void BleuAudioProcessor::getNextFFTData(float *outputData) noexcept {
  window.multiplyWithWindowingTable(fftData, fftSize);

  std::vector<std::complex<float>> complexOutput(fftSize / 2 + 1);
  forwardFFT.fft(fftData, complexOutput.data());

  for (int i = 0; i < fftSize / 2; ++i) {
    float mag = std::abs(complexOutput[i]) / (float)fftSize;
    fftMagnitudes[i] = mag; // Store for shared use
    if (outputData != nullptr)
      outputData[i] = mag;
  }
}

void BleuAudioProcessor::setupDefaultChain() {
  auto chain = new EffectChain();

  // 1. Comp (Smooth Consistency)
  if (auto comp = EffectRegistry::getInstance().createEffect("Comp")) {
    comp->setParameter("Threshold", -24.0f);
    comp->setParameter("Ratio", 3.5f);
    comp->setParameter("Attack", 10.0f);
    comp->setParameter("Release", 150.0f);
    chain->effects.add(comp);
  }

  // 2. Tube Drive (Warm Crunch)
  if (auto drive = EffectRegistry::getInstance().createEffect("Tube Drive")) {
    drive->setParameter("Drive", 0.35f);
    drive->setParameter("Tone", 0.6f);
    drive->setParameter("Output", 0.0f);
    chain->effects.add(drive);
  }

  // 3. Amp Sim (NAM - Punchy Tone)
  if (auto amp = EffectRegistry::getInstance().createEffect("Amp Sim")) {
    amp->setParameter("Input Gain", 0.0f);
    amp->setParameter("Output Gain", 3.0f);
    amp->setParameter("Bass", 0.4f);
    amp->setParameter("Mid", 0.6f);
    amp->setParameter("Treble", 0.55f);
    amp->setParameter("Presence", 0.5f);
    chain->effects.add(amp);
  }

  // 4. Chorus (Subtle Width)
  if (auto chorus = EffectRegistry::getInstance().createEffect("Chorus")) {
    chorus->setParameter("Rate", 0.8f);
    chorus->setParameter("Depth", 0.25f);
    chorus->setParameter("Mix", 0.2f);
    chain->effects.add(chorus);
  }

  // 5. Delay (Ambient Space)
  if (auto delay = EffectRegistry::getInstance().createEffect("Delay")) {
    delay->setParameter("Time", 450.0f);
    delay->setParameter("Feedback", 0.25f);
    delay->setParameter("Mix", 0.15f);
    chain->effects.add(delay);
  }

  activeChain = chain;
  // Note: Master EQ is initialized in prepareToPlay() with a valid sample rate.
}

//==============================================================================
bool BleuAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *BleuAudioProcessor::createEditor() {
  return new BleuAudioProcessorEditor(*this);
}

//==============================================================================
void BleuAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
  auto state = getCurrentState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void BleuAudioProcessor::setStateInformation(const void *data, int sizeInBytes) {
  std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
  if (xmlState != nullptr) {
    setCurrentState(juce::ValueTree::fromXml(*xmlState));
  }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new BleuAudioProcessor();
}

#include "RubberBandSingle_Include.cpp"

void BleuAudioProcessor::toggleABComparison() {
  auto currentState = getCurrentState();
  if (useComparisonStateB) {
    stateB = currentState;
    if (stateA.isValid()) setCurrentState(stateA);
  } else {
    stateA = currentState;
    if (stateB.isValid()) setCurrentState(stateB);
  }
  useComparisonStateB = !useComparisonStateB;
}

juce::ValueTree BleuAudioProcessor::getCurrentState() {
  juce::ValueTree state("BleuPluginState");
  state.setProperty("version", 1, nullptr);
  juce::ValueTree chain("EffectChain");
  for (auto *effect : getTargetEffects()) {
    chain.addChild(effect->getState(), -1, nullptr);
  }
  state.addChild(chain, -1, nullptr);
  state.setProperty("masterGain", masterGain, nullptr);
  state.setProperty("oversamplingFactor", globalOversamplingFactor, nullptr);
  state.setProperty("hdMode", hdEnhancementEnabled, nullptr);
  state.setProperty("phaseInvert", phaseInvert, nullptr);
  state.setProperty("stereoWidth", stereoWidth, nullptr);
  return state;
}

void BleuAudioProcessor::setCurrentState(const juce::ValueTree& state) {
  if (state.isValid()) {
    auto chain = state.getChildWithName("EffectChain");
    if (chain.isValid()) {
        auto nextChain = new EffectChain();
        for (int i = 0; i < chain.getNumChildren(); ++i) {
            auto child = chain.getChild(i);
            if (auto effect = EffectRegistry::getInstance().createEffect(child.getType().toString())) {
                effect->setState(child);
                if (getSampleRate() > 0) effect->prepare(getSampleRate(), getBlockSize());
                nextChain->effects.add(effect);
            }
        }
        prepareShadowChain(nextChain);
        commitTransition(50.0f);
    }
    masterGain = (float)state.getProperty("masterGain", 0.0f);
    globalOversamplingFactor = (int)state.getProperty("oversamplingFactor", 1);
    hdEnhancementEnabled = (bool)state.getProperty("hdMode", true);
    phaseInvert = (bool)state.getProperty("phaseInvert", false);
    stereoWidth = (float)state.getProperty("stereoWidth", 1.0f);
  }
}
