/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
BleuAudioProcessorEditor::BleuAudioProcessorEditor(BleuAudioProcessor &p)
    : AudioProcessorEditor(p), audioProcessor(p), navBar(themeManager), tooltipWindow(this, 700) {
  addAndMakeVisible(panelHolder);

  effectPanel = std::make_unique<EffectPanel>(audioProcessor, themeManager);
  ampPanel = std::make_unique<AmpPanel>(audioProcessor, themeManager);
  outputPanel = std::make_unique<OutputPanel>(audioProcessor, themeManager);
  performancePanel = std::make_unique<PerformancePanel>(themeManager);

  panelHolder.addAndMakeVisible(effectPanel.get());
  panelHolder.addAndMakeVisible(ampPanel.get());
  panelHolder.addAndMakeVisible(outputPanel.get());
  panelHolder.addAndMakeVisible(performancePanel.get());

  presetPanel = std::make_unique<PresetPanel>(audioProcessor, themeManager);
  addAndMakeVisible(presetPanel.get());

  addAndMakeVisible(navBar);
  
  addAndMakeVisible(navBar);
    
  navBar.onIndexChanged = [this](int index) { showPanel(index); };
  navBar.setCurrentIndex(0);

  // Initial Speed Optimization: Buffer panels for smoother sliding transitions
  effectPanel->setBufferedToImage(true);
  ampPanel->setBufferedToImage(true);
  outputPanel->setBufferedToImage(true);
  performancePanel->setBufferedToImage(true);

  performancePanel->onWahSliderChanged = [this](float value) {
    auto effects = audioProcessor.getActiveEffects();
    for (auto *effect : effects) {
      if (effect->getName() == "Whammy") {
        float pitch = juce::jmap(value, 0.0f, 1.0f, -12.0f, 12.0f);
        effect->setParameter("Pitch", pitch);
      } else if (effect->getName() == "Auto-Wah") {
        effect->setParameter("Sensitivity", value);
      }
    }
  };

  performancePanel->onStompToggle = [this](int index, bool isOn) {
    juce::StringArray fxNames = { 
        "Whammy", "Tube Drive", "Amp Sim", "Chorus", "Delay", "Power",
        "Octaver", "Harmonizer", "Auto-Wah", "Comp", "Filter", "EQ" 
    };
    if (index >= 0 && index < fxNames.size()) {
        auto name = fxNames[index];
        auto effects = audioProcessor.getActiveEffects();
        for (auto* effect : effects) {
            if (effect->getName() == name) {
                effect->setBypassed(!isOn);
                break;
            }
        }
    }
  };

  performancePanel->onPresetChanged = [this](int index) {
      auto effects = audioProcessor.getActiveEffects();
      if (index == 0) { // Clean Twang
          for (auto* e : effects) {
              if (e->getName() == "Amp Sim" || e->getName() == "Comp") e->setBypassed(false);
              else e->setBypassed(true);
          }
      } else if (index == 1) { // Solo Lead
          for (auto* e : effects) {
              if (e->getName() == "Tube Drive" || e->getName() == "Amp Sim" || e->getName() == "Delay") e->setBypassed(false);
              else e->setBypassed(true);
          }
      } else if (index == 2) { // Octave Chaos
          for (auto* e : effects) {
              if (e->getName() == "Octaver" || e->getName() == "Whammy" || e->getName() == "Tube Drive") e->setBypassed(false);
              else e->setBypassed(true);
          }
      } else if (index == 3) { // Ambient Swell
          for (auto* e : effects) {
              if (e->getName() == "Delay" || e->getName() == "Chorus" || e->getName() == "Auto-Wah") e->setBypassed(false);
              else e->setBypassed(true);
          }
      }
      
      for (int i = 0; i < 12; ++i) {
          juce::StringArray fxNames = { "Whammy", "Tube Drive", "Amp Sim", "Chorus", "Delay", "Power", "Octaver", "Harmonizer", "Auto-Wah", "Comp", "Filter", "EQ" };
          auto name = fxNames[i];
          for (auto* e : effects) {
              if (e->getName() == name) {
                  performancePanel->setStompState(i, !e->getBypassed());
                  break;
              }
          }
      }
  };

  setResizable(true, true);
  getConstrainer()->setFixedAspectRatio(1.5); 
  setResizeLimits(600, 400, 1500, 1000);
  
  setSize(1050, 700); // Increased default size (1.5 ratio)
  startTimerHz(30); // Update levels at 30Hz
}

BleuAudioProcessorEditor::~BleuAudioProcessorEditor() { }

//==============================================================================
void BleuAudioProcessorEditor::paint(juce::Graphics &g) {
  auto colors = themeManager.getColors();

  juce::ColourGradient gradient(colors.backgroundGradientStart, 0, 0,
                                colors.backgroundGradientEnd, 0,
                                (float)getHeight(), false);
  g.setGradientFill(gradient);
  g.fillAll();
}

void BleuAudioProcessorEditor::resized() {
  auto w = getWidth();
  
  // During resize, buffering to image can be slow. 
  // We disable it temporarily to keep resize snappy.
  effectPanel->setBufferedToImage(false);
  ampPanel->setBufferedToImage(false);
  outputPanel->setBufferedToImage(false);
  performancePanel->setBufferedToImage(false);

  auto area = getLocalBounds();
  auto headerArea = area.removeFromTop(80);
  navBar.setBounds(headerArea.withSizeKeepingCentre(400, 50));
  presetPanel->setBounds(headerArea.removeFromRight(250).reduced(10, 20));

  panelHolder.setBounds(0, 80, w * 4, area.getHeight());

  effectPanel->setBounds(0, 0, w, area.getHeight());
  ampPanel->setBounds(w, 0, w, area.getHeight());
  outputPanel->setBounds(w * 2, 0, w, area.getHeight());
  performancePanel->setBounds(w * 3, 0, w, area.getHeight());

  updatePanelPositions();
  
  // Re-enable buffering after layout is settled
  effectPanel->setBufferedToImage(true);
  ampPanel->setBufferedToImage(true);
  outputPanel->setBufferedToImage(true);
  performancePanel->setBufferedToImage(true);
}

void BleuAudioProcessorEditor::showPanel(int index) {
  currentPanelIndex = juce::jlimit(0, 3, index);
  float targetX = -(float)(currentPanelIndex * getWidth());
  float startX = currentX;

  if (panelAnimator) animatorUpdater.removeAnimator(*panelAnimator);

  panelAnimator = juce::ValueAnimatorBuilder()
      .withDurationMs(400)
      .withEasing(juce::Easings::createEaseOut())
      .withValueChangedCallback([this, startX, targetX](float t) {
          currentX = startX + (targetX - startX) * t;
          panelHolder.setTopLeftPosition((int)currentX, 80);
      })
      .build();

  animatorUpdater.addAnimator(*panelAnimator);
  panelAnimator->start();
}

void BleuAudioProcessorEditor::updatePanelPositions() {
  currentX = -(float)(currentPanelIndex * getWidth());
  panelHolder.setTopLeftPosition((int)currentX, 80);
}

void BleuAudioProcessorEditor::timerCallback() {
    if (outputPanel != nullptr) {
        double latencyMs = (audioProcessor.getLatencySamples() / audioProcessor.getSampleRate()) * 1000.0;
        outputPanel->setLevels(audioProcessor.getInputLeftLevel(),
                               audioProcessor.getInputRightLevel(),
                               audioProcessor.getLeftLevel(),
                               audioProcessor.getRightLevel(),
                               latencyMs,
                               0.0f);
    }
}
