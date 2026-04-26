/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ThemeManager.h"
#include "EffectPanel.h"
#include "AmpPanel.h"
#include "OutputPanel.h"
#include "PerformancePanel.h"
#include "NavigationBar.h"
#include "PresetPanel.h"

//==============================================================================
/**
*/
class BleuAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                 public juce::Timer
{
public:
    BleuAudioProcessorEditor (BleuAudioProcessor&);
    ~BleuAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    void updatePanelPositions();
    void showPanel(int index);

    BleuAudioProcessor& audioProcessor;
    ThemeManager themeManager;

    juce::Component panelHolder;
    std::unique_ptr<EffectPanel> effectPanel;
    std::unique_ptr<AmpPanel> ampPanel;
    std::unique_ptr<OutputPanel> outputPanel;
    std::unique_ptr<PerformancePanel> performancePanel;

    int currentPanelIndex = 0;
    float currentX = 0.0f;

    NavigationBar navBar;
    juce::TooltipWindow tooltipWindow;

    std::unique_ptr<PresetPanel> presetPanel;
    
    // VBlank Synchronization for ultra-smooth animations
    juce::VBlankAnimatorUpdater animatorUpdater { this };
    std::optional<juce::Animator> panelAnimator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BleuAudioProcessorEditor)
};
