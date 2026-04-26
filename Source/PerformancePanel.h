/*
  ==============================================================================

    Source/PerformancePanel.h
    Created: 23 Apr 2026
    Description: 고성능 라이브 대시보드. 루프, 이펙터, 프리셋 제어 통합.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"
#include "LoopPad.h"
#include "PerformanceComponents.h"
#include "CustomLookAndFeel.h"

class PerformancePanel : public juce::Component
{
public:
    PerformancePanel(ThemeManager& tm) 
        : themeManager(tm),
          customLF(tm)
    {
        // 1. Loop Pads (Square)
        for (int i = 0; i < 4; ++i)
        {
            auto* pad = new LoopPad(i, themeManager);
            addAndMakeVisible(pad);
            loopPads.add(pad);
        }

        // 2. Stomp Switches (Circular)
        juce::StringArray fxNames = { 
            "Whammy", "Tube Drive", "Amp Sim", "Chorus", "Delay", "Power",
            "Octaver", "Harmonizer", "Auto-Wah", "Comp", "Filter", "EQ" 
        };
        for (int i = 0; i < 12; ++i)
        {
            auto* btn = new StompButton(fxNames[i], themeManager);
            btn->setClickingTogglesState(true);
            btn->onClick = [this, i, btn]() { handleStompToggle(i, btn->getToggleState()); };
            addAndMakeVisible(btn);
            fxButtons.add(btn);
        }

        // 3. Preset Buttons (Rectangular)
        juce::StringArray presetNames = { "Clean Twang", "Solo Lead", "Octave Chaos", "Ambient Swell" };
        for (int i = 0; i < 4; ++i)
        {
            auto* btn = new PresetButton(presetNames[i], themeManager);
            btn->onClick = [this, i]() { if (presetButtons[i]->getToggleState()) handlePresetChange(i); };
            addAndMakeVisible(btn);
            presetButtons.add(btn);
        }

        // 4. Wah/Whammy Pedal
        wahSlider.setSliderStyle(juce::Slider::LinearVertical);
        wahSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        wahSlider.setLookAndFeel(&customLF);
        wahSlider.onValueChange = [this]() { if (onWahSliderChanged) onWahSliderChanged((float)wahSlider.getValue()); };
        addAndMakeVisible(wahSlider);
    }

    ~PerformancePanel() override
    {
        wahSlider.setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat().reduced(10.0f);
        
        // 배경 유리판
        ThemeManager::drawGlassPanel(g, area, colors, 20.0f);
        
        g.setColour(colors.textHeader.withAlpha(0.3f));
        g.setFont(juce::Font(juce::FontOptions(12.0f).withStyle("Bold").withName("Outfit")));
        g.drawText("LIVE PERFORMANCE STATION", area.removeFromTop(35).reduced(25, 0), juce::Justification::centredLeft);
        
        // 영역 구분선 (은은하게)
        g.setColour(colors.textHeader.withAlpha(0.1f));
        float leftWidth = area.getWidth() * 0.75f;
        g.drawLine(leftWidth + 20, 50, leftWidth + 20, getHeight() - 50, 1.0f);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(20);
        area.removeFromTop(40); // 헤더 영역 제외

        // 1. 우측 페달 구역 (1/4 너비)
        auto rightArea = area.removeFromRight(area.getWidth() * 0.25f);
        wahSlider.setBounds(rightArea.reduced(15, 30));

        area.removeFromRight(20); // 간격

        // 2. 좌측 메인 구역 (수직 2:2:1 비율)
        int totalHeight = area.getHeight();
        int unitH = totalHeight / 5;

        // Top: Loop Pads (2/5)
        auto topArea = area.removeFromTop(unitH * 2);
        int padW = topArea.getWidth() / 4;
        for (int i = 0; i < 4; ++i)
            loopPads[i]->setBounds(topArea.removeFromLeft(padW).reduced(8));

        area.removeFromTop(10);

        // Middle: Stomp FX (2/5)
        auto midArea = area.removeFromTop(unitH * 2);
        int cols = 6;
        int rows = 2;
        int btnW = midArea.getWidth() / cols;
        int btnH = midArea.getHeight() / rows;
        for (int i = 0; i < 12; ++i)
        {
            int r = i / cols;
            int c = i % cols;
            fxButtons[i]->setBounds(midArea.getX() + c * btnW, midArea.getY() + r * btnH, btnW, btnH);
        }

        area.removeFromTop(10);

        // Bottom: Presets (1/5)
        auto botArea = area;
        int preW = botArea.getWidth() / 4;
        for (int i = 0; i < 4; ++i)
            presetButtons[i]->setBounds(botArea.removeFromLeft(preW).reduced(10, 15));
    }

    std::function<void(float)> onWahSliderChanged;
    std::function<void(int)> onPresetChanged;
    std::function<void(int, bool)> onStompToggle;

    void setStompState(int index, bool isOn)
    {
        if (index >= 0 && index < fxButtons.size())
            fxButtons[index]->setToggleState(isOn, juce::dontSendNotification);
    }

private:
    void handlePresetChange(int index)
    {
        if (onPresetChanged) onPresetChanged(index);
    }

    void handleStompToggle(int index, bool isOn)
    {
        if (onStompToggle) onStompToggle(index, isOn);
    }

    ThemeManager& themeManager;
    CustomLookAndFeel customLF;

    juce::OwnedArray<LoopPad> loopPads;
    juce::OwnedArray<StompButton> fxButtons;
    juce::OwnedArray<PresetButton> presetButtons;
    juce::Slider wahSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PerformancePanel)
};
